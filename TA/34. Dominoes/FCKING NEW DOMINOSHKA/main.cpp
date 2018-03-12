#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <ctime>
#include <vector>

using namespace std;

ifstream fin("in.txt");
//ofstream fout("devOut.txt");
ofstream prodOut("out.txt");
//ofstream makeHard("in.txt");

long OFFSET;
int MINI_OFFSET = 12;

struct Result {
    long terms = -1;
    long amount = -1;

    Result(long terms, long amount) {
        this->terms = terms;
        this->amount = amount;
    };
    Result() {
        this->terms = -1;
        this->amount = -1;
    }
};

long findClosest(vector<Result> dp, long Sum) {
    if (dp[Sum + OFFSET].terms != -1) return dp[Sum + OFFSET].terms;

    for (int i = 1; i < 13; i += 1) {
        bool plus = dp[Sum + i + OFFSET].terms != -1;
        bool minus = dp[Sum - i + OFFSET].terms != -1;
        if (plus && minus)
            return min(dp[Sum + i + OFFSET].terms, dp[Sum - i + OFFSET].terms);
        else if (plus)
            return dp[Sum + i + OFFSET].terms;
        else if (minus)
            return dp[Sum - i + OFFSET].terms;
    }

    return 0;
}

int main()
{
    // makeHard << "240000" << endl;
    // for (long i = 0; i < 160000; i += 1) {
    //     makeHard << "6 0" << endl;
    // }
    // for (long i = 0; i < 80000; i += 1) {
    //     makeHard << "0 " << i % 7 << endl;
    // }
    // return 0;

    int startTime = clock();

    long n; //количество доминошек
    fin >> n;

    long Sum = 0; // сумма всех доминоше в изначальном состоянии
    long  A[25];    //вклад каждой доминошки в сумму при переворот
    for (int i = 0; i < 25; i += 1) {
        A[i] = 0;
    }

    for (long i = 0; i < n; i += 1) {
        int top, bot;
        fin >> top >> bot;
        if (top == bot) continue;

        Sum += top - bot;
        A[2 * (bot - top) + MINI_OFFSET] += 1;
    }

    if (Sum == 0) {
        prodOut << 0;
        return 0;
    }

    //fout << "Sum: " << Sum << "; ";

    //предположим что sum - отрицательное . тогда пытаемся идти положительными до posSum;
    long posSum = 0;    //сумма положительных вкладов
    for (int i = 2; i < 13; i += 2) {
        posSum += i * A[i + MINI_OFFSET];
    }

    long negSum = 0;    //сумма отрицательных вкладов
    for (int i = -2; i > -13; i += -2) {
        negSum += i * A[i + MINI_OFFSET];
    }

    OFFSET = -negSum;

    vector<Result> dp;
    for (long i = negSum; i <= posSum; i += 1) {
        Result temp;
        temp.amount = -1;
        temp.terms = -1;
        dp.push_back(temp);
    }
    dp[OFFSET].terms = 0;
    dp[OFFSET].amount = 0;

    if (Sum < 0) {
        for (int i = 2; i < 13; i += 2) {
            if (A[i + MINI_OFFSET] <= 0) continue;

            vector<Result> mins;

            for (long j = 0; j <= posSum; j += i) {
                bool cur = dp[j + OFFSET].amount >= 0;
                bool prev = mins.size() > 0;
                if (cur) {
                    mins.push_back(Result(dp[j + OFFSET].terms, A[i + MINI_OFFSET]));
                    
                } else if (j - i >= 0 && dp[j - i + OFFSET].amount > 0) {
                    dp[j + OFFSET].amount = dp[j - i + OFFSET].amount - 1;
                    dp[j + OFFSET].terms == -1 ? dp[j + OFFSET].terms = dp[j - i + OFFSET].terms + 1 : dp[j + OFFSET].terms = min(dp[j - i + OFFSET].terms + 1, dp[j + OFFSET].terms);
                }
            }
        }

        for (int i = -2; i > -13; i += -2) {
            if (A[i + MINI_OFFSET] <= 0) continue;
            //fout << i << "\t| ";

            for (long j = posSum; j >= negSum; j -= 2) {
                if (dp[j + OFFSET].amount >= 0) {
                    dp[j + OFFSET].amount = A[i + MINI_OFFSET];
                    if (j - i <= posSum && dp[j - i + OFFSET].amount > 0 && dp[j - i + OFFSET].terms != -1) {
                        dp[j + OFFSET].terms = min(dp[j - i + OFFSET].terms + 1, dp[j + OFFSET].terms);
                    }
                } else if (j - i <= posSum && dp[j - i + OFFSET].amount > 0) {
                    dp[j + OFFSET].amount = dp[j - i + OFFSET].amount - 1;
                    dp[j + OFFSET].terms == -1 ? dp[j + OFFSET].terms = dp[j - i + OFFSET].terms + 1 : dp[j + OFFSET].terms = min(dp[j - i + OFFSET].terms + 1, dp[j + OFFSET].terms);
                }
             }
            // for (long j = negSum; j < posSum + 1; j += 1) {
            //     if (j == 0) fout << "|";
            //     fout << dp[j + OFFSET].amount << "(" << dp[j + OFFSET].terms << ")\t";
            // }
            // fout << endl;
        }
    } else {
        for (int i = -2; i > -13; i += -2) {
            if (A[i + MINI_OFFSET] <= 0) continue;

            for (long j = 0; j >= negSum; j -= 2) {
             if (dp[j + OFFSET].amount >= 0) {
                    dp[j + OFFSET].amount = A[i + MINI_OFFSET];
                    if (j - i <= 0 && dp[j - i + OFFSET].amount > 0 && dp[j - i + OFFSET].terms != -1) {
                        dp[j + OFFSET].terms = min(dp[j - i + OFFSET].terms + 1, dp[j + OFFSET].terms);
                    }
                } else if (j - i <= 0 && dp[j - i + OFFSET].amount > 0) {
                    dp[j + OFFSET].amount = dp[j - i + OFFSET].amount - 1;
                    dp[j + OFFSET].terms == -1 ? dp[j + OFFSET].terms = dp[j - i + OFFSET].terms + 1 : dp[j + OFFSET].terms = min(dp[j - i + OFFSET].terms + 1, dp[j + OFFSET].terms);
                }
            }
        }

        for (int i = 2; i <= 12; i += 2) {
            if (A[i + MINI_OFFSET] <= 0) continue;

            for (long j = negSum; j <= posSum; j += 2) {
             if (dp[j + OFFSET].amount >= 0) {
                    dp[j + OFFSET].amount = A[i + MINI_OFFSET];
                    if (j - i >= negSum && dp[j - i + OFFSET].amount > 0 && dp[j - i + OFFSET].terms != -1) {
                        dp[j + OFFSET].terms = min(dp[j - i + OFFSET].terms + 1, dp[j + OFFSET].terms);
                    }
                } else if (j - i >= negSum && dp[j - i + OFFSET].amount > 0) {
                    dp[j + OFFSET].amount = dp[j - i + OFFSET].amount - 1;
                    dp[j + OFFSET].terms == -1 ? dp[j + OFFSET].terms = dp[j - i + OFFSET].terms + 1 : dp[j + OFFSET].terms = min(dp[j - i + OFFSET].terms + 1, dp[j + OFFSET].terms);
                }
            }
        }
    }

    prodOut << findClosest(dp, -Sum);
    return 0;
}