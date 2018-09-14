#include <iostream>
#include <bitset>         // std::bitset
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

/// specify population size
const int population_size = 50;

const bool verbose = false;

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

/** A class that contains an evolving population */
class cPopulation
{
public:

    void Add( evolver_t e )
    {
        myPopulation.push_back( e );
    }
    void Breed()
    {
        // calculate selection probabilities
        SelectionProbability();

        vector< evolver_t > new_generation;
        for( int k = 0; k < population_size / 2; k++ )
        {
            // parent selection based on fitness
            int p =  Select();
            new_generation.push_back( myPopulation[ p ] );
            new_generation.push_back( myPopulation[ p ]->Mutate() );

            if( verbose )
                cout << "breed " << p <<" "<< myPopulation[ p ]->Fitness()
                     <<" "<< myPopulation[ p ]->SelectionProbability() << "\n";
        }

        myPopulation = new_generation;
    }

    void Display()
    {
        int ftotal = 0;
        int maxf   = 0;
        for( int k = 0; k < (int)myPopulation.size(); k++ )
        {
            int f = myPopulation[ k ]->Fitness();
            ftotal += f;
            if( f > maxf )
                maxf = f;
            if( verbose )
            {
                cout << k <<" "
                     <<myPopulation[k]->Chromo()
                     <<" fitness " << f
                     <<" prob "<<myPopulation[k]->SelectionProbability()
                     << "\n";
            }
        }
        cout << "average fitness " << (double)ftotal/ population_size << " max " << maxf << "\n";

        // stop when perfection reached
        if( maxf == 20 )
        {
            for( int k = 0; k < population_size; k++ )
            {
                cout << myPopulation[k]->Chromo()
                     <<" fitness " << myPopulation[k]->Fitness()
                     << "\n";
            }
            exit( 0 );
        }
    }

    int maxfitness()
    {
        int m = 0;
        for( auto c : myPopulation )
        {
            if( c->Fitness() > m )
                m = c->Fitness();
        }
        return m;
    }
private:
    vector< evolver_t > myPopulation;

    /// Fitness proportionate selection, also known as roulette wheel selection
    /// https://en.wikipedia.org/wiki/Fitness_proportionate_selection
    void SelectionProbability()
    {
        // sum of all population fitness
        double total = 0;
        for( auto c : myPopulation )
        {
            total += c->Fitness();
        }

        /* selection probability for an evolver is
            evolver's fitness divided by sum of all population fitness
        */
        for( auto c : myPopulation )
        {
            c->SelectionProbability( c->Fitness() / total );
        }
    }
    /// select from population using roulette wheel probabilities
    int Select()
    {
        int r = rand() % 99;
        int limit = myPopulation[0]->SelectionProbability() * 100;
        for( int k = 0; k < population_size; k++ )
        {
            //cout << r <<" "<< limit << "\n";
            if( r < limit )
            {
                //cout << k << "\n";
                return k;
            }
            limit += myPopulation[k]->SelectionProbability() * 100;
        }
        return population_size - 1;
    }
};

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
