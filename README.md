# genetic


A simple genetic algorithm using C++. Goal is to evolve a string of all ones. The STL bitset is used to hold the bits of a chromosome. Population size is 50 chromosomes and each chromosome has 20 bits long. The mutation operation randomly flips each bit with a probability of 0.01. A roulette proportional selection algorithm is used. New individuals are created by randomly filling each bit with a 0 or 1.
