#include "State.h"
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

  State::   State(){
    }

   State::  State(bool a,string b)
    {
        endState=a;
        reg_of_end_state=b;
    }
