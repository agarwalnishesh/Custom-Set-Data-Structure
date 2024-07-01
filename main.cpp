#include <iostream>
#include <set>
#include <unordered_set>
#include <vector>
#include <list>
#include <bitset>
#include <memory>
#include <algorithm>

using namespace std;

const int MAX_SIZE = 1000;

template<typename T>
class SET {
public:
    set<T> ordered_data;
    unordered_set<T> unordered_data;
    list<T> list_data;
    bitset<MAX_SIZE> bitset_data;
    size_t size = 0;

    void sync_bitset() {
        bitset_data.reset();
        for (const auto& elem : ordered_data) {
            if (elem < MAX_SIZE) {
                bitset_data.set(elem);
            }
        }
    }

    int insert(T a) {
        ordered_data.insert(a);
        unordered_data.insert(a);
        if (find(list_data.begin(), list_data.end(), a) == list_data.end()) {
            list_data.push_back(a);
            list_data.sort();
        }
        if (a < MAX_SIZE) {
            bitset_data.set(a);
        }
        return ++size;
    }

    int del(T a) {
        ordered_data.erase(a);
        unordered_data.erase(a);
        list_data.remove(a);
        if (a < MAX_SIZE) {
            bitset_data.reset(a);
        }
        return --size;
    }

    int uni(SET* other) {
        ordered_data.insert(other->ordered_data.begin(), other->ordered_data.end());
        unordered_data.insert(other->unordered_data.begin(), other->unordered_data.end());
        for (const auto& elem : other->list_data) {
            if (find(list_data.begin(), list_data.end(), elem) == list_data.end()) {
                list_data.push_back(elem);
            }
        }
        list_data.sort();
        bitset_data |= other->bitset_data;
        size = ordered_data.size();
        return size;
    }

    int inter(SET* other) {
        set<T> ordered_result;
        unordered_set<T> unordered_result;
        list<T> list_result;
        for (const auto& elem : ordered_data) {
            if (other->contains(elem)) {
                ordered_result.insert(elem);
                unordered_result.insert(elem);
                list_result.push_back(elem);
            }
        }
        ordered_data = ordered_result;
        unordered_data = unordered_result;
        list_data = list_result;
        sync_bitset();
        size = ordered_data.size();
        return size;
    }

    int dif(SET* other) {
        for (const auto& elem : other->ordered_data) {
            ordered_data.erase(elem);
            unordered_data.erase(elem);
            list_data.remove(elem);
        }
        sync_bitset();
        size = ordered_data.size();
        return size;
    }

    int symdif(SET* other) {
        set<T> result;
        for (const auto& elem : ordered_data) {
            if (!other->contains(elem)) {
                result.insert(elem);
            }
        }
        for (const auto& elem : other->ordered_data) {
            if (!contains(elem)) {
                result.insert(elem);
            }
        }
        ordered_data = result;
        unordered_data = unordered_set<T>(result.begin(), result.end());
        list_data = list<T>(result.begin(), result.end());
        sync_bitset();
        size = ordered_data.size();
        return size;
    }

    bool contains(T a) const {
        return ordered_data.find(a) != ordered_data.end();
    }

    void print() const {
        bool first = true;
        for (const auto& elem : ordered_data) {
            if (!first) cout << ",";
            cout << elem;
            first = false;
        }
        cout << "\r\n";
    }
};

int main() {
    int code, data1, data2;
    vector<unique_ptr<SET<int>>> sets;

    while (cin >> code) {
        if (code == 6 || code == 9) cin >> data1;
        else cin >> data1 >> data2;

        while (data1 >= sets.size()) sets.push_back(make_unique<SET<int>>());
        if (code != 6 && code != 9) while (data2 >= sets.size()) sets.push_back(make_unique<SET<int>>());

        switch (code) {
            case 1: cout << sets[data1]->insert(data2) << "\r\n"; break;
            case 2: cout << sets[data1]->del(data2) << "\r\n"; break;
            case 3: cout << (sets[data1]->contains(data2) ? "1" : "0") << "\r\n"; break;
            case 4: cout << sets[data1]->uni(sets[data2].get()) << "\r\n"; break;
            case 5: cout << sets[data1]->inter(sets[data2].get()) << "\r\n"; break;
            case 6: cout << sets[data1]->size << "\r\n"; break;
            case 7: cout << sets[data1]->dif(sets[data2].get()) << "\r\n"; break;
            case 8: cout << sets[data1]->symdif(sets[data2].get()) << "\r\n"; break;
            case 9: sets[data1]->print(); break;
            default: return 0;
        }
    }
}
