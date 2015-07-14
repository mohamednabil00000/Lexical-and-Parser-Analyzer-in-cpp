#ifndef PARSER_H
#define PARSER_H

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



#include "Minimization.h"
#include "FinalTransition.h"
#include "State.h"
#include "Node.h"
#include "stack_value.h"
#include "sort_for_expr.h"
#include "lexical.h"
class parser
{
    public:
        parser();
        static void read_CFG_Rules();
        static void  buildLeftFactoring();
        static void  buildLeftRecursion();
        static void init_parse_table();
        static void print_cfgRules();
        static void print_cfgRules_file();
        static void buildFirst();
        static void buildFollow();
        static void print_set(set<string> a[],int num_of_expr,string b);
        static void print_set_file(set<string> a[],int num_of_expr,string b);
        static void print_parse_table();
        static void print_parse_table_file();
        static void parseToken(int takeShot);
        static void printFollowGraph();
        static void printLeftDerivation(stack<string>rule);
         static void printLeftDerivation_file(stack<string>rule,ofstream &f);
        static void read_lexical_analyser();
        static void parseCFGRule(string a);


        static int get_num_of_expr();
        static set<string> * get_first();
        static set<string> * get_follow();


        static void  set_num_of_expr(int n_o_e);
        static vector<string> get_temp();
        static void  set_terminalNum(string e , int c);
        static void set_terminal(string a);
        static void set_CFG(int n, vector<string> t);
        static string get_epson();
        static int  get_terminalCounter();
        static void  set_terminalCounter(int t);
        static void set_temp_clear();

    protected:
    private:
};

#endif // PARSER_H
