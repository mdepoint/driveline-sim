// TestApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//



#include <iostream>
#include <iomanip>

const double PI = 3.14159;

double toRad(double deg) {
    return (deg * PI / 180.0);
}


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

    friend std::ostream& operator << (std::ostream& out, const Position& p);
};

std::ostream& operator << (std::ostream& out, const Position& p)
{
    out <<  "( " << p.x << ", " << p.y << ", " << p.h << " )";
    return out;
}


//const double TIRE_ANGLE = toRad(45);
const double TIRE_ANGLE = toRad(0);
const double WHEEL_RADIUS = 0.05; // 5 cm
const double RPM_MAX_POWER = 1000;

const Position LF_POSITION = Position(-0.1, 0.1, toRad(45));
const Position RF_POSITION = Position(0.1, 0.1, -toRad(45));
const Position LR_POSITION = Position(-0.1, -0.1, -toRad(45));
const Position RR_POSITION = Position(0.1, -0.1, toRad(45));

// Straight Wheel Drive
//const Position LF_POSITION = Position(-0.1, 0.1, toRad(0));
//const Position RF_POSITION = Position(0.1, 0.1, toRad(0));
//const Position LR_POSITION = Position(-0.1, -0.1, toRad(0));
//const Position RR_POSITION = Position(0.1, -0.1, toRad(0));



double powerToRev(double power, double dt) {
    const double wheelCircumference = 2 * PI * WHEEL_RADIUS;

    // 0 power = 0 rpm
    // 1.0 power = RPM_MAX_POWER

   double revs = power * RPM_MAX_POWER / 60.0 * dt;

   return revs * wheelCircumference;

}


Position computeMovement(double lfPower, double rfPower, double lrPower, double rrPower, Position curPos, double dt) {

    // Summate the forces in local coordinates
    double lfX = lfPower * cos(LF_POSITION.h);
    double lfY = lfPower * sin(LF_POSITION.h);

    double rfX = rfPower * cos(RF_POSITION.h);
    double rfY = rfPower * sin(RF_POSITION.h);

    double lrX = lrPower * cos(LR_POSITION.h);
    double lrY = lrPower * sin(LR_POSITION.h);

    double rrX = rrPower * cos(RR_POSITION.h);
    double rrY = rrPower * sin(RR_POSITION.h);

    double totalForceX = lfX + rfX + lrX + rrX;
    double totalForceY = lfY + rfY + lrY + rrY;

    // Summate the moments
    double lfMoment = lfX * LF_POSITION.x + lfY * LF_POSITION.y;
    double rfMoment = rfX * RF_POSITION.x + rfY * RF_POSITION.y;
    double lrMoment = lrX * LR_POSITION.x + lrY * LR_POSITION.y;
    double rrMoment = rrX * RR_POSITION.x + rrY * RR_POSITION.y;
    double totalMoment = lfMoment + rfMoment + lrMoment + rrMoment;

 
   
    // Convert force to distace
    // TO DO
    Position  newPos;

    double factor = 1.0;  // FIXME
    double momentFactor = 7500000;

    newPos.h = curPos.h + totalMoment * momentFactor * dt;


    double localX = totalForceX * factor * dt;
    double localY = totalForceY * factor * dt;



    double cosH = cos(newPos.h);
    double sinH = sin(newPos.h);

    
    newPos.x = curPos.x + (localX * cos(newPos.h) - localY * sin(newPos.h));
    newPos.y = curPos.y + (localY * cos(newPos.h) + localX * sin(newPos.h));
            
    
    return newPos;

}










int main()
{

    // All forward
    // Forward
    computeMovement(1.0, 1.0, 1.0, 1.0, Position(0, 0, 0), 0.1);

    // Face and Drive Left
//    computeMovement(1.0, 1.0, 1.0, 1.0, Position(0, 0, toRad(-90)), 0.1);

    // Face and Drive Right
//    computeMovement(1.0, 1.0, 1.0, 1.0, Position(0, 0, toRad(90)), 0.1);

    // Face and Drive Backward
//    computeMovement(1.0, 1.0, 1.0, 1.0, Position(0, 0, toRad(180)), 0.1);



    // Strafe
    computeMovement(1.0, -1.0, -1.0, 1.0, Position(0, 0, 0), 0.1);

    // Spin
    computeMovement(1.0, -1.0, 1.0, -1.0, Position(0, 0, 0), 0.1);

    // One Side
    computeMovement(1.0, 0.0, 1.0, 0.0, Position(0, 0, 0), 0.1);


    Position pos;
    double t = 0;
    double dt = 0.05;

    std::cout << std::fixed << std::setprecision(3);

    

    // All motors for 2 seconds
    std::cout << "--- FORWARD ---" << std::endl;
    while( t < 2.0) {
        pos = computeMovement(1.0, 1.0, 1.0, 1.0, pos, dt);
        t += dt;
        std::cout << "Time: " << t << " Pos: " << pos << std::endl;

    }

    // Strafe for 2 seconds
    std::cout << "--- STRAFE ---" << std::endl;
    while (t < 4.0) {
        pos = computeMovement(1.0, -1.0, -1.0, 1.0, pos, dt);
        t += dt;
        std::cout << "Time: " << t << " Pos: " << pos << std::endl;

    }


    // Spin for 2 second
    std::cout << "--- SPIN ---" << std::endl;
   while (t < 6.0) {
        pos = computeMovement(1.0, -1.0, 1.0, -1.0, pos, dt);
        t += dt;
        std::cout << "Time: " << t << " Pos: " << pos << std::endl;

    }

  
    // Back for 2 seconds
    std::cout << "--- BACK ---" << std::endl;
    while (t < 8.0) {
        pos = computeMovement(-1.0, -1.0, -1.0, -1.0, pos, dt);
        t += dt;
        std::cout << "Time: " << t << " Pos: " << pos << std::endl;
    }







}




// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
