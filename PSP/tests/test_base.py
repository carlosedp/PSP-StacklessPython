#-*- coding: ISO-8859-1 -*-

import unittest, md5

class TestBase(unittest.TestCase):
    def hashFile(self, filename):
        h = md5.new()
        fp = file(filename, 'rb')
        try:
            bf = fp.read(4096)
            while bf:
                h.update(bf)
                bf = fp.read(4096)
        finally:
            fp.close()

        return h.hexdigest()
