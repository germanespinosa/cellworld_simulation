#include <planner.h>

using namespace cell_world;
using namespace std;

Planner::Planner(const Planning_parameters &parameters,
                 const Particle_filter &filter,
                 const Graph &world_graph,
                 const Graph &pois_graph,
                 const Graph &visibility,
                 const Paths &paths,
                 const Cell &start_cell,
                 const Cell &goal) :
                 parameters(parameters),
                 filter(filter),
                 pois_graph(pois_graph),
                 paths(paths),
                 model(world_graph.cells),
                 prey(start_cell,goal),
                 predator(world_graph,visibility,paths)
{
    model.add_agent(prey);
    model.add_agent(predator);
    model.start_episode();
}

Move Planner::get_best_move(const Model_public_state &current_state,
                            double &estimated_reward,
                            const vector<Particle> &particles) {
    Model_public_state mps = current_state;
    auto &prey_sate = model.state.public_state.agents_state[0];
    auto &prey_cell = prey_sate.cell;
    auto &predator_sate = model.state.public_state.agents_state[1];
    auto &predator_cell = predator_sate.cell;

    Cell_group options = pois_graph[prey_cell];
    vector<double> options_rewards_acum(options.size(),0);
    vector<unsigned int> options_counters(options.size(),0);

    for (unsigned int t = 0; t < parameters.roll_outs; t++){
        // if the planner has particles it uses it
        // otherwise it uses the actual location of the predator
        if (!particles.empty()) mps.agents_state[1] = pick_random(particles).public_state;
        model.set_public_state(mps);
        unsigned int option_index = pick_random_index (options);
        Cell option =  options[option_index]; // select a random option
        double &option_reward_acum = options_rewards_acum[option_index];
        unsigned int &option_counter = options_counters[option_index];

        while (true){
            prey.move = paths.get_move(prey_cell, option); //get the move from the path
            model.update(); // execute the move
            if (prey_cell == prey.goal ) break; // if the prey wins stop;
            if (prey_cell == option) { // if the prey reaches the current option
                option = pois_graph[prey_cell].random_cell(); // it selects a new option
            }
            model.update(); // predator moves
            if (prey_cell == predator_cell ) break; // the predator wins
            if (model.state.public_state.iterations == predator_sate.iteration) break; // neither agent wins
        }
        double current_reward;
        unsigned int steps = prey_sate.iteration - mps.agents_state[0].iteration;
        if (prey_sate.cell == prey.goal) {
            current_reward = parameters.reward.compute(Success, steps);
        } else {
            current_reward = parameters.reward.compute(Fail, steps);
        }
        option_reward_acum += current_reward;
        option_counter++;
    }
    double best_reward;
    int best_option = Not_found;
    for (unsigned int option_index = 0; option_index < options.size(); option_index++ ){
        double &option_reward_acum = options_rewards_acum[option_index];
        unsigned int &option_counter = options_counters[option_index];
        double current_option_reward = option_reward_acum / option_counter;
        if (current_option_reward > best_reward || best_option == Not_found){
            best_reward = current_option_reward;
            best_option = (int)option_index;
        }
    }
    estimated_reward = best_reward;
    return paths.get_move(mps.agents_state[0].cell, options[best_option]);
}

cell_world::Move Planner::get_best_move(const Model_public_state &current_state,
                                        double &estimated_reward) {
    return get_best_move(current_state, estimated_reward, {});
}

