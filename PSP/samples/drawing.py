#-*- coding: ISO-8859-1 -*-

import psp2d, math

def main():
    screen = psp2d.Screen()

    screen.fillRect(10, 10, 60, 30, psp2d.Color(255, 0, 0))
    screen.drawText(15, 45, 'Hello, world!', psp2d.Color(0, 0, 255))
    screen.drawLine(0, 200, 200, 0, psp2d.Color(0, 255, 0))

    screen.swap()

    while True:
        pad = psp2d.Controller()
        if pad.circle:
            break

if __name__ == '__main__':
    main()
