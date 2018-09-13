#include <iostream>
#include <bitset>         // std::bitset
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <vector>
#include <algorithm>

using namespace std;

/// specify population size
const int population_size = 8;

/// chromsone
typedef bitset<20> chromo_t;

/// a poulation of chromosones
vector< chromo_t > population;

/// generate random chromosone
chromo_t generate()
{
    chromo_t c;
    for( int k = 0; k<20; k++ )
        c.set( k, rand()%2 );
    return c;
}

/// generate random initial population
void generate_initial_population()
{
    for( int k = 0; k < population_size; k++ )
    {
        population.push_back( generate() );
    }
}

/// calculate fitnesss by counting set bits in chromo
int fitness( chromo_t c  )
{
    int count = 0;
    for( int k = 0; k<20; k++ )
        if( c[ k ] )
            count++;
    return count;
}

/// Fitness proportionate selection, also known as roulette wheel selection
/// https://en.wikipedia.org/wiki/Fitness_proportionate_selection
vector< double > selection_probability()
{
    vector< double > probs;
    double total = 0;
    for( auto& c : population )
    {
        total += fitness( c );
    }
    for( auto& c : population )
    {
        probs.push_back( fitness( c ) / total );
    }
//    for( auto p : probs )
//    {
//        cout << p <<" ";
//    }
//    cout << "\n";
    return probs;
}

/// select from population using roulette wheel probabilities
int select(  const vector< double >& probs )
{
    int r = rand() % 99;
    int limit = probs[0]*100;
    for( int k = 0; k < (int)probs.size(); k++ )
    {
        //cout << r <<" "<< limit << "\n";
        if( r < limit )
        {
            //cout << k << "\n";
            return k;
        }
        limit += probs[k+1] * 100;
    }
    return probs.size() - 1;
}

/// test roulette wheel selection
void select_as_parent_test()
{
    vector< double > probs { 0.1, 0.2, 0.3, 0.4 };
    vector< int > hist( 4 );
    for( int k = 0; k<10000; k++ )
    {
        hist[ select( probs ) ]++;
    }
    bool fOK = true;
    for( int k = 0; k < 4; k++ )
    {
        if( fabs( hist[k] - 10000 * probs[k] ) > 150 )
            fOK = false;
    }
    if( ! fOK )
    {
        for( int h : hist )
            cout << h << " ";
        cout << "\n";
        throw runtime_error("select_as_parent failed unit test");
    }
}


// mutate chromosome
chromo_t mutate( chromo_t& c )
{
    chromo_t child = c;
    for( int k = 0; k < 20; k++ )
    {
        if( rand()%2 )
            child[k] = ! child[k];
    }
    return child;
}


// Display population
void Display()
{
    vector< double > probs = selection_probability();
    int ftotal = 0;
    int maxf   = 0;
    for( int k = 0; k < population_size; k++ )
    {
        int f = fitness( population[k] );
        ftotal += f;
        if( f > maxf )
            maxf = f;
//        cout << k <<" "
//             <<population[k]
//             <<" fitness " << f
//             <<" prob "<<probs[k]
//             << "\n";
    }
    cout << "average fitness " << (double)ftotal/ population_size << " max " << maxf << "\n";

    // stop when perfection reached
    if( maxf == 20 )
    {
        for( int k = 0; k < population_size; k++ )
        {
            int f = fitness( population[k] );
            cout << population[k]
                 <<" fitness " << f
                 << "\n";
        }
        exit( 0 );
    }
}

// simulate breeding
void breed( )
{
    // calculate selection probabilities
    vector< double > probs = selection_probability();

    vector< chromo_t > new_generation;
    for( int k = 0; k < population_size / 2; k++ )
    {
        // parent selection based on fitness
        int p =  select( probs );
        new_generation.push_back( population[ p ] );
        new_generation.push_back( mutate( population[ p ] ) );
        //cout << "breed " << p <<" "<< fitness( population[ p ] ) <<" "<< probs[p] << "\n";
    }

    population = new_generation;

    Display();
}

int main()
{
    /* initialize random seed: */
    srand (time(NULL));

    // test the roulette wheel
    select_as_parent_test();

    // initial population
    generate_initial_population();

    // for generations
    for( int gen = 0; gen < 10000; gen++ )
    {
        breed();
    }

    return 0;
}
