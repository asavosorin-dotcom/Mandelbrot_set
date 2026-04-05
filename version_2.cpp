#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

int main()
{
    const int screenWidth  = 1600;
    const int screenHeight = 900;

    int screenSize = screenWidth * screenHeight;

    Color* arr_pixel = (Color*) calloc(screenSize, sizeof(Color));

    const float r2max = 100;
    const unsigned char nmax  = 255; 
                        
    unsigned char N[4] = {0};

    char check_N[4] = {0};
    char check_n = 0;

    int ix = 0, 
        iy = 0;
    
    float x_start = -1.2, 
          y_start = -1.5;

    float dx = 0.001,
          dy = 0.001;

    float ddx = dx / 10,
          ddy = dy / 10;

    float X0[4] = {x_start, x_start + dx, x_start + dx * 2, x_start + dx * 3},
          Y0[4] = {y_start, y_start, y_start, y_start},

           X[4] = {x_start, x_start + dx, x_start + dx * 2, x_start + dx * 3},
           Y[4] = {y_start, y_start, y_start, y_start};
    
    float X2[4] = {0},
          Y2[4] = {0},
          XY[4] = {0};
    
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

        for (int i = 0; i < 4; i++) {Y0[i] = y_start;}

        if (IsKeyPressedRepeat(KEY_J)) y_start += 50 * dy;
        if (IsKeyPressedRepeat(KEY_L)) x_start += 50 * dx;
        if (IsKeyPressedRepeat(KEY_K)) y_start -= 50 * dy;
        if (IsKeyPressedRepeat(KEY_H)) x_start -= 50 * dx;

        if (IsKeyPressedRepeat(KEY_Z)) dx += ddx, dy += ddy, x_start -= screenWidth / 2 * ddx, y_start -= screenHeight / 2 * ddy;
        if (IsKeyPressedRepeat(KEY_X)) dx -= ddx, dy -= ddy, x_start += screenWidth / 2 * ddy, y_start += screenHeight / 2 * ddy;

        for (iy = 0; iy < screenHeight; iy++)
        {
            for (int i = 0; i < 4; i++) {Y0[i] += dy;}

            for (int i = 0; i < 4; i++) {X0[i] = x_start + i * dx;}

            for (ix = 0; ix < screenWidth; ix += 4)
            {
                for (int i = 0; i < 4; i++) {X0[i] += 4 * dx;}

                for (int i = 0; i < 4; i++) {X[i] = X0[i];}
                for (int i = 0; i < 4; i++) {Y[i] = Y0[i];}

                check_n = 0;
                for (int i = 0; i < 4; i++) {check_N[i] = 0;}
                for (int i = 0; i < 4; i++) {N[i] = 0;}
                int n = 0;

                for (; n != nmax; n++) // переделать N под массивы
                {
                    for (int i = 0; i < 4; i++) {X2[i] = X[i] * X[i];}
                    for (int i = 0; i < 4; i++) {Y2[i] = Y[i] * Y[i];}
                    for (int i = 0; i < 4; i++) {XY[i] = X[i] * Y[i];}

                    for (int i = 0; i < 4; i++) {X[i] = X2[i] - Y2[i] + X0[i];}
                    for (int i = 0; i < 4; i++) {Y[i] = XY[i] + XY[i] + Y0[i];}
                    
                    for (int i = 0; i < 4; i++) 
                    {
                        
                        if (X2[i] + Y2[i] > r2max && check_N[i] == 0) 
                        {
                            N[i] = n;
                            check_N[i] = 1; 
                        }
                    }

                    for (int i = 0; i < 4; i++) {if (check_N[i] == 1) check_n |= 1 << i;}
                    if (check_n == 0b1111)
                        break;
                }
                // if (N != 255) printf("%d\n", N);
                if (n == 255)
                {
                    for (int i = 0; i < 4; i++) 
                    {
                        if (N[i] == 0) N[i] = 255;
                    }
                }
                for (int i = 0; i < 4; i++) {arr_pixel[ix + i + iy * screenWidth] = {N[i], N[i], N[i], 255}; }
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
            DrawTexture(My_texture, 0, 0, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
