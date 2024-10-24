#include <knowledge_simulation.h>
#include <params_cpp.h>

using namespace std;

int main(int argc, char **argv)
{
    params_cpp::Parser p(argc, argv);
    Knowledge_simulation_parameters parameters;
    parameters.
    Knowledge_simulation simulation(parameters);
    simulation.run();
}