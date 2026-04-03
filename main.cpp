#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

#define SIZE_X 10

typedef struct Pixel
{
    Vector2 vector;
    Color   color;
} Pixel;

int main()
{
    const int screenWidth  = 1600;
    const int screenHeight = 900;

    int screenSize = screenWidth * screenHeight;

    Pixel* arr_pixel = (Pixel*) calloc(screenSize, sizeof(Pixel));

    const float r2max = 100;
    const unsigned char nmax  = 255; 
                        
    unsigned char N = 0;

    int ix = 0, 
        iy = 0;
    
    float x_start = -1.2, 
          y_start = -1.5;

    float dx = 0.0025,
          dy = 0.0025;

    float x = 0,
          y = 0,
          x0 = 0,
          y0 = 0;

    float x2 = 0,
          y2 = 0,
          xy = 0;

    InitWindow(screenWidth, screenHeight, "hello");
    SetTargetFPS(60);
        
    while (!WindowShouldClose())
    {
        y0 = y_start;
        if (IsKeyPressed(KEY_K)) y_start += 100 * dy;
        if (IsKeyPressed(KEY_H)) x_start += 10 * dx;
        if (IsKeyPressed(KEY_J)) y_start -= 10 * dy;
        if (IsKeyPressed(KEY_L)) x_start -= 10 * dx;
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
                if (N != 255) printf("%d\n", N);
                arr_pixel[ix + iy * screenWidth].vector = {(float) ix, (float) iy};
                arr_pixel[ix + iy * screenWidth].color  = {N, N, N, 255};
            }
        }

        BeginDrawing();
            ClearBackground(RAYWHITE);

            for (int i = 0; i < screenSize; i++)
            {
                DrawPixelV(arr_pixel[i].vector, arr_pixel[i].color);
            }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
