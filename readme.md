Sente 
---

Sente (from the Japanese せんて/先手) is a general-purpose
open source python library for the ancient asian board
game Go/Badok/Weiqi. Sente is designed to be a Python 3
replacement for the [gomill](https://github.com/mattheww/gomill)
library. 

In addition to supporting Python 3, Sente is written in C++
which allows AIs that use Sente to have comparable to


Installation & Requirements
---

Sente runs on any operating system with python 3.x
and can be installed via pip

```zsh
pip install sente
```
-- or --
```zsh
python3 -m pip install sente
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

Sente records all moves played in a game in a tree
structure. the sente.Game 

Documentation
---

[WIP will have documentation using sphinx and 
readthedocs.io]

Building & Contributing
---

Sente uses a [cmake](https://cmake.org) system for
building 