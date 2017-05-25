#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <robotdriver/movecontroller.h>
#include <robotdriver/motordriver.h>
#include <robotdriver/timing.h>
#include "robotdefs.big.h"
#include "actionsgrobot.h"

#define TAILLE_MAX 32
#define TAILLE_COORDONNEES 5
#define RADIUS 30
#define STARTRADIUS 30
#define OFFSET_X_BIG 1320
#define OFFSET_Y_BIG 180

// 0: blue, 1: yellow
static int team = 0;
// 0: translation, 1: rotation
static int mov = 0;
// 0: forward, 1: backward
static int dir = 0;

int readAndCall(FILE * file, char c)
{
    if((c >= '0') && (c <= '9')) {
		if(!mov){
		    char Xcoord[TAILLE_COORDONNEES];
		    char Ycoord[TAILLE_COORDONNEES];
		    Xcoord[0] = c;
		    int i = 1;
		    while((c = fgetc(file)) != ' ')
		        Xcoord[i++] = c;
		    i = 0;
		    while((c = fgetc(file)) != '\n')
		        Ycoord[i++] = c;
		    int x = atoi(Xcoord);
		    int y = atoi(Ycoord);

	        // offset
	        if(!team) { // blue
	            x -= OFFSET_X_BIG;
	            y -= OFFSET_Y_BIG;
	        } else { // yellow
	            x += OFFSET_X_BIG;
	            y -= OFFSET_Y_BIG;
	        }

		    printf("[FILE] go to [%d,%d]\n", x, y);
		    point_t dest = {x, y};
		    if(!dir)
	                goForward(&dest);
		    else
			goBackward(&dest);
	            return 0;
		} else {
    	    char rot_str[TAILLE_COORDONNEES];
    	    rot_str[0] = c;
    	    int i = 1;
    	    while((c = fgetc(file)) != '\n')
    	        rot_str[i++] = c;
    	    int rot = atoi(rot_str);
	    if(rot < 0)
		rot += 3600;
    	    printf("[FILE] go to [%ddeg]\n", rot / 10);
            rotate(rot);
            return 0;
	}
    }
    else if(c == 'I'){
        printf("[FILE] It's an I \n");
    	char initXchar[TAILLE_COORDONNEES];
    	char initYchar[TAILLE_COORDONNEES];
    	char initAchar[TAILLE_COORDONNEES];
    	initXchar[0] = c;
    	int i = 1;
    	while((c = fgetc(file)) != ' ')
    	    initXchar[i++] = c;
    	i = 0;
    	while((c = fgetc(file)) != '\n')
    	    initYchar[i++] = c;
    	i = 0;
    	while((c = fgetc(file)) != '\n')
    	    initAchar[i++] = c;
    	int x = atoi(initXchar);
    	int y = atoi(initYchar);
    	int a = atoi(initAchar);
	setHeading(a);
	// ==============================================================
    	// MUST INIT X AND Y
	// ==============================================================
	printf("[FILE] init [%d,%d] %d\n", x, y, a);
        return 0;
    }
    while(fgetc(file) != '\n');
    // Big expands then get balls
    if(c == 'X'){
        printf("[FILE] It's an X \n");
	expandGrobot();
    }
    // Big release balls
    else if(c == 'Y'){
        printf("[FILE] It's a Y \n");
	activerLanceurDeBalles();
    }
    // More forward
    else if(c == 'A'){
        printf("[FILE] It's an A \n");
	mov = 0;
	dir = 0;
    }
    // Move backward
    else if(c == 'R'){
        printf("[FILE] It's a R \n");
		mov = 0;
		dir = 1;
    }
    // Rotate
    else if(c == 'T'){
        printf("[FILE] It's a T \n");
		mov = 1;
    }
    else if(c == '#'){}
    else if(c == '\n'){}
    else {
	if(c != 255)
        printf("[FILE] PARSING ERROR: %d\n", c);
        return 1;
    }
    return 0;
}
