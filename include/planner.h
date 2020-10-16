#include <reward.h>
#include <particle_filter.h>
#include <search_tree.h>

struct Planner_parameters : json_cpp::Json_object {
    unsigned int roll_outs;
    Reward reward;
    std::string tree_mode;
    Json_object_members({
                            Add_member(roll_outs);
                            Add_member(reward);
                            Add_member(tree_mode);
                        })
};

struct Planner{
    struct Panner_prey : cell_world::Stateless_agent{
        Panner_prey (const cell_world::Cell &start, const cell_world::Cell &goal) :
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

    Planner(const Planner_parameters &,
            const Particle_filter_parameters &,
            const Predator_parameters &,
            const Static_data &,
            const cell_world::Cell &,
            const cell_world::Cell &);

    cell_world::Move get_best_move(const cell_world::Model_public_state &,
                                   double &);

    cell_world::Agent_status_code update_state(const cell_world::Model_public_state &);

    cell_world::Cell_group get_valid_options(const cell_world::Cell &, unsigned int) const;


    void reset();

    const Planner_parameters &parameters;
    const Static_data &data;

    cell_world::Coordinates option;
    Particle_filter filter;
    cell_world::Model model;
    Panner_prey prey;
    Predator predator;
    Search_tree::Mode tree_mode;
};