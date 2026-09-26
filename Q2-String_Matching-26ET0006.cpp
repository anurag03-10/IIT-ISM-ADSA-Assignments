#include <iostream>
#include <string>
#include <vector>
using namespace std;

void getInputStrings(string &pattern, string &document) {
    cout << "Enter the word string to match: ";
    getline(cin, pattern);
    cout << "Enter the document string: ";
    getline(cin, document);
}

void printInputs(const string &pattern, const string &document) {
    cout << "Input word string to matched is " << pattern << "\n";
    cout << "Input document string is " << document << "\n";
}

int bruteForceSearch(const string &pattern, const string &document) {
    int m = pattern.size();
    int n = document.size();
    if (m == 0 || n == 0 || m > n) return 0;
    int freq = 0;
    for (int i = 0; i <= n - m; ++i) {
        int j = 0;
        while (j < m && document[i + j] == pattern[j]) ++j;
        if (j == m) ++freq;
    }
    return freq;
}

int rabinKarpSearch(const string &pattern, const string &document) {
    int m = pattern.size();
    int n = document.size();
    if (m == 0 || n == 0 || m > n) return 0;

    const unsigned long long base = 256ULL;
    const unsigned long long mod = 1000000007ULL;

    unsigned long long patternHash = 0, windowHash = 0, power = 1;
    for (int i = 0; i < m; ++i) {
        patternHash = (patternHash * base + (unsigned char)pattern[i]) % mod;
        windowHash = (windowHash * base + (unsigned char)document[i]) % mod;
        if (i > 0) power = (power * base) % mod;
    }

    int freq = 0;
    for (int i = 0; i <= n - m; ++i) {
        if (patternHash == windowHash) {
            bool match = true;
            for (int j = 0; j < m; ++j) {
                if (document[i + j] != pattern[j]) { match = false; break; }
            }
            if (match) ++freq;
        }
        if (i < n - m) {
            unsigned long long left = (unsigned char)document[i];
            unsigned long long right = (unsigned char)document[i + m];
            unsigned long long temp = (mod + windowHash
                                       - (left * power) % mod) % mod;
            windowHash = (temp * base + right) % mod;
        }
    }
    return freq;
}

int main() {
    string pattern, document;
    // Read full lines (may contain spaces)
    getInputStrings(pattern, document);

    printInputs(pattern, document);

    int bfCount = bruteForceSearch(pattern, document);
    cout << "\nSample Output of Brute-Force:\n";
    if (bfCount > 0)
        cout << "String is present in the document with frequency " << bfCount << "\n";
    else
        cout << "String is not present in the document\n";

    int rkCount = rabinKarpSearch(pattern, document);
    cout << "\nSample Output of Rabin-Karp:\n";
    if (rkCount > 0)
        cout << "String is present in the document with frequency " << rkCount << "\n";
    else
        cout << "String is not present in the document\n";

    return 0;
}
