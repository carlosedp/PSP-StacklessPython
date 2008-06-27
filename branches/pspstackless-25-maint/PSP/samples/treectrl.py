#!/usr/bin/env python
#-*- coding: ISO-8859-1 -*-

#****h* /treectrl.py
#  NAME
#    treectrl -- 
#  COPYRIGHT
#
#  Copyright (C) 2008 Jérôme Laheurte <fraca7@free.fr>
#
# This library  is free software; you can  redistribute it and/or
# modify  it under  the terms  of the  GNU Lesser  General Public
# License as  published by  the Free Software  Foundation; either
# version  2.1 of  the License,  or  (at your  option) any  later
# version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY;  without even the implied warranty of
# MERCHANTABILITY or  FITNESS FOR A PARTICULAR  PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You  should have  received a  copy  of the  GNU Lesser  General
# Public License  along with this  library; if not, write  to the
# Free  Software Foundation,  Inc., 59  Temple Place,  Suite 330,
# Boston, MA 02111-1307 USA
#
#  CREATION DATE
#    12 May 2008
#***

__rcsid__ = '$Id$'

from psp2d import *

class FontFactory(object):
    fonts = {}

    def __init__(self, *args, **kwargs):
        super(FontFactory, self).__init__(*args, **kwargs)

    def getFont(self, name):
        if not self.fonts.has_key(name):
            self.fonts[name] = Font(name + '.png')
        return self.fonts[name]

class TreeItem(object):
    def __init__(self, *args, **kwargs):
        super(TreeItem, self).__init__(*args, **kwargs)

        self.children = []
        self.collapsed = True

    def draw(self, drw):
        raise NotImplementedError

    def visibleChildren(self, level):
        ls = []

        if not self.collapsed:
            for child in self.children:
                ls.append((level + 1, child))
                ls.extend(child.visibleChildren(level + 1))

        return ls

class TextTreeItem(TreeItem):
    def __init__(self, fontName='font_small', *args, **kwargs):
        super(TextTreeItem, self).__init__(*args, **kwargs)

        self.font = FontFactory().getFont(fontName)

    def getText(self):
        """Should return the text, encoded in ISO-8859-1"""

        raise NotImplementedError

    def draw(self, drw):
        txt = self.getText()

        h = self.font.textHeight(txt)
        self.font.drawText(drw, 2, (16 - h) // 2, txt)

class StaticTextTreeItem(TextTreeItem):
    def __init__(self, text, *args, **kwargs):
        super(StaticTextTreeItem, self).__init__(*args, **kwargs)

        self.text = text

    def getText(self):
        return self.text

class TreeCtrl(object):
    def __init__(self, *args, **kwargs):
        super(TreeCtrl, self).__init__(*args, **kwargs)

        self.root = TreeItem()
        self.root.collapsed = False

        self.frozen = False
        self.first = 0

        self.arrow1 = Image('arrow1.png')
        self.arrow2 = Image('arrow2.png')

        self.selected = None
        self.select = Image(480, 16)
        self.select.clear(Color(0, 0, 255, 128))

    def freeze(self):
        self.frozen = True

    def thaw(self):
        self.frozen = False
        self.redraw()

    def append(self, parent, item):
        if parent is None:
            self.root.children.append(item)
        else:
            parent.children.append(item)

        item.parent = parent

        if self.selected is None:
            self.selected = item

        if not self.frozen:
            self.redraw()

        return item

    def selectNext(self):
        children = self.root.visibleChildren(0)[self.first:]

        for idx, (level, item) in enumerate(children):
            if item is self.selected:
                if idx + 1 >= len(children):
                    self.selected = self.root.children[0]
                    self.first = 0
                else:
                    self.selected = children[idx + 1][1]
                    if idx + 1 >= 17:
                        self.first += 1
                break

        if not self.frozen:
            self.redraw()

    def selectPrev(self):
        children = self.root.visibleChildren(0)[self.first:]

        for idx, (level, item) in enumerate(children):
            if item is self.selected:
                if idx == 0:
                    if self.first == 0:
                        children = self.root.visibleChildren(0)
                        self.selected = children[-1][1]
                        self.first = max(0, len(children) - 17)
                    else:
                        self.first -= 1
                        self.selected = self.root.visibleChildren(0)[self.first][1]
                else:
                    self.selected = children[idx - 1][1]
                break

        if not self.frozen:
            self.redraw()

    def redraw(self):
        screen = Screen()
        screen.clear(Color(255, 255, 255))

        for idx, (level, item) in enumerate(self.root.visibleChildren(0)[self.first:]):
            drw = Image(480 - (16 * level), 16)

            drw.clear(Color(255, 255, 255, 0))

            item.draw(drw)
            screen.blit(drw, dx=16*level, dy=idx*16, blend=True)

            if item.children:
                if item.collapsed:
                    screen.blit(self.arrow1, dx=16*(level - 1), dy=idx*16, blend=True)
                else:
                    screen.blit(self.arrow2, dx=16*(level - 1), dy=idx*16, blend=True)

            if item is self.selected:
                screen.blit(self.select, dx=0, dy=idx*16, blend=True)

        screen.swap()

def main():
    tree = TreeCtrl()
    tree.freeze()
    try:
        item = tree.append(None, StaticTextTreeItem('Test 1'))
        tree.append(item, StaticTextTreeItem('Test 2'))
        tree.append(item, StaticTextTreeItem('Test 3'))
        tree.append(None, StaticTextTreeItem('Test 4'))
        item.collapsed = False

        for i in xrange(5, 20):
            tree.append(None, StaticTextTreeItem('Test %d' % i))
    finally:
        tree.thaw()

    while True:
        ctrl = Controller()
        if ctrl.circle:
            break
        elif ctrl.right:
            tree.selected.collapsed = False
            tree.redraw()
        elif ctrl.left:
            tree.selected.collapsed = True
            tree.redraw()
        elif ctrl.up:
            tree.selectPrev()
            tree.redraw()
        elif ctrl.down:
            tree.selectNext()
            tree.redraw()

if __name__ == '__main__':
    main()
