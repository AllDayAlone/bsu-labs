#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>

std::ifstream fin("report.in");
std::ofstream fout("report.out");
// std::ofstream testIn("testIn.txt");

const int HUGE = 200000;

void printVector(std::vector<int> arr) {
    for (int i = 0; i < arr.size(); i += 1) {
        fout << arr[i] + 1 << " ";
    }
    fout << std::endl;
}

std::vector<int> findLIS(int *arr, int n, int k) {
    std::vector<int> d;         // последний элемент посл-ти длины i
    std::vector<int> indexList; // индекс поледнего элемента посл-ти длины i
    std::vector<int> prevList;  // предок элемента под индексом i

    d.push_back(-1);
    indexList.push_back(-1);
    for (int i = 1; i <= n; i += 1) {
        d.push_back(HUGE);    
        indexList.push_back(-1);    
    }

    for (int i = 0; i < n; i += 1) {       
        prevList.push_back(-1);
    }

    for (int i = 0; i < n; i += 1) {
        int j = int (std::upper_bound (d.begin(), d.end(), arr[i]) - d.begin());
        if (d[j - 1] < arr[i] && arr[i] < d[j]) {
            d[j] = arr[i];
            indexList[j] = i;
            prevList[i] = indexList[j - 1];
        }
    }
    // std::cout << "prevList: ";
    // printVector(prevList);
    // std::cout << "k= " << k << std::endl;
    // std::cout << "indexList: ";
    // printVector(indexList);

    std::vector<int> answer;
    int it = indexList[k];
    while (it != -1) {
       answer.push_back(it);
       it = prevList[it];
    }

    std::reverse(answer.begin(), answer.end());

    return answer;
}

int main()
{
    // for (int test = 1; test <= 100000; test += 1) {
    //     int n = 10;

    //     int *Arr, *Rev;
    //     Arr = new int[n];
    //     Rev = new int[n];

    //     for (int i = 0; i < 10; i += 1) {
    //         int numb = (rand() % 10) + 1;
    //         testIn << numb << " ";
    //         Rev[n - i - 1] = Arr[i] = numb;
    //     }
    //     testIn << std::endl;
        int n;
        fin >> n;

        int *Arr, *Rev;
        Arr = new int[n];
        Rev = new int[n];

        for (int i = 0; i < n; i += 1) {
            int temp;
            fin >> temp;

            Arr[i] = temp;
            Rev[n - i - 1] = temp;
        }    

        std::vector<int> dl; // элемент, на который оканчивается lis длины i
        std::vector<int> dl_k;  // максимальная длина последовательности, оканчивающаяся на i-ом элементе
        std::vector<int> dr;
        std::vector<int> dr_k;

        dl.push_back(-1);
        dr.push_back(-1);
        for (int i = 1; i <= n; i += 1) {
            dl.push_back(HUGE);
            dr.push_back(HUGE);
        }

        for (int i = 0; i < n; i += 1) {
            dl_k.push_back(-1);
            dr_k.push_back(-1);
        }

        int maxL = 0, maxR = 0;

        for (int i = 0, iRev = n - 1; i < n; i += 1, iRev -= 1) {
            int j = int (std::upper_bound (dl.begin(), dl.end(), Arr[i]) - dl.begin());

            if (dl[j - 1] < Arr[i] && Arr[i] < dl[j]) {
                if (dl[j] == HUGE) maxL += 1;
                dl[j] = Arr[i];
            }


            int k = int (std::upper_bound (dr.begin(), dr.end(), Arr[iRev]) - dr.begin());
            if (dr[k - 1] < Arr[iRev] && Arr[iRev] < dr[k]) {
                if (dr[k] == HUGE) maxR += 1;
                dr[k] = Arr[iRev];
            }

            dl_k[i] = maxL;
            dr_k[iRev] = maxR;

            // std::cout << i << " ";
            // printVector(dr);
        }
        // printVector(dl_k);
        // printVector(dr_k);

        int k = 0, // длина возрастающей и убывающей посл-ти (считая серединный элемент)
            pos = 0;    // позиция(в 0-нотации) серединного элемента
        for (int i = 0; i < n; i += 1) {
            int cur = std::min(dl_k[i], dr_k[i]);
            if (k < cur) {
                k = cur;
                pos = i;
            }
        }

        //fout << k - 1 /*<< " " << pos*/ << std::endl;     

        std::vector<int> incAns = findLIS(Arr, pos + 1, k);    

        // std::cout << "Rev: ";
        // for (int i = 0; i < n; i += 1) {
        //     std::cout << Rev[i] << " ";
        // } 
        // std::cout << "miniN: " << n - pos << std::endl;

        std::vector<int> decAns = findLIS(Rev, n - pos, k); 

        for (int i = 0; i < decAns.size(); i += 1) {
            decAns[i] = n - 1 - decAns[i];
        }

        std::reverse(decAns.begin(), decAns.end());

        std::vector<int> answer;

        for (int i = 0; i < k - 1; i += 1) {
            //fout << incAns[i] + 1 << " ";
            answer.push_back(incAns[i]);
        }

       // fout << std::max(incAns[incAns.size() - 1], decAns[0]) + 1 << " ";
        if (Arr[incAns[incAns.size() - 1]] > Arr[decAns[0]])
            answer.push_back(incAns[incAns.size() - 1]);
        else
            answer.push_back(decAns[0]);

        for (int i = 1; i < k; i += 1) {
            // fout << decAns[i] + 1 << " ";
            answer.push_back(decAns[i]);
        }
        
        fout << k - 1 << std::endl;
        printVector(answer);

        // fout << std::endl;

    //     bool flagOk = true;
    //     for (int i = 0; i < k - 1; i += 1) {
    //         if (Arr[answer[i + 1]] <= Arr[answer[i]] )
    //             flagOk = false;
    //     }
    //     for (int i = k - 1; i < answer.size() - 1; i += 1) {
    //         if (Arr[answer[i + 1]] >= Arr[answer[i]] )
    //             flagOk = false;
    //     }
    //     if (flagOk)
    //         ;//fout << "OK";
    //     else {
    //         for (int i = 0; i < n; i += 1) {
    //             fout << Arr[i] << " ";
    //         }
    //         fout << std::endl;
    //         fout << k - 1 << std::endl;
    //         printVector(incAns);
    //         printVector(decAns);
    //         printVector(answer);
    //         return 0;
    //     }
    //     delete [] Arr; 
    //     delete [] Rev;
    // }
    
    return 0;
}
