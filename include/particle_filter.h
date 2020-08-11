#pragma once
#include <cell_world.h>
#include <predator.h>

struct Particle_filter_parameters : json_cpp::Json_object {
    unsigned int particle_count;
    unsigned int attempts;
    Json_object_members({
        Add_member(particle_count);
        Add_member(attempts);
    })
};

struct Particle {
    Particle ( const cell_world::Agent_public_state &, const Predator_state &);
    cell_world::Agent_public_state public_state;
    Predator_state internal_state;
};

struct Belief_state {
    cell_world::Coordinates_list particles_coordinates;
    json_cpp::Json_vector<unsigned int> hits;
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

    Particle_filter(const Particle_filter_parameters &,
                    const Predator_parameters &,
                    const Static_data &,
                    const cell_world::Cell &start,
                    const cell_world::Cell &goal);

    int create_particles ();
    void record_observation(const cell_world::Model_public_state &state);
    Belief_state get_belief_state();
    const Particle_filter_parameters &parameters;
    const Static_data &data;
    const cell_world::Cell &start;
    const cell_world::Cell &goal;
    const cell_world::Cell_group predator_start_locations;

    Prey prey;
    Predator predator;
    cell_world::Model model;
    std::vector<Particle> particles;
    std::vector<cell_world::Move> trajectory;
    cell_world::Model_public_state last_observation;
    unsigned int observation_counter;
private:
    void _from_no_observation();
    void _from_last_observation();
};