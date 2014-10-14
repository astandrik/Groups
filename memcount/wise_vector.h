
#include <vector>
#include <typeinfo>

using namespace std;


template<class T>
struct wise_vector {
    vector<T> inner_vector;
    bool push(T value, bool check = true) {
        if(check && !vector_contains(&value)) {
            inner_vector.push_back(value);
            return true;
        }
        else if (!check) {
            inner_vector.push_back(value);
            return true;
        }
        return false;

    }

    void pop_last() {
        inner_vector.pop_back();
    }
    T& operator[](int index) {
        return inner_vector[index];
    }

    operator vector<T>& () {
        return inner_vector;
    }

    void sort() {
        if( typeid(inner_vector[0]) != typeid(inner_vector)) {
            std::sort(inner_vector.begin(), inner_vector.end());
        }
    }

    void merge(const vector<T>& v) {
        for (int i = 0; i < v.size(); ++i) {
            if (!vector_contains(v[i])) {
                push(v[i]);
            }
        }
    }

    wise_vector(){}
    wise_vector(const vector<T>& v) {
        inner_vector = v;
    }

    unsigned long size() const {
        return inner_vector.size();
    }

    void clear() {
        inner_vector.clear();
    }

    friend bool operator ==(wise_vector<T>& l1, wise_vector<T>& l2) {
        if(l1.size() != l2.size()) return false;
        for (int i = 0, j  = 0; i < l1.size(), j < l1.size(); ++i, ++j) {
            if (l1[i] != l2[i]) return false;
        }
        return true;
    }

    friend bool operator !=(wise_vector<T>& l1, wise_vector<T>& l2) {
        if(l1.size() != l2.size()) return false;
        for (int i = 0, j  = 0; i < l1.size(), j < l1.size(); ++i, ++j) {
            if (l1[i] != l2[i]) return true;
        }
        return false;
    }

    friend ostream& operator << (ostream & out, wise_vector &v) {
        for (int i = 0; i < v.inner_vector.size(); ++i) {
            out << v[i] << " ";
        }
        out << endl;
        out << endl;
    }
    template <class K>
    bool vector_contains(K* element) {
        typename vector<K>::iterator p;
        for (p = inner_vector.begin(); p != inner_vector.end(); ++p) {
            if(*p == *element) return true;
        }
        return false;
    };
};
