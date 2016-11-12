#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
#include <curses.h>
#include <menu.h>
#include "menus.h"
#include "stats.h"
#include "gameengine.h"

// Create player inventory (global).
struct stats playerstats;
struct stats cpustats;

int main()
{
    // Initialization
    initscr();
    checkTerminalSize();
    curs_set(0);
	start_color();
    cbreak();
    noecho();

	//nodelay(stdscr, TRUE); // Don't stop by getch().
	keypad(stdscr, TRUE);

	refresh();  // Initial refresh before creating first window.

	// Call game engine.
	mainloop();
	
	endwin();
	printf("\n Player exited. Thanks for playing.\n\n");
	return 0;
}

