#include <simulation.h>
#include <cell_world_tools.h>

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
    srand(sp.seed);
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
    Json_vector<Data> journal;
    do {
        show_map();
        journal.emplace_back(model.state.public_state,
                             prey.internal_state(),
                             predator.internal_state());
    } while (model.update());
    show_map();
    journal.emplace_back(model.state.public_state,
                         prey.internal_state(),
                         predator.internal_state());
    model.end_episode();
    cout << journal;
    return 1;
}

Map_symbols ms;

void Simulation::show_map() {
    Screen_map sm (data.map);
    auto &prey_cell = prey.public_state().cell;
    auto &prey_move = prey.internal_state().move;
    auto &prey_prev = data.map[prey_cell.coordinates-prey_move];
    auto &predator_cell = predator.public_state().cell;
    auto &predator_move = predator.internal_state().move;
    auto &predator_prev = data.map[predator_cell.coordinates-predator_move];

    if (model.state.public_state.current_turn){
        auto &prey_visibility = data.visibility[prey_prev];
        auto belief_state = prey.planner.filter.belief_state();

        if (prey_visibility.contains(predator_cell)) {
            sm.add_special_cell(predator_cell, ms.two.front(Blue).back(Yellow));
        } else {
            if (belief_state.contains(predator_cell)) {
                sm.add_special_cell(predator_cell, ms.two.front(Red).back(Blue));
            }else{
                sm.add_special_cell(predator_cell, ms.two.front(Blue));
            }
        }

        sm.add_special_cell(prey.internal_state().option, ms.goal.front(Red));
        sm.add_special_cell(prey_prev,ms.one.front(Red));
        sm.add_group(prey_visibility, ms.clear.back(Yellow));

        sm.add_group(belief_state, ms.clear.back(Blue));

        sm.add_special_cell(prey_cell, ms.get_direction(prey_move));
        cout << sm << endl;

    }
//  else {
//        auto &predator_visibility = data.visibility[predator_prev];
//
//        sm.add_special_cell(predator.internal_state().goal, ms.goal.front(Blue));
//        if (predator_visibility.contains(prey_cell))
//            sm.add_special_cell(prey_cell, ms.one.front(Red).back(Cyan));
//        else
//            sm.add_special_cell(prey_cell, ms.one.front(Red));
//
//        sm.add_special_cell(predator_prev,ms.two.front(Blue));
//        sm.add_group(predator_visibility, ms.clear.back(Cyan));
//
//        sm.add_special_cell(predator_cell, ms.get_direction(predator_move));
//    }
}
