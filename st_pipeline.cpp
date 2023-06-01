#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstdint> // Include for uintptr_t
#include <cstdlib>
#include <ctime>
#include <time.h>

#include "st_activeObject.hpp"

using namespace std;

// global vars:
PActiveObject p_ao1 = nullptr;
PActiveObject p_ao2 = nullptr;
PActiveObject p_ao3 = nullptr;
PActiveObject p_ao4 = nullptr;
bool ao1_finished = false;
bool ao2_finished = false;
bool ao3_finished = false;
bool ao4_finished = false;

// part A:
bool isPrime(unsigned int number)
{
    cout << number << endl;
    if (number == 0 || number == 1)
    {
        cout << "false" << endl;
        return false;
    }
    if (number % 2 == 0)
    {
        printf("false\n");
        return false;
    }

    for (int i = 3; i * i <= number; i += 2)
    {
        if (number % i == 0)
        {
            cout << "false" << endl;
            return false;
        }
    }
    cout << "true" << endl;
    return true;
}

void *isPrimePiped2(unsigned int number)
{
    if (number == -1)
    {
        getQueue(p_ao3)->enqueue(reinterpret_cast<void *>(-1));
        stop(p_ao2);
        ao2_finished = true;
        return nullptr;
    }

    isPrime(number);
    number += 11;
    getQueue(p_ao3)->enqueue(reinterpret_cast<void *>(number));
    return nullptr;
}

void *isPrimePiped3(unsigned int number)
{
    if (number == -1)
    {
        getQueue(p_ao4)->enqueue(reinterpret_cast<void *>(-1));
        stop(p_ao3);
        ao3_finished = true;
        return nullptr;
    }

    isPrime(number);
    number -= 13;
    getQueue(p_ao4)->enqueue(reinterpret_cast<void *>(number));
    return nullptr;
}

void *isPrimePiped4(unsigned int number)
{
    if (number == -1)
    {
        stop(p_ao4);
        ao4_finished = true;
        return nullptr;
    }

    cout << number << endl;
    number += 2;
    cout << number << endl;
    return nullptr;
}

void generateRandomNumbers(unsigned int NPlusSeed[2])
{
    srand(NPlusSeed[1]);

    for (unsigned int i = 0; i < NPlusSeed[0]; i++)
    {
        int randomNum = std::rand() % 900000 + 100000;
        getQueue(p_ao2)->enqueue(reinterpret_cast<void *>(randomNum));
        usleep(1000);
    }

    // sending -1 for shutting down:
    getQueue(p_ao2)->enqueue(reinterpret_cast<void *>(-1));
    stop(p_ao1);
    ao1_finished = true;
}

int main(int argc, char *argv[])
{
    p_ao1 = CreateActiveObject(reinterpret_cast<func>(&generateRandomNumbers));
    p_ao2 = CreateActiveObject(reinterpret_cast<func>(&isPrimePiped2));
    p_ao3 = CreateActiveObject(reinterpret_cast<func>(&isPrimePiped3));
    p_ao4 = CreateActiveObject(reinterpret_cast<func>(&isPrimePiped4));
    while (p_ao1 == nullptr && p_ao2 == nullptr && p_ao3 == nullptr && p_ao4 == nullptr)
    {
        usleep(1000);
    }

    // making the random numbers:

    // if the random seed is missing, genarate a new seed:
    if (argc == 2)
    {
        // setting the seed:
        unsigned int currentSeed = static_cast<unsigned int>(std::time(NULL));
        // setting the array:
        unsigned int NPlusSeed[2] = {static_cast<unsigned int>(strtoul(argv[1], nullptr, 10)), currentSeed};

        // enqueing new seed and number:
        PST_Queue queue = getQueue(p_ao1);
        queue->enqueue(NPlusSeed);

    }
    else if (argc == 3)
    {
        // setting the array:
        unsigned int NPlusSeed[2] = {static_cast<unsigned int>(strtoul(argv[1], nullptr, 10)), static_cast<unsigned int>(strtoul(argv[2], nullptr, 10))};

        // enqueing new seed and number:
        PST_Queue queue = getQueue(p_ao1);
        queue->enqueue(NPlusSeed);
    }
    else
    {
        cout << "error" << endl;
        stop(p_ao1);
        stop(p_ao2);
        stop(p_ao3);
        stop(p_ao4);
        return 1;
    }

    while (!(ao1_finished && ao2_finished && ao3_finished && ao4_finished))
    {
        //waiting for the ao's to finish
    }

    return 0;
}