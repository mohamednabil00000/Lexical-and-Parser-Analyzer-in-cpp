#include "lexical.h"
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

lexical::lexical()
{
    //ctor
}

vector< Node> NFA[500000];
State acceptancyState[500000];
vector<vector<string> >definations;
vector<vector<string> >regularExpression;
vector<string>keyWords;
vector<string>Punctuations;
map<string,int>def_names;
map<string,int>reg_names;
map<int,string>reg_names_by_int;
vector<string>priority;
//vector<string> *tokens = new vector<string> ();
//vector <string> codeWords;
//queue<string> tokens;
//vector<string> tokens2;
set<char>transitionSymbol;
map<int,int>state_in_set;
set<int>clas[500000];
set<int>transState[50000];
int stateDFA=1;
int transTable[50000][128];
int same[50000];
vector<FinalTransition>final_transition_table;
bool level;
bool previousLevel;
vector<FinalTransition>after_minimization;
int start_state_after_minimization;
bool ok=true;
string key="Keyword";
string punc="Punctuation";
int state=3;
//zero start state
//1 for punctuation
//2 for keywords


//*************getters& setters*******************
/*vector<string>  lexical::get_tokens2(){
  return tokens2;
}

queue<string>  lexical::get_tokens(){
  return tokens;
}

void lexical :: set_tokens(string a){
tokens.push(a);
}


void lexical :: set_tokens2(string a){
  tokens2.push_back(a);
}

void lexical:: pop_tokens(){
 tokens.pop();
}*/
 bool lexical::get_ok(){
   return ok;
 }
 void lexical::set_ok(bool o){
 ok=o;
 }



//postfix....................................................


vector<string> postFix(vector<string>inp,int start){

    vector<string>ans;
    stack<string>s;

    for(int i=start;i<inp.size();i++){
            if(inp[i]=="-"){
                s.push(inp[i]);
            }
            else if(inp[i]=="("){
                if(i>start&&inp[i-1]==")")
                    s.push("\\&");
                s.push(inp[i]);
            }
            else if(inp[i]=="|"){

                while(!s.empty()){
                    string st=s.top();
                    s.pop();
                    if(st=="-"||st=="\\&")
                        ans.push_back(st);
                    else{
                        s.push(st);
                        break;
                    }
                }
                s.push(inp[i]);
            }
            else if(inp[i]==")"){

                while(!s.empty())
                {
                    string st=s.top();
                    s.pop();
                    if(st=="(")
                        break;
                    ans.push_back(st);
                }
            }
            else if(inp[i]=="+"||inp[i]=="*")
                ans.push_back(inp[i]);
            else{

                if(i>start&&inp[i-1]!="|"&&inp[i-1]!="-"&&inp[i-1]!="(")
                    s.push("\\&");

                    if(i+1<inp.size()&&inp[i+1]=="(")
                        s.push("\\&");
                ans.push_back(inp[i]);
            }
    }

    while(!s.empty()){
        ans.push_back(s.top());
        s.pop();
    }
    return ans;

}

string splitSlach(string line){
string ln="";
                for(int j=0;j<line.size();j++)
                {
                    if(line[j]=='\\'){
                        if(j+1<line.size()&&line[j+1]=='L'){
                            ln+=' ';
                            j++;
                        }
                        else if(j+1<line.size()&&line[j+1]=='\\'){
                            ln+=line[j+1];
                            j++;
                        }

                        continue;
                    }

                    ln+=line[j];
                }
                return  ln;
}


void lexical :: scan(string inp){
priority.push_back(punc);
        priority.push_back(key);
    vector<string>temp;
    string emp="";
    for(int i=0;i<inp.size();i++){
        if(inp[i]==' '){
              if(emp!="")
              temp.push_back(emp);
            emp="";
            continue;
        }
        if((inp[i]=='='||inp[i]==':')&&temp.size()>1){
            emp+=inp[i];
            continue;
        }

        if(inp[i]=='['||inp[i]==']'||inp[i]=='{'||inp[i]=='}'||inp[i]=='|'||inp[i]=='('||inp[i]==')'||inp[i]==':'||inp[i]=='+'||inp[i]=='-'||inp[i]=='*'||inp[i]=='='){
              if(i>0&&inp[i-1]=='\\')
              emp+=inp[i];
              else{
              if(emp!="")
              temp.push_back(emp);
                emp="";
                emp+=inp[i];
                temp.push_back(emp);
                emp="";
              }
                continue;
        }
         emp+=inp[i];
    }

    if(emp!="")
        temp.push_back(emp);



    if(temp[0]=="["){

        for(int i=1;i<temp.size()-1;i++)
            Punctuations.push_back(splitSlach(temp[i]));



    }
    else if(temp[0]=="{"){
        for(int i=1;i<temp.size()-1;i++)
            keyWords.push_back(splitSlach(temp[i]));

    }
    else if(temp[1]=="="){
        //tie_with_start_state(splitChar(temp[0]));

            def_names.insert(pair<string,int>(temp[0],definations.size()));

            definations.push_back(postFix(temp,2));
    }
    else if(temp[1]==":"){
            priority.push_back(temp[0]);
           // tie_with_start_state(splitChar(temp[0]));
             reg_names.insert(pair<string,int>(temp[0],regularExpression.size()));
     reg_names_by_int.insert(pair<int,string>(regularExpression.size(),temp[0]));
                regularExpression.push_back(postFix(temp,2));
    }


}
/*void lexical::readLexicalRule(){
    string line;
  ifstream myfile ;
  myfile.open("example.txt");

    while ( getline (myfile,line) )
    {
        scan(line);
    }
    myfile.close();


}*/


//NFAAAAAAA.........................................
void tie_with_start_state(stack_value tie,string regular){

 if(tie.st==-1){
         NFA[0].push_back(Node(state,tie.value));
         if(regular==key){
                NFA[state].push_back(Node(1,' '));
            acceptancyState[1].endState=1;
                acceptancyState[1].reg_of_end_state=regular;

         }
         else if(regular==punc){
         NFA[state].push_back(Node(2,' '));
            acceptancyState[2].endState=1;
                acceptancyState[2].reg_of_end_state=regular;
         }
         else{
          acceptancyState[state].endState=1;
         acceptancyState[state].reg_of_end_state=regular;
        }
         state++;

    }
    else{
    NFA[0].push_back(Node(tie.st,' '));

     if(regular==key){
                NFA[tie.ended].push_back(Node(1,' '));
            acceptancyState[1].endState=1;
                acceptancyState[1].reg_of_end_state=regular;

         }
         else if(regular==punc){
         NFA[tie.ended].push_back(Node(2,' '));
            acceptancyState[2].endState=1;
                acceptancyState[2].reg_of_end_state=regular;
         }
         else{
    acceptancyState[tie.ended].endState=1;
    acceptancyState[tie.ended].reg_of_end_state=regular;
        }
    }
}

stack_value concatenate(stack_value a,stack_value b){
        int start=0;
        int endd=0;

            if(a.st!=-1&&b.st!=-1){
            NFA[a.ended].push_back(Node(b.st,' '));
           // acceptancyState[a.ended].endState=0;
            start=a.st;
            endd=b.ended;
        }
        else if(a.st!=-1){
                NFA[a.ended].push_back(Node(state,b.value));
         //   acceptancyState[state].endState=1;
         //acceptancyState[a.ended].endState=0;
            start=a.st;
            endd=state;
            state++;
        }
        else if(b.st!=-1){
                NFA[state].push_back(Node(b.st,a.value));
            start=state;
            endd=b.ended;
            state++;
        }
        else{
            NFA[state].push_back(Node(state+1,a.value));
            NFA[state+1].push_back(Node(state+2,b.value));
           // acceptancyState[state+2].endState=1;
            start=state;
            endd=state+2;
            state+=3;

        }
        return stack_value(start,endd,' ');
}
stack_value concChar(string ln){


                    stack<stack_value> temp;
                for(int j=0;j<ln.size();j++){
                        temp.push(stack_value(-1,-1,ln[j]));
                        transitionSymbol.insert(ln[j]);

                }

                    while(!temp.empty()&&temp.size()>1){
                        stack_value two=temp.top();
                        temp.pop();
                        stack_value one=temp.top();
                        temp.pop();
                        temp.push(concatenate(one,two));
                    }

             return temp.top();

}
stack_value clausre(stack_value a){
        int start=state;
    int endd=0;
                if(a.st!=-1){
                NFA[state].push_back(Node(a.st,' '));
                NFA[a.ended].push_back(Node(state+1,' '));
            NFA[state].push_back(Node(state+1,' '));
            NFA[a.ended].push_back(Node(state,' '));
          //  acceptancyState[a.ended].endState=0;
          //  acceptancyState[state+1].endState=1;
            state+=2;
            endd=state-1;

        }
        else{
                NFA[state].push_back(Node(state+1,' '));
                NFA[state+1].push_back(Node(state+2,a.value));
            NFA[state+2].push_back(Node(state+3,' '));
            NFA[state].push_back(Node(state+3,' '));
            NFA[state+2].push_back(Node(state,' '));
          //  acceptancyState[state+3].endState=1;
            state+=4;
            endd=state-1;
        }
        return stack_value(start,endd,' ');

}

stack_value postiveClausre(stack_value a){
        int start=state;
    int endd=0;
                if(a.st!=-1){
                NFA[state].push_back(Node(a.st,' '));
                NFA[a.ended].push_back(Node(state+1,' '));
           // NFA[state].push_back(Node(state+1,"L"));
            NFA[a.ended].push_back(Node(state,' '));
          //  acceptancyState[a.ended].endState=0;
           // acceptancyState[state+1].endState=1;
            state+=2;
            endd=state-1;

        }
        else{
                NFA[state].push_back(Node(state+1,' '));
                NFA[state+1].push_back(Node(state+2,a.value));
            NFA[state+2].push_back(Node(state+3,' '));
       //     NFA[state].push_back(Node(state+3,"L"));
            NFA[state+2].push_back(Node(state,' '));
          //  acceptancyState[state+3].endState=1;
            state+=4;
            endd=state-1;
        }
        return stack_value(start,endd,' ');
}

stack_value oring(stack_value a,stack_value b){



        NFA[state].push_back(Node (state+1,' '));
    NFA[state].push_back(Node (state+2,' '));


    if(a.st!=-1){
        NFA[state+1].push_back(Node(a.st,' '));
        NFA[a.ended].push_back(Node(state+3,' '));
     //   acceptancyState[a.ended].endState=0;

        }

    else{
        NFA[state+1].push_back(Node(state+3,a.value));
        }


    if(b.st!=-1){
        NFA[state+2].push_back(Node(b.st,' '));
        NFA[b.ended].push_back(Node(state+4,' '));
     //  acceptancyState[b.ended].endState=0;
        }
    else{
        NFA[state+2].push_back(Node(state+4,b.value));

        }
        NFA[state+3].push_back(Node(state+5,' '));
        NFA[state+4].push_back(Node(state+5,' '));


     //   acceptancyState[state+5].endState=1;
        state+=6;

        return stack_value(state-6,state-1,' ');

}



stack_value range(stack_value a,stack_value b){
        char one =a.value;
    char two=b.value;

    for(int i=one;i<=two;i++){
        char val=(char)i;
        transitionSymbol.insert(val);
        NFA[state].push_back(Node(state+1,val));
    }
   // acceptancyState[state+1].endState=1;
    state+=2;
    return stack_value(state-2,state-1,' ');

}



stack_value evaluate (vector<string> line){

        stack<stack_value>s;

    for(int i=0;i<line.size();i++){


        if(line[i]=="*"){
             stack_value one=s.top();
            s.pop();
            s.push(clausre(one));

        }
        else if(line[i]=="|"){
                stack_value one=s.top();
            s.pop();
            stack_value two=s.top();
            s.pop();

            s.push(oring(one,two));

        }
        else if(line[i]=="-"){
                stack_value two=s.top();
             s.pop();
            stack_value one=s.top();
            s.pop();
            s.push(range(one,two));
        }
        else if(line[i]=="+"){
             stack_value one=s.top();
            s.pop();
            s.push(postiveClausre(one));
        }
        else if(line[i]=="\\&"){
                if(s.size()<2)
                continue;

            stack_value two=s.top();
            s.pop();
            stack_value one=s.top();
            s.pop();

            s.push(concatenate(one,two));

        }
        else {
                if(def_names.find(line[i])!= def_names.end()){
                int no_of_row=def_names[line[i]];
                s.push(evaluate(definations[no_of_row]));
            }
            else if(reg_names.find(line[i])!= reg_names.end()){
                int no_of_row=reg_names[line[i]];
                s.push(evaluate(regularExpression[no_of_row]));
            }
            else{
                s.push(concChar(splitSlach(line[i])));
            }
        }
    }


    while(!s.empty()&&s.size()>1){
        stack_value two=s.top();
        s.pop();
        stack_value one=s.top();
        s.pop();

        s.push(concatenate(one,two));

    }

    stack_value tie=s.top();
    s.pop();
    return tie;

}
    void lexical:: buildNFA(){
      for(int i=0;i<keyWords.size();i++)
        tie_with_start_state(concChar(keyWords[i]),key);
        for(int i=0;i<Punctuations.size();i++)
        tie_with_start_state(concChar(Punctuations[i]),punc);
        for(int i=0;i<regularExpression.size();i++)
        tie_with_start_state(evaluate(regularExpression[i]),reg_names_by_int[i]);

}

//DFAAAAAAAAAAAAAAAAAA.................................................................................................................




void dfs (int n,int orig){
        clas[orig].insert(n);
        for(int i=0;i<NFA[n].size();i++){
        Node node=NFA[n][i];
        if(node.value==' '&&clas[orig].find(node.to)==clas[orig].end()){
                dfs(node.to,orig);
        }
    }

return ;

}
void lexical::clausre(){
        for(int i=0;i<state;i++){
        dfs(i,i);
    }
}
//stat.push(value);
//stat.top() then state.pop()
void lexical::buildTransition(){
        transState[stateDFA++].insert(0);
    queue<int>stat ;
    queue<int>emptyState;
    stat.push(1);
    State acceptanceDFA[50000];

    while(!stat.empty()){
        int ss=stat.front();
        stat.pop();
            bool gone=true;

                for(int i=1;i<ss;i++){
                if(same[i]==0&&transState[i]==transState[ss]){
                       // cout<<"aaaaa"<<endl;
                        same[ss]=i;
                    emptyState.push(ss);
                    gone=false;
                    break;
                }
            }
            if(gone){
        for (set<int>::iterator i = transState[ss].begin(); i !=transState[ss].end(); i++) {
                int element = *i;
                for (set<int>::iterator j = clas[element].begin(); j != clas[element].end(); j++) {
            int elem = *j;
                    if(acceptancyState[elem].endState==1){
                                acceptanceDFA[ss].endState=1;
                            if(acceptanceDFA[ss].reg_of_end_state=="")
                            acceptanceDFA[ss].reg_of_end_state=acceptancyState[elem].reg_of_end_state;
                            else{
                                for(int j2=0;j2<priority.size();j2++){
                                        if(priority[j2]==acceptanceDFA[ss].reg_of_end_state)
                                    break;
                                     if(priority[j2]==acceptancyState[elem].reg_of_end_state){
                                      acceptanceDFA[ss].reg_of_end_state=acceptancyState[elem].reg_of_end_state;
                                      break;
                                     }
                                  }

                                }

                        }
                        for(int k=0;k<NFA[elem].size();k++){
                        Node node=NFA[elem][k];
                    if(node.value==' ')
                                 continue;


                    int ascii=(int)node.value;


                    if(transTable[ss][ascii]==0){
                        transTable[ss][ascii]=stateDFA;
                        transState[stateDFA].insert(node.to);
                        stat.push(stateDFA);

                        //endState=a;
                                          // reg_of_end_state=b;


                        stateDFA++;

                    }
                    else{
                        transState[transTable[ss][ascii]].insert(node.to);


                    }



                         }

                }
        }

        }

    }

    for(int i=1;i<stateDFA;i++){
            if(same[i]!=0)
            continue;
        vector<int>temp;
        for(int j=0;j<128;j++){
                if(same[transTable[i][j]]!=0){
                transTable[i][j]=same[transTable[i][j]];
            }
            temp.push_back(transTable[i][j]);
        }
       // cout<<acceptanceDFA[i].reg_of_end_state<<endl;
        final_transition_table.push_back(FinalTransition (i,temp,acceptanceDFA[i].endState==1?1:0,acceptanceDFA[i].reg_of_end_state));
    }

    int change[50000];
    int nextState=0;
    change[0]=-1;
    for(int i=0;i<final_transition_table.size();i++){
       // if(final_transition_table[i].state_name!=nextState)
        change[final_transition_table[i].state_name]=nextState;

        nextState++;
    }

     for(int i=0;i<final_transition_table.size();i++){
         final_transition_table[i].state_name=change[final_transition_table[i].state_name];
         for(int j=0;j<128;j++){
            final_transition_table[i].final_transition_table[j]=change[final_transition_table[i].final_transition_table[j]];
         }
     }

}

//Minimization...............................................................................................................

  // int set_num;
  //  int num_level;
  //  vector<int> currentSet;


void lexical::minimize(){
        previousLevel=0;
    level=1;
    int num_of_set=2;
    vector<Minimization>setState;
    vector<int>setOne;
    vector<int>setZero;
    state_in_set[-1]=-1;
    //dead state
    for(int i=0;i<final_transition_table.size();i++){
        if(final_transition_table[i].endState==1){
                setOne.push_back(i);
            state_in_set[i]=1;
            }
        else{
            setZero.push_back(i);
             state_in_set[i]=0;
            }
    }


    setState.push_back(Minimization(0,0,setZero));
    setState.push_back(Minimization(1,0,setOne));

    queue<Minimization>q;
    q.push(setState[0]);
    q.push(setState[1]);
    int currentLevel=0;
    while(!q.empty()){
        Minimization e=q.front();
        q.pop();

        if(e.num_level!=currentLevel){
                previousLevel=level;
            level=1;
                currentLevel++;
        }

        if(previousLevel)
                break;

            vector<vector<int> >temp;

        for(int i=0;i<e.currentSet.size();i++){
                int state=e.currentSet[i];
            vector<int>nextSet;
                for(int j=0;j<128;j++){
                nextSet.push_back(state_in_set[final_transition_table[state].final_transition_table[j]]);
            }
            temp.push_back(nextSet);
        }
        bool vis[50000];
        memset(vis,0,sizeof(vis));

        int ch=0;
        for(int i=0;i<temp.size();i++){
                        if(vis[i]!=0)
                                 continue;
                     vis[i]=1;
                     ch++;
                Minimization newSet;
            vector<int>new_set;
            new_set.push_back(e.currentSet[i]);
            state_in_set[e.currentSet[i]]=num_of_set;
                for(int j=i+1;j<temp.size();j++){
                if(vis[j]!=0)
                continue;
                int k;
                for( k=0;k<128;k++){
                        if(temp[i][k]!=temp[j][k])
                    break;

                }
                if(k==128&&final_transition_table[e.currentSet[i]].endStateValue==final_transition_table[e.currentSet[j]].endStateValue){
                        new_set.push_back(e.currentSet[j]);
                    state_in_set[e.currentSet[j]]=num_of_set;
                    vis[j]=1;
                }
            }

            newSet=Minimization (num_of_set,e.num_level+1,new_set);

            setState.push_back(newSet);
            q.push(newSet);
            num_of_set++;
        }
    /*cout<<e.num_level<<" "<<e.set_num<<" "<<ch<<" : ";
        for(int i=0;i<e.currentSet.size();i++)
            cout<<e.currentSet[i]<<" ";
        cout<<endl;
        */
        if(ch>1)
                level=0;



    }




    int chang[50000];
    bool del[50000];
    memset(del,0,sizeof(del));
    int no_of_state=-1;
    for(int i=0;i<setState.size();i++){
        if(setState[i].num_level==currentLevel){
                 no_of_state++;
                for(int j=0;j<setState[i].currentSet.size();j++){
                if(j>0)
                del[setState[i].currentSet[j]]=1;
                chang[setState[i].currentSet[j]]=no_of_state;
                if(setState[i].currentSet[j]==0)
                        start_state_after_minimization=no_of_state;
            }
        }

    }
    int r=0;
    for(int i=0;i<final_transition_table.size();i++){
                if(del[i]==1)
                continue;
        after_minimization.push_back(final_transition_table[i]);
      //  cout<<after_minimization[r].state_name<<endl;
        r++;
    }

    for(int i=0;i<after_minimization.size();i++){
        after_minimization[i].state_name=chang[after_minimization[i].state_name];
        //cout<<after_minimization[i].state_name<<"  "<<after_minimization[i].endStateValue<<" : "<<endl;
        for(int j=0;j<128;j++){
                if(after_minimization[i].final_transition_table[j]==-1)
                    continue;
                after_minimization[i].final_transition_table[j]=chang[after_minimization[i].final_transition_table[j]];
                //if(after_minimization[i].final_transition_table[j]!=-1)
                //cout<<(char)j<<" "<<after_minimization[i].final_transition_table[j]<<endl;

        }

    }

    int rearange[50000];
    for(int i=0;i<after_minimization.size();i++){
        rearange[after_minimization[i].state_name]=i;
    }
    vector<FinalTransition>arrange;
    for(int i=0;i<after_minimization.size();i++)
        arrange.push_back(after_minimization[rearange[i]]);


    after_minimization.clear();
    for(int i=0;i<arrange.size();i++)
        after_minimization.push_back(arrange[i]);



}


//Longest pattern............................................



void lexical::generateLexicalAnalizer(string line,ofstream &file){

   int  index=-1;
    while(index+1<line.size()){
    string longestPattern="";
    string temp="";
    int nextState=start_state_after_minimization;
    int i;
    for( i=index+1;i<line.size();i++){
        if(line[i]==' ')
        {
            if(temp!="")
                break;
            continue;
        }
                temp+=line[i];
        int ascii=(int)line[i];
        //cout<<temp<<endl;

        nextState=after_minimization[nextState].final_transition_table[ascii];
        if(nextState==-1){
                break;
        }
        else{
                if(after_minimization[nextState].endState==1){
                if(after_minimization[nextState].endStateValue==key||after_minimization[nextState].endStateValue==punc)
                longestPattern=temp;
                else
                    longestPattern=after_minimization[nextState].endStateValue;
                index=i;
            }
        }



    }

     if(longestPattern==""){
            index=i;
            if(temp=="")
                continue;
            file<<"Error in '"<<temp<<"'"<<endl;
            ok=false;

        }
        else{
               // cout<<longestPattern<<endl;
            file<<longestPattern<<endl;
                longestPattern="";
        }



    }


}





/*void lexical::buildLexicalAnalizer(){
    ok=true;
    for(int i=0;i<codeWords.size();i++)
         generateLexicalAnalizer(codeWords[i]);


}*/
void lexical::printLexicalAnalizer(){
    ifstream file;
    file.open("token.txt");
    string tok;
    while(getline(file,tok)){
        cout<<tok<<endl;
    }
    file.close();
        cout<<"*********************************************************************************************************"<<endl;
}

void lexical::printLexicalAnalizer_file(){
    ofstream myfile;
    myfile.open("lexical_analizer.txt");
    ifstream file;
    file.open("token.txt");
    string tok;
    while(getline(file,tok)){
           myfile<<tok<<endl;
    }
    file.close();
        myfile<<"*********************************************************************************************************"<<endl;
        myfile.close();
}


void lexical::printTransitionTable(){

 ofstream myfile;
  myfile.open ("transition.txt");
    myfile<<"  ";
    for (set<char>::iterator i = transitionSymbol.begin(); i != transitionSymbol.end(); i++) {
                char element = *i;
                myfile<<element<<" |";
        }
    myfile<<endl<<endl;
  for(int j=0;j<after_minimization.size();j++){
        for(int t=0;t<128;t++)
        myfile<<"_ ";
        myfile<<endl;
        myfile<<j<<" :| ";
    for (set<char>::iterator i = transitionSymbol.begin(); i != transitionSymbol.end(); i++) {
                char element = *i;
                int k=(int)element;
                if(after_minimization[j].final_transition_table[k]>9||after_minimization[j].final_transition_table[k]<0)
                myfile<<after_minimization[j].final_transition_table[k]<<"|";
                else
            myfile<<after_minimization[j].final_transition_table[k]<<" |";

        }
        myfile<<endl;
        myfile<<endl;
  }

  myfile.close();

}
