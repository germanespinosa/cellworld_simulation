#include <catch.h>
#include <cell_world_tools.h>
#include <search_tree.h>
#include <predator.h>
#include <json_cpp.h>
#include <sstream>
#include <simulation.h>

using namespace cell_world;
using namespace std;
using namespace json_cpp;

TEST_CASE("test predator parameters"){
    Predator_parameters p;
    Web_resource::from("predator").key("aggressive").get() >> p;
    CHECK(p.randomness == 0);
    CHECK(p.speed == 1.5);
}

TEST_CASE("gradient") {
//    auto g = gradient(Blue,White);
//    for (auto u:g) cout << u;
}

TEST_CASE("search_tree"){
    World world = Json_create<World>(Web_resource::from("world").key("world_0_0").get());

    Graph_builder gb = Json_create<Graph_builder>(Web_resource::from("graph").key("world_0_0").key("pois").key("astar").get());
    Graph graph = world.create_graph(gb);

    Path_builder pb =Json_create<Path_builder>(Web_resource::from("paths").key("world_0_0").key("astar").get());
    Paths paths = world.create_paths(pb);

    Cell_group cells = world.create_cell_group();
    Map map (cells);

    Search_tree st(graph, paths, map[{0, -7}], 50, Search_tree::ucb1);
    CHECK(st.root.options_cells.size() == 8);
    auto o = st.get_option();
    int i=0;
    while ( !st.finished() ){
        st.get_option();
        if (st.finished()) st.record_reward(100);
        CHECK (i++ < 50);
        if (i==50) break;
    }
    auto o2 = st.get_best_option();
    CHECK(o==o2);
}

TEST_CASE("format result file") {
    CHECK(Simulation::format("simulation_result_%d", 30) == "simulation_result_30");
    CHECK(Simulation::format("simulation_result_%d_here", 15000) == "simulation_result_15000_here");
}