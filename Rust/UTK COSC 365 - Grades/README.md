Introduction
You will write a program that after given a set of grades, calculates the letter grade according to a letter grade chart.

Input
The first input will be an integer that represents the number of numeric grades, n.

The next n lines will contain one grade each as a floating-point number.

Warning
Notice there are no prompts!
Process
Average all of the grades together and then compare them to the letter grade chart below.

Output
Output the average precise to two decimal digits in fixed notation first.

Then, output the letter grade.

Letter Grades
Letter	Cutoff	Letter	Cutoff	Letter	Cutoff
A+

N/A

A

92

A-

88

B+

85

B

82

B-

78

C+

75

C

72

C-

68

D+

65

D

62

D-

58

Anything below a D- (58.0) is an F.

Examples
Example 1
Input
5
55.3
100
91.7
92
69.3
Output
81.66 B-
In this example, there are five grades. The average of 55.3, 100, 91.7, 92, and 69.3 is 81.66. This falls below the B cutoff of 82, so the grade output is B-.

Example 2
Input
2
68.5
71.2
Output
69.85 C-
Example 3
Input
3
55.4
60.2
51.9
Output
55.83 F
Example 4
Input
6
100.5
101.2
107.4
100
100
98.2
Output
101.22 A