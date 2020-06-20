#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

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
		return read;
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

class FirstnFollow
{
public:
	int count, n;
	int size1 = 100;
	char calc_first[100][100];

	// Stores the final result 
	// of the Follow Sets 
	char calc_follow[100][100];
	int m;

	// Stores the production rules 
	char production[100][100];
	char f[100], first[100];
	int k;
	char ck;
	int e;


	FirstnFollow()
	{
		n = 0;
		m = 0;
		count = 0;
	}

	void followfirst(char c, int c1, int c2)
	{
		int k;

		// The case where we encounter 
		// a Terminal 
		if (!(isupper(c)))
			f[m++] = c;
		else
		{
			int i = 0, j = 1;
			for (i = 0; i < count; i++)
			{
				if (calc_first[i][0] == c)
					break;
			}

			//Including the First set of the 
			// Non-Terminal in the Follow of 
			// the original query 
			while (calc_first[i][j] != '!')
			{
				if (calc_first[i][j] != '#')
				{
					f[m++] = calc_first[i][j];
				}
				else
				{
					if (production[c1][c2] == '\0')
					{
						// Case where we reach the 
						// end of a production 
						follow(production[c1][0]);
					}
					else
					{
						// Recursion to the next symbol 
						// in case we encounter a "#" 
						followfirst(production[c1][c2], c1, c2 + 1);
					}
				}
				j++;
			}
		}
	}
	void follow(char c)
	{
		int i, j;

		// Adding "$" to the follow 
		// set of the start symbol 
		if (production[0][0] == c) {
			f[m++] = '$';
		}
		for (i = 0; i < 100; i++)
		{
			for (j = 2; j < 100; j++)
			{
				if (production[i][j] == c)
				{
					if (production[i][j + 1] != '\0')
					{
						// Calculate the first of the next 
						// Non-Terminal in the production 
						followfirst(production[i][j + 1], i, (j + 2));
					}

					if (production[i][j + 1] == '\0' && c != production[i][0])
					{
						// Calculate the follow of the Non-Terminal 
						// in the L.H.S. of the production 
						follow(production[i][0]);
					}
				}
			}
		}
	}
	void findfirst(char c, int q1, int q2)
	{
		int j;

		// The case where we 
		// encounter a Terminal 
		if (!(isupper(c))) {
			first[n++] = c;
		}
		for (j = 0; j < count; j++)
		{
			if (production[j][0] == c)
			{
				if (production[j][2] == '#')
				{
					if (production[q1][q2] == '\0')
						first[n++] = '#';
					else if (production[q1][q2] != '\0'
						&& (q1 != 0 || q2 != 0))
					{
						// Recursion to calculate First of New 
						// Non-Terminal we encounter after epsilon 
						findfirst(production[q1][q2], q1, (q2 + 1));
					}
					else
						first[n++] = '#';
				}
				else if (!isupper(production[j][2]))
				{
					first[n++] = production[j][2];
				}
				else
				{
					// Recursion to calculate First of 
					// New Non-Terminal we encounter 
					// at the beginning 
					findfirst(production[j][2], j, 3);
				}
			}
		}
	}

	void addProdcution(string str)
	{
		char* chr = const_cast<char*>(str.c_str());
		strcpy_s(production[count], chr);
		count++;
	}

	void inputFile(string filePath)
	{
		csvManager m;
		m.read(filePath);

		for (int i = 0; i < m.fileContent.size(); i++)
		{
			for (int j = 0; j < m.fileContent[i].size(); j++)
			{
				if (m.fileContent[i][j].length() > 1)
				{
					addProdcution(m.fileContent[i][j]);
				}
			}
		}
	}

	void perform()
	{
		int jm = 0;
		int km = 0;
		int i, choice;
		char c, ch;
		
		// The Input grammar 
		//inputGrammer();

		int kay;
		char done[100];
		char donee[100];
		int ptr = -1;

		// Initializing the calc_first array 
		for (k = 0; k < count; k++) {
			for (kay = 0; kay < 100; kay++) {
				calc_first[k][kay] = '!';
			}
		}
		int point1 = 0, point2, xxx;

		for (k = 0; k < count; k++)
		{
			c = production[k][0];
			point2 = 0;
			xxx = 0;

			// Checking if First of c has 
			// already been calculated 
			for (kay = 0; kay <= ptr; kay++)
				if (c == done[kay])
					xxx = 1;

			if (xxx == 1)
				continue;

			// Function call	 
			findfirst(c, 0, 0);
			ptr += 1;

			// Adding c to the calculated list 
			done[ptr] = c;
			cout << "First(" << c << ") = {";
			calc_first[point1][point2++] = c;

			// Printing the First Sets of the grammar 
			for (i = 0 + jm; i < n; i++) {
				int lark = 0, chk = 0;

				for (lark = 0; lark < point2; lark++) {

					if (first[i] == calc_first[point1][lark])
					{
						chk = 1;
						break;
					}
				}
				if (chk == 0)
				{
					if (i == jm) cout << first[i];
					else cout<<", "<< first[i];
					calc_first[point1][point2++] = first[i];
				}
			}
			cout << "}"<<endl;
			jm = n;
			point1++;
		}
		cout << "-----------------------------------------------" << endl;
		ptr = -1;

		// Initializing the calc_follow array 
		for (k = 0; k < count; k++) {
			for (kay = 0; kay < 100; kay++) {
				calc_follow[k][kay] = '!';
			}
		}
		point1 = 0;
		int land = 0;
		for (e = 0; e < count; e++)
		{
			ck = production[e][0];
			point2 = 0;
			xxx = 0;

			// Checking if Follow of ck 
			// has alredy been calculated 
			for (kay = 0; kay <= ptr; kay++)
				if (ck == donee[kay])
					xxx = 1;

			if (xxx == 1)
				continue;
			land += 1;

			// Function call 
			follow(ck);
			ptr += 1;

			// Adding ck to the calculated list 
			donee[ptr] = ck;
			cout << "Follow(" << ck << ") = {";
			calc_follow[point1][point2++] = ck;

			// Printing the Follow Sets of the grammar 
			for (i = 0 + km; i < m; i++) {
				int lark = 0, chk = 0;
				for (lark = 0; lark < point2; lark++)
				{
					if (f[i] == calc_follow[point1][lark])
					{
						chk = 1;
						break;
					}
				}
				if (chk == 0)
				{
					if (i == km) cout << f[i];
					else cout << ", " << f[i];
					calc_follow[point1][point2++] = f[i];
				}
			}
			cout << "}" << endl;
			km = m;
			point1++;
		}
	}

};

int main()
{
	FirstnFollow ff;
	ff.inputFile("E:\\git\\CC-Course\\CC-Assignment6 - First and Follow\\infile.csv");
	ff.perform();
}