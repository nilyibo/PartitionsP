#include <iostream>
#include <fstream>
using namespace std;

#define L 4
#define INPUTFILE "mod4-10m.txt"
#define OUTPUTFILE "output.csv"
#define INTSIZE 32

//#define DEBUG

/*
	For n = 3/4, each 16 2-bit values uses one unsigned int as bit representation.
		I.e. For any n = 16k, n -> number, such that
		p(n + offset) = (number >> (2 * offset)) & 0x3.
*/

/*
	Database format (n == 3/4):

	CREATE TABLE  `*************`.`p_n_mod3` (
	`n` INT( 12 ) UNSIGNED NOT NULL COMMENT  'independent variable',
	`parity` INT( 32 ) UNSIGNED NOT NULL COMMENT  'PartitionsP(n) mod 3',
	UNIQUE (
	`n`
	)
	) ENGINE = MYISAM COMMENT =  'Data access: p(16k+offset) = p(16k)''s ''offset * 2 + 1~0'' bit.';
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
	const long limit = 20;
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

		if (curr < '0' || curr > '0' + L)
		{
			--count;	// Discard current character
			cerr << "Illegal character detected!" << endl;
			continue;
		}

		if ((2 * count) % INTSIZE == 0)
		{
			output << "'" << (count - INTSIZE / 2) << "','" << temp << "'" << endl;
			temp = 0;
		}

		switch (curr)
		{
			case '1':
				temp |= (1 << (2 * (count % INTSIZE)));
				break;
			case '2':
				temp |= (2 << (2 * (count % INTSIZE)));
				break;
#if (L == 4)
			case '3':
				temp |= (3 << (2 * (count % INTSIZE)));
				break;
#endif
		}
#endif
	}

#ifndef DEBUG
	output << "'" << (count - count % (INTSIZE / 2)) << "','" << temp << "'" << endl;
#endif

	system("pause");
	return 0;
}