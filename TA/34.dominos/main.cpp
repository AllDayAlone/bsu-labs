#include <iostream>
#include <fstream>
#include <math.h>
#include <algorithm>

using namespace std;

ifstream fin("in.txt");
ofstream fout("out.txt");

class Result {
public:
    long delta;
    long rotated;

    void setState(long delta, long rotated) {
        this->delta = delta;
        this->rotated = rotated;
    }

    void addDomino(long top, long bot) {
        delta += top - bot;
    }

    void addDelta(long delta) {
        this->delta += delta;
    }

    Result(long delta = 0, long rotated = 0) {
        this->delta = delta;
        this->rotated = rotated;
    }
};

class Domino {
public:
    long top;
    long bot;
    long delta;

    void setState(long top, long bot) {
        this->top = top;
        this->bot = bot;
        this->delta = top - bot;
    }
};

Domino* dominos;
Result** results;
long n;

Result* calc(Result res, Domino dom) {
    long noRotateDelta = res.delta + dom.delta;
    long rotateDelta = res.delta - dom.delta;
    if (abs(noRotateDelta) <= abs(rotateDelta)) {
        return new Result(noRotateDelta, res.rotated);
    } else {
        return new Result(rotateDelta, res.rotated + 1);
    }

}

void logDominos() {
    for (long i = 0; i < n; i++) {
        cout << dominos[i].top << " " << dominos[i].bot << " " << dominos[i].delta << endl;
    }
}

int main()
{
    //-Initialization
    fin >> n;
    dominos = new Domino[n];

    long top, bot;
    for (long i = 0; i < n; ++i) {
        fin >> top >> bot;
        dominos[i].setState(top, bot);
    }

    results = new Result*[n];
    for (long i = 0; i < n; ++i) {
        results[i] = new Result[n];
    }

    for (long i = 0; i < n; ++i) {
        results[i][i].addDomino(dominos[i].top, dominos[i].bot);
    }

    long i, j;
    for (long k = 1; k < n; k += 1) {
        for (i = 0, j = k; j < n; i += 1, j += 1) {
            Result* leftDep = calc(results[i][j - 1], dominos[j]);
            Result* downDep = calc(results[i + 1][j], dominos[i]);

            if (leftDep->delta < downDep->delta) {
                results[i][j].setState(leftDep->delta, leftDep->rotated);
            } else if (leftDep->delta == downDep->delta && leftDep->rotated < downDep->rotated) {
                results[i][j].setState(leftDep->delta, leftDep->rotated);
            } else {
                results[i][j].setState(downDep->delta, downDep->rotated);
            }
        }
    }
    /*
    fout << 200000 << endl;
    for (long i = 0; i < 200000; i++)
        fout << i*2 % 7 << " " << (i + 3)% 6234 % 7 << endl;
        */
    fout << results[0][n-1].rotated;

    return 0;
}
