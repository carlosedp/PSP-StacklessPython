#-*- coding: ISO-8859-1 -*-

import unittest, os

import test_tickets

def main():
    print 'Running tests from %s' % os.getcwd()

    s = unittest.TestSuite()

    s.addTest(test_tickets.suite())

    unittest.TextTestRunner(verbosity=2).run(s)

if __name__ == '__main__':
    main()
