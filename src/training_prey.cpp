#include <training_prey.h>
#include <cell_world/chance.h>

using namespace cell_world;
using namespace std;
using namespace json_cpp;

Training_prey::Training_prey(
            const Static_data &data,
            const Cell &goal,
            const string &input_file,
            const string &output_file):
        data(data),
        goal(goal),
        start_cells(data.cells.free_cells()),
        knowledge_map(data.world_graph),
        output_file(output_file){
    if (input_file.empty())
        knowledge_map.load(input_file);
}

Training_prey::~Training_prey(){
    knowledge_map.save(output_file);
}

const Cell &Training_prey::start_episode() {
    internal_state().status = cell_world::Running;
    start = start_cells.random_cell();
    return start;
}

Move Training_prey::get_move(const Model_public_state &state) {
    const Cell & prey_cell = state.agents_state[0].cell;
    const Cell & predator_cell = state.agents_state[1].cell;
    if (prey_cell == predator_cell) return {0,0};
    double probability_best = 0;
    if (state.agents_state[0].iteration > 1) {
        return knowledge_map.get_move(state);
    } else {
        const Cell & destination = data.world_graph[prey_cell].random_cell();
        key = {prey_cell.id, predator_cell.id, destination.id};
        return destination.coordinates - prey_cell.coordinates;
    }
}

Agent_status_code Training_prey::update_state(const Model_public_state &state) {
    const Cell & prey_cell = state.agents_state[0].cell;
    if (state.agents_state[0].cell == goal){
        update_knowledge_map(true);
        return Agent_status_code::Finished;
    } else {
        const Cell &predator_cell = state.agents_state[1].cell;
        if (prey_cell == predator_cell || state.iterations == state.agents_state[0].iteration) {
            update_knowledge_map(false);
            return Agent_status_code::Finished;
        };
    }
    return Agent_status_code::Running;
}

void Training_prey::update_knowledge_map(bool success){
    double reward = success?100:0;
    const Cell & prey_cell = data.cells[key.prey_cell_index];
    const Cell & predator_cell = data.cells[key.predator_cell_index];
    const Cell & destination = data.cells[key.destination];
    knowledge_map.save_reward(prey_cell, predator_cell, destination, reward);
}