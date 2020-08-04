#include <poi_prey.h>

using namespace cell_world;
using namespace std;

Poi_prey::Poi_prey(const Cell_group &cells,
                   const Graph &world_graph,
                   const Cell_group &pois,
                   const Graph &pois_graph,
                   const Graph &visibility,
                   const Paths &paths,
                   const Cell &start_cell,
                   const Cell &goal,
                   const Reward &reward):
                   cells(cells),
                   pois(pois),
                   pois_graph(pois_graph),
                   world_graph(world_graph),
                   visibility(visibility),
                   paths(paths),
                   start_cell(start_cell),
                   goal(goal),
                   reward(reward),
                   model(cells),
                   prey(start_cell, goal),
                   predator(world_graph, visibility, paths),
                   particle_filter(cells,world_graph,visibility,paths, start_cell, goal){ }

const Cell &Poi_prey::start_episode() {
    return start_cell;
}

Move Poi_prey::get_move(const Model_public_state &state) {
    bool contact = process_state (state);
    return plan(contact, state);
}

Agent_status_code Poi_prey::update_state(const Model_public_state &state) {
    if (!process_state (state)) particle_filter.trajectory.push_back(move);
    return cell_world::Finished;
}

bool Poi_prey::process_state(const Model_public_state &state) {
    // if predator is visible records the
    // observation and returns true
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    bool contact = visibility[prey_cell].contains(predator_cell);
    if (contact) particle_filter.record_observation(state);
    return contact;
}

Move Poi_prey::plan(bool use_full_state, const Model_public_state &current_state) {
    Model_public_state mps = current_state;
    auto &prey_sate = model.state.public_state.agents_state[0];
    auto &prey_cell = prey_sate.cell;
    auto &predator_sate = model.state.public_state.agents_state[1];
    auto &predator_cell = predator_sate.cell;

    if (!use_full_state) particle_filter.create_particles(500,5000);

    Cell_group options = pois_graph[prey_cell];
    vector<double> options_rewards_acum(options.size(),0);
    vector<unsigned int> options_counters(options.size(),0);

   for (unsigned int t = 0; t < 1000; t++){
        // if predator location is unknown, replace the predator location
        // with a particle from the filter
        if (!use_full_state) mps.agents_state[1] = particle_filter.get_particle();
        model.set_public_state(mps);
        unsigned int option_index = pick_random_index (options);
        Cell option =  options[option_index]; // select a random option
        double &option_reward_acum = options_rewards_acum[option_index];
        unsigned int &option_counter = options_counters[option_index];

        while (true){
            prey.move = paths.get_move(prey_cell, option); //get the move from the path
            model.update(); // execute the move
            if (prey_cell == goal ) break; // if the prey wins stop;
            if (prey_cell == option) { // if the prey reaches the current option
                option = pois_graph[prey_cell].random_cell(); // it selects a new option
            }
            model.update(); // predator moves
            if (prey_cell == predator_cell ) break; // the predator wins
            if (model.state.public_state.iterations == predator_sate.iteration) break; // neither agent wins
        }
        double current_reward;
        unsigned int steps = prey_sate.iteration - mps.agents_state[0].iteration;
        if (prey_sate.cell == goal) {
            current_reward = reward.compute(Success, steps);
        } else {
            current_reward = reward.compute(Fail, steps);
        }
       option_reward_acum += current_reward;
        option_counter ++;
    }
    double best_reward;
    int best_option = Not_found;
    for (unsigned int option_index = 0; option_index < options.size(); option_index++ ){
        double &option_reward_acum = options_rewards_acum[option_index];
        unsigned int &option_counter = options_counters[option_index];
        double current_option_reward = option_reward_acum / option_counter;
        if (current_option_reward > best_reward || best_option == Not_found){
            best_reward = current_option_reward;
            best_option = option_index;
        }
    }
    return paths.get_move(mps.agents_state[0].cell, options[best_option]);
}
