# -*- coding: ISO-8859-1 -*-

import osl

class SpriteDemo(object):
    def __init__(self):
        osl.initGfx()
        osl.initConsole()

        self.fond = osl.Image('fond.png', osl.IN_RAM, osl.PF_5551)
        self.mickey = osl.Image('mickey.png', osl.IN_RAM, osl.PF_5551)

        self.mickey.correctHalfBorder()

        self.mickey.centerX = self.mickey.sizeX // 2
        self.mickey.centerY = self.mickey.sizeY // 2

        self.mickey.x = 240
        self.mickey.y = 136

    def run(self):
        while not osl.mustQuit():
            osl.startDrawing()

            self.keys()

            self.fond.draw()

            self.text()

            self.mickey.draw()

            osl.endDrawing()
            osl.syncFrame()

        osl.endGfx()
        osl.quit()

    def keys(self):
        ctrl = osl.Controller()

        if ctrl.held_down:
            self.mickey.y += 2

        if ctrl.held_up:
            self.mickey.y -= 2

        if ctrl.held_left:
            self.mickey.x -= 2

        if ctrl.held_right:
            self.mickey.x += 2

        for i in xrange(24, 121, 16):
            if ctrl.analogX > i:
                self.mickey.stretchX += 1

            if ctrl.analogY > i:
                self.mickey.stretchY += 1

            if ctrl.analogX < -i:
                self.mickey.stretchX -= 1

            if ctrl.analogY < -i:
                self.mickey.stretchY -= 1

        if ctrl.held_square:
            self.mickey.stretchX += 1
            self.mickey.stretchY += 1

        if ctrl.held_circle:
            self.mickey.stretchX -= 1
            self.mickey.stretchY -= 1

        if ctrl.pressed_cross:
            osl.setBilinearFilter(1)

        if ctrl.pressed_triangle:
            osl.setBilinearFilter(0)

        if ctrl.held_R:
            self.mickey.angle -= 2

        if ctrl.held_L:
            self.mickey.angle += 2

        if ctrl.pressed_start:
            osl.doQuit()

    def text(self):
        osl.setTextColor(osl.RGBA(0, 0, 0, 255))
        osl.setBkColor(osl.RGBA(0, 0, 0, 0))

        osl.printxy(1, 5, 'OSLib sprite sample')
        osl.printxy(1, 25, 'Joypad to move Mickey')
        osl.printxy(1, 35, 'Joystick to deform Mickey')
        osl.printxy(1, 45, '[] & () to zoom Mickey')
        osl.printxy(2, 55, 'X & ^ to smooth')
        osl.printxy(1, 65, 'L & R to rotate Mickey')
        osl.printxy(1, 75, 'Start to quit')

if __name__ == '__main__':
    demo = SpriteDemo()
    demo.run()
