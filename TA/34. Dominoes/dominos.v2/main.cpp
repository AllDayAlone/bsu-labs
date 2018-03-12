#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <ctime>

using namespace std;

ifstream fin("in.txt");
ofstream fout("out.txt");

long findMin(map<long, long> map) {
    for (int i = 0; i <= 6; i += 1) {
        long pos = map.count(i);
        long neg = map.count(-i);
        if (pos == 0 && neg == 0)
            continue;
        else if (pos > 0 && neg == 0)
            return map[i];
        else if (pos == 0 && neg > 0)
            return map[-i];
        else if (pos > 0 && neg > 0)
            return min(map[i], map[-i]);
    }
}   

int main()
{
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
    /*
    fout << "dominoes: [";
    for (int i = 0; i < 12; i += 1) {
        fout << dominoes[i] << ", ";
    }
    fout << dominoes[12] << "]\n";
    */

    //..<sum, turns>
    map<long, long> mapST;
    mapST.insert( pair<long, long>(0, 0) );
    int startTime = clock();

    for (int i = 0; i < 13; i += 1) {   
        if (dominoes[i] <= 0) continue;
        map<long, long> newMap;

        for (map<long, long>::iterator it = mapST.begin(); it != mapST.end(); ++it) {
            long count = dominoes[i];
            int domino = i - 6;
            long pos = domino * count + it->first;


            for (long j = 0; j < count + 1; j += 1, pos += -2 * domino) {
                //fout << " |pos = " << pos << "| ";
                if (newMap.count(pos) > 0)
                    newMap[pos] = min(newMap[pos], j + it->second);
                else
                    newMap[pos] = j + it->second;
            }
            
        }
        /*
        for (map<long, long>::iterator it = newMap.begin(); it != newMap.end(); ++it) {
            fout << "(" << it->first << ", " << it->second << ") ";
        }
        fout << endl;
        */

        mapST = newMap;
    }
    /*
    fout << 250000 << endl;
    for (long i = 0; i < 250000; i += 1) {
        fout << "0 " << i % 6 + 1 << endl;
    }*/

    fout << clock() - startTime << endl;

    fout << findMin(mapST);

    return 0;
}
