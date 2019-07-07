#ifndef CPU_H
#define CPU_H



/* Function prototypes */
int cpu_ai();
int cpupunch();
int cpuuse(enum stuff Item);
void cpubuy(enum stuff Item);
void cpu_mainmenu(enum mainchoice Mainchoice);
void cpu_inventorymenu(enum stuff InventoryChoice);
void cpu_shopmenu(enum stuff ShopChoice);
void clear_cpuwindow();
void cpu_pause();


#endif

