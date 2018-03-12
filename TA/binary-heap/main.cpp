#include <iostream>
#include <fstream>

using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");

bool solve(long long *arr, long long n) {
    long long i = 1;
    while (2*i <= n) {
        if (arr[i] > arr[2*i])
            return 0;
        if (2*i + 1 <= n && arr[i] > arr[2*i + 1])
            return 0;
        i += 1;
    }

    return 1;
}

int main()
{
    long long n;
    fin >> n;

    long long  *arr;
    arr = new long long[n + 1];
    for (long long i = 1; i < n + 1; i += 1) {
        long long numb;
        fin >> numb;
        arr[i] = numb;
    }

    if (solve(arr, n)) {
        fout << "Yes";
    } else {
        fout << "No";
    }
    return 0;
}
