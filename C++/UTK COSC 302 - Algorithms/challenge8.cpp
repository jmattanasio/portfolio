//Jonathan Attanasio
//This program is a dynamic one that is being used to take two strings similar to DNA
//sequences and determine the best way to match, mismatch, or miss the alignment. The total
//score comes from the diagonal like nature of the table.

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

// Function to perform global alignment using dynamic programming
int global_alignment(const std::string& S1, const std::string& S2) {
    int m = S1.length();
    int n = S2.length();

    // Initialize table with zeroes
    std::vector<std::vector<int>> F(m + 1, std::vector<int>(n + 1, 0));

    // Base cases 
    for (int i = 1; i <= m; ++i) {
        F[i][0] = F[i - 1][0] - 1;  // Gaps in S2 (deleting all from S1)
    }
    for (int j = 1; j <= n; ++j) {
        F[0][j] = F[0][j - 1] - 1;  // Gaps in S1 (deleting all from S2)
    }

    // Fill the table based on the recurrence relation
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            int match = (S1[i - 1] == S2[j - 1]) ? 1 : -1; // Match or mismatch score
            F[i][j] = std::max({ 
                F[i - 1][j - 1] + match,  // Match/mismatch case
                F[i - 1][j] - 1,          // Gap in S2
                F[i][j - 1] - 1           // Gap in S1
            });
        }
    }

    // The final score is in the bottom right of the table
    return F[m][n];
}

int main() {
    std::string S1, S2;

    // Read two sequences from standard input 
    std::getline(std::cin, S1);  // First sequence
    std::getline(std::cin, S2);  // Second sequence

    // Compute the optimal alignment score
    int score = global_alignment(S1, S2);

    // Output the result as a number (without any text)
    std::cout << score << std::endl;

    return 0;
}


