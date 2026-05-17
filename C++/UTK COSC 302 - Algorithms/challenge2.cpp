//Jonathan Attanasio
//
//Description: This program finds the smallest difference between two elements in the array provided by the input file,
//if there is more than one pair both are printed.

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath> // for abs()

int main() {
    int n;

    // Keep reading until end of input (EOF)
    while (std::cin >> n) { // Read the number of elements
        std::vector<int> numbers(n);

        // Read the elements into the vector
        for(int i = 0; i < n; ++i) {
            std::cin >> numbers[i];
        }

        // Sort the vector
        std::sort(numbers.begin(), numbers.end());

        // Initialize the minimum difference to a large value
        int min_diff = std::abs(numbers[1] - numbers[0]);

        // Vector to store the pairs with the smallest difference
        std::vector<std::pair<int, int>> min_diff_pairs;

        // Traverse the sorted array to find the minimum difference
        for(int i = 1; i < n; ++i) {
            int diff = std::abs(numbers[i] - numbers[i - 1]);
            if(diff < min_diff) {
                min_diff = diff;
                min_diff_pairs.clear(); // Clear the previous pairs
                min_diff_pairs.push_back({numbers[i - 1], numbers[i]}); // Add the new pair
            } else if(diff == min_diff) {
                min_diff_pairs.push_back({numbers[i - 1], numbers[i]}); // Add another pair with the same minimum difference
            }
        }

        // Output all pairs with the smallest difference in array format
        for(size_t i = 0; i < min_diff_pairs.size(); ++i) {
            const auto& pair = min_diff_pairs[i];
            std::cout << pair.first << " " << pair.second;
            if (i < min_diff_pairs.size() - 1) {
                std::cout << " ";
            }
        }
        std::cout << std::endl; // Print a new line after the output to ensure that extra space is gone
    }

    return 0;
}
