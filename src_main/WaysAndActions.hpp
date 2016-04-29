#include "Action.hpp"
#include <librobot/doors.h>
#include <librobot/eater.h>

/**
*  This file maps the actions methods and paths to be executed by the main program
**/

std::vector<std::string> ways = {"start2cubes","zone2doors"};
std::vector<Action> actions;

void initWaysAndActions() {
    actions.push_back(Action()); // DO NOT REMOVE
    actions.push_back(Action(startEaterAction,&pecher_pause,&pecher_continue,&pecher_isFinished)); // TODO
    actions.push_back(Action(startDoorsAction, pauseDoorsAction, resumeDoorsAction, isDoorsActionFinished));
    //etc ...
}
