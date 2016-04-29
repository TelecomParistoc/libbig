#include "Action.hpp"
#include <librobot/doors.h>
#include <librobot/eater.h>

/**Attention : ceci n'est qu'un exemple complètement irréaliste.
* Chaque robot doit avoir son propre fichier ways_and_actions qui doit être rempli en conséquences
**/

std::vector<std::string> ways = {"move_cubes_go_fish","water2net","net2water","water2net","net2water","water2net","net2rocks","rocks2rocks"};
std::vector<Action> actions;

void initWaysAndActions()
{
    actions.push_back(Action()); // DO NOT REMOVE
    actions.push_back(Action(startEaterAction,&pecher_pause,&pecher_continue,&pecher_isFinished)); // TODO
    actions.push_back(Action(startDoorsAction, pauseDoorsAction, resumeDoorsAction, isDoorsActionFinished));
    //etc ...
}
