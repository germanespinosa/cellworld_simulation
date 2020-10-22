#include <knowledge_prey.h>

using namespace cell_world;
using namespace std;

Agent_status_code Knowledge_prey::update_state(const cell_world::Model_public_state &state) {
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    if (state.agents_state[0].cell == goal){
        return Agent_status_code::Finished;
    } else {
        const Cell &predator_cell = state.agents_state[1].cell;
        if (prey_cell == predator_cell || state.iterations == state.agents_state[0].iteration) {
            return Agent_status_code::Finished;
        };
    }
    bool contact = data.visibility[prey_cell].contains(predator_cell);
    if (contact) filter.record_observation(state);
    return Agent_status_code::Running;
}

cell_world::Move Knowledge_prey::get_move(const Model_public_state &state) {
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    bool contact = data.visibility[prey_cell].contains(predator_cell);
    if (contact) {
        filter.record_observation(state);
        return knowledge_map.get_move(state);
    } else {
        filter.create_particles();
        unsigned int destination_count = data.world_graph[prey_cell].size();
        vector<double> rewards = vector<double>(destination_count);
        for (auto &particle: filter.particles) {
            auto &particle_rewards = knowledge_map.get_rewards(particle.public_state);
            for (unsigned int index=0; index < destination_count; index++){
                rewards[index] += particle_rewards[index];
            }
        }
        unsigned int destination_index = Chance::pick_best(1,rewards);
        auto &destination = data.world_graph[prey_cell][destination_index];
        auto move = destination.coordinates - prey_cell.coordinates;
        return move;
    }
}

const cell_world::Cell &Knowledge_prey::start_episode() {
    return start;
}

Knowledge_prey::Knowledge_prey(const cell_world::Coordinates &start_coordinates,
                               const cell_world::Coordinates &goal_coordinates,
                               const Particle_filter_parameters &filter_parameters,
                               const Predator_parameters &predator_parameters,
                               const Static_data &data,
                               const std::string &knowledge_file) :
         data(data),
         start(data.map[start_coordinates]),
         goal(data.map[goal_coordinates]),
         filter(filter_parameters,
                predator_parameters,
                data,
                start,
                goal),
         knowledge_map(data.world_graph){
    knowledge_map.load(knowledge_file);
}
