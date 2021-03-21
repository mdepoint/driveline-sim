#include <windows.h>
#include <cmath>
#include <math.h>

#include "spline.h"
#include "simulator.h"


namespace mp {
    double distance;
    double time = 0;
    double acceleration_max = 30;
    double deceleration_max = 5;

    bool starting = true;

    double current_distance = 0;
    double remaining_distance = 0;

    double velocity = 0;
    double velocity_max = 24;
    double velocity_minimum = 5;

    double voltage = 0;
    double lastHeading = 0;
    double voltage_max = 12;


    void move_forward_temporary(double time, Spline& spline) {

        if (time == 0) {
            distance = spline.getDistance();
            current_distance = 0;
            remaining_distance = distance;
            velocity = 0;
            voltage = 0;
        }

//        vex::timer timer = vex::timer();

        Position pos = get_position();

        double dT = 10;

//        while (true) {
        if( 1 ){

            double acceleration_speed = fmax(acceleration_max * time, velocity_minimum);

            remaining_distance = distance - current_distance;
            double deceleration_speed = remaining_distance * deceleration_max;


            velocity = fmin(acceleration_speed, velocity_max);
            velocity = fmin(velocity, deceleration_speed);


            double distance_percentage = current_distance / distance;

            double heading = spline.getH(distance_percentage);

            double headingDiff = heading - lastHeading;

            lastHeading = heading;


            double leftFactor = 1.0;
            double rightFactor = 1.0;

            double adjustment = fabs(headingDiff * 20);

            if (headingDiff > 0) {
                leftFactor = 1.0 - adjustment;
            }

            else if (headingDiff < 0) {
                rightFactor = 1.0 - adjustment;
            }



            voltage = (velocity / velocity_max) * voltage_max;

//            if (velocity == deceleration_speed) {

            if (current_distance >= remaining_distance && velocity <= velocity_minimum) {
                return;
            }


            pos = computeMovement(voltage/12.0*leftFactor, voltage/12.0*rightFactor, voltage/12.0*leftFactor, voltage/12.0*rightFactor, pos, dT/1000.0);
            set_position(pos);


//                frontLeft.spin(fwd, voltage, voltageUnits::volt);
  //              backLeft.spin(fwd, voltage, voltageUnits::volt);
    //            frontRight.spin(fwd, voltage * 0.92, voltageUnits::volt);
      //          backRight.spin(fwd, voltage * 0.92, voltageUnits::volt);
  //          }

    //        else {
        //        frontLeft.spin(fwd, voltage, voltageUnits::volt);
          //      backLeft.spin(fwd, voltage, voltageUnits::volt);
            //    frontRight.spin(fwd, voltage, voltageUnits::volt);
              //  backRight.spin(fwd, voltage, voltageUnits::volt);

//            }

            //task::sleep(dT);
//            Sleep(dT);

                //            double new_time = timer.time(sec);
                double new_time = time + dT/1000.0;

            current_distance += (new_time - time) * velocity;
            time = new_time;


//            if (current_distance >= remaining_distance && velocity <= velocity_minimum) {
                //frontLeft.stop();
                //frontRight.stop();
                //backLeft.stop();
                //backRight.stop();
      //          break;
        //    }

        }

    }

	}