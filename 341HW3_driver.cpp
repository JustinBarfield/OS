#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "341_Semaphore.h"

using namespace std;

#define NUM_PEEPS 10

Semaphore sem(1);       // Semaphore to control concurrent access to the club
vector<int> insideClub; // Vector to keep track of threads inside the club

// Function that the threads must execute inside
void Club(int id)
{
    sem.acquire(); // Wait to enter the club
    cout << "Thread " << id << " entered the club." << endl;
    insideClub.push_back(id); // Add thread ID to list of threads inside club

    // Simulate partying
    sleep(rand() % 5 + 5);

    cout << "Thread " << id << " left the club." << endl;
    insideClub.erase(std::remove(insideClub.begin(), insideClub.end(), id), insideClub.end()); // Remove thread ID from list of threads inside club
    sem.release();                                                                             // Allow another thread to enter the club
}

int main(int argc, char **argv)
{
    srand(time(NULL));

    int totalThreads = 0;
    int concurrentThreads = 0;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i)
    {
        if (string(argv[i]) == "-t" && i + 1 < argc)
        {
            totalThreads = atoi(argv[i + 1]);
        }
        else if (string(argv[i]) == "-c" && i + 1 < argc)
        {
            concurrentThreads = atoi(argv[i + 1]);
        }
    }

    // Error handling for invalid arguments
    if (totalThreads <= 0 || concurrentThreads <= 0 || concurrentThreads > totalThreads)
    {
        cout << "Invalid arguments. Usage: ./341HW3 -t <num> -c <num>" << endl;
        return 1;
    }

    // Have each person (thread) go into the club and party hard
    vector<thread> threads;
    for (int i = 0; i < totalThreads; i++)
    {
        threads.emplace_back(Club, i + 1);
    }

    // Wait for all threads to finish
    for (auto &thread : threads)
    {
        thread.join();
    }

    return 0;
}
