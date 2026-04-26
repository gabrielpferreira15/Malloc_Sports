#include "raylib.h"

static bool ButtonClicked(Rectangle rect)
{
    return CheckCollisionPointRec(GetMousePosition(), rect) &&
    IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static void DrawMenuButton(Rectangle rect, const char *label, bool active)
{
    bool hover = CheckCollisionPointRec(GetMousePosition(), rect);

    Color fill = active ? (hover ? DARKBLUE : BLUE)
                        : (hover ? DARKGRAY : GRAY);

    DrawRectangleRounded(rect, 0.2f, 12, fill);
    DrawRectangleRoundedLines(rect, 0.2f, 12, RAYWHITE);

    int fontSize = 28;
    int textWidth = MeasureText(label, fontSize);

    DrawText(label,
        (int)(rect.x + rect.width/2 - textWidth/2),
        (int)(rect.y + rect.height/2 - fontSize/2 - 4),
        fontSize,
        RAYWHITE);
}

int main(void)
{
    const int screenWidth = 1280;
    const int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "Malloc Sports");
    SetTargetFPS(60);

    bool running = true;

    Rectangle btnPlay     = { screenWidth/2.0f - 180, 280, 360, 60 };
    Rectangle btnSettings = { screenWidth/2.0f - 180, 360, 360, 60 };
    Rectangle btnExit     = { screenWidth/2.0f - 180, 440, 360, 60 };

    while (running && !WindowShouldClose())
    {
        if (ButtonClicked(btnExit) || IsKeyPressed(KEY_ESCAPE))
            running = false;

        BeginDrawing();
        ClearBackground((Color){ 18, 22, 35, 255 });

        DrawText("MALLOC SPORTS",
                screenWidth/2 - MeasureText("MALLOC SPORTS", 56)/2,
                90, 56, RAYWHITE);

        DrawText("Mini games esportivos multiplayer",
            screenWidth/2 - MeasureText("Mini games esportivos multiplayer", 24)/2,
            150, 24, LIGHTGRAY);

        DrawMenuButton(btnPlay, "Jogar", false);
        DrawMenuButton(btnSettings, "Configuracoes", false);
        DrawMenuButton(btnExit, "Sair", true);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}