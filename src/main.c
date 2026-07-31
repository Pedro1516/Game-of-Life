#include <raylib.h>
#include <stdlib.h>
#include "../includes/logic.h"

int main()
{
    unsigned short world1[COL][LIN] = {0};
    unsigned short world2[COL][LIN] = {0};
    unsigned short (*current)[COL] = world1;
    unsigned short (*next)[COL] = world2;

    unsigned short width = 600;
    unsigned short height = 800;
    int speed = 1;
    float timer = 0;
    State estado = PAUSED;

    InitWindow(width, height, "Game of Life");
    SetTargetFPS(60);

    world1[10][10] = 1;
    world1[11][10] = 1;
    world1[12][10] = 1;

    while (!WindowShouldClose())
    {

        if (estado == RUNNING)
        {
            timer += GetFrameTime();
            if (timer >= 1.0f / speed)
            {
                timer -= 1.0f / speed;
                update_game(current, next);

                unsigned short (*tmp)[COL] = current;
                current = next;
                next = tmp;
            }
        }
        if (estado == PAUSED)
        {
            Vector2 mousepoint = GetMousePosition();
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                current[(int)mousepoint.y / 20][(int)mousepoint.x / 20] = 1;
            if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
                current[(int)mousepoint.y / 20][(int)mousepoint.x / 20] = 0;
        }

        if (IsKeyPressed(KEY_SPACE))
            estado = (estado == PAUSED) ? RUNNING : PAUSED;

        if (IsKeyPressed(KEY_UP) && speed < 3)
            speed++;
        else if (IsKeyPressed(KEY_DOWN) && speed > 1)
            speed--;

        BeginDrawing();
        ClearBackground(BLACK);

        for (int y = 0; y < height / 20; y++)
            for (int x = 0; x < width / 20; x++)
                if (current[y][x] == 1)
                    DrawRectangle(x * 20, y * 20, 20, 20, WHITE);

        for (int i = 0; i < height / 20; i++)
        {
            DrawLine(0, i * 20, width, i * 20, (Color){127, 127, 127, 127});
        }

        for (int i = 0; i < width / 20; i++)
        {
            DrawLine(i * 20, 0, i * 20, height, (Color){255, 255, 255, 80});
        }

        DrawText(TextFormat("Speed: %dx", speed), width - MeasureText(TextFormat("Speed: %dx", speed), 20) - 10, 20, 20, WHITE);
        if (estado == PAUSED)
            DrawText("PAUSED", width - MeasureText("PAUSED", 20) - 10, 50, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}