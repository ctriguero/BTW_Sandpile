# BTW Sandpile implementation

## The model:
Bibliography:
[The Abelian sandpile model of Bak-Tang-Wiesenfeld](https://journals.aps.org/pra/abstract/10.1103/PhysRevA.38.364)


## Compilation:
compilation: 

g++ BTW_DET_160722.cpp 

* The code by default performs a simulation on a 501X501 grid droping sand grains deterministically at the position (251,251).

## Launch:
You can run the program with default parameters simply by executing after the compilation:

./a.out

The easiest way to modify the default parameters is to use the defined flags:

-n    To set the number of avalanches.
-lx   To set the x dimension of the grid.
-ly   To set the y dimension of the grid.

## Output

1. A pgm figure it is generated at the end of the run. The colors correspond to the different heights in the sandpile i.e. 4 colors.

2. A file with the size histogram for the run is generated. This data can be plotted and interpreted. 
