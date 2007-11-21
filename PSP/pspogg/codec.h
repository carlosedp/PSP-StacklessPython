#ifndef _CODEC_H_
#define _CODEC_H_

typedef void (*fd_initFunc) (int);
typedef int (*fd_loadFunc) (char *);
typedef int (*fd_playFunc) (void);
typedef void (*fd_pauseFunc) (void);
typedef int (*fd_stopFunc) (void);
typedef void (*fd_endFunc) (void);
typedef void (*fd_tickFunc) (void);
typedef void (*fd_timeFunc) (char *);
typedef int (*fd_EOSFunc) (void);

typedef struct {
    fd_initFunc init;
    fd_loadFunc load;
    fd_playFunc play;
    fd_pauseFunc pause;
    fd_stopFunc stop;
    fd_endFunc end;
    fd_tickFunc tick;
    fd_timeFunc time;
    fd_EOSFunc eos;		// have we reached end of playback.. ie for non-loop mode
    char extension[200];	// 4 byte entries. eg   mp3\0  or \0\0\0\0 to terminate list
} codecStubs;

#endif
