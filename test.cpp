//
// Created by sergey on 5/21/19.
//
#include <algorithm>
#include <iostream>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <regex>
#include <string>
#include "test.hpp"
using namespace std;


int main(){
    regex wordNumWord("^\\b[\\w']+ \\d+ [\\w']\\b$");
    regex notWord("^\\bNOT [\\w']+\\b$");
    regex oneWord("^\\b[\\w']+\\b$");
    smatch match;
    if(regex_match("xxx 5 xxxx",wordNumWord))
        cout<<"dddddddddddddd"<<endl;
    return 0;
}