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
ofstream fs("/home/anton/ClionProjects/Test/results_new_EIGHT.txt");
ofstream fgroup("/home/anton/ClionProjects/Test/results_new_groups_EIGHT.txt");


void getfullVector(vector<unsigned short>& new_element, Matrix *mult_matr, int full_size, int vector_size, int begin, bool* vectorbitmap) {

    for (int j = begin; j < vector_size; j++) {
        for (int i = 0; i < vector_size; ++i) {
            unsigned short mult_result1 = (mult_matr->Get(new_element[j], new_element[i]));
            unsigned short mult_result2 = (mult_matr->Get(new_element[i], new_element[j]));
            if (vectorbitmap[mult_result1] == 0) {
                vectorbitmap[mult_result1] = 1;
                new_element.push_back(mult_result1);
                vector_size++;
            }
            if (vectorbitmap[mult_result2] == 0) {
                vectorbitmap[mult_result2] = 1;
                new_element.push_back(mult_result2);
                vector_size++;
            }
            if (vector_size >= full_size) {
                new_element = vector<unsigned short>();
                i = vector_size + 1000;
                j = vector_size + 1000;
                break;
            }
        }
    }


}

vector<unsigned short> build_full_group_from_seed(wise_vector<unsigned short> new_element, Matrix* mult_matr, int full_size, bool* vectorbitmap) {
    for (int i = 0; i < new_element.size(); ++i) {
        new_element.push(GroupChecker::inverse_elements[new_element.inner_vector[i]]);
    }

    int vector_size = new_element.inner_vector.size();
    bzero(vectorbitmap, full_size);
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


void build_groups(Matrix *matr, int greater_fact, wise_vector<unsigned short> new_element, int begin) {

    bool* vectorbitmap = new bool[greater_fact];
    for (int i = begin; i < greater_fact; i+=2) {
        if(new_element.push(i)) {
	cout << "Current element: " << i << " in " << greater_fact << endl;
            wise_vector<unsigned short> new_l = build_full_group_from_seed(new_element, matr, greater_fact, vectorbitmap);
            new_element.pop_last();
            new_l.sort();
            if(new_l.size()  > 0) {
                groups.push(new_l);
            }
        }
    }
    delete[] vectorbitmap;
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

struct FileWorker
{
    long size;
    int elements_number;
    FILE* fp;
    FileWorker(string filename) {
        fp = fopen(filename.c_str(), "rb");
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0 ,SEEK_SET);
        elements_number = size/ 2;
    }

    unsigned short* get_file_as_array() {
        unsigned short* buffer = new unsigned short[elements_number];
        fread(buffer, size, 1, fp);
        return buffer;
    }

};

void count_groups(Matrix *matr);

template <class T>
int elements_in_file(FILE* fp) {
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0 ,SEEK_SET);
    return file_size/ sizeof(T);
}


int main()
{
    string complexities_filename = "/home/anton/workspace/COMPLEXITIES_EIGHT";
    string inverse_filename = "/home/anton/workspace/INVERSE_EIGHT";
    string matrix_filename = "/home/anton/workspace/MATRIX_RESULT_EIGHT";

    GroupChecker::Init(inverse_filename);
    FileWorker fileWorker(matrix_filename);
    unsigned short * buffer = fileWorker.get_file_as_array();

    shared_ptr<Matrix> matrix(new Matrix(buffer, static_cast<int>(sqrt(fileWorker.elements_number))));
    Matrix* matr = &*matrix;
    count_groups(matr);
    std::sort(groups.inner_vector.begin(), groups.inner_vector.end(), less_vectors);

    cout<< groups;
    string to_write = "";
    for (int i1 = 0; i1 < groups.size(); ++i1) {
        to_write = to_string(i1);
        to_write += " ";
        fgroup << to_write;
        fgroup << groups[i1];

    }


    cout << "Calculating complexities: " << endl;
    double complex;
    vector<double> complexities;
    FILE *fcomplex = fopen (complexities_filename.c_str(), "rb");
    while(fread(&complex, sizeof(double), 1, fcomplex)) {
        complexities.push_back(complex);
    }
    fclose(fcomplex);

    double group_complexity;
    for (int k = 0; k < groups.size(); ++k) {
        group_complexity = 0;
        for (int i = 0; i < groups[k].size(); ++i) {
            group_complexity += complexities[groups[k][i]];
        }
        group_complexities.push_back(group_complexity / 10.0);
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
    vector<string> graph_strings;
    string str;
    double sum = 0;
    for (int m = 0; m < joined_chains.size(); ++m) {
        cout<< "(" <<joined_chains[m][0].first << ") ";

        str = to_string(joined_chains[m][0].first);
        str += "," + to_string(groups[joined_chains[m][0].first].size());
        for (int i = 0; i < joined_chains[m].size(); ++i) {
            cout << group_complexities[joined_chains[m][i].first];
            sum = group_complexities[joined_chains[m][i].second] - group_complexities[joined_chains[m][i].first];
            str += "," + to_string(sum) + "," + to_string(joined_chains[m][i].second) + "," + to_string(groups[joined_chains[m][i].second].size());

            cout <<" --> " <<sum << " --> (" << joined_chains[m][i].second
                    << ", size: " << groups[joined_chains[m][i].second].size() << " ) ";

        }
        cout << group_complexities[joined_chains[m][joined_chains[m].size() - 1].second] << endl;
        graph_strings.push_back(str);
       // cout << joined_chains[m] << " COMP: " << sum<< endl << endl;
    }
    for (int n = 0; n < graph_strings.size(); ++n) {
        fs << graph_strings[n] << endl;
    }
    return 0;
}


void count_groups(Matrix *matr) {
    wise_vector<unsigned short> new_element;
    int prev_size = groups.size();
    int size_begin = 0;
    new_element.fill_default(2);
    groups.push(new_element);
    for(int k = 2; k < 8; k += 2) {
        int fact = factorial(k);
        int greater_fact = factorial(k+2);
        cout << groups.size() << endl;
        int size_end = groups.size();
        prev_size = size_end;
        while (1) {

            for (int i = size_begin; i < size_end; ++i) {

                thread thr(build_groups, matr, greater_fact, groups[i], fact + 2);
                build_groups(matr, greater_fact, groups[i], fact + 1);
                thr.join();

                cout << "Groups now: " << groups.size() << endl;
            }
            size_begin = size_end;
            size_end = groups.size();

            if (size_begin == size_end) break;
        }
	size_begin = prev_size;
        new_element.fill_default(greater_fact);
        groups.push(new_element);

        new_element.fill_default(fact);
        groups.push(new_element);

        cout << "TOTAL SIZE: " << groups.size() << endl;
    }
}


/*
 */
