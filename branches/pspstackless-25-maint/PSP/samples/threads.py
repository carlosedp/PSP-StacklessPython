#-*- coding: ISO-8859-1 -*-

import psp, threading, time

class NumberedThread(threading.Thread):
    def __init__(self, n, lock, scr):
        self.n = n
        self.lock = lock
        self.scr = scr

        threading.Thread.__init__(self)

    def run(self):
        for i in xrange(10):
            self.scr.printf('Hello, thread %d here\n', self.n)
            time.sleep(1)

def main():
    scr = psp.debug.DebugScreen()

    # We use a lock so that output doesn't get mangled
    lock = threading.Lock()

    thr1 = NumberedThread(1, lock, scr)
    thr2 = NumberedThread(2, lock, scr)

    thr1.start()
    thr2.start()

    thr1.join()
    thr2.join()

    scr.printf('==== FINISHED\n')
    time.sleep(2)

if __name__ == '__main__':
    main()
