#include "parser.h"
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


class Compare
{
public:
    bool operator() (const sort_for_expr & a,const sort_for_expr & b)
    {
        if(a.numOfNonTerm<b.numOfNonTerm)
            return true;
        else
            return false;

    }
} myobject;



//parserAnalyzer............................................................................



const int maxN=50000;
int eq=0;
int num_of_expr=-1;
vector< vector <string> > CFG [maxN];
vector<string>name_of_expr;
vector<string> temp ;
map<string,int> terminalNum;
map<string ,int> exprNum;
string temp2="";
int terminalCounter=0;
set<string> first[maxN];
set<string> follow [maxN];
int parseTable[maxN][500];
set<int>vis;
bool vis2[maxN];
vector<string> terminal;
vector<sort_for_expr> sortExprForFirst;
int haveEpson[maxN];
string epson="'\\L'";
set<int>followGraph[maxN];
set<int>reversedFollowGraph[maxN];
string dollarSign="'$'";

parser::parser()
{


}

//***********getters and setters

set<string> * parser :: get_first(){
 return first;
}
set<string> * parser :: get_follow(){
 return follow;
}
int parser :: get_num_of_expr(){
 return num_of_expr;
}


void parser :: set_num_of_expr(int n_o_e){
 num_of_expr =n_o_e;
}

vector<string> parser :: get_temp(){
 return temp;
}
void parser :: set_terminalNum(string e, int t){
 terminalNum[e]=t;
}
void parser :: set_terminal(string a){
  return terminal.push_back(a);
}
void parser :: set_CFG(int n, vector<string> t){

 CFG[n].push_back(t);
}
string parser ::get_epson(){
 return epson;
}
int parser :: get_terminalCounter(){
 return terminalCounter;
}
void parser :: set_terminalCounter(int t){
 terminalCounter = t;
}

void parser :: set_temp_clear(){
 temp.clear();
}


//********************************************************************

void parser :: parseCFGRule(string a){
    int singlecout=0;

        for(int i=0;i<a.size();i++){
             if(a[i]=='\''&&(i==0||a[i-1]!='\\')){
                if(singlecout==0){
                if(temp2!="")
                        temp.push_back(temp2);
                    temp2="";
                temp2+=a[i];
                singlecout=1;

            }
            else{
                temp2+=a[i];
                if(terminalNum.find(temp2)==terminalNum.end()){
                terminalNum[temp2]=terminalCounter++;
                terminal.push_back(temp2);
                }
                temp.push_back(temp2);
                temp2="";
                singlecout=0;

            }
            continue;
        }

         if(singlecout==1){
            temp2+=a[i];
            continue;
        }

        if(a[i]==' '){
            if(eq==1){
                if(temp2!="")
            temp.push_back(temp2);
            temp2="";
             }
            continue;
        }

        if(a[i]=='#'&&(i==0||a[i-1]!='\\')&&singlecout==0){
             eq=0;
            if(temp2!="")
            temp.push_back(temp2);
            temp2="";
            if(temp.size()>0)
            CFG[num_of_expr].push_back(temp);
            temp.clear();
            num_of_expr++;
            continue;
        }
        if(a[i]=='='&&(i==0||a[i-1]!='\\')&&singlecout==0){
            exprNum[temp2]=name_of_expr.size();
            name_of_expr.push_back(temp2);
            temp2="";
            eq=1;
            continue;
        }
        if(a[i]==':'&&i+1<a.size()&&a[i+1]=='='&&eq==0&&singlecout==0)
            continue;

        if(a[i]=='|'&&(i==0||a[i-1]!='\\')&&singlecout==0){
                if(temp2!="")
                temp.push_back(temp2);
                CFG[num_of_expr].push_back(temp);
                temp2="";
                temp.clear();
                continue;
        }



        temp2+=a[i];
    }

    if(temp2!="")
    temp.push_back(temp2);
    temp2="";


}


/*void parser::read_CFG_Rules(){
    terminalNum[epson]=terminalCounter++;
    terminal.push_back(epson);
  ifstream cfgRule ;
  cfgRule.open("cfgrule5.txt");
  string lin;
    while (getline(cfgRule,lin))
    {
    //cout<<lin<<endl;
    parseCFGRule(lin);

    }
        if(temp.size()!=0){
        CFG[num_of_expr].push_back(temp);
        temp.clear();
    }
    num_of_expr++;
    cfgRule.close();
    }*/

   void parser::print_cfgRules(){
       cout<<"CFG RULES"<<endl;
       cout<<"***************"<<endl;
          for(int i=0;i<num_of_expr;i++){
            cout<<name_of_expr[i]<<"  ->  ";
        for(int j=0;j<CFG[i].size();j++){
            for(int k=0;k<CFG[i][j].size();k++){
                cout<<CFG[i][j][k]<<" ";
            }
            if(j<CFG[i].size()-1)
            cout<<" | ";
        }
        cout<<endl;
    }

    cout<<"*********************************************************************************************************"<<endl;
   }

    void parser::print_cfgRules_file(){
        ofstream myfile;
        myfile.open("CFG_Rules.txt");
       myfile<<"CFG RULES"<<endl;
       myfile<<"***************"<<endl;
          for(int i=0;i<num_of_expr;i++){
            myfile<<name_of_expr[i]<<"  ->  ";
        for(int j=0;j<CFG[i].size();j++){
            for(int k=0;k<CFG[i][j].size();k++){
                myfile<<CFG[i][j][k]<<" ";
            }
            if(j<CFG[i].size()-1)
            myfile<<" | ";
        }
        myfile<<endl;
    }

    myfile<<"*********************************************************************************************************"<<endl;
    myfile.close();
   }


  void solveFirst(int exprNumber){

                vis.insert(exprNumber);


        for(int i=0;i<CFG[exprNumber].size();i++){
                string s=CFG[exprNumber][i][0];
                if(s[0]=='\''){
                        if(s==epson)
                        haveEpson[exprNumber]=i+1;
                        first[exprNumber].insert(s);
                        int termNum=terminalNum[s];
                        parseTable[exprNumber][termNum]=i;

                }
                else{
                        int nextExprNumber=exprNum[s];
                    if(vis.find(nextExprNumber)!=vis.end()){
                            continue;
                    }
                     if(!vis2[nextExprNumber]){
                        solveFirst(nextExprNumber);
                    }

                    for (set<string>::iterator ii = first[nextExprNumber].begin(); ii != first[nextExprNumber].end(); ii++) {
                            string element = *ii;
                            first[exprNumber].insert(element);
                            int termNum=terminalNum[element];
                                    parseTable[exprNumber][termNum]=i;
                              if(element==epson)
                                        haveEpson[exprNumber]=i+1;

                        }
                }
        }
   }

void solveFollow(int exprNumber){

        vis.insert(exprNumber);
    for (set<int>::iterator ii = followGraph[exprNumber].begin(); ii != followGraph[exprNumber].end(); ii++) {
        int nextExprNumber=*ii;
        if(vis.find(nextExprNumber)!=vis.end())
                continue;


        if(!vis2[nextExprNumber]){
                solveFollow(nextExprNumber);
        }

       for (set<string>::iterator ii = follow[nextExprNumber].begin(); ii !=follow[nextExprNumber].end(); ii++) {
                string element = *ii;
                follow[exprNumber].insert(element);

        }

    }



}
void parser:: buildFirst(){


    for(int i=0;i<num_of_expr;i++){
            int nonTerm=0;
        for(int j=0;j<CFG[i].size();j++){
            string s=CFG[i][j][0];
            if(s[0]!='\'')
                nonTerm++;
        }
        sortExprForFirst.push_back(sort_for_expr(i,nonTerm));
    }
    sort(sortExprForFirst.begin(),sortExprForFirst.end(),myobject);

                for(int i=0;i<num_of_expr;i++){
            vis.clear();
                solveFirst(sortExprForFirst[i].exprNum);
                vis2[sortExprForFirst[i].exprNum]=1;
        }


   }

void parser::buildFollow(){
                follow[0].insert(dollarSign);
                for(int i=0;i<num_of_expr;i++){
                for(int j=0;j<CFG[i].size();j++){
                for(int k=0;k<CFG[i][j].size();k++){
                        string s=CFG[i][j][k];
                    if(s[0]!='\''){
                        int exprNumber=exprNum[s];
                        if(k==CFG[i][j].size()-1){
                            followGraph[exprNumber].insert(i);
                            reversedFollowGraph[i].insert(exprNumber);
                        }
                        else{
                                string s2=CFG[i][j][k+1];
                            if(s2[0]!='\''){
                                int exprNumber2=exprNum[s2];
                             for (set<string>::iterator ii = first[exprNumber2].begin(); ii != first[exprNumber2].end(); ii++) {
                                        string element = *ii;

                                        if(element!=epson){

                                            follow[exprNumber].insert(element);
                                        }
                                    else{

                                        followGraph[exprNumber].insert(exprNumber2);
                                        reversedFollowGraph[exprNumber2].insert(exprNumber);
                                    }


                                                }

                            }else{
                                follow[exprNumber].insert(s2);
                            }
                        }
                    }

                }
            }
        }


        vector<sort_for_expr>sortForFollow;

        for(int i=0;i<num_of_expr;i++){
                vis2[i]=0;
                sortForFollow.push_back(sort_for_expr(i,followGraph[i].size()));
        }

        sort(sortForFollow.begin(),sortForFollow.end(),myobject);

        queue<int>q;
        for(int i=0;i<num_of_expr;i++){
            int elem=sortForFollow[i].exprNum;
                if(vis2[elem])
                    continue;
                q.push(elem);

                while(!q.empty()){
                        int node =q.front();
                    q.pop();
                    vis.clear();
                    solveFollow(node);
                    vis2[node]=1;

                    for (set<int>::iterator ii = reversedFollowGraph[node].begin(); ii != reversedFollowGraph[node].end(); ii++) {
                            int element = *ii;
                                if(vis2[element])
                                        continue;
                                q.push(element);

                                        }

                }


        }

                for(int i=0;i<num_of_expr;i++){
                if(haveEpson[i]!=0){
                 for (set<string>::iterator ii = follow[i].begin(); ii != follow[i].end(); ii++) {
                                          string element = *ii;

                          parseTable[i][terminalNum[element]]=haveEpson[i]-1;

                           }

            }
            else{
                for (set<string>::iterator ii = follow[i].begin(); ii != follow[i].end(); ii++) {
                                          string element = *ii;
                          int numOfTerm=terminalNum[element];
                          if(parseTable[i][numOfTerm]==-1)
                            parseTable[i][numOfTerm]=-2;

                }

            }
       }

}

void parser::printFollowGraph(){
 for(int i=0;i<num_of_expr;i++){
            cout<<i<<"  :   ";
           for (set<int>::iterator ii = followGraph[i].begin(); ii != followGraph[i].end(); ii++) {
            int element = *ii;
            cout<<element<<"  ,  ";
        }
        cout<<endl;
    }
}
void parser :: print_set(set <string> a[],int size,string b){

            cout<<b<<endl;
            cout<<"*******"<<endl;
        for(int i=0;i<size;i++){
            cout<<name_of_expr[i]<<"  :{ ";
            int p=0;
            for (set<string> ::iterator ii = a[i].begin(); ii != a[i].end(); ii++) {
                            string element = *ii;
                            if(p!=0)
                                cout<<" , ";
                            cout<<element;
                            p++;

                    }
                    cout<<" }"<<endl;
              }

              cout<<"***********************************************************************************"<<endl;
}
void parser :: print_set_file(set <string> a[],int size,string b){
            ofstream myfile;
            string f=b;
            f+=".txt";

            myfile.open(f.c_str());
            myfile<<b<<endl;
            myfile<<"*******"<<endl;
        for(int i=0;i<size;i++){
            myfile<<name_of_expr[i]<<"  :{ ";
            int p=0;
            for (set<string> ::iterator ii = a[i].begin(); ii != a[i].end(); ii++) {
                            string element = *ii;
                            if(p!=0)
                                myfile<<" , ";
                            myfile<<element;
                            p++;

                    }
                    myfile<<" }"<<endl;
              }

              myfile<<"***********************************************************************************"<<endl;
              myfile.close();
}
void parser::print_parse_table(){
    for(int i=1;i<terminalCounter;i++){
       cout<<i<<" -> "<<terminal[i]<<" "<<endl;
            }
    cout<<endl;

   for(int i=0;i<num_of_expr;i++){
            cout<<name_of_expr[i]<<"  :";
        for(int j=1;j<terminalCounter ;j++){
            cout<<"  "<<parseTable[i][j]<<"  ";
                }
    cout<<endl;
    cout<<"*************************************************"<<endl;
        }


}
void parser::print_parse_table_file(){
    ofstream myfile;
    myfile.open("parse_table.txt");
    for(int i=1;i<terminalCounter;i++){
        myfile<<i<<" -> "<<terminal[i]<<" "<<endl;
            }
   myfile<<endl;

   for(int i=0;i<num_of_expr;i++){
             myfile<<name_of_expr[i]<<"  :";
        for(int j=1;j<terminalCounter ;j++){
             myfile<<"  "<<parseTable[i][j]<<"  ";
                }
     myfile<<endl;
     myfile<<"*************************************************"<<endl;
        }
 myfile.close();

}
void parser::printLeftDerivation(stack<string>rule){

    stack<string>rule2;


    while(!rule.empty()){
        rule2.push(rule.top());
        rule.pop();
    }

    while(!rule2.empty()){
        cout<<rule2.top()<<" ";
                rule2.pop();
    }

    cout<<endl;

}

void parser::printLeftDerivation_file(stack<string>rule,ofstream &myfile){


    stack<string>rule2;


    while(!rule.empty()){
        rule2.push(rule.top());
        rule.pop();
    }

    while(!rule2.empty()){
       myfile<<rule2.top()<<" ";
                rule2.pop();
    }

    myfile<<endl;


}
void Error(string a,string b){
    cout<<endl;
    cout<<"                 /--------------------------";
    for(int i=0;i<a.size()+1;i++)
        cout<<"-";
        for(int i=0;i<b.size();i++)
        cout<<"-";
        cout<<"\\"<<endl;
    cout<<"                 |# Error Here ! Skip "<<a<<" From "<<b<<" |"<<endl;
  cout<<"                 \\--------------------------";
    for(int i=0;i<a.size()+1;i++)
        cout<<"-";
        for(int i=0;i<b.size();i++)
        cout<<"-";
        cout<<"/"<<endl;
        cout<<endl;
}

void Error_file(string a,string b,ofstream &myfile){
    myfile<<endl;
    myfile<<"                 /--------------------------";
    for(int i=0;i<a.size()+1;i++)
        myfile<<"-";
        for(int i=0;i<b.size();i++)
        myfile<<"-";
        myfile<<"\\"<<endl;
    myfile<<"                 |# Error Here ! Skip "<<a<<" From "<<b<<" |"<<endl;
  myfile<<"                 \\--------------------------";
    for(int i=0;i<a.size()+1;i++)
        myfile<<"-";
        for(int i=0;i<b.size();i++)
        myfile<<"-";
        myfile<<"/"<<endl;
        myfile<<endl;
}

void parser:: parseToken(int takeShot){

         stack < string > expRules;
         expRules.push(dollarSign);
         expRules.push(name_of_expr[0]);
        ofstream myfile;
        if(takeShot)
        myfile.open("left_derivation.txt");

        ifstream tokens;
        tokens.open("token.txt");
        string tok="";
        int take=0;
        string token="";
        bool pop=1;
        while(!expRules.empty()){
            if(takeShot){
            printLeftDerivation(expRules);
            printLeftDerivation_file(expRules,myfile);
            }
            if(pop==1){
                if(getline(tokens,tok)){
                    token="'";
                    token+=tok;
                    token+="'";
                    pop=0;
                }
                else{
                   // ok=false;
                   lexical::set_ok(false);
                    break;
                }
            }


        string rule=expRules.top();
        if(rule[0]=='\''){//rule terminal;
        //      string ruleSymbol=remove_cout_slash(rule);
            if(token==rule){
                pop=1;
                expRules.pop();
            }
            else{
                //error;
                if(takeShot){
                 Error(expRules.top(),"Stack");
                 Error_file(expRules.top(),"Stack",myfile);
                }
               // ok=false;
               lexical::set_ok(false);
                expRules.pop();

            }

        }
        else{
            if(exprNum.find(rule)==exprNum.end()){
                //error
                if(takeShot){
                Error(expRules.top(),"Stack");
                Error_file(expRules.top(),"Stack",myfile);
                }
               // ok=false;
               lexical::set_ok(false);
                expRules.pop();

                continue;
            }
            if(terminalNum.find(token)==terminalNum.end()){
                //error
                if(takeShot){
                Error(tok,"Input");
                Error_file(tok,"Input",myfile);
                }
                //ok=false;
                lexical::set_ok(false);
                pop=1;

                continue;
            }
                int exprNumber=exprNum[rule];
            int termNum=terminalNum[token];

            if(parseTable[exprNumber][termNum]==-1){
                //error
                if(takeShot){
                Error(tok,"Input");
                  Error_file(tok,"Input",myfile);

                }

               // ok=false;
               lexical::set_ok(false);
                pop=1;
            }
            else if(parseTable[exprNumber][termNum]==-2){
                //error
                if(takeShot){
                Error (expRules.top(),"Stack");
                Error_file (expRules.top(),"Stack",myfile);
                }

                //ok=false;
                lexical::set_ok(false);
                expRules.pop();
            }
            else{
                expRules.pop();
                for(int i=CFG[exprNumber][parseTable[exprNumber][termNum]].size()-1;i>=0;i--){
                        if(CFG[exprNumber][parseTable[exprNumber][termNum]][i]==epson)
                                continue;
                        expRules.push(CFG[exprNumber][parseTable[exprNumber][termNum]][i]);
                }

            }



        }


    }


        if(expRules.size()!=0){
            if(takeShot){
             printLeftDerivation(expRules);
             printLeftDerivation_file(expRules,myfile);
              Error("The Reminder","Stack");
               Error_file("The Reminder","Stack",myfile);
            }
          //ok=false;
          lexical::set_ok(false);
        //error
    }
    else if(getline(tokens,tok)){
        if(takeShot){
            printLeftDerivation(expRules);
            printLeftDerivation_file(expRules,myfile);
              Error("The Reminder","Input");
              Error_file("The Reminder","Input",myfile);
            }
         // ok=false;
         lexical::set_ok(false);
    }
    tokens.close();
    if(takeShot)
    myfile.close();

}
void parser ::init_parse_table(){

    terminalNum[dollarSign]=terminalCounter++;
    terminal.push_back(dollarSign);
   for(int i=0;i<num_of_expr;i++){
        for(int j=0;j<terminalCounter;j++){
            parseTable[i][j]=-1;
            }
    }

}


 void  AddExprLeftFactoring(vector<int>num,int exprNumber){
        int counter=1;
        string s=name_of_expr[exprNumber];
      string String = static_cast<ostringstream*>( &(ostringstream() << counter) )->str();
      s+=String;

      while(exprNum.find(s)!=exprNum.end()){
        counter++;
        String = static_cast<ostringstream*>( &(ostringstream() << counter) )->str();
        s=name_of_expr[exprNumber];
        s+=String;

      }
      int mi=0;
      exprNum[s]=num_of_expr;
      name_of_expr.push_back(s);
        vector<string> tem;
        tem.push_back(CFG[exprNumber][num[0]][0]);
        tem.push_back(s);
      CFG[exprNumber].push_back(tem);

      for(int i=0;i<num.size();i++){
            tem.clear();
         for(int j=1;j<CFG[exprNumber][num[i]-mi].size();j++){
            tem.push_back(CFG[exprNumber][num[i]-mi][j]);
         }
         if(tem.size()==0)
            tem.push_back(epson);
            CFG[num_of_expr].push_back(tem);
             CFG[exprNumber].erase (CFG[exprNumber].begin()+(num[i]-mi));
             mi++;
      }

    num_of_expr++;

 }

bool leftFactoring(int expNumber){
    vector<int>send;
    for(int i=0;i<CFG[expNumber].size();i++){
        string s=CFG[expNumber][i][0];
            send.clear();
            send.push_back(i);
         for(int j=i+1;j<CFG[expNumber].size();j++){
            if(s==CFG[expNumber][j][0]){
                send.push_back(j);
            }
         }
         if(send.size()>1){
            AddExprLeftFactoring(send,expNumber);
            return true;
         }
    }
    return false;
}


void parser::buildLeftFactoring(){
    for(int i=0;i<num_of_expr;i++){
        while(leftFactoring(i))
            leftFactoring(i);
    }
}

void addExprLeftRecursion(int num,int exprNumber){
     int counter=1;
        string s=name_of_expr[exprNumber];
      string String = static_cast<ostringstream*>( &(ostringstream() << counter) )->str();
      s+=String;

      while(exprNum.find(s)!=exprNum.end()){
        counter++;
        String = static_cast<ostringstream*>( &(ostringstream() << counter) )->str();
        s=name_of_expr[exprNumber];
        s+=String;

      }
       int mi=0;
      exprNum[s]=num_of_expr;
      name_of_expr.push_back(s);
    vector <string>tem;
    for(int i=1;i<CFG[exprNumber][num].size();i++)
        tem.push_back(CFG[exprNumber][num][i]);

        tem.push_back(s);
        CFG[num_of_expr].push_back(tem);
        tem.clear();
        tem.push_back(epson);
        CFG[num_of_expr].push_back(tem);
        CFG[exprNumber].erase(CFG[exprNumber].begin()+num);
        for(int i=0;i<CFG[exprNumber].size();i++)
            CFG[exprNumber][i].push_back(s);

        num_of_expr++;
}
void CheckLeftRecursion(int exprNumber){
    for(int i=0;i<CFG[exprNumber].size();i++){
        if(CFG[exprNumber][i][0]==name_of_expr[exprNumber]){
            addExprLeftRecursion(i,exprNumber);
        }

    }
}
void parser::buildLeftRecursion(){
    for(int i=0;i<num_of_expr;i++){
            CheckLeftRecursion(i);

    }

}
