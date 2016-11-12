#ifndef MENUS_H
#define MENUS_H

#define DUMMYPTR "/dev/null"

enum mainchoice { FALCON_PUNCH, INVENTORY, SHOP, QUIT };



extern WINDOW *player_menu_win;
extern WINDOW *cpu_menu_win;

/* Function prototypes */
void checkTerminalSize();
void cpuwindow();
enum mainchoice playermainmenu();
enum mainchoice playerinventorymenu();
void playershopmenu();
void textevent(char textToPrint[]);
void print_player_titlebar();
void print_player_stocks();
void print_player_prices();
void print_cpu_titlebar();
void print_cpu_stocks();
void print_cpu_prices();

#endif
