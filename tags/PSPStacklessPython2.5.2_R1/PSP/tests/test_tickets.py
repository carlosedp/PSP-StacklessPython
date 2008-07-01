#-*- coding: ISO-8859-1 -*-

import unittest, os, threading, time

import test_base, psp2d

class TestTicket018(test_base.TestBase):
    def setUp(self):
        self.screen = psp2d.Screen()
        self.font = psp2d.Font('font.png')

    def test_to_screen(self):
        "Draw text to screen, with accented characters"

        self.screen.clear(psp2d.Color(127, 127, 127))
        self.font.drawText(self.screen, 0, 0, 'Jérôme')
        self.screen.swap()
        self.screen.saveToFile('test.png')

        h = self.hashFile('test.png')
        self.failIf(h != '3f5273009ee9ec5eadd0385aeaf2e27d', h)

    def test_to_image(self):
        "Draw text to image (with alpha != 0)"

        img = psp2d.Image(200, 200)
        img.clear(psp2d.Color(127, 127, 127, 127))
        self.font.drawText(img, 0, 0, 'Jérôme')
        img.saveToFile('test.png')

        h = self.hashFile('test.png')
        self.failIf(h != '9757122d8962cac6317da2f561b3a822', h)

class TestTicket028(test_base.TestBase):
    def test_clock(self):
        "setclock"

        os.setclock(100)
        self.failUnless(os.getclock() == 99, os.getclock())

        os.setclock(266)
        self.failUnless(os.getclock() == 265, os.getclock())

class TestTicket033(test_base.TestBase):
    def test_dirs(self):
        ". and .. shouldn't be listed by os.listdir"

        files = os.listdir(os.getcwd())

        self.failIf('.' in files, str(files))
        self.failIf('..' in files, str(files))

class TestTicket335(test_base.TestBase):
    "Blit from screen"

    def test_blit(self):
        screen = psp2d.Screen()
        img = psp2d.Image('background.png')
        screen.blit(img)
        screen.swap()

        img = psp2d.Image(480, 272)
        img.blit(screen)
        img.saveToFile('test.png')

        h = self.hashFile('test.png')
        self.failIf(h != '73f23b452de36ca58ab5a65d72c2ebd5', h)

class TestTicket337(test_base.TestBase):
    def test_sleep(self):
        "time.sleep() has resolution < 1s"

        t0 = time.time()
        time.sleep(0.1)
        self.failUnless(time.time() - t0 < 0.2)

def suite():
    s = unittest.TestSuite()

    s.addTest(unittest.makeSuite(TestTicket018, 'test'))
    s.addTest(unittest.makeSuite(TestTicket028, 'test'))
    s.addTest(unittest.makeSuite(TestTicket033, 'test'))
    s.addTest(unittest.makeSuite(TestTicket335, 'test'))
    s.addTest(unittest.makeSuite(TestTicket337, 'test'))

    return s
