#include <stdlib.h>
#include <librobot/robot.h>
#include <librobot/umbrella.h>

int main(int argc, char * argv[]){
	initRobot();
	if(atoi(argv[1]))
		deployUmbrella();
	else
		closeUmbrella();
	while(1);
	return 0;
}
