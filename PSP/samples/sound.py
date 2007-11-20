#-*- coding: ISO-8859-1 -*-

__rcsid__ = '$Id: sound.py 1470 2005-11-20 13:04:29Z fraca7 $'

try:
    import time, os, pspsnd

    name = 'explode.wav'

    if not os.path.exists(name):
        raise IOError('File does not exist')

    s = pspsnd.Sound(name)

    time.sleep(5)
    s.start()
    time.sleep(5)
except Exception, e:
    file('trace.txt', 'w').write('%s - %s\n' % (e.__class__.__name__, str(e)))
