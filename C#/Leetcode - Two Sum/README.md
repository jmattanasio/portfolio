## Intuition
Initially I went with a function that found the highest number that was less than the target value. Then upon taking a break and revisiting I realized I only need to realistically check if each number had a matching complement that would add up to make target.

## Approach
I went for two for loops in order to ensure that each element was crosschecked to find complements. I want to refine this as the time complexity was deemed as "brute force." I now see that a hashmap would be exponentially faster.

## Complexity
Time complexity:
On^2