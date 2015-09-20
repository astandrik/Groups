#include <iostream>
#include <vector>
#include <list>
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <map>
#include <complex.h>
#include <fstream>
#include "wise_vector.h"
using namespace std;

 ofstream fiterpretations("/home/nastia/workspace/inerpretationd_SIGHT");

const int elements_number = 6;

template<class T>
ostream& operator << (ostream& out, pair<T,T> p) {
    out << "(" << p.first << "," << p.second << ")";
    return out;
};

map<int, unsigned short> global_indexer;

int* multiplication_result(int *left, int * right, int size)
{
    int* result = new int[size];
    for (int i = 0; i < size; ++i) {
        result[i] = right[left[i] - 1];
    }
    return result;
}



bool arrays_equal(int * a, int * b, int size) {
    for (int i = 0; i < size; ++i) {
        if(a[i] != b[i]) return false;
    }
    return true;
}

int array_to_int(int * array){
    int result = 0;
    for (int i = 0; i < elements_number; ++i) {
        result *= 10;
        result += array[i];
    }
    return  result;
}

template <class T>
bool vector_contains(const vector<T>& v, const T& element)  {
    for (int i = 0; i < v.size(); ++i) {
        if(v[i] == element) {
            return true;
        }
    }
    return false;
};

template <class T>
void vector_print (const vector<T>& v) {
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
};

struct Element {
    unsigned short number;
    int * elements;
    Element(int * element) {
        elements = new int[elements_number];
        for (int i = 0; i < elements_number; ++i) {
            elements[i] = element[i];
        }
        number = counter--;
        global_indexer.insert(pair<int, unsigned short>(array_to_int(elements), number));
    }
    static unsigned short counter;
    void print() {
        for(int i = 0; i < elements_number; i++) {
            fiterpretations << *(elements + i) << " ";
        }
        fiterpretations << endl;
    }

};

template <class T>
T& min (T &lhs, T &rhs) {
    return lhs <= rhs ? lhs : rhs;
};



double calculate_complexity(int* elements) {
    vector<vector<int>> chains;
    vector<int> single_chain;
    wise_vector<pair<int,int>> r1;
    wise_vector<pair<int,int>> r2;
    wise_vector<pair<int,int>> r3;
    wise_vector<pair<int,int>> r4;
    wise_vector<pair<int,int>> r5;

    int current_number;
    int next_number;
    for(int i = elements_number -1; i >=0; i--) {
        current_number = i+1;
        single_chain.push_back(current_number);
        next_number = elements[current_number - 1];
        while(current_number != next_number && !vector_contains(single_chain, next_number)) {
            single_chain.push_back(next_number);
            current_number = next_number;
            next_number = elements[current_number - 1];
        }
        if(single_chain.size() != 1) {
            for (int j = 1; j < single_chain.size(); ++j) {
                if(single_chain[j] % 2 == 1 && single_chain[j-1] > single_chain[j] + 1 ||
                        single_chain[j] % 2 == 0 && single_chain[j-1] > single_chain[j]) {
                    r1.push(pair<int,int>(single_chain[j-1], single_chain[j]));
                } else if (single_chain[j] % 2 == 1 && single_chain[j-1] < single_chain[j] ||
                        single_chain[j] % 2 == 0 && single_chain[j-1] < single_chain[j] -1 ) {
                    r2.push(pair<int,int>(single_chain[j-1], single_chain[j]));
                } else if ( single_chain[j] % 2 == 0 && single_chain[j-1] == single_chain[j] - 1 ||
                        single_chain[j] % 2 == 1 && single_chain[j-1] == single_chain[j] + 1) {
                    r5.push(pair<int,int>(single_chain[j-1], single_chain[j]));
                    r5.push(pair<int,int>(single_chain[j], single_chain[j-1]));
                }
                for (int k = j + 1; k < single_chain.size(); ++k) {
                    if(single_chain[j - 1] > single_chain[k]) {
                        r3.push(pair<int,int>(single_chain[j-1], single_chain[k]));
                    } else {
                        r4.push(pair<int,int>(single_chain[j-1], single_chain[k]));
                    }
                }
            }
        }
        single_chain.clear();
    }
    double overall = r1.size() + r2.size() + r3.size() + r4.size() + r5.size();
    double l1 = r1.size() != 0 ? -(r1.size() * ((r1.size() / overall) * log(r1.size()/overall))) : 0;
    double l2 = r2.size() != 0 ? -(r2.size() * ((r2.size() / overall) * log(r2.size()/overall))) : 0;
    double l3 = r3.size() != 0 ? -(r3.size() * ((r3.size() / overall) * log(r3.size()/overall))) : 0;
    double l4 = r4.size() != 0 ? -(r4.size() * ((r4.size() / overall) * log(r4.size()/overall))) : 0;
    double l5 = r5.size() != 0 ? -(r5.size() * ((r5.size() / overall) * log(r5.size()/overall))) : 0;
    if(overall > 0) {
        double result = l1 + l2 + l3 + l4 + l5;
        return result;
    }

}


unsigned short  Element::counter = 0;

unsigned short get_permutation_number(int * el) {
    return global_indexer[array_to_int(el)];
}

unsigned short* vector_to_array(vector<unsigned short> v,int size) {
    unsigned short* buffer = new unsigned short [size];
    for (int i = 0; i < size; ++i) {
        buffer[i] = v[i];
    }
    return  buffer;
}

int factorial(int n) {
    return n > 1 ? n * factorial(n-1): 1;
}

int main()
{
    int* a = new int[elements_number];
    int* tmp = new int[elements_number];
    for (int i = 1; i <= elements_number ; ++i) {
        a[i-1] = i;
    }
    list<Element> permutationElements;
    int size = elements_number;
    Element::counter = factorial(elements_number) - 1;
    do
    {
	for(int k = 0; k < elements_number; k++) {
		tmp[elements_number - 1 - k] = a[k];	
	}
        permutationElements.push_front(Element(tmp));
    } while(std::next_permutation(&a[0], &a[elements_number]));
    list<Element>::iterator p;
    list<Element>::iterator q;
    int overall = 0;
    FILE *f;
    FILE *fcomplex_create;
    FILE *finverse;
    f = fopen("/home/nastia/workspace/MATRIX_RESUL_SIGHT", "wb");
    fcomplex_create = fopen ("/home/nastia/workspace/COMXITIES_SIGHT", "wb");
    finverse = fopen ("/home/nastia/workspace/INVERSE_SIGHT", "wb");
    vector<unsigned short> nums;
    int k = permutationElements.size();

    double complex;
    int nn = 0;
    for (p = permutationElements.begin(); p != permutationElements.end(); ++p) {
        fiterpretations << nn++ << " ";
        p->print();
        complex = calculate_complexity(p->elements);
        fwrite(&complex, sizeof(double), 1, fcomplex_create);
    }
    fclose(fcomplex_create);



    for (p = permutationElements.begin(); p != permutationElements.end(); ++p) {
        bool has_inv = false;
        for (q = permutationElements.begin(); q != permutationElements.end(); ++q) {
            int* mresult = multiplication_result(p->elements, q->elements, size);
            unsigned short permNumber = get_permutation_number(mresult);
            if(!has_inv && permNumber == 0) {
                unsigned short inverse = get_permutation_number(q->elements);
                fwrite(&inverse, sizeof(unsigned short), 1, finverse);
                has_inv = true;
            }
            nums.push_back(permNumber);

            delete [] mresult;
            overall++;
        }
        unsigned short * buffer = vector_to_array(nums, nums.size());
        fwrite(buffer, sizeof(unsigned short), nums.size(), f);
        delete[] buffer;
        nums.clear();
        nums.shrink_to_fit();
        if(overall % 55 == 0)
            cout << overall << endl;

    }
    fclose(finverse);
    fclose(f);
    cout << "End";



    return 0;
}

/*

 */
