# H-coloring 4-regular graphs

This repository contains code and data related to the paper "A note on $\bar{X}$-coloring and $\hat{A}$-coloring 4-regular graphs". All code can be found in the directory "Code", whereas all data can be found in the directory "Data".

Below, we briefly describe the different programs and data.

## DATA
This folders contains two files each containing a graph in graph6 format. The file "planar4Regular18Vertices.g6" contains the graph shown in Fig. 2 from the paper "A note on $\bar{X}$-coloring and $\hat{A}$-coloring 4-regular graphs". This graph provides a negative answer to Questions 1.2 and 1.3 from that paper. The file "lineGraphTietze.g6" contains the line graph of the Tietze graph. This graph is a counterexample to Conjecture 21 from "Malnegro, Analen A., and Kenta Ozeki. H-colorings for 4-regular graphs. Discrete Mathematics 347.3 (2024): 113844."

## CODE

### filterDoesNotHaveXBarColoring.cpp

The program can be compiled by executing the following command:
```bash
g++ -g -std=c++11 -O3 filterDoesNotHaveXBarColoring.cpp -o filterDoesNotHaveXBarColoringExecutable
```

This program expects as input a list of graph6 strings (one graph per line). The program will output each graph that is not $\bar{X}$-colorable.
For example, executing the following command:

```bash
./filterDoesNotHaveXBarColoringExecutable < ../Data/lineGraphTietze.g6 
```
 will output the line graph of the Tietze graph after a few minutes:

```bash
Q{daHG?CGB`C?h??AAGCFA@O@HG
```

This means that the line graph of the Tietze graph is not $\bar{X}$-colorable.

### filterDoesNotHaveXBarColoringOtherImplementation.cpp

The program can be compiled by executing the following command:
```bash
g++ -g -std=c++11 -O3 filterDoesNotHaveXBarColoringOtherImplementation.cpp -o filterDoesNotHaveXBarColoringOtherImplementationExecutable
```

This program behaves similarly as the previous program, but it tends to be much faster. For example,

```bash
./filterDoesNotHaveXBarColoringOtherImplementationExecutable < ../Data/lineGraphTietze.g6
```
 will output the line graph of the Tietze graph after a few seconds:

```bash
Q{daHG?CGB`C?h??AAGCFA@O@HG
```

### filterDoesNotHave2C3Coloring.cpp, filterDoesNotHave2C3ColoringOtherImplementation.cpp, filterDoesNotHaveAHatColoring.cpp, filterDoesNotHaveAHatColoringOtherImplementation.cpp

These programs are similar as the ones described earlier, but this time filters out graphs that are not $2C_3$-colorable or $\hat{A}$-colorable.
