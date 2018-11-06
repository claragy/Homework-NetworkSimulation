#include "random.h"
#include "simulation.h"
#include "network.h"
#include <iostream>

Simulation::Simulation(Network *_net) : _network(_net), stepnum(10) {}

void Simulation::initialize(int argc, char **argv) {
    TCLAP::CmdLine cmd("Network simulation");
    TCLAP::ValueArg<int> argNode("n", "nodes", "Number of nodes", false, 0, "int");
    cmd.add(argNode);
    TCLAP::ValueArg<long> argSeed("S", "seed", "Random seed", false, 0, "long");
    cmd.add(argSeed);
    TCLAP::ValueArg<double> argDegree("d", "degree", "Mean node degree", false, 4, "double");
    cmd.add(argDegree);
    TCLAP::ValueArg<int> argTime("t", "time", "Number of steps to simulate", false, 10, "int");
    cmd.add(argTime);

    cmd.parse(argc, argv);

    RNG.initialize(argSeed.getValue());
    int nodes(argNode.getValue());
    if (nodes < 1) nodes = RNG.poisson(50);
    _network->resize(nodes);
    double degree(argDegree.getValue());
    if (degree < 1) degree = RNG.uniform_double(1, std::sqrt(nodes));
    size_t nlink = _network->random_connect(degree);
    std::cout << _network->size() << " nodes, " << nlink << " links\n";
    maxtime = argTime.getValue();
}


void Simulation::run() {
    for (int time=0; time<maxtime; time++) {
        if ((time % stepnum) == 0) print(time);
        step();
    }
    print(maxtime);
}

void Simulation::print(const int &time) const {
    std::cout << time << ": ";
    std::vector<double> vals = _network->sorted_values();
    for (auto I : vals) std::cout << I << " ";
    std::cout << std::endl;
}

void Simulation::step() const {
    std::vector<double> next_values(_network->size());
    std::vector<double> epsilon(_network->size());
    RNG.uniform_double(epsilon);
    for (size_t node=0; node<next_values.size(); node++) {
        double cumul_neighbs = 0;
        for (auto I : _network->neighbors(node)) cumul_neighbs += _network->value(I);
        double ddeg(_network->degree(node)); 
        if (ddeg > .5)
            next_values[node] = epsilon[node]*cumul_neighbs/ddeg+(1-epsilon[node])*_network->value(node);
    }
    _network->set_values(next_values);
}
