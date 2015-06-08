PROG=		testav
OBJS=		testav.o
CC=		cc
LD=		$(CC)
CFLAGS=		-I..
LDFLAGS=
LDADD=		../libavformat/libavformat.a				\
		../libavcodec/libavcodec.a				\
		../libavdevice/libavdevice.a				\
		../libavfilter/libavfilter.a				\
		../libavresample/libavresample.a			\
		../libswscale/libswscale.a				\
		../libavutil/libavutil.a				\
		-lm -lpthread -lz -lbz2
RM=		rm

.PHONY: all clean

all: $(PROG)

clean:
	$(RM) -f $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

$(PROG): $(OBJS)
	$(LD) $(LDFLAGS) -o $(PROG) $(OBJS) $(LDADD)
