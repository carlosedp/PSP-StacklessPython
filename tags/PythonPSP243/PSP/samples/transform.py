#-*- coding: ISO-8859-1 -*-

__rcsid__ = '$Id: transform.py 1470 2005-11-20 13:04:29Z fraca7 $'

import psp2d

def main():
    scr = psp2d.Screen()
    img = psp2d.Image('background.png')

    simg1 = psp2d.Image(160, 272)
    simg1.blit(0, 0, 160, 272, img, 0, 0)

    simg2 = psp2d.Image(160, 272)
    simg2.blit(320, 0, 160, 272, img, 0, 0)

    tr = psp2d.Transform(psp2d.TR_PLUS, 20.0)
    tr.apply(simg1)
    img.blit(0, 0, 160, 272, simg1, 0, 0)

    tr = psp2d.Transform(psp2d.TR_MULT, 0.5)
    tr.apply(simg2)
    img.blit(0, 0, 160, 272, simg2, 320, 0)

    scr.blit(0, 0, img.width, img.height, img, 0, 0)
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
