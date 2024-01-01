#include <iostream>
#include <conio.h>
#include <dos.h>
#include <stdlib.h>
#include <graphics.h>
#include <time.h>
#include <Windows.h>
#include <limits>
#include <fstream>
#include <sstream>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
int paddle_x, paddle_y;
int ball_x, ball_y;
int ball2_x, ball2_y; // New variables for the second ball
int dx, dy;
int dx2, dy2; // New variables for the second ball
int score;
int bricks[5][10];
int difficulty; // Declare difficulty as a global variable

bool gameWon = false; // New variable to track if the game is won

void initialize() {
    paddle_x = 300;
    paddle_y = 450;
    ball_x = 300;
    ball_y = 400;
    ball2_x = 350; // Starting position for the second ball
    ball2_y = 400; // Starting position for the second ball
    dx = -2;
    dy = -2;
    dx2 = 2; // Initial direction for the second ball
    dy2 = -2; // Initial direction for the second ball
    score = 0;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            bricks[i][j] = 1;
        }
    }
}

void drawbackground() {
    int bgColor;

    switch (difficulty) {
    case 1:
        bgColor = GREEN;
        break;
    case 2:
        bgColor = YELLOW;
        break;
    case 3:
        bgColor = RED;
        break;
    case 4:
        bgColor = CYAN; // New color for the "Hardcore" level
        break;
    default:
        bgColor = BLACK;
        break;
    }

    setbkcolor(bgColor);
    cleardevice();
}

void draw() {
    drawbackground();

    setfillstyle(SOLID_FILL, BLACK);
    bar(paddle_x, paddle_y, paddle_x + 100, paddle_y + 10);

    setcolor(13);
    setfillstyle(HATCH_FILL, 13);
    fillellipse(ball_x, ball_y, 10, 10);

    if (difficulty == 4) // Render the second ball only for the "Hardcore" level
    {
        setcolor(RED);
        setfillstyle(HATCH_FILL, RED);
        fillellipse(ball2_x, ball2_y, 10, 10);
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            if (bricks[i][j] == 1) {
                int brickColor = rand() % 15 + 1;
                setfillstyle(SOLID_FILL, brickColor);
                bar(j * 60 + 12, i * 20 + 12, j * 60 + 57, i * 20 + 27);
            }
        }
    }
}

void input() {
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        paddle_x -= 20;
    }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        paddle_x += 20;
    }

    if (paddle_x <= 0) {
        paddle_x = 0;
    }
    if (paddle_x >= getmaxx() - 100) {
        paddle_x = getmaxx() - 100;
    }
}

void move() {
    int speed = 3;
    if (difficulty == 2) {
        speed = 4;
    } else if (difficulty == 3) { // Hardcore level also uses speed 4
        speed = 7;
    } else if (difficulty == 4) {
        speed = 4;
    }

    ball_x += dx * speed;
    ball_y += dy * speed;

    if (difficulty == 4) // Move the second ball only for the "Hardcore" level
    {
        ball2_x += dx2 * speed;
        ball2_y += dy2 * speed;
    }

    if (ball_x <= 10 || ball_x >= getmaxx() - 10) {
        dx = -dx;
    }
    if (ball_y <= 10) {
        dy = -dy;
    }

    if (ball_x >= paddle_x && ball_x <= paddle_x + 100 && ball_y >= paddle_y - 10 && ball_y <= paddle_y + 10) {
        dy = -dy;
    }

    if (ball2_x <= 10 || ball2_x >= getmaxx() - 10) {
        dx2 = -dx2;
    }
    if (ball2_y <= 10) {
        dy2 = -dy2;
    }

    if (ball2_x >= paddle_x && ball2_x <= paddle_x + 100 && ball2_y >= paddle_y - 10 && ball2_y <= paddle_y + 10) {
        dy2 = -dy2;
    }

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            if (bricks[i][j] == 1) {
                int brick_x = j * 60 + 12;
                int brick_y = i * 20 + 12;
                int brick_width = 45;
                int brick_height = 15;

                if (ball_x + 10 >= brick_x && ball_x - 10 <= brick_x + brick_width &&
                    ball_y + 10 >= brick_y && ball_y - 10 <= brick_y + brick_height) {
                    bricks[i][j] = 0;
                    dy = -dy;
                    score++;
                    Beep(1000, 100);
                }

                if (ball2_x + 10 >= brick_x && ball2_x - 10 <= brick_x + brick_width &&
                    ball2_y + 10 >= brick_y && ball2_y - 10 <= brick_y + brick_height) {
                    bricks[i][j] = 0;
                    dy2 = -dy2;
                    score++;
                    Beep(1000, 100);
                }
            }
        }
    }

    if (ball_y >= getmaxy() - 10 || ball2_y >= getmaxy() - 10) {
        outtextxy(180, 180, "Game Over! Press 'R' to Restart ");
        outtextxy(220, 220, "OR");
        outtextxy(270, 270, "Press E to exit");
        while (true) {
            if (_kbhit()) {
                char ch = _getch();
                ofstream gamefile;
                gamefile.open("Game(WAS3).txt", ios::app);
                if (ch == 'E' || ch == 'e') {
                    closegraph();
                    if (difficulty == 1) {
                        SetConsoleTextAttribute(h, 10);
                        cout << "Your score " << score << endl;
                        gamefile << "Your score " << score << endl;
                    } else if (difficulty == 2) {
                        SetConsoleTextAttribute(h, 6);
                        cout << "Your score " << score << endl;
                        gamefile << "Your score " << score << endl;
                    } else if (difficulty == 3) {
                        SetConsoleTextAttribute(h, 4);
                        cout << "Your score " << score << endl;
                        gamefile << "Your score " << score << endl;
                    } else if (difficulty == 4) {
                        SetConsoleTextAttribute(h, 1);
                        cout << "Your score " << score << endl;
                        gamefile << "Your score " << score << endl;
                    }
                    exit(0);
                } else if (ch == 'R' || ch == 'r') {
                    initialize(); // Restart the game
                    gameWon = false; // Reset the gameWon variable
                    return;
                } else {
                    cout << "Invalid input. Exiting the game." << endl;
                    gamefile << "Invalid input. Exiting the game." << endl;
                    gamefile.close();
                    closegraph();
                    exit(0);
                }
            }
        }
    }
}

void drawUI() {
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);

    string scoreText = "Score: " + to_string(score);
    outtextxy(10, 10, const_cast<char *>(scoreText.c_str()));

    string difficultyText;
    if (difficulty == 1) {
        difficultyText = "Easy";
    } else if (difficulty == 2) {
        difficultyText = "Medium";
    } else if (difficulty == 3) {
        difficultyText = "Hard";
    } else if (difficulty == 4) {
        difficultyText = "Hardcore";
    } else {
        cout << "Invalid entry";
    }

    outtextxy(getmaxx() - 100, 10, const_cast<char *>(difficultyText.c_str()));
}

int main() {
    SetConsoleTextAttribute(h, 11);
    cout << "Welcome To : " << endl;
    string var = "================================================================="
                  "\n             BBBB       RRRR     IIIIIIII     CCCCC    K    K"
                  "\n             B   BB     R   RR      II      CC        K   K"
                  "\n             B  BB      R  RR       II     CC         K  K"
                  "\n             B BB       R RR        II    CC          K K"
                  "\n             B  BB      R   R       II     CC         K  K"
                  "\n             B   BB     R    R      II      CC        K   K"
                  "\n             B  BB      R     R     II       CC       K    K"
                  "\n             BBBB       R     R  IIIIIIII     CCCCC   K      K"
                  "\n                                 BBBB       RRRR        EEEEEEE            A          K    K   EEEEEE   RRRR"
                  "\n                                 B   BB     R   RR      EE               A   A        K   K    EE       R   RR"
                  "\n                                 B  BB      R  RR       EE              A     A       K  K     EE       R  RR"
                  "\n                                 B BB       R RR        EEEEEEE        A       A      K K      EEEEE    R RR"
                  "\n                                 B  BB      R   R       EE            AAAAAAAAAAA     K  K     EE       R   R"
                  "\n                                 B   BB     R    R      EE           A           A    K   K    EE       R    R"
                  "\n                                 B  BB      R     R     EE          A             A   K    K   EE       R     R"
                  "\n                                 BBBB       R     R     EEEEEEE    A               A  K     K  EEEEEE   R      R"
                  "\n                           ============================================================================================= \n";
    for (int i = 0; i < var.size(); i++) {
        Sleep(4);
        cout << var[i];
    }
    int gd = DETECT, gm;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    string username;
    ofstream gamefile;
    gamefile.open("Game(WAS3).txt", ios::app);
    SetConsoleTextAttribute(h, 13);
    cout << "Enter username please: ";
    getline(cin, username);
    cout << endl;
    gamefile << endl
             << username << endl;
    cout << "Select Difficulty:\n";
    gamefile << "Select Difficulty:\n"
             << endl;
    SetConsoleTextAttribute(h, 10);
    cout << "1. EASY " << endl;
    SetConsoleTextAttribute(h, 6);
    cout << "2. MEDIUM " << endl;
    SetConsoleTextAttribute(h, 4);
    cout << "3. HARD " << endl;
    SetConsoleTextAttribute(h, 1);
    cout << "4. HARDCORE " << endl; // New difficulty level
    gamefile << "1. Easy\n2. Medium\n3. Hard\n4. Hardcore\n"
             << endl;

    string input;
    while (true) {
        getline(cin, input);
        stringstream ss(input);
        if (ss >> difficulty && difficulty >= 1 && difficulty <= 4 && ss.eof()) {
            break;
        } else {
            cout << "Invalid Entry. Please enter a valid numeric value (1, 2, 3, or 4)" << endl;
        }
    }

    initgraph(&gd, &gm, "");
    initialize();

    while (1) {
        draw();
        drawUI();
        move();
        input();
        delay(30);

        if (score == 50 && !gameWon) {
            gamefile.open("Game(WAS3).txt", ios::app);
            gameWon = true; // Set gameWon to true to avoid repeatedly showing the message
            outtextxy(200, 150, "Congratulations! You Won the Game. ");
            gamefile << "Congratulations! You Won the Game. " << endl;
            SetConsoleTextAttribute(h, 11);
            cout << "Your final score: " << score << endl;
            gamefile << "Your final score: " << score << endl;

            outtextxy(180, 180, "Game Over! Press 'R' to Restart ");
            outtextxy(220, 220, "OR");
            outtextxy(270, 270, "Press E to exit");
            char ch;
            cin >> ch;
            gamefile << ch << endl;
            if (ch == 'E' || ch == 'e') {
                cout << "bye" << endl;
                closegraph();
                exit(0);
            } else if (ch == 'R' || ch == 'r') {
                initialize(); // Restart the game
                gameWon = false; // Reset the gameWon variable
            }
        }
}
	return 0;
}


