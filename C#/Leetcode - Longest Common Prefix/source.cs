public class Solution {
    public string LongestCommonPrefix(string[] strs) {
        string prefix = strs[0];
        foreach(string word in strs){
            if(word == ""){
                prefix = "";
                break;
            }else{
                for (int i = 0; i < Math.Min(word.Length, prefix.Length); i++){
                    if(word.Length < prefix.Length){
                        prefix = prefix.Substring(0,word.Length);
                    }
                    if(word[i] != prefix[i]){
                        prefix = word.Substring(0,i);
                        break;
                    }
                }
            }
        }
    return prefix;
    }
     
}