#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <menu.h>
#include "gameengine.h"
#include "menus.h"
#include "actions.h"
#include "stats.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

WINDOW *player_menu_win;  // Make player win global.
WINDOW *cpu_menu_win;  // Make cpu win global.

char *main_choices[] = {
	"Falcon Punch",
	"Inventory",
	"Shop",
	"Quit",
	(char *)NULL
};

char *inventory_choices[] = {
	"Lottery ticket",
	"Hand grenade",
	"Rocket launcher",
	"Cruise missile",
	"EMP bomb",
	"H-bomb",
	"Exit",
	(char *)NULL
};

char *shop_choices[] = {
	"Lottery ticket",
	"Hand grenade",
	"Rocket launcher",
	"Cruise missile",
	"EMP bomb",
	"H-bomb",
	"Exit",
	(char *)NULL
};

void checkTerminalSize()
{
	int maxy,maxx;
	getmaxyx(stdscr,maxy,maxx);
	if(maxy < 25 || maxx < 58){
		endwin();
		printf("\nTerminal window too small! Please enlarge.\n\n");
		exit(1);
	}
}

void cpuwindow()
{
	// Create CPU window
	cpu_menu_win = newwin( 12, 50, 0, 4);

	print_cpu_titlebar();

	// Print a border around the cpu window and print a title
	box(cpu_menu_win, 0, 0);

	if(cpustats.fastmode){  // Print fast indicator if fast mode is set.
		mvwprintw(cpu_menu_win,1,44,"fast");
		mvwchgat(cpu_menu_win,1,44,4,A_BOLD,5,NULL);
	}

	wrefresh(cpu_menu_win);
}


enum mainchoice playermainmenu()
{
	ITEM **main_items;
	MENU *main_menu;

	int c, n_choices, i;

	// Create menu items
	n_choices = ARRAY_SIZE(main_choices);
	main_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	for(i = 0; i < n_choices; ++i)
		main_items[i] = new_item(main_choices[i], main_choices[i]);

	// Create menu
	main_menu = new_menu((ITEM **)main_items);

	// Set menu option not to show the description
	menu_opts_off(main_menu, O_SHOWDESC);

	// Create player window
	player_menu_win = newwin(12, 50, 13, 4);
	keypad(player_menu_win, TRUE);

	// Set main window and menu win
	set_menu_win(main_menu, player_menu_win);
	set_menu_sub(main_menu, derwin(player_menu_win, 7, 48, 5, 1));
	set_menu_format(main_menu, 25, 25);
	set_menu_mark(main_menu, " ");
	set_current_item(main_menu, main_items[1]);

	// Print a border around the player window
	box(player_menu_win, 0, 0);

	// Print dosh, title and energy.
	print_player_titlebar(player_menu_win);

	refresh();

	// Post the menu
	post_menu(main_menu);
	wrefresh(player_menu_win);

	int main_menu_choice = 99;

	while((c = wgetch(player_menu_win)) != 27){
		switch(c){
			case KEY_DOWN:
				menu_driver(main_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(main_menu, REQ_UP_ITEM);
				break;
			case KEY_LEFT:
				menu_driver(main_menu, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(main_menu, REQ_RIGHT_ITEM);
				break;
			case 10:  // Enter
				main_menu_choice = item_index(current_item(main_menu));
				break;
			case 70:  // F
			case 102: // f
				toggle_fast_mode();  // Toggle CPU fast mode.
				break;
		}
       	wrefresh(player_menu_win);
       	if(main_menu_choice != 99)  // Exit menu instantly when choice is made.
		break;
	}
	// Unpost and free memory
	unpost_menu(main_menu);
	free_menu(main_menu);
	for(i = 0; i < n_choices; ++i)
        	free_item(main_items[i]);
	return main_menu_choice;
}


enum mainchoice playerinventorymenu()
{
	ITEM **inventory_items;
	MENU *inventory_menu;
	int c, n_choices, i;
	init_pair(1, COLOR_WHITE, COLOR_BLACK);

	// Create menu items
	n_choices = ARRAY_SIZE(inventory_choices);
	inventory_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));
	for(i = 0; i < n_choices; ++i){
		inventory_items[i] = new_item(inventory_choices[i], inventory_choices[i] );
	}

	// Create menu
	inventory_menu = new_menu((ITEM **)inventory_items);

	// Set menu option not to show the description
	menu_opts_off(inventory_menu, O_SHOWDESC);

	// Create player window
	player_menu_win = newwin(12, 50, 13, 4);
	keypad(player_menu_win, TRUE);

	// Set main window and menu win
	set_menu_win(inventory_menu, player_menu_win);
	set_menu_sub(inventory_menu, derwin(player_menu_win, 7, 48, 3, 1));
	set_menu_format(inventory_menu, 25, 1);
	set_menu_mark(inventory_menu, " ");

	// Print a border around the player window
	box(player_menu_win, 0, 0);

	// Print dosh, title and energy.
	print_player_titlebar(player_menu_win);

	refresh();

	// Post the menu
	post_menu(inventory_menu);
	print_player_stocks();
	mvwprintw(player_menu_win,5, 38, "INVENTORY");
	wrefresh(player_menu_win);

	int inventory_menu_choice = 99;

	while((c = wgetch(player_menu_win)) != 27 && inventory_menu_choice == 99){
		switch(c){
			case KEY_DOWN:
				menu_driver(inventory_menu, REQ_DOWN_ITEM);
			 	break;
			case KEY_UP:
				menu_driver(inventory_menu, REQ_UP_ITEM);
				break;
			case KEY_LEFT:
				menu_driver(inventory_menu, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(inventory_menu, REQ_RIGHT_ITEM);
				break;
			case 10:  // Enter
				inventory_menu_choice = item_index(current_item(inventory_menu));
				for(i=0;i<=5;i++){  // Menu cursor blink at choice
					set_menu_fore(inventory_menu, COLOR_PAIR(1) | A_REVERSE);
					wrefresh(player_menu_win);
					usleep(65000);
					set_menu_fore(inventory_menu, COLOR_PAIR(1) | A_NORMAL);
					wrefresh(player_menu_win);
					usleep(65000);
				}
				break;
		}
        wrefresh(player_menu_win);
        if(inventory_menu_choice != 99)  // Exit menu instantly when choice is made.
		break;
	}
	// Unpost and free memory
	unpost_menu(inventory_menu);
	free_menu(inventory_menu);
	for(i = 0; i < n_choices; ++i)
        	free_item(inventory_items[i]);
	return inventory_menu_choice;  // Return value to gameengine.
}


void playershopmenu()
{
	enum stuff Shopchoice;
	ITEM **shop_items;
	MENU *shop_menu;
	int c, n_choices, i;
	init_pair(1, COLOR_WHITE, COLOR_BLACK);

	// Create menu items
	n_choices = ARRAY_SIZE(shop_choices);
	shop_items = (ITEM **)calloc(n_choices, sizeof(ITEM *));

	for(i = 0; i < n_choices; ++i)
		shop_items[i] = new_item(shop_choices[i], shop_choices[i] );

	// Create menu
	shop_menu = new_menu((ITEM **)shop_items);

	// Set menu option not to show the description
	menu_opts_off(shop_menu, O_SHOWDESC);

	// Create player window
	player_menu_win = newwin(12, 50, 13, 4);
	keypad(player_menu_win, TRUE);

	// Set main window and menu win
	set_menu_win(shop_menu, player_menu_win);
	set_menu_sub(shop_menu, derwin(player_menu_win, 7, 48, 3, 1));
	set_menu_format(shop_menu, 25, 1);
	set_menu_mark(shop_menu, " ");

	// Print a border around the player window
	box(player_menu_win, 0, 0);

	// Print dosh, title and energy.
	print_player_titlebar(player_menu_win);

	refresh();

	// Post the menu
	post_menu(shop_menu);
	print_player_prices();
	mvwprintw(player_menu_win,5, 41, "SHOP");
	wrefresh(player_menu_win);

	while((c = wgetch(player_menu_win)) != 27 && Shopchoice != EXIT){
		switch(c){
			case KEY_DOWN:
				menu_driver(shop_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(shop_menu, REQ_UP_ITEM);
				break;
			case KEY_LEFT:
				menu_driver(shop_menu, REQ_LEFT_ITEM);
				break;
			case KEY_RIGHT:
				menu_driver(shop_menu, REQ_RIGHT_ITEM);
				break;
			case 10:  // Enter
				Shopchoice = item_index(current_item(shop_menu));

				switch(Shopchoice){
					case TICKET:
						buy_ticket(shop_menu);
						break;
					case HAND_GRENADE:
						buy_handgrenade(shop_menu);
						break;
					case PANZERFAUST:
						buy_panzerfaust(shop_menu);
						break;
					case MISSILE:
						buy_missile(shop_menu);
						break;
					case EMP_BOMB:
						buy_empbomb(shop_menu);
						break;
					case HBOMB:
						buy_hbomb(shop_menu);
						break;
					case EXIT:
						break;
				}
				wrefresh(player_menu_win);
				usleep(500000);
				set_menu_fore(shop_menu, COLOR_PAIR(0) | A_REVERSE);
				break;
			}
		wrefresh(player_menu_win);
		if(Shopchoice == EXIT)  // Exit menu instantly when choice is made.
			break;
	}
	// Unpost and free memory
	unpost_menu(shop_menu);
	free_menu(shop_menu);
	for(i = 0; i < n_choices; ++i)
		free_item(shop_items[i]);
}

void textevent(char textToPrint[])  // General routine for messagebar printing.
{
	size_t i,len;
	mvprintw(12,5,textToPrint);
	len = sizeof(&textToPrint) / sizeof(textToPrint[0]);  // Get length of char array.
	for(i=0;i<50-len;i++)  // Blank any old message.
		printw(" ");
	refresh();
}


void print_player_titlebar()
{
	mvwprintw(player_menu_win,1, 2, "$:%d",playerstats.dosh);
	mvwchgat(player_menu_win,1,2,7,A_BOLD,3,NULL);
	mvwprintw(player_menu_win,1, 20, "Player");
	mvwprintw(player_menu_win,1, 37, "Energy:%d",playerstats.energy);
	mvwchgat(player_menu_win,1,37,11,A_BOLD,4,NULL);
}

void print_player_stocks()
{
	mvwprintw(player_menu_win,3, 25, " %d",playerstats.ticket);
	mvwprintw(player_menu_win,4, 25, " %d",playerstats.hand_grenade);
	mvwprintw(player_menu_win,5, 25, " %d",playerstats.panzerfaust);
	mvwprintw(player_menu_win,6, 25, " %d",playerstats.missile);
	mvwprintw(player_menu_win,7, 25, " %d",playerstats.empbomb);
	mvwprintw(player_menu_win,8, 25, " %d",playerstats.hbomb);
}

void print_player_prices()
{
	mvwprintw(player_menu_win,3, 25, "$ %d",TICKET_PRICE);
	mvwprintw(player_menu_win,4, 25, "$ %d",HANDGRENADE_PRICE);
	mvwprintw(player_menu_win,5, 25, "$ %d",PANZERFAUST_PRICE);
	mvwprintw(player_menu_win,6, 25, "$ %d",MISSILE_PRICE);
	mvwprintw(player_menu_win,7, 25, "$ %d",EMPBOMB_PRICE);
	mvwprintw(player_menu_win,8, 25, "$ %d",HBOMB_PRICE);
}

void print_cpu_titlebar()
{
	mvwprintw(cpu_menu_win,10, 2, "$:%d",cpustats.dosh);
	mvwchgat(cpu_menu_win,10,2,7,A_BOLD,3,NULL);
	mvwprintw(cpu_menu_win,10, 21, "CPU");
	mvwprintw(cpu_menu_win,10, 37, "Energy:%d",cpustats.energy);
	mvwchgat(cpu_menu_win,10,37,11,A_BOLD,4,NULL);
}

void print_cpu_stocks()
{
	mvwprintw(cpu_menu_win,3, 25, " %d",cpustats.ticket);
	mvwprintw(cpu_menu_win,4, 25, " %d",cpustats.hand_grenade);
	mvwprintw(cpu_menu_win,5, 25, " %d",cpustats.panzerfaust);
	mvwprintw(cpu_menu_win,6, 25, " %d",cpustats.missile);
	mvwprintw(cpu_menu_win,7, 25, " %d",cpustats.empbomb);
	mvwprintw(cpu_menu_win,8, 25, " %d",cpustats.hbomb);
}

void print_cpu_prices()
{
	mvwprintw(cpu_menu_win,3, 25, "$ %d",TICKET_PRICE);
	mvwprintw(cpu_menu_win,4, 25, "$ %d",HANDGRENADE_PRICE);
	mvwprintw(cpu_menu_win,5, 25, "$ %d",PANZERFAUST_PRICE);
	mvwprintw(cpu_menu_win,6, 25, "$ %d",MISSILE_PRICE);
	mvwprintw(cpu_menu_win,7, 25, "$ %d",EMPBOMB_PRICE);
	mvwprintw(cpu_menu_win,8, 25, "$ %d",HBOMB_PRICE);
}

void toggle_fast_mode()
{
	if(!cpustats.fastmode){
		cpustats.fastmode=TRUE;
		mvwprintw(cpu_menu_win,1,44,"fast");
		mvwchgat(cpu_menu_win,1,44,4,A_BOLD,5,NULL);
		wrefresh(cpu_menu_win);
	}
	else{
		cpustats.fastmode=FALSE;
		mvwprintw(cpu_menu_win,1,44,"    ");
		wrefresh(cpu_menu_win);
	}
}
