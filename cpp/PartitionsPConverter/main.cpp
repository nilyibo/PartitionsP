#include <iostream>
#include <fstream>
using namespace std;

#define INPUTFILE "mod2-1m.txt"
#define OUTPUTFILE "output.sql"
#define TABLENAME "mod2_1m"

#define DEBUG

int main()
{
	ifstream input(INPUTFILE);
	ofstream output(OUTPUTFILE);

	if (!input.is_open())
	{
		cerr << "Cannot open input file \"" << INPUTFILE << "\"!" << endl;
		return -1;
	}

	if (!output.is_open())
	{
		cerr << "Cannot open output file \"" << OUTPUTFILE << "\"!" << endl;
		return -1;
	}

#ifdef DEBUG
	const long limit = 10;
#endif

	output << "USE `yiboguo2_partitionsp`;\n"
		<< "DROP TABLE IF EXIST `" << TABLENAME << "`;\n"
		<< "CREATE TABLE `" << TABLENAME << "` (\n"
		<< "`index` INT(12) NOT NULL PRIMARY KEY,\n"
		<< "`parity` BOOL NOT NULL\n"
		<< ") ENGINE = MYISAM;\n"
		<< endl;

	output << "INSERT INTO  `yiboguo2_partitionsp`.`" << TABLENAME << "`"
		<< "(`index`, `parity`) VALUES"
		<< endl;

	long count = 0;

	while (!input.eof())
	{
		++count;
		char curr;
		input >> curr;
#ifdef DEBUG
		cout << curr << endl;
		if (count > limit)
			break;
#else
		output << "('" << count << "','" << curr << "')," << endl;
#endif
	}

	output << ";" << endl;

	system("pause");
	return 0;
}