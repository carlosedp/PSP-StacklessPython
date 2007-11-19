#-*- coding: ISO-8859-1 -*-

import psp2d

def getName():
    return 'Controller'

def getDescription():
    return 'Simple psp2d.Controller demo'

def main():
    scr = psp2d.Screen()

    fnt = psp2d.Font('font_small.png')
    dy = fnt.textHeight('') + 5

    while True:
        pad = psp2d.Controller()

        img = psp2d.Image(480, 272)
        img.clear(psp2d.Color(0, 0, 0))

        fnt.drawText(img, 0, 0, 'Analog X: %d' % pad.analogX)
        fnt.drawText(img, 0, dy, 'Analog Y: %d' % pad.analogY)
        fnt.drawText(img, 0, 2 * dy, 'Square: %d' % int(pad.square))
        fnt.drawText(img, 0, 3 * dy, 'Circle: %d' % int(pad.circle))
        fnt.drawText(img, 0, 4 * dy, 'Cross: %d' % int(pad.cross))
        fnt.drawText(img, 0, 5 * dy, 'Triangle: %d' % int(pad.triangle))
        fnt.drawText(img, 0, 6 * dy, 'Left: %d' % int(pad.left))
        fnt.drawText(img, 0, 7 * dy, 'Right: %d' % int(pad.right))
        fnt.drawText(img, 0, 8 * dy, 'Up: %d' % int(pad.up))
        fnt.drawText(img, 0, 9 * dy, 'Down: %d' % int(pad.down))
        fnt.drawText(img, 0, 10 * dy, 'Left trigger: %d' % int(pad.l))
        fnt.drawText(img, 0, 11 * dy, 'Right trigger: %d' % int(pad.r))

        scr.blit(img)
        scr.swap()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
    except:
        import traceback
        traceback.print_exc(file = file('trace.txt', 'w'))
