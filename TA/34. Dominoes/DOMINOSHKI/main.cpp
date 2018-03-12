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
    long minAmount = -1;
    long maxAmount = -1;

    Result(long terms, long minAmount, long maxAmount) {
        this->terms = terms;
        this->minAmount = minAmount;
        this->maxAmount = maxAmount;
    };
    Result() {
        this->terms = -1;
        this->minAmount = -1;
        this->maxAmount = -1;
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
        temp.minAmount = -1;
        temp.maxAmount = -1;
        temp.terms = -1;
        dp.push_back(temp);
    }
    dp[OFFSET].terms = 0;
    dp[OFFSET].minAmount = 0;
    dp[OFFSET].maxAmount = 0;

    if (Sum < 0) {
        for (int i = 2; i < 13; i += 2) {
            if (A[i + MINI_OFFSET] <= 0) continue;
           // fout << i << " | ";

           //--------------------------
            for (long j = 0; j <= posSum; j += 2) {
                bool cur = dp[j + OFFSET].maxAmount >= 0;
                bool prev = j - i >= 0 && dp[j - i + OFFSET].maxAmount > 0;
                if (cur && prev) {
                    dp[j + OFFSET].maxAmount = A[i + MINI_OFFSET];   
                    if (dp[j - i + OFFSET].minAmount <= 0) {
                        dp[j + OFFSET].minAmount = 0;
                        dp[j + OFFSET].terms = min(dp[j + OFFSET].terms, dp[j - i + OFFSET].terms + 2);
                    }
                    else  {
                        dp[j + OFFSET].minAmount = dp[j - i + OFFSET].minAmount - 1;
                        dp[j + OFFSET].terms = min(dp[j - i + OFFSET].terms + 1, dp[j + OFFSET].terms);        
                    }     
                          
                } else if (cur) {
                    dp[j + OFFSET].maxAmount = A[i + MINI_OFFSET];
                    dp[j + OFFSET].minAmount = A[i + MINI_OFFSET];
                    //минимальная сумма остается такой же, т.к. нет слагаемых из новой итерации 
                    //максимальная сумма тоже самое
                } else if (prev) {
                    dp[j + OFFSET].maxAmount = dp[j - i + OFFSET].maxAmount - 1;
                    if (dp[j - i + OFFSET].minAmount <= 0) 
                        dp[j + OFFSET].minAmount = 0;
                    else
                        dp[j + OFFSET].minAmount = dp[j - i + OFFSET].minAmount - 1;
                    dp[j + OFFSET].terms = dp[j - i + OFFSET].terms + 1;
                }
            }
           //-----------------------
        }

        for (int i = -2; i > -13; i += -2) {
            if (A[i + MINI_OFFSET] <= 0) continue;
            //fout << i << "\t| ";

             for (long j = posSum; j >= negSum; j -= 2) {
                bool cur = dp[j + OFFSET].maxAmount >= 0;
                bool prev = j - i <= posSum && dp[j - i + OFFSET].maxAmount > 0;
                if (cur && prev) {
                    dp[j + OFFSET].maxAmount = A[i + MINI_OFFSET];   
                    if (dp[j - i + OFFSET].minAmount <= 0) {
                        dp[j + OFFSET].minAmount = 0;
                        dp[j + OFFSET].terms = min(dp[j + OFFSET].terms, dp[j - i + OFFSET].terms + 2);
                    }
                    else  {
                        dp[j + OFFSET].minAmount = dp[j - i + OFFSET].minAmount - 1;
                        dp[j + OFFSET].terms = min(dp[j - i + OFFSET].terms + 1, dp[j + OFFSET].terms);
                    }                   
                } else if (cur) {
                    dp[j + OFFSET].maxAmount = A[i + MINI_OFFSET];
                    dp[j + OFFSET].minAmount = A[i + MINI_OFFSET];
                    //минимальная сумма остается такой же, т.к. нет слагаемых из новой итерации 
                    //максимальная сумма тоже самое
                } else if (prev) {
                    dp[j + OFFSET].maxAmount = dp[j - i + OFFSET].maxAmount - 1;
                    if (dp[j - i + OFFSET].minAmount <= 0) 
                        dp[j + OFFSET].minAmount = 0;
                    else
                        dp[j + OFFSET].minAmount = dp[j - i + OFFSET].minAmount - 1;
                    dp[j + OFFSET].terms = dp[j - i + OFFSET].terms + 1;
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
                bool cur = dp[j + OFFSET].maxAmount >= 0;
                bool prev = j - i <= 0 && dp[j - i + OFFSET].maxAmount > 0;
                if (cur && prev) {
                    dp[j + OFFSET].maxAmount = A[i + MINI_OFFSET];   
                    if (dp[j - i + OFFSET].minAmount <= 0) {
                        dp[j + OFFSET].minAmount = 0;
                        dp[j + OFFSET].terms = min(dp[j + OFFSET].terms, dp[j - i + OFFSET].terms + 2);
                    }
                    else  {
                        dp[j + OFFSET].minAmount = dp[j - i + OFFSET].minAmount - 1;
                        dp[j + OFFSET].terms = min(dp[j - i + OFFSET].terms + 1, dp[j + OFFSET].terms);
                    }               
                } else if (cur) {
                    dp[j + OFFSET].maxAmount = A[i + MINI_OFFSET];
                    dp[j + OFFSET].minAmount = A[i + MINI_OFFSET];
                    //минимальная сумма остается такой же, т.к. нет слагаемых из новой итерации 
                    //максимальная сумма тоже самое
                } else if (prev) {
                    dp[j + OFFSET].maxAmount = dp[j - i + OFFSET].maxAmount - 1;
                    if (dp[j - i + OFFSET].minAmount <= 0) 
                        dp[j + OFFSET].minAmount = 0;
                    else
                        dp[j + OFFSET].minAmount = dp[j - i + OFFSET].minAmount - 1;
                    dp[j + OFFSET].terms = dp[j - i + OFFSET].terms + 1;
                }
            }
        }

        for (int i = 2; i <= 12; i += 2) {
            if (A[i + MINI_OFFSET] <= 0) continue;

            for (long j = negSum; j <= posSum; j += 2) {
                bool cur = dp[j + OFFSET].maxAmount >= 0;
                bool prev = j - i >= negSum && dp[j - i + OFFSET].maxAmount > 0;
                if (cur && prev) {
                    dp[j + OFFSET].maxAmount = A[i + MINI_OFFSET];   
                    if (dp[j - i + OFFSET].minAmount <= 0) {
                        dp[j + OFFSET].minAmount = 0;
                        dp[j + OFFSET].terms = min(dp[j + OFFSET].terms, dp[j - i + OFFSET].terms + 2);
                    }
                    else  {
                        dp[j + OFFSET].minAmount = dp[j - i + OFFSET].minAmount - 1;
                        dp[j + OFFSET].terms = min(dp[j - i + OFFSET].terms + 1, dp[j + OFFSET].terms);
                    }                  
                } else if (cur) {
                    dp[j + OFFSET].maxAmount = A[i + MINI_OFFSET];
                    dp[j + OFFSET].minAmount = A[i + MINI_OFFSET];
                    //минимальная сумма остается такой же, т.к. нет слагаемых из новой итерации 
                    //максимальная сумма тоже самое
                } else if (prev) {
                    dp[j + OFFSET].maxAmount = dp[j - i + OFFSET].maxAmount - 1;
                    if (dp[j - i + OFFSET].minAmount <= 0) 
                        dp[j + OFFSET].minAmount = 0;
                    else
                        dp[j + OFFSET].minAmount = dp[j - i + OFFSET].minAmount - 1;
                    dp[j + OFFSET].terms = dp[j - i + OFFSET].terms + 1;
                }
            }
        }
    }

    prodOut << findClosest(dp, -Sum);
    return 0;
}
