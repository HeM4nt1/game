#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <mmsystem.h>

/* Global Variables for Modifications */
int score = 0;              // Modification 1: Score System
int lives = 3;              // Modification 2: Player Lives
int game_speed_ms = 120;    // Modification 3: Difficulty Levels (initial delay)

/*to remove flicker*/
void clear_screen_fast() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {0, 0};
    SetConsoleCursorPosition(h, pos);
}

// Function to handle the player losing a life or the game ending
void handle_collision_impact() {
    lives--;
    PlaySound(NULL, NULL, 0); // stop background sound
    PlaySound(TEXT("impact.wav"), NULL, SND_ASYNC);

    if (lives <= 0) {
        // --- GAME OVER ---
        Sleep(2500);
        // The loop will break because the condition is handled below
    } else {
        // --- LIFE LOST ---
        printf("\nOuch! You lost a life! Remaining: %d\n", lives);
        
        // Reset the current obstacle, but keep the player in the current lane
        // (You could also reset the player position here: x = 1;)
        
        // Pause briefly after collision animation/sound
        Sleep(1000); 
        
        // Restart the background music
        PlaySound(TEXT("bg.wav"), NULL, SND_ASYNC | SND_LOOP);
    }
}


int main() {
    system("color 4F");
    PlaySound(TEXT("bg.wav"), NULL, SND_ASYNC | SND_LOOP);
    srand(time(0));

    int x = 1;              // player position (0 to 2)
    int step = 0;           // obstacle vertical movement
    int obstaclePos = rand() % 3;    // 0,1,2 lane
    
    // The main loop condition now relies on the lives count
    while (lives > 0) {

        // ---- INPUT ----
        if (_kbhit()) {
            char ch = getch();

            if (ch == 75 && x > 0)          // LEFT arrow
                x--;

            if (ch == 77 && x < 2)          // RIGHT arrow
                x++;
        }

        // ---- DRAW ----
        clear_screen_fast();
        
        // --- Display Score and Lives (Modification 1 & 2) ---
        printf("SCORE: %d | LIVES: %d\n", score, lives);
        printf("|--- --- ---|\n");

        for (int i = 0; i < 10; i++) {
            if (i == step) {
                // Drawing the obstacle (char 1 is a smiley face)
                if (obstaclePos == 0)
                    printf("| %c      |\n", 1);
                else if (obstaclePos == 1)
                    printf("|   %c    |\n", 1);
                else if (obstaclePos == 2)
                    printf("|     %c |\n", 1);
            } else {
                printf("|          |\n");
            }
        }

        // ---- PLAYER ---- (char 6 is a spade)
        if (x == 0)
            printf("| %c        |\n", 6);
        else if (x == 1)
            printf("|   %c      |\n", 6);
        else if (x == 2)
            printf("|     %c    |\n", 6);

        // ---- COLLISION CHECK (Modified for Lives) ----
        if (step == 10 && x == obstaclePos) {
            handle_collision_impact();
        }

        // --- Difficulty Scaling (Modification 3) ---
        // Increase speed (decrease delay) based on score
        if (score > 100 && game_speed_ms == 120) {
            game_speed_ms = 90; 
            // Optional: Show message briefly
            printf("--- LEVEL UP: SPEED 2 ---\n"); 
        } else if (score > 300 && game_speed_ms == 90) {
            game_speed_ms = 60;  
            printf("--- LEVEL UP: SPEED 3 ---\n");
        }

        // Control Loop Speed
        Sleep(game_speed_ms);

        // Move obstacle down
        step++;

        // Reset when reaches bottom (SUCCESSFUL DODGE)
        if (step > 10) {
            step = 0;
            obstaclePos = rand() % 3; // new lane
            score += 10; // --- Score Increment (Modification 1) ---
        }
    } // End of while (lives > 0) loop

    // --- GAME OVER SCREEN ---
    PlaySound(NULL, NULL, 0); // Ensure all sound is off
    printf("\n\n############################\n");
    printf("#      G A M E  O V E R    #\n");
    printf("#    FINAL SCORE: %d      #\n", score);
    printf("############################\n");

    getch(); // Wait for a key press before closing
    return 0;
}