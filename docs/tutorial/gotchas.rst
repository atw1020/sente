FAQs & Gotchas
=======

This page contains some easy-to-make mistakes that come up when writing code using sente.

Boards vs Games
---------------

Sente provides two different constructs that represent go boards: the ``sente.Game`` object and the various ``sente.Board<9, 13, 19>`` objects.
On the surface, it seems like these objects are similar to each other because both represent a position on a go board and both have a ``play`` method.

TL;DR just use ``sente.Game``.
******************************

The difference between the two is that a ``sente.Board`` contains only the location of the stones on the board and nothing else.
The ``sente.Board`` Objects do not know anything about capturing stones, Whose turn it is, or Kos.
``sente.Board`` objects only exist in sente if for some reason you need to ignore the rules of the game.

