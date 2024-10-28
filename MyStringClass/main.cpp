#include <iostream>
#include "MyString.h"
#include <ctime>
#include <sstream>
#include <fstream>
#include <math.h>
#include <pybind11/pybind11.h>
using namespace std;

enum LogType
{
    info = 0,
    success,
    error
};

void logger(LogType type, string msg);


void logger(LogType type, string msg) {
    auto current_time = time(0);
    char current_time_str[size("yyyy-mm-ddThh:mm:ssZ")];
    tm calendar_datetime;
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
    gmtime_r(&current_time, &calendar_datetime);
#elif _WIN32
    gmtime_s(&calendar_datetime, &current_time);
#endif
    strftime(current_time_str, size(current_time_str), "%FT%TZ", &calendar_datetime);
    current_time_str[strlen(current_time_str) - 1] = '\0';

    switch (type)
    {
    case info:
        cout << "\033[34m" << "[" << current_time_str << "]" << " INFO: " << msg << "\033[0m\n";
        break;
    case success:
        cout << "\033[32m" << "[" << current_time_str << "]" << " Success: " << msg << "\033[0m\n";
        break;
    case error:
        cout << "\033[31m" << "[" << current_time_str << "]" << " Error: " << msg << "\033[0m\n";
        break;
    }
}


void log_assertion(bool condition, std::string test_name)
{
    condition
        ? logger(success, test_name)
        : logger(error, test_name);
}


void test_insert_methods() {
    logger(info, "INSERT METHODS TESTING");
    string test_name;
    //--------------------------------|
    // TEST 1: INSERT IN EMPTY STRING |
    //--------------------------------|
    test_name = "TEST 1 - INSERT IN EMPTY STRING";
    MyString str1("");
    str1.insert(0, "Hello", 5);
    !strcmp(str1.c_str(), "Hello") ? logger(success, test_name) : logger(error, test_name);
    //-----------------------------------|
    // TEST 2: INSERT IN BEGIN OF STRING |
    //-----------------------------------|
    test_name = "TEST 2 - INSERT IN BEGIN OF STRING";
    MyString str2(" World");
    str2.insert(0, "Hello", 5);
    !strcmp(str2.c_str(), "Hello World") ? logger(success, test_name) : logger(error, test_name);
    //------------------------------------|
    // TEST 3: INSERT IN MIDDLE OF STRING |
    //------------------------------------|
    test_name = "TEST 3 - INSERT IN MIDDLE OF STRING";
    MyString str3("Hello World");
    str3.insert(6, "Beautiful ", 10);
    !strcmp(str3.c_str(), "Hello Beautiful World") ? logger(success, test_name) : logger(error, test_name);
    //---------------------------------|
    // TEST 4  INSERT IN END OF STRING |
    //---------------------------------|
    test_name = "TEST 4 - INSERT IN END OF STRING";
    MyString str4("Hello");
    str4.insert(5, " World", 6);
    !strcmp(str4.c_str(), "Hello World") ? logger(success, test_name) : logger(error, test_name);
    //-----------------------------|
    // TEST 5: INSERT EMPTY STRING |
    //-----------------------------|
    test_name = "TEST 5 - INSERT EMPTY STRING";
    MyString str5("Hello World");
    str5.insert(6, "", 0);
    !strcmp(str5.c_str(), "Hello World") ? logger(success, test_name) : logger(error, test_name);
    //---------------------------------------------|
    // TEST 6: INSERT EMPTY STRING IN EMPTY STRING |
    //---------------------------------------------|
    test_name = "TEST 6 - INSERT EMPTY STRING IN EMPTY STRING";
    MyString str6;
    str6.insert(0, "", 0);
    !strcmp(str6.c_str(), "") && str6.length() == 0 && \
        str6.capacity() >= 1 ? logger(success, test_name) : logger(error, test_name);
    //----------------------------|
    // TEST 7: INSERT LONG STRING |
    //----------------------------|
    test_name = "TEST 7 - INSERT LONG TAIL REWRITE ON COPY";
    MyString str7("0123456789");
    str7.insert(3, "**");
    !strcmp(str7.c_str(), "012**3456789") ? logger(success, test_name) : logger(error, test_name);
    //---------------------------|
    // TEST 8: INSERT ONE SYMBOL |
    //---------------------------|
    test_name = "TEST 8 - INSERT ONE SYMBOL";
    MyString str8("Hello World");
    str8.insert(5, "X", 1);
    !strcmp(str8.c_str(), "HelloX World") ? logger(success, test_name) : logger(error, test_name);
    //-----------------------------|
    // TEST 9: INSERT A LOT OF 'A' |
    //-----------------------------|
    test_name = "TEST 9 - INSERT A LOT OF \'A\'";
    MyString str9;
    str9.insert(0, 5000, 'a');
    str9.length() == 5000 && str9.capacity() >= 5001 ? logger(success, test_name) : logger(error, test_name);
    //------------------------------------|
    // TEST 10: INSERT STRING INTO ITSELF |
    //------------------------------------|
    test_name = "TEST 10 - INSERT STRING INTO ITSELF";
    MyString str10("Hello");
    str10.insert(5, str10.c_str());
    !strcmp(str10.c_str(), "HelloHello") ? logger(success, test_name) : logger(error, test_name);
    //---------------------------------------|
    // TEST 10: INSERT BY OUT OF RANGE INDEX |
    //---------------------------------------|
    test_name = "TEST 11 - INSERT BY OUT OF RANGE INDEX";
    MyString str11("Hello");
    try {
        str11.insert(1, " World", 6);
    }
    catch (...) {}
    logger(success, test_name); // If this line is executed, test has been successfully passed. Otherwise, the program should crash
}


void test_replace()
{
    logger(info, "REPLACE TESTS");
    std::string test_name;
    // TEST 1
    test_name = "TEST 1 - REPLACE FROM START";
    MyString s1("LongString");
    s1.replace(0, 4, "Short");
    log_assertion(s1.length() == 11 && strcmp(s1.c_str(), "ShortString") == 0, test_name);
    // TEST 2
    test_name = "TEST 2 - REPLACE FROM END";
    MyString s2("LongString");
    s2.replace(4, 6, "Int");
    log_assertion(s2.length() == 7
        && s2.capacity() >= 11
        && strcmp(s2.c_str(), "LongInt") == 0, test_name);
}


void test_capacity()
{
    logger(info, "CHANGE CAPACITY TESTS");
    std::string test_name;

    // TEST 1
    test_name = "TEST 1 - 1001 CHAR";
    MyString str1(1000, 'a');
    MyString str2("1234567890");
    str1 = str2;
    str1.insert(3, "****", 3);
    log_assertion(strcmp(str1.c_str(), "123***4567890") == 0 && str1.length() == 13 && str1.capacity() == 1001, test_name);
    // TEST 2
    test_name = "TEST 2 - SHRINK TO FIT";
    str1.shrink_to_fit();
    log_assertion(strcmp(str1.c_str(), "123***4567890") == 0 && str1.length() == 13 && str1.capacity() == 14, test_name);
}


void test_op_plus()
{
    logger(info, "OPERATOR+ TESTS");
    std::string test_name;

    MyString str1("Hello");
    MyString str2("World");
    std::string std_str1 = "!!!";
    const char* cstr = "!!!";
    char exclamation = '!';

    // TEST 1
    test_name = "TEST 1 - MYSTRING + MYSTRING";
    MyString result1 = str1 + " " + str2;
    log_assertion(strcmp(str1.c_str(), "Hello World") != 0 && strcmp(result1.c_str(), "Hello World") == 0, test_name);
    str1 = "Hello";
    // TEST 2
    test_name = "TEST 2 - MYSTRING + CONST CHAR*";
    MyString result2 = str1 + cstr;
    log_assertion(strcmp(str1.c_str(), "Hello!!!") != 0 && strcmp(result2.c_str(), "Hello!!!") == 0, test_name);
    str1 = "Hello";
    // TEST 3
    test_name = "TEST 3 - MYSTRING + STD::STRING";
    MyString result3 = str1 + " " + std_str1;
    log_assertion(strcmp(str1.c_str(), "Hello !!!") != 0 && strcmp(result3.c_str(), "Hello !!!") == 0, test_name);
}


void test_op_plusassign()
{
    logger(info, "OPERATOR+= TESTS");
    std::string test_name;

    MyString str1("Hello");
    std::string std_str1 = "World";
    const char* cstr = "!!!";
    char exclamation = '!';

    // TEST 1
    test_name = "TEST 1 - MYSTRING+=STD::STRING";
    str1 += " ";
    str1 += std_str1;
    log_assertion(strcmp(str1.c_str(), "Hello World") == 0, test_name);
    // TEST 2
    test_name = "TEST 2 - MYSTRING+=CONST CHAR*";
    str1 += cstr;
    log_assertion(strcmp(str1.c_str(), "Hello World!!!") == 0, test_name);
}


void test_getters()
{
    logger(info, "GETTERS TESTS");
    std::string test_name;
    MyString str("1234567890");
    // TEST 1
    test_name = "TEST 1 - C_STR";
    log_assertion(strcmp(str.c_str(), "1234567890") == 0, test_name);
    // TEST 2
    test_name = "TEST 2 - DATA";
    log_assertion(strcmp(str.data(), "1234567890") == 0, test_name);
    // TEST 3
    test_name = "TEST 3 - LENGTH";
    log_assertion(str.length() == 10, test_name);
    // TEST 4
    test_name = "TEST 4 - SIZE";
    log_assertion(str.size() == 10, test_name);
    // TEST 5
    test_name = "TEST 5 - CAPACITY";
    log_assertion(str.capacity() == 11, test_name);
    // TEST 6
    test_name = "TEST 6 - EMPTY (REAL NOT EMPTY)";
    log_assertion(str.empty() == false, test_name);
    // TEST 6
    test_name = "TEST 7 - EMPTY";
    MyString empty_str;
    log_assertion(empty_str.empty() == true, test_name);
}

void test_erase()
{
    logger(info, "ERASE TESTS");
    std::string test_name;
    // TEST 1
    test_name = "TEST 1 - ERASE FROM START";
    MyString s1("LongString");
    s1.erase(0, 4);
    log_assertion(s1.length() == 6 && strcmp(s1.c_str(), "String") == 0, test_name);
    // TEST 2
    test_name = "TEST 2 - ERASE FROM END";
    MyString s2("LongString");
    s2.erase(4, 6);
    log_assertion(s2.length() == 4 && strcmp(s2.c_str(), "Long") == 0, test_name);
    // TEST 3
    test_name = "TEST 3 - ERASE BY OUT OF BOUND INDEX";
    MyString s3("LongString");
    try {
        s2.erase(12, 6);
    }
    catch (...) {}
    logger(success, test_name);
}

void test_constructors() {
    logger(info, "CONSTRUCTORS TESTING");
    string test_name;
    //-----------------------------|
    // TEST 1: DEFAULT CONSTRUCTOR |
    //-----------------------------|
    MyString str1;
    test_name = "TEST 1 - DEFAULT";
    str1.length() == 0 && str1.capacity() >= 1 ? logger(success, test_name) : logger(error, test_name);
    //--------------------------|
    // TEST 2: CHAR CONSTRUCTOR |
    //--------------------------|
    test_name = "TEST 2 - CHAR";
    MyString str2(10, 'a');
    str2.length() == 10 && str2.capacity() >= 11 && \
        !strcmp(str2.c_str(), "aaaaaaaaaa") ? logger(success, test_name) : logger(error, test_name);
    //--------------------------|
    // TEST 3: COPY CONSTRUCTOR |
    //--------------------------|
    test_name = "TEST 3 - COPY";
    MyString str3(str2);
    str2.length() == 10 && str2.capacity() >= 11 && !strcmp(str2.c_str(), "aaaaaaaaaa") && \
        str3.length() == 10 && str3.capacity() >= 11 && !strcmp(str3.c_str(), "aaaaaaaaaa") \
        ? logger(success, test_name) : logger(error, test_name);
    //-------------------------------------------|
    // TEST 4: CHAR ARRAY WITH COUNT CONSTRUCTOR |
    //-------------------------------------------|
    test_name = "TEST 4 - CHAR ARRAY WITH COUNT";
    MyString str4("0123456789", 5);
    str4.length() == 5 && str4.capacity() >= 6 && !strcmp(str4.c_str(), "01234") \
        ? logger(success, test_name) : logger(error, test_name);
    //--------------------------------|
    // TEST 5: CHAR ARRAY CONSTRUCTOR |
    //--------------------------------|
    test_name = "TEST 5 - CHAR ARRAY";
    MyString str5("0123456789");
    str5.length() == 10 && str5.capacity() >= 11 && !strcmp(str5.c_str(), "0123456789") \
        ? logger(success, test_name) : logger(error, test_name);
    //---------------------------------|
    // TEST 6: STD::STRING CONSTRUCTOR |
    //---------------------------------|
    test_name = "TEST 5 - STD::STRING";
    std::string std_str1("0123456789");
    MyString str6(std_str1);
    str6.length() == 10 && str6.capacity() >= 11 && !strcmp(str6.c_str(), "0123456789") \
        ? logger(success, test_name) : logger(error, test_name);
}


void test_op_assign()
{
    logger(info, "OPERATOR= TESTS");
    std::string test_name;
    // TEST 1
    test_name = "TEST 1 - ASSIGN CONST CHAR*";
    MyString s1("Hello");
    s1 = "1234567890";
    log_assertion(s1.length() == 10 && strcmp(s1.c_str(), "1234567890") == 0, test_name);
    // TEST 2
    test_name = "TEST 2 - ASSIGN CHAR";
    MyString s2("Hello");
    s2 = 'a';
    log_assertion(s2.length() == 1 && strcmp(s2.c_str(), "a") == 0, test_name);
    // TEST 3
    test_name = "TEST 2 - ASSIGN STD::STRING";
    MyString s3("Hello");
    std::string std_s3("1234567890");
    s3 = std_s3;
    log_assertion(s3.length() == 10 && strcmp(s3.c_str(), std_s3.c_str()) == 0, test_name);
}


void test_op_index()
{
    logger(info, "OPERATOR[] TESTS");
    std::string test_name;
    // TEST 1
    test_name = "TEST 1 - GET FIRST ELEMENT BY INDEX";
    MyString str("Hello, world!");
    log_assertion(str[0] == 'H', test_name);
    // TEST 2
    test_name = "TEST 2 - GET MIDDLE ELEMENT BY INDEX";
    log_assertion(str[7] == 'w', test_name);
    // TEST 3
    test_name = "TEST 3 - GET LAST ELEMENT BY INDEX";
    log_assertion(str[str.size() - 1] == '!', test_name);
    // TEST 4
    test_name = "TEST 4 - GET ELEMENT BY OUT BOUND INDEX";
    try {
        char ch = str[str.size()];
    }
    catch (...) {}
    logger(success, test_name);
    // TEST 5
    test_name = "TEST 5 - WRITE ELEMENT BY INDEX";
    str[0] = 'h';
    log_assertion(str[0] == 'h', test_name);
    // TEST 6
    test_name = "TEST 6 - WRITE ELEMENT BY OUT BOUND INDEX";
    try {
        str[str.size()] = 'X';
    }
    catch (...) {}
    logger(success, test_name);
}


void test_op_io()
{
    logger(info, "IO TESTS");
    std::string test_name;
    // TEST 1
    test_name = "TEST 1 - INPUT LONG STRING";
    MyString s1;
    std::string input_str(4000, 'a');
    std::stringstream stream1(input_str.c_str());
    stream1 >> s1;
    log_assertion(s1.length() == 4000 && strcmp(s1.c_str(), input_str.c_str()) == 0, test_name);
    // TEST 2
    test_name = "TEST 2 - OUTPUT STRING";
    MyString s2("Hello");
    std::stringstream stream2;
    stream2 << s2;
    log_assertion(strcmp(stream2.str().c_str(), "Hello") == 0, test_name);
}



void test_op_equality()
{
    logger(info, "OPERATOR= TESTS");
    std::string test_name;

    MyString str1("apple");
    MyString str2("banana");
    MyString str3("apple");

    // TEST 1
    test_name = "TEST 1 - OPERATOR== EQUAL";
    log_assertion(str1 == str3, test_name);
    // TEST 2
    test_name = "TEST 2 - OPERATOR== NOT EQUAL";
    log_assertion(!(str1 == str2), test_name);
    // TEST 3
    test_name = "TEST 3 - OPERATOR!= NOT EQUAL";
    log_assertion(str1 != str2, test_name);
//    // TEST 4
    test_name = "TEST 4 - OPERATOR!= EQUAL";
    log_assertion(!(str1 != str3), test_name);
//    // TEST 5
    test_name = "TEST 5 - OPERATOR<";
    log_assertion(str1 < str2, test_name);
//    // TEST 6
    test_name = "TEST 6 - OPERATOR<=";
    log_assertion(str1 <= str3, test_name);
//    // TEST 7
    test_name = "TEST 7 - OPERATOR>";
    log_assertion(str2 > str1, test_name);
//    // TEST 8
    test_name = "TEST 8 - OPERATOR>=";
    log_assertion(str2 >= str1, test_name);
//
}


void test_find()
{
    logger(info, "FIND TESTS");
    std::string test_name;

    std::string str = "Hello, world! Hello, world!";

    // TEST 1
    test_name = "TEST 1 - FIND CONST CHAR* SUBSTR";
    std::size_t pos1 = str.find("Hello");
    log_assertion(pos1 == 0, test_name);
    // TEST 2
    test_name = "TEST 2 - FIND CONST CHAR* SUBSTR BY INDEX";
    std::size_t pos2 = str.find("world", 9);
    log_assertion(pos2 == 21, test_name);
    // TEST 3
    test_name = "TEST 3 - FIND STD::STRING SUBSTR";
    std::size_t pos3 = str.find(std::string("Hello"));
    log_assertion(pos3 == 0, test_name);
    // TEST 4
    test_name = "TEST 4 - FIND STD::STRING SUBSTR BY INDEX";
    std::size_t pos4 = str.find(std::string("world"), 9);
    log_assertion(pos4 == 21, test_name);
    // TEST 5
    test_name = "TEST 5 - FIND STD::STRING SUBSTR BY OUT OF BOUND INDEX";
    try {
        std::size_t pos5 = str.find(std::string("world"), 35);
    }
    catch (...) {};
    logger(success, test_name);
}


void test_substr()
{
    logger(info, "SUBSTRING TESTS");
    std::string test_name;
    // TEST 1
    test_name = "TEST 1 - SUBSTRING FULL SIZE";
    MyString s1("Str1 Str2 Str3");
    auto s1sub = s1.substr(5);
    log_assertion(s1sub.length() == 9 && strcmp(s1sub.c_str(), "Str2 Str3") == 0, test_name);
    // TEST 2
    test_name = "TEST 2 - SUBSTRING OF LENGTH";
    MyString s2("Str1 Str2 Str3");
    auto s2sub = s2.substr(5, 4);
    log_assertion(s2sub.length() == 4 && strcmp(s2sub.c_str(), "Str2") == 0, test_name);
}



void test_append()
{
    logger(info, "APPEND TESTS");
    std::string test_name;
    // TEST 1
    test_name = "TEST 1 - APPEND COUNT CHARS FROM C STR";
    MyString s1("str1");
    s1.append(3, 'a');
    log_assertion(s1.length() == 7 && strcmp(s1.c_str(), "str1aaa") == 0, test_name);
    // TEST 2
    test_name = "TEST 2 - APPEND FROM C STR";
    MyString s2("str2");
    s2.append("str1");
    log_assertion(s2.length() == 8 && strcmp(s2.c_str(), "str2str1") == 0, test_name);
    // TEST 3
    test_name = "TEST 3 - APPEND COUNT OF CHAR";
    MyString s3("str3");
    s3.append(5, 'a');
    log_assertion(s3.length() == 4 + 5 && strcmp(s3.c_str(), "str3aaaaa") == 0, test_name);
    // TEST 4
    test_name = "TEST 4 - APPEND STD STRING";
    std::string stdstr = "stdstr";
    MyString s4("str4");
    s4.append(stdstr);
    log_assertion(s4.length() == 10 && strcmp(s4.c_str(), "str4stdstr") == 0, test_name);
}

void test_aho_corasick() {

    std::vector<std::string> patterns = { "he", "she", "his", "hers" };

    MyString s1 = "ahishershe";
    cout << "TEST 1 --- " << s1 << endl;


    std::vector<std::pair<int, std::string>> results = s1.AHO_FIND(patterns);

    for (const auto& result : results) {
        std::cout << result.second << ":::" << result.first << "\n";
    }


    MyString s2 = "TTaaaaa";
    cout << "TEST 2 --- " << s2 << endl;

    std::vector<std::string> patterns2 = { "a", "aa", "aaa", "aaaaa" };

    std::vector<std::pair<int, std::string>> results2 = s2.AHO_FIND(patterns2);

    for (const auto& result : results2) {
        std::cout << result.second << ":::" << result.first << "\n";
    }

}



void test_add_task1() {
    logger(info, "\n- - - - - ADD_TASK1 TESTS - - - - - \n");

    string test_name = "TEST 1 - MOVE CONSTR DEFAULT";
    MyString original("Hello");
    MyString moved = std::move(original);
    log_assertion(strcmp(moved.c_str(), "Hello") == 0 && original.c_str() == nullptr, test_name);

    test_name = "TEST 2 - INT CONSTR DEFAULT";
    int num = 305419896;
    MyString str(num);
    log_assertion(strcmp(str.c_str(), "305419896") == 0, test_name);


    test_name = "TEST 3 - FLOAT CONSTR DEFAULT";
    float num2 = 0.05;
    MyString str2(num2);
    log_assertion(fabs((atof(str2.c_str()) - num2)) < 0.000000001, test_name);

    test_name = "TEST 4 - MOVE-OP DEFAULT";
    MyString original2("Test");
    MyString moved2;

    moved2 = std::move(original2);

    log_assertion(strcmp(moved2.c_str(), "Test") == 0 && original2.c_str() == nullptr, test_name);

    test_name = "TEST 5 - FILE-OUT-OP DEFAULT";
    MyString str3("TestOutput");
    std::ofstream outfile("testfile.txt");
    outfile << str3;
    outfile.close();

    std::ifstream infile("testfile.txt");
    std::string content;
    std::getline(infile, content);
    log_assertion(strcmp(content.c_str(), "TestOutput") == 0, test_name);

    test_name = "TEST 6 - FILE-IN-OP DEFAULT";
    std::ofstream outfile2("testfile2.txt");
    outfile2 << "TestInput";
    outfile2.close();

    MyString str4;
    std::ifstream infile2("testfile2.txt");
    infile2 >> str4;

    log_assertion(strcmp(str4.c_str(), "TestInput") == 0, test_name);


    logger(info, "\n- - - - - ADD_TASK1 AHOCORASICK TESTS - - - - - \n");
    test_aho_corasick();
}


void test_iterators() {

    cout << "\n\n";

    MyString myStr;
    const char* testStr = "Hello, World!";
    myStr = MyString(testStr);

    auto it = myStr.begin();
    auto end = myStr.end();

    assert(*it == 'H');
    it++;
    assert(*it == 'e');
    it++;
    assert(*(end - 1) == '!');

    assert(end - it == 11);

    it++;
    assert(*it == 'l');

    it--;
    assert(*it == 'l');

    assert(it != end);
    assert(it == myStr.begin() + 2);

    cout << "TESTS ITERATORS SUCCESS\n";
}


void test_reverse_iterators() {
    MyString myStr;
    const char* testStr = "Hello, World!";
    myStr = MyString(testStr);

    auto rit = myStr.rbegin();
    auto rend = myStr.rend();

    assert(*rit == '\0');
    rit++;
    assert(*rit == '!');
    rit++;
    assert(*(rend) == 'H');

    assert(rit - rend == 11);

    rit++;
    assert(*rit == 'l');

    rit--;
    assert(*rit == 'd');

    assert(rit != rend);
    assert(rit == myStr.rbegin() + 2);

    cout << "TESTS REVERSE_ITERATORS SUCCESS\n";
}


void test_iterator_funcs() {

    MyString s = "qwerty123";
    auto it1 = s.cbegin();
    it1 += 2;
    s.insert(it1, "!!!");

    assert(s == "qw!!!erty123");

    MyString s2 = "qwerty123";

    auto it2 = s2.cbegin();
    it2 += 6;
    s2.replace(it2, 3, "098");

    assert(s2 == "qwerty098");

    MyString s3 = "qwerty123";
    auto it3 = s3.cbegin();
    it3 += 2;

    assert(4 == s3.find("ty", it3));

    MyString s4 = "qwerty123";
    auto it4 = s4.cbegin();
    it4 += 6;
    MyString sub_s4 = s4.substr(it4, 2);

    assert(strcmp(sub_s4.c_str(), "12") == 0);


    MyString s5 = "qwerty123";
    auto it5 = s5.cbegin();
    it5 += 3;
    s5.erase(it5, 6);

    assert(strcmp(s5.c_str(), "qwe") == 0);

    cout << "TESTS ITERATORS FUNCS SUCCESS\n";
}


void test_add_task_2() {
    test_iterators();
    test_reverse_iterators();
    test_iterator_funcs();
}


void test() {
    test_constructors();
    test_capacity();
    test_getters();
    test_op_assign();
    test_op_plus();
    test_op_plusassign();
    test_op_equality();
    test_op_index();
    test_op_io();
    test_insert_methods();
    test_replace();
    test_find();
    test_substr();
    test_append();
    test_erase();

    test_add_task1();
    test_add_task_2();
}


int main() {


    test();
    
    return 0;

}
