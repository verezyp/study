#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include "MyString.h"
#include <sstream>


//---------------------------------------------------------------------------

// ------------------------------- CONSTRUCTORS -----------------------------

//---------------------------------------------------------------------------


MyString::~MyString() {
    delete _str;
    _str = NULL;
    _size = 0;
    _capacity = 0;
}


MyString::MyString() {
    _size = 0;
    _capacity = 3;
    _str = new char[_capacity] {};
}


MyString::MyString(const char string[]) {
    if (string != NULL) {
        int new_size = static_cast<int>(strlen(string)) + 1;
        _capacity = new_size;
        _str = new char[_capacity] {};
        strcpy(_str, string);
        _size = new_size - 1;
    }
    else {
        new (this)MyString();
    }
}


MyString::MyString(const std::string& string) :MyString(string.c_str()) {}


MyString::MyString(const char string[], int count) {

    if (strlen(string) < count || count < 0) {
        throw MyStringInvalidConversionError("Constructor : bad args");
    }

    int new_size = count + 1;
    _capacity = new_size;
    _str = new char[_capacity] {};
    strncpy(_str, string, count);
    _str[count] = '\0';
    _size = new_size - 1;
    _capacity = new_size;
}


MyString::MyString(int count, char _char) {

    if (count < 0) {
        throw MyStringInvalidConversionError("Constructor : bad args");
    }

    int new_size = count + 1;
    _capacity = new_size;
    _str = new char [_capacity] {};
    memset(_str, _char, count);
    _str[count] = '\0';
    _size = new_size - 1;

}


MyString::MyString(const MyString& obj) :MyString(obj._str) {
    _capacity = obj._capacity;
}


MyString::MyString(MyString&& moved) noexcept {
    _str = NULL;
    *this = std::move(moved);

}


MyString::MyString(int _number) :MyString(std::to_string(_number)) {

}


MyString::MyString(double _number) :MyString(std::to_string(_number)) {

}


//---------------------------------------------------------------------------

//------------------------------- METHODS -----------------------------------

//---------------------------------------------------------------------------


void MyString::insert(int index, int count, char _char) {

    char* tmp_ptr = new char [count + 1] {};
    memset(tmp_ptr, _char, count);
    tmp_ptr[count] = '\0';
    MyString::insert(index, tmp_ptr, count);
    delete[] tmp_ptr;
    tmp_ptr = NULL;
}


void MyString::insert(int index, const char string[]) {
    insert(index, string, static_cast<int>(strlen(string)));
}


void MyString::insert(int index, const char string[], int count) {

    if (index > size()) {
        throw std::out_of_range("Insert: index out\n");
    }
    if (strlen(string) < count) {
        throw std::out_of_range("Insert: count out\n");
    }
    if (count < 0) {
        throw std::out_of_range("Insert: count negative\n");
    }

    bool string_no_static = false;

    if (_capacity < (_size + count + 1)) {
        char* new_ptr = new char[_size + 2 * count];
        if (_str != NULL) {
            strcpy(new_ptr, _str);

            if (&_str[0] == &string[0]) {

                char* save_ptr = new char[strlen(string) + 1] {};
                strcpy(save_ptr, string);
                string = save_ptr;
                save_ptr = NULL;
                string_no_static = true;
            }
            delete _str;
        }
        _str = new_ptr;
        new_ptr = NULL;
        _capacity = _size + 2 * count;
    }

    char* tail = new char[_size - index + 3 + count] {};
    strcpy(tail, &_str[index]);


    _str[index] = '\0';
    strncat(_str, string, count);
    _str[index + count] = '\0';


    if (tail != NULL) {
        strcat(_str, tail);
        delete[] tail;
        tail = NULL;
    }
    if (string_no_static) {
        delete[] string;
        string = NULL;
    }
    _size = static_cast<int>(strlen(_str));
}


void MyString::insert(int index, std::string _string) {
    insert(index, _string.c_str(), static_cast<int>(_string.length()));
}


void MyString::insert(int index, std::string _string, int count) {
    insert(index, _string.c_str(), count);
}


void MyString::replace(int index, int count, const char _string[]) {
    erase(index, count);
    insert(index, _string);
}


void MyString::replace(int index, int count, std::string _string) {
    erase(index, count);
    insert(index, _string);
}


void MyString::erase(int index, int count) {
    if (index >= _capacity) {
        throw std::out_of_range("Erase: index out of\n");
    }

    if (index + count > _capacity) {
        throw std::out_of_range("Erase: index + count out of\n");
    }
    if (index + count < _size)
        memcpy(&_str[index], &_str[index + count], _size - count);
    memset(&_str[_size - count], 0, count);
    _size -= count;
}


int MyString::find(const char _string[], int _index) {

    if (_index < 0 || _index > size()) {
        throw std::out_of_range("Error: find() index out of str");
    }

    /*AhoCorasick ac(_string);
    int index = ac.find(&_str[_index]);
    if (index == -1) {
        return index;
    }*/
    return strstr(&_str[_index], _string) - _str;
    //return index + _index;

}


int MyString::find(const char _string[]) {
    return find(_string, 0);
}


int MyString::find(const std::string _string) {
    return find(_string.c_str(), 0);
}


int MyString::find(const std::string _string, int _index) {
    return find(_string.c_str(), _index);
}


MyString MyString::substr(int _index, int count) {
    if (_index >= _size) {
        throw std::out_of_range("Substr: index out of\n");
    }
    if (count < 0) {
        throw std::out_of_range("Substr: count negative\n");
    }
    if (count + _index > _size) {
        throw std::out_of_range("Insert: (count + index) out of\n");
    }

    MyString new_string(&_str[_index], count);
    return new_string;
}


MyString MyString::substr(int _index) {
    return substr(_index, _size - _index);
}


void MyString::append(const char _string[], int _index, int count) {
    insert(this->size(), &_string[_index], count);
}


void MyString::append(const char _string[]) {
    insert(this->size(), _string);
}


void MyString::append(int count, char _char) {
    insert(this->size(), count, _char);
}


void MyString::append(const std::string _string) {
    insert(this->size(), _string);
}


void MyString::append(const std::string _string, int _index, int count) {
    insert(this->size(), &_string[_index], count);
}


//-----------------------------------------------------------------------

//----------------------------  OPERATORS  ------------------------------

//-----------------------------------------------------------------------


MyString MyString::operator+(MyString& _r_operand) {
    return operator+(_r_operand.c_str());
}


MyString MyString::operator+(const char _string[]) {
    MyString _resobj;
    _resobj.insert(0, this->c_str());
    _resobj.insert(this->size(), _string);
    return _resobj;
}


MyString MyString::operator+(std::string _string) {
    return operator+(_string.c_str());
}


MyString MyString::operator+=(const char _string[]) {
    this->insert(size(), _string);
    return *this;
}


MyString MyString::operator+=(std::string _string) {
    this->insert(size(), _string);
    return *this;
}



MyString MyString::operator+=(MyString& _obj) {
    this->insert(size(), _obj.c_str());
    return *this;
}


MyString MyString::operator=(const char _string[]) {
    this->clear();
    this->insert(0, _string);
    return *this;
}


MyString MyString::operator=(const MyString& _r_operand) {
    return operator=(_r_operand.c_str());
}


MyString MyString::operator=(std::string _string) {
    this->clear();
    this->insert(0, _string);
    return *this;
}


MyString MyString::operator=(char _char) {
    this->clear();
    this->insert(0, 1, _char);
    return *this;
}


char& MyString::operator[](int _index) {
    if (_index < 0 || _index >= size()) {
        throw std::out_of_range("Insert: index out\n");
    }
    return _str[_index];
}


std::ostream& operator << (std::ostream& os, const MyString& _obj) {
    return os << _obj.c_str();
}


std::istream& operator >> (std::istream& in, MyString& _obj) {
    std::string _string;
    in >> _string;
    _obj.clear();
    _obj.insert(0, _string);
    return in;
}


bool MyString::operator == (const MyString& _obj) {
    return (size() == _obj.size()) && (this->find(_obj.c_str(), 0) == 0);

}


bool MyString::operator != (const MyString& _obj) {
    return (size() != _obj.size()) || (find(_obj.c_str(), 0) != 0);
}


bool MyString::operator > (const MyString& _obj) const {
    if (this->size() != _obj.size()) {
        if (this->size() > _obj.size()) return true;
        return false;
    }
    for (int ind = 0; ind < size(); ind++) {
        if (this->c_str()[ind] != _obj.c_str()[ind]) {
            return this->c_str()[ind] > _obj.c_str()[ind];
        }
    }
    return false;
}


bool MyString::operator < (const MyString& _obj) const {
    bool inv_res = operator >(_obj);
    if (this->size() != _obj.size()) {
        return !inv_res;
    }
    return false;
}


bool MyString::operator >= (const MyString& _obj) const {
    return !operator <(_obj);
}


bool MyString::operator <= (const MyString& _obj) const {
    return !operator>(_obj);
}


MyString& MyString::operator=(MyString&& moved) noexcept {
    if (&moved != this) {
        if (_str != nullptr) delete _str;
        _str = moved._str;
        _size = moved._size;
        _capacity = moved._capacity;
        moved._str = nullptr;
        moved._size = 0;
        moved._capacity = 0;
    }
    return *this;
}


//
// -- -- -- THROUGHT ITERATORS -- -- --
//


void MyString::erase(const_iterator _iter, int count) {
    erase(_size - (this->cend() - _iter), count);
}


void MyString::replace(const_iterator _iter, int count, const char _string[]) {
    replace(_size - (this->cend() - _iter), count, _string);
}


void MyString::replace(const_iterator _iter, int count, std::string _string) {
    replace(_size - (this->cend() - _iter), count, _string);
}


int MyString::find(const char _string[], const_iterator _iter) {
    return find(_string, _size - (this->cend() - _iter));
}


int MyString::find(const std::string _string, const_iterator _iter) {
    return find(_string, _size - (this->cend() - _iter));
}


MyString MyString::substr(const_iterator _iter, int count) {
    return substr(_size - (this->cend() - _iter), count);
}


MyString MyString::substr(const_iterator _iter) {
    return substr(_size - (this->cend() - _iter));
}


void MyString::insert(const_iterator _iter, int count, char _char) {
    insert(_size - (this->cend() - _iter), _char, count);
}


void MyString::insert(const_iterator _iter, const char string[]) {
    insert(_size - (this->cend() - _iter), string);
}


void MyString::insert(const_iterator _iter, const char string[], int count) {
    insert(_size - (this->cend() - _iter), string, count);
}


void MyString::insert(const_iterator _iter, std::string _string) {
    insert(_size - (this->cend() - _iter), _string);
}


void MyString::insert(const_iterator _iter, std::string _string, int count) {
    insert(_size - (this->cend() - _iter), _string, count);
}




