public class Solution {
    static int IntConversion(char c){
        int num = 0;
        switch(c){
            case 'I':
                num = 1;
                break;
            case 'V':
                num = 5;
                break;
            case 'X':
                num = 10;
                break;
            case 'L':
                num = 50;
                break;
            case 'C':
                num = 100;
                break;
            case 'D':
                num = 500;
                break;
            case 'M':
                num = 1000;
                break;
        }
        return num;
    }

    public int RomanToInt(string s) {
        int sum = 0;
        int prev = 0;
        int current = 0;
        for (int i = 0; i < s.Length; i++){

            current = IntConversion(s[i]);
            if(prev < current){
                sum -= prev;
                sum += current - prev;
            }else{
            sum += current;
            }
            prev = current;
            
        }
        return sum;
    }
}