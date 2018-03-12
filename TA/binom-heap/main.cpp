#include <iostream>
#include <fstream>
#include <vector>

std::ifstream fin("input.txt");
std::ofstream fout("output.txt");

int main()
{
    long long n;
    fin >> n;

    long long twoD = 1;
    long counter = 0;
    do {
        twoD *= 2;
        counter += 1;
    } while(twoD <= n);
    counter -= 1;
    twoD /= 2;

    std::vector<long> out;
    while (n > 0) {
        if (n >= twoD) {
            n -= twoD;
            out.push_back(counter);
        }
        twoD /= 2;
        counter -= 1;
    }

    for (long i = out.size() - 1; i >= 0; i -= 1) {
        fout << out[i] << std::endl;
    }
    return 0;
}
