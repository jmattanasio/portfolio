Introduction
DRM Encryption is a new kind of encryption. Given an encrypted string (which we will call a DRM message), the decryption process involves three steps: Divide, Rotate and Merge.

Assignment
Write four functions: divide, rotate, merge, and main. The process is described in the following example with the DRM message "EWPGAJRB":

Divide
First, divide the message in half to "EWPG" and "AJRB". You can do this by slicing the string at the halfway point using the length of the string.

Rotate
For each half, calculate its rotation value by summing up the values of each character (A = 0, B = 1, …, Z = 25). For example, the rotation value of "EWPG" is 4+22+15+6=47. Rotate each character in "EWPG" 47 positions forward (wrapping from Z to A when necessary) to obtain the new string "ZRKB". Following the same process on "AJRB" results in "BKSC".

Now, when we rotate, we are going to have to add the rotation value to each character after we determine it. For this, we need to convert each character into an integer, add to it, and convert back into a character.

We need to do the following steps:

First, get the integer value of the character.

Add to it the "shift" amount.

Wrap it around 26, since there are only 26 letters in the alphabet

Convert it back to a character

Merge
The last step is to combine these new strings ("ZRKB" and "BKSC") by rotating each character in the first string by the value of the corresponding character in the second string. For the first position, rotating Z by B means moving it forward 1 character, which wraps it around to A. Continuing this process for every character results in the final decrypted message, "ABCD".

For this step, we need the integer value of the left side and the right side. The left side is the character value and the right side is the shift amount—almost exactly like we did above. However, when we are finished, we will have half the number of characters we started with.

Input
The input contains a single DRM message to be decrypted. There will be no prompts to the user.

Convert all letters to uppercase (using upper method) and make sure there is an even number of characters. If there are not an even number of characters, display the following error message:

Error: The number of characters is not even!
Output
Display the decrypted DRM:

XXYYZZ
Where above, XXYYZZ is replaced with the decrypted message in all uppercase.

Examples
Example 1
Example Input 1
EWPGAJrb
Example Output 1
ABCD
Example 2
Example Input 2
UEQBJPJCBUDGBNKCAHXCVERXUCVK
Example Output 2
ACMECNACONTEST
Example 3
Example Input 3
ABCDEFG
Example Output 3
Error: The number of characters is not even!