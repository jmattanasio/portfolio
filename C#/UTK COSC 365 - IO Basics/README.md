## Introduction
This little lab will give you practice with I/O, converting data types, and executing conditional code, such as a switch statement.

You will need to write the entire program, including the class and Main.

## Assignment
Your program will read one (1) line of input. This input will contain three values: (1) operator, (2) left integer, and (3) right integer. All of these values are separated by a single space. The operator is an arithmetic operator in:

+ - addition

- - subtraction

* - multiplication

/ - division

% - modulo

Your job will be to take the given arithmetic operator and apply it to the left and right values. For example, if the input is:

* 2 5
Then the result will be 10, since it is the result of 2 times 5.

### Splitting Strings
The easiest way to split a string is to use the Split method in the String class: https://learn.microsoft.com/en-us/dotnet/api/system.string.split

## Error Handling
If a line can not be read for any reason, output "Error reading line." and quit.

If an operator that is not one of the five (5) listed, output "Invalid operator X.", where X is the operator the user gave you.

If there is no error, output the result.