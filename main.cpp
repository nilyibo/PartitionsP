#include <iostream>
#include <math.h>
#include <fstream>
#include <string>
#include <omp.h> // Open MP
#include <time.h> // clock() - CPU time
#include <signal.h> // ctrl-c handler
using namespace std;

#define LIMIT 1500000000 // 1.5b
#define OMPLIMIT 50
#define outFile "mod2-1.5b.txt"

const int NUM_THREAD = 15;
bool *data = NULL;
long currCount;
time_t initialTime;
bool goAhead;

void partitionsP2()
{
    long loBound =  (long)ceil((1 - sqrt(1 + 24 * (double)currCount)) / 6);
    long hiBound = (long)floor((1 + sqrt(1 + 24 * (double)currCount)) / 6);
    long diff = hiBound - loBound + 1;
    if (diff < OMPLIMIT)
        for (long k = loBound; k <= hiBound; ++k)
        {
            if (k == 0)
                continue;
            long index = currCount - k * (3 * k - 1) / 2;
            if (data[index]) // Add odd means invert bool
                data[currCount] = !data[currCount];
        }
    else
    {
        // Divide the diff item into NUM_THREAD groups as evenly as possible
        long eachSize = diff / NUM_THREAD;
        long remainder = diff % NUM_THREAD;
        long *startIndex = new long[NUM_THREAD + 1];
        startIndex[0] = loBound;
        for (int i = 1; i < NUM_THREAD + 1; ++i)
            startIndex[i] = startIndex[i - 1] + eachSize
                            + ((i <= remainder) ? 1 : 0);

        // Partial sum for each thread
        bool *threadParity = new bool[NUM_THREAD];
        #pragma omp parallel for
        for (int i = 0; i < NUM_THREAD; ++i)
        {
            threadParity[i] = false;
            for (long k = startIndex[i]; k <= startIndex[i + 1] - 1; ++k)
            {
                if (k == 0)
                    continue;
                long index = currCount - k * (3 * k - 1) / 2;
                if (data[index])
                    threadParity[i] = !threadParity[i];
            }
        }
        // Sum all thread partial result
        for (int i = 0; i < NUM_THREAD; ++i)
            if (threadParity[i])
                data[currCount] = !data[currCount];
        delete[] startIndex;
        delete[] threadParity;
    }
}

void writeToFile(long min, long max)
{
    ofstream output; // output file to write to
    output.open(outFile);
    if (output.is_open())
    {
        cout << "Start writing to file \"" << outFile << "\" ..." << endl;

        clock_t t = clock();
        output << "******** p(n) mod 2 from n = " << min << " to "
               << max << " ********" << endl;
        output << "======== START ========\n" << endl;
        for (long i = min; i <= max; ++i)
        {
            output << data[i];
            if (i % 1000 == 0) // // 1000 chars per line
                output << "\n";
        }
        output << "\n\n======== END ========" << endl;

        t = clock() - t;
        cout << "Finished writing to file." << endl;
        cout << "Time elapsed: "
             << ((double)t / CLOCKS_PER_SEC) << "s." << endl;
    }
    else
        cout << "Error writing to file.\n"
             << "Failed to open " << outFile << "." << endl;
}

void checkProgressHandler(int s)
{
    cerr << "Progress:\tn = " << currCount
         << ".\nTime elapsed:\t" << difftime(time(NULL), initialTime)
         << "s." << endl;
}

void ctrlCHandler(int s)
{
    cout << "\nCaught signal ctrl-c. Writing data to disk ..." << endl;
    goAhead = false;
}

void mod2Wrapper()
{
    // Open MP Settings
    omp_set_num_threads(NUM_THREAD);
    cout << "Using " << NUM_THREAD << " threads." << endl;

    // Set up signal handler
    signal(SIGINT, &ctrlCHandler);
    signal(SIGTTOU, &checkProgressHandler);
    long initialCount = currCount - 1;

    cout << "\nStart calculating p(n) mod 2 from n = "
         << currCount << " to " << LIMIT << "." << endl;

    initialTime = time(NULL);
    clock_t t1 = clock();
    if (currCount == 1) // the formula does not apply for p(1) = 1
        data[currCount++] = 1;
    while (currCount <= LIMIT)
    {
        data[currCount] = false; // Start from the 2nd
        partitionsP2();
        // Report percentage when every 5% is done
        long remainder = (currCount - initialCount)
                        % ((LIMIT - initialCount) / 20);
        if (remainder == 0)
        {
            long result = (currCount - initialCount)
                        / ((LIMIT - initialCount) / 100);
            cout << result << "% done." << endl;
        }
        ++currCount;
        // ctrl-c break
        if (!goAhead)
            break;
    }
    double seconds = difftime(time(NULL), initialTime);
    t1 = clock() - t1;

    cout << "Finished calculating p(n) up to " << (currCount - 1) << "." << endl;
    cout << "CPU time used: " << ((double)t1 / CLOCKS_PER_SEC) << "s" << endl;
    cout << "Time elapsed: " << seconds << "s." << endl;

    writeToFile(initialCount + 1, currCount - 1);
}

int main(int argc, char* argv[])
{
    // data setup
    data = new bool[LIMIT + 1];
    data[0] = true; // by convention, p(0) = 1
    currCount = 1;
    goAhead = true;

    // Check existing data
    cout << "Trying to load data from \"existing.txt\" ... ";
    ifstream input;
    input.open("existing.txt");
    if (input.is_open())
    {
        string line;
        while (line[0] != '1') // Wait till 1st '1' (p[0] = 1)
            getline(input, line);
        // Loop till an empty line terminates
        while (line.length() != 0)
        {
            for (long n = 0; n < (long)line.length(); ++n)
                data[currCount + n] = (line[n] == '1');
            currCount += line.length();
            getline(input, line);
        }
        cout << "done."  << endl;
        input.close();
    }
    else
    {
        cout << " failed (file does not exist).\n"
             << "Start from the beginning ..." << endl;
    }
    cout << endl;

    // Start calculation
    mod2Wrapper();
}
