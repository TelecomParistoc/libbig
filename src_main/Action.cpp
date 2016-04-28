#include "Action.hpp"

Action::Action(const std::function<void()>& start, const std::function<void()>& pause, const std::function<void()>& cont, const std::function<bool()>& isFinished) :
    startFunction(start),
    pauseFunction(pause),
    continueFunction(cont),
    isFinishedFunction(isFinished)
{}

Action::Action(void (*start)(void), void (*pause)(void), void (*cont)(void), int (*isFinished)(void)) :
    startFunction(start),
    pauseFunction(pause),
    continueFunction(cont),
    isFinishedFunction(convertCtoCpp(isFinished))
{}

void Action::start()
{startFunction();}

void Action::pauseAction()
{pauseFunction();}

void Action::continueAction()
{continueFunction();}

bool Action::isFinished() const
{return isFinishedFunction();}

std::function<bool()> Action::convertCtoCpp(int (*isFinished)(void))
{return std::bind(&Action::convertIntCtoBool,std::function<int()>(isFinished));}

bool Action::convertIntCtoBool(const std::function<int()>& f)
{return (bool)f();}
