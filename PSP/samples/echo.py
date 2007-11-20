#-*- coding: ISO-8859-1 -*-

import asyncore, asynchat, socket, StringIO, threading
import pspnet, psp2d

class Echoer(asynchat.async_chat):
    def __init__(self, *args, **kwargs):
        asynchat.async_chat.__init__(self, *args, **kwargs)

        self.__data = StringIO.StringIO()
        self.set_terminator('\r\n')

    def data(self):
        r = self.__data.getvalue()
        self.__data = StringIO.StringIO()
        return r

    def collect_incoming_data(self, d):
        self.__data.write(d)

    def found_terminator(self):
        msg = self.data()
        self.push('Did you say "%s" ?\r\n' % msg)
        self.set_terminator('\r\n')

class Acceptor(asyncore.dispatcher):
    def handle_accept(self):
        fd, addr = self.accept()
        Echoer(fd)

def runloop():
    try:
        asyncore.loop()
    except:
        import traceback
        traceback.print_exc(file = file('trace.txt', 'a+'))

class Connector(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)

        self.msg = ''
        self.connected = False

    def run(self):
        pspnet.connectToAPCTL(1, self.callback, 10)

    def callback(self, state):
        print 'STATE', state

        if state >= 0:
            self.msg = 'State: %d/4' % state
        else:
            self.msg = 'Connected.'
            self.connected = True

def main():
    screen = psp2d.Screen()
    font = psp2d.Font('font.png')
    bgimg = psp2d.Image('background.png')

    screen.blit(bgimg)
    font.drawText(screen, 0, 0, 'Connecting...')
    screen.swap()

##     def cb(s):
##         print 'STATE', s
##         screen.blit(bgimg)
##         if s >= 0:
##             font.drawText(screen, 0, 0, 'State: %d/4' % s)
##         else:
##             font.drawText(screen, 0, 0, 'Connected. IP: %s' % pspnet.getIP())
##         screen.swap()

##     pspnet.connectToAPCTL(1, cb, 10)

    cn = Connector()
    cn.start()

    while not cn.connected:
        screen.blit(bgimg)
        font.drawText(screen, 0, 0, cn.msg)
        screen.swap()

    screen.blit(bgimg)
    font.drawText(screen, 0, 0, cn.msg)
    screen.swap()

    a = Acceptor()
    a.create_socket(socket.AF_INET, socket.SOCK_STREAM)
    a.bind(('', 23))
    a.listen(5)

    thr = threading.Thread(target = runloop)
    thr.setDaemon(True)
    thr.start()

    while True:
        pad = psp2d.Controller()
        if pad.circle:
            break

if __name__ == '__main__':
    main()
