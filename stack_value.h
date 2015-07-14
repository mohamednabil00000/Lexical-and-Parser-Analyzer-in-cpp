#ifndef STACK_VALUE_H
#define STACK_VALUE_H
#include <iostream>
#include<vector>
#include<string>
#include<cstring>
#include<fstream>
#include<map>
#include <set>
#include <sstream>
#include<stack>
#include <queue>
#include<algorithm>
using namespace std;


class stack_value
{
public:
    int st;
    int ended;
    char value;

    stack_value();

    stack_value(int st1,int en,char val);

};

#endif // STACK_VALUE_H
