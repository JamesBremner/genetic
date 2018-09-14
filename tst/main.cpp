#include <iostream>
#include <cmath>
#include <time.h>       /* time */

#include "cEvolver.h"
#include "cPopulation.h"

using namespace std;

class cTestEvolver : public cEvolver
{
public:

    /// Construct new bit counter with random chromosome
    cTestEvolver()
        : cEvolver()
    {

    }

    /// Construc new bit counter with specified chromosome
    cTestEvolver( chromo_t c )
        : cEvolver( c )
    {

    }

    int Fitness()
    {
        return 1;
    }

    /// Construct mutated child
    evolver_t Mutate()
    {
        return evolver_t( new cTestEvolver( cTestEvolver() ) );
    }

};

int main()
{
    srand (time(NULL));

    cPopulation thePopulation;
    for( int k = 0; k < 4; k++ )
    {
        evolver_t e( new cTestEvolver );
        e->SelectionProbability( (k+1)  / 10.0 );
        thePopulation.Add( e );
    }
    vector< int > hist( 4 );
    for( int k = 0; k<10000; k++ )
    {
        hist[ thePopulation.Select() ]++;
    }
    bool fOK = true;
    for( int k = 0; k < 4; k++ )
    {
        if( fabs( hist[k] - 1000 * ( k + 1 ) ) > 150 )
            fOK = false;
    }
    if( ! fOK )
    {
        for( int h : hist )
            cout << h << " ";
        cout << "\n";
        throw runtime_error("select_as_parent failed unit test");
    }
    return 0;
}
