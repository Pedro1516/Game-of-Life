#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>
#include "../includes/logic.h"

void update_camera(Camera2D *camera);

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

    Camera2D cam = {
        .offset = (Vector2){GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f},
        .rotation = 0,
        .zoom = 1.0f,
        .target = (Vector2){WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0f}};

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
            Vector2 mousepoint = GetScreenToWorld2D(GetMousePosition(), cam);
            int col = (int)(mousepoint.x / 20);
            int lin = (int)(mousepoint.y / 20);

            if (!IsKeyDown(KEY_LEFT_CONTROL))
                if (lin >= 0 && lin < LIN && col >= 0 && col < COL)
                {
                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
                        current[(int)mousepoint.y / 20][(int)mousepoint.x / 20] = 1;
                    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
                        current[(int)mousepoint.y / 20][(int)mousepoint.x / 20] = 0;
                }
        }

        if (IsKeyPressed(KEY_SPACE))
            estado = (estado == PAUSED) ? RUNNING : PAUSED;

        if (IsKeyPressed(KEY_UP) && speed < 20)
            speed++;
        else if (IsKeyPressed(KEY_DOWN) && speed > 1)
            speed--;

        update_camera(&cam);

        BeginDrawing();
        BeginMode2D(cam);
        ClearBackground(BLACK);

        for (int y = 0; y < WORLD_HEIGHT / 20; y++)
            for (int x = 0; x < WORLD_WIDTH / 20; x++)
                if (current[y][x] == 1)
                    DrawRectangle(x * 20, y * 20, 20, 20, WHITE);

        for (int i = 0; i < WORLD_HEIGHT / 20; i++)
        {
            DrawLine(0, i * 20, WORLD_WIDTH, i * 20, (Color){127, 127, 127, 127});
        }

        for (int i = 0; i < WORLD_WIDTH / 20; i++)
        {
            DrawLine(i * 20, 0, i * 20, WORLD_HEIGHT, (Color){127, 127, 127, 127});
        }

        EndMode2D();

        DrawText("Press SPACE to pause", 18, height - 90, 18, (Color){200, 200, 200, 255});
        DrawText("Press UP to increase the game speed and DOWN to decrease it", 18, height - 60, 18, (Color){200, 200, 200, 255});
        DrawText("Hold LEFT CTRL to move the camera", 18, height - 30, 18, (Color){200, 200, 200, 255});
        DrawText(TextFormat("Speed: %dx", speed), width - MeasureText(TextFormat("Speed: %dx", speed), 20) - 10, 20, 20, WHITE);
        if (estado == PAUSED)
            DrawText("PAUSED", width - MeasureText("PAUSED", 20) - 10, 50, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

void update_camera(Camera2D *camera)
{

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && IsKeyDown(KEY_LEFT_CONTROL)) // Movimento da camera
    {
        Vector2 delta = GetMouseDelta();
        delta = Vector2Scale(delta, -1.0f / camera->zoom);
        camera->target = Vector2Add(camera->target, delta);
    }

    float wheel = GetMouseWheelMove();
    if (wheel != 0)
    {
        // Get the world point that is under the mouse
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);

        // Set the offset to where the mouse is
        camera->offset = GetMousePosition();

        // Set the target to match, so that the camera maps the world space point
        // under the cursor to the screen space point under the cursor at any zoom
        camera->target = mouseWorldPos;

        // Zoom increment
        // Uses log scaling to provide consistent zoom speed
        float scale = 0.2f * wheel;
        camera->zoom = Clamp(expf(logf(camera->zoom) + scale), 0.125f, 64.0f);
    }
}