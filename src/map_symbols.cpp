#include <json_cpp.h>
#include <cell_world.h>
#include <map_symbols.h>


using namespace std;

Map_symbol Map_symbols::get_direction(const cell_world::Move &c) const {
    if (c.x > 0) {
        if (c.y == 0) return E;
        if (c.y < 0) return NE;
        if (c.y > 0) return SE;
    }
    if (c.x < 0) {
        if (c.y == 0) return W;
        if (c.y < 0) return NW;
        if (c.y > 0) return SW;
    }
    if (c.y < 0) return N;
    if (c.y > 0) return S;

    return highlight;
}


string unicode_to_utf8(int unicode) {
    string s;
    if (unicode >= 0 and unicode <= 0x7f)  // 7F(16) = 127(10)
    {
        s = static_cast<char>(unicode);
        return s;
    } else if (unicode <= 0x7ff)  // 7FF(16) = 2047(10)
    {
        unsigned char c1 = 192, c2 = 128;
        for (int k = 0; k < 11; ++k) {
            if (k < 6) c2 |= (unicode % 64) & (1 << k);
            else c1 |= (unicode >> 6) & (1 << (k - 6));
        }

        s = c1;
        s += c2;

        return s;
    } else if (unicode <= 0xffff)  // FFFF(16) = 65535(10)
    {
        unsigned char c1 = 224, c2 = 128, c3 = 128;
        for (int k = 0; k < 16; ++k) {
            if (k < 6) c3 |= (unicode % 64) & (1 << k);
            else if (k < 12) c2 |= (unicode >> 6) & (1 << (k - 6));
            else c1 |= (unicode >> 12) & (1 << (k - 12));
        }
        s = c1;
        s += c2;
        s += c3;
        return s;
    } else if (unicode <= 0x1fffff)  // 1FFFFF(16) = 2097151(10)
    {
        unsigned char c1 = 240, c2 = 128, c3 = 128, c4 = 128;
        for (int k = 0; k < 21; ++k) {
            if (k < 6) c4 |= (unicode % 64) & (1 << k);
            else if (k < 12) c3 |= (unicode >> 6) & (1 << (k - 6));
            else if (k < 18) c2 |= (unicode >> 12) & (1 << (k - 12));
            else c1 |= (unicode >> 18) & (1 << (k - 18));
        }
        s = c1;
        s += c2;
        s += c3;
        s += c4;
        return s;
    }
    return "";
}


std::ostream &operator<<(ostream &o, const Map_symbol &s) {
    o << "\033[3;" << s.bg << ";" << s.fg << "m";
    o << unicode_to_utf8(s.symbol);
    o << "\033[0m";
    return o;
}

Map_symbol Map_symbol::front(Map_symbol_color c) {
    Map_symbol ms = *this;
    ms.fg = (unsigned int) c;
    return ms;
}

Map_symbol Map_symbol::back(Map_symbol_color c) {
    Map_symbol ms = *this;
    ms.bg = (unsigned int) c + 10;
    return ms;
}

Map_symbol::Map_symbol(int symbol) : symbol(symbol) {
    fg = 0;
    bg = 0;
}

Map_symbol::Map_symbol(int symbol, Map_symbol_color foreground) : symbol(symbol) {
    fg = (unsigned int) foreground;
    bg = 0;
}

Map_symbol::Map_symbol(int symbol, Map_symbol_color foreground, Map_symbol_color background) : symbol(symbol) {
    fg = (unsigned int) foreground;
    bg = (unsigned int) background + 10;
}