# genetic


A genetic algorithm in C++. The STL bitset is used to hold the bits of a chromosome. Population size is 50 chromosomes and each chromosome is 20 bits long. The mutation operation randomly flips each bit with a probability of 0.01. A roulette proportional selection algorithm is used. New individuals are created by randomly filling each bit with a 0 or 1.

The evolver class can be specialized for any goal that can be calculated from a chomosone of 20 bits.  An example is provided which evolves towards the goal of all 20 bits set.
