#-*- coding: ISO-8859-1 -*-

import psp
import sys
import time

class PSPOutput(psp.debug.DebugScreen):
    def __init__(self):
        self.fd = file('output.txt', 'w')

        psp.debug.DebugScreen.__init__(self)

    def write(self, bf):
        self.printf(bf)
        self.fd.write(bf)

    def close(self):
        pass

    def flush(self):
        pass

sys.stdout = sys.stderr = PSPOutput()

def testit():
    print '===== Starting.'

    print 'Testing sys'

    print 'sys.path=%s' % str(sys.path)
    print 'sys.platform=%s' % sys.platform

    import os

    print 'Testing os'

    os.mkdir('ms0:/foo')

    if os.path.exists('ms0:/foo'):
        print 'exists OK'
    else:
        print 'exists KO'

    if os.path.isdir('ms0:/foo'):
        print 'isdir OK'
    else:
        print 'isdir KO'

    if os.path.isfile('ms0:/foo'):
        print 'isfile KO'
    else:
        print 'isfile OK'

    print 'os.listdir(ms0:/):'
    print os.listdir('ms0:/')

    print '===== Finished.'
    time.sleep(5)

if __name__ == '__main__':
    testit()
