#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include <x86intrin.h>

//#define DRAW

typedef struct Pixel
{
    Vector2 vector;
    Color   color;
} Pixel;

int main()
{
    FILE* lab_file = fopen("laba.csv", "a");
    fprintf(lab_file, "%s \n", __FILE__);
    int counter = 0;

    const int screenWidth  = 1600;
    const int screenHeight = 900;

    int screenSize = screenWidth * screenHeight;

    Color* arr_pixel = (Color*) calloc(screenSize, sizeof(Pixel));

    const float r2max = 100;
    const unsigned char nmax  = 255; 
                        
    unsigned char N = 0;

    int ix = 0, 
        iy = 0;
    
    float x_start = -2.3, 
          y_start = -0.5;

    float dx = 0.001,
          dy = 0.001;

    float ddx = dx / 10,
          ddy = dy / 10;

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
        unsigned long long int start = __rdtsc();

        ddx = dx / 10;
        ddy = dy / 10;

        y0 = y_start;

#ifdef DRAW
        if (IsKeyDown(KEY_J)) y_start += 50 * dy;
        if (IsKeyDown(KEY_L)) x_start += 50 * dx;
        if (IsKeyDown(KEY_K)) y_start -= 50 * dy;
        if (IsKeyDown(KEY_H)) x_start -= 50 * dx;

        if (IsKeyDown(KEY_Z)) dx += ddx, dy += ddy, x_start -= screenWidth / 2 * ddx, y_start -= screenHeight / 2 * ddy;
        if (IsKeyDown(KEY_X)) dx -= ddx, dy -= ddy, x_start += screenWidth / 2 * ddy, y_start += screenHeight / 2 * ddy;
#endif

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
        
        unsigned long long int end = __rdtsc();
        counter++;
        if ((100 < counter) && (counter < 1100)) fprintf(lab_file, "%ld\t", end - start);
        if (counter < 1100) printf("%ld\n", counter);
        if (counter >= 1100) printf("END!!!\n");
        
        BeginDrawing();
#ifdef DRAW
            
            ClearBackground(RAYWHITE);
            My_texture = LoadTextureFromImage(My_image);
            DrawTexture(My_texture, 0, 0, WHITE);

#endif
        EndDrawing();  
    }

    fprintf(lab_file, "\n\n\n");
    fclose(lab_file);

    CloseWindow();
    return 0;
}
