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
void set_position(Position p);
extern double toDegrees(double rad);
extern Position computeMovement(double lfPower, double rfPower, double lrPower, double rrPower, Position curPos, double dt);