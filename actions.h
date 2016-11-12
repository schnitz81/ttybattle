#ifndef ACTIONS_H
#define ACTIONS_H

#include <menu.h>

/* Function prototypes */

void use_falcon_punch();
void use_ticket();
int use_handgrenade();
int use_panzerfaust();
int use_missile();
int use_empbomb();
int use_hbomb();

void buy_ticket(MENU *shop_menu);
void buy_handgrenade(MENU *shop_menu);
void buy_panzerfaust(MENU *shop_menu);
void buy_missile(MENU *shop_menu);
void buy_empbomb(MENU *shop_menu);
void buy_hbomb(MENU *shop_menu);
void initialize_stats();
void add_dosh(enum player Player, int nbrToAdd);
void subtract_dosh(enum player Player, int nbrToSubtract);
void subtract_energy(enum player Player, int nbrToSubtract);
#endif
