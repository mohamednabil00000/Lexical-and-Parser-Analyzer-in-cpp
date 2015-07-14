#ifndef MINIMIZATION_H
#define MINIMIZATION_H
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


class Minimization
{
  public:
    int set_num;
    int num_level;
    vector<int> currentSet;
    Minimization();
    Minimization(int s_n,int n_l,vector<int> cS);
};


#endif // MINIMIZATION_H
