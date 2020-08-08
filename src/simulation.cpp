#include <simulation.h>

using namespace cell_world;
using namespace json_cpp;
using namespace std;

Simulation::Simulation(Simulation_parameters &sp) :
    world ( sp.world ),
    pois ( sp.world.create_cell_group(
                    Json_create<Cell_group_builder>(
                            Web_resource::from("cell_group")
                            .key(world.name)
                            .key("pois").get()
                    )
            )
    ),
    world_graph(sp.world.create_graph()),
    poi_graph ( sp.world.create_graph(
                    Json_create<Graph_builder>(
                            Web_resource::from("graph")
                            .key(sp.cmd_parameters["world"].value)
                            .key("pois")
                            .key(sp.cmd_parameters["path_type"].value).get()
                    )
                )
    ),
    cells (sp.world.create_cell_group()),
    map (cells),
    paths (sp.world.create_paths(sp.path_type)),
    visibility ( sp.world.create_graph(
                    Json_create<Graph_builder>(
                            Web_resource::from("graph")
                            .key(sp.world.name).key("visibility").get()
                    )
                )
    ),
    model (cells),
    planning_parameters (sp.planning),
    prey (cells, world_graph, pois, poi_graph, visibility, paths, map[sp.prey_start], map[sp.goal], sp.planning),
    predator (world_graph, visibility, paths, map[sp.predator_start])
{
    model.add_agent(prey);
    model.add_agent(predator);
}


struct Data : json_cpp::Json_object{
    Data () = default;
    Data (Model_public_state &state,
          Poi_prey_state & prey_state,
          Predator_state & predator_state) :
            iteration(state.agents_state[0].iteration),
            prey(state.agents_state[0].cell.coordinates),
            predator(state.agents_state[1].cell.coordinates),
            prey_state(prey_state),
            predator_state(predator_state){}
    unsigned int iteration;
    Coordinates prey;
    Coordinates predator;
    Poi_prey_state prey_state;
    Predator_state predator_state;
    Json_object_members({
                            Add_member(iteration);
                            Add_member(prey);
                            Add_member(predator);
                            Add_member(prey_state);
                            Add_member(predator_state);
                        })
};


unsigned int Simulation::run() {
    model.start_episode();
    cout << "prey: " << model.state.public_state.agents_state[0].cell.coordinates << endl;
    cout << "predator: " << model.state.public_state.agents_state[1].cell.coordinates << endl;
    do {
        if (!model.state.public_state.current_turn) {
            Data data {model.state.public_state,
                       prey.internal_state(),
                       predator.internal_state()};
            cout << data << endl;
        }
    } while (model.update());
    cout << "prey: " << model.state.public_state.agents_state[0].cell.coordinates << endl;
    cout << "predator: " << model.state.public_state.agents_state[1].cell.coordinates << endl;
    model.end_episode();
    return 1;
}
