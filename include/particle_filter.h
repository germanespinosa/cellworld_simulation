#pragma once
#include <cell_world.h>
#include <predator.h>

struct Particle {
    Particle ( const cell_world::Agent_public_state &, const Predator_state &);
    cell_world::Agent_public_state public_state;
    Predator_state internal_state;
};

struct Particle_filter {
    struct Prey : cell_world::Stateless_agent {
        Prey(const cell_world::Cell &);
        const cell_world::Cell &start_episode() override;
        cell_world::Move get_move(const cell_world::Model_public_state &) override;
        cell_world::Agent_status_code update_state(const cell_world::Model_public_state &) override;

        const cell_world::Cell &start_cell;
        cell_world::Move move;
    };

    Particle_filter(const cell_world::Cell_group &,
                    const cell_world::Graph &,
                    const cell_world::Graph &,
                    const cell_world::Paths &,
                    const cell_world::Cell &start_cell,
                    const cell_world::Cell &goal,
                    unsigned int,
                    unsigned int );

    unsigned int create_particles ();
    void record_observation(const cell_world::Model_public_state &state);

    const cell_world::Cell_group &cells;
    const cell_world::Graph &world_graph;
    const cell_world::Graph &visibility;
    const cell_world::Paths &paths;
    const cell_world::Cell &start_cell;
    const cell_world::Cell &goal;
    const cell_world::Cell_group &predator_start_locations;
    const unsigned int particle_count;
    const unsigned int attempts_limit;

    Prey prey;
    Predator predator;
    cell_world::Model model;
    std::vector<Particle> particles;
    std::vector<cell_world::Move> trajectory;
    cell_world::Model_public_state last_observation;

private:
    void _from_no_observation();
    void _from_last_observation();

    const cell_world::Cell &_prey_cell;

    const cell_world::Agent_public_state & _predator_state;
    const cell_world::Cell & _predator_cell;
    const Predator_state &_predator_internal_state;

};