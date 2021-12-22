[![](sente_logo.svg)](https://discord.gg/RyeFekyrFA)


[<img src="https://discord.com/assets/ff41b628a47ef3141164bfedb04fb220.png" alt="drawing" width="200"/>](https://discord.gg/RyeFekyrFA)

[![PyPI version](https://badge.fury.io/py/sente.svg)](https://badge.fury.io/py/sente)
[![docs](https://readthedocs.org/projects/sente/badge/?version=latest)](https://sente.readthedocs.io/en/latest/)
[![tests](https://github.com/atw1020/sente/actions/workflows/tests.yml/badge.svg)](https://github.com/atw1020/sente/actions/workflows/tests.yml)
[![pip](https://github.com/atw1020/sente/actions/workflows/pip.yml/badge.svg)](https://pypi.org/project/sente/)


Sente (from the Japanese せんて/先手) is a general-purpose open-source python library for the ancient chinese board game Go/Badok/Weiqi.
It allows for python code to simulate and play a game of Go and also read, edit and create SGF files.
Sente is designed to be a Python 3 replacement for the [gomill](https://github.com/mattheww/gomill) library. 
In addition to supporting Python 3, Sente is written in C++ which allows AIs that use Sente to take advantage of C++'s superior performance.


Installation & Requirements
---

Sente runs on any operating system with python 3.5, 3.8 and 3.9 and can be installed via pip.

```bash
$ pip install sente
```
-- or --
```bash
$ python3 -m pip install sente
```

Getting Started
---

The Basic element in Sente is the `sente.Game` object which represents a Simple Game.

```python
>>> import sente
>>> game = sente.Game()
```
By default, sente creates a 19x19 game with Chinese Rules.
9x9 and 13x13 board sizes and Japanese rules can be specified if desired.

_note: japanese rules are not reccomended as sente does not currently include automatic dead stone removal._
```python
>>> game = sente.Game(13)
>>> game = sente.Game(19, sente.rules.JAPANESE)
```
moves can be played on the game using the `play()` method, and the board can be printed using the python `print()` function.
```python
>>> game.play(4, 4)
>>> game.play(16, 4)
>>> game.play(4, 17)
>>> print(game)
```
```
 1  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
 2  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
 3  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
 4  .  .  .  ⚫ .  .  .  .  .  *  .  .  .  .  .  *  ⚫ .  .
 5  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
 6  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
 7  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
 8  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
 9  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
10  .  .  .  *  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .
11  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
12  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
13  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
14  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
15  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
16  .  .  .  ⚪ .  .  .  .  .  *  .  .  .  .  .  *  .  .  .
17  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T

```
The text created by printing a Sente board is designed to be similar to the [gomill ascii boards output](https://mjw.woodcraft.me.uk/gomill/doc/0.7/ascii_boards.html).
Unlike Gomill however, Sente uses unicode characters to represent black and white stones to make complex board positions more visible and also plots star points.

Finally, Sente also provides utilities for reading and saving SGF files using the `sente.SGF` module

```python
>>> from sente import sgf
>>> game = sgf.load("Lee Sedol ladder game.sgf")
>>> move_sequence = game.get_default_sequence()[:95]
>>> game.play_sequence(move_sequence)
>>> print(game)
1  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
2  .  ⚪ ⚫ ⚪ .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
3  .  .  ⚪ ⚫ ⚫ .  .  .  .  .  .  .  .  ⚫ .  .  .  .  .
4  .  .  .  ⚪ .  ⚫ .  .  .  *  .  .  .  .  .  ⚫ .  .  .
5  .  .  ⚪ ⚪ .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
6  .  .  ⚪ ⚫ ⚫ .  .  .  .  .  .  .  .  .  .  .  .  .  .
7  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  .  .  .  .  .  .  .  .  .
8  .  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  .  .  .  .  .  .  .  .
9  .  .  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  .  .  .  .  .  .  .
10  .  .  .  *  .  ⚫ ⚪ ⚪ ⚫ *  .  .  .  .  .  *  .  ⚫ .
11  .  .  .  .  .  .  ⚫ ⚪ ⚪ ⚫ .  .  .  .  ⚫ .  ⚫ ⚪ .
12  .  .  .  .  .  .  .  ⚫ ⚪ ⚪ ⚫ .  ⚪ ⚫ .  ⚫ .  ⚪ ⚪
13  .  .  .  .  .  .  .  .  ⚫ ⚪ ⚪ ⚫ ⚪ ⚫ ⚫ ⚪ ⚪ ⚪ .
14  .  .  .  .  .  .  .  .  .  ⚫ ⚪ ⚫ ⚪ ⚪ ⚪ ⚫ ⚫ ⚫ ⚪
15  .  .  ⚪ .  .  .  .  .  .  .  .  ⚫ ⚫ ⚫ ⚫ ⚪ ⚪ ⚫ ⚪
16  .  .  .  *  .  .  .  .  .  *  .  .  ⚪ .  .  ⚪ ⚪ ⚫ .
17  .  .  .  ⚪ .  .  .  .  ⚫ .  .  .  .  ⚫ ⚫ ⚫ ⚪ ⚫ ⚫
18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  ⚪ ⚪ ⚫
19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  ⚪ .
    A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T
```

### Further Reading

* [Game Trees](https://sente.readthedocs.io/en/latest/tutorial/game%20tree%20navigation.html)
* [SFG file reader](https://sente.readthedocs.io/en/latest/tutorial/sgf.html)
* [NumPy conversion](https://sente.readthedocs.io/en/latest/tutorial/numpy.html)
* [GTP (Go Text Protocol)]() (Coming Soon!)
* [Automatic Dead Stone removal]() (Coming Soon!)

Building & Contributing
---

Sente uses [CMake](https://cmake.org), [git](https://git-scm.com) and C++11. In order to 
build the library, you will need CMake, and a 
C++ compiler that supports C++11. You can install these 
using a software package installer like homebrew or apt.

OSx
```bash
$ brew install git
$ brew install cmake
$ xcode-select --install  # installs Apple clang++
```
Debian/Ubuntu
```bash
$ sudo apt-get install git
$ sudo apt-get install cmake
$ sudo apt-get install g++
```

A sente binary can be built by running the setup script.

```bash
$ python3 setup.py develop
```

To import the resulting .so file, simply import sente in a local python interpreter
```bash
$ python3 setup.py develop
$ python3
```
```
Python 3.8.10 (v3.8.10:3d8993a744, May  3 2021, 09:09:08) 
[Clang 12.0.5 (clang-1205.0.22.9)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> import sente
>>> 
```

### Building documentation

in order to build the documentation, you will need to have [sphinx](https://www.sphinx-doc.org/en/master/) installed and build a development version of sente.
```bash
$ pip install -r requirements.txt # install sphinx
$ python setup.py develop # build a development version of sente
$ cd docs
$ make html # make the html
```
