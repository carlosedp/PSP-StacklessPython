#-*- coding: ISO-8859-1 -*-

import operator, weakref, os, imp

import psp2d


#==============================================================================
#

class BasicClearer(object):
    def __init__(self):
        self.count = 0

    def Clear(self, img, focused):
        if focused:
            self.count = (self.count + 8) % 256
            if self.count <= 127:
                img.clear(psp2d.Color(0, 0, 255,  128 + self.count))
            else:
                img.clear(psp2d.Color(0, 0, 255, 255 - (self.count - 128)))
        else:
            img.clear(psp2d.Color(0, 0, 0, 200))


#==============================================================================
#

class Widget(object):
    def __init__(self, clearer=None):
        self.observers = {}

        if clearer is None:
            self.clearer = BasicClearer()
        else:
            self.clearer = clearer

    def connect(self, sig, cb):
        ls = self.observers.get(sig, [])
        ls.append(cb)
        self.observers[sig] = ls

    def signal(self, sig, *args, **kwargs):
        for cb in self.observers.get(sig, []):
            cb(*args, **kwargs)

    def GetMinSize(self):
        raise NotImplementedError

    def Paint(self, drw, x, y, w, h, focused):
        img = psp2d.Image(w, h)
        self.clearer.Clear(img, focused)
        return img

    def OnKey(self, pad):
        raise NotImplementedError


#==============================================================================
#

HORIZONTAL   = 0
VERTICAL     = 1

class BoxSizer(Widget):
    EXPAND       = 1
    ALIGN_CENTER = 2
    ALIGN_LEFT   = 4
    ALIGN_RIGHT  = 8
    ALIGN_TOP    = 16
    ALIGN_BOTTOM = 32

    def __init__(self, clearer=None, direction=0, border=0):
        Widget.__init__(self, clearer)

        self.direction = direction
        self.border = border

        self.widgets = []
        self.focus = 0

    def Add(self, widget, stretch=0, flags=0):
        self.widgets.append((widget, stretch, flags))

    def GetMinSize(self):
        minW = 0
        minH = 0

        for widget, stretch, flags in self.widgets:
            w, h = widget.GetMinSize()

            if self.direction == HORIZONTAL:
                minH = max(minH, h)
                minW += w
            else:
                minW = max(minW, w)
                minH += h

        if self.direction == HORIZONTAL:
            minW += self.border * (len(self.widgets) + 1)
            minH += self.border * 2
        else:
            minH += self.border * (len(self.widgets) + 1)
            minW += self.border * 2

        return minW, minH

    def Paint(self, drw, x, y, w, h, focused):
        img = psp2d.Image(w, h)
        img.clear(psp2d.Color(0, 0, 0, 0))

        totalStretch = reduce(operator.add, [k[1] for k in self.widgets], 0)

        minW, minH = self.GetMinSize()
        remainW = w - minW
        remainH = h - minH

        curX = self.border
        curY = self.border

        for idx, (widget, stretch, flags) in enumerate(self.widgets):
            minW, minH = widget.GetMinSize()

            if self.direction == HORIZONTAL:
                if flags & self.EXPAND:
                    ch = h - self.border * 2
                else:
                    ch = minH

                if stretch:
                    cw = minW + remainW * stretch // totalStretch
                else:
                    cw = minW

                if flags & self.ALIGN_CENTER:
                    cx = (cw - minW) // 2
                    cy = (h - minH) // 2
                else:
                    if flags & self.ALIGN_RIGHT:
                        cx = cw - minW
                    else:
                        # Default left
                        cx = 0

                    if flags & self.ALIGN_BOTTOM:
                        cy = h - minH
                    else:
                        # Default top
                        cy = 0

                widget.Paint(img,
                             curX + cx, curY + cy,
                             cw, ch,
                             focused and self.focus == idx)

                curX += cw + self.border
            else:
                if flags & self.EXPAND:
                    cw = w - self.border * 2
                else:
                    cw = minW

                if stretch:
                    ch = minH + remainH * stretch // totalStretch
                else:
                    ch = minH

                if flags & self.ALIGN_CENTER:
                    cx = (w - minW) // 2
                    cy = (ch - minH) // 2
                else:
                    if flags & self.ALIGN_RIGHT:
                        cx = cw - minW
                    else:
                        # Default left
                        cx = 0

                    if flags & self.ALIGN_BOTTOM:
                        cy = h - minH
                    else:
                        # Default top
                        cy = 0

                widget.Paint(img,
                             curX + cx, curY + cy,
                             cw, ch,
                             focused and self.focus == idx)

                curY += ch + self.border

        drw.blit(img, dx=x, dy=y, blend=True)

    def OnKey(self, pad):
        if pad.square:
            if self.direction == HORIZONTAL:
                if pad.right:
                    self.focus = (self.focus + 1) % len(self.widgets)
                    return
                elif pad.left:
                    self.focus = (self.focus + len(self.widgets) - 1) % len(self.widgets)
                    return
            else:
                if pad.down:
                    self.focus = (self.focus + 1) % len(self.widgets)
                    return
                elif pad.up:
                    self.focus = (self.focus + len(self.widgets) - 1) % len(self.widgets)
                    return

        self.widgets[self.focus][0].OnKey(pad)


#==============================================================================
#

class TextCtrl(Widget):
    def __init__(self,
                 clearer=None,
                 text='',
                 font=None,
                 margin=3):
        Widget.__init__(self, clearer)

        self.margin = margin

        if font is None:
            self.font = psp2d.Font('font_small.png')
        else:
            self.font = font

        self.text = text

    def _computePositions(self, w):
        curY = self.margin

        self._text = []
        for line in [k.split() for k in self.text.split('\n')]:
            while line:
                words = [line.pop(0)]

                while line and self.font.textWidth(' '.join(words)) <= w - self.margin * 2:
                    words.append(line.pop(0))

                if self.font.textWidth(' '.join(words)) > w - self.margin * 2:
                    line.insert(0, words.pop())

                spacing = 0
                if line:
                    if len(words) > 1:
                        spacing = (w - self.margin * 2 - self.font.textWidth(' '.join(words))) // (len(words) - 1)

                curX = self.margin
                for word in words:
                    self._text.append((curX, curY, word))
                    curX += self.font.textWidth(word + ' ') + spacing

                curY += self.font.textHeight(''.join(words)) + 3

            curY += 5

    def GetMinSize(self):
        minW = 0
        minH = self.font.textHeight(' ') + self.margin * 2

        for line in [k.split() for k in self.text.split('\n')]:
            for word in line:
                minW = max(minW, self.font.textWidth(word))

        minW += self.margin * 2

        return minW, minH

    def Paint(self, drw, x, y, w, h, focused):
        img = Widget.Paint(self, drw, x, y, w, h, focused)

        self._computePositions(w)

        for wx, wy, word in self._text:
            self.font.drawText(img, wx, wy, word)

        drw.blit(img, dx=x, dy=y, blend=True)

    def OnKey(self, pad):
        pass


#==============================================================================
#

class ChoiceCtrl(Widget):
    def __init__(self, clearer=None, height=10, font=None):
        Widget.__init__(self, clearer)

        self.height = height
        self.choices = []
        self.up = psp2d.Image('up.png')
        self.down = psp2d.Image('down.png')
        self.selection = None
        self.first = None

        if font is None:
            self.font = psp2d.Font('font_small.png')
        else:
            self.font = font

        self.minW = 16
        self.minH = 32 + self.font.textHeight(' ') * height

    def Append(self, choice):
        self.choices.append(choice)
        self.minW = max(self.minW, self.font.textWidth(str(choice)))

        if self.selection is None:
            self.selection = 0
            self.first = 0
            self.signal('SelectionChanged', self.GetSelection())

    def GetMinSize(self):
        return self.minW, self.minH

    def Paint(self, drw, x, y, w, h, focused):
        img = Widget.Paint(self, drw, x, y, w, h, focused)

        # Arrows

        if self.first != 0:
            img.blit(self.up, dx=(w - 16) // 2, dy=0, blend=True)

        if self.first + self.height < len(self.choices):
            img.blit(self.down, dx=(w - 16) // 2, dy=h - 16, blend=True)

        # Choices

        dy = (h - 32) // self.height
        curY = 16
        bg = psp2d.Image(w, dy)

        for idx in xrange(self.first, min(self.first + self.height, len(self.choices))):
            if idx == self.selection:
                bg.clear(psp2d.Color(255, 0, 0, 200))
            else:
                bg.clear(psp2d.Color(0, 0, 0, 0))

            self.font.drawText(bg, 0, 0, str(self.choices[idx]))
            img.blit(bg, dx=0, dy=curY, blend=True)
            curY += dy

        drw.blit(img, dx=x, dy=y, blend=True)

    def OnKey(self, pad):
        if pad.up:
            self.Prev()
        elif pad.down:
            self.Next()
        elif pad.cross:
            self.signal('Activated', self.GetSelection())

    def Next(self):
        if self.selection == len(self.choices) - 1:
            return

        self.selection += 1
        if self.selection - self.first == self.height:
            self.first += 1

        self.signal('SelectionChanged', self.GetSelection())

    def Prev(self):
        if self.selection == 0:
            return

        self.selection -= 1
        if self.selection == self.first - 1:
            self.first -= 1

        self.signal('SelectionChanged', self.GetSelection())

    def GetSelection(self):
        return self.choices[self.selection]


#==============================================================================
#

class ButtonCtrl(Widget):
    def __init__(self, clearer=None, label='', bitmap=None, font=None):
        Widget.__init__(self, clearer)

        self.label = label
        self.bitmap = bitmap

        if font is None:
            self.font = psp2d.Font('font_small.png')
        else:
            self.font = font

    def GetMinSize(self):
        if self.bitmap is None:
            return self.font.textWidth(self.label), self.font.textHeight(self.label)

        minW = self.bitmap.width + self.font.textWidth(self.label) + 3
        minH = max(self.bitmap.height, self.font.textHeight(self.label))

        return minW, minH

    def Paint(self, drw, x, y, w, h, focused):
        img = Widget.Paint(self, drw, x, y, w, h, focused)

        minW, minH = self.GetMinSize()

        cx = (w - minW) // 2
        cy = (h - minH) // 2

        img.blit(self.bitmap,
                 dx=cx,
                 dy=cy + (minH - self.bitmap.height) // 2,
                 blend=True)
        self.font.drawText(img,
                           cx + self.bitmap.width + 3,
                           cy + (minH - self.font.textHeight(self.label)) // 2,
                           self.label)

        drw.blit(img, dx=x, dy=y, blend=True)

    def OnKey(self, pad):
        if pad.cross:
            self.signal('Activated')


#==============================================================================
#

def loadModule(fullname):
    path, filename = os.path.split(fullname)
    name, ext = os.path.splitext(filename)

    fp, pathname, description = imp.find_module(name, [path])
    try:
        return imp.load_module(name, fp, pathname, description)
    finally:
        if fp:
            fp.close()

def main():
    bgimg = psp2d.Image('background.png')

    choice = ChoiceCtrl(height=10)
    description = TextCtrl()
    btn = ButtonCtrl(label='Quit', bitmap=psp2d.Image('quit.png'))

    def updateDescription(obj):
        description.text = obj.getDescription()

    def launchDemo(obj):
        obj.main()

    def quit():
        raise KeyboardInterrupt('quit')

    choice.connect('SelectionChanged', updateDescription)
    choice.connect('Activated', launchDemo)
    btn.connect('Activated', quit)

    class ModuleProxy(object):
        def __init__(self, mdl):
            self.mdl = mdl
        def __str__(self):
            return self.mdl.getName()
        def getDescription(self):
            return self.mdl.getDescription()
        def main(self):
            self.mdl.main()

    names = os.listdir('.')
    names.sort()
    for name in names:
        if name.endswith('.py') and name != 'script.py':
            mdl = loadModule(os.path.abspath(name))

            if hasattr(mdl, 'getName'):
                choice.Append(ModuleProxy(mdl))

    vsz = BoxSizer(direction=VERTICAL, border=5)
    vsz.Add(choice)
    vsz.Add(btn, 1, vsz.EXPAND)

    hsz = BoxSizer(direction=HORIZONTAL, border=5)
    hsz.Add(vsz, 0, hsz.EXPAND)
    hsz.Add(description, 1, hsz.EXPAND)

    screen = psp2d.Screen()

    oldpad = psp2d.Controller()
    while True:
        screen.blit(bgimg)
        hsz.Paint(screen, 0, 0, 480, 272, True)
        screen.swap()

        pad = psp2d.Controller()
        if pad != oldpad:
            hsz.OnKey(pad)
            oldpad = pad

if __name__ == '__main__':
    main()
