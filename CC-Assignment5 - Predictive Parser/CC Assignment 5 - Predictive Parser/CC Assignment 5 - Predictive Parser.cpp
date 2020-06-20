#include<iostream>
#include<string>
#include<deque>
#include<vector>
#include<fstream>
#include<sstream>

using namespace std;

class csvManager
{
public:
    int maxRows;
    int maxCols;
    vector<vector<string>> fileContent;

    csvManager()
    {
        maxRows = 0;
        maxCols = 0;
    }

    bool read(string filePath)
    {
        ifstream  iFile(filePath);
        bool read = iFile.good();
        string line;
        for (int i = 0; getline(iFile, line); i++)
        {
            if (maxRows < i + 1) maxRows = i + 1;
            stringstream  lineStream(line);
            string        cell;
            vector<string> cellsLine;
            for (int j = 0; getline(lineStream, cell, ','); j++)
            {
                if (maxCols < j + 1) maxCols = j + 1;
                cellsLine.push_back(rectifyString(cell));
            }
            fileContent.push_back(cellsLine);
        }
        iFile.close();
        refreshContent();
        return read;
    }

    void refreshContent()
    {
        for (int i = 0; i < fileContent.size(); i++)
        {
            while (fileContent[i].size() < maxCols)
            {
                fileContent[i].push_back("");
            }
        }
    }

    string rectifyString(string str)
    {
        string result = "";
        for (int i = 0; i < str.length(); i++)
        {
            if (validateCharacter(str[i])) result += str[i];
        }
        return result;
    }

    bool validateCharacter(char c)
    {
        if (c > -1 && c < 255) return true;
        return false;
    }

    void addCell(string cell, int row, int col)
    {

        for (int i = 0; i < row + 1; i++)
        {
            if (fileContent.size() < i + 1)
            {
                vector<string> cellsLine;
                fileContent.push_back(cellsLine);
            }
            for (int j = 0; j < col + 1; j++)
            {
                if (fileContent[i].size() < j + 1) fileContent[i].push_back("");
            }
        }

        fileContent[row][col] = cell;
    }

    void refreshDimensions()
    {
        for (int i = 0; i < fileContent.size(); i++)
        {
            if (maxRows < i + 1) maxRows = i + 1;
            for (int j = 0; j < fileContent[i].size(); j++)
            {
                if (maxCols < j + 1) maxCols = j + 1;
            }
        }
    }

    void print()
    {
        for (int i = 0; i < fileContent.size(); i++)
        {
            for (int j = 0; j < fileContent[i].size(); j++)
            {
                cout << i << " " << j << " " << fileContent[i][j] << endl;
            }
        }
    }

    bool writeToFile(string filePath)
    {
        ofstream oFile;
        oFile.open(filePath);
        bool read = oFile.good();
        for (int i = 0; i < fileContent.size(); i++)
        {
            for (int j = 0; j < fileContent[i].size(); j++)
            {
                if (j == 0) oFile << fileContent[i][j];
                else oFile << "," << fileContent[i][j];
            }
            oFile << endl;
        }
        oFile.close();
        return read;
    }
};

int numOfProds, numOfNonTerms, numOfTerms;

int getPosition(string arr[], string q, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (q == arr[i])
            return i;
    }
    return -1;
}

class PParser
{
public:
    int numOfProds;
    int numOfNonTerms;
    int numOfTerms;
    string terms[20];
    string nonterms[10];
    string pp_table[20][20] = {};
    csvManager manager;

    PParser(string fileName)
    {    
        manager.read(fileName);
        fillTerms();
        fillNonTerms();
        fillPPTable();
    }

    void fillTerms()
    {
        numOfTerms = manager.fileContent[0].size() - 1;
        for (int i = 1; i < manager.fileContent[0].size(); i++)
        {
            terms[i - 1] = manager.fileContent[0][i];
        }
    }

    void fillNonTerms()
    {
        numOfNonTerms = 0;
        for (int i = 1; i < manager.fileContent.size(); i++)
        {
            nonterms[i - 1] = manager.fileContent[i][0];
            numOfNonTerms++;
        }
    }

    void fillPPTable()
    {
        for (int i = 0; i < numOfNonTerms; i++)
        {
            for (int j = 0; j < numOfTerms; j++)
            {
                pp_table[i][j] = manager.fileContent[i + 1][j + 1];
            }
        }
    }

    void printTable()
    {
        for (int j = 0; j < numOfTerms; j++)
            cout << "\t" << terms[j];
        cout << endl;
        for (int i = 0; i < numOfNonTerms; i++)
        {
            cout << nonterms[i] << "\t";
            for (int j = 0; j < numOfTerms; j++)
            {
                cout << pp_table[i][j] << "\t";
            }
            cout << endl;
        }
    }

    void parseString()
    {
        char c;
        do {
            string ip;
            deque<string> pp_stack;
            pp_stack.push_front("$");                       
            pp_stack.push_front(nonterms[0]);     
            cout << "Enter the string to be parsed : ";
            getline(cin, ip);                               
            ip.push_back('$');                              
            cout << "Stack\tInput\tAction" << endl;
            while (true)
            {
                for (int i = 0; i < pp_stack.size(); i++)
                    cout << pp_stack[i];
                cout << "\t" << ip << "\t";
                int row1 = getPosition(nonterms, pp_stack.front(), numOfNonTerms);
                int row2 = getPosition(terms, pp_stack.front(), numOfTerms);
                int column = getPosition(terms, ip.substr(0, 1), numOfTerms);
                if (row1 != -1 && column != -1)
                {
                    string p = pp_table[row1][column];
                    if (p.empty())
                    {
                        cout << endl << "String cannot be Parsed." << endl;
                        break;
                    }
                    pp_stack.pop_front();
                    if (p[3] != '#')
                    {
                        for (int x = p.size() - 1; x > 2; x--)
                        {
                            pp_stack.push_front(p.substr(x, 1));
                        }
                    }
                    cout << p;
                }
                else
                {
                    if (ip.substr(0, 1) == pp_stack.front())
                    {
                        if (pp_stack.front() == "$")
                        {
                            cout << endl << "String Parsed." << endl;
                            break;
                        }
                        cout << "Match " << ip[0];
                        pp_stack.pop_front();
                        ip = ip.substr(1);
                    }
                    else
                    {
                        cout << endl << "String cannot be Parsed." << endl;
                        break;
                    }
                }
                cout << endl;
            }
            cout << "Continue?(Y/N) ";
            cin >> c;
            cin.ignore();
        } while (c == 'y' || c == 'Y');
    }

    void perform()
    {
        string prods[10], first[10], follow[10], nonterms[10], terms[10];
        string pp_table[20][20] = {};

        numOfProds = 5;
        numOfTerms = 5;
        numOfNonTerms = 3;

        prods[0] = "S->aXYb";
        prods[1] = "X->c";
        prods[2] = "X->#";
        prods[3] = "Y->d";
        prods[4] = "Y->#";

        first[0] = "a";
        first[1] = "c";
        first[2] = "#";
        first[3] = "d";
        first[4] = "#";

        terms[0] = "a";
        terms[1] = "b";
        terms[2] = "c";
        terms[3] = "d";
        terms[4] = "$";

        nonterms[0] = "S";
        nonterms[1] = "X";
        nonterms[2] = "Y";

        follow[0] = "$";
        follow[1] = "bd";
        follow[2] = "b";


        for (int j = 0; j < numOfProds; j++)
        {
            int row = getPosition(nonterms, prods[j].substr(0, 1), numOfNonTerms);
            if (prods[j].at(3) != '#')
            {
                for (int i = 0; i < first[j].length(); i++)
                {
                    int col = getPosition(terms, first[j].substr(i, 1), numOfTerms);
                    pp_table[row][col] = prods[j];
                }
            }
            else
            {
                for (int i = 0; i < follow[row].length(); i++)
                {
                    int col = getPosition(terms, follow[row].substr(i, 1), numOfTerms);
                    pp_table[row][col] = prods[j];
                }
            }
        }
        //Display Table
        for (int j = 0; j < numOfTerms; j++)
            cout << "\t" << terms[j];
        cout << endl;
        for (int i = 0; i < numOfNonTerms; i++)
        {
            cout << nonterms[i] << "\t";
            //Display Table
            for (int j = 0; j < numOfTerms; j++)
            {
                cout << pp_table[i][j] << "\t";
            }
            cout << endl;
        }
        //Parsing String
        char c;
        do {
            string ip;
            deque<string> pp_stack;
            pp_stack.push_front("$");                       //push $ to stack
            pp_stack.push_front(prods[0].substr(0, 1));     //push starting non-terminal on stack
            cout << "Enter the string to be parsed : ";
            getline(cin, ip);                               //get input string to be parsed
            ip.push_back('$');                              //place $ at end of input string
            cout << "Stack\tInput\tAction" << endl;
            while (true)
            {
                for (int i = 0; i < pp_stack.size(); i++)
                    cout << pp_stack[i];
                cout << "\t" << ip << "\t";
                int row1 = getPosition(nonterms, pp_stack.front(), numOfNonTerms);
                int row2 = getPosition(terms, pp_stack.front(), numOfTerms);
                int column = getPosition(terms, ip.substr(0, 1), numOfTerms);
                if (row1 != -1 && column != -1)
                {
                    string p = pp_table[row1][column];
                    if (p.empty())
                    {
                        cout << endl << "String cannot be Parsed." << endl;
                        break;
                    }
                    pp_stack.pop_front();
                    if (p[3] != '#')
                    {
                        for (int x = p.size() - 1; x > 2; x--)
                        {
                            pp_stack.push_front(p.substr(x, 1));
                        }
                    }
                    cout << p;
                }
                else
                {
                    if (ip.substr(0, 1) == pp_stack.front())
                    {
                        if (pp_stack.front() == "$")
                        {
                            cout << endl << "String Parsed." << endl;
                            break;
                        }
                        cout << "Match " << ip[0];
                        pp_stack.pop_front();
                        ip = ip.substr(1);
                    }
                    else
                    {
                        cout << endl << "String cannot be Parsed." << endl;
                        break;
                    }
                }
                cout << endl;
            }
            cout << "Continue?(Y/N) ";
            cin >> c;
            cin.ignore();
        } while (c == 'y' || c == 'Y');
    }
};

int main()
{
    PParser p("E:\\git\\CC-Course\\CC-Assignment5 - Predictive Parser\\pp_table.csv");
    p.printTable();
    p.parseString();
}