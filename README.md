# PageReplacementAlgs

## About
This program was made to understand how the different page replacement algorithms work and what makes them unique. The three algorithms that will be looked at are First In First Out (FIFO), Least Recently Used (LRU), and Optimal Page Replacement (OPT). First In First Out simply replaces pages based on which one came first, hence the name. Least Recently Used keeps track of how long it has been since a page was used, and the one that is least recently used gets replaced (again, hence the name). Finally, Optimal Page Replacement looks ahead in the string of pages to be inputted and the page that has its next occurance farthest away (if ever) gets replaced. 

## Compiling
To compile this program, simply download main.cpp and main.h, and ensure they are in a folder together. Open that folder in a terminal and type `g++ main.cpp -o main`, which should give you the executable to run. 

## Comments about Time Complexity
First In First Out is the simplest to implement as it only is set up to cycle through the table for every cache miss, it would make sense for it to be O(n). Least Recently Used would end up being O(n^2) due to not only comparing the hits, but also what is the minimum. Finally, the way I implemented Optimal page Replacement required O(n^3) due to clearing the array of distances, finding said distances, and finally finding the maximum of all of that.
