#include <simulation.h>

using namespace cell_world;
using namespace cell_world_tools;
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
                            .key(sp.world.name)
                            .key("pois")
                            .key(sp.cmd_parameters["path_type"]).get()
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


unsigned int Simulation::run() {
    struct Data : json_cpp::Json_object{
        Data () = default;
        Data (unsigned int iteration,
              Coordinates prey,
              Coordinates predator) : iteration(iteration), prey(prey), predator(predator) {}
        unsigned int iteration;
        Coordinates prey;
        Coordinates predator;
        Json_object_members({
            Add_member(iteration);
            Add_member(prey);
            Add_member(predator);
        })
    };
    model.start_episode();
    cout << "prey: " << model.state.public_state.agents_state[0].cell.coordinates << endl;
    cout << "predator: " << model.state.public_state.agents_state[1].cell.coordinates << endl;
    do {
        if (!model.state.public_state.current_turn) {
            Data data {model.state.public_state.agents_state[0].iteration,
                       model.state.public_state.agents_state[0].cell.coordinates,
                       model.state.public_state.agents_state[1].cell.coordinates};
            cout << data << endl;
        }
    } while (model.update());
    cout << "prey: " << model.state.public_state.agents_state[0].cell.coordinates << endl;
    cout << "predator: " << model.state.public_state.agents_state[1].cell.coordinates << endl;
    model.end_episode();
    return 1;
}
