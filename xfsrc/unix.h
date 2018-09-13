#ifndef _UNIX_H
#define _UNIX_H

#if !defined(UNIX)

extern char *ttyname(int fd);
extern char *uname(void);
extern int REMOTEFILE(int st_dev);
extern int alarm(void);
extern int pause(void );
extern int wait(int *stat_loc);
extern int32_t getuid(void);
extern unsigned int valid_fd(int fd);
#if !AP_BUILD
extern intptr_t LIBCALL execv(const char *path, const char *const *argv);
extern intptr_t LIBCALL execl(const char *path, const char *arg,...);
#endif
extern void force_user_io(int fd);
extern void fork(void);
extern int fsync(int fd);
extern void kill(int pid,int sig);
extern void slave(int *fd,UCH *path,UCH *cmd,UCH **params);
extern void sync(void );
extern void times(void);

extern void binary_stream(FILE *stream);
extern void text_stream(FILE *stream);
extern void binary_file(int fd);
extern void text_file(int fd);

#endif 	// !defined(UNIX)

extern int isadisk(int fd);
extern int is_user_io(int fd);

#endif // _UNIX_H
