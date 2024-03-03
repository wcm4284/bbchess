#ifndef TIME_H
#define TIME_H

// exit flag
bool quit = false;

// UCI movestogo command counter
int movestogo = 30;

// UCI movetime command time counter
int movetime = -1;

// UCI time command holder (ms)
int time = -1;

// UCI inc command's time increment holder
int inc = 0;

// UCI starttime command time holder
int starttime = 0;

// UCI stoptime command time holder
int stoptime = 0;

// variable to flag time control availability
bool timeset = false;

// time up flag
bool stopped = false;

int get_time_ms();
int input_waiting();
void read_input();
void communicate();

#endif