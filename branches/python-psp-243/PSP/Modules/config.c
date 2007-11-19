
/**
 * @file config.c
 */

/**********************************************************************

  Created: 02 Oct 2005

**********************************************************************/
// $Id: config.c 1817 2006-03-05 16:49:09Z fraca7 $

#include "Python.h"

extern void PyMarshal_Init(void);
extern void initimp(void);
extern void initgc(void);
extern void initswi(void);

extern void inittime(void);
extern void initmath(void);
extern void initstruct(void);
extern void initbinascii(void);
extern void init_random(void);
extern void init_sre(void);
extern void initcStringIO(void);
extern void initerrno(void);
extern void initthread(void);
extern void initcollections(void);
extern void initmd5(void);
extern void init_socket(void);
extern void initselect(void);
extern void initzipimport(void);
extern void initsha(void);
extern void init_codecs(void);
extern void initdatetime(void);
extern void init_bisect(void);
extern void init_csv(void);
extern void init_heapq(void);
extern void init_weakref(void);
extern void initarray(void);
extern void initaudioop(void);
extern void initcPickle(void);
extern void initcmath(void);
extern void inititertools(void);
extern void initoperator(void);
extern void initparser(void);
extern void init_symtable(void);
extern void inittiming(void);
extern void initunicodedata(void);
extern void initzlib(void);

extern void initpspos(void);

#ifdef WITH_PSP2D
extern void initpsp2d(void);
#endif

#ifdef WITH_PSPSND
extern void initpspsnd(void);
#endif

#ifdef WITH_PSPNET
extern void initpspnet(void);
#endif

#ifdef WITH_OSLIB
extern void initosl(void);
#endif

#ifdef WITH_SQLITE
extern void init_sqlite(void);
#endif

struct _inittab _PyImport_Inittab[] = {
	{"marshal", PyMarshal_Init},
	{"imp", initimp},

	{"__main__", NULL},
	{"__builtin__", NULL},

	{"sys", NULL},
	{"exceptions", NULL},

	{"gc", initgc},

        {"time", inittime},
        {"math", initmath},
        {"struct", initstruct},
        {"binascii", initbinascii},
        {"_random", init_random},
        {"_sre", init_sre},
        {"cStringIO", initcStringIO},
        {"errno", initerrno},
        {"thread", initthread},
        {"collections", initcollections},
        {"md5", initmd5 },
        {"_socket", init_socket},
        {"select", initselect},
        {"zipimport", initzipimport},
        {"sha", initsha},
        {"_codecs", init_codecs},
        {"datetime", initdatetime},
        {"_bisect", init_bisect},
        {"_csv", init_csv},
        {"_heapq", init_heapq},
        {"_weakref", init_weakref},
        {"array", initarray},
        {"audioop", initaudioop},
        {"cPickle", initcPickle},
        {"cmath", initcmath},
        {"itertools", inititertools},
        {"operator", initoperator},
        {"parser", initparser},
        {"_symtable", init_symtable},
        {"timing", inittiming},
        {"unicodedata", initunicodedata},
        {"zlib", initzlib},

        {"pspos", initpspos},

#ifdef WITH_PSP2D
        {"psp2d", initpsp2d},
#endif

#ifdef WITH_PSPSND
        {"pspsnd", initpspsnd},
#endif

#ifdef WITH_PSPNET
        {"pspnet", initpspnet},
#endif

#ifdef WITH_OSLIB
        {"osl", initosl},
#endif

#ifdef WITH_SQLITE
        {"_sqlite", init_sqlite},
#endif

	{0, 0}
};

#ifdef _GNUC
static const char* _rcsid_config __attribute__((unused)) = "$Id: config.c 1817 2006-03-05 16:49:09Z fraca7 $";
#endif
