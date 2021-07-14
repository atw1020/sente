Sente 
---

Sente (from the Japanese せんて/先手) is a general-purpose open-source python library for the ancient asian board game Go/Badok/Weiqi. 
Sente is designed to be a Python 3 replacement for the [gomill](https://github.com/mattheww/gomill) library. 

In addition to supporting Python 3, Sente is written in C++ which allows AIs that use Sente to take advantage of C++'s superior performance.


Installation & Requirements
---

Sente runs on any operating system with python 3.x and can be installed via pip.

```zsh
$ pip install sente
```
-- or --
```zsh
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
 4  .  .  .  #  .  .  .  .  .  *  .  .  .  .  .  *  #  .  .
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
16  .  .  .  O  .  .  .  .  .  *  .  .  .  .  .  *  .  .  .
17  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
18  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
19  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .  .
    A  B  C  D  E  F  G  H  J  K  L  M  N  O  P  Q  R  S  T

```
The Sente board display is designed to be similar to the [gomill ascii boards output](https://mjw.woodcraft.me.uk/gomill/doc/0.7/ascii_boards.html).
A hash (`#`) denotes a Black stone and an "O" (`O`) denotes a white stone. 
One difference between sente and gomill is that in sente, the star points are denoted using an asterisk (`*`)

Finally, Sente also provides the ability to check if a move is legal using the `is_legal()` method

```python
>>> import sente
>>> game = sente.Game()
>>> game.play(4, 4)
>>> game.is_legal(4, 4)
False
```

### Further Reading

* [Game Trees]()
* [SFG file reader]()
* [NumPy conversion]() (Coming Soon!)
* [GTP (Go Text Protocol)]() (Coming Soon!)
* [Automatic Dead Stone removal]() (Coming Soon!)

Documentation
---

[WIP will have documentation using sphinx and 
readthedocs.io]

Building & Contributing
---

Sente uses [CMake](https://cmake.org), [git](https://git-scm.com) and C++11. In order to 
build the library, you will need CMake, and a 
C++ compiler that supports C++11. You can install these 
using a software package installer like homebrew or apt.

OSx
```zsh
$ brew install git
$ brew install cmake
$ brew install clang++
```
Debian/Ubuntu
```zsh
$ sudo apt-get install git
$ sudo apt-get install cmake
$ sudo apt-get install g++
```
Be sure to create a virtual that you can install any
custom Sente builds in so that they don't interfere
with any projects you have that use Sente.

```zsh
$ git clone https://github.com/atw1020/sente
$ cd sente
$ python3 -m venv venv
$ source venv/bin/activate # (OSx and Linux Only, see python docs for windows equivalent)
```

To build the library and install it into your activated
environment, run the setup.py file

```zsh
$ python3 setup.py install
```

### Building documentation

in order to build the documentation, you will need to have [sphinx](https://www.sphinx-doc.org/en/master/) installed and build a development version of sente.
```zsh
$ pip install sphinx # install sphinx
$ python setup.py develop # build a development version of sente
$ cd docs
$ make html # make the html
```
