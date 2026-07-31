#include "../includes/logic.h"

#include <stdio.h>

unsigned short check_around(unsigned short world[LIN][COL], int posX, int posY)
{
    if (posX - 1 < 0 || posX + 1 >= COL || posY - 1 < 0 || posY + 1 >= LIN)
        return 0U;

    unsigned short neighbors = 0;

    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;

            if (world[posY + dy][posX + dx])
                neighbors++;
        }
    }

    if (world[posY][posX] == 1)
    {
        if (neighbors == 3 || neighbors == 2)
            return 1U;
    }
    else if (neighbors == 3)
        return 1U;

    return 0U;
}

void update_game(unsigned short current[LIN][COL], unsigned short next[LIN][COL])
{
    for (unsigned short i = 0; i < LIN; i++)
    {
        for (unsigned short j = 0; j < COL; j++)
        {
            next[i][j] = check_around(current, j, i);
        }
    }
}