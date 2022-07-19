#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <menu.h>
#include <unistd.h>
#include <time.h>
#include "gameengine.h"
#include "menus.h"
#include "actions.h"
#include "stats.h"
#include "cpu.h"


void mainloop()
{
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);

	//Create player stats
	enum mainchoice Mainchoice = INVENTORY;
	enum stuff Inventorychoice;
	int playerAttacked, cpuAttacked;

	// Randomness.
	srand(time(NULL));

	// Set initial values for both player and CPU stats.
	initialize_stats();

	//Generate CPU-window
	cpuwindow();

	while(Mainchoice != QUIT){

		// *** Player turn ***

		playerAttacked = FALSE;

		// Player game over
		if(playerstats.energy < 1)
			gameover(PLAYER);

		textevent("Player turn.");

		// Handling of player stunned behavior.
		if(playerstats.stunned>0){  // Check if stunned.
			textevent("Player is stunned.");
			playerstats.stunned--;
			sleep(1);
			playerAttacked = TRUE;  // True since being stunned is counted as an attack turn.
		}

		// Player turn until weapon is used.
		while(!playerAttacked){

			Mainchoice = playermainmenu();  // Display main menu

			switch(Mainchoice){

				case FALCON_PUNCH:  // Falcon Punch chosen
					playerAttacked = TRUE;
					use_falcon_punch();
					break;

				case INVENTORY:  // Inventory chosen
					Inventorychoice = playerinventorymenu();
					wrefresh(player_menu_win);  // Clean player inv menu from keep displaying.
					switch(Inventorychoice){
						case TICKET:
							use_ticket();
							break;
						case HAND_GRENADE:
							playerAttacked = use_handgrenade();
							break;
						case PANZERFAUST:
							playerAttacked = use_panzerfaust();
							break;
						case MISSILE:
							playerAttacked = use_missile();
							break;
						case EMP_BOMB:
							playerAttacked = use_empbomb();
							break;
						case HBOMB:
							playerAttacked = use_hbomb();
							break;
						case EXIT:
							break;
					}
					break;

				case SHOP:  // Shop chosen
					playershopmenu();  // Display shop menu
					break;

				case QUIT:
					return;

			} // switch

			box(player_menu_win, 0, 0);
			wrefresh(player_menu_win);

		} // Player turn loop


		// *** CPU turn ***

		if(cpustats.energy < 1)  // Player game over
			gameover(CPU);
		else{ // CPU good to go.
			textevent("CPU turn.");
			cpuAttacked = FALSE;
			char ch;
			nodelay(stdscr, TRUE); // Don't stop by getch().
			while(!cpuAttacked){
				cpuAttacked = cpu_ai();
				ch=getch();
				switch(ch){
					case 27:
						return;
					case 70:  // F
					case 102: // f
						toggle_fast_mode();  // Toggle CPU fast mode.
						break;
				}
			}
			nodelay(stdscr, FALSE);  // Switch to standard mode again.
		}

	} // Loop while quit isn't chosen.
}


int getRndNum(const int nmbOfTurnouts)  // Function to return a random number.
{
	int rand_nbr = (rand()%nmbOfTurnouts+1);
	return rand_nbr;
}

void gameover(const enum player Player)
{
	switch(Player){
		case PLAYER:
			textevent("Player is destroyed!");
			sleep(3);
			textevent("CPU wins.");
			sleep(3);
			endwin();
			printf("\n CPU won with %d energy left.\n\n", cpustats.energy);
			exit(0);
			break;
		case CPU:
			textevent("CPU is destroyed!");
			sleep(3);
			textevent("Player wins.");
			sleep(3);
			endwin();
			printf("\n Player won with %d energy left.\n\n", playerstats.energy);
			exit(0);
			break;
	}
}
