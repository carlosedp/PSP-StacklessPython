import unittest


class TestException(unittest.TestCase):
    def testTaskletExitCode(self):
        # Tasklet exit was defined as the wrong kind of exception.
        # When its code attribute was accessed the runtime would
        # crash.  This has been fixed.
        exc = TaskletExit()
        exc.code
