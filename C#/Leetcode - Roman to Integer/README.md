## Intuition
I first thought about a switch statement with a for loop because it seemed like the perfect way to solve a regular "descending" roman numeral. The problem came with the subtraction.

## Approach
To deal with the subtraction by placing the 'prev = current;' at the end of the for loop we make a linear scan of sorts. This allows for me to check if the previous is smaller than the current character. If so, subtract and readd, if not just add.

 ## Complexity
Time complexity (1ms runtime):
O(n)

Space complexity:
O(1)