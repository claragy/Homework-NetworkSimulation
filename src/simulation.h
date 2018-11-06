#include <tclap/CmdLine.h>

class Network;

/*!
  This is the top-level class, building all the objects and running the simulation, based on user-defined parameters.
  The dynamics is a discrete stochastic heat equation on the graph: each node in the network is initialized 
  with a random value and these values then diffuse across the links.
  At each time-step random diffusion coefficients are drawn for each node.  
 */

class Simulation {

public:
/*!
  The constructor takes a pointer to the network which the simulation will be run over.
 */
    Simulation(Network*);
/*!
  The initialization takes the user arguments and configures the network and the simulation parameters.
 */
    void initialize(int, char**);
/*!
  Runs the entire simulation by calling \ref step() \ref maxtime times.
 */
    void run();

private:
    void step() const; /*!<  One time step. */
    void print(const int&) const; /*!<  Prints out the network values every \ref stepnum steps. */
    Network *_network;
    int maxtime;  /*!<  number of steps until the end of the simulation */
    int stepnum;  /*!<  number of steps between printouts */


};
