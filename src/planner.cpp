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
        options_graph(parameters.use_pois ? data.pois_graph : data.world_graph),
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
    if (parameters.tree_mode == "mcts"){
        tree_mode = Search_tree::mcts;
    } else {
        tree_mode = Search_tree::ucb1;
    }
}

Move Planner::get_best_move(const Model_public_state &state,
                            double &estimated_reward) {

    model.set_public_state(state);

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



    Cell_group options = get_valid_options(current_cell,state.agents_state[0].iteration);
    if (options.empty()){
        return {0,0};
    }
    vector<double> options_rewards_acum(options.size(),0);
    vector<unsigned int> options_counters(options.size(),0);

    Search_tree tree (options_graph, data.paths, prey_cell, model.state.public_state.iterations - prey_sate.iteration, tree_mode);

    for (unsigned int t = 0; t < parameters.roll_outs; t++){
        // if the planner has particles it uses it
        // otherwise it uses the actual location of the predator
        // because it is visible
        if (particle_count) mps.agents_state[1] = pick_random(filter.particles).public_state.agents_state[1];
        model.set_public_state(mps);

        // selects an option stochastically with probability proportional
        // to reward and inversely proportional to previous visits
        tree.rewind();
        tree.get_option();
        while (true){
            auto option = tree.current_option();
            prey.move = data.paths.get_move(prey_cell, option); //get the move from the path
            model.update(); // execute the move
            if (prey_cell == prey.goal ) break; // if the prey wins stop;
            if (prey_cell == option) { // if the prey reaches the current option
                if (tree.finished()){ // no valid options left
                    break;
                }
                tree.get_option(); // it selects a new option
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
        tree.record_reward(current_reward);
    }
    Move best_move = data.paths.get_move(current_cell, tree.get_best_option());
    estimated_reward = tree.estimated_reward;
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

cell_world::Cell_group Planner::get_valid_options(const Cell &cell, unsigned int iteration) const {
    unsigned int remaining_steps = model.state.public_state.iterations - iteration;
    Cell_group options = options_graph[cell];
    Cell_group valid_options;
    for (auto &option:options){
        int min_steps_to_goal = data.paths.get_steps(cell,option) + data.paths.get_steps(option,prey.goal);
        if (min_steps_to_goal<remaining_steps) valid_options.add(option);
    }
    return valid_options;
}

void Planner::reset() {
    model.restart_episode();
    filter.reset();
}

