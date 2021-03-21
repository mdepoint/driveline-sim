#ifndef _motion_profile_h
#define _motion_profile_h

#include "spline.h"

//#include "autonomous/auton.h"


namespace mp {
  //  autonomous_func::position_info motion_profile();
    void move_forward_temporary( double time, Spline& spline);
}



#endif

