#pragma once
#include <cell_world.h>

struct Open_prey : cell_world::Stateless_agent{
    Open_prey (cell_world::Cell &, cell_world::Coordinates);
    const cell_world::Cell &start_episode() override;
    cell_world::Move get_move(const cell_world::Model_public_state &) override;
    cell_world::Agent_status_code update_state(const cell_world::Model_public_state &) override;
    void end_episode(const cell_world::Model_public_state &) override;
    const cell_world::Cell &cell() const;
    cell_world::Cell &start_cell;
    cell_world::Coordinates goal;
    cell_world::Move move;
};