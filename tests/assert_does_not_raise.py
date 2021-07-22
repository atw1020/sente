import traceback
import unittest
from unittest.case import _AssertRaisesContext


class _AssertNotRaisesContext(_AssertRaisesContext):
    def __exit__(self, exc_type, exc_value, tb):
        if exc_type is not None:
            self.exception = exc_value.with_traceback(None)

            try:
                exc_name = self.expected.__name__
            except AttributeError:
                exc_name = str(self.expected)

            if self.obj_name:
                self._raiseFailure("{} raised by {}".format(exc_name,
                                                            self.obj_name))
            else:
                self._raiseFailure("{} raised".format(exc_name))

        else:
            traceback.clear_frames(tb)

        return True


class DoesNotRaiseTestCase(unittest.TestCase):

    def assertDoesNotRaise(self, expected_exception, *args, **kwargs):
        context = _AssertNotRaisesContext(expected_exception, self)
        try:
            return context.handle('assertNotRaises', args, kwargs)
        finally:
            context = None
