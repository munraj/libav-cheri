#include <stdio.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#define	TRY(x) do {							\
	int rc;								\
									\
	rc = (x);							\
	if (rc != 0) {							\
		fprintf(stderr, "FAILED: " #x " (rc=%d)\n", rc);	\
		exit(1);						\
	}								\
} while (0)

#define	TRYP(x) do {							\
	if ((x) == NULL) {						\
		fprintf(stderr, "FAILED: " #x "\n");			\
		exit(1);						\
	}								\
} while (0)

int
main(void)
{
	AVFormatContext *fctx;
	AVCodecContext *cctx, *cctx2;
	AVCodec *cod;
	AVFrame *fra;
	const char *filename;
	uint8_t *buf;
	size_t i, v, sz;
	int rc;

	av_register_all();

	filename = "../trailer.mp4";
	fctx = NULL;
	TRY(avformat_open_input(&fctx, filename, NULL, NULL));
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
	TRY(avcodec_open2(cctx2, cod));
	TRYP(fra = av_frame_alloc());
	sz = avpicture_get_size(PIX_FMT_RGB24, cctx2->width,
	    cctx2->height);
	TRYP(buf = av_malloc(sz));
	avpicture_fill((AVPicture *)fra, buf, PIX_FMT_RGB24, cctx2->width,
	    cctx2->height);

	/* Get frame. */

	return (0);
}
