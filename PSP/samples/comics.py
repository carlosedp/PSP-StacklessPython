# -*- coding: ISO-8859-1 -*-

import time, psp2d

class ProgressBar(object):
    def __init__(self, msg, name='progress'):
        self.img = psp2d.Image(name + '.png')
        self.bgi = psp2d.GrayFilter().apply(self.img)

        self.msg = msg
        self.frac = 0

        self.w, self.h = self.img.size

    def update(self, count, total):
        self.frac = 1.0 * count / total

    def draw(self, drw, x, y):
        drw.drawText(x, y, self.msg + ('%.2f%%' % (100 * self.frac)), psp2d.Color(255, 0, 0))
        drw.blit(self.bgi, dx=x, dy=y+16)
        drw.blit(self.img, dx=x, dy=y+16, dw=int(self.frac * self.w), blend=True)

class Rectangle(object):
    def __init__(self, x1, y1, x2, y2):
        self.x1 = x1
        self.y1 = y1
        self.x2 = x2
        self.y2 = y2

    def isIn(self, x, y):
        return x >= self.x1 and y >= self.y1 and x <= self.x2 and y <= self.y2

    def draw(self, img, color=psp2d.Color(255, 0, 0)):
        img.fillRect(self.x1, self.y1, self.x2 - self.x1 + 1, self.y2 - self.y1 + 1, color)

    def intersects(self, r):
        if self.y2 < r.y1:
            return False
        if self.y1 > r.y2:
            return False
        if self.x2 < r.x1:
            return False
        if self.x1 > r.x2:
            return False
        return True

    def union(self, r):
        self.x1 = min(self.x1, r.x1)
        self.x2 = max(self.x2, r.x2)
        self.y1 = min(self.y1, r.y1)
        self.y2 = max(self.y2, r.y2)

class RectangleSet(object):
    def __init__(self, model=None):
        self.rectangles = []

        if model is not None:
            for r in model.rectangles:
                self.add(r)

    def add(self, r):
        for o in self.rectangles:
            if o.intersects(r):
                o.union(r)
                break
        else:
            self.rectangles.append(r)

    def isIn(self, x, y):
        for r in self.rectangles:
            if r.isIn(x, y):
                return True
        return False

def main():
    img = psp2d.Image('orig.jpg')
    scr = psp2d.Screen()

    class CBProgress(ProgressBar):
        def update(self, value, total):
            super(CBProgress, self).update(value, total)

            scr.clear(psp2d.Color(0, 0, 0))
            self.draw(scr, 0, 0)
            scr.swap()

    p = CBProgress('Monochrome')

    t0 = time.time()
    psp2d.MonochromeInPlaceFilter(200).apply(img, p.update)
    print 'Monochrome: %.2f seconds' % (time.time() - t0)
    img.saveToFile('result01.png')

    p = CBProgress('Erosion')

    t0 = time.time()
    img = psp2d.ErosionFilter(psp2d.StructuringElement(3)).apply(img, p.update)
    print 'Erosion: %.2f seconds' % (time.time() - t0)
    img.saveToFile('result02.png')

    p = CBProgress('Dilation')

    t0 = time.time()
    img = psp2d.DilationFilter(psp2d.StructuringElement(3)).apply(img, p.update)
    print 'Dilation: %.2f seconds' % (time.time() - t0)
    img.saveToFile('result03.png')

    rset = RectangleSet()

    p = CBProgress('Cutting')

    t0 = time.time()

    for y in xrange(img.height):
        for x in xrange(img.width):
            if not rset.isIn(x, y):
                p = img.getPixel(x, y)
                if p.red == 0 and p.green == 0 and p.blue == 0:
                    rset.add(Rectangle(*img.boundingRectangle(x, y)))
        p.update(y, img.height)

    print 'Rectangles: %.2f seconds' % (time.time() - t0)

    for i, r in enumerate(RectangleSet(rset).rectangles):
        del img # Or else, MemoryError...

        img = psp2d.Image('orig.jpg')
        r.draw(img)
        img.saveToFile('result%02d.png' % (i + 4))

if __name__ == '__main__':
    main()
