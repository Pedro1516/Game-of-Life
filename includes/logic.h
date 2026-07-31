#ifndef LOGIC_H
#define LOGIC_H

#define COL 1000
#define LIN 1000

typedef enum{
    PAUSED,
    RUNNING
}State;

void update_game(unsigned short current[LIN][COL], unsigned short next[LIN][COL]);

#endif