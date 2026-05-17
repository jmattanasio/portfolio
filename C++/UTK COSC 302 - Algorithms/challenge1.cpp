/* Challenge 01
This program is supposed to take inputs that describe the array it will pass,
and then act on those values. You have the size of the array, the number of
rotations,
and then the direction that those rotations are supposed to take place towards
(either L or R)*/
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
using namespace std;
// Function to rotate the array to the right
void rotateRight(vector<int>& arr, int rotations) {
int size = arr.size();
if (size == 0) return; // Handle the case when the array is empty
rotations = rotations % size; // Normalize rotations
if (rotations > 0) {
vector<int> temp(arr.end() - rotations, arr.end()); // Extract the last
'rotations' elements
arr.erase(arr.end() - rotations, arr.end()); // Remove the last 'rotations'
elements
arr.insert(arr.begin(), temp.begin(), temp.end()); // Insert them at the
beginning
}
}
// Function to rotate the array to the left
void rotateLeft(vector<int>& arr, int rotations) {
int size = arr.size();
if (size == 0) return; // Handle the case when the array is empty
rotations = rotations % size; // Normalize rotations
if (rotations > 0) {
vector<int> temp(arr.begin(), arr.begin() + rotations); // Extract the
first 'rotations' elements
arr.erase(arr.begin(), arr.begin() + rotations); // Remove the first
'rotations' elements
arr.insert(arr.end(), temp.begin(), temp.end()); // Insert them at the end
}
}
// Function to print the elements of the vector
void printVector(const vector<int>& vec) {
for (int num : vec) {
cout << num << " ";
}
cout << endl;
}
int main() {
int num, rotations;
char direction;
// Process input until the end of the file so that regardless it will run
while (cin >> num >> rotations >> direction) {
vector<int> arr(num);
for (int i = 0; i < num; ++i) {
cin >> arr[i];
}
// Call rotation functions based on direction
if (direction == 'R') {
rotateRight(arr, rotations);
} else if (direction == 'L') {
rotateLeft(arr, rotations);
}
// Print the rotated array
printVector(arr);
}
return 0;
}