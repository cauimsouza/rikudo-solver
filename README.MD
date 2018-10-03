# Rikudo puzzle generator and solver

<img src="https://github.com/cauimsouza/rikudo-solver/blob/master/media/title-image.png" alt="Rikudo view" >

Rikudo is a game where the goal of the player is to find a path (numbering) of adjacent cells in an hex grid, given that some cells are already numbered and some pairs of cells must be consecutive in the path (http://www.rikudo.fr). Given a black and white image, the code: 
(i) creates a hex grid of approximately the same format 
(ii) generates a rikudo puzzle with minimal constraints such that the solution is unique 
(iii) outputs the solution

## Authors

Cauim de Souza Lima  
Victor Hugo Vianna

## Prerequisites

Java 1.9 for the GUI

## Instructions

To compile open a shell and run
```
$ make
```
Then, add a black and white image with the name "input.png" to the directory and execute

```
$ ./run.sh
```

You will be given an image of the board and will be asked to choose starting and ending vertices for your path. If there is no possible solution for this choice, you'll be asked to provide other vertices. Otherwise, the program will output a puzzle with minimal constraints and its solution. Just close all windows to finish. [Notice that for the Christmas Tree example, the only vertices with a possible hamiltonian path are 1 and 63, as they both have degree 1!] Have fun :)

## Example of results

<img src="https://github.com/cauimsouza/rikudo-solver/blob/master/media/example.png" alt="Input">

## Main ideas

SAT-solver for finding hamiltonian paths, with well-chosen clauses to reduce running time (sometimes redundant clauses can help)  
Finding a base solution and using binary search to find set of constraints that enforce such solution

## Acknowledgments

https://github.com/msoos/cryptominisat (SAT-solver used)

