#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <ctime>

using namespace std;

ifstream fin("in.txt");
ofstream fout("out.txt");

long findMin(pair<long, long> *dp, long S) {
    long koef;
    long winner;
    if (S % 2 == 0) {
        koef = S / 2;
        for (int i = 1; i < 10; koef -= 2, i += 4) {
            if (dp[koef].first > -1 && dp[koef + i].first > -1)
                return min(winner = dp[koef].second, winner = dp[koef + i].second);
            else if (dp[koef].first > -1)
                return winner = dp[koef].second;
            else if (dp[koef + i].first > -1)
                return winner = dp[koef + i].second;    
        }
    } else {
        koef = S / 2 + 1;
        for (int i = 0; i <= 6; i += 2) {
            if (dp[koef - i].first > -1 && dp[koef + i].first > -1)
                return min(winner = dp[koef - i].second, winner = dp[koef + i].second);
            else if (dp[koef - i].first > -1)
                return winner = dp[koef - i].second;
            else if (dp[koef + i].first > -1)
                return winner = dp[koef + i].second;    
        }

    }
}

int main()
{
    int startTime = clock();

    long n;
    fin >> n;

    map<int, long> dominoes;
    long absSum = 0;

    for (int i = -6; i < 7; i += 1) {
        dominoes[i] = 0;
    }

    for (long i = 0; i < n; i += 1) {
        int top, bot;
        fin >> top >> bot;
        if (top == bot) continue;

        absSum += abs(top - bot);
        dominoes[top - bot] += 1;
    }

    pair<long, long> *dp;
    long *positives;
    positives = new long[absSum + 1];
    dp = new pair<long, long>[absSum + 1];
    for (long i = 0; i <= absSum; i += 1) {
        dp[i] = pair<long, long>(-1, 0);
        positives[i] = 0;
    }
    dp[0] = pair<long, long>(0, 0);

    for (int i = -6; i < 7; i += 1) {
        if (dominoes[i] <= 0) continue;

        long count = dominoes[i];
        int val = abs(i);
        int neg = i < 0 ? 1 : 0;
        int pos = i < 0 ? 0 : 1;

        for (long j = 0; j < absSum; j += 1) {
            bool isCurSum = dp[j].first >= 0;
            bool isDeltaSum = j >= val && dp[j - val].first > 0;
            if (isCurSum && isDeltaSum) {
                if (dp[j].second > dp[j-val].second + neg) {
                    dp[j].second = dp[j - val].second + neg;
                    positives[j] = positives[j - val] + pos;
                }
            } else if (isCurSum) {
                dp[j] = pair<long, long>(count, dp[j].second);
            } else if (isDeltaSum) {
                dp[j].first = dp[j - val].first - 1;
                dp[j].second = dp[j - val].second + neg;
                positives[j] = positives[j - val] + pos;
            }
         fout << dp[j].first  << " ";
        }
        fout << endl;
    }

    for (long i = 0; i < n; i += 1) {
        if (dp[i].second > positives[i])
            dp[i].second = positives[i];
    }
    fout << "\nAnswer: " << findMin(dp, absSum);

    //fout << "\nTime: " << clock() - startTime << " ms";

    return 0;
}
