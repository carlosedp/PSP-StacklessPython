#-*- coding: ISO-8859-1 -*-

import pspnet, psp2d
import os, threading, urlparse, time

from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler

class MSHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        try:
            path = 'ms0:' + urlparse.urlparse(self.path)[2]
            file('log.txt', 'a+').write('Request: %s (%s)\n' % (self.path, path))
            if path != 'ms0:/' and not os.path.exists(path):
                self.send_response(404)
                self.end_headers()
            elif os.path.isdir(path) or path == 'ms0:/':
                self.send_response(200)
                html  = '<html><head><title>Directory listing for %s</title></head><body>' % path
                html += '<h1>Directories</h1><ul>'
                for name in os.listdir(path):
                    if name != '.':
                        fname = os.path.join(path, name)
                        if os.path.isdir(fname):
                            html += '<li><a href="%s">%s</a></li>' % (fname[4:], name)
                html += '</ul><h1>Files</h1><ul>'
                for name in os.listdir(path):
                    fname = os.path.join(path, name)
                    if os.path.isfile(fname):
                        html += '<li><a href="%s">%s</a></li>' % (fname[4:], name)

                self.send_header('Content-Type', 'text/html')
                self.send_header('Content-Length', str(len(html)))
                self.end_headers()
                self.wfile.write(html)
            elif os.path.isfile(path):
                sz = os.path.getsize(path)
                self.send_response(200)
                self.send_header('Content-Type', 'octet/stream') # TODO: guess
                self.send_header('Content-Length', str(sz))
                self.end_headers()

                fp = file(path, 'rb')
                bf = fp.read(4096)
                while bf:
                    self.wfile.write(bf)
                    bf = fp.read(4096)
                fp.close()
            else:
                self.send_response(400)
                html  = '<html><head><title>Error</title></head><body>'
                html += 'Bad request (%s)' % path
                self.send_header('Content-Type', 'text/html')
                self.send_header('Content-Length', str(len(html)))
                self.end_headers()
                self.wfile.write(html)
        except:
            import traceback
            traceback.print_exc(file = file('trace.txt', 'a+'))

    def log_message(self, fmt, *args):
        file('messages.txt', 'a+').write((fmt % args) + '\n')

class MSServer(HTTPServer):
    allow_reuse_address = True

def main():
    screen = psp2d.Screen()
    font = psp2d.Font('font.png')
    bgimg = psp2d.Image('background.png')

    screen.blit(bgimg)
    font.drawText(screen, 0, 0, 'Connecting...')
    screen.swap()

    def cb(s):
        screen.blit(bgimg)
        if s >= 0:
            font.drawText(screen, 0, 0, 'State: %d/4' % s)
        else:
            font.drawText(screen, 0, 0, 'Connected. IP: %s' % pspnet.getIP())
        screen.swap()

    pspnet.connectToAPCTL(1, cb)

    srv = MSServer(('', 80), MSHandler)
    thr = threading.Thread(target = srv.serve_forever)
    thr.setDaemon(True)
    thr.start()

    ip = pspnet.getIP()

    while True:
        pad = psp2d.Controller()
        if pad.circle:
            break

        screen.blit(bgimg)
        font.drawText(screen, 0, 0, 'IP: %s' % ip)
        font.drawText(screen, 0, 100, 'Time: %.2f' % time.time())
        screen.swap()

    pspnet.disconnectAPCTL()

if __name__ == '__main__':
    main()
