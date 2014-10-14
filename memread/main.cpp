#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <algorithm>
#include <map>
#include <math.h>
#include <typeinfo>
#include <unistd.h>
#include <string.h>
#include "GroupChecker.h"
#include <thread>
#include <fstream>
#include "wise_vector.h"

using namespace std;

const int MEGABYTE = 1000000;
const int GIGABYTE =  MEGABYTE * 1000;


bool less_vectors(const vector<unsigned short>& a,const vector<unsigned short>& b) {
    return a.size() < b.size();
}


bool less_vectors2(const vector<pair<int,int>>& a,const vector<pair<int,int>>& b) {
    return a.size() < b.size();
}





list<unsigned short>::iterator p;


void getfullVector(vector<unsigned short>& new_element, Matrix *mult_matr, int full_size, int vector_size, int begin, bool* vectorbitmap) {


    for (int j = begin; j < vector_size; j++) {
        for (int i = 0; i < vector_size; ++i) {
            unsigned short mult_result = (mult_matr->Get(new_element[j], new_element[i]));
            if (vectorbitmap[mult_result] == 0) {
                vectorbitmap[mult_result] = 1;
                new_element.push_back(mult_result);
                vector_size++;
            }
            if (vector_size >= full_size) {
                new_element = vector<unsigned short>();
                i = vector_size + 10;
                j = vector_size + 10;
                break;
            }


        }
    }

}

vector<unsigned short> build_full_group_from_seed(wise_vector<unsigned short> new_element, int seed_size,Matrix* mult_matr, int full_size, bool* vectorbitmap) {
    for (int i = 0; i < new_element.size(); ++i) {
        new_element.push(GroupChecker::inverse_elements[new_element.inner_vector[i]]);
    }
    int vector_size = new_element.inner_vector.size();
    memset(vectorbitmap, 0, full_size);
    for (int k = 0; k < vector_size; ++k) {
        vectorbitmap[new_element.inner_vector[k]] = 1;
    }
    getfullVector(new_element.inner_vector, mult_matr, full_size, vector_size, new_element.inner_vector.size() - 1, vectorbitmap);
    return new_element.inner_vector;
}


int factorial(int n) {
    return n > 1 ? n * factorial(n-1): 1;
}

static     wise_vector<wise_vector<unsigned  short > >groups;

ofstream fs("/home/anton/ClionProjects/Test/results.txt");
bool* vectorbitmap1;
bool* vectorbitmap2;



void build_groups(Matrix *matr, int greater_fact, wise_vector<unsigned short> new_element, int begin) {

    bool* vectorbitmap;
    if(begin == 0) {
        vectorbitmap= vectorbitmap1;
    }
    else if (begin == 1) {
        vectorbitmap = vectorbitmap2;
    }
    for (int i = begin; i < greater_fact; i+=2) {
        //cout<< "current seed value: " << i << endl;
        if(new_element.push(i)) {
            wise_vector<unsigned short> new_l = build_full_group_from_seed(new_element, new_element.size() + 1, matr, greater_fact, vectorbitmap);
            new_element.pop_last();
            new_l.sort();
            if(new_l.size()  > 0) {
                groups.push(new_l);
            }
        }
    }


}



wise_vector<wise_vector<pair<int,int>>> joined_chains;
wise_vector<pair<int,int>> joined_chain;
vector<double> group_complexities;

void get_chains(wise_vector<pair<int,int>>& chains) {
    bool isleaf = true;
    for (int i = 0; i < chains.size(); ++i) {
        if (joined_chain[joined_chain.size() - 1].second == chains[i].first) {
            isleaf  =false;
            joined_chain.push(chains[i]);
            get_chains(chains);
            joined_chain.pop_last();
        }
    }
    if(isleaf && joined_chain[0].first == 0)
    joined_chains.push(joined_chain);
}

void generate_joined_chains(wise_vector<pair<int,int>>& chains) {
    for (int m = 0; m < chains.size(); ++m) {
        joined_chain.push(chains[m]);
        bool isleaf = true;
        for (int i = 0; i < chains.size(); ++i) {
            if (joined_chain[joined_chain.size() - 1].second == chains[i].first) {
                isleaf = false;
                joined_chain.push(chains[i]);
                get_chains(chains);
                joined_chain.pop_last();
            }
        }
        if(joined_chain[0].first == 0 && isleaf)
        joined_chains.push(joined_chain);
        joined_chain.clear();
    }
}

int main()
{
    GroupChecker();
    FILE* fp = fopen("/home/anton/workspace/MATRIX_RESULT_SIX", "rb");


    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    fseek(fp, 0 ,SEEK_SET);
    int shorts_to_read =  filesize/ 2;

    unsigned short * buffer = new unsigned short[shorts_to_read];
    fread(buffer, filesize, 1, fp);
    Matrix* matr = new Matrix(buffer, static_cast<int>(sqrt(shorts_to_read)));
    for(int k = 2; k < 6; k += 2) {
        int fact = factorial(k);
        int greater_fact = factorial(k+2);
        wise_vector<unsigned short> new_element;
        for (int j = 0; j < fact; ++j) {
            new_element.push(j, false);
        }

        int counter = 0;
        vectorbitmap1 = new bool[greater_fact];
        vectorbitmap2 = new bool[greater_fact];

        bool *vectorbitmap = new bool[greater_fact];
        thread thr(build_groups, matr, greater_fact, new_element, 1);
        build_groups(matr, greater_fact, new_element, 0);
        thr.join();
        cout << groups.size() << endl;
        int size_end = groups.size();
        int size_begin = 1;
        while (1) {

            for (int i = size_begin - 1; i < size_end; ++i) {
                thread thr(build_groups, matr, greater_fact, groups[i], 1);
                build_groups(matr, greater_fact, groups[i], 0);
                thr.join();
                cout << groups.size() << endl;
            }
            size_begin = size_end;
            size_end = groups.size();

            cout << "Checkpoint: " << size_begin << endl;
            if (size_begin == size_end) break;
        }
        cout << endl;
        new_element.clear();
        for (int i = 0; i < greater_fact; ++i) {
            new_element.push(i);
        }
        groups.push(new_element);
        new_element.clear();
        for (int i = 0; i < fact; ++i) {
            new_element.push(i);
        }
        groups.push(new_element);
        cout << "TOTAL SIZE: " << groups.size() << endl;
    }
        std::sort(groups.inner_vector.begin(), groups.inner_vector.end(), less_vectors);
        cout<< groups;
        delete [] vectorbitmap1;
        delete [] vectorbitmap2;


    cout << "Calculating complexities: " << endl;
    double complex;
    FILE *fcomplex;
    fcomplex = fopen ("/home/anton/workspace/COMPLEXITIES", "rb");
    vector<double> complexities;
    while(fread(&complex, sizeof(double), 1, fcomplex)) {
        complexities.push_back(complex);
    }
    fclose(fcomplex);





    double group_complexity = 0;
    for (int k = 0; k < groups.size(); ++k) {
        group_complexity = 0;
        for (int i = 0; i < groups[k].size(); ++i) {
            group_complexity += complexities[i];
        }
        group_complexities.push_back(group_complexity / 10.0);
        cout << group_complexity << endl;
    }


    wise_vector<pair<int,int>> chains;




    for (int l = 0; l < groups.size(); ++l) {
        for (int i = 0; i < groups.size(); ++i) {
            if(l!= i && groups[l].contains(groups[i])) {
                chains.push(pair<int,int>(i,l));
            }
        }
    }
    generate_joined_chains(chains);




    std::sort(joined_chains.inner_vector.begin(), joined_chains.inner_vector.end(), less_vectors2);
    vector<double> chain_complexities;
    double sum = 0;
    for (int m = 0; m < joined_chains.size(); ++m) {
        cout<< "(" <<joined_chains[m][0].first << ") ";
        for (int i = 0; i < joined_chains[m].size(); ++i) {
            sum = group_complexities[joined_chains[m][i].second] - group_complexities[joined_chains[m][i].first];
            cout <<" --> " <<sum << " --> (" << joined_chains[m][i].second
                    << ", size: " << groups[joined_chains[m][i].second].size() << " ) ";

        }
        cout << endl;
       // cout << joined_chains[m] << " COMP: " << sum<< endl << endl;
    }
    return 0;
}


/*

 */