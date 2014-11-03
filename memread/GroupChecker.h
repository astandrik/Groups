#include <iostream>
#include <vector>
#include <list>
#include <stdio.h>
#include <algorithm>
#include <map>
#include <math.h>
#include <typeinfo>
#include <unistd.h>

using namespace std;


class Matrix {
    unsigned short* elements;
    int size;
public:
    Matrix(unsigned short* elem, int size): elements(elem), size(size) {}
    inline unsigned short Get(int i, int j) {
        return elements[i * size + j];
    }
};

struct GroupChecker {
    static vector<unsigned short> inverse_elements;
    static void Init(string filename) {
        FILE * finverse = fopen (filename.c_str(), "rb");
        unsigned short inverse;
        while(fread(&inverse, sizeof(unsigned short), 1, finverse)) {
            inverse_elements.push_back(inverse);
        }
        fclose(finverse);
    };
    static bool IsGroup(vector<unsigned short>& elements, int size, Matrix *mult_matrix);
private:
    static bool HasInverse(vector<unsigned short>& elements, int size, Matrix *mult_matrix);
    static bool IsAssociative(vector<unsigned short>& elements, int size, Matrix *mult_matrix);
};

bool GroupChecker::IsGroup(vector<unsigned short>& elements, int size, Matrix *mult_matrix) {
    if(IsAssociative(elements, size, mult_matrix)) return true;
    else return false;
}
bool GroupChecker::IsAssociative(vector<unsigned short>& elements, int size, Matrix *mult_matrix) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                if(mult_matrix->Get(
                        mult_matrix->Get(elements[i], elements[j]),
                        elements[k])
                        !=
                        mult_matrix->Get(elements[i],
                                mult_matrix->Get(elements[j], elements[k]))
                        )
                    return false;

            }
        }
    }
    return true;
}

bool GroupChecker::HasInverse(vector<unsigned short>& elements, int size, Matrix *mult_matrix) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if(mult_matrix->Get(elements[i], elements[j]) == 0 ||
                    mult_matrix->Get(elements[j], elements[i]) == 0) break;
            if (j == size - 1) return false;
        }
    }
    return true;
}

vector<unsigned short> GroupChecker::inverse_elements = vector<unsigned short>();

