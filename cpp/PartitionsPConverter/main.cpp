#include <iostream>
#include <fstream>
using namespace std;

#define INPUTFILE "mod2-1k.txt"
#define OUTPUTFILE "output.csv"
#define INTSIZE 32

//#define DEBUG

/*
	Each 32 bool values uses one unsigned int as bit representation.
	I.e. For any n = 32k, n -> number, such that p(n + offset) = (number >> offset) & 0x1.
*/

/*
	Database format:

	CREATE TABLE  `*************`.`p_n_mod2` (
	`n` INT( 12 ) UNSIGNED NOT NULL COMMENT  'independent variable',
	`parity` INT( 32 ) UNSIGNED NOT NULL COMMENT  'PartitionsP(n) mod 2',
	UNIQUE (
	`n`
	)
	) ENGINE = MYISAM COMMENT =  'Data access: p(32k+offset) = p(32k)''s ''offset'' bit.';
*/

int main()
{
	ifstream input(INPUTFILE);
	ofstream output(OUTPUTFILE);

	if (!input.is_open())
	{
		cerr << "Cannot open input file \"" << INPUTFILE << "\"!" << endl;
		system("pause");
		return -1;
	}

	if (!output.is_open())
	{
		cerr << "Cannot open output file \"" << OUTPUTFILE << "\"!" << endl;
		system("pause");
		return -1;
	}

#ifdef DEBUG
	const long limit = 10;
#endif

	long count = 0;
	unsigned int temp = 0;
	char curr;

	while (true)
	{
		++count;
		input >> curr;
#ifdef DEBUG
		cout << curr << endl;
		if (count > limit)
			break;
#else
		if (input.eof())
			break;

		if (curr != '0' && curr != '1')
		{
			--count;	// Discard current character
			cerr << "Illegal character detected!" << endl;
			continue;
		}

		if (count % INTSIZE == 0)
		{
			output << "'" << (count - INTSIZE) << "','" << temp << "'" << endl;
			temp = 0;
		}

		if (curr == '1')
			temp |= (1 << (count % INTSIZE));
#endif
	}

	output << "'" << (count - count % INTSIZE) << "','" << temp << "'" << endl;

	system("pause");
	return 0;
}