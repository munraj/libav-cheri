.include "../config.mak"
#CC?=		cc
LD=		$(CC)
RM=		rm
SANDBOX_ABI=	yes

.ifndef CHERIBSD
.error "Need to define CHERIBSD."
.endif

PROG=		testav

AVLIBS=		../libavformat/libavformat.a				\
		../libavcodec/libavcodec.a				\
		../libavdevice/libavdevice.a				\
		../libavfilter/libavfilter.a				\
		../libavresample/libavresample.a			\
		../libswscale/libswscale.a				\
		../libavutil/libavutil.a
AVDEPS=		-static -lm

.ifdef SANDBOX_ABI
CFLAGS=		-I.. -DSANDBOX_ABI					\
		-DCHERI_CALL="__attribute__((cheri_ccall))		\
		    __attribute__((cheri_method_suffix(\"_cap\")))	\
		    __attribute__((cheri_method_class(cheri_libav)))"
LDADD=		-static -lcheri
LDADD=		-static -lcheri

HELPER=		testav_helper
HELPER_OBJS=	fixunsdfdi.help.o					\
		fixunssfdi.help.o					\
		isinf.help.o						\
		testav_helper.help.o					\
		syscalls.o
HELPER_LDFLAGS=	-Wl,--script=$(CHERIBSD)/lib/libc_cheri/sandbox.ld	\
		-nostdlib -Wl,--whole-archive
HELPER_CFLAGS=	-I.. -mabi=sandbox					\
		-DCHERI_CALL="__attribute__((cheri_ccallee))	\
		    __attribute__((cheri_method_suffix(\"_cap\")))	\
		    __attribute__((cheri_method_class(cheri_libav)))"	\
		-Wno-shift-count-overflow
HELPER_LDADD=	$(AVLIBS) -Wl,--no-whole-archive $(AVDEPS) -lc_cheri
.else # !SANDBOX_ABI
LDADD=		$(AVLIBS) $(AVDEPS)
.endif # SANDBOX_ABI

.PHONY: all clean
.SUFFIXES: .S .c .o .help.o

all: $(PROG)

clean:
	$(RM) -f $(OBJS) $(HELPER_OBJS) $(HELPER) $(PROG)

.c.help.o:
	$(CC) $(HELPER_CFLAGS) -o $@ -c $<

.S.help.o:
	$(CC) $(HELPER_CFLAGS) -o $@ -c $<

.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

$(PROG): $(OBJS) $(HELPER)
	$(LD) $(LDFLAGS) -o $(PROG) $(OBJS) $(LDADD)

$(HELPER): $(HELPER_OBJS)
	$(LD) $(HELPER_LDFLAGS) -o $(HELPER) $(HELPER_OBJS) $(HELPER_LDADD)
