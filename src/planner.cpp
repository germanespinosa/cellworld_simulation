#include <planner.h>

using namespace cell_world;
using namespace std;

Planner::Planner(const Planner_parameters &parameters,
                 const Particle_filter_parameters &filter_parameters,
                 const Predator_parameters &predator_parameters,
                 const Static_data &data,
                 const Cell &start,
                 const Cell &goal) :
        parameters(parameters),
        data(data),
        filter(filter_parameters,
               predator_parameters,
               data,
               start,
               goal),
        model(data.cells),
        prey(start,goal),
        predator(predator_parameters, data)
{
    model.add_agent(prey);
    model.add_agent(predator);
    model.start_episode();
}

Move Planner::get_best_move(const Model_public_state &state,
                            double &estimated_reward) {

    auto &current_cell = state.agents_state[0].cell;
    auto &prey_sate = model.state.public_state.agents_state[0];
    auto &prey_cell = prey_sate.cell;

    if (update_state(state) == Finished) return {0,0};

    auto particle_count = filter.create_particles();

    if (particle_count == Not_found) { // there is no predator
        return data.paths.get_move(current_cell,prey.goal);
    }

    Model_public_state mps = state;
    auto &predator_sate = model.state.public_state.agents_state[1];
    auto &predator_cell = predator_sate.cell;

    Cell_group options = data.pois_graph[current_cell];
    vector<double> options_rewards_acum(options.size(),0);
    vector<unsigned int> options_counters(options.size(),0);

    for (unsigned int t = 0; t < parameters.roll_outs; t++){
        // if the planner has particles it uses it
        // otherwise it uses the actual location of the predator
        // because it is visible
        if (particle_count) mps.agents_state[1] = pick_random(filter.particles).public_state;
        model.set_public_state(mps);
        unsigned int option_index = pick_random_index (options);
        Cell option =  options[option_index]; // select a random option
        double &option_reward_acum = options_rewards_acum[option_index];
        unsigned int &option_counter = options_counters[option_index];

        while (true){
            prey.move = data.paths.get_move(prey_cell, option); //get the move from the path
            model.update(); // execute the move
            if (prey_cell == prey.goal ) break; // if the prey wins stop;
            if (prey_cell == option) { // if the prey reaches the current option
                option = data.pois_graph[prey_cell].random_cell(); // it selects a new option
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
    option = options[best_option].coordinates;
    Move best_move = data.paths.get_move(current_cell, options[best_option]);
    filter.trajectory.push_back(best_move);
    return best_move;
}

cell_world::Agent_status_code Planner::update_state(const Model_public_state &state) {
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    bool contact = data.visibility[prey_cell].contains(predator_cell);
    if (contact) filter.record_observation(state);
    if (prey_cell == prey.goal || predator_cell == prey_cell) return Finished;
    return Running;
}

