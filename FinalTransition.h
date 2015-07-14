#ifndef FINALTRANSITION_H
#define FINALTRANSITION_H
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


class FinalTransition
{
   public :
     int state_name;
     vector <int>final_transition_table;
     bool endState;
     string endStateValue;
     FinalTransition();
     FinalTransition(int s_n,vector<int>f_t_t,bool es,string esv);
};

#endif // FINALTRANSITION_H
