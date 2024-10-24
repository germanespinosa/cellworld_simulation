#include <off_line_knowledge.h>

using namespace cell_world;
using namespace std;
using namespace json_cpp;

Off_line_knowledge_map::Off_line_knowledge_map(const cell_world::Graph &world_graph):
        _graph(world_graph){
    for (auto &predator_cell:_graph.cells){
        Json_vector<Json_vector<float>> predator_cell_reward;
        Json_vector<Json_vector<unsigned int>> predator_cell_counter;
        for (auto &prey_cell:_graph.cells){
            predator_cell_reward.push_back(Json_vector<float>(_graph[prey_cell].size()));
            predator_cell_counter.push_back(Json_vector<unsigned int>(_graph[prey_cell].size()));
        }
        rewards.push_back(predator_cell_reward);
        counters.push_back(predator_cell_counter);
    }
}

void Off_line_knowledge_map::save_reward(const Cell &prey_cell, const Cell &predator_cell, const Cell &destination, float reward){
    unsigned int predator_cell_index = _graph.cells.find(predator_cell);
    unsigned int prey_cell_index = _graph.cells.find(prey_cell);
    unsigned int destination_index = _graph[prey_cell].find(destination);
    auto &reward_value = rewards[predator_cell_index][prey_cell_index][destination_index];
    auto &counter_value = counters[predator_cell_index][prey_cell_index][destination_index];
    reward_value = (reward_value * counter_value + reward) / (counter_value + 1);
    counter_value ++;
}

const Cell &Off_line_knowledge_map::get_destination(const cell_world::Cell &prey_cell, const cell_world::Cell &predator_cell, float probability_best){
    unsigned int predator_cell_index = _graph.cells.find(predator_cell);
    unsigned int prey_cell_index = _graph.cells.find(prey_cell);
    auto &destination_rewards = rewards[predator_cell_index][prey_cell_index];
    return _graph[prey_cell][Chance::pick_best(probability_best, destination_rewards)];
}

json_cpp::Json_vector<float> & Off_line_knowledge_map::get_rewards(const Cell &prey_cell, const Cell &predator_cell) {
    unsigned int predator_cell_index = _graph.cells.find(predator_cell);
    unsigned int prey_cell_index = _graph.cells.find(prey_cell);
    return rewards[predator_cell_index][prey_cell_index];
}

cell_world::Move Off_line_knowledge_map::get_move(const Model_public_state &state) {
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    return get_destination(prey_cell,predator_cell,1).coordinates - prey_cell.coordinates;
}

const cell_world::Cell &Off_line_knowledge_map::get_destination(const Model_public_state &state, float probability_best) {
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    return get_destination(prey_cell,predator_cell,probability_best);
}

json_cpp::Json_vector<float> &Off_line_knowledge_map::get_rewards(const Model_public_state &state) {
    auto &prey_cell = state.agents_state[0].cell;
    auto &predator_cell = state.agents_state[1].cell;
    unsigned int predator_cell_index = _graph.cells.find(predator_cell);
    unsigned int prey_cell_index = _graph.cells.find(prey_cell);
    return rewards[predator_cell_index][prey_cell_index];
}
