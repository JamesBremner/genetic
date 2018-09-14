#pragma once
#include <stdlib.h>     /* srand, rand */
#include <bitset>         // std::bitset
#include <memory>

using namespace std;

class cEvolver;

/// chromsone
typedef bitset<20> chromo_t;

/// evolver
typedef shared_ptr< cEvolver > evolver_t;

/** A pure base class for classes that evolve towards a specialized goal

    The speialized evolver needs to over-ride two methods

    int Fitness()  calculates the relative fitness of the chromosome, according to specialized goal

    evolver_t Mutate()  constructs new mutated child of specialized evolver

 */
class cEvolver
{
public:
    cEvolver()
    {
        for( int k = 0; k<20; k++ )
            myChromo.set( k, rand()%2 );
    }
    cEvolver( const chromo_t& c )
        : myChromo( c )
    {

    }

    /** calculate fitness
        @return relative fitness, a positive integer, more fit is larger

        This must be over-ridden in a specialized evolver
        to calculate the fitness from the chromosone
        according to the goalof the specialized evolver
    */
    virtual int Fitness() = 0;

    /** Construct a mutated child of specialized evolver
        @return shared pointer to mutated child

        This must be over-ridden in a specialized evolver
        to construct a virtual pointer to a newly constructed specialized evolver

        The code should look like this:

    <pre>
    evolver_t Mutate()
    {
        return evolver_t( new MySpecialzedEvolver( MutateChromo() ) );
    }
    </pre>
    */
    virtual evolver_t Mutate() = 0;

    /** mutate chromosome
    @return the new evolver with mutated chromosome

    Each bit has a 0.01 chance of being flipped
    */
    chromo_t MutateChromo() const
    {
        chromo_t child = myChromo;
        for( int k = 0; k < 20; k++ )
        {
            if( ! rand()%100 )
                child[k] = ! child[k];
        }
        return child;
    }
    void SelectionProbability( float prob )
    {
        mySelectionProbability = prob;
    }
    float SelectionProbability() const
    {
        return mySelectionProbability;
    }
    chromo_t Chromo()
    {
        return myChromo;
    }
protected:
    chromo_t myChromo;
private:
    float mySelectionProbability;
};

