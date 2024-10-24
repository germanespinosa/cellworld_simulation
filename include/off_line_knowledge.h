#include <cell_world.h>

struct Off_line_knowledge_key {
    Off_line_knowledge_key( unsigned int prey_cell_index,
                            unsigned int predator_cell_index,
                            unsigned int destination):
            prey_cell_index(prey_cell_index),
            predator_cell_index(predator_cell_index),
            destination(destination){}
    unsigned int prey_cell_index;
    unsigned int predator_cell_index;
    unsigned int destination;
};

struct Off_line_knowledge_map : json_cpp::Json_object{
    Off_line_knowledge_map(const cell_world::Graph &);
    void save_reward(const cell_world::Cell &, const cell_world::Cell &, const cell_world::Cell &, float);
    const cell_world::Cell &get_destination(const cell_world::Cell &, const cell_world::Cell &, float);
    const cell_world::Cell &get_destination(const cell_world::Model_public_state &, float);
    cell_world::Move get_move(const cell_world::Model_public_state &);
    json_cpp::Json_vector<float> &get_rewards(const cell_world::Cell &, const cell_world::Cell &);
    json_cpp::Json_vector<float> &get_rewards(const cell_world::Model_public_state &);

    const cell_world::Graph &_graph;
    json_cpp::Json_vector<json_cpp::Json_vector<json_cpp::Json_vector<float>>> rewards;
    json_cpp::Json_vector<json_cpp::Json_vector<json_cpp::Json_vector<unsigned int>>> counters;
    Json_object_members({
        Add_member(rewards);
        Add_member(counters);
    })
};
