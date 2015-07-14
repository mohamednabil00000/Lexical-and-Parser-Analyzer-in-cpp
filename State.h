#ifndef STATE_H
#define STATE_H

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

class State
{
public:
    bool endState;
    string reg_of_end_state;
    State();

    State(bool a,string b);
};

#endif // STATE_H
