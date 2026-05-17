Purpose
Base 64 is a widely used standard that allows binary data to be encoded using normal ASCII or UTF-8 text. As the name implies, there are 64 symbols that convert to values 0 - 63.

Base 64 is popular when storing cryptographic keys. You can see this if you look at your public keys under $HOME/.ssh, if you’ve created any. These are stored in a text file, and the base 64 encoded values store the binary key data.

Base 64 Encoding
Characters A through Z are used to encode 0 (A) through 25 (Z).

Characters a through z are used to encode 26 (a) through 51 (z).

Characters 0 through 9 are used to encode 52 (0) through 61 (9).

Character + is used to encode 62.

Character / is used to encode 63.

Character = is used as padding to ensure a multiple of 4 characters.

Each Base 64 digit represents 6 bits of data. So, three 8-bit bytes (3×8 bits = 24 bits) can be represented by four 6-bit Base 64 symbols (4×6 = 24 bits).

Since each Base 64 digit represents 6 bits of data, the characters TG9s would decode to:

  T       G       9       s
010011  000110  111101  101100
When regrouped as 8 bits:

01001100  01101111  01101100
  76        111       108
When converting these into chars:

println!("{}{}{}", 76 as char, 111 as char, 108 as char);
This will print:

Lol
Since capital L has the ASCII code 76, lowercase o has the ASCII code 111, and lowercase l has the ASCII code 108.

Assignment
Write a program that takes a file name from the console. This file will be a Base 64 encoded text file. Your job is to read this text file, decode the base 64, and output the original message.

The file will always be a multiple of 4 characters. Those files that need to be extended to make a multiple of 4 will include the padding value '='.

Example
Input File
VGVzdGluZyE=
Output
Testing!