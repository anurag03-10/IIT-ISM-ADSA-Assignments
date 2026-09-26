#include <iostream>
#include <vector>
using namespace std;

int main() {
    int n;
    cout << "Enter the value of n: ";
    if (!(cin >> n)) return 0;

    cout << "Input array:\n";
    vector<vector<int>> matrix(n, vector<int>(n));
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            cin >> matrix[row][col];
        }
    }

    int totalOnes = 0;
    int row = 0;
    int col = n - 1; // start at top-right corner

    // Move left when we see 0, move down when we count 1s.
    while (row < n && col >= 0) {
        if (matrix[row][col] == 1) {
            totalOnes += (col + 1); // columns 0..col in this row are 1
            ++row;                  // go to next row
        } else {
            --col;                  // move left to find 1s
        }
    }

    cout << "Total number of 1's in the matrix: " << totalOnes << endl;
    return 0;
}
