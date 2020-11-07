#pragma once

struct Position {

    Position() {
        x = y = h = 0;
    }

    Position(double xx, double yy, double hh) {
        x = xx;
        y = yy;
        h = hh;


    }
    double x, y, h;

//    friend std::ostream& operator << (std::ostream& out, const Position& p);
};


extern void run_sim();
extern Position get_position();
extern double toDegrees(double rad);