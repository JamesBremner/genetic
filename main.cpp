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


/// test roulette wheel selection
//void select_as_parent_test()
//{
//    vector< double > probs { 0.1, 0.2, 0.3, 0.4 };
//    vector< int > hist( 4 );
//    for( int k = 0; k<10000; k++ )
//    {
//        hist[ select( probs ) ]++;
//    }
//    bool fOK = true;
//    for( int k = 0; k < 4; k++ )
//    {
//        if( fabs( hist[k] - 10000 * probs[k] ) > 150 )
//            fOK = false;
//    }
//    if( ! fOK )
//    {
//        for( int h : hist )
//            cout << h << " ";
//        cout << "\n";
//        throw runtime_error("select_as_parent failed unit test");
//    }
//}






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
