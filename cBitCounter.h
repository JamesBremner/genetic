#include "cEvolver.h"

/** An example specialized evolver

    This is an example of how to specialize an evolver for a particular goal,
    in this case setting every bit in the chromosome.
*/
class cBitCounter : public cEvolver
{
public:

    /// Construct new bit counter with random chromosome
    cBitCounter()
    : cEvolver()
    {

    }

    /// Construc new bit counter with specified chromosome
    cBitCounter( chromo_t c )
    : cEvolver( c )
    {

    }

    /// Calculate relative fitness by counting set bits
    int Fitness()
    {
        int count = 0;
        for( int k = 0; k<20; k++ )
            if( myChromo[ k ] )
                count++;
        return count;
    }

    /// Construct mutated child
    evolver_t Mutate()
    {
        return evolver_t( new cBitCounter( MutateChromo() ) );
    }

};


