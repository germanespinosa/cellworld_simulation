#include <simulation.h>

using namespace cell_world;
using namespace json_cpp;
using namespace std;

Simulation::Simulation(Simulation_parameters &sp) :
    parameters(sp),
    data (
            sp.world,
            sp.path_type),
    model (data.cells),
    prey (sp.prey, sp.planner, sp.particle_filter, sp.predator, data),
    predator (sp.predator, data)
{
    model.add_agent(prey);
    model.add_agent(predator);
}


struct Data : json_cpp::Json_object{
    Data () = default;
    Data (Model_public_state &state,
          Poi_prey_state & prey_state,
          Predator_state & predator_state) :
            iteration(state.agents_state[0].iteration),
            prey(state.agents_state[0].cell.coordinates),
            predator(state.agents_state[1].cell.coordinates),
            prey_state(prey_state),
            predator_state(predator_state){}
    unsigned int iteration;
    Coordinates prey;
    Coordinates predator;
    Poi_prey_state prey_state;
    Predator_state predator_state;
    Json_object_members({
                            Add_member(iteration);
                            Add_member(prey);
                            Add_member(predator);
                            Add_member(prey_state);
                            Add_member(predator_state);
                        })
};


unsigned int Simulation::run() {
    model.start_episode();
    cout << "prey: " << model.state.public_state.agents_state[0].cell.coordinates << endl;
    cout << "predator: " << model.state.public_state.agents_state[1].cell.coordinates << endl;
    do {
        if (!model.state.public_state.current_turn) {
            Data data {model.state.public_state,
                       prey.internal_state(),
                       predator.internal_state()};
            cout << data << endl;
        }
    } while (model.update());
    cout << "prey: " << model.state.public_state.agents_state[0].cell.coordinates << endl;
    cout << "predator: " << model.state.public_state.agents_state[1].cell.coordinates << endl;
    model.end_episode();
    return 1;
}
