#pragma once
#include <cell_world.h>
#include <predator.h>

struct Particle_filter {
    struct Prey : cell_world::Stateless_agent {
        Prey(const cell_world::Cell &);
        const cell_world::Cell &start_episode() override;
        cell_world::Move get_move(const cell_world::Model_public_state &) override;
        cell_world::Agent_status_code update_state(const cell_world::Model_public_state &) override;
        void end_episode(const cell_world::Model_public_state &) override;

        const cell_world::Cell &start_cell;
        cell_world::Move move;
    };

    Particle_filter(const cell_world::Cell_group &,
                    const cell_world::Graph &,
                    const cell_world::Graph &,
                    const cell_world::Paths &,
                    const cell_world::Cell &start_cell,
                    const cell_world::Cell &goal);

    bool create_particles (int, int);

    const cell_world::Cell_group &cells;
    const cell_world::Graph &world_graph;
    const cell_world::Graph &visibility;
    const cell_world::Paths &paths;
    const cell_world::Cell &start_cell;
    const cell_world::Cell &goal;
    Prey prey;
    Predator predator;
    cell_world::Model model;
    std::vector<cell_world::Agent_public_state> public_particles;
    std::vector<Predator_state> internal_particles;
};