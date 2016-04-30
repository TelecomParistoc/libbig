#include <pathfollower/pathFollower.hpp>
#include <librobot/doors.h>
#include <librobot/eater.h>
#include "Action.hpp"

/**
*  This file maps the actions methods and paths to be executed by the main program
**/

std::vector<std::string> ways = {"start2cubes","zone2doors"};
std::vector<Action> actions;
std::pair<double,double> curPos = std::pair<double,double>(130,1255);

void initWaysAndActions()
{
    actions.push_back(Action()); // DO NOT REMOVE
    actions.push_back(Action(startEaterAction, pauseEaterAction, resumeEaterAction, isEaterActionFinished));
    actions.push_back(Action(startDoorsAction, pauseDoorsAction, resumeDoorsAction, isDoorsActionFinished));

    PathFollower::setRadiusPositiveSpeed(180);
    PathFollower::setRadiusNegativeSpeed(120);

    setLED(1,1);
}

bool start = false;

void go()
{start = true;}

void allume()
{
    std::cout<<"Jack grabed"<<std::endl;
    onGameStart(&go);
    /**A completer pour chaque robot**/
    onStopGame(&exitAndClean);
}

void exitAndClean()
{
    std::cout<<"Exiting"<<std::endl;
    exit(0);
}
