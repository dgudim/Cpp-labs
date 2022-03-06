#include <iostream>
#include <iterator>
#include <stack>
#include <sstream>
#include <vector>
using namespace std;
 
bool in_number(const string &symbol);
int get_priority(const string &c);
bool is_operator(const string &c);

int main()
{
    string infix = "3 ^ 4 + ( 11 - ( 3 * 2 ) ) / 2";//our infix expression
    istringstream iss(infix);
    vector<string> tokens;//store the tokens here
    while(iss)
    {
        string temp;
        iss >> temp;
        tokens.push_back(temp);
    }
    
    vector<string> outputList;//output vector
    stack<string> s;//main stack
 
    for(unsigned int i = 0; i < tokens.size(); i++)  //read from right to left
    {
        if(in_number(tokens[i]))
        {
            outputList.push_back(tokens[i]);
        }
        if(tokens[i] == "(")
        {
            s.push(tokens[i]);
        }
        if(tokens[i] == ")")
        {
            while(!s.empty() && s.top() != "(")
            {
                outputList.push_back(s.top());
                s.pop();
            }
            s.pop();
        }
        if(is_operator(tokens[i]))
        {
            while(!s.empty() && get_priority(s.top()) >= get_priority(tokens[i]))
            {
                outputList.push_back(s.top());
                s.pop();
            }
            s.push(tokens[i]);
        }
    }
    //pop any remaining operators from the stack and insert to outputlist
    while(!s.empty())
    {
        outputList.push_back(s.top());
        s.pop();
    }
 
    for(unsigned int i = 0; i < outputList.size(); i++)
    {
        cout << outputList[i];
    }
    return 0;
}
bool in_number(const string &symbol)
{
    for(unsigned int i = 0; i < symbol.size(); i++)
    {
        if(!isdigit(symbol[i]))
        {
            return false;
        }
    }
    return true;
}
int get_priority(const string &c)
{    
    return (c == "+" || c == "-") + (c == "*" || c == "/") * 2 + (c == "^") * 3;
}
bool is_operator(const string &c)
{
    return (c == "+" || c == "-" || c == "*" || c == "/" || c == "^");
}