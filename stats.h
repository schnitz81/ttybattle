#ifndef STATS_H
#define STATS_H

struct stats{
	int dosh;
	int energy;
	int ticket;
	int hand_grenade;
	int panzerfaust;
	int missile;
	int empbomb;
	int hbomb;
	int stunned;
};

/* Make inventories global. */
extern struct stats playerstats;
extern struct stats cpustats;
















































/* Prices */
#define TICKET_PRICE 500
#define HANDGRENADE_PRICE 250
#define PANZERFAUST_PRICE 1500
#define MISSILE_PRICE 5500
#define EMPBOMB_PRICE 6000
#define HBOMB_PRICE 25000

#define MAXIMUM_WIN 20000

#endif


