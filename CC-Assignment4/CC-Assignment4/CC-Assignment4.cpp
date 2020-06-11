#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;

enum symbolType {Terminal, NonTerminal};

class Symbol
{
public:
    string name;
    symbolType type;

    Symbol()
    {
        this->name = "";
        this->type = NonTerminal;
    }

    Symbol(string name, symbolType type)
    {
        this->name = name;
        this->type = type;
    }

    string toString()
    {
        string str = this->name; 
        if (this->type == NonTerminal)
        {    
            transform(str.begin(), str.end(), str.begin(), ::toupper);
        }
        else
        {
            transform(str.begin(), str.end(), str.begin(), ::tolower);
        }
        return str;
    }
};


class GrammerRule
{
public:
    Symbol lhs;
    vector<vector<Symbol>> rhs;

    GrammerRule(string lhs)
    {
        this->lhs = Symbol(lhs, NonTerminal);
    }

    void addAtRight(string name, symbolType type, bool beginNew = false)
    {
        Symbol s(name, type);
        if (beginNew == true)
        {
            vector<Symbol> tempVec;
            rhs.push_back(tempVec);
        }
        rhs[rhs.size() - 1].push_back(s);
    }

    string toString()
    {
        string str;
        str += this->lhs.toString() + " -> ";
        for (int i = 0; i < rhs.size(); i++)
        {
            for (int j = 0; j < rhs[i].size(); j++)
            {
                str += rhs[i][j].toString() + " ";
            }
            if (i < (rhs.size() - 1))
            {
                str += "| ";
            }
        }
        return str;
    }

    void print()
    {
        cout << this->toString() << endl;
    }

    bool checkLeftRecursion()
    {
        for (int i = 0; i < rhs.size(); i++)
        {
            if (rhs[i][0].name == lhs.name && rhs[i][0].type == lhs.type) return true;
        }
        return false;
    }

    bool checkLeftFactoring()
    {
        for (int i = 0; i < rhs.size(); i++)
        {
            if (rhs[i][0].type == Terminal)
            {
                for (int j = i + 1; j < rhs.size(); j++)
                {
                    if (rhs[j][0].name == rhs[i][0].name && rhs[i][0].type == Terminal) return true;
                }
            }
        }
        return false;
    }
};

int main()
{
    GrammerRule g1("A");
    g1.addAtRight("b", Terminal, true);
    g1.addAtRight("c", Terminal);
    g1.addAtRight("b", NonTerminal, true);
    g1.print();
    cout << g1.checkLeftFactoring() << endl;
}
