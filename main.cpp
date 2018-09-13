#include <iostream>
#include <bitset>         // std::bitset
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include <algorithm>

using namespace std;

/// specify population size
const int population_size = 50;

const bool verbose = false;

/// chromsone
typedef bitset<20> chromo_t;

/// a poulation of chromosones
vector< chromo_t > population;

/** A class that evolves */
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

    /// calculate fitnesss by counting set bits in chromo
    int Fitness()
    {
        int count = 0;
        for( int k = 0; k<20; k++ )
            if( myChromo[ k ] )
                count++;
        return count;
    }
    /** mutate chromosome
    @return the new evolver with mutated chromosome

    Each bit has a 0.01 chance of being flipped
*/
    cEvolver Mutate()
    {
        chromo_t child = myChromo;
        for( int k = 0; k < 20; k++ )
        {
            if( ! rand()%100 )
                child[k] = ! child[k];
        }
        return cEvolver( child );
    }
    chromo_t Chromo()
    {
        return myChromo;
    }
private:
    chromo_t myChromo;
};

/** A class that contains an evolving population */
class cPopulation
{
public:
    cPopulation( int s )
        : myPopulation( s )
    {

    }
    void Breed()
    {
        // calculate selection probabilities
        SelectionProbability();

        vector< cEvolver > new_generation;
        for( int k = 0; k < population_size / 2; k++ )
        {
            // parent selection based on fitness
            int p =  Select();
            new_generation.push_back( myPopulation[ p ] );
            new_generation.push_back( myPopulation[ p ].Mutate() );

            if( verbose )
                cout << "breed " << p <<" "<< myPopulation[ p ].Fitness() <<" "<< mySelectionProbability[p] << "\n";
        }

        myPopulation = new_generation;

        Display();
    }

    void Display()
    {
        int ftotal = 0;
        int maxf   = 0;
        for( int k = 0; k < myPopulation.size(); k++ )
        {
            int f = myPopulation[ k ].Fitness();
            ftotal += f;
            if( f > maxf )
                maxf = f;
            if( verbose )
            {
                cout << k <<" "
                     <<myPopulation[k].Chromo()
                     <<" fitness " << f
                     <<" prob "<<mySelectionProbability[k]
                     << "\n";
            }
        }
        cout << "average fitness " << (double)ftotal/ population_size << " max " << maxf << "\n";

        // stop when perfection reached
        if( maxf == 20 )
        {
            for( int k = 0; k < population_size; k++ )
            {
                cout << population[k]
                     <<" fitness " << myPopulation[k].Fitness()
                     << "\n";
            }
            exit( 0 );
        }
    }
private:
    vector< cEvolver > myPopulation;
    vector< double > mySelectionProbability;

    /// Fitness proportionate selection, also known as roulette wheel selection
    /// https://en.wikipedia.org/wiki/Fitness_proportionate_selection
    void SelectionProbability()
    {
        mySelectionProbability.clear();
        double total = 0;
        for( auto& c : myPopulation )
        {
            total += c.Fitness();
        }
        for( auto& c : myPopulation )
        {
            mySelectionProbability.push_back( c.Fitness() / total );
        }
    }
    /// select from population using roulette wheel probabilities
    int Select()
    {
        int r = rand() % 99;
        int limit = mySelectionProbability[0]*100;
        for( int k = 0; k < (int)mySelectionProbability.size(); k++ )
        {
            //cout << r <<" "<< limit << "\n";
            if( r < limit )
            {
                //cout << k << "\n";
                return k;
            }
            limit += mySelectionProbability[k+1] * 100;
        }
        return mySelectionProbability.size() - 1;
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

    // initial population
    cPopulation thePopulation( population_size );

    // for generations
    for( int gen = 0; gen < 100000; gen++ )
    {
        thePopulation.Breed();
    }

    return 0;
}
