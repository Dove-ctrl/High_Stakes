#include "vex.h"
#include "elementOS.h"
#include "opfunction.h"

using namespace vex;

competition Competition;

void usercontrol_threads(){
  vex::thread(op_move , &Competition);
}

void autonomous(void) {
  waitUntil(INITIALIZE_DONE);
  switch (PROGRAMFLAG)
  {
  case 1://tournament1
    tournament_route1(&Competition);
    break;
  case 13://tournament2
    tournament_route2(&Competition);
    break;
  case 14://tournament3
    tournament_route3(&Competition);
    break;
  case 15://tournament4
    tournament_route4(&Competition);
    break;
  case 2://skill
    skill_route(&Competition);
    break;
  default:
    break;
  }
}

void usercontrol(void) {
  waitUntil(INITIALIZE_DONE);
  switch (PROGRAMFLAG)
  {
  case 1://tournament1
    Brain.Screen.printAt(1,10,"tournament1");
    usercontrol_threads();
    break;
  case 13://tournament2
    Brain.Screen.printAt(100,100,"tournament2");
    usercontrol_threads();
    break;
  case 14://tournament3
    Brain.Screen.printAt(100,100,"tournament3");
    usercontrol_threads();
    break;
  case 15://tournament4
    Brain.Screen.printAt(100,100,"tournament4");
    usercontrol_threads();
    break;
  case 2://skill
    Brain.Screen.printAt(100,100,"skill");
    usercontrol_threads();
    break;
  case 3://driver_debug
    Brain.Screen.printAt(1,15,"driver_debug");
    usercontrol_threads();
    break;
  case 4://auto_skill_debug
    Brain.Screen.printAt(100,100,"auto_skill_debug");
    skill_route(&Competition);
    break;
  case 9://auto_tournament_debug_1
    Brain.Screen.printAt(100,100,"auto_tournament_debug_route_1");
    tournament_route1(&Competition);
    break;
  case 10://auto_tournament_debug_2
    Brain.Screen.printAt(100,100,"auto_tournament_debug_route_2");
    tournament_route2(&Competition);
    break;
  case 11://auto_tournament_debug_3
    Brain.Screen.printAt(100,100,"auto_tournament_debug_route_3");
    tournament_route3(&Competition);
    break;
  case 12://auto_tournament_debug_4
    Brain.Screen.printAt(100,100,"auto_tournament_debug_route_4");
    tournament_route4(&Competition);
    break;
  default:
    break;
  }
}

int main() {
  eos::SystemInitialize(&Brain , &Controller);
  thread main_elementOS(elementOS , &Competition);
  chassis::GetInstance();
  thread chassis(chassis_drive_thread);
  
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);

  while (true) {
    wait(100, msec);
  }
}