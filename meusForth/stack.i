






































 





















 




































 























 




























 




























 
 




































 









 










 

 






 








 









































 


















 






 








 





















 

 

 

 




 





 






 



 



 




 




 




 



























 



























 




 

typedef signed char		__int8_t;
typedef unsigned char		__uint8_t;
typedef	short			__int16_t;
typedef	unsigned short		__uint16_t;
typedef int			__int32_t;
typedef unsigned int		__uint32_t;
typedef long long		__int64_t;
typedef unsigned long long	__uint64_t;

typedef long			__darwin_intptr_t;
typedef unsigned int		__darwin_natural_t;

















 

typedef int			__darwin_ct_rune_t;	 




 
typedef union {
	char		__mbstate8[128];
	long long	_mbstateL;			 
} __mbstate_t;

typedef __mbstate_t		__darwin_mbstate_t;	 

typedef long	__darwin_ptrdiff_t;	 

typedef unsigned long		__darwin_size_t;	 

typedef __builtin_va_list	__darwin_va_list;	 

typedef int		__darwin_wchar_t;	 

typedef __darwin_wchar_t	__darwin_rune_t;	 

typedef int		__darwin_wint_t;	 

typedef unsigned long		__darwin_clock_t;	 
typedef __uint32_t		__darwin_socklen_t;	 
typedef long			__darwin_ssize_t;	 
typedef long			__darwin_time_t;	 



 

struct __darwin_pthread_handler_rec
{
	void           (*__routine)(void *);	 
	void           *__arg;			 
	struct __darwin_pthread_handler_rec *__next;
};
struct _opaque_pthread_attr_t { long __sig; char __opaque[56]; };
struct _opaque_pthread_cond_t { long __sig; char __opaque[40]; };
struct _opaque_pthread_condattr_t { long __sig; char __opaque[8]; };
struct _opaque_pthread_mutex_t { long __sig; char __opaque[56]; };
struct _opaque_pthread_mutexattr_t { long __sig; char __opaque[8]; };
struct _opaque_pthread_once_t { long __sig; char __opaque[8]; };
struct _opaque_pthread_rwlock_t { long __sig; char __opaque[192]; };
struct _opaque_pthread_rwlockattr_t { long __sig; char __opaque[16]; };
struct _opaque_pthread_t { long __sig; struct __darwin_pthread_handler_rec  *__cleanup_stack; char __opaque[1168]; };





 


typedef	__int64_t	__darwin_blkcnt_t;	 
typedef	__int32_t	__darwin_blksize_t;	 
typedef __int32_t	__darwin_dev_t;		 
typedef unsigned int	__darwin_fsblkcnt_t;	 
typedef unsigned int	__darwin_fsfilcnt_t;	 
typedef __uint32_t	__darwin_gid_t;		 
typedef __uint32_t	__darwin_id_t;		 
typedef __uint64_t	__darwin_ino64_t;	 
typedef __uint32_t	__darwin_ino_t;		 
typedef __darwin_natural_t __darwin_mach_port_name_t;  
typedef __darwin_mach_port_name_t __darwin_mach_port_t;  
typedef __uint16_t	__darwin_mode_t;	 
typedef __int64_t	__darwin_off_t;		 
typedef __int32_t	__darwin_pid_t;		 
typedef struct _opaque_pthread_attr_t
			__darwin_pthread_attr_t;  
typedef struct _opaque_pthread_cond_t
			__darwin_pthread_cond_t;  
typedef struct _opaque_pthread_condattr_t
			__darwin_pthread_condattr_t;  
typedef unsigned long	__darwin_pthread_key_t;	 
typedef struct _opaque_pthread_mutex_t
			__darwin_pthread_mutex_t;  
typedef struct _opaque_pthread_mutexattr_t
			__darwin_pthread_mutexattr_t;  
typedef struct _opaque_pthread_once_t
			__darwin_pthread_once_t;  
typedef struct _opaque_pthread_rwlock_t
			__darwin_pthread_rwlock_t;  
typedef struct _opaque_pthread_rwlockattr_t
			__darwin_pthread_rwlockattr_t;  
typedef struct _opaque_pthread_t
			*__darwin_pthread_t;	 
typedef __uint32_t	__darwin_sigset_t;	 
typedef __int32_t	__darwin_suseconds_t;	 
typedef __uint32_t	__darwin_uid_t;		 
typedef __uint32_t	__darwin_useconds_t;	 
typedef	unsigned char	__darwin_uuid_t[16];


typedef	int		__darwin_nl_item;
typedef	int		__darwin_wctrans_t;
typedef	__uint32_t	__darwin_wctype_t;





 
typedef __darwin_va_list	va_list;

typedef	__darwin_off_t		off_t;

typedef	__darwin_size_t		size_t;


typedef __darwin_off_t		fpos_t;






 

 
struct __sbuf {
	unsigned char	*_base;
	int		_size;
};

 
struct __sFILEX;


























 
typedef	struct __sFILE {
	unsigned char *_p;	 
	int	_r;		 
	int	_w;		 
	short	_flags;		 
	short	_file;		 
	struct	__sbuf _bf;	 
	int	_lbfsize;	 

	 
	void	*_cookie;	 
	int	(*_close)(void *);
	int	(*_read) (void *, char *, int);
	fpos_t	(*_seek) (void *, fpos_t, int);
	int	(*_write)(void *, const char *, int);

	 
	struct	__sbuf _ub;	 
	struct __sFILEX *_extra;  
	int	_ur;		 

	 
	unsigned char _ubuf[3];	 
	unsigned char _nbuf[1];	 

	 
	struct	__sbuf _lb;	 

	 
	int	_blksize;	 
	fpos_t	_offset;	 
} FILE;


extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;


	 









 






 
				 

 





 

void	 clearerr(FILE *);
int	 fclose(FILE *);
int	 feof(FILE *);
int	 ferror(FILE *);
int	 fflush(FILE *);
int	 fgetc(FILE *);
int	 fgetpos(FILE * restrict, fpos_t *);
char	*fgets(char * restrict, int, FILE *);
FILE	*fopen(const char * restrict, const char * restrict);
int	 fprintf(FILE * restrict, const char * restrict, ...) ;
int	 fputc(int, FILE *);
int	 fputs(const char * restrict, FILE * restrict) __asm("_" "fputs" );
size_t	 fread(void * restrict, size_t, size_t, FILE * restrict);
FILE	*freopen(const char * restrict, const char * restrict,
	    FILE * restrict) __asm("_" "freopen" );
int	 fscanf(FILE * restrict, const char * restrict, ...) ;
int	 fseek(FILE *, long, int);
int	 fsetpos(FILE *, const fpos_t *);
long	 ftell(FILE *);
size_t	 fwrite(const void * restrict, size_t, size_t, FILE * restrict) __asm("_" "fwrite" );
int	 getc(FILE *);
int	 getchar(void);
char	*gets(char *);
extern const int sys_nerr;		 
extern const char *const sys_errlist[];
void	 perror(const char *);
int	 printf(const char * restrict, ...) ;
int	 putc(int, FILE *);
int	 putchar(int);
int	 puts(const char *);
int	 remove(const char *);
int	 rename (const char *, const char *);
void	 rewind(FILE *);
int	 scanf(const char * restrict, ...) ;
void	 setbuf(FILE * restrict, char * restrict);
int	 setvbuf(FILE * restrict, char * restrict, int, size_t);
int	 sprintf(char * restrict, const char * restrict, ...) ;
int	 sscanf(const char * restrict, const char * restrict, ...) ;
FILE	*tmpfile(void);
char	*tmpnam(char *);
int	 ungetc(int, FILE *);
int	 vfprintf(FILE * restrict, const char * restrict, va_list) ;
int	 vprintf(const char * restrict, va_list) ;
int	 vsprintf(char * restrict, const char * restrict, va_list) ;
int	 asprintf(char **, const char *, ...) ;
int	 vasprintf(char **, const char *, va_list) ;




 


char	*ctermid(char *);
char	*ctermid_r(char *);
FILE	*fdopen(int, const char *);
char	*fgetln(FILE *, size_t *);
int	 fileno(FILE *);
void	 flockfile(FILE *);
const char
	*fmtcheck(const char *, const char *);
int	 fpurge(FILE *);
int	 fseeko(FILE *, off_t, int);
off_t	 ftello(FILE *);
int	 ftrylockfile(FILE *);
void	 funlockfile(FILE *);
int	 getc_unlocked(FILE *);
int	 getchar_unlocked(void);
int	 getw(FILE *);
int	 pclose(FILE *);
FILE	*popen(const char *, const char *);
int	 putc_unlocked(int, FILE *);
int	 putchar_unlocked(int);
int	 putw(int, FILE *);
void	 setbuffer(FILE *, char *, int);
int	 setlinebuf(FILE *);
int	 snprintf(char * restrict, size_t, const char * restrict, ...) ;
char	*tempnam(const char *, const char *) __asm("_" "tempnam" );
int	 vfscanf(FILE * restrict, const char * restrict, va_list) ;
int	 vscanf(const char * restrict, va_list) ;
int	 vsnprintf(char * restrict, size_t, const char * restrict, va_list) ;
int	 vsscanf(const char * restrict, const char * restrict, va_list) ;
FILE	*zopen(const char *, const char *, int);




 

FILE	*funopen(const void *,
		int (*)(void *, char *, int),
		int (*)(void *, const char *, int),
		fpos_t (*)(void *, fpos_t, int),
		int (*)(void *));




 

int	__srget(FILE *);
int	__svfscanf(FILE *, const char *, va_list) ;
int	__swbuf(int, FILE *);





 
static __inline int __sputc(int _c, FILE *_p) {
	if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
		return (*_p->_p++ = _c);
	else
		return (__swbuf(_c, _p));
}














































 





 





void __assert_rtn(const char *, const char *, int, const char *) __attribute__((__noreturn__));
void __eprintf(const char *, const char *, unsigned, const char *) __attribute__((__noreturn__));










































 






















 

































 



typedef __darwin_ssize_t	ssize_t;




void	*memchr(const void *, int, size_t);
int	 memcmp(const void *, const void *, size_t);
void	*memcpy(void *, const void *, size_t);
void	*memmove(void *, const void *, size_t);
void	*memset(void *, int, size_t);
char	*stpcpy(char *, const char *);
char	*strcasestr(const char *, const char *);
char	*strcat(char *, const char *);
char	*strchr(const char *, int);
int	 strcmp(const char *, const char *);
int	 strcoll(const char *, const char *);
char	*strcpy(char *, const char *);
size_t	 strcspn(const char *, const char *);
char	*strerror(int) __asm("_" "strerror" );
int	 strerror_r(int, char *, size_t);
size_t	 strlen(const char *);
char	*strncat(char *, const char *, size_t);
int	 strncmp(const char *, const char *, size_t);
char	*strncpy(char *, const char *, size_t);
char	*strnstr(const char *, const char *, size_t);
char	*strpbrk(const char *, const char *);
char	*strrchr(const char *, int);
size_t	 strspn(const char *, const char *);
char	*strstr(const char *, const char *);
char	*strtok(char *, const char *);
size_t	 strxfrm(char *, const char *, size_t);

 
void	*memccpy(void *, const void *, int, size_t);
char	*strtok_r(char *, const char *, char **);
char	*strdup(const char *);
int	 bcmp(const void *, const void *, size_t);
void	 bcopy(const void *, void *, size_t);
void	 bzero(void *, size_t);
int	 ffs(int);
int	 ffsl(long);
int	 fls(int);
int	 flsl(long);
char	*index(const char *, int);
void     memset_pattern4(void *, const void *, size_t);
void     memset_pattern8(void *, const void *, size_t);
void     memset_pattern16(void *, const void *, size_t);
char	*rindex(const char *, int);
int	 strcasecmp(const char *, const char *);
size_t	 strlcat(char *, const char *, size_t);
size_t	 strlcpy(char *, const char *, size_t);
void	 strmode(int, char *);
int	 strncasecmp(const char *, const char *, size_t);
char	*strsep(char **, const char *);
char	*strsignal(int sig);
void	 swab(const void * restrict, void * restrict, ssize_t);











 




 


 



 

		 
		 
typedef long ptrdiff_t;

typedef unsigned long	size_t;

typedef int	wchar_t;



   



 



 

 
    typedef signed char           int8_t;

    typedef short                int16_t;

    typedef int                  int32_t;

        typedef long long   int64_t;

    typedef unsigned char         uint8_t;

    typedef unsigned short       uint16_t;

    typedef unsigned int         uint32_t;

        typedef unsigned long long      uint64_t;

 
typedef int8_t           int_least8_t;
typedef int16_t         int_least16_t;
typedef int32_t         int_least32_t;
typedef int64_t         int_least64_t;
typedef uint8_t         uint_least8_t;
typedef uint16_t       uint_least16_t;
typedef uint32_t       uint_least32_t;
typedef uint64_t       uint_least64_t;


 
typedef int8_t            int_fast8_t;
typedef int16_t          int_fast16_t;
typedef int32_t          int_fast32_t;
typedef int64_t          int_fast64_t;
typedef uint8_t          uint_fast8_t;
typedef uint16_t        uint_fast16_t;
typedef uint32_t        uint_fast32_t;
typedef uint64_t        uint_fast64_t;

 

    typedef long   intptr_t;

    typedef unsigned long   uintptr_t;


 
        typedef long int             intmax_t;

        typedef long unsigned int             uintmax_t;






 





 


 

   




 


 



 



 



 


 



 







 






 

 



























































































 






















 

































 

































































 




 





 



 



 



 



 



 







 






 





 










 






 






 





 











 






 






 






 





 









 






 






 






 






 





 










 






 






 






 






 






 





 









 






 






 






 






 






 






 





 






























 
 

































 
 





 




 
typedef enum {
	P_ALL,
	P_PID,
	P_PGID
} idtype_t;




 
typedef __darwin_pid_t	pid_t;

typedef __darwin_id_t	id_t;

















 


























 
 






































 




























 









































 



























 



 



typedef int sig_atomic_t; 










 


























 


















 




























 
































 


























 





























 


 






 

struct __darwin_i386_thread_state
{
    unsigned int	__eax;
    unsigned int	__ebx;
    unsigned int	__ecx;
    unsigned int	__edx;
    unsigned int	__edi;
    unsigned int	__esi;
    unsigned int	__ebp;
    unsigned int	__esp;
    unsigned int	__ss;
    unsigned int	__eflags;
    unsigned int	__eip;
    unsigned int	__cs;
    unsigned int	__ds;
    unsigned int	__es;
    unsigned int	__fs;
    unsigned int	__gs;
};

 

struct __darwin_fp_control
{
    unsigned short		__invalid	:1,
    				__denorm	:1,
				__zdiv		:1,
				__ovrfl		:1,
				__undfl		:1,
				__precis	:1,
						:2,
				__pc		:2,
				__rc		:2,
					 	:1,
						:3;
};
typedef struct __darwin_fp_control	__darwin_fp_control_t;



 

struct __darwin_fp_status
{
    unsigned short		__invalid	:1,
    				__denorm	:1,
				__zdiv		:1,
				__ovrfl		:1,
				__undfl		:1,
				__precis	:1,
				__stkflt	:1,
				__errsumm	:1,
				__c0		:1,
				__c1		:1,
				__c2		:1,
				__tos		:3,
				__c3		:1,
				__busy		:1;
};
typedef struct __darwin_fp_status	__darwin_fp_status_t;
				
 

struct __darwin_mmst_reg
{
	char	__mmst_reg[10];
	char	__mmst_rsrv[6];
};


 

struct __darwin_xmm_reg
{
	char		__xmm_reg[16];
};



 


struct __darwin_i386_float_state
{
	int 			__fpu_reserved[2];
	struct __darwin_fp_control	__fpu_fcw;		 
	struct __darwin_fp_status	__fpu_fsw;		 
	__uint8_t		__fpu_ftw;		 
	__uint8_t		__fpu_rsrv1;		  
	__uint16_t		__fpu_fop;		 
	__uint32_t		__fpu_ip;		 
	__uint16_t		__fpu_cs;		 
	__uint16_t		__fpu_rsrv2;		 
	__uint32_t		__fpu_dp;		 
	__uint16_t		__fpu_ds;		 
	__uint16_t		__fpu_rsrv3;		 
	__uint32_t		__fpu_mxcsr;		 
	__uint32_t		__fpu_mxcsrmask;	 
	struct __darwin_mmst_reg	__fpu_stmm0;		 
	struct __darwin_mmst_reg	__fpu_stmm1;		 
	struct __darwin_mmst_reg	__fpu_stmm2;		 
	struct __darwin_mmst_reg	__fpu_stmm3;		 
	struct __darwin_mmst_reg	__fpu_stmm4;		 
	struct __darwin_mmst_reg	__fpu_stmm5;		 
	struct __darwin_mmst_reg	__fpu_stmm6;		 
	struct __darwin_mmst_reg	__fpu_stmm7;		 
	struct __darwin_xmm_reg		__fpu_xmm0;		 
	struct __darwin_xmm_reg		__fpu_xmm1;		 
	struct __darwin_xmm_reg		__fpu_xmm2;		 
	struct __darwin_xmm_reg		__fpu_xmm3;		 
	struct __darwin_xmm_reg		__fpu_xmm4;		 
	struct __darwin_xmm_reg		__fpu_xmm5;		 
	struct __darwin_xmm_reg		__fpu_xmm6;		 
	struct __darwin_xmm_reg		__fpu_xmm7;		 
	char			__fpu_rsrv4[14*16];	 
	int 			__fpu_reserved1;
};

struct __darwin_i386_exception_state
{
    unsigned int	__trapno;
    unsigned int	__err;
    unsigned int	__faultvaddr;
};

struct __darwin_x86_debug_state32
{
	unsigned int	__dr0;
	unsigned int	__dr1;
	unsigned int	__dr2;
	unsigned int	__dr3;
	unsigned int	__dr4;
	unsigned int	__dr5;
	unsigned int	__dr6;
	unsigned int	__dr7;
};



 

struct __darwin_x86_thread_state64
{
	__uint64_t	__rax;
	__uint64_t	__rbx;
	__uint64_t	__rcx;
	__uint64_t	__rdx;
	__uint64_t	__rdi;
	__uint64_t	__rsi;
	__uint64_t	__rbp;
	__uint64_t	__rsp;
	__uint64_t	__r8;
	__uint64_t	__r9;
	__uint64_t	__r10;
	__uint64_t	__r11;
	__uint64_t	__r12;
	__uint64_t	__r13;
	__uint64_t	__r14;
	__uint64_t	__r15;
	__uint64_t	__rip;
	__uint64_t	__rflags;
	__uint64_t	__cs;
	__uint64_t	__fs;
	__uint64_t	__gs;
};


struct __darwin_x86_float_state64
{
	int 			__fpu_reserved[2];
	struct __darwin_fp_control	__fpu_fcw;		 
	struct __darwin_fp_status	__fpu_fsw;		 
	__uint8_t		__fpu_ftw;		 
	__uint8_t		__fpu_rsrv1;		  
	__uint16_t		__fpu_fop;		 

	 
	__uint32_t		__fpu_ip;		 
	__uint16_t		__fpu_cs;		 

	__uint16_t		__fpu_rsrv2;		 

	 
	__uint32_t		__fpu_dp;		 
	__uint16_t		__fpu_ds;		 

	__uint16_t		__fpu_rsrv3;		 
	__uint32_t		__fpu_mxcsr;		 
	__uint32_t		__fpu_mxcsrmask;	 
	struct __darwin_mmst_reg	__fpu_stmm0;		 
	struct __darwin_mmst_reg	__fpu_stmm1;		 
	struct __darwin_mmst_reg	__fpu_stmm2;		 
	struct __darwin_mmst_reg	__fpu_stmm3;		 
	struct __darwin_mmst_reg	__fpu_stmm4;		 
	struct __darwin_mmst_reg	__fpu_stmm5;		 
	struct __darwin_mmst_reg	__fpu_stmm6;		 
	struct __darwin_mmst_reg	__fpu_stmm7;		 
	struct __darwin_xmm_reg		__fpu_xmm0;		 
	struct __darwin_xmm_reg		__fpu_xmm1;		 
	struct __darwin_xmm_reg		__fpu_xmm2;		 
	struct __darwin_xmm_reg		__fpu_xmm3;		 
	struct __darwin_xmm_reg		__fpu_xmm4;		 
	struct __darwin_xmm_reg		__fpu_xmm5;		 
	struct __darwin_xmm_reg		__fpu_xmm6;		 
	struct __darwin_xmm_reg		__fpu_xmm7;		 
	struct __darwin_xmm_reg		__fpu_xmm8;		 
	struct __darwin_xmm_reg		__fpu_xmm9;		 
	struct __darwin_xmm_reg		__fpu_xmm10;		 
	struct __darwin_xmm_reg		__fpu_xmm11;		 
	struct __darwin_xmm_reg		__fpu_xmm12;		 
	struct __darwin_xmm_reg		__fpu_xmm13;		 
	struct __darwin_xmm_reg		__fpu_xmm14;		 
	struct __darwin_xmm_reg		__fpu_xmm15;		 
	char			__fpu_rsrv4[6*16];	 
	int 			__fpu_reserved1;
};

struct __darwin_x86_exception_state64
{
    unsigned int	__trapno;
    unsigned int	__err;
    __uint64_t		__faultvaddr;
};

struct __darwin_x86_debug_state64
{
	__uint64_t	__dr0;
	__uint64_t	__dr1;
	__uint64_t	__dr2;
	__uint64_t	__dr3;
	__uint64_t	__dr4;
	__uint64_t	__dr5;
	__uint64_t	__dr6;
	__uint64_t	__dr7;
};




struct __darwin_mcontext32
{
	struct __darwin_i386_exception_state	__es;
	struct __darwin_i386_thread_state	__ss;
	struct __darwin_i386_float_state	__fs;
};

struct __darwin_mcontext64
{
	struct __darwin_x86_exception_state64	__es;
	struct __darwin_x86_thread_state64	__ss;
	struct __darwin_x86_float_state64	__fs;
};

typedef struct __darwin_mcontext64	*mcontext_t;




 
struct __darwin_sigaltstack
{
	void		*ss_sp;		 
	__darwin_size_t ss_size;	 
	int		ss_flags;	 
};




struct __darwin_ucontext
{
	int			uc_onstack;
	__darwin_sigset_t	uc_sigmask;	 
	struct __darwin_sigaltstack 	uc_stack;	 
	struct __darwin_ucontext	*uc_link;	 
	__darwin_size_t		uc_mcsize;	 
	struct __darwin_mcontext64	*uc_mcontext;	 
};




typedef struct __darwin_sigaltstack	stack_t;  

 
typedef struct __darwin_ucontext	ucontext_t;	 



typedef __darwin_pthread_attr_t		pthread_attr_t;

typedef __darwin_sigset_t		sigset_t;


typedef __darwin_uid_t			uid_t;

union sigval {
	 
	int	sival_int;
	void	*sival_ptr;
};


struct sigevent {
	int				sigev_notify;				 
	int				sigev_signo;				 
	union sigval	sigev_value;				 
	void			(*sigev_notify_function)(union sigval);	   
	pthread_attr_t	*sigev_notify_attributes;	 
};


typedef struct __siginfo {
	int	si_signo;		 
	int	si_errno;		 
	int	si_code;		 
	pid_t	si_pid;			 
	uid_t	si_uid;			 
	int	si_status;		 
	void	*si_addr;		 
	union sigval si_value;		 
	long	si_band;		 
	unsigned long	__pad[7];	 
} siginfo_t;











 

 

 

 

 

 

 

 

 

 
union __sigaction_u {
	void    (*__sa_handler)(int);
	void    (*__sa_sigaction)(int, struct __siginfo *,
		       void *);
};

 
struct	__sigaction {
	union __sigaction_u __sigaction_u;   
	void    (*sa_tramp)(void *, int, int, siginfo_t *, void *);
	sigset_t sa_mask;		 
	int	sa_flags;		 
};



 
struct	sigaction {
	union __sigaction_u __sigaction_u;   
	sigset_t sa_mask;		 
	int	sa_flags;		 
};



 

 



 



 

 

typedef	void (*sig_t)(int);	 



 





 
struct	sigvec {
	void	(*sv_handler)(int);	 
	int	sv_mask;		 
	int	sv_flags;		 
};





 
struct	sigstack {
	char	*ss_sp;			 
	int	ss_onstack;		 
};




 







 

void	(*signal(int, void (*)(int)))(int);



























 
 

































 






 


























 










struct timeval
{
	__darwin_time_t		tv_sec;		 
	__darwin_suseconds_t	tv_usec;	 
};









 




 
typedef __uint64_t	rlim_t;




 




 




 




 






 




 









 
struct	rusage {
	struct timeval ru_utime;	 
	struct timeval ru_stime;	 
	



 
	long	ru_maxrss;		 
	long	ru_ixrss;		 
	long	ru_idrss;		 
	long	ru_isrss;		 
	long	ru_minflt;		 
	long	ru_majflt;		 
	long	ru_nswap;		 
	long	ru_inblock;		 
	long	ru_oublock;		 
	long	ru_msgsnd;		 
	long	ru_msgrcv;		 
	long	ru_nsignals;		 
	long	ru_nvcsw;		 
	long	ru_nivcsw;		 
};






 





 




 




 
struct rlimit {
	rlim_t	rlim_cur;		 
	rlim_t	rlim_max;		 
};

 

 

 




int	getpriority(int, id_t);
int	getiopolicy_np(int, int);
int	getrlimit(int, struct rlimit *) __asm("_" "getrlimit" );
int	getrusage(int, struct rusage *);
int	setpriority(int, id_t, int);
int	setiopolicy_np(int, int, int);
int	setrlimit(int, const struct rlimit *) __asm("_" "setrlimit" );











 




 

 




 
 





 
 
 
 


 



 



























 


 



























 


 

































 




 
 



 




 































 




























 































 





 




























 








 


 






























 



 

static inline
__uint16_t
_OSSwapInt16(
    __uint16_t        _data
)
{
    return ((_data << 8) | (_data >> 8));
}

static inline
__uint32_t
_OSSwapInt32(
    __uint32_t        _data
)
{
    __asm__ ("bswap   %0" : "+r" (_data));
    return _data;
}

static inline
__uint64_t
_OSSwapInt64(
    __uint64_t        _data
)
{
    __asm__ ("bswap   %0" : "+r" (_data));
    return _data;
}



















 
union wait {
	int	w_status;		 
	

 
	struct {
		unsigned int	w_Termsig:7,	 
				w_Coredump:1,	 
				w_Retcode:8,	 
				w_Filler:16;	 
	} w_T;
	



 
	struct {
		unsigned int	w_Stopval:8,	 
				w_Stopsig:8,	 
				w_Filler:16;	 
	} w_S;
};




pid_t	wait(int *) __asm("_" "wait"  );
pid_t	waitpid(pid_t, int *, int) __asm("_" "waitpid"  );
int	waitid(idtype_t, id_t, siginfo_t *, int) __asm("_" "waitid"  );
pid_t	wait3(int *, int, struct rusage *);
pid_t	wait4(pid_t, int *, int, struct rusage *);






















 





void	*alloca(size_t);		 


 



typedef	__darwin_ct_rune_t	ct_rune_t;

typedef __darwin_rune_t   	rune_t;


typedef struct {
	int quot;		 
	int rem;		 
} div_t;

typedef struct {
	long quot;		 
	long rem;		 
} ldiv_t;

typedef struct {
	long long quot;
	long long rem;
} lldiv_t;





extern int __mb_cur_max;



void	 abort(void) __attribute__((__noreturn__));
int	 abs(int) __attribute__((__const__));
int	 atexit(void (*)(void));
double	 atof(const char *);
int	 atoi(const char *);
long	 atol(const char *);
long long
	 atoll(const char *);
void	*bsearch(const void *, const void *, size_t,
	    size_t, int (*)(const void *, const void *));
void	*calloc(size_t, size_t);
div_t	 div(int, int) __attribute__((__const__));
void	 exit(int) __attribute__((__noreturn__));
void	 free(void *);
char	*getenv(const char *);
long	 labs(long) __attribute__((__const__));
ldiv_t	 ldiv(long, long) __attribute__((__const__));
long long
	 llabs(long long);
lldiv_t	 lldiv(long long, long long);
void	*malloc(size_t);
int	 mblen(const char *, size_t);
size_t	 mbstowcs(wchar_t * restrict , const char * restrict, size_t);
int	 mbtowc(wchar_t * restrict, const char * restrict, size_t);
void	 qsort(void *, size_t, size_t,
	    int (*)(const void *, const void *));
int	 rand(void);
void	*realloc(void *, size_t);
void	 srand(unsigned);
double	 strtod(const char *, char **) __asm("_" "strtod" );
float	 strtof(const char *, char **) __asm("_" "strtof" );
long	 strtol(const char *, char **, int);
long double
	 strtold(const char *, char **) ;
long long 
	 strtoll(const char *, char **, int);
unsigned long
	 strtoul(const char *, char **, int);
unsigned long long
	 strtoull(const char *, char **, int);
int	 system(const char *) __asm("_" "system"  );
size_t	 wcstombs(char * restrict, const wchar_t * restrict, size_t);
int	 wctomb(char *, wchar_t);

void	_Exit(int) __attribute__((__noreturn__));
long	 a64l(const char *);
double	 drand48(void);
char	*ecvt(double, int, int *restrict, int *restrict);  
double	 erand48(unsigned short[3]); 
char	*fcvt(double, int, int *restrict, int *restrict);  
char	*gcvt(double, int, char *);  
int	 getsubopt(char **, char * const *, char **);
int	 grantpt(int);
char	*initstate(unsigned, char *, size_t);  
long	 jrand48(unsigned short[3]);
char	*l64a(long);
void	 lcong48(unsigned short[7]);
long	 lrand48(void);
char	*mktemp(char *);
int	 mkstemp(char *);
long	 mrand48(void); 
long	 nrand48(unsigned short[3]);
int	 posix_openpt(int);
char	*ptsname(int);
int	 putenv(char *) __asm("_" "putenv" );
long	 random(void);
int	 rand_r(unsigned *);
char	*realpath(const char * restrict, char * restrict) __asm("_" "realpath" "$DARWIN_EXTSN");
unsigned short
	*seed48(unsigned short[3]);
int	 setenv(const char *, const char *, int) __asm("_" "setenv" );
void	 setkey(const char *) __asm("_" "setkey" );
char	*setstate(const char *);
void	 srand48(long);
void	 srandom(unsigned);
int	 unlockpt(int);
int	 unsetenv(const char *) __asm("_" "unsetenv" );



























 


 



























 


 

































 





 
typedef	unsigned char		u_int8_t;
typedef	unsigned short		u_int16_t;
typedef	unsigned int		u_int32_t;
typedef	unsigned long long	u_int64_t;

typedef int64_t			register_t;


 
typedef u_int64_t		user_addr_t;	
typedef u_int64_t		user_size_t;	
typedef int64_t			user_ssize_t;
typedef int64_t			user_long_t;
typedef u_int64_t		user_ulong_t;
typedef int64_t			user_time_t;

 
typedef u_int64_t		syscall_arg_t;




typedef	__darwin_dev_t	dev_t;

typedef	__darwin_mode_t	mode_t;

u_int32_t
	 arc4random(void);
void	 arc4random_addrandom(unsigned char *dat, int datlen);
void	 arc4random_stir(void);

	  
char	*cgetcap(char *, const char *, int);
int	 cgetclose(void);
int	 cgetent(char **, char **, const char *);
int	 cgetfirst(char **, char **);
int	 cgetmatch(const char *, const char *);
int	 cgetnext(char **, char **);
int	 cgetnum(char *, const char *, long *);
int	 cgetset(const char *);
int	 cgetstr(char *, const char *, char **);
int	 cgetustr(char *, const char *, char **);

int	 daemon(int, int) __asm("_" "daemon" "$1050") __attribute__((deprecated));
char	*devname(dev_t, mode_t);
char	*devname_r(dev_t, mode_t, char *buf, int len);
char	*getbsize(int *, long *);
int	 getloadavg(double [], int);
const char
	*getprogname(void);

int	 heapsort(void *, size_t, size_t,
	    int (*)(const void *, const void *));
int	 mergesort(void *, size_t, size_t,
	    int (*)(const void *, const void *));
void	 qsort_r(void *, size_t, size_t, void *,
	    int (*)(void *, const void *, const void *));
int	 radixsort(const unsigned char **, int, const unsigned char *,
	    unsigned);
void	 setprogname(const char *);
int	 sradixsort(const unsigned char **, int, const unsigned char *,
	    unsigned);
void	 sranddev(void);
void	 srandomdev(void);
void	*reallocf(void *, size_t);
long long
	 strtoq(const char *, char **, int);
unsigned long long
	 strtouq(const char *, char **, int);
extern char *suboptarg;		 
void	*valloc(size_t);

 










































 





 





void __assert_rtn(const char *, const char *, int, const char *) __attribute__((__noreturn__));
void __eprintf(const char *, const char *, unsigned, const char *) __attribute__((__noreturn__));

























 

























 
     

     

typedef struct Object* Object;


struct Object {
    
    
    Object  __c_class__;
    
    int8_t* __name__;
    
    int8_t* __doc__;
    
    int32_t __size;
};

static inline uint8_t* c_class_mem_dup(uint8_t const*  ptr,
                                       size_t const    size)
{
    (__builtin_expect(!(ptr), 0) ? __assert_rtn(__func__, "c_class.h", 243, "ptr") : (void)0);

    uint8_t* newp = (uint8_t*) malloc(size);

    (__builtin_expect(!(newp), 0) ? __assert_rtn(__func__, "c_class.h", 247, "newp") : (void)0);

    if(newp)
        memcpy(newp, ptr, size);

    return newp;
}
















typedef struct I32Stack* I32Stack; extern void I32Stack_init(void); extern void I32Stack_Alloc(I32Stack this); extern int32_t __I32Stack_initialised; extern struct I32Stack __I32Stack; struct I32Stack { I32Stack __c_class__;

        int32_t     m_front;
        int32_t     m_rear;
        size_t      m_stack_size;
        int32_t*    m_stack;

        I32Stack    (*construct) (I32Stack this, size_t size);
        void        (*destruct) (I32Stack this);
        void        (*reset) (I32Stack this);
        _Bool        (*isEmpty) (I32Stack this);
        _Bool        (*isFull) (I32Stack this);
        void        (*push) (I32Stack this, int32_t x);
        int32_t     (*pop) (I32Stack this);
        void        (*rwlock) (I32Stack this);
        void        (*rwunlock) (I32Stack this);

};


typedef struct PI8Stack* PI8Stack; extern void PI8Stack_init(void); extern void PI8Stack_Alloc(PI8Stack this); extern int32_t __PI8Stack_initialised; extern struct PI8Stack __PI8Stack; struct PI8Stack { PI8Stack __c_class__;

        int32_t     m_front;
        int32_t     m_rear;
        size_t      m_stack_size;
        int8_t**    m_stack;

        PI8Stack    (*construct) (PI8Stack this, size_t size);
        void        (*destruct) (PI8Stack this);
        void        (*reset) (PI8Stack this);
        _Bool        (*isEmpty) (PI8Stack this);
        _Bool        (*isFull) (PI8Stack this);
        void        (*push) (PI8Stack this, int8_t* x);
        int8_t*     (*pop) (PI8Stack this);
        void        (*rwlock) (PI8Stack this);
        void        (*rwunlock) (PI8Stack this);

};


typedef struct PVOIDStack* PVOIDStack; extern void PVOIDStack_init(void); extern void PVOIDStack_Alloc(PVOIDStack this); extern int32_t __PVOIDStack_initialised; extern struct PVOIDStack __PVOIDStack; struct PVOIDStack { PVOIDStack __c_class__;

        int32_t     m_front;
        int32_t     m_rear;
        size_t      m_stack_size;
        void**      m_stack;

        PVOIDStack  (*construct) (PVOIDStack this, size_t size);
        void        (*destruct) (PVOIDStack this);
        void        (*reset) (PVOIDStack this, _Bool do_dealloc);
        _Bool        (*isEmpty) (PVOIDStack this);
        _Bool        (*isFull) (PVOIDStack this);
        void        (*push) (PVOIDStack this, void* x);
        void*       (*pop) (PVOIDStack this);
        void*       (*peek) (PVOIDStack this);
        void        (*rwlock) (PVOIDStack this);
        void        (*rwunlock) (PVOIDStack this);

};


        extern I32Stack    I32Stack_construct(I32Stack self, size_t size);
        extern void        I32Stack_destruct(I32Stack self);
        extern void        I32Stack_reset(I32Stack self);
        extern void        I32Stack_push(I32Stack self, int32_t x);
        extern int32_t     I32Stack_pop(I32Stack self);

        extern PI8Stack    PI8Stack_construct(PI8Stack self, size_t size);
        extern void        PI8Stack_destruct(PI8Stack self);
        extern void        PI8Stack_reset(PI8Stack self);
        extern void        PI8Stack_push(PI8Stack self, int8_t* x);
        extern int8_t*     PI8Stack_pop(PI8Stack self);

        extern PVOIDStack   PVOIDStack_construct(PVOIDStack self, size_t size);
        extern void         PVOIDStack_destruct(PVOIDStack self);
        extern void         PVOIDStack_reset(PVOIDStack self, _Bool do_dealloc);
        extern void         PVOIDStack_push(PVOIDStack self, void* x);
        extern void*        PVOIDStack_pop(PVOIDStack self);
        extern void*        PVOIDStack_peek(PVOIDStack self);










































 


        extern int8_t*      str_dup(int8_t const* src);








static _Bool I32Stack_isEmpty(I32Stack self);
static _Bool I32Stack_isFull(I32Stack self);
static void I32Stack_rwlock(I32Stack self);
static void I32Stack_rwunlock(I32Stack self);

static _Bool PI8Stack_isEmpty(PI8Stack self);
static _Bool PI8Stack_isFull(PI8Stack self);
static void PI8Stack_rwlock(PI8Stack self);
static void PI8Stack_rwunlock(PI8Stack self);

static _Bool PVOIDStack_isEmpty(PVOIDStack self);
static _Bool PVOIDStack_isFull(PVOIDStack self);
static void PVOIDStack_rwlock(PVOIDStack self);
static void PVOIDStack_rwunlock(PVOIDStack self);


 
I32Stack I32Stack_construct(I32Stack  self,
                            size_t    size)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 68, "self") : (void)0);
    (__builtin_expect(!(size), 0) ? __assert_rtn(__func__, "stack.c", 69, "size") : (void)0);


    self->m_stack       = (int32_t*) malloc(sizeof(int32_t) * size);

    (__builtin_expect(!(self->m_stack), 0) ? __assert_rtn(__func__, "stack.c", 77, "self->m_stack") : (void)0);

    self->m_stack_size  = size + 1;
    self->m_front       = 0;
    self->m_rear        = 0;


    return self;
}

void I32Stack_destruct(I32Stack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 92, "self") : (void)0);

    
    free(self->m_stack);

    
    free(self);
}

void I32Stack_push(I32Stack self,
                   int32_t  x)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 104, "self") : (void)0);


    if(self->isFull(self))
    {
        
        self->m_stack_size  = self->m_stack_size + self->m_stack_size;
        self->m_stack       = (int32_t*) realloc((void*) self->m_stack, sizeof(int32_t) * self->m_stack_size);

        (__builtin_expect(!(self->m_stack), 0) ? __assert_rtn(__func__, "stack.c", 116, "self->m_stack") : (void)0);
    }

    self->m_stack[self->m_rear] = x;
    self->m_rear                = self->m_rear + 1;

}

int32_t I32Stack_pop(I32Stack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 129, "self") : (void)0);

    int32_t item = (-1);


    if(!(self->isEmpty(self)))
    {
       self->m_rear                 = self->m_rear - 1;
       item                         = self->m_stack[self->m_rear];
       self->m_stack[self->m_rear]  = 0;
    }


    return item;
}

_Bool I32Stack_isEmpty(I32Stack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 153, "self") : (void)0);

    _Bool status  = 0;


    if(self->m_front > (self->m_rear - 1))
        status = 1;


    return status;
}

_Bool I32Stack_isFull(I32Stack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 173, "self") : (void)0);

    _Bool status = 0;


    if(self->m_rear == self->m_stack_size)
        status = 1;


    return status;
}

void I32Stack_reset(I32Stack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 193, "self") : (void)0);


    self->m_front = 0;
    self->m_rear  = 0;

}

void I32Stack_rwlock(I32Stack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 209, "self") : (void)0);
}

void I32Stack_rwunlock(I32Stack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 214, "self") : (void)0);
}

struct I32Stack __I32Stack; void I32Stack_init(void) { if(!__I32Stack_initialised) { I32Stack_Alloc(&__I32Stack); __I32Stack_initialised = 1; } } int32_t __I32Stack_initialised = 0; void I32Stack_Alloc(I32Stack this) { ((Object) this)->__c_class__ = (Object) &__I32Stack; ((Object) this)->__size = sizeof(struct I32Stack);
  this->construct    = I32Stack_construct;
  this->destruct     = I32Stack_destruct;
  this->reset        = I32Stack_reset;
  this->isEmpty      = I32Stack_isEmpty;
  this->isFull       = I32Stack_isFull;
  this->push         = I32Stack_push;
  this->pop          = I32Stack_pop;
  this->rwlock       = I32Stack_rwlock;
  this->rwunlock     = I32Stack_rwunlock;
}



 
PI8Stack PI8Stack_construct(PI8Stack  self,
                            size_t    size)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 235, "self") : (void)0);
    (__builtin_expect(!(size), 0) ? __assert_rtn(__func__, "stack.c", 236, "size") : (void)0);


    self->m_stack       = (int8_t**) malloc(sizeof(int8_t*) * size);

    (__builtin_expect(!(self->m_stack), 0) ? __assert_rtn(__func__, "stack.c", 244, "self->m_stack") : (void)0);

    self->m_stack_size  = size;
    self->m_front       = 0;
    self->m_rear        = 0;


    return self;
}

void PI8Stack_destruct(PI8Stack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 259, "self") : (void)0);

    
    self->reset(self);

    
    free(self->m_stack);

    
    free(self);
}

void PI8Stack_push(PI8Stack self,
                   int8_t*  x)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 274, "self") : (void)0);
    (__builtin_expect(!(x), 0) ? __assert_rtn(__func__, "stack.c", 275, "x") : (void)0);


    if(self->isFull(self))
    {
        
        self->m_stack_size  = self->m_stack_size + self->m_stack_size;
        self->m_stack       = (int8_t**) realloc((void*) self->m_stack, sizeof(int8_t*) * self->m_stack_size);

        (__builtin_expect(!(self->m_stack), 0) ? __assert_rtn(__func__, "stack.c", 287, "self->m_stack") : (void)0);
    }

    self->m_stack[self->m_rear] = (int8_t*) str_dup(x);
    self->m_rear                = self->m_rear + 1;

}

int8_t* PI8Stack_pop(PI8Stack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 300, "self") : (void)0);

    int8_t* item = 0;


    if(!(self->isEmpty(self)))
    {
       self->m_rear                 = self->m_rear - 1;
       item                         = self->m_stack[self->m_rear];
       self->m_stack[self->m_rear]  = 0;
    }


    return item;
}

_Bool PI8Stack_isEmpty(PI8Stack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 324, "self") : (void)0);

    _Bool status = 0;


    if(self->m_front > (self->m_rear - 1))
        status = 1;


    return status;
}

_Bool PI8Stack_isFull(PI8Stack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 344, "self") : (void)0);

    _Bool status = 0;


    if(self->m_rear == self->m_stack_size)
        status = 1;


    return status;
}

void PI8Stack_reset(PI8Stack self)
{
    int8_t* item = 0;

    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 366, "self") : (void)0);


    
    while(1)
    {
        item = self->pop(self);

        if(item)
            free(item);
        else
            break;
    }

    self->m_front = 0;
    self->m_rear  = 0;

}

void PI8Stack_rwlock(PI8Stack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 393, "self") : (void)0);
}

void PI8Stack_rwunlock(PI8Stack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 398, "self") : (void)0);
}

struct PI8Stack __PI8Stack; void PI8Stack_init(void) { if(!__PI8Stack_initialised) { PI8Stack_Alloc(&__PI8Stack); __PI8Stack_initialised = 1; } } int32_t __PI8Stack_initialised = 0; void PI8Stack_Alloc(PI8Stack this) { ((Object) this)->__c_class__ = (Object) &__PI8Stack; ((Object) this)->__size = sizeof(struct PI8Stack);
  this->construct    = PI8Stack_construct;
  this->destruct     = PI8Stack_destruct;
  this->reset        = PI8Stack_reset;
  this->isEmpty      = PI8Stack_isEmpty;
  this->isFull       = PI8Stack_isFull;
  this->push         = PI8Stack_push;
  this->pop          = PI8Stack_pop;
  this->rwlock       = PI8Stack_rwlock;
  this->rwunlock     = PI8Stack_rwunlock;
}



 
PVOIDStack PVOIDStack_construct(PVOIDStack  self,
                                size_t      size)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 419, "self") : (void)0);
    (__builtin_expect(!(size), 0) ? __assert_rtn(__func__, "stack.c", 420, "size") : (void)0);


    self->m_stack       = (void**) malloc(sizeof(int8_t*) * size);

    (__builtin_expect(!(self->m_stack), 0) ? __assert_rtn(__func__, "stack.c", 428, "self->m_stack") : (void)0);

    self->m_stack_size  = size;
    self->m_front       = 0;
    self->m_rear        = 0;


    return self;
}

void PVOIDStack_destruct(PVOIDStack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 443, "self") : (void)0);

    
    free(self->m_stack);

    
    free(self);
}

void PVOIDStack_push(PVOIDStack self,
                     void*      x)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 455, "self") : (void)0);
    (__builtin_expect(!(x), 0) ? __assert_rtn(__func__, "stack.c", 456, "x") : (void)0);


    if(self->isFull(self))
    {
        
        self->m_stack_size  = self->m_stack_size + self->m_stack_size;
        self->m_stack       = (void**) realloc((void*) self->m_stack, sizeof(void*) * self->m_stack_size);

        (__builtin_expect(!(self->m_stack), 0) ? __assert_rtn(__func__, "stack.c", 468, "self->m_stack") : (void)0);
    }

    self->m_stack[self->m_rear] = x;
    self->m_rear                = self->m_rear + 1;

}

void* PVOIDStack_pop(PVOIDStack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 481, "self") : (void)0);

    int8_t* item = 0;


    if(!(self->isEmpty(self)))
    {
       self->m_rear                 = self->m_rear - 1;
       item                         = self->m_stack[self->m_rear];
       self->m_stack[self->m_rear]  = 0;
    }


    return item;
}

void* PVOIDStack_peek(PVOIDStack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 505, "self") : (void)0);

    void* item = 0;


    if(!(self->isEmpty(self)))
        item = self->m_stack[self->m_rear - 1];


    return item;
}

_Bool PVOIDStack_isEmpty(PVOIDStack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 525, "self") : (void)0);

    _Bool status = 0;


    if(self->m_front > (self->m_rear - 1))
        status = 1;


    return status;
}

_Bool PVOIDStack_isFull(PVOIDStack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 545, "self") : (void)0);

    _Bool status = 0;


    if(self->m_rear == self->m_stack_size)
        status = 1;


    return status;
}

void PVOIDStack_reset(PVOIDStack    self,
                      _Bool          do_dealloc)
{
    void* item = 0;

    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 568, "self") : (void)0);


    if(do_dealloc)
    {
        
        while(1)
        {
            item = self->pop(self);

            if(item)
                free(item);
            else
                break;
        }
    }

    self->m_front = 0;
    self->m_rear  = 0;

}

void PVOIDStack_rwlock(PVOIDStack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 598, "self") : (void)0);
}

void PVOIDStack_rwunlock(PVOIDStack self)
{
    (__builtin_expect(!(self), 0) ? __assert_rtn(__func__, "stack.c", 603, "self") : (void)0);
}

struct PVOIDStack __PVOIDStack; void PVOIDStack_init(void) { if(!__PVOIDStack_initialised) { PVOIDStack_Alloc(&__PVOIDStack); __PVOIDStack_initialised = 1; } } int32_t __PVOIDStack_initialised = 0; void PVOIDStack_Alloc(PVOIDStack this) { ((Object) this)->__c_class__ = (Object) &__PVOIDStack; ((Object) this)->__size = sizeof(struct PVOIDStack);
  this->construct    = PVOIDStack_construct;
  this->destruct     = PVOIDStack_destruct;
  this->reset        = PVOIDStack_reset;
  this->isEmpty      = PVOIDStack_isEmpty;
  this->isFull       = PVOIDStack_isFull;
  this->push         = PVOIDStack_push;
  this->pop          = PVOIDStack_pop;
  this->peek         = PVOIDStack_peek;
  this->rwlock       = PVOIDStack_rwlock;
  this->rwunlock     = PVOIDStack_rwunlock;
}




 

 

