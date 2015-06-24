/*
 * XXX: For use with modified sandbox environment that has the SYSCALL perm
 * bit set in PCC.
 */

#include <sys/syscall.h>

#define	MOVE		move
#define	LI		li
#define	SYSCALL		syscall
#define	CGETBASE	cgetbase
#define	CTOPTR		ctoptr $v0, $c3, $c0
#define	CA0		$c3
#define	CA1		$c4
#define	CA2		$c5
#define	CA3		$c6

.global write
write:
	MOVE $a3, $a1		/* size_t nbytes */
	CGETBASE $a2, CA0	/* const void *buf */
	MOVE $a1, $a0		/* int fd */
	LI $a0, SYS_write
	SYSCALL

.global read
read:
	MOVE $a3, $a1		/* size_t nbytes */
	CGETBASE $a2, CA0	/* void *buf */
	MOVE $a1, $a0		/* int fd */
	LI $a0, SYS_read
	SYSCALL

.global close
close:
	MOVE $a1, $a0		/* int fd */
	LI $a0, SYS_close
	SYSCALL

.global open
open:
	MOVE $a3, $a1		/* mode_t mode */
	MOVE $a2, $a0		/* int flags */
	CGETBASE $a1, CA0	/* const char *path */
	LI $a0, SYS_open
	SYSCALL

.global rename
rename:
	CGETBASE $a2, CA1		/* const char *to */
	CGETBASE $a1, CA0		/* const char *from */
	LI $a0, SYS_rename
	SYSCALL

.global unlink
unlink:
	CGETBASE $a1, CA0		/* const char *path */
	LI $a0, SYS_unlink
	SYSCALL

.global fstat
fstat:
	CGETBASE $a2, CA0	/* struct stat *sb */
	MOVE $a1, $a0		/* int fd */
	LI $a0, SYS_fstat
	SYSCALL

.global stat
stat:
	CGETBASE $a2, CA1	/* struct stat *sb */
	CGETBASE $a1, CA0	/* const char *path */
	LI $a0, SYS_stat
	SYSCALL

.global lseek
lseek:
	MOVE $a3, $a2		/* int whence */
	MOVE $a2, $a1		/* off_t offset */
	MOVE $a1, $a0		/* int filedes */
	LI $a0, SYS_lseek
	SYSCALL

.global mkdir
mkdir:
	MOVE $a2, $a0		/* mode_t mode */
	CGETBASE $a1, CA0	/* const char *path */
	LI $a0, SYS_mkdir
	SYSCALL

.global rmdir
rmdir:
	CGETBASE $a1, CA0	/* const char *path */
	LI $a0, SYS_rmdir
	SYSCALL

.global poll
poll:
	MOVE $a3, $a2		/* int timeout */
	MOVE $a2, $a1		/* nfds_t nfds */
	CGETBASE $a1, CA0	/* struct pollfd fds[] */
	LI $a0, SYS_poll
	SYSCALL

.global socket
socket:
	MOVE $a3, $a2		/* int protocol */
	MOVE $a2, $a1		/* int type */
	MOVE $a1, $a0		/* int domain */
	LI $a0, SYS_socket
	SYSCALL

.global setsockopt
setsockopt:
	MOVE $a5, $a3		/* socklen_t optlen */
	CGETBASE $a4, CA0	/* const void *optval */
	MOVE $a3, $a2		/* int optname */
	MOVE $a2, $a1		/* int level */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_setsockopt
	SYSCALL

.global getsockopt
getsockopt:
	CGETBASE $a5, CA1	/* socklen_t *optlen */
	CGETBASE $a4, CA0	/* void *optval */
	MOVE $a3, $a2		/* int optname */
	MOVE $a2, $a1		/* int level */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_getsockopt
	SYSCALL


.global bind
bind:
	MOVE $a3, $a1		/* socklen_t addrlen */
	CGETBASE $a2, CA0	/* const struct sockaddr *addr */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_bind
	SYSCALL

.global listen
listen:
	MOVE $a2, $a1		/* int backlog */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_listen
	SYSCALL

.global accept
accept:
	CGETBASE $a3, CA1	/* socklen_t *addrlen */
	CGETBASE $a2, CA0	/* struct sockaddr *addr */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_accept
	SYSCALL

.global connect
connect:
	MOVE $a3, $a1		/* socklen_t namelen */
	CGETBASE $a2, CA0	/* const struct sockaddr *name */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_connect
	SYSCALL

.global recv
recv:
	LI $a6, 0		/* (socklen_t *fromlen) */
	LI $a5, 0		/* (struct sockaddr *from) */
	MOVE $a4, $a2		/* int flags */
	MOVE $a3, $a1		/* size_t len */
	CGETBASE $a2, CA0	/* void *buf */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_recvfrom
	SYSCALL

.global recvfrom
recvfrom:
	CGETBASE $a6, CA2	/* socklen_t *fromlen */
	CGETBASE $a5, CA1	/* struct sockaddr *from */
	MOVE $a4, $a2		/* int flags */
	MOVE $a3, $a1		/* size_t len */
	CGETBASE $a2, CA0	/* void *buf */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_recvfrom
	SYSCALL

.global recvmsg
recvmsg:
	MOVE $a3, $a1		/* int flags */
	CGETBASE $a2, CA0	/* struct msghdr *msg */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_recvmsg
	SYSCALL

.global send
send:
	LI $a6, 0		/* (socklen_t tolen) */
	LI $a5, 0		/* (const struct sockaddr *to) */
	MOVE $a4, $a2		/* int flags */
	MOVE $a3, $a1		/* size_t len */
	CGETBASE $a2, CA0	/* const void *msg */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_sendto
	SYSCALL

.global sendto
sendto:
	MOVE $a6, $a3		/* socklen_t tolen */
	CGETBASE $a5, CA1	/* const struct sockaddr *to */
	MOVE $a4, $a2		/* int flags */
	MOVE $a3, $a1		/* size_t len */
	CGETBASE $a2, CA0	/* const void *msg */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_sendto
	SYSCALL

.global getsockname
getsockname:
	CGETBASE $a3, CA1	/* socklen_t *namelen */
	CGETBASE $a2, CA0	/* struct sockaddr *name */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_getsockname
	SYSCALL

.global sendmsg
sendmsg:
	MOVE $a3, $a1		/* int flags */
	CGETBASE $a2, CA0	/* const struct msghdr *msg */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_sendmsg
	SYSCALL

.global getpeername
getpeername:
	CGETBASE $a3, CA1	/* socklen_t *namelen */
	CGETBASE $a2, CA0	/* struct sockaddr *name */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_getpeername
	SYSCALL

.global shutdown
shutdown:
	MOVE $a2, $a1		/* int how */
	MOVE $a1, $a0		/* int s */
	LI $a0, SYS_shutdown
	SYSCALL

/* XXX: Doubly-indirect capability. */
.global sigaction
sigaction:
	CGETBASE $a3, CA1	/* struct sigaction *oact */
	CGETBASE $a2, CA0	/* const struct sigaction *act */
	MOVE $a1, $a0		/* int sig */
	LI $a0, SYS_sigaction
	SYSCALL

.global mmap
mmap:
	MOVE $a6, $a4		/* off_t offset */
	MOVE $a5, $a3		/* int fd */
	MOVE $a4, $a2		/* int flags */
	MOVE $a3, $a1		/* int prot */
	MOVE $a2, $a0		/* size_t len */
	CGETBASE $a1, CA0	/* void *addr */
	LI $a0, SYS_mmap
	SYSCALL
	CTOPTR

.global munmap
munmap:
	MOVE $a2, $a0		/* size_t len */
	CGETBASE $a1, CA0	/* void *addr */
	LI $a0, SYS_munmap
	SYSCALL

.global nanosleep
nanosleep:
	CGETBASE $a2, CA1	/* struct timespec *rmtp */
	CGETBASE $a1, CA0	/* const struct timespec *rqtp */
	LI $a0, SYS_nanosleep
	SYSCALL

.global getrusage
getrusage:
	CGETBASE $a2, CA0	/* struct rusage *rusage */
	MOVE $a1, $a0		/* int who */
	LI $a0, SYS_getrusage
	SYSCALL

.global __fcntl_int
__fcntl_int:
	MOVE $a3, $a1
	CGETBASE $a2, CA0	/* int cmd */
	MOVE $a1, $a0		/* int fd */
	LI $a0, SYS_fcntl
	SYSCALL

.global __fcntl_ptr
__fcntl_ptr:
	CGETBASE $a3, CA1
	CGETBASE $a2, CA0	/* int cmd */
	MOVE $a1, $a0		/* int fd */
	LI $a0, SYS_fcntl
	SYSCALL
