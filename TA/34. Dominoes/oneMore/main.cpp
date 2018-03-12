#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <ctime>

using namespace std;

ifstream fin("in.txt");
ofstream fout("out.txt");
// ofstream makeHard("in.txt");

long findMin(long *dp, long Sum) {
    if (dp[Sum] != -1) return dp[Sum];

    for (int i = 1; i <= 6; i += 1) {
        long pos = dp[Sum + i];
        long neg = dp[Sum -i];
        if (pos == 0 && neg == 0)
            continue;
        else if (pos > 0 && neg == 0)
            return dp[Sum + i];
        else if (pos == 0 && neg > 0)
            return dp[Sum -i];
        else if (pos > 0 && neg > 0)
            return min(dp[Sum + i], dp[Sum -i]);
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

    long n;
    fin >> n;

    long* dominoes = new long[13];
    for (int i = 0; i < 13; i += 1) {
        dominoes[i] = 0;
    }

    for (long i = 0; i < n; i += 1) {
        int top, bot;
        fin >> top >> bot;
        if (top == bot) continue;

        dominoes[top - bot + 6] += 1;
    }

    long Sum = 0; 
    long extra = 0;

    for (int i = 0; i < 13; i += 1) {
        Sum += (i - 6) * dominoes[i];
    }

    for (int i = 0; i < 13; i += 1) {
        fout << dominoes[i] << " ";
    }
    fout << endl;

    while (Sum < -150) {
        int k = -1;
        for (int i = 0; i < 6; i += 1) {
            if (dominoes[i] <= 10) continue;
            k = i;
            break;
        }
        if (k == -1) break;

        Sum -= k - 6;
        extra += 1;
        dominoes[k] -= 1;
    }

    while (Sum > 150) {
        int k = -1;
        for (int i = 12; i > 6; i -= 1) {
            if (dominoes[i] <= 10) continue;
            k = i;
            break;
        }
        if(k == -1) break;

        Sum -= k - 6;
        extra += 1;
        dominoes[k] -= 1;
    }

    for (int i = 0; i < 13; i += 1) {
        fout << dominoes[i] << " ";
    }
    fout << endl;

    for (int i = 1; i < 7; i += 1) {
        while (dominoes[6 + i] > 20 && dominoes[6 - i] > 20) {
            dominoes[6 + i] -= 10;
            dominoes[6 - i] -= 10;
        }
    }

    for (int i = 0; i < 13; i += 1) {
        fout << dominoes[i] << " ";
    }
    fout << endl;

    Sum = max(abs(Sum), 200l);
    

    //..<sum, turns>
    long **dp;
    dp = new long*[14];
    for (int i = 0; i < 14; i += 1) {
        dp[i] = new long[2 * Sum + 1];
    }
    
    for (int i = 0; i < 14; i += 1) {
        for (long j = 0; j < 2 * Sum + 1; j += 1) {
            dp[i][j] = -1;
        }
    }

    dp[0][Sum] = 0;
    
    // map<long, long> mapST;
    // mapST.insert( pair<long, long>(0, 0) );
    int startTime = clock();

    for (int i = 0; i < 13; i += 1) {
        if (dominoes[i] <= 0)  {
            for (long k = -Sum; k <= Sum; k += 1) 
                dp[i + 1][k + Sum] = dp[i][k + Sum];
            continue;
        }
        //map<long, long> newMap;

        for (long k = -Sum; k <= Sum; k += 1) {
            if (dp[i][k + Sum] == -1) continue;
            long count = dominoes[i];
            int domino = i - 6;
            long pos = domino * count + k;
            long j = 0;

            for ()
                while(pos >= -Sum && pos <= Sum && j < count + 1) {
                if (dp[i + 1][pos + Sum] != -1)
                    dp[i + 1][pos + Sum] = min(dp[i + 1][pos + Sum], j + dp[i][k + Sum]);
                else
                    dp[i + 1][pos + Sum] = j + dp[i][k + Sum];
                pos += -2 * domino;
            }
            
        }
    }

    fout << "Time: " << clock() - startTime << "ms\n";
    fout << extra + findMin(dp[13], Sum);

    return 0;
}