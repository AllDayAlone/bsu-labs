#include <iostream>
#include <fstream>
#include <algorithm>
#include <new>

using namespace std;

ifstream fin("in.txt");
ofstream fout("out.txt");

unsigned long n, S = 0;
int *dominoes;


long solve() {
    long* oldS = new long[2*S + 1];
    long* newS = new long[2*S + 1];
    for (long i = 0; i <= 2*S; i += 1) {
        if (i == dominoes[0] + S)
            oldS[i] = 0;
        else if (i == -dominoes[0] + S)
            oldS[i] = 1;
        else
            oldS[i] = -1;
        newS[i] = -1;
    }
    /*
    for (int i = 0; i < 25; i += 1)
        fout << oldS[i] << " ";
    fout << endl;
    */


    for (unsigned long i = 1; i < n; i += 1) {
        int Numb = dominoes[i];
        for (int j = -S; j < S + 1; j += 1) {
            if (oldS[j + S] >= 0) {
                if (newS[j + Numb + S] == -1)
                    newS[j + Numb + S] = oldS[j + S];
                else
                    newS[j + Numb + S] = min(newS[j + Numb + S], oldS[j + S]);

                if (newS[j - Numb + S] == -1)
                    newS[j - Numb + S] = oldS[j + S] + 1;
                else
                    newS[j - Numb + S] = min(newS[j - Numb + S], oldS[j + S] + 1);
            }
        }

        for (int k = 0; k < 2*S + 1; k += 1) {
            oldS[k] = newS[k];
            newS[k] = -1;
        }
        /*
        for (int k = 0; k < 25; k += 1)
            fout << oldS[k] << " ";
        fout << endl;*/
    }

    for (int i = 0; i <= 6; i += 1) {
        long pos = oldS[S + i];
        long neg = oldS[S - i];
        if (pos == -1 && neg == -1)
            continue;
        else if (pos != -1 && neg == -1)
            return pos;
        else if (pos == -1 && neg != -1)
            return neg;
        else if (pos != -1 && neg != -1)
            return min(pos, neg);
    }
    return 0;
};

int main() {
    fin >> n;

    dominoes = new int[n];
    for (unsigned long i = 0; i < n; i += 1) {
        int top, bot;
        fin >> top >> bot;

        dominoes[i] = top - bot;
        S += abs(dominoes[i]);
    }

    /*
    for (int i = 0; i < 13; i++) {
        dominoes[i] = 0;
    }

    for (unsigned long i = 0; i < n; i += 1) {
        int top, bot;
        fin >> top, bot;

        if (top == bot) continue;
        dominoes[top - bot + 6] += 1;
    }
     ?????
    for (int i = 1; i <= 6; i += 1) {
        if (dominoes[6 - i] != 0 && dominoes[6 + i] != 0) {
            unsigned long knockedOut = abs(dominoes[6 - i] - dominoes[6 + i]);
            dominoes[6 - i] -= knockedOut;
            dominoes[6 + i] -= knockedOut;
        }
    }
    fout << 250000 << endl;
    for (long i = 0; i < 125000; i += 1)
        fout << "6 0" <<  endl;
    for (long i = 0; i < 125000; i += 1)
        fout << "0 6" <<  endl;*/
    fout << solve();

    return 0;
}
