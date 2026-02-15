## Intuition
Well since a negative sign would effectively make the number an automatic non-palindrome, I set one of my criteria that x is not negative.

## Approach
Then all you need is a function to strip and reverse the number int by int. This can be done by modulo 10 from the number, store that, then multiply the reversed number and add that position back onto the number. This is done because of tens places. Adding and removing one int at a time.

## Complexity
Time complexity:
Ran out of free uses but 2ms
