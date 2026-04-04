#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

typedef struct Pixel
{
    Vector2 vector;
    Color   color;
} Pixel;
// avbalsk
int main()
{
    const int screenWidth  = 1600;
    const int screenHeight = 900;

    int screenSize = screenWidth * screenHeight;

    Color* arr_pixel = (Color*) calloc(screenSize, sizeof(Pixel));

    const float r2max = 100;
    const unsigned char nmax  = 255; 
                        
    unsigned char N = 0;

    int ix = 0, 
        iy = 0;
    
    float x_start = -1.2, 
          y_start = -1.5;

    float dx = 0.001,
          dy = 0.001;

    float x = 0,
          y = 0,
          x0 = 0,
          y0 = 0;

    float x2 = 0,
          y2 = 0,
          xy = 0;
    
    Texture2D My_texture = {};
    Image My_image = {};
    My_image.data = arr_pixel;
    My_image.width  = screenWidth;
    My_image.height = screenHeight;
    My_image.mipmaps = 1;
    My_image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    InitWindow(screenWidth, screenHeight, "hello");
    SetTargetFPS(60);
        
    while (!WindowShouldClose())
    {
        clock_t start = clock();

        y0 = y_start;
        if (IsKeyPressedRepeat(KEY_J)) y_start += 50 * dy;
        if (IsKeyPressedRepeat(KEY_L)) x_start += 50 * dx;
        if (IsKeyPressedRepeat(KEY_K)) y_start -= 50 * dy;
        if (IsKeyPressedRepeat(KEY_H)) x_start -= 50 * dx;

        if (IsKeyPressed(KEY_Z)) dx += 0.0001, dy += 0.0001, x_start -= 800 * 0.0001, y_start -= 450 * 0.0001;
        if (IsKeyPressed(KEY_X)) dx -= 0.0001, dy -= 0.0001, x_start += 800 * 0.0001, y_start += 450 * 0.0001;

        for (iy = 0; iy < screenHeight; iy++, y0 += dy)
        {
            x0 = x_start;
            for (ix = 0; ix < screenWidth; ix++, x0 += dx)
            {
                N = 0;
                x = x0;
                y = y0;
                for (; N != nmax; N++)
                {
                    x2 = x * x;
                    y2 = y * y;
                    xy = x * y;

                    x = x2 - y2 + x0;
                    y = xy + xy + y0;
                    
                    if (x2 + y2 > r2max)
                        break;
                }
                // if (N != 255) printf("%d\n", N);
                arr_pixel[ix + iy * screenWidth]  = {N, N, N, 255};
                if (ix == 800 && iy == 450) arr_pixel[ix + iy * screenWidth] = RED;
            }
        }
        
        clock_t end = clock();
        double time = (double) (end - start) / CLOCKS_PER_SEC;
        int fps     = (int) 1 / time;
        printf("time = %f\n", time);
        printf("fps = %d\n", fps);

        BeginDrawing();
            ClearBackground(RAYWHITE);
            My_texture = LoadTextureFromImage(My_image);
            //DrawPixel(800, 450, RED);
            DrawTexture(My_texture, 0, 0, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
