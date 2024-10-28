#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>


class MyStringInvalidConversionError : public std::runtime_error { // add_task1
public:
    explicit MyStringInvalidConversionError(const std::string& message)
        : std::runtime_error("Invalid Conversion: " + message) {}
};


class AhoCorasick {
public:
    AhoCorasick(const std::vector<std::string>& patterns) {
        buildGoto(patterns);
        buildFailure();
    }

    std::vector<std::pair<int, std::string>> find(const std::string& text) {
        int state = 0;
        std::vector<std::pair<int, std::string>> results;

        for (int i = 0; i < text.size(); ++i) {
            state = getNextState(state, text[i]);

            for (int patternIndex : out[state]) {
                const std::string& pattern = patterns[patternIndex];
                results.emplace_back(i - pattern.size() + 1, pattern);
            }
        }

        return results;
    }

private:
    std::vector<std::unordered_map<char, int>> trie;
    std::vector<int> fail;
    std::vector<std::vector<int>> out;
    std::vector<std::string> patterns;

    void buildGoto(const std::vector<std::string>& inputPatterns) {
        trie.emplace_back();
        patterns = inputPatterns;

        for (int index = 0; index < patterns.size(); ++index) {
            const std::string& pattern = patterns[index];
            int state = 0;

            for (char c : pattern) {
                if (trie[state].find(c) == trie[state].end()) {
                    trie.emplace_back();
                    trie[state][c] = static_cast<int>(trie.size()) - 1;
                }
                state = trie[state][c];
            }

            if (out.size() <= state) {
                out.resize(trie.size());
            }

            out[state].push_back(index);
        }
    }

    void buildFailure() {
        fail.resize(trie.size(), 0);
        std::queue<int> q;

        for (auto& p : trie[0]) {
            int state = p.second;
            fail[state] = 0;
            q.push(state);
        }

        while (!q.empty()) {
            int state = q.front();
            q.pop();

            for (auto& p : trie[state]) {
                char c = p.first;
                int nextState = p.second;

                int failState = fail[state];
                while (failState != 0 && trie[failState].find(c) == trie[failState].end()) {
                    failState = fail[failState];
                }

                if (trie[failState].find(c) != trie[failState].end()) {
                    fail[nextState] = trie[failState][c];
                }
                else {
                    fail[nextState] = 0;
                }

                out[nextState].insert(out[nextState].end(), out[fail[nextState]].begin(), out[fail[nextState]].end());

                q.push(nextState);
            }
        }
    }

    int getNextState(int state, char c) {
        while (state != 0 && trie[state].find(c) == trie[state].end()) {
            state = fail[state];
        }
        if (trie[state].find(c) != trie[state].end()) {
            return trie[state][c];
        }
        return 0;
    }
};


class MyString {
private:
    template <class TYPE>
    class MS_Iterator {

    private:
        TYPE _ptr;
    public:

        MS_Iterator(TYPE _first) {
            _ptr = _first;
        }

        auto operator*() -> decltype(*_ptr) {
            return *_ptr;
        }

        MS_Iterator& operator++(int) {
            _ptr++;
            return *this;
        }

        MS_Iterator& operator--(int) {
            _ptr--;
            return *this;
        }

        MS_Iterator operator+(int _number) {
            return MS_Iterator(_ptr + _number);
        }

        MS_Iterator operator-(int _number) {
            return MS_Iterator(_ptr - _number);
        }

        MS_Iterator& operator+=(int _number) {
            _ptr += _number;
            return *this;
        }

        MS_Iterator& operator-=(int _number) {
            _ptr -= _number;
            return *this;
        }

        int operator-(const MS_Iterator& _otherIter) {
            return this->_ptr - _otherIter._ptr;
        }

        bool operator!=(const MS_Iterator& other) const {
            return _ptr != other._ptr;
        }

        bool operator==(const MS_Iterator& other) const {
            return _ptr == other._ptr;
        }

        friend std::ostream& operator << (std::ostream& os, MS_Iterator& _obj) {
            return os << _obj._ptr;
        }

    };

    template <class TYPE>
    class MS_Reverse_Iterator { // inherit??

    private:
        TYPE _ptr;

    public:

        //using MS_Iterator<TYPE>::MS_Iterator;
        MS_Reverse_Iterator(TYPE _first) {
            _ptr = _first;
        }

        MS_Reverse_Iterator& operator++(int) {
            _ptr--;
            return *this;
        }

        auto operator*() -> decltype(*_ptr) {
            return *_ptr;
        }

        MS_Reverse_Iterator& operator--(int) {
            _ptr++;
            return *this;
        }

        MS_Reverse_Iterator operator+(int _number) {
            return MS_Reverse_Iterator(_ptr - _number);
        }

        MS_Reverse_Iterator operator-(int _number) {
            return MS_Reverse_Iterator(_ptr + _number);
        }

        MS_Reverse_Iterator& operator+=(int _number) {
            _ptr -= _number;
            return *this;
        }

        MS_Reverse_Iterator& operator-=(int _number) {
            _ptr += _number;
            return *this;
        }

        int operator-(const MS_Reverse_Iterator& _otherIter) {
            return this->_ptr - _otherIter._ptr;
        }

        bool operator!=(const MS_Reverse_Iterator& other) const {
            return _ptr != other._ptr;
        }

        bool operator==(const MS_Reverse_Iterator& other) const {
            return _ptr == other._ptr;
        }

        friend std::ostream& operator << (std::ostream& os, MS_Reverse_Iterator& _obj) {
            return os << _obj._ptr;
        }
    };

    int _size;
    int _capacity;
    char* _str;

public:
    using iterator = MS_Iterator<char*>;
    using const_iterator = MS_Iterator<const char*>;
    using reverse_iterator = MS_Reverse_Iterator<char*>;
    using const_reverse_iterator = MS_Reverse_Iterator<const char*>;

    iterator begin() {
        return iterator(_str);
    }
    iterator end() {
        return iterator(_str + _size);
    }
    const_iterator cbegin() {
        return const_iterator(_str);
    }
    const_iterator cend() {
        return const_iterator(_str + _size);
    }
    reverse_iterator rbegin() {
        return reverse_iterator(_str + _size);
    }
    reverse_iterator rend() {
        return reverse_iterator(_str);
    }
    const_reverse_iterator rcbegin() {
        return const_reverse_iterator(_str + _size);
    }
    const_reverse_iterator rcend() {
        return const_reverse_iterator(_str);
    }


    // - - - CONSTRUCTORS - - - //


    MyString();
    ~MyString();
    MyString(const char[]);
    MyString(const std::string&);
    MyString(const char[], int);
    MyString(int count, char _char);
    MyString(const MyString&);
    MyString(MyString&& moved) noexcept;
    MyString(int); // add_task1
    MyString(double); // add_task1


    // - - - OPERATORS - - - //


    MyString operator+(MyString&);
    MyString operator=(const MyString&);
    MyString operator+(const char _string[]);
    MyString operator+(std::string _string);


    MyString operator+=(const char _string[]);
    MyString operator+=(std::string _string);
    MyString operator+=(MyString&);


    MyString operator=(const char _string[]);
    MyString operator=(std::string _string);
    MyString operator=(char _char);
    char& operator[](int _index);
    MyString& operator=(MyString&& moved) noexcept; // add_task1

    //cmp
    bool operator == (const MyString&);
    bool operator != (const MyString&);
    bool operator > (const MyString&) const;
    bool operator < (const MyString&) const;
    bool operator >= (const MyString&) const;
    bool operator <= (const MyString&) const;
    // 

    friend std::ostream& operator << (std::ostream& os, const MyString&);
    friend std::istream& operator >> (std::istream& in, MyString&);


    // - - - GETERS - - - //


    char* c_str() const {
        return this->_str;
    }

    char* data() const {
        return c_str();
    }

    int length() const {
        return this->_size;
    }

    int size() const {
        return length();
    }

    bool empty() const {
        return !(bool)length();
    }

    int capacity() const {
        return this->_capacity;
    }

    void shrink_to_fit() {
        this->_capacity = _size + 1;
    }

    void clear() {
        this->erase(0, this->size());
    }


    char& at(int _index) const { // add_task1
        if (_index >= _size) {
            throw std::out_of_range("Iteration error");
        }
        return this->c_str()[_index];
    }

    double to_float() const { // add_task1

        bool dot_flag = false;

        for (int i = 0; i < _size; i++) {

            if (((c_str()[i] < '0' || c_str()[i] > '9') && c_str()[i] != '.') || (c_str()[i] == '.' && dot_flag)) {
                throw MyStringInvalidConversionError("Invalid conversation to float\n");
            }
            if (c_str()[i] == '.') dot_flag = true;
        }

        return atof(_str);
    }

    int to_int() { // add_task1
        for (int i = 0; i < size(); i++) {
            if ((c_str()[i] < '0' || c_str()[i] > '9')) {
                throw MyStringInvalidConversionError("Invalid conversation to int\n");
            }
        }
        return atoi(c_str());
    }

    // insert erase replace substr find

    void insert(const_iterator, int count, char _char);
    void insert(const_iterator, const char string[]);
    void insert(const_iterator, const char string[], int count);
    void insert(const_iterator, std::string _string);
    void insert(const_iterator, std::string _string, int count);

    void replace(const_iterator, int count, const char _string[]);
    void replace(const_iterator, int count, std::string _string);

    int find(const char[], const_iterator);
    int find(const std::string, const_iterator);

    MyString substr(const_iterator, int);
    MyString substr(const_iterator);

    void erase(const_iterator _iter, int count);


    // - - - METHODS - - - //

    void insert(int index, int count, char _char);
    void insert(int index, const char string[]);
    void insert(int index, const char string[], int count);
    void insert(int index, std::string _string);
    void insert(int index, std::string _string, int count);

    void replace(int index, int count, const char _string[]);
    void replace(int index, int count, std::string _string);

    void erase(int index, int count);

    int find(const char[]);
    int find(const char[], int);
    int find(const std::string);
    int find(const std::string, int);

    MyString substr(int, int);
    MyString substr(int);

    void append(int count, char _char);
    void append(const char _string[]);
    void append(const char _string[], int _index, int count);
    void append(const std::string _string);
    void append(const std::string _string, int _index, int count);


    std::vector<std::pair<int, std::string>> AHO_FIND(const std::vector<std::string>& patterns) {
        AhoCorasick obj(patterns);
        return obj.find(this->c_str());
    }
};


//class AhoCorasick { // add_task1
//    public:
//        AhoCorasick(const std::string& pattern) {
//            buildGoto(pattern);
//            buildFailure();
//        }
//
//        int find(const std::string& text) {
//            int state = 0;
//            for (int i = 0; i < text.size(); ++i) {
//                state = getNextState(state, text[i]);
//                if (!out[state].empty()) {
//                    return i - out[state][0] + 1;
//                }
//            }
//            return -1;
//        }
//
//    private:
//        std::vector<std::unordered_map<char, int>> trie;
//        std::vector<int> fail;
//        std::vector<std::vector<int>> out;
//        void buildGoto(const std::string& pattern) {
//            trie.emplace_back();
//            int state = 0;
//
//            for (int i = 0; i < pattern.size(); ++i) {
//                char c = pattern[i];
//                if (trie[state].find(c) == trie[state].end()) {
//                    trie.emplace_back();
//                    trie[state][c] = static_cast<int>(trie.size()) - 1;
//                }
//                state = trie[state][c];
//            }
//
//            out.resize(trie.size());
//            out[state].push_back(static_cast<int>(pattern.size()));
//        }
//
//        void buildFailure() {
//            fail.resize(trie.size(), 0);
//            std::queue<int> q;
//
//            for (auto& p : trie[0]) {
//                int state = p.second;
//                fail[state] = 0;
//                q.push(state);
//            }
//
//            while (!q.empty()) {
//                int state = q.front();
//                q.pop();
//
//                for (auto& p : trie[state]) {
//                    char c = p.first;
//                    int nextState = p.second;
//
//                    int failState = fail[state];
//                    while (failState != 0 && trie[failState].find(c) == trie[failState].end()) {
//                        failState = fail[failState];
//                    }
//
//                    if (trie[failState].find(c) != trie[failState].end()) {
//                        fail[nextState] = trie[failState][c];
//                    }
//                    else {
//                        fail[nextState] = 0;
//                    }
//
//                    out[nextState].insert(out[nextState].end(), out[fail[nextState]].begin(), out[fail[nextState]].end());
//
//                    q.push(nextState);
//                }
//            }
//        }
//
//        int getNextState(int state, char c) {
//            while (state != 0 && trie[state].find(c) == trie[state].end()) {
//                state = fail[state];
//            }
//            if (trie[state].find(c) != trie[state].end()) {
//                return trie[state][c];
//            }
//            return 0;
//        }
//};
