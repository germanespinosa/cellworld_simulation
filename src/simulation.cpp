#include <simulation.h>
#include <cell_world.h>
#include <map_symbols.h>

using namespace cell_world;
using namespace json_cpp;
using namespace std;

Simulation::Simulation(Simulation_parameters &sp) :
    parameters(sp),
    data (
            sp.world_info,
            sp.path_type),
    model (data.cells),
    prey (sp.prey, sp.planner, sp.particle_filter, sp.predator, data),
    predator (sp.predator, sp.predator_location, data)
{
    model.add_agent(prey);
    if (sp.predator_present) {
        model.add_agent(predator);
    } else {
        model.add_agent(ghost);
    }
}


struct Predator_data {
    Coordinates coordinates;

};

struct Simulation_step : json_cpp::Json_object{
    Simulation_step () = default;
    Simulation_step (Model_public_state &state,
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


struct Simulation_episode : json_cpp::Json_object{
    unsigned int iterations;
    unsigned int result;
    json_cpp::Json_vector<Simulation_step> steps;
    Json_object_members({
        Add_member(iterations);
        Add_member(result);
        Add_member(steps);
    })
};


unsigned int Simulation::run() {
    for (auto seed:parameters.seeds) {
        string output_file = format(parameters.result_file, seed);
        {
            ifstream test_stream(output_file);
            if (test_stream.good()) {
                // the output file exists
                // do not perform the simulation
                continue;
            }
        }
        Simulation_episode episode;
        srand(seed);
        model.start_episode();
        do {
            episode.steps.emplace_back(model.state.public_state,
                                 prey.internal_state(),
                                 predator.internal_state());
            //show_map();
        } while (model.update());
        //show_map();
        episode.steps.emplace_back(model.state.public_state,
                             prey.internal_state(),
                             predator.internal_state());
        episode.result = prey.goal == prey.public_state().cell;
        episode.iterations = prey.public_state().iteration;
        model.end_episode();
        ofstream result_file;
        result_file.open (output_file);
        result_file << episode;
        result_file.close();
    }
    return 1;
}

Map_symbols ms;


vector<Map_symbol> gradient(Map_symbol_color fg, Map_symbol_color bg){
    vector<Map_symbol> g;
    g.emplace_back(9617, fg, bg);
    g.emplace_back(9618, fg, bg);
    g.emplace_back(9619, fg, bg);
    g.emplace_back(9608, fg, bg);
    return g;
}

vector<Coordinates_list> beliefs(Belief_state bs, unsigned int groups){
    unsigned  int max=0;
    for (auto b:bs.hits) max=b>max?b:max;
    unsigned int div = max / groups + 1;
    vector<Coordinates_list> bg (groups);
    for (unsigned int i=0; i<bs.hits.size() ; i++){
        unsigned int index = bs.hits[i]/div;
        bg[index].push_back(bs.particles_coordinates[i]);
    }
    return bg;
}

void Simulation::show_map() {
    // if (model.state.public_state.current_turn){
    //     Screen_map sm (data.map);
    //     auto &prey_cell = prey.public_state().cell;
    //     auto &prey_move = prey.internal_state().move;
    //     auto &prey_prev = data.map[prey_cell.coordinates-prey_move];
    //     auto &predator_cell = model.state.public_state.agents_state[1].cell;
    //     auto &prey_visibility = data.visibility[prey_prev];
    //     auto belief_state_grouped = beliefs(prey.planner.filter.get_belief_state(), 4);
    //     if (prey_visibility.contains(predator_cell)) {
    //         sm.add_special_cell(predator_cell, ms.two.front(Blue).back(Yellow));
    //     }
    //     sm.add_special_cell(prey.internal_state().option, ms.goal.front(Red));
    //     sm.add_special_cell(prey_prev,ms.one.front(Red));
    //     sm.add_group(prey_visibility, ms.clear.back(Yellow));
    //
    //     auto g = gradient(Blue,White);
    //     sm.add_group(belief_state_grouped[0],g[0]);
    //     sm.add_group(belief_state_grouped[1],g[1]);
    //     sm.add_group(belief_state_grouped[2],g[2]);
    //     sm.add_group(belief_state_grouped[3],g[3]);
    //
    //     sm.add_special_cell(prey_cell, ms.get_direction(prey_move));
    //     cout << sm << endl;
    // }
}

std::string Simulation::format(const string &format_string, unsigned int seed) {
    char buff[255];
    snprintf(buff, sizeof(buff), format_string.c_str(), seed);
    std::string buffAsStdStr = buff;
    return buffAsStdStr;
}

