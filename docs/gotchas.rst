Gotchas
=======

This page contains some easy-to-make mistakes that come up when writing code using sente.
These Issues may lead to sleeper

Internal vs external Co-ordinates
---------------------------------

Internally, Sente represents co-ordinates on a go board with `zero based indexing <https://en.wikipedia.org/wiki/Zero-based_numbering>`_.
This means that internally, a stone on the 1-1 corner point would actually be located at the index ``(0, 0)`` rather than ``(1, 1)``.
The ``Game.play()`` method hides this from you by automatically subtracting 1 from the co-ordinates that you specify

Go
