#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include <xmmintrin.h>
#include <x86intrin.h>

// #define DRAW

int main()
{
    FILE* lab_file = fopen("laba.csv", "a");
    fprintf(lab_file, "%s \n", __FILE__);

    const int screenWidth  = 1600;
    const int screenHeight = 900;

    int screenSize = screenWidth * screenHeight;

    Color* arr_pixel = (Color*) calloc(screenSize, sizeof(Color));

    const float r2max = 100;
    const unsigned char nmax  = 255; 
                        
    __m128 N = {0}, 
           N_ITER = {0};

    char check_n = 0;

    int ix = 0, 
        iy = 0;
    
    float x_start = -2.3, 
          y_start = -0.5;

    float dx = 0.001,
          dy = 0.001;

    __m128 DX = _mm_set1_ps(dx),
           DY = _mm_set1_ps(dy);

    __m128 NUM_SEQ = {0, 1, 2, 3},
           DX_SEQ  = _mm_mul_ps(DX, NUM_SEQ), 
           DY_SEQ  = _mm_mul_ps(DY, NUM_SEQ);

    float ddx = dx / 10,
          ddy = dy / 10;

    __m128 X0 = {0},
           Y0 = {0};

    __m128 X = {0},
           Y = {0};

    __m128 X2 = {0},
           Y2 = {0},
           XY = {0};
 
    __m128 R2 = {0},
           R2_CMP = {0}, 
           R2MAX = _mm_set1_ps(r2max);

    int counter = 0;

    Texture2D My_texture = {};
    Image My_image = {};

    My_image.data = arr_pixel;
    My_image.width  = screenWidth;
    My_image.height = screenHeight;
    My_image.mipmaps = 1;
    My_image.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;

    InitWindow(screenWidth, screenHeight, "hello");
        
    while (!WindowShouldClose())
    {
        unsigned long long int start = __rdtsc();
        ddx = dx / 10;
        ddy = dy / 10;

        Y0 = _mm_set1_ps(y_start);
    #ifdef DRAW
        if (IsKeyDown(KEY_J)) y_start += 50 * dy;
        if (IsKeyDown(KEY_L)) x_start += 50 * dx;
        if (IsKeyDown(KEY_K)) y_start -= 50 * dy;
        if (IsKeyDown(KEY_H)) x_start -= 50 * dx;

        if (IsKeyDown(KEY_Z)) dx += ddx, dy += ddy, x_start -= screenWidth / 2 * ddx, y_start -= screenHeight / 2 * ddy;
        if (IsKeyDown(KEY_X)) dx -= ddx, dy -= ddy, x_start += screenWidth / 2 * ddy, y_start += screenHeight / 2 * ddy;
    #endif

        for (iy = 0; iy < screenHeight; iy++)
        {
            DY = _mm_set1_ps(dy);
            Y0 = _mm_add_ps(Y0, DY);

            DX = _mm_set1_ps(dx);
            DX_SEQ = _mm_mul_ps(DX, NUM_SEQ);
            X0 = _mm_set1_ps(x_start);
            X0 = _mm_add_ps(X0, DX_SEQ);

            for (ix = 0; ix < screenWidth; ix += 4)
            {
                X0 = _mm_add_ps(X0, _mm_mul_ps(DX, _mm_set1_ps(4)));

                X = X0;
                Y = Y0;

                check_n = 0;
                N = _mm_set1_ps(0);
                int n = 0;

                N = _mm_set1_ps(0);

                for (; n != nmax; n++) 
                {
                    X2 = _mm_mul_ps(X, X);
                    Y2 = _mm_mul_ps(Y, Y);
                    XY = _mm_mul_ps(X, Y);

                    X = _mm_add_ps(_mm_sub_ps(X2, Y2), X0);
                    Y = _mm_add_ps(_mm_add_ps(XY, XY), Y0);

                    R2 = _mm_add_ps(X2, Y2);
                    R2_CMP = _mm_cmpgt_ps(R2, R2MAX);
                    check_n = _mm_movemask_ps(R2_CMP);
                    
                    N_ITER = _mm_set1_ps(n);
                    N = _mm_or_ps(N, _mm_and_ps(N_ITER, _mm_and_ps(R2_CMP, _mm_cmpeq_ps(N, _mm_set1_ps(0)))));

                    if (check_n == 0b1111)
                        break;
                }
                // if (N != 255) printf("%d\n", N);

                for (int i = 0; i < 4; i++) {arr_pixel[ix + i + iy * screenWidth] = {N[i] + 255, N[i] + 255, N[i] + 255, 255}; }
                if (ix == screenWidth / 2 && iy == screenHeight / 2) arr_pixel[ix + iy * screenWidth] = RED;
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
            DrawText(TextFormat("FPS: %i", GetFPS()), 10, 30, 20, LIME);
        #endif
        EndDrawing();
    }
    
    fprintf(lab_file, "\n\n\n");
    fclose(lab_file);
    CloseWindow();
    return 0;
}
