# Demand Paging and Page Replacements

### Purpose: 
The user enters values into command line with how many physical pages, unique reference pages,
and the length of their reference stream. The assumptions that there can only be 3-5 physical pages,
5-7 for unique reference pages, and any number for length of reference stream. The code is to calculate
the amount of hit ratios the application would incur using an algorithm we designed. I chose to design
an algorithm that was a mix of first-in first-out (FIFO) and least frequently used (LFU). That means 
that if a unique page is hit after being hit three other times, then it will be replaced using the FIFO 
method. If no unique pages have three hits, and a new unique page comes in, then it replaces whichever 
unique page was inserted first. Same thing if there were more than two pages that had three hits, and 
there is a new unique page, then the first page found will be replaced. Each page reference in the 
schedule will be saved onto a single-dimensional array that is the length of how much the user inputted.
The array will also be an array of structs that has a unique reference page and the count on how many 
times hit. For the FIFO, the page references will be saved onto a queue to keep track of the order they 
came in.

### Input: 
The user enters values into command line with how many physical pages, unique reference pages,
       and the length of their reference stream. The user can enter a number from range 3-5 for physical
       pages, a number ranged 5-7 for unique reference pages, and a number greater than 100 for length
       of reference stream.

### Output: 
The program will output the reference stream of unique reference pages, the number of hits and
        the hit ratio of the reference stream to our algorithm.

### To compile and run program, here is what you should do:
1. g++ main.cpp -o a.out
2. ./a.out *(number of physical pages) (number of unique reference pages) (how long you want the reference stream to be)*
    - ex.) ./a.out 3 5 100
