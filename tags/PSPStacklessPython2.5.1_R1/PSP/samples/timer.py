#-*- coding: ISO-8859-1 -*-

__rcsid__ = '$Id: timer.py 1469 2005-11-20 13:04:05Z fraca7 $'

import psp2d

class Animation(psp2d.Timer):
    def __init__(self):
        psp2d.Timer.__init__(self, 1000 / 25) # 25 fps

        self.img = psp2d.Image('sprite.png')
        self.x = 0
        self.y = 200
        self.u = 2

    def fire(self):
        scr = psp2d.Screen()

        scr.clear(psp2d.Color(0, 0, 0))
        scr.blit(self.img, dx = self.x, dy = self.y, blend = True)

        scr.swap()

        self.x += self.u
        if self.x <= 0 or self.x + self.img.width >= 480:
            self.u = -self.u

        pad = psp2d.Controller()
        return not pad.circle

def main():
    a = Animation()
    a.run()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
    except:
        import traceback
        traceback.print_exc(file = file('trace.txt', 'w'))
