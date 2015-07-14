#ifndef NODE_H
#define NODE_H
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

class Node
{
public:
    int to;
    char value;
    Node();

    Node(int a,char b);
};

#endif // NODE_H
