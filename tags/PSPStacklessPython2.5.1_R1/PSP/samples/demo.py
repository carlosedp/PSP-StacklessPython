#-*- coding: ISO-8859-1 -*-

__rcsid__ = '$Id: demo.py 1470 2005-11-20 13:04:29Z fraca7 $'

import psp2d, math

def main(msgs):
    screen = psp2d.Screen()
    bgimg = psp2d.Image('background.png')
    font = psp2d.Font('font.png')

    widths = []
    totalw = []
    for msg in msgs:
        widths.append([])
        totalw.append(0)
        for c in msg:
            widths[-1].append(font.textWidth(c))
            totalw[-1] += font.textWidth(c)

    y = (272 - font.textHeight('')) / 2
    xs = []
    for tw in totalw:
        xs.append((480 - tw)/2)

    angle = 0.0

    while True:
        screen.blit(bgimg)
        dy = 0
        for i, msg in enumerate(msgs):
            dx = 0
            for idx, c in enumerate(msg):
                font.drawText(screen, xs[i] + dx, y + dy + 20 * math.sin(angle + 0.2 * idx), c)
                dx += widths[i][idx]
            dy += font.textHeight('') + 20

        screen.swap()
        angle += 0.2

        pad = psp2d.Controller()
        if pad.circle:
            break

if __name__ == '__main__':
    try:
        main(['NOBODY EXPECTS', 'THE SPANISH INQUISITION!'])
    except KeyboardInterrupt:
        pass
    except:
        import traceback
        traceback.print_exc(file = file('trace.txt', 'w'))
