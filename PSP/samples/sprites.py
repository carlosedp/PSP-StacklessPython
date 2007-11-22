#-*- coding: ISO-8859-1 -*-

__rcsid__ = '$Id: sprites.py 1470 2005-11-20 13:04:29Z fraca7 $'

import psp2d
import math, random, time

class Sprite:
    def __init__(self, img):
        x = 1.0 * random.randrange(0, 480 - img.width, 1)
        y = 1.0 * random.randrange(0, 272 - img.height, 1)

        self.data = img, x, y
        self.w = img.width
        self.h = img.height

        self.u = random.random() * 3 - 1.5, random.random() * 3 - 1.5

    def nextFrame(self):
        x = self.data[1] + self.u[0]
        y = self.data[2] + self.u[1]

        if x + self.w >= 480 or x <= 0:
            self.u = -self.u[0], self.u[1]

        if y + self.h >= 272 or y <= 0:
            self.u = self.u[0], -self.u[1]

        self.data = self.data[0], x, y

def main():
    img = psp2d.Image('sprite.png')
    sprites = [ Sprite(img) for unused in xrange(30) ]
    screen = psp2d.Screen()

    batch = psp2d.BlitBatch()
    for sprite in sprites:
        batch.add(sprite)

    batching = False

    while True:
        if batching:
            while True:
                screen.clear(psp2d.Color(0, 0, 0))

                for sprite in sprites:
                    sprite.nextFrame()
                batch.blit()

                screen.swap()

                pad = psp2d.Controller()
                if pad.cross:
                    batching = False
                    time.sleep(0.2)
                    break
        else:
            while True:
                screen.clear(psp2d.Color(0, 0, 0))

                for sprite in sprites:
                    sprite.nextFrame()
                    img, x, y = sprite.data
                    screen.blit(img, dx = x, dy = y, blend = True)

                screen.swap()

                pad = psp2d.Controller()
                if pad.cross:
                    batching = True
                    time.sleep(0.2)
                    break

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
    except:
        import traceback
        traceback.print_exc(file = file('trace.txt', 'w'))
