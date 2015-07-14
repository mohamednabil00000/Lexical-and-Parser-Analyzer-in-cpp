#include "FinalTransition.h"
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

  FinalTransition::FinalTransition(){
     }

  FinalTransition:: FinalTransition(int s_n,vector<int>f_t_t,bool es,string esv){
        state_name=s_n;
        final_transition_table=f_t_t;
        endState=es;
        endStateValue=esv;
     }
