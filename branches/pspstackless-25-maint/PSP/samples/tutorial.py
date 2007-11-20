#-*- coding: ISO-8859-1 -*-

import psp2d, pspsnd, math

def getName():
    return 'Tutorial'

def getDescription():
    return 'Full script of the python-PSP tutorial.'

def main(msg='Hello, world!'):
    screen = psp2d.Screen()
    bgimg = psp2d.Image('background.png')
    font = psp2d.Font('font.png')

##     pspsnd.setMusicVolume(128)
##     music = pspsnd.Music('stranglehold.xm', loop = True)
##     music.start()

    widths = []
    totalw = 0
    for c in msg:
        widths.append(font.textWidth(c))
        totalw += font.textWidth(c)

    y = (272 - font.textHeight(msg)) / 2
    x = (480 - totalw) / 2

    angle = 0.0

    menu = Menu(10, 10)

    amplitude = FloatMenuItem(20, 0.5, 'Amplitude')
    step = FloatMenuItem(0.2, 0.02, 'Step')
    slope = FloatMenuItem(0.2, 0.02, 'Slope')

    menu.addItem(amplitude)
    menu.addItem(step)
    menu.addItem(slope)

    menu.realize(font)

    while True:
	screen.blit(bgimg)
        menu.draw(font, screen)

        dx = 0
        for idx, c in enumerate(msg):
            font.drawText(screen, x + dx, y + amplitude.value * math.sin(angle + slope.value * idx), c)
            dx += widths[idx]

        screen.swap()
        angle += step.value

        pad = psp2d.Controller()
        if pad.circle:
            break

        menu.update(pad)

class MenuItem:
    """Abstract base class for menu items"""
    def __init__(self, name):
        """Each menu item has a name. It may be used as a label."""
        self.name = name

    def getDimension(self, font):
        """This method should return a 2-tuple (w, h) holding the preferred
        width and height for this menu item."""
        raise NotImplementedError

    def draw(self, font, drw, x, y, w, h):
        """Override this to do the actual drawing. You should draw on the 'drw'
        drawable, int the (x, y, w, h) rectangle, using the 'font' font."""
        raise NotImplementedError

    def onLeft(self):
        """Will be called if the user presses the Left button when this menu
        item is active"""
        pass

    def onRight(self):
        """Same as onLeft for the Right button"""
        pass

class Menu:
    def __init__(self, x, y):
        """The menu will be displayed at coordinates (x, y). Initially empty.
        Default selection is item #0."""
        self.x = x
        self.y = y

        self.w = 0
        self.h = 0

        self.items = []

        self.abg = None
        self.bg = None

        self.current = 0

        self.old = None

    def addItem(self, item):
        """Add an item to this menu."""
        self.items.append(item)

    def realize(self, font):
        """You should call this when you're finished adding items to the menu."""

        self.w = 0
        self.h = 0

        for item in self.items:
            w, h = item.getDimension(font)
            w += 20
            h += 10

            self.w = max(self.w, w)
            self.h = max(self.h, h)

        self.abg = psp2d.Image(self.w, self.h)
        self.abg.clear(psp2d.Color(0, 0, 255, 200))

        self.bg = psp2d.Image(self.w, self.h)
        self.bg.clear(psp2d.Color(0, 0, 255, 100))

    def draw(self, font, drw):
        cy = self.y
        for idx, item in enumerate(self.items):
            if idx == self.current:
                bg = self.abg
            else:
                bg = self.bg

            drw.blit(bg, dx = self.x, dy = cy, blend = True)
            item.draw(font, drw, self.x, cy, self.w, self.h)
            cy += self.h

    def update(self, pad):
        """The main loop should call this method to allow the menu to
        react to user actions. 'pad' should be a Controller instance."""

        if pad == self.old:
            return
        self.old = pad

        if pad.left:
            self.items[self.current].onLeft()

        if pad.right:
            self.items[self.current].onRight()

        if pad.up:
            self.current = (self.current - 1 + len(self.items)) % len(self.items)

        if pad.down:
            self.current = (self.current + 1) % len(self.items)

class FloatMenuItem(MenuItem):
    def __init__(self, value, step, name):
        MenuItem.__init__(self, name)

        self.value = value
        self.step = step

    def getDimension(self, font):
        txt = '%s: %.2f' % (self.name, self.value)
        return (font.textWidth(txt), font.textHeight(txt))

    def draw(self, font, drw, x, y, w, h):
        txt = '%s: %.2f' % (self.name, self.value)
        font.drawText(drw,
                      x + (w - font.textWidth(txt))/2,
                      y + (h - font.textHeight(txt))/2,
                      txt)

    def onLeft(self):
        self.value -= self.step

    def onRight(self):
        self.value += self.step

if __name__ == '__main__':
    main('Hello, world!')
