Sente 
---

Sente (from the Japanese せんて/先手) is a general-purpose
open-source python library for the ancient asian board
game Go/Badok/Weiqi. Sente is designed to be a Python 3
replacement for the [gomill](https://github.com/mattheww/gomill)
library. 

In addition to supporting Python 3, Sente is written in C++
which allows AIs that use Sente to take advantage of C++'s
superior performance.


Installation & Requirements
---

Sente runs on any operating system with python 3.x
and can be installed via pip

```zsh
$ pip install sente
```
-- or --
```zsh
$ python3 -m pip install sente
```

Getting Started
---

The Basic element in Sente is the `sente.Game` object
which represents a Simple Go Game


```python
>>> import sente
>>> game = sente.Game()
```
by default, sente creates a 19x19 game with Chinese Rules.
9x9 and 13x13 board sizes and Japanese rules can be specified if desired.
```python
>>> game = sente.Game(13)
>>> game = sente.Game(19, sente.rules.JAPANESE)
```
moves can be played on the game using the `play()` method
and the board can be printed using the python print() function
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
    A  B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S

```
The Sente board display is designed to be similar to the 
[gomill ascii boards output](https://mjw.woodcraft.me.uk/gomill/doc/0.7/ascii_boards.html).
A hash (`#`) denotes a Black stone and an "O" (`O`) 
denotes a white stone. The star points are denoted
using an asterisk (`*`)

The last part of the 

### Further Reading

* [Game Trees]()
* [SFG file reader]()
* [NumPy conversion]() (Coming Soon!)
* [GTP (Go Text Protocol)]() (Coming Soon!)

Documentation
---

[WIP will have documentation using sphinx and 
readthedocs.io]

Building & Contributing
---

Sente uses [CMake](https://cmake.org), [git](https://git-scm.com) and C++11. In order to 
build the library, you will need CMake, and a 
C++ compiler that supports C++11.

```zsh
$ git clone https://github.com/atw1020/sente
$ cd sente
```

Be sure to create a virtual that you can install any
custom Sente builds in so that they don't interfere
with any projects you have that use Sente.

```zsh
$ python3 -m venv venv
$ source venv/bin/activate # (OSx and Linux Only, see python docs for windows)
```

To build the library and install it into your activated
environment, run the setup.py file

```zsh
$ python3 setup.py install
```