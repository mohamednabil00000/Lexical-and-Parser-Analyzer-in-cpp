#ifndef LEXICAL_H
#define LEXICAL_H
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

class lexical
{
    public:
        lexical();
      /*  static queue<string> get_tokens();
         static vector<string> get_tokens2();*/
        static bool get_ok();
        static void set_ok(bool o);

        static void readLexicalRule();
        static void buildNFA();
        static void clausre();
        static void buildTransition();
        static void minimize();
        static void read_code();
        static void buildLexicalAnalizer();
        static void read_lexical_analyser();
        static void printLexicalAnalizer();
        static void printLexicalAnalizer_file();
        static void printTransitionTable();
        static void generateLexicalAnalizer(string line,ofstream &file);
        static void scan(string inp);
        static void splitSpaces(string line);
       /* static void set_tokens(string a);
        static void set_tokens2(string a);
        static void pop_tokens();*/
    protected:
    private:
};

#endif // LEXICAL_H
