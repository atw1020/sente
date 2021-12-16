Numpy Conversion
================

For deep learning applications, it is often desirable to represent a board as a NumPy array to be passed to a deep learning library such as `TensorFlow <https://www.tensorflow.org>`_ or `PyTorch <https://pytorch.org>`_.
The ``sente.game`` object provides the ``numpy()`` method to create a numpy representation of the board.

.. code-block:: python

    >>> import sente
    >>> game = sente.Game()
    >>> numpy_array = game.numpy()
    >>> print(numpy_array)
    [[[0 0 1 0]
      [0 0 1 0]
      [0 0 1 0]
      ...
      [0 0 1 0]
      [0 0 1 0]
      [0 0 1 0]]

     ...

     [[0 0 1 0]
      [0 0 1 0]
      [0 0 1 0]
      ...
      [0 0 1 0]
      [0 0 1 0]
      [0 0 1 0]]]

.. doctest:: python
    :hide:

    >>> import sente
    >>> game = sente.Game()
    >>> numpy_array = game.numpy()
    >>> print(numpy_array)
    [[[0 0 1 0]
      [0 0 1 0]
      [0 0 1 0]
      ...
      [0 0 1 0]
      [0 0 1 0]
      [0 0 1 0]]]


Sente converts a game to a NumPy array by creating several 19x19 (or 9x9 or 13x13 respectively) NumPy array that contains some feature associated with each point on the board.
These features are 1-hot encoded facts about the board, such as the presence of a black or white stone or whether or not the point in question is a Ko point.

By default, the ``numpy()`` method uses four such features, namely the presence of white stones, black stones, empty points, and ko points.
However, if a different set of features is desired, it is possible to pass the ``numpy()`` method a list of strings specifying the desired characteristics.

.. code-block:: python

    >>> import sente
    >>> game = sente.Game()
    >>> array = game.numpy(["black_stones", "white_stones"])
    >>> array.shape
    (19, 19, 2)

.. doctest:: python
    :hide:

    >>> import sente
    >>> game = sente.Game()
    >>> array = game.numpy(["black_stones", "white_stones"])
    >>> array.shape
    (19, 19, 2)

Thus, the ``Game.numpy()`` method returns an NxNxF NumPy array where N denotes the size of the board (i.e., 19) and F denotes the number of features per point on the board.