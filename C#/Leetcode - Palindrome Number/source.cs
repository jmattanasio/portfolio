public class Solution {
    public static int ReverseNumber(int num)
{
    int reversedNumber = 0;
    while (num != 0)
    {
        int remainder = num % 10; 
        reversedNumber = (reversedNumber * 10) + remainder;
        num /= 10; 
    }
    return reversedNumber;
}
    public bool IsPalindrome(int x) {
        if(x == ReverseNumber(x) && x >= 0){
            return true;
        }
        return false;
    }
}