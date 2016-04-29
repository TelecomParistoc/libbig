#include <pathfollower/pathFollower.hpp>
#include <robotdriver/speedcontroller.h>
#include <robotdriver/motioncontroller.h>
#include <robotdriver/toolboxdriver.h>
#include <robotdriver/motordriver.h>
#include <librobot/robot.h>
#include <iostream>
#include <chrono>


#include "WaysAndActions.hpp"


bool start = false;
bool isMovingToAction = false;
bool started = false;
bool blocked = false;
bool seeBlocked = false;

int way = 0;
std::pair<double,double> curPos = std::pair<double,double>(40,1000);

void go()
{start = true;}

void checkCollisionAndReact(int)
{seeBlocked = true;}

void endAction()
{
    std::cout<<"End of action "<<way<<std::endl;
    curPos = PathFollower::getCurrentPos();
    PathFollower::setCurrentPosition(curPos.first,curPos.second);
    way++;
    if(way<ways.size())
        ffollow(ways[way].c_str(), &endWay);
    isMovingToAction = true;
}

void endWay()
{
    std::cout<<"End of way "<<way<<std::endl;
    actions[way].start();
    isMovingToAction = false;
}

int main()
{
    initRobot();
    setRGB(255, 0, 0);

    initWaysAndActions();

    onGameStart(&go); //start = true;
    onCollisionDetect(&checkCollisionAndReact);

    setMoveStartCallback(&PathFollower::updateAngleStartingMove);
    setMoveEndCallback(&PathFollower::updatePositionEndingMove);
    setCurrentLocation(curPos.first,curPos.second);
    PathFollower::resetPosition(curPos);

    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::milliseconds milliseconds;
    Clock::time_point clk_start = Clock::now();

    double seconds = 90;
    endWay();
    while(std::chrono::duration_cast<milliseconds>(Clock::now()-clk_start).count()<seconds*1000)
    {
        if(start&&!started)
        {
            printf("Start\n");
            started = true;
            ffollow(ways[way].c_str(), &endWay);
        }

        if(!isMovingToAction)
            if(actions[way].isFinished())
                endAction();

        if(seeBlocked)
            if(PathFollower::isSpeedPositive())
            {
                if(!isRobotFront())
                    seeBlocked = false;
                else
                    if(!PathFollower::isPaused())
                    {
                        blocked = true;
                        if(isMovingToAction)
                            PathFollower::pause();
                        else
                            actions[way].pauseAction();
                    }
            }
            else
            {
                if(!isRobotBehind())
                    seeBlocked = false;
                else
                    if(!PathFollower::isSpeedPositive())
                        if(!PathFollower::isPaused())
                        {
                            blocked = true;
                            if(isMovingToAction)
                                PathFollower::pause();
                            else
                                actions[way].pauseAction();
                        }
            }

        if(!seeBlocked)
            if(blocked)
            {
                blocked = false;
                if(isMovingToAction)
                    PathFollower::continueMoving();
                else
                    actions[way].continueAction();
            }

        /*curPos = PathFollower::getCurrentPos();
        curDir = PathFollower::getCurrentDirection();
        std::cout<<curPos.first<<" "<<curPos.second<<";"<<curDir.first<<" "<<curDir.second<<std::endl;*/

        waitFor(50);
    }

    return 0;
}