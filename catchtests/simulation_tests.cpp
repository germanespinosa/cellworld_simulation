#include <catch.h>
#include <cell_world_tools.h>
#include <predator.h>
#include <json_cpp.h>
#include <sstream>

using namespace cell_world;
using namespace std;
using namespace json_cpp;

TEST_CASE("test predator parameters"){
    Predator_parameters p;
    Web_resource::from("predator").key("aggressive").get() >> p;
    CHECK(p.randomness == 0);
    CHECK(p.speed == 1);
    CHECK(p.start == Coordinates{-7,7});
}

vector<Map_symbol> gradient(Map_symbol_color fg, Map_symbol_color bg){
    vector<Map_symbol> g;
    g.emplace_back(32, fg, bg);
    g.emplace_back(9617, fg, bg);
    g.emplace_back(9618, fg, bg);
    g.emplace_back(9619, fg, bg);
    g.emplace_back(9608, fg, bg);
    return g;
}


TEST_CASE("gradient") {
    auto g = gradient(Blue,White);
    for (auto u:g) cout << u;
}