#ifndef LOGIC_H
#define LOGIC_H

#define COL 1000
#define LIN 1000
#define WORLD_WIDTH 5000
#define WORLD_HEIGHT 5000
#define CELL_SIZE 20

typedef enum
{
    PAUSED,
    RUNNING
} State;

void update_game(unsigned short current[LIN][COL], unsigned short next[LIN][COL]);

#endif