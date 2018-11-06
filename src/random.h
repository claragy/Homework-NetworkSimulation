#include <random>
#include <vector>
#include <algorithm>

/*!
  This is a random number class based on standard c++-11 generators.
 */

class RandomNumbers {

public:
/*! @name Initializing
  The generator \ref rng is a Mersenne twister *mt19937* engine. 
  A seed *s>0* can be provided, by default it is seeded with a *random_device*.
*/
///@{
    RandomNumbers(unsigned long int s=0);
    void initialize(unsigned long int s=0);
///@}

/*! @name Distributions
  These functions either return a single number 
  or fill a given vector with random numbers distributed 
  according the specified distributions. 
  The additional parameters are the standard parameters of these distributions.
*/
///@{
    void uniform_double(std::vector<double>&, double lower=0, double upper=1);
    double uniform_double(double lower=0, double upper=1);
    void normal(std::vector<double>&, double mean=0, double sd=1);
    double normal(double mean=0, double sd=1);
    void poisson(std::vector<int>&, double mean=1);
    int poisson(double mean=1);
    void uniform_int(std::vector<int>&, int lower=0, int upper=100);
///@}
    void shuffle(std::vector<size_t> &_v) {std::shuffle(_v.begin(), _v.end(), rng);}
     
private:
    std::mt19937 rng;
    long int seed;

};

extern RandomNumbers RNG;
