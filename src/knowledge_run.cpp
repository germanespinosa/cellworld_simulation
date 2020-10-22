#include <knowledge_simulation.h>

using namespace std;

int main(int argc, char **argv)
{
    Knowledge_simulation_parameters p;
    p.load(argc, argv);
    Knowledge_simulation simulation(p);
    simulation.run();
}