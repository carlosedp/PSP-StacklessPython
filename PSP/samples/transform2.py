#-*- coding: ISO-8859-1 -*-

__rcsid__ = '$Id: transform2.py 1470 2005-11-20 13:04:29Z fraca7 $'

import psp2d

def main():
    scr = psp2d.Screen()
    img = psp2d.Image('background.png')

    def change(x, y, color):
        color.red = min(255, color.red + 20)
        color.green = min(255, color.green + 20)
        color.blue = min(255, color.blue + 20)

        return True

    simg = psp2d.Image(20, 20)
    simg.blit(100, 100, 20, 20, img, 0, 0)
    tr = psp2d.Transform(change)
    tr.apply(simg)
    img.blit(0, 0, 20, 20, simg, 100, 100)

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
