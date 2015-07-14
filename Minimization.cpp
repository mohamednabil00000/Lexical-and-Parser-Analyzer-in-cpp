#include "Minimization.h"
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

 Minimization::   Minimization(){

    }
  Minimization::  Minimization(int s_n,int n_l,vector<int> cS){
       set_num=s_n;
       num_level=n_l;
       currentSet=cS;
    }

