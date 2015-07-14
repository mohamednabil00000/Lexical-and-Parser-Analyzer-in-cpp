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
#include "parser.h"




void readLexicalRule(){
    string line;
  ifstream myfile ;
  myfile.open("lexicalrule.txt");

    while ( getline (myfile,line) )
    {
        lexical :: scan(line);
    }
    myfile.close();


}
void read_code_and_generate_lexical_analyzer(){


  ifstream code ;
  code.open("code2.txt");
    string lin;
    ofstream file;
    file.open ("token.txt");
    while (getline(code,lin))
    {
    //cout<<lin<<endl;
   lexical :: generateLexicalAnalizer(lin,file);

    }
    code.close();
    file<<"$"<<endl;
    file.close();
}
/*void read_lexical_analyser(){

  ifstream token ;
  token.open("token.txt");
  string lin;
    while (getline(token,lin))
    {
        //tokens.push(lin);
      //  tokens2.push_back(lin);
      lexical :: set_tokens(lin);
      lexical :: set_tokens2(lin);
    //cout<<lin<<endl;

    }

}*/

void parser::read_CFG_Rules(){

    int tC =parser :: get_terminalCounter();
    parser :: set_terminalNum(parser ::get_epson(),tC);
    tC++;
    parser :: set_terminalCounter(tC);
    parser :: set_terminal(parser ::get_epson());
  ifstream cfgRule ;

  cfgRule.open("cfgrule1.txt");
  string lin;
    while (getline(cfgRule,lin))
    {
    //cout<<lin<<endl;
    parser ::parseCFGRule(lin);

    }
        if(parser::get_temp().size()!=0){
        parser :: set_CFG(parser :: get_num_of_expr(),parser ::get_temp());

        parser ::set_temp_clear();
    }
    int nE = parser :: get_num_of_expr();
    parser::set_num_of_expr(nE);
    nE++;
    parser :: set_num_of_expr(nE);
    cfgRule.close();
    }

    void writeStatment(string a){
       cout<<endl;
    cout<<"        /";
    for(int i=0;i<a.size()+2;i++)
        cout<<"-";
        cout<<"\\"<<endl;
    cout<<"        | "<<a<<" |"<<endl;
  cout<<"        \\";
    for(int i=0;i<a.size()+2;i++)
        cout<<"-";
        cout<<"/"<<endl;
        cout<<endl;
    }
//.................................................................................
int main()
{

    readLexicalRule();
   lexical :: buildNFA();
    lexical ::clausre();
    lexical ::buildTransition();
    lexical ::minimize();

//  printTransitionTable();
    parser ::read_CFG_Rules();
    parser ::buildLeftFactoring();
    parser :: buildLeftRecursion();
    parser :: init_parse_table();
    parser ::  print_cfgRules();
    parser ::  print_cfgRules_file();
    parser ::buildFirst();
    parser ::buildFollow();
    parser :: print_set(parser ::get_first(),parser::get_num_of_expr(),"First");
    parser :: print_set(parser ::get_follow(),parser ::get_num_of_expr(),"Follow");
    parser :: print_set_file(parser ::get_first(),parser::get_num_of_expr(),"First");
    parser :: print_set_file(parser ::get_follow(),parser ::get_num_of_expr(),"Follow");
   parser :: print_parse_table();
   parser :: print_parse_table_file();
   read_code_and_generate_lexical_analyzer();
   // read_lexical_analyser();
   lexical ::printLexicalAnalizer();
   lexical ::printLexicalAnalizer_file();
     if(lexical::get_ok()){
       parser ::parseToken(1);
        if(lexical::get_ok())
        writeStatment("Good parsing!");
    else
        writeStatment("Error found during parsing !");
    }
       else{
            writeStatment("Error found during lexical analyzer !");
       }




    //cout<<name_of_expr[13]<<endl;



   // cout<<start_state_after_minimization<<endl;



 /* cout<<"Definations \n.....................\n\n\n";
  for(int i=0;i<definations.size();i++){
         //   cout<<def_names["digit"]<<" = ";
       for(int j=0;j<definations[i].size();j++)
            cout<<definations[i][j]<<" ";
        cout<<endl;
 }
cout<<"...................................\n\n\nRegular Expression \n.....................\n\n\n";
   for(int i=0;i<regularExpression.size();i++){
            cout<<reg_names_by_int[i]<<" : ";
       for(int j=0;j<regularExpression[i].size();j++)
            cout<<regularExpression[i][j]<<" ";
        cout<<endl;
 }
cout<<"\n....................................\n";


 for(int i=0;i<50;i++){
                cout<<"Node: "<<i<<" "<<acceptancyState[i].endState<<" "<<acceptancyState[i].reg_of_end_state<<endl;
        for(int j=0;j<NFA[i].size();j++){
        cout<<NFA[i][j].to<<" "<<NFA[i][j].value<<endl;
        }
 }
 cout<<"state: "<<state<<endl;

 cout<<"input symbols: "<<endl;
 for (set<char>::iterator i = transitionSymbol.begin(); i != transitionSymbol.end(); i++) {
   char element = *i;
   cout<<element<<endl;
}


    for(int j=0;j<state;j++){
        cout<<j <<" : ";
        for (set<int>::iterator i = clas[j].begin(); i != clas[j].end(); i++) {
                int element = *i;
                cout<<element<<" ";
        }
        cout<<endl;
    }


    int c=0;
    for(int i=0;i<final_transition_table.size();i++){

             //   cout<<final_transition_table[i].endStateValue<<endl;

            cout<<"State : "<<final_transition_table[i].state_name<<" "<<final_transition_table[i].endState<<" "<<final_transition_table[i].endStateValue<<endl;
            c++;
           for(int j=0;j<128;j++){
                if(final_transition_table[i].final_transition_table[j]!=0){
                    cout<<(char)j<<" "<<final_transition_table[i].final_transition_table[j]<<endl;
                }
           }

    }

    cout<<"DFA state : "<<final_transition_table.size()<<endl;

*/


    return 0;
}
