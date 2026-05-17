//Jonathan Attanasio
//This program is supposed to parse through a DNA sequence and find repeated substring within the input.
//Used push_back for input of the sequencing, also made sure the output lines up with desired format.
#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>
#include <string>

//This function is the entire program essentially, it takes in the sequence and finds all 9-letter substrings
//then finds the dupes, and formats the output (lexicographically) in sequence.
void findRepeatedSequences(const std::string &dna) {
    const int SEQ_LENGTH = 9;
    std::unordered_map<std::string, int> sequenceCount;
    std::set<std::string> repeats;

    // Iterate over the DNA string to capture all 9-letter substrings
    for (size_t i = 0; i + SEQ_LENGTH <= dna.size(); ++i) {
        std::string substring = dna.substr(i, SEQ_LENGTH);
        sequenceCount[substring]++;
    }

    // Collect substrings that appear more than once
    for (const auto &entry : sequenceCount) {
        if (entry.second > 1) {
            repeats.insert(entry.first);
        }
    }

    // Output each repeated sequence
    for (const auto &seq : repeats) {
        std::cout << seq << std::endl;
    }
    // Output -1 after each DNA sequence
    std::cout << "-1" << std::endl;
}

int main() {
    std::vector<std::string> dnaSequences;
    std::string dna;

    // Read all DNA sequences from standard input
    while (std::cin >> dna) {
        dnaSequences.push_back(dna);
    }

    // Process each DNA sequence
    for (const auto &dna : dnaSequences) {
        findRepeatedSequences(dna);
    }

    return 0;
}
