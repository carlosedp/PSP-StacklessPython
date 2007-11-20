# -*- coding: ISO-8859-1 -*-

import osl, maps

class MapsDemo(object):
    def __init__(self):
        osl.initGfx()
        osl.initConsole()
        osl.initAudio()

        self.maps = []

        for name in ['nuages', 'grassland', 'nuage1', 'nuage2', 'montagne',
                     'niveau', 'decors', 'sol']:
            img = osl.Image(name + '.png', osl.IN_RAM, osl.PF_5551)

            # TODO: swizzle

            self.maps.append(osl.Map(img, getattr(maps, name + '_map'), 8, 8))

            self.music = osl.Sound('music.bgm', osl.FMT_STREAM)
            self.jump = osl.Sound('jump.wav')
            self.key = osl.Sound('key.wav')

            self.music.loop()

            self.layers = 8

    def run(self):
        while not osl.mustQuit():
            osl.startDrawing()

            osl.drawGradientRect(0, 0, 480, 272,
                                 osl.RGB(0, 0, 128),
                                 osl.RGB(0, 0, 128),
                                 osl.RGB(0, 255, 255),
                                 osl.RGB(0, 255, 255))

            self.keys()

            for i in xrange(8):
                if self.layers > i:
                    self.maps[i].drawSimple()

            self.text()

            osl.endDrawing()
            osl.syncFrame()
            osl.audioVSync()

        osl.endGfx()
        osl.doQuit()

    def keys(self):
        ctrl = osl.Controller()

        if ctrl.pressed_down and self.layers > 0:
            self.layers -= 1

        if ctrl.pressed_up and self.layers < 8:
            self.layers += 1

        if ctrl.pressed_left:
            self.layers = 0

        if ctrl.pressed_right:
            self.layers = 8

        for i in xrange(32, 121, 48):
            if ctrl.analogX > i:
                for j in xrange(8):
                    self.maps[j].scrollX += j + 1

            if ctrl.analogX < -i:
                for j in xrange(8):
                    self.maps[j].scrollX -= j + 1

        if ctrl.pressed_square:
            self.music.play(-1)

        if ctrl.pressed_circle:
            self.key.play(1)

        if ctrl.pressed_cross:
            self.jump.play(2)

        if ctrl.pressed_triangle:
            self.music.stop()

        if ctrl.pressed_R:
            self.music.play(0)

        if ctrl.pressed_start:
            osl.doQuit()

    def text(self):
        osl.setTextColor(osl.RGBA(0, 0, 0, 255));
        osl.setBkColor(osl.RGBA(0, 0, 0, 0));

        osl.printxy(1, 5, 'OSLib Maps & Sound sample');
        osl.printxy(1, 25, 'Left/right: display maps');
        osl.printxy(1, 35, 'Up/down: more/less maps');
        osl.printxy(1, 45, 'Joystick: scroll maps');
        osl.printxy(1, 55, 'R: play music');
        osl.printxy(1, 65, '[]: pause music');
        osl.printxy(1, 75, '^: stop music');
        osl.printxy(1, 85, 'X & (): play sounds');
        osl.printxy(1, 105, 'Start: quit');

if __name__ == '__main__':
    demo = MapsDemo()
    demo.run()
