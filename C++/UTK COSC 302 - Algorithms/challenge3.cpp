//Jonathan Attanasio
//
//This program takes an input from the 'input' file and checks to see if it were rearranged could it make the same statement. Not to be
//confused with a palindrome, which is reverse order creates the same word.



#include <iostream>
#include <string>
#include <unordered_map>
#include <cctype>

bool is_palindrome_permutation(const std::string& s) {
    std::unordered_map<char, int> freq_map;

    // Count frequency of each character (ignoring non-alphabetic characters)
    for (char ch : s) {
        if (std::isalpha(ch)) {
            char lower_ch = std::tolower(ch);
            freq_map[lower_ch]++;
        }
    }

    // Check the number of characters with an odd amount of occurences
    int odd_count = 0;
    for (const auto& pair : freq_map) {
        if (pair.second % 2 != 0) {
            odd_count++;
        }
    }

    // A string can be permuted to a palindrome if at most there are 1 letter with an odd amount of occurences. If more it is not a palindrome
    return odd_count <= 1;
}

int main() {
    std::string line;
    while (std::getline(std::cin, line)) {
        // Remove leading/trailing spaces if any.
        // Check if the phrase is a palindrome permutation, by passing it to the true/false function to check if the letters have the correct amounts of
                // each.
        if (is_palindrome_permutation(line)) {
            std::cout << "\"" << line << "\" is a palindrome permutation" << std::endl;
        } else {
            std::cout << "\"" << line << "\" is not a palindrome permutation" << std::endl;
        }
    }
    return 0;
}
