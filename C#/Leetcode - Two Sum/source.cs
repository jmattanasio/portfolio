public class Solution {
    public int[] TwoSum(int[] nums, int target) {
        int[] output = new int[2];
        for(int i = 0; i < nums.Length; i++){
            for(int s = 0; s < nums.Length; s++){
                int sum = nums[i] + nums[s];
                if(sum == target && i != s){
            
                    output[0] = i;
                    output[1] = s;
                }
            }
           
        }
        return output;
    } 
}