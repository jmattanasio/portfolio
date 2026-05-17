Introduction
This little lab will give you practice with I/O, converting data types, and moving data within lists.

Assignment
Your program will read one (1) line of input. This will be made up of one or more words separated by a space, but it will all be on the same line.

Afterwards, you will convert the sentence into a pig latin sentence using the following rules:

If the word starts with a vowel (aeiou)

Simply add "yay" to the end of it.

Example: iterator becomes iteratoryay

If the word starts with a consonant (not aeiou)

Take all of the letters before the first vowel and move them to the end of the word.

Add "ay".

Example: john becomes ohnjay

Example: that becomes atthay

You will need to apply these rules to every word in the sentence. The rules are case insensitive. Case insensitivity can be applied by comparing all characters after it is ran through the toLower function, which is part of the Data.Char library.

Examples
Adam said you did not use our fan
becomes

Adamyay aidsay ouyay idday otnay useyay ouryay anfay
Splitting Strings
You will need to convert a sentence, which is a String into a list of String data types containing one word each. Luckily, Haskell has the words function which will convert one String into a list of Strings by splitting it by using the space as a delimiter. Analagous to that, you can convert a list of Strings back to a single String by using unwords.

Error Handling
You do not need to handle malformed data.