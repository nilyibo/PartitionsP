#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main()
{
	cout << "This program can append the content of the first file "
		<< "to the end of the second file." << endl;
	cout << "Enter the first file name: ";
	string filename1;
	cin >> filename1;
	ifstream input;
	input.open(filename1.c_str());
	vector<string> data;
	cout << "Loading data from " << filename1 << " ... ";
	if (input.is_open())
	{
		string line;
		getline(input, line);
		while (line.length() != 0)
		{
			data.push_back(line);
			getline(input, line);
		}
		cout << "done." <<endl;
		input.close();
	}
	else
	{
		cout << "error opening file " << filename1
			<< ".\nProgram will exit." << endl;
		return 1;
	}

	cout << "Enter the second file name: ";
	string filename2;
	cin >> filename2;
	ofstream output;
	output.open(filename2.c_str(), ios::out | ios::app);
	cout << "Writing to file ... ";
	if (output.is_open())
	{
		for (int i = 0; i < data.size(); ++i)
			output << data[i] << "\n";
		output.close();
		cout << "done." <<endl;
	}
	else
	{
		cout << "error writing to file.\nProgram will exit." << endl;
		return 1;
	}
	cout << "Program executed successfully." << endl;
	return 0;
}
