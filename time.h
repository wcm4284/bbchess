#ifndef TIME_H
#define TIME_H

// exit flag
extern bool quit;

// UCI movestogo command counter
extern int movestogo;

// UCI movetime command time counter
extern int movetime;

// UCI time command holder (ms)
extern int time;

// UCI inc command's time increment holder
extern int inc;

// UCI starttime command time holder
extern int starttime;

// UCI stoptime command time holder
extern int stoptime;

// variable to flag time control availability
extern bool timeset;

// time up flag
extern bool stopped;

int get_time_ms();
int input_waiting();
void read_input();
void communicate();

#endif