#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");

struct Matrix {
    int n;
    int m;
};

long **F;
Matrix* arr;

long solve(int i, int j) {
    if (F[i][j]  != -1) return F[i][j];
    if (i == j) return 0;
    if (j == i + 1) return arr[i].n * arr[i].m * arr[j].m;

    long Min = solve(i, i) + solve(i + 1, j) + arr[i].n * arr[i].m * arr[j].m;
    for (int k = i + 1; k < j;  k+= 1) {
        long Cur = solve(i, k) + solve(k + 1, j) + arr[i].n * arr[k].m * arr[j].m;
        if (Cur < Min) Min = Cur;
    } 
    F[i][j] = Min;
    return Min;

}

int main()
{
    int n;
    fin >> n;
    F = new long*[n];
    for (int i = 0; i < n; i += 1) {
        F[i] = new long[n];
    }
    for (int i = 0; i < n; i += 1) {
        for (int j = 0; j < n; j += 1) {
            F[i][j] = -1;
        }
        F[i][i] = 0;
    }

    arr = new Matrix[n];
    for (int i = 0; i < n; i += 1) {
        fin >> arr[i].n >> arr[i].m;
    }

    fout << solve(0, n - 1); 

    return 0;
}
