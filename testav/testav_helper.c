#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>

#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>

#include <machine/cheri.h>

int	__fcntl_int();
int	__fcntl_ptr();

u_quad_t
__fixunssfdi(float f);

/* XXX: Hack. */
u_quad_t
__fixunssfsi(float f)
{

	return (__fixunssfdi(f));
}

char *
getenv(const char *name)
{

	(void)name;
	return (NULL);
}

int
sysctl(const int *name, u_int namelen, void *oldp, size_t oldlenp,
    const void *newp, size_t newlen)
{

	(void)name;
	(void)namelen;
	(void)oldp;
	(void)oldlenp;
	(void)newp;
	(void)newlen;
	errno = ENOSYS;
	return (-1);
}

int
fcntl(int fd, int cmd, ...)
{

	switch (cmd) {
	case F_DUPFD:
	case F_DUPFD_CLOEXEC:
	case F_DUP2FD:
	case F_DUP2FD_CLOEXEC:
	case F_GETFD:
	case F_SETFD:
	case F_GETFL:
	case F_SETFL:
	case F_GETOWN:
	case F_SETOWN:
	case F_READAHEAD:
	case F_RDAHEAD:
		return (__fcntl_int());
	case F_GETLK:
	case F_SETLK:
	case F_SETLKW:
		return (__fcntl_ptr());
	default:
		errno = ENOSYS;
		return (-1);
	}

}

int
ioctl(int fd, unsigned long request, ...)
{

	(void)fd;
	(void)request;
	errno = ENOSYS;
	return (-1);
}

int
sigemptyset(sigset_t *set)
{

	(void)set;
	errno = ENOSYS;
	return (-1);
}

double
ldexp(double x, int exp)
{
	double m;

	m = 1 << exp;
	return (x * m);
}

double
atof(const char *nptr)
{

	return (strtod(nptr, NULL));
}

double
fabs(double x)
{

	return ((x < 0) ? -x : x);
}

/* XXX: Naive implementation. */
void *
bsearch(const void *key, const void *base, size_t nmemb, size_t size,
    int (*compar)(const void *, const void *))
{
	const char *p;

	for (p = base; nmemb != 0; p += size, nmemb--)
		if ((*compar)(key, p) == 0)
			return ((void *)p);

	return (NULL);
}

int
isatty(int fd)
{

	(void)fd;
	errno = ENOSYS;
	return (-1);
}

int
getaddrinfo(const char *hostname, const char *servname,
    const struct addrinfo *hints, struct addrinfo **res)
{

	(void)hostname;
	(void)servname;
	(void)hints;
	(void)res;
	errno = ENOSYS;
	return (EAI_SYSTEM);
}

int
getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host,
    size_t hostlen, char *serv, size_t servlen, int flags)
{

	(void)sa;
	(void)salen;
	(void)host;
	(void)hostlen;
	(void)serv;
	(void)servlen;
	(void)flags;
	errno = ENOSYS;
	return (EAI_SYSTEM);
}

void
freeaddrinfo(struct addrinfo *ai)
{

	(void)ai;
}

int
gethostname(char *name, size_t namelen)
{

	(void)name;
	(void)namelen;
	errno = ENOSYS;
	return (-1);
}

int
sethostname(const char *name, int namelen)
{

	(void)name;
	(void)namelen;
	errno = ENOSYS;
	return (-1);
}

const char *
gai_strerror(int ecode)
{

	(void)ecode;
	return (NULL);
}

/* XXX: Unfreeable! */
int
posix_memalign(void **ptr, size_t alignment, size_t size)
{
	char *p;

	p = malloc(size + alignment - 1);
	if (p == NULL)
		return (-1);

	/* XXX: Avoiding bitwise &. */
	if ((uintptr_t)p % alignment != 0)
		p += alignment - (uintptr_t)p % alignment;
	*ptr = p;
	return (0);
}

clock_t
clock(void)
{
	int rc;
	struct rusage rusage;
	clock_t u, s;
	

	rc = getrusage(RUSAGE_SELF, &rusage);
	if (rc != 0)
		return (-1);
	u = rusage.ru_utime.tv_sec * CLOCKS_PER_SEC +
	    rusage.ru_utime.tv_usec * CLOCKS_PER_SEC / 1000000;
	s = rusage.ru_stime.tv_sec * CLOCKS_PER_SEC +
	    rusage.ru_stime.tv_usec * CLOCKS_PER_SEC / 1000000;
	return (u + s);
}

int
usleep(useconds_t microseconds)
{
	struct timespec rqt;

	rqt.tv_sec = microseconds / 1000000;
	rqt.tv_nsec = (microseconds % 1000000) * 1000;
	return (nanosleep(&rqt, NULL));
}

int
invoke(void)
{

	return (1234);
}
