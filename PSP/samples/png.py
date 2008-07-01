#-*- coding: ISO-8859-1 -*-

__rcsid__ = '$Id: png.py 1470 2005-11-20 13:04:29Z fraca7 $'

import psp2d, time

def main():
    scr = psp2d.Screen()

    img = psp2d.Image('background.png')

    img2 = psp2d.Image(200, 200)
    img2.clear(psp2d.Color(255, 0, 0, 128))
    img.blit(img2, dx = 30, dy = 30, blend = True)

    scr.blit(img)
    scr.swap()

    while True:
        pad = psp2d.Controller()
        if pad.circle:
            break

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
    except:
        import traceback
        traceback.print_exc(file = file('trace.txt', 'w'))
