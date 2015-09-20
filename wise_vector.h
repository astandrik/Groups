
#include <vector>
#include <typeinfo>

using namespace std;

template<class T>
ostream& operator << (ostream& out, pair<T,T> p) {
    out << "(" << p.first << "," << p.second << ")";
    return out;
};


template <class T>
T& min (T &lhs, T &rhs) {
    return lhs <= rhs ? lhs : rhs;
};

template<class T>
struct wise_vector {
    vector<T> inner_vector;

    long int get_hash_sum() {
        long int sum = 0;
        for (int i = 0; i < inner_vector.size(); i += 1) {
            sum += inner_vector[i] * i;
        }
        return sum;
    }

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


    void fill_default(int size = 1) {
        inner_vector.clear();
        for (int i = 0; i < size; ++i) {
            inner_vector.push_back(i);
        }
    }

    wise_vector(){}
    wise_vector(const vector<T>& v) {
        inner_vector = v;
    }

    wise_vector(const wise_vector<T>& wv) {
        inner_vector = wv.inner_vector;
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
    }

    bool starts_with(const vector<T>& v) {
        if(inner_vector.size() < v.size())return false;
        for (int i = 0; i < v.size(); ++i) {
            if(inner_vector[i] != v[i]) return false;
        }
        return true;
    };

    bool contains(const vector<T>& v) {
        if(inner_vector.size() < v.size()) return false;
        int j = 0;
        for (int i = 0; i < inner_vector.size(); ++i) {
            if (j == v.size()) return true;
            if(inner_vector[i] != v[j] && inner_vector[i] < v[j]) continue;
            if(inner_vector[i] > v[j]) return false;
            if(inner_vector[i] == v[j]) j++;
        }
        if(j==v.size()) return true;
        else return false;
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

