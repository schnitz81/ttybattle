#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include "gameengine.h"
#include "menus.h"
#include "actions.h"
#include "stats.h"


void use_falcon_punch()
{
	int hitpoints;
	textevent("Player threw a falcon punch!");
	sleep(1);
	if(getRndNum(6)==1){  // Player misses.
		textevent("CPU blocked!");
		sleep(1);
	}
	else{  // Player doesn't miss.
		if(getRndNum(6)==6){  // Chance of critical hit.
			textevent("Critical hit!");
			move(12,19);
			sleep(1);
			hitpoints = getRndNum(100)+20 * 1.5;
		}
		else{				  // Common hit.
			hitpoints = getRndNum(100)+20;
			textevent("Hit!");
			move(12,10);
			sleep(1);
		}
		printw("CPU lost %d energy.						", hitpoints);
		refresh();
		sleep(1);
		subtract_energy(CPU, hitpoints);
		sleep(1);
	}
}

void use_ticket()
{
	if(playerstats.ticket < 1)
		textevent("No lottery ticket in inventory!");
	else{
		int winningAmount;
		playerstats.ticket--;
		/*print_player_stocks();
		wrefresh(player_menu_win);*/
		textevent("Player opened lottery ticket.");
		usleep(800000);
		
		// Generate ticket result
		if(getRndNum(4) == 3){
			winningAmount = getRndNum(MAXIMUM_WIN);
			mvprintw(12,5,"Win! Player won $%d                      ", winningAmount);
			refresh();
			add_dosh(PLAYER, winningAmount);
		}
		else{
			textevent("Blank ticket. No win.");
			sleep(1);
		}
	}
}

int use_handgrenade()
{
	int hitpoints;
	if(playerstats.hand_grenade < 1){
		textevent("No hand grenade in inventory!");
		return FALSE;
	}
	else{
		playerstats.hand_grenade--;
		/*print_player_stocks();
		wrefresh(player_menu_win);*/
		textevent("Player threw hand grenade.");
		sleep(1);
		if(getRndNum(6)==1){  // Player misses.
			textevent("Player missed!");
			sleep(1);
		}
		else{  // Player doesn't miss.
			if(getRndNum(6)==6){  // Chance of critical hit.
				textevent("Critical hit!");
				move(12,19);
				sleep(1);
				hitpoints = getRndNum(75)+75 * 1.5;
			}
			else{				  // Common hit.
				hitpoints = getRndNum(75)+75;
				textevent("Hit!");
				move(12,10);
				sleep(1);
			}
			printw("CPU lost %d energy.						", hitpoints);
			refresh();
			sleep(1);
			subtract_energy(CPU, hitpoints);
			sleep(1);
		}
		return TRUE;
	}
}

int use_panzerfaust()
{
	int hitpoints;
	if(playerstats.panzerfaust < 1){
		textevent("No rocket launcher in inventory!");
		return FALSE;
	}
	else{
		playerstats.panzerfaust--;
		/*print_player_stocks();
		wrefresh(player_menu_win);*/
		textevent("Player fired rocket launcher.");
		sleep(1);
		if(getRndNum(6)==1){  // Player misses.
			textevent("Player missed!");
			sleep(1);
		}
		else{  // Player doesn't miss.
			if(getRndNum(6)==6){  // Chance of critical hit.
				textevent("Critical hit!");
				move(12,19);
				sleep(1);
				hitpoints = getRndNum(150)+100 * 1.5;
			}
			else{				  // Common hit.
				hitpoints = getRndNum(150)+100;
				textevent("Hit!");
				move(12,10);
				sleep(1);
			}
			printw("CPU lost %d energy.						", hitpoints);
			refresh();
			sleep(1);
			subtract_energy(CPU, hitpoints);
			sleep(1);
		}
		return TRUE;
	}
	
}

int use_missile()
{
	int hitpoints;
	if(playerstats.missile < 1){
		textevent("No missile in inventory!");
		return FALSE;
	}
	else{
		playerstats.missile--;
		/*print_player_stocks();
		wrefresh(player_menu_win);*/
		textevent("Player fired missile.");
		sleep(1);
		if(getRndNum(6)==1){  // Player misses.
			textevent("Player missed!");
			sleep(1);
		}
		else{  // Player doesn't miss.
			if(getRndNum(6)==6){  // Chance of critical hit.
				textevent("Critical hit!");
				move(12,19);
				sleep(1);
				hitpoints = getRndNum(250)+100 * 1.5;
			}
			else{				  // Common hit.
				hitpoints = getRndNum(250)+100;
				textevent("Hit!");
				move(12,10);
				sleep(1);
			}
			printw("CPU lost %d energy.						", hitpoints);
			refresh();
			sleep(1);
			subtract_energy(CPU, hitpoints);
			sleep(1);
		}
		return TRUE;
	}
}

int use_empbomb()
{
	int stuntime;
	if(playerstats.empbomb < 1){
		textevent("No emp bomb in inventory!");
		return FALSE;
	}
	else{
		playerstats.empbomb--;
		/*print_player_stocks();
		wrefresh(player_menu_win);*/
		textevent("Player detonated EMP bomb.");
		sleep(1);
		if(getRndNum(6)==1){  // Player misses.
			textevent("EMP had no effect!");
			sleep(1);
		}
		else{  // Player doesn't miss.
			if(getRndNum(6)==6){  // Chance of critical hit.
				textevent("EMP critical hit!");
				move(12,23);
				sleep(1);
				stuntime = 3;
			}
			else{				  // Common hit.
				stuntime = 2;
				textevent("EMP hit!");
				move(12,14);
				sleep(1);
			}
			printw("CPU stunned for %d turns!						", stuntime);
			refresh();
			sleep(1);
			cpustats.stunned = stuntime;
		}
		return TRUE;
	}
}

int use_hbomb()
{
	int hitpoints;
	if(playerstats.hbomb < 1){
		textevent("No H-bomb in inventory!");
		return FALSE;
	}
	else{
		playerstats.hbomb--;
		/*print_player_stocks();
		wrefresh(player_menu_win);*/
		textevent("Player detonated Hbomb!");
		sleep(1);
		if(getRndNum(6)==1){  // Player misses.
			textevent("H-bomb did not detonate properly!");
			sleep(1);
		}
		else{  // Player doesn't miss.
			if(getRndNum(6)==6){  // Chance of critical hit.
				textevent("Critical hit!");
				move(12,19);
				sleep(1);
				hitpoints = getRndNum(250)+400 * 1.5;
			}
			else{				  // Common hit.
				hitpoints = getRndNum(250)+400;
				textevent("Hit!");
				move(12,10);
				sleep(1);
			}
			printw("CPU lost %d energy.						", hitpoints);
			refresh();
			sleep(1);
			subtract_energy(CPU, hitpoints);
			sleep(1);
		}
		return TRUE;
	}
}


void buy_ticket(MENU *shop_menu)
{

	if(TICKET_PRICE > playerstats.dosh){
		set_menu_fore(shop_menu, COLOR_PAIR(2) | A_REVERSE);
		textevent("Insufficient funds!");
		wrefresh(player_menu_win);
	}
	else{
		set_menu_fore(shop_menu, COLOR_PAIR(1) | A_REVERSE);
		textevent("Player bought lottery ticket.");
		refresh();
		playerstats.ticket++;
		subtract_dosh(PLAYER, TICKET_PRICE);
	}
	
}

void buy_handgrenade(MENU *shop_menu)
{
	//init_pair(1, COLOR_GREEN, COLOR_BLACK);
	//init_pair(2, COLOR_RED, COLOR_BLACK);
	if(HANDGRENADE_PRICE > playerstats.dosh){
		set_menu_fore(shop_menu, COLOR_PAIR(2) | A_REVERSE);
		textevent("Insufficient funds!");
		wrefresh(player_menu_win);
	}
	else{
		set_menu_fore(shop_menu, COLOR_PAIR(1) | A_REVERSE);
		textevent("Player bought hand grenade.");
		refresh();
		playerstats.hand_grenade++;
		subtract_dosh(PLAYER, HANDGRENADE_PRICE);
	}
}

void buy_panzerfaust(MENU *shop_menu)
{
	//init_pair(1, COLOR_GREEN, COLOR_BLACK);
	//init_pair(2, COLOR_RED, COLOR_BLACK);
	if(PANZERFAUST_PRICE > playerstats.dosh){
		set_menu_fore(shop_menu, COLOR_PAIR(2) | A_REVERSE);
		textevent("Insufficient funds!");
		wrefresh(player_menu_win);
	}
	else{
		set_menu_fore(shop_menu, COLOR_PAIR(1) | A_REVERSE);
		textevent("Player bought rocket launcher.");
		refresh();
		playerstats.panzerfaust++;
		subtract_dosh(PLAYER, PANZERFAUST_PRICE);
	}
}

void buy_missile(MENU *shop_menu)
{
	//init_pair(1, COLOR_GREEN, COLOR_BLACK);
	//init_pair(2, COLOR_RED, COLOR_BLACK);
	if(MISSILE_PRICE > playerstats.dosh){
		set_menu_fore(shop_menu, COLOR_PAIR(2) | A_REVERSE);
		textevent("Insufficient funds!");
		wrefresh(player_menu_win);
	}
	else{
		set_menu_fore(shop_menu, COLOR_PAIR(1) | A_REVERSE);
		textevent("Player bought missile.");
		refresh();
		playerstats.missile++;
		subtract_dosh(PLAYER, MISSILE_PRICE);
	}
}

void buy_empbomb(MENU *shop_menu)
{
	//init_pair(1, COLOR_GREEN, COLOR_BLACK);
	//init_pair(2, COLOR_RED, COLOR_BLACK);
	if(EMPBOMB_PRICE > playerstats.dosh){
		set_menu_fore(shop_menu, COLOR_PAIR(2) | A_REVERSE);
		textevent("Insufficient funds!");
		wrefresh(player_menu_win);
	}
	else{
		set_menu_fore(shop_menu, COLOR_PAIR(1) | A_REVERSE);
		textevent("Player bought EMP bomb.");
		refresh();
		playerstats.empbomb++;
		subtract_dosh(PLAYER, EMPBOMB_PRICE);
	}
}

void buy_hbomb(MENU *shop_menu)
{
    //init_pair(1, COLOR_GREEN, COLOR_BLACK);
	//init_pair(2, COLOR_RED, COLOR_BLACK);
	if(HBOMB_PRICE > playerstats.dosh){
		set_menu_fore(shop_menu, COLOR_PAIR(2) | A_REVERSE);
		textevent("Insufficient funds!");
		wrefresh(player_menu_win);
	}
	else{
		set_menu_fore(shop_menu, COLOR_PAIR(1) | A_REVERSE);
		textevent("Player bought H-bomb.");
		refresh();
		playerstats.hbomb++;
		subtract_dosh(PLAYER, HBOMB_PRICE);
	}
}

void initialize_stats(/*struct inventory *inv*/)
{
	playerstats.dosh = 10000;
	playerstats.energy = 1000;
	playerstats.ticket = 0;
	playerstats.hand_grenade = 0;
	playerstats.panzerfaust = 0;
	playerstats.missile = 0;
	playerstats.empbomb = 0;
	playerstats.hbomb = 0;
	playerstats.stunned = 0;
	
	cpustats.dosh = 10000;
	cpustats.energy = 1000;
	cpustats.ticket = 0;
	cpustats.hand_grenade = 0;
	cpustats.panzerfaust = 0;
	cpustats.missile = 0;
	cpustats.empbomb = 0;
	cpustats.hbomb = 0;
	cpustats.stunned = 0;
}

void add_dosh(enum player Player, int nbrToAdd)
{
	int i,sleeptime;
	if(nbrToAdd<1100)  // Different speed depending on amount.
		sleeptime = 3000;
	else if(nbrToAdd<6000)
		sleeptime = 600;
	else 
		sleeptime = 200;
	
	switch(Player){
		case PLAYER:			
			for(i=1;i<=nbrToAdd;++i){
				playerstats.dosh++;
				mvwprintw(player_menu_win,1, 2, "$:%d  ",playerstats.dosh);
				mvwchgat(player_menu_win,1,2,7,A_BOLD,5,NULL);
				wrefresh(player_menu_win);
				usleep(sleeptime);
			}
			print_player_titlebar();
			wrefresh(player_menu_win);
		break;
	
		case CPU:			
			for(i=1;i<=nbrToAdd;++i){
				cpustats.dosh++;
				mvwprintw(cpu_menu_win,10, 2, "$:%d  ",cpustats.dosh);
				mvwchgat(cpu_menu_win,10,2,7,A_BOLD,5,NULL);
				wrefresh(cpu_menu_win);
				usleep(sleeptime);
			}
			print_cpu_titlebar();
			wrefresh(cpu_menu_win);
		break;
	}  // switch
}

void subtract_dosh(enum player Player, int nbrToSubtract)
{
	//init_pair(2, COLOR_RED, COLOR_BLACK);
	//init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	int i,sleeptime;
	if(nbrToSubtract<1100)  // Different speed depending on amount.
		sleeptime = 3000;
	else if(nbrToSubtract<6000)
		sleeptime = 600;
	else 
		sleeptime = 200;
	switch(Player){
		case PLAYER:		
			for(i=nbrToSubtract;i>=1;--i){
				playerstats.dosh--;
				mvwprintw(player_menu_win,1, 2, "$:%d  ",playerstats.dosh);
				mvwchgat(player_menu_win,1,2,7,A_BOLD,2,NULL);
				wrefresh(player_menu_win);
				usleep(sleeptime);	
			}
			print_player_titlebar();
			wrefresh(player_menu_win);
			break;
		case CPU:	
			for(i=nbrToSubtract;i>=1;--i){
				cpustats.dosh--;
				mvwprintw(cpu_menu_win,10, 2, "$:%d  ",cpustats.dosh);
				mvwchgat(cpu_menu_win,10,2,7,A_BOLD,2,NULL);
				wrefresh(cpu_menu_win);
				usleep(sleeptime);	
			}
			print_cpu_titlebar();
			wrefresh(cpu_menu_win);
			break;
	}  // switch
}

void subtract_energy(enum player Player, int nbrToSubtract)
{
	int i,sleeptime;
	if(nbrToSubtract<75)  // Different speed depending on amount.
		sleeptime = 8000;
	if(nbrToSubtract<500)
		sleeptime = 5000;
	else 
		sleeptime = 2500;

	switch(Player){
		case PLAYER:		
			for(i=nbrToSubtract;i>=1;--i){
				playerstats.energy--;
				mvwprintw(player_menu_win,1, 44, "%d ",playerstats.energy);
				mvwchgat(player_menu_win,1,36,11,A_BOLD,2,NULL);
				wrefresh(player_menu_win);
				usleep(sleeptime);
			}
			print_player_titlebar();
			wrefresh(player_menu_win);
			break;
		case CPU:
			for(i=nbrToSubtract;i>=1;--i){
				cpustats.energy--;
				mvwprintw(cpu_menu_win,10, 44, "%d ",cpustats.energy);
				mvwchgat(cpu_menu_win,10,36,11,A_BOLD,2,NULL);
				wrefresh(cpu_menu_win);
				usleep(sleeptime);			
			}
			print_cpu_titlebar();
			wrefresh(cpu_menu_win);
			break;
	}  // switch
}

