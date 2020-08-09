#pragma once
#include <cell_world.h>
#include <static_data.h>

struct Predator_parameters :json_cpp::Json_object{
    cell_world::Coordinates start;
    double speed;
    double randomness;
    Json_object_members({
        Add_member(start);
        Add_member(speed);
        Add_member(randomness);
    });
};

struct Predator_state : cell_world::Agent_internal_state {
    cell_world::Cell goal;
};

struct Predator : cell_world::Stateful_agent<Predator_state> {
    Predator (const Predator_parameters &, const Static_data &);
    const cell_world::Cell &start_episode() override;
    cell_world::Move get_move(const cell_world::Model_public_state &) override;
    cell_world::Agent_status_code update_state(const cell_world::Model_public_state &) override;

    const Predator_parameters &parameters;
    const Static_data &data;
    cell_world::Cell start_cell;
private:
    void _process_state(const cell_world::Model_public_state &);
    unsigned int _steps() const;
    unsigned int _speed_base;
    double _speed_overflow;
};