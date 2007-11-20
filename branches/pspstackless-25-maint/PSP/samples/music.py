#-*- coding: ISO-8859-1 -*-

__rcsid__ = '$Id: music.py 1470 2005-11-20 13:04:29Z fraca7 $'

try:
    import pspsnd
    import time

    m = pspsnd.Music('stranglehold.xm')
    m.start()

    time.sleep(10)
except Exception, e:
    file('trace.txt', 'w').write('%s - %s\n' % (e.__class__.__name__, str(e)))
