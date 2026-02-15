## Overview
You will be writing a program that takes a starting number and an ending number and it will in parallel determine the factors for each number. It will then print each number in ascending order and that number’s factors, including 1 and itself.

## Assignment
The staring number will be on the first line. The ending number will be on the second line. You do not need to error check these numbers.

Since none of the numbers will interact with each other, you will need to use Parallel.For to calculate the factors of each number.

When you are finished, output the results as follows:

1.      One number on a line by itself in a right-justified field, 5 characters wide.

2.      A colon and a space.

3.      The factors separated by a space.

## Examples
#### Input
10
20
#### Output
   10: 1 2 5 10
   11: 1 11
   12: 1 2 3 4 6 12
   13: 1 13
   14: 1 2 7 14
   15: 1 3 5 15
   16: 1 2 4 8 16
   17: 1 17
   18: 1 2 3 6 9 18
   19: 1 19
   20: 1 2 4 5 10 20
#### Input
1111
1117
#### Output
 1111: 1 11 101 1111
 1112: 1 2 4 8 139 278 556 1112
 1113: 1 3 7 21 53 159 371 1113
 1114: 1 2 557 1114
 1115: 1 5 223 1115
 1116: 1 2 3 4 6 9 12 18 31 36 62 93 124 186 279 372 558 1116
 1117: 1 1117