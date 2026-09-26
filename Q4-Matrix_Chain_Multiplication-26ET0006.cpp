#include <iostream>
#include <vector>
#include <climits>
using namespace std;

void printOptimalParens(vector<vector<int>>& split, int i, int j) {
    if (i == j) {
        cout << "A" << i;
        return;
    }
    cout << "(";
    printOptimalParens(split, i, split[i][j]);
    printOptimalParens(split, split[i][j] + 1, j);
    cout << ")";
}

int main() {
    int n;
    cout << "Enter number of matrices: ";
    if (!(cin >> n)) return 0;

    vector<int> p(n+1);
    cout << "Enter dimensions (p0 p1 ... pn): ";
    for (int i = 0; i <= n; ++i) cin >> p[i];

    const long long INF = LLONG_MAX / 4;
    vector<vector<long long>> dp(n+1, vector<long long>(n+1, 0));
    vector<vector<int>> split(n+1, vector<int>(n+1, 0));

    for (int len = 2; len <= n; ++len) {
        for (int i = 1; i <= n - len + 1; ++i) {
            int j = i + len - 1;
            dp[i][j] = INF;
            for (int k = i; k < j; ++k) {
                long long cost = dp[i][k] + dp[k+1][j] + (long long)p[i-1]*p[k]*p[j];
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    split[i][j] = k;
                }
            }
        }
    }

    cout << "Min. no. of scalar multiplications: " << dp[1][n] << "\n";
    cout << "Optimal Parenthesization: ";
    printOptimalParens(split, 1, n);
    cout << endl;
    return 0;
}
