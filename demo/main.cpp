#include <iostream>

#include <time.h>       /* time */
#include <vector>
#include <algorithm>


#include "cEvolver.h"
#include "cPopulation.h"

// replace this with your own specialized evolver
#include "cBitCounter.h"

using namespace std;

/// specify population size
const int population_size = 50;

const bool verbose = false;

int main()
{
    /* initialize random seed: */
    srand (time(NULL));

    // test the roulette wheel
    //select_as_parent_test();

    // initial population of BitCounter evolvers
    cPopulation thePopulation;
    for( int k = 0; k < population_size; k++ )
    {
        // replace this with your own specialized evolver!
        thePopulation.Add( evolver_t( new cBitCounter ));
    }

    thePopulation.Display();

    // for generations
    for( int gen = 0; gen < 100000; gen++ )
    {
        // evolve!
        thePopulation.Breed();

        // display results every 1000 generations
        if( gen % 1000  == 1  )
        {
            cout << gen <<" ";
            thePopulation.Display();
        }

        // check if goal has been achieved
        if( thePopulation.maxfitness() == 20 )
        {
            cout << gen <<" ";
            thePopulation.Display();
            return 0;
        }
    }

    return 0;
}
