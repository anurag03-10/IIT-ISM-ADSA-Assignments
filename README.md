# ADSA Assignment Solutions (26ET0006)

This folder contains solution files for the assignment problems.

- Q1: Count ones in a matrix — [Q1-Count_Ones_in_Matrix-26ET0006.cpp](Q1-Count_Ones_in_Matrix-26ET0006.cpp)
- Q2: String matching (Brute-force & Rabin-Karp) — [Q2-String_Matching-26ET0006.cpp](Q2-String_Matching-26ET0006.cpp)
- Q3: Strongly Connected Components (Kosaraju) — [Q3-Strongly_Connected_Components-26ET0006.c](Q3-Strongly_Connected_Components-26ET0006.c)
- Sample input for Q3 — [q3_input.txt](q3_input.txt)

Build & run (macOS / Linux):

Q1 (Count ones in matrix)
```bash
g++ -std=c++17 -O2 "Q1-Count_Ones_in_Matrix-26ET0006.cpp" -o Q1
# then run and provide input interactively or pipe sample input
./Q1
```

Q2 (String matching)
```bash
g++ -std=c++17 -O2 "Q2-String_Matching-26ET0006.cpp" -o Q2
# run and type two lines: pattern then document (or pipe sample)
./Q2
```

Q3 (SCC via Kosaraju) — uses a file containing edges
```bash
gcc -std=c11 -O2 "Q3-Strongly_Connected_Components-26ET0006.c" -o Q3
# run and when prompted, give the path to the input file (example below)
printf "%s\n" "q3_input.txt" | ./Q3
```

Notes
- All programs were tested locally and produce the sample outputs provided in the assignment.
- If you want a single combined README with more examples or automated test scripts, tell me and I will add them.
