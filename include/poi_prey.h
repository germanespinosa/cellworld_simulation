#pragma once
#include <cell_world.h>
#include <reward.h>
#include <predator.h>
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

struct Poi_prey_state : cell_world::Agent_internal_state {
    cell_world::Agent_status_code status;
    cell_world::Move move;
    cell_world::Coordinates coordinates;
    double estimated_reward;
};

struct Poi_prey : cell_world::Stateful_agent<Poi_prey_state> {
    Poi_prey(const cell_world::Cell_group &,
             const cell_world::Graph &,
             const cell_world::Cell_group &,
             const cell_world::Graph &,
             const cell_world::Graph &,
             const cell_world::Paths &,
             const cell_world::Cell &,
             const cell_world::Cell &,
             const Reward &);
    const cell_world::Cell &start_episode() override;
    cell_world::Move get_move(const cell_world::Model_public_state &) override;
    cell_world::Agent_status_code update_state(const cell_world::Model_public_state &) override;

    bool process_state(const cell_world::Model_public_state &);
    cell_world::Move plan (bool ,const cell_world::Model_public_state &);

    const cell_world::Cell_group &cells;
    const cell_world::Cell_group &pois;
    const cell_world::Graph &pois_graph;
    const cell_world::Graph &world_graph;
    const cell_world::Graph &visibility;
    const cell_world::Paths &paths;
    const cell_world::Cell &start_cell;
    const cell_world::Cell &goal;
    const Reward reward;

    cell_world::Model model;
    Panning_prey prey;
    Predator predator;
    Particle_filter particle_filter;
};