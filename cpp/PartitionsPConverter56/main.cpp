#include <iostream>
#include <fstream>
#include <cassert>
using namespace std;

#define L 6
#define INPUTFILE "mod6-10m.txt"
#define OUTPUTFILE "output.csv"
#define INTSIZE 30

//#define DEBUG

/*
	For n = 5/6, each 10 3-bit values uses one unsigned int as bit representation.
		I.e. For any n = 10k, n -> number, such that
		p(n + offset) = (number >> (3 * offset)) & 0x7.
*/

/*
	Database format (n == 5/6):

	CREATE TABLE  `*************`.`p_n_mod5` (
	`n` INT( 12 ) UNSIGNED NOT NULL COMMENT  'independent variable',
	`parity` INT( 30 ) UNSIGNED NOT NULL COMMENT  'PartitionsP(n) mod 5',
	UNIQUE (
	`n`
	)
	) ENGINE = MYISAM COMMENT =  'Data access: p(10k+offset) = p(10k)''s ''offset * 3 + 2~0'' bit.';
*/

int main()
{
	ifstream input(INPUTFILE);
	if (!input.is_open())
	{
		cerr << "Cannot open input file \"" << INPUTFILE << "\"!" << endl;
		system("pause");
		return -1;
	}

	ofstream output(OUTPUTFILE);
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

		if ((3 * count) % INTSIZE == 0)
		{
			output << "'" << (count - INTSIZE / 3) << "','" << temp << "'" << endl;
			temp = 0;
		}

		switch (curr)
		{
			case '1':
				temp |= (1 << ((3 * count) % INTSIZE));
				break;
			case '2':
				temp |= (2 << ((3 * count) % INTSIZE));
				break;
			case '3':
				temp |= (3 << ((3 * count) % INTSIZE));
				break;
			case '4':
				temp |= (4 << ((3 * count) % INTSIZE));
				break;
#if (L == 6)
			case '5':
				temp |= (5 << ((3 * count) % INTSIZE));
				break;
#endif
		}
#endif
	}

#ifndef DEBUG
	output << "'" << (count - count % (INTSIZE / 3)) << "','" << temp << "'" << endl;
#endif

	system("pause");
	return 0;
}