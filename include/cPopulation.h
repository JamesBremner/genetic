#include <vector>

/** A class that contains an evolving population */
class cPopulation
{
public:

    void Add( evolver_t e )
    {
        myPopulation.push_back( e );
    }

        /// select from population using roulette wheel probabilities
    int Select()
    {
        int r = rand() % 99;
        int limit = myPopulation[0]->SelectionProbability() * 100;
        for( int k = 1; k < (int)myPopulation.size(); k++ )
        {
            //cout << r <<" "<< limit << "\n";
            if( r < limit )
            {
                //cout << k << "\n";
                return k-1;
            }
            limit += myPopulation[k]->SelectionProbability() * 100;
        }
        return myPopulation.size() - 1;
    }

    void Breed()
    {
        // calculate selection probabilities
        SelectionProbability();

        vector< evolver_t > new_generation;
        for( int k = 0; k < (int)myPopulation.size() / 2; k++ )
        {
            // parent selection based on fitness
            int p =  Select();
            new_generation.push_back( myPopulation[ p ] );
            new_generation.push_back( myPopulation[ p ]->Mutate() );

//            if( verbose )
//                cout << "breed " << p <<" "<< myPopulation[ p ]->Fitness()
//                     <<" "<< myPopulation[ p ]->SelectionProbability() << "\n";
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
//            if( verbose )
//            {
//                cout << k <<" "
//                     <<myPopulation[k]->Chromo()
//                     <<" fitness " << f
//                     <<" prob "<<myPopulation[k]->SelectionProbability()
//                     << "\n";
//            }
        }
        cout << "average fitness " << (double)ftotal/ myPopulation.size() << " max " << maxf << "\n";

        // stop when perfection reached
        if( maxf == 20 )
        {
            for( int k = 0; k < (int)myPopulation.size(); k++ )
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

};

