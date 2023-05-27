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
PActiveObject p_ao1;
PActiveObject p_ao2;
PActiveObject p_ao3;
PActiveObject p_ao4;

// part A:
bool isPrime(unsigned int number)
{
    printf("in isPrime\n");

    cout << number << endl;
    if (number == 0 || number == 1)
    {
        cout << "false" << endl;
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
    isPrime(number);
    number += 11;
    getQueue(p_ao3)->enqueue(reinterpret_cast<void *>(number));
    return nullptr;
}

void *isPrimePiped3(unsigned int number)
{
    isPrime(number);
    number -= 13;
    getQueue(p_ao4)->enqueue(reinterpret_cast<void *>(number));
    return nullptr;
}

void *isPrimePiped4(unsigned int number)
{
    isPrime(number);
    number += 2;
    cout << number << endl;
    return nullptr;
}

void generateRandomNumbers(unsigned int NPlusSeed[2])
{
    printf("received %u , %u\n", NPlusSeed[0], NPlusSeed[1]);
    srand(NPlusSeed[1]);

    for (unsigned int i = 0; i < NPlusSeed[0]; i++)
    {
        int randomNum = std::rand() % 900000 + 100000;
        // std::cout << "Random number " << i+1 << ": " << randomNum << std::endl;
        getQueue(p_ao2)->enqueue(reinterpret_cast<void *>(randomNum));
        sleep(1);
    }
}

int main(int argc, char *argv[])
{
    printf("in main\n");
    printf("sending func: %p\n", &generateRandomNumbers);

    p_ao1 = CreateActiveObject(reinterpret_cast<func>(&generateRandomNumbers));
    p_ao2 = CreateActiveObject(reinterpret_cast<func>(&isPrimePiped2));
    p_ao3 = CreateActiveObject(reinterpret_cast<func>(&isPrimePiped3));
    p_ao4 = CreateActiveObject(reinterpret_cast<func>(&isPrimePiped4));

    // making the random numbers:

    // if the random seed is missing, genarate a new seed:
    if (argc == 2)
    {
        printf("in argv = 2\n");

        // setting the seed:
        unsigned int currentSeed = static_cast<unsigned int>(std::time(NULL));
        // setting the array:
        unsigned int NPlusSeed[2] = {static_cast<unsigned int>(strtoul(argv[1], nullptr, 10)), currentSeed};

        printf("enquing %u , %u\n", NPlusSeed[0], NPlusSeed[1]);
        // enqueing new seed and number:
        PST_Queue queue = getQueue(p_ao1);
        queue->enqueue(NPlusSeed);


        void* ptr = queue->p_queue->front();
    unsigned int* value2 =reinterpret_cast<unsigned int*>(ptr);
    
    cout << "value enqued from argc ==2 : "<< *(value2) << endl;
    }
    else if (argc == 3)
    {
        printf("in argv = 3\n");

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

    stop(p_ao1);
    stop(p_ao2);
    stop(p_ao3);
    stop(p_ao4);

    return 0;
}