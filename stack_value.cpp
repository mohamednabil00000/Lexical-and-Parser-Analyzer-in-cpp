#include "stack_value.h"
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

   stack_value::   stack_value(){
    }

    stack_value::  stack_value(int st1,int en,char val)
    {
        st=st1;
        ended=en;
        value=val;

    }
