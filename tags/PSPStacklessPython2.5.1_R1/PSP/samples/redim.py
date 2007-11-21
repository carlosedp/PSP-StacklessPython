#-*- coding: ISO-8859-1 -*-

__rcsid__ = '$Id: redim.py 1549 2005-12-09 12:27:01Z fraca7 $'

import psp2d

def main():
    bg = psp2d.Image('background.png')
    img = psp2d.Image('sprite.png')

    scr = psp2d.Screen()
    scr.blit(img, dx = 0, dy = 0, blend = True)
    scr.blit(img, dx = 240, dy = 136, dw = 128, dh = 128, blend = True)
    scr.blit(img, dx = 0, dy = 136, dw = 32, blend = True)
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
