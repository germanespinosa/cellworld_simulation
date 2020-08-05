#include <reward.h>
#include <particle_filter.h>

struct Panning_prey : cell_world::Stateless_agent{
    Panning_prey (const cell_world::Cell &start, const cell_world::Cell &goal) :
            start_cell(start), goal(goal){
    }
    const cell_world::Cell &start_episode() override {
        return start_cell;
    };
    cell_world::Move get_move(const cell_world::Model_public_state &) override {
        return move;
    };
    cell_world::Agent_status_code update_state(const cell_world::Model_public_state &) override {
        return cell_world::Running;
    };
    const cell_world::Cell &start_cell;
    const cell_world::Cell &goal;
    cell_world::Move move;
};

struct Planning_parameters : json_cpp::Json_object {
    unsigned int particles;
    unsigned int attempts;
    unsigned int roll_outs;
    Reward reward;
    Json_object_members({
                            Add_member(particles);
                            Add_member(attempts);
                            Add_member(roll_outs);
                            Add_member(reward);
                        })
};

struct Planner{
    Planner(const Planning_parameters &,
            const Particle_filter &,
            const cell_world::Graph &,
            const cell_world::Graph &,
            const cell_world::Graph &,
            const cell_world::Paths &,
            const cell_world::Cell &,
            const cell_world::Cell &);

    cell_world::Move get_best_move(const cell_world::Model_public_state &,
                                   double &,
                                   const std::vector<Particle> &);

    cell_world::Move get_best_move(const cell_world::Model_public_state &,
                                   double &);


    const Planning_parameters &parameters;
    const Particle_filter &filter;
    const cell_world::Graph &pois_graph;
    const cell_world::Paths &paths;

    cell_world::Model model;
    Panning_prey prey;
    Predator predator;
};