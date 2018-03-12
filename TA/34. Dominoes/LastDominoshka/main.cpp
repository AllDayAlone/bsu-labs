#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <ctime>

using namespace std;

ifstream fin("in.txt");
ofstream fout("out.txt");
// ofstream makeHard("in.txt");

long findMin(long *dp, long Sum, long n) {
    if (dp[Sum] != n) return dp[Sum];

    for (int i = 1; i <= 6; i += 1) {
        if (dp[Sum + i] != n || dp[Sum - i] != n)
            return min(dp[Sum + i], dp[Sum - i]);
    }
}

int main()
{
    // makeHard << "240000" << endl;
    // for (long i = 0; i < 80000; i += 1) {
    //     makeHard << "0 " << i % 7 << endl;
    // }
    // for (long i = 0; i < 160000; i += 1) {
    //     makeHard << "6 0" << endl;
    // }
    // return 0;
    int startTime = clock();    

    long n, Sum = 0, extra = 0;
    fin >> n;

    long* dominoes = new long[13];
    for (int i = 0; i < 13; i += 1) {
        dominoes[i] = 0;
    }

    for (long i = 0; i < n; i += 1) {
        int top, bot;
        fin >> top >> bot;

        dominoes[top - bot + 6] += 1;
    }

    for (int i = 0; i < 13; i += 1) {
        Sum += (i - 6) * dominoes[i];
    }

    while (Sum < -300) {
        int k = -1;
        for (int i = 0; i < 6; i += 1) {
            if (dominoes[i] <= 15) continue;
            k = i;
            break;
        }
        if (k == -1) break;

        Sum -= 2 * (k - 6);
        extra += 1;
        dominoes[k] -= 1;
        dominoes[12 - k] += 1;
    }

    while (Sum > 300) {
        int k = -1;
        for (int i = 12; i > 6; i -= 1) {
            if (dominoes[i] <= 15) continue;
            k = i;
            break;
        }
        if (k == -1) break;

        Sum -= 2 * (k - 6);
        extra += 1;
        dominoes[k] -= 1;
        dominoes[12 - k] += 1;
    }

    int lim = max(abs(Sum), 50l);

    long **dp;
    dp = new long*[n + 1];
    for (long i = 0; i <= n; i += 1) 
        dp[i] = new long[2 * lim + 1];

    for (long i = 0; i <= n; i += 1)
        for (long j = 0; j < 2 * lim + 1; j += 1) {
            dp[i][j] = n;
        }

    dp[0][Sum + lim] = extra;

    for (long i = 1; i <= n; i += 1) {
        long domino = -10;
        for (int k = 0; k < 13; k += 1) {
            if (dominoes[k] == 0) continue;

            dominoes[k] -= 1;
            domino = k - 6;
            break;
        } 
        if (domino == -10) break;
        long dif = -2 * domino;

        for (int j = -lim; j <= lim; j += 1) {
            if (dp[i - 1][j + lim] == n) continue;
            
            dp[i][j + lim] = min(dp[i - 1][j + lim], dp[i][j + lim]);
            if (j + dif >= -lim && j + dif <= lim)
                dp[i][j + dif + lim] = min(dp[i - 1][j + lim] + 1, dp[i - 1][j + dif + lim]);
        }

        // for (int j = -lim; j <= lim; j += 1) {
        //     if (j == 0) fout << "|";
        //     fout << dp[i][j + lim] << "\t";
        // }
        // fout << endl;
    }

    //fout << "Time: " << clock() - startTime << "ms\n";
    fout << findMin(dp[n], lim, n);

    return 0;
}
