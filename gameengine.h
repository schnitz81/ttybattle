#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#define TRUE 1
#define FALSE 0

enum player { PLAYER, CPU };
enum stuff { TICKET, HAND_GRENADE, PANZERFAUST, MISSILE, EMP_BOMB, HBOMB, EXIT };

/* Function prototypes */
void mainloop();
int getRndNum(int nmbOfTurnouts);
void gameover(enum player Player);

#endif

