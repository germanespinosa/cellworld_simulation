#include <simulation.h>

using namespace std;

int main(int argc, char **argv)
{
    Simulation_parameters p;
    p.load(argc, argv);
    Simulation simulation(p);
}