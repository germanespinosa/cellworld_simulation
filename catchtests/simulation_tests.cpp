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
    CHECK(p.randomness == .1);
    CHECK(p.speed == 1.8);
    CHECK(p.start == Coordinates{-7,-7});
}