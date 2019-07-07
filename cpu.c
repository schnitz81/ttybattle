#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include "gameengine.h"
#include "menus.h"
#include "actions.h"
#include "stats.h"
#include "cpu.h"


int cpu_ai()  // CPU choicemaking.
{
	int attackMade = FALSE;
	cpu_pause();
	
	// Main CPU logic
	
	if(cpustats.stunned>0){ 	// Check if stunned.
		textevent("CPU is stunned.");
		cpustats.stunned--;
		cpu_pause();
		attackMade = TRUE;  // True since being stunned is counted as an attack turn.
	}
	
	else if(cpustats.ticket > 0)  // If there's a lottery ticket in the inventory, open it.
		attackMade = cpuuse(TICKET);
		
	else if(cpustats.dosh >= HBOMB_PRICE) // Enough dosh for Hbomb - buy.
		cpubuy(HBOMB);
		
	else if(cpustats.hbomb > 0)  // Use H-bomb if owned.
		attackMade = cpuuse(HBOMB);
	
	else if(cpustats.dosh >= TICKET_PRICE && getRndNum(3)==1)  // Decide if buying lottery ticket.
		cpubuy(TICKET);
	
	else if(cpustats.empbomb > 0 && playerstats.stunned==0)  // Use EMP bomb if player isn't already stunned.
		attackMade = cpuuse(EMP_BOMB);
		
	else if(cpustats.dosh >= EMPBOMB_PRICE && getRndNum(2)==1 && playerstats.stunned==0)  // Buy EMP bomb by 50% chance if player isn't stunned.
		cpubuy(EMP_BOMB);
	
	else if(cpustats.missile > 0)
		attackMade = cpuuse(MISSILE);
		
	else if(cpustats.dosh >= MISSILE_PRICE) 
		cpubuy(MISSILE);
	
	else if(cpustats.panzerfaust > 0)
		attackMade = cpuuse(PANZERFAUST);
	
	else if(cpustats.dosh >= PANZERFAUST_PRICE) 	
		cpubuy(PANZERFAUST);
		
	else if(cpustats.dosh >= TICKET_PRICE && cpustats.dosh >= (TICKET_PRICE + HANDGRENADE_PRICE)) 
		cpubuy(TICKET);  // Always buy ticket if a last hand grenade can be afforded too.
	
	else if(cpustats.dosh >= TICKET_PRICE && getRndNum(2)==1) 
		cpubuy(TICKET);  // 50% chance to buy ticket even if nothing more will be afforded. 
						 
	else if(cpustats.hand_grenade > 0)
		attackMade = cpuuse(HAND_GRENADE);
		
	else if(cpustats.dosh >= HANDGRENADE_PRICE)
		cpubuy(HAND_GRENADE);
	
	else 									// Last outpost - punch.
		attackMade = cpupunch();
	
	// Return a report on if attack was made to gameengine.
	if(attackMade)
		return TRUE;
	else
		return FALSE;	
}


int cpupunch()  // CPU falcon punch.
{
	int hitpoints;
	cpu_mainmenu(FALCON_PUNCH);
	textevent("CPU threw a falcon punch!");
	cpustats.hand_grenade--;
	cpu_pause();
	if(getRndNum(6)==1){  // CPU misses.
		textevent("Player blocked!");
		cpu_pause();
	}
	else{  // CPU doesn't miss.
		if(getRndNum(6)==6){  // Chance of critical hit.
			textevent("Critical hit!");
			move(12,19);
			cpu_pause();
			hitpoints = getRndNum(100)+20 * 1.5;
		}
		else{  // Common hit.
			hitpoints = getRndNum(100)+20;
			textevent("Hit!");
			move(12,10);
			cpu_pause();
		}
		printw("Player lost %d energy.				         ", hitpoints);
		refresh();
		cpu_pause();
		subtract_energy(PLAYER, hitpoints);
		cpu_pause();
	}
	return TRUE;  // A punch is an attack.
}


int cpuuse(const enum stuff Item)  // CPU use selected item.
{
	int hitpoints, stuntime;
	switch(Item){

		case TICKET:
			cpu_mainmenu(INVENTORY);
			cpu_inventorymenu(TICKET);
			textevent("CPU opened lottery ticket.");
			cpustats.ticket--;
			cpu_pause();
			if(getRndNum(4) == 3){
				int winningAmount = getRndNum(MAXIMUM_WIN);
				mvprintw(12,5,"Win! CPU won $%d                        ", winningAmount);
				refresh();
				cpu_pause();
				add_dosh(CPU, winningAmount);
			}
			else{
				textevent("Blank ticket. No win.");
				refresh();
				cpu_pause();
			}
			return FALSE;
			
		case HAND_GRENADE:
			cpu_mainmenu(INVENTORY);
			cpu_inventorymenu(HAND_GRENADE);
			textevent("CPU threw hand grenade.");
			cpustats.hand_grenade--;
			cpu_pause();
			if(getRndNum(6)==1){  // CPU misses.
				textevent("CPU missed!");
				cpu_pause();
			}
			else{  // CPU doesn't miss.
				if(getRndNum(6)==6){  // Chance of critical hit.
					textevent("Critical hit!");
					move(12,19);
					cpu_pause();
					hitpoints = getRndNum(75)+75 * 1.5;
				}
				else{  // Common hit.
					hitpoints = getRndNum(75)+75;
					textevent("Hit!");
					move(12,10);
					cpu_pause();
				}
				printw("Player lost %d energy.				         ", hitpoints);
				refresh();
				cpu_pause();
				subtract_energy(PLAYER, hitpoints);
				cpu_pause();
			}
			return TRUE;

		case PANZERFAUST:
			cpu_mainmenu(INVENTORY);
			cpu_inventorymenu(PANZERFAUST);
			textevent("CPU fired rocket launcher.");
			cpustats.panzerfaust--;
			cpu_pause();
			if(getRndNum(6)==1){  // CPU misses.
				textevent("CPU missed!");
				cpu_pause();
			}
			else{  // CPU doesn't miss.
				if(getRndNum(6)==6){  // Chance of critical hit.
					textevent("Critical hit!");
					move(12,19);
					cpu_pause();
					hitpoints = getRndNum(150)+100 * 1.5;
				}
				else{  // Common hit.
					hitpoints = getRndNum(150)+100;
					textevent("Hit!");
					move(12,10);
					cpu_pause();
				}
				printw("Player lost %d energy.				         ", hitpoints);
				refresh();
				cpu_pause();
				subtract_energy(PLAYER, hitpoints);
				cpu_pause();
			}
			return TRUE;
			
		case MISSILE:
			cpu_mainmenu(INVENTORY);
			cpu_inventorymenu(MISSILE);
			textevent("CPU fired missile.");
			cpustats.missile--;
			cpu_pause();
			if(getRndNum(6)==1){  // CPU misses.
				textevent("CPU missed!");
				cpu_pause();
			}
			else{  // CPU doesn't miss.
				if(getRndNum(6)==6){  // Chance of critical hit.
					textevent("Critical hit!");
					move(12,19);
					cpu_pause();
					hitpoints = getRndNum(250)+100 * 1.5;
				}
				else{  // Common hit.
					hitpoints = getRndNum(250)+100;
					textevent("Hit!");
					move(12,10);
					cpu_pause();
				}
				printw("Player lost %d energy.				         ", hitpoints);
				refresh();
				cpu_pause();
				subtract_energy(PLAYER, hitpoints);
				cpu_pause();
			}
			return TRUE;			
		
		case EMP_BOMB:
			cpu_mainmenu(INVENTORY);
			cpu_inventorymenu(EMP_BOMB);
			textevent("CPU detonated EMP bomb.");
			cpustats.empbomb--;
			cpu_pause();
			if(getRndNum(6)==1){  // Player misses.
				textevent("EMP had no effect!");
				cpu_pause();
			}
			else{  // CPU doesn't miss.
				if(getRndNum(6)==6){  // Chance of critical hit.
					textevent("EMP critical hit!");
					move(12,23);
					cpu_pause();
					stuntime = 3;
				}
				else{  // Common hit.
					textevent("EMP hit!");
					move(12,14);
					stuntime = 2;
					cpu_pause();
				}
				printw("Player stunned for %d turns!	 	   	        ", stuntime);
				refresh();
				cpu_pause();
				playerstats.stunned = playerstats.stunned + stuntime;
				cpu_pause();
			}
			return TRUE;
			
		case HBOMB:
			cpu_mainmenu(INVENTORY);
			cpu_inventorymenu(HBOMB);
			textevent("CPU fired H-bomb.");
			cpustats.hbomb--;
			cpu_pause();
			if(getRndNum(6)==1){  // CPU misses.
				textevent("H-bomb did not detonate properly!");
				cpu_pause();
			}
			else{  // CPU doesn't miss.
				if(getRndNum(6)==6){  // Chance of critical hit.
					textevent("Critical hit!");
					move(12,19);
					cpu_pause();
					hitpoints = getRndNum(250)+400 * 1.5;
				}
				else{				  // Common hit.
					hitpoints = getRndNum(250)+400;
					textevent("Hit!");
					move(12,10);
					cpu_pause();
				}
				printw("Player lost %d energy.				         ", hitpoints);
				refresh();
				cpu_pause();
				subtract_energy(PLAYER, hitpoints);
				cpu_pause();
			}
			return TRUE;
		
		default:
			return FALSE;
		
	}
}


void cpubuy(const enum stuff Item)  // CPU buy selected item.
{
	switch(Item){
		case TICKET:
			cpu_mainmenu(SHOP);
			cpu_shopmenu(TICKET);
			textevent("CPU bought lottery ticket.");
			cpustats.ticket++;
			subtract_dosh(CPU,TICKET_PRICE);
			break;
		case HAND_GRENADE:
			cpu_mainmenu(SHOP);
			cpu_shopmenu(HAND_GRENADE);
			textevent("CPU bought hand grenade.");
			cpustats.hand_grenade++;
			subtract_dosh(CPU,HANDGRENADE_PRICE);
			break;
		case PANZERFAUST:
			cpu_mainmenu(SHOP);
			cpu_shopmenu(PANZERFAUST);
			textevent("CPU bought rocket launcher.");
			cpustats.panzerfaust++;
			subtract_dosh(CPU,PANZERFAUST_PRICE);
			break;
		case MISSILE:
			cpu_mainmenu(SHOP);
			cpu_shopmenu(MISSILE);
			textevent("CPU bought missile.");
			cpustats.missile++;
			subtract_dosh(CPU,MISSILE_PRICE);
			break;
		case EMP_BOMB:
			cpu_mainmenu(SHOP);
			cpu_shopmenu(EMP_BOMB);
			textevent("CPU bought EMP bomb.");
			cpustats.empbomb++;
			subtract_dosh(CPU,EMPBOMB_PRICE);
			break;
		case HBOMB:
			cpu_mainmenu(SHOP);
			cpu_shopmenu(HBOMB);
			textevent("CPU bought H-bomb.");
			cpustats.hbomb++;
			subtract_dosh(CPU,HBOMB_PRICE);
			break;
		case EXIT:
			break; // Never happens.
	}
}


void cpu_mainmenu(const enum mainchoice Mainchoice)  // CPU choose selected main category.
{
	cpuwindow();  // Redraw CPU window.
	mvwprintw(cpu_menu_win,5, 2,"Falcon Punch  Inventory     Shop ");
	wrefresh(cpu_menu_win);
	cpu_pause();
	switch(Mainchoice){
		case FALCON_PUNCH:
			mvwchgat(cpu_menu_win,5,2,12,A_STANDOUT,0,NULL);
			break;
		case INVENTORY:
			mvwchgat(cpu_menu_win,5,16,9,A_STANDOUT,0,NULL);
			break;
		case SHOP:
			mvwchgat(cpu_menu_win,5,30,4,A_STANDOUT,0,NULL);		
			break;
		case QUIT:
			break;
	}
	wrefresh(cpu_menu_win);
	cpu_pause();
	clear_cpuwindow();  // Erase all CPU menu items.
}


void cpu_inventorymenu(const enum stuff InventoryChoice)  // CPU use selected inventory item.
{
	// Print CPU inventory items.
	mvwprintw(cpu_menu_win, 5, 38,"INVENTORY");
	mvwprintw(cpu_menu_win, 3, 2,"Lottery ticket");
	mvwprintw(cpu_menu_win, 4, 2,"Hand grenade");
	mvwprintw(cpu_menu_win, 5, 2,"Rocket launcher");
	mvwprintw(cpu_menu_win, 6, 2,"Cruise missile");
	mvwprintw(cpu_menu_win, 7, 2,"EMP bomb");
	mvwprintw(cpu_menu_win, 8, 2,"H-bomb");
	print_cpu_stocks();  // Print inventory tally.
	wrefresh(cpu_menu_win);
	cpu_pause();
	switch(InventoryChoice){
		case TICKET:
			mvwchgat(cpu_menu_win,3,2,14,A_STANDOUT,0,NULL);
			break;
		case HAND_GRENADE:
			mvwchgat(cpu_menu_win,4,2,12,A_STANDOUT,0,NULL);
			break;
		case PANZERFAUST:
			mvwchgat(cpu_menu_win,5,2,15,A_STANDOUT,0,NULL);
			break;
		case MISSILE:
			mvwchgat(cpu_menu_win,6,2,14,A_STANDOUT,0,NULL);
			break;
		case EMP_BOMB:
			mvwchgat(cpu_menu_win,7,2,8,A_STANDOUT,0,NULL);
			break;
		case HBOMB:
			mvwchgat(cpu_menu_win,8,2,6,A_STANDOUT,0,NULL);
			break;
		case EXIT:  // Never happens.
			break;
	}
	wrefresh(cpu_menu_win);
	cpu_pause();
	clear_cpuwindow();  // Erase all CPU menu items.
	
}


void cpu_shopmenu(const enum stuff ShopChoice)  // CPU buy selected shop item.
{
	// Print CPU shop items.
	mvwprintw(cpu_menu_win, 5, 41, "SHOP");
	mvwprintw(cpu_menu_win, 3, 2, "Lottery ticket");
	mvwprintw(cpu_menu_win, 4, 2, "Hand grenade");
	mvwprintw(cpu_menu_win, 5, 2, "Rocket launcher");
	mvwprintw(cpu_menu_win, 6, 2, "Cruise missile");
	mvwprintw(cpu_menu_win, 7, 2, "EMP bomb");
	mvwprintw(cpu_menu_win, 8, 2, "H-bomb");
	print_cpu_prices();  // List prices.
	wrefresh(cpu_menu_win);
	cpu_pause();
	switch(ShopChoice){  // Highlight choice.
		case TICKET:
			mvwchgat(cpu_menu_win,3,2,14,A_STANDOUT,0,NULL);  
			break;
		case HAND_GRENADE:
			mvwchgat(cpu_menu_win,4,2,12,A_STANDOUT,0,NULL);
			break;
		case PANZERFAUST:
			mvwchgat(cpu_menu_win,5,2,15,A_STANDOUT,0,NULL);
			break;
		case MISSILE:
			mvwchgat(cpu_menu_win,6,2,14,A_STANDOUT,0,NULL);
			break;
		case EMP_BOMB:
			mvwchgat(cpu_menu_win,7,2,8,A_STANDOUT,0,NULL);
			break;
		case HBOMB:
			mvwchgat(cpu_menu_win,8,2,6,A_STANDOUT,0,NULL);
			break;
		case EXIT:
			break;
	}
	wrefresh(cpu_menu_win);
	cpu_pause();
	clear_cpuwindow();  // Erase all CPU menu items.
}


void clear_cpuwindow()  // Erase everyting in CPU window except menu bar and fast indicator.
{
int i,j;
	for(i=1;i<=48;i++){
		for(j=1;j<=9;j++)
			mvwprintw(cpu_menu_win,j,i," ");  // Erase character.
	}
	if(cpustats.fastmode){  // Print fast indicator if fast mode is set.
		mvwprintw(cpu_menu_win,1,44,"fast");
		mvwchgat(cpu_menu_win,1,44,4,A_BOLD,5,NULL);
	}
	wrefresh(cpu_menu_win);
}

void cpu_pause()  // CPU play pause.
{
	if(cpustats.fastmode)
		usleep(240000);
	else
		sleep(1);
}

