#include <stdio.h>
#include <string.h>

#ifdef SANDBOX_ABI
#include <machine/cheri.h>
#include <cheri/sandbox.h>

struct cheri_object cheri_libav;
struct sandbox_class *sb_cp;
struct sandbox_object *sb_op;
#endif

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

#define	TRY(x) do {							\
	int rc;								\
									\
	printf("Trying " #x "\n");					\
	rc = (x);							\
	if (rc != 0) {							\
		fprintf(stderr, "FAILED: " #x " (rc=%d)\n", rc);	\
		return (1);						\
	}								\
	printf("Done " #x "\n");					\
} while (0)

#define	TRYP(x) do {							\
	printf("Trying " #x "\n");					\
	if ((x) == NULL) {						\
		fprintf(stderr, "FAILED: " #x "\n");			\
		return (1);						\
	}								\
	printf("Done " #x "\n");					\
} while (0)

int	fwritebmp(const uint8_t *buf, size_t w, size_t h, size_t l,
	    FILE *fp);
void	sb_init(void);

int
main(void)
{
	AVFormatContext *fctx;
	AVCodecContext *cctx, *cctx2;
	AVCodec *cod;
	AVFrame *fra, *swfra;
	AVPacket pkt;
	struct SwsContext *sctx;
	FILE *ofp;
	const char *infile, *outfile;
	uint8_t *buf;
	size_t i, v, sz;
	int fdone, rc;

	sb_init();

	av_register_all();

	infile = "../trailer.mp4";
	outfile = "frame.bmp";
	fctx = NULL;
	TRYP(ofp = fopen(outfile, "wb"));
	TRY(avformat_open_input(&fctx, infile, NULL, NULL));
	TRY(avformat_find_stream_info(fctx, NULL));

	/* Get first video stream. */
	v = -1;
	for (i = 0; i < fctx->nb_streams; i++) {
		cctx = fctx->streams[i]->codec;
		printf("stream %zu: ", i);
		if (cctx->codec_type == AVMEDIA_TYPE_VIDEO) {
			if (v == -1)
				v = i;
		} else
			printf("not ");
		printf("video\n");
	}
	if (v == -1) {
		fprintf(stderr, "no video streams\n");
		exit(1);
	}

	/* Copy context. */
	cctx = fctx->streams[v]->codec;
	TRYP(cod = avcodec_find_decoder(cctx->codec_id));
	TRYP(cctx2 = avcodec_alloc_context3(cod));
	TRY(avcodec_copy_context(cctx2, cctx));

	/* Open codec, allocate frame. */
	TRY(avcodec_open2(cctx2, cod, NULL));
	TRYP(fra = av_frame_alloc());
	TRYP(swfra = av_frame_alloc());
	sz = avpicture_get_size(PIX_FMT_RGB24, cctx2->width,
	    cctx2->height);
	TRYP(buf = av_malloc(sz));
	(void)avpicture_fill((AVPicture *)swfra, buf, PIX_FMT_RGB24,
	    cctx2->width, cctx2->height);

	/* Initialize software scaling. */
	TRYP(sctx = sws_getContext(cctx2->width, cctx2->height,
	    cctx2->pix_fmt, cctx2->width, cctx2->height, PIX_FMT_RGB24,
	    SWS_BILINEAR, NULL, NULL, NULL));

	/* Get one frame. */
	fdone = 0;
	for (;;) {
		rc = av_read_frame(fctx, &pkt);
		if (rc != 0) {
			fprintf(stderr, "av_read_frame failed\n");
			break;
		}

		if (pkt.stream_index != v) {
			av_free_packet(&pkt);
			continue;
		}

		rc = avcodec_decode_video2(cctx2, fra, &fdone, &pkt);
		av_free_packet(&pkt);
		if (rc <= 0) {
			fprintf(stderr, "avcodec_decode_video2 failed "
			    "(rc=%d)\n", rc);
			break;
		}

		if (fdone) {
			(void)(sws_scale(sctx, (void *)fra->data,
			    fra->linesize, 0, cctx2->height, swfra->data,
			    swfra->linesize));
			TRY(fwritebmp((void *)swfra->data[0], cctx2->width,
			    cctx2->height, swfra->linesize[0], ofp));
			break;
		}
	}

	return (0);
}

#define	BMP_HDR_SZ	14
#define	DIB_HDR_SZ	40
#define	LEST32(buf, n) do {						\
	(buf)[0] = (uint32_t)(n) & 0xFF;				\
	(buf)[1] = ((uint32_t)(n) >> 8) & 0xFF;				\
	(buf)[2] = ((uint32_t)(n) >> 16) & 0xFF;			\
	(buf)[3] = ((uint32_t)(n) >> 24) & 0xFF;			\
} while (0)
#define	LEST16(buf, n) do {						\
	(buf)[0] = (uint16_t)(n) & 0xFF;				\
	(buf)[1] = ((uint16_t)(n) >> 8) & 0xFF;				\
} while (0)
int
fwritebmp(const uint8_t *buf, size_t w, size_t h, size_t l, FILE *fp)
{
	uint8_t bmp_hdr[BMP_HDR_SZ], dib_hdr[DIB_HDR_SZ], zero[4];
	size_t rc, rowsz, rownpad, i;

	rownpad = (w % 4 != 0) ? 4 - (w % 4) : 0;
	rowsz = w * 3 + rownpad;

	memset(zero, 0, sizeof(zero));

	memset(&bmp_hdr, 0, BMP_HDR_SZ);
	bmp_hdr[0] = 0x42;
	bmp_hdr[1] = 0x4D;
	LEST32(&bmp_hdr[2], rowsz * h + BMP_HDR_SZ + DIB_HDR_SZ);
	LEST16(&dib_hdr[6], 0);		/* reserved */
	LEST16(&dib_hdr[8], 0);		/* reserved */
	LEST32(&bmp_hdr[10], BMP_HDR_SZ + DIB_HDR_SZ);

	memset(&dib_hdr, 0, DIB_HDR_SZ);
	LEST32(&dib_hdr[0], DIB_HDR_SZ);
	LEST32(&dib_hdr[4], w);
	LEST32(&dib_hdr[8], h);
	LEST16(&dib_hdr[12], 1);	/* color planes */
	LEST16(&dib_hdr[14], 24);	/* bits per pixel */
	LEST32(&dib_hdr[16], 0);	/* compression method */
	LEST32(&dib_hdr[20], 0);	/* uncompressed bitmap size */
	LEST32(&dib_hdr[24], w);	/* horizontal resolution (pix/m) */
	LEST32(&dib_hdr[28], h);	/* vertical resolution (pix/m) */
	LEST32(&dib_hdr[32], 0);	/* no. of colors in palette */
	LEST32(&dib_hdr[36], 0);	/* no. of important colors used */

	TRY(fwrite(bmp_hdr, BMP_HDR_SZ, 1, fp) - 1);
	TRY(fwrite(dib_hdr, DIB_HDR_SZ, 1, fp) - 1);

	for (i = h; i != 0; i--) {
		TRY(fwrite(&buf[(i - 1) * l], 3 * w, 1, fp) - 1);
		if (rownpad != 0)
			TRY(fwrite(zero, rownpad, 1, fp) - 1);
	}

	return (0);
}

void
sb_init(void)
{
#ifdef SANDBOX_ABI
	int rc;

	rc = sandbox_class_new("/tmp2/testav_helper", 8 * 1024 * 1024,
	    &sb_cp, 1);
	if (rc != 0) {
		fprintf(stderr, "sandbox_class_new failed\n");
		exit(1);
	}

	rc = sandbox_object_new(sb_cp, 1 * 1024 * 1024, &sb_op);
	if (rc != 0) {
		fprintf(stderr, "sandbox_object_new failed\n");
		exit(1);
	}

	cheri_libav = sandbox_object_getobject(sb_op);
#endif
}
