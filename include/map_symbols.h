#pragma once
#include <cell_world.h>
#include <json_cpp.h>


enum Map_symbol_color{
    Default,
    Black = 30,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
};

struct Map_symbol : json_cpp::Json_object {
    Map_symbol() = default;
    explicit Map_symbol (int);
    Map_symbol (int, Map_symbol_color);
    Map_symbol (int, Map_symbol_color, Map_symbol_color);
    unsigned int symbol = 0;
    unsigned int fg = 0;
    unsigned int bg = 0;
    friend std::ostream & operator << (std::ostream & , const Map_symbol &);
    Map_symbol front(Map_symbol_color);
    Map_symbol back(Map_symbol_color);
};

struct Map_symbols : json_cpp::Json_object {
    Map_symbol clear{32, White, Black};
    Map_symbol occluded{32};
    Map_symbol goal{10026, White, Black};
    Map_symbol N{9650, White, Black};
    Map_symbol W{9664, White, Black};
    Map_symbol S{9660, White, Black};
    Map_symbol E{9658, White, Black};
    Map_symbol NE{9701, White, Black};
    Map_symbol NW{9700, White, Black};
    Map_symbol SE{9698, White, Black};
    Map_symbol SW{9699, White, Black};
    Map_symbol highlight{9618};
    Map_symbol zero{48, White, Black};
    Map_symbol one{49, White, Black};
    Map_symbol two{50, White, Black};
    Map_symbol three{51, White, Black};
    Map_symbol four{52, White, Black};
    Map_symbol five{53, White, Black};
    Map_symbol six{54, White, Black};
    Map_symbol seven{55, White, Black};
    Map_symbol eight{56, White, Black};
    Map_symbol nine{57, White, Black};

    Json_object_members({
        Add_member(clear);
        Add_member(occluded);
        Add_member(N);
        Add_member(W);
        Add_member(S);
        Add_member(E);
        Add_member(NE);
        Add_member(NW);
        Add_member(SE);
        Add_member(SW);
        Add_member(highlight);
        Add_member(goal);
    })

    Map_symbol get_direction(const cell_world::Move &) const;
};
