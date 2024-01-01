#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <graphics.h>
#include <time.h>
#include <Windows.h>
#include <fstream>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

using namespace std;
// Declaring the Global Variables
HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
int paddle_x, paddle_y;
int ball_x, ball_y;
int ball2_x, ball2_y;
int dx, dy;
int dx2, dy2;
int score;
int bricks[5][10];
char difficulty;
//Game is not won
bool gameWon = false;
//Setting the dimensions of bricks, slider, balls, score
void initialize() {
    paddle_x = 300;
    paddle_y = 450;
    ball_x = 300;
    ball_y = 400;
    ball2_x = 350;
    ball2_y = 400;
    dx = -2;
    dy = -2;
    dx2 = 2;
    dy2 = -2;
    score = 0;
	//Draw the bricks using array
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            bricks[i][j] = 1;
        }
    }
}
	//Setting Background colour w.r.t Difficulty level
void drawbackground() {
    int bgColor;

    switch (difficulty) {
    case '1':
        bgColor = GREEN;
        break;
    case '2':
        bgColor = YELLOW;
        break;
    case '3':
        bgColor = RED;
        break;
    case '4':
        bgColor = CYAN;
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
	// Draw background colour regarding the difficulty level
    setfillstyle(SOLID_FILL, BLACK);
    bar(paddle_x, paddle_y, paddle_x + 100, paddle_y + 10);
	// Draw the paddle as a solid filled rectangle using the specified coordinates.
    setcolor(13);
    setfillstyle(HATCH_FILL, 13);
    fillellipse(ball_x, ball_y, 10, 10);
	// Draw the first ball as a filled ellipse with a hatched pattern.
    if (difficulty == '4') {
        setcolor(RED);
        setfillstyle(HATCH_FILL, RED);
        fillellipse(ball2_x, ball2_y, 10, 10);
    }
	// Draw the second ball as a filled ellipse with a hatched pattern, but only for difficulty level 4.
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            if (bricks[i][j] == 1) {
            	// Check if the brick at the current position is active (value is 1)
                int brickColor = rand() % 15 + 1;
                // Generate a random color for the brick.
                setfillstyle(SOLID_FILL, brickColor);
                // Set the fill style to a solid color using the randomly generated color.
                bar(j * 60 + 12, i * 20 + 12, j * 60 + 57, i * 20 + 27);
                // Draw the brick as a solid filled rectangle using the calculated coordinates
            }
        }
    }
}

void input() {
	//to move the slider left and right by arrow keys
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
    if (difficulty == '2') {
        speed = 4;
    } else if (difficulty == '3') {
        speed = 7;
    } else if (difficulty == '4') {
        speed = 3;
    }

    ball_x += dx * speed;
    ball_y += dy * speed;

    if (difficulty == '4') {
        ball2_x += dx2 * speed;
        ball2_y += dy2 * speed;
    }

    if (ball_x <= 10 || ball_x >= getmaxx() - 10) {
        dx = -dx;
    }
    if (ball_y <= 10) {
        dy = -dy;
    }

//   if (ball_y >= paddle_y - 10 && ball_y <= paddle_y + 10) {
//    if (ball_x + 10 >= paddle_x && ball_x - 10 <= paddle_x + 100) {
//        dy = -dy;
//    }
//	}
	if (ball_y + 10 >= paddle_y && ball_y - 10 <= paddle_y + 10) {
    if (ball_x + 10 >= paddle_x && ball_x - 10 <= paddle_x + 100) {
        dy = -dy;
    }
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
	//this code checks for a collision between the ball and a brick
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
        outtextxy(110, 200, "Game Over! Press 'R' to Restart ");
        outtextxy(290, 240, "OR");
        outtextxy(210, 290, "Press E to exit");

        while (true) {
            if (_kbhit()) {
                char ch = _getch();
                ofstream gamefile;
                gamefile.open("Game(WAS3).txt", ios::app);

                if (ch == 'E' || ch == 'e') {
                    closegraph();
                    if (difficulty == '1') {
                        SetConsoleTextAttribute(h, 10);
                        cout << "Your score " << score << endl;
                        gamefile << "Your score " << score << endl;
                    } else if (difficulty == '2') {
                        SetConsoleTextAttribute(h, 6);
                        cout << "Your score " << score << endl;
                        gamefile << "Your score " << score << endl;
                    } else if (difficulty == '3') {
                        SetConsoleTextAttribute(h, 4);
                        cout << "Your score " << score << endl;
                        gamefile << "Your score " << score << endl;
                    } else if (difficulty =='4' ) {
                        SetConsoleTextAttribute(h, 1);
                        cout << "Your score " << score << endl;
                        gamefile << "Your score " << score << endl;
                    }
                    exit(0);
                } else if (ch == 'R' || ch == 'r') {
                    initialize(); // Restart the game
                    gameWon = false;  // Reset the gameWon variable
                    break;  // Exit the loop after restarting
                }
            }
        }
    }
}

void drawUI() {
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    setcolor(BLACK);

    string scoreText = "Score: " + to_string(score);
    outtextxy(10, 10, const_cast<char *>(scoreText.c_str()));

    string difficultyText;
    if (difficulty == '1') {
        difficultyText = "Easy";
    } else if (difficulty == '2') {
        difficultyText = "Medium";
    } else if (difficulty == '3') {
        difficultyText = "Hard";
    } else if (difficulty == '4') {
        difficultyText = "Xtreme";
    } else {
        cout << "Invalid entry";
    }

    outtextxy(getmaxx() - 100, 10, const_cast<char *>(difficultyText.c_str()));
}

int main() {
    SetConsoleTextAttribute(h, 11);
    cout << "Welcome To : " << endl;
    string var = "================================================================="
                "\n             BBBB       RRRR     IIIIIIII     CCCCC   K    K"
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
//      for (int i = 0; i < var.size(); i++) {
//        Sleep(2);
//        cout << var[i];
//    }
	int gd = DETECT, gm;
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    string username;
    ofstream gamefile;
    gamefile.open("Game(WAS3).txt", ios::app);
    SetConsoleTextAttribute(h, 13);
    cout << "Enter username please: ";
    getline(cin, username);
    cout << endl;
    gamefile << endl << username << endl;
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
    cout << "4. Extreme " << endl;
    gamefile << "1. Easy\n2. Medium\n3. Hard\n4. Extreme\n"
             << endl;
    while (true) {
        cin >> difficulty;
        gamefile << difficulty << endl;
        gamefile.close();
        if (difficulty >= '1' && difficulty <= '4') {
            break;
        } else {
            cout << "Invalid Entry. Please enter (1, 2, 3, or 4)" << endl;
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
            gameWon = true;
            outtextxy(150, 150, "Congratulations! You Won.");
            gamefile << "Congratulations! You Won the Game. " << endl;
            SetConsoleTextAttribute(h, 11);
            cout << "Your final score: " << score << endl;
            outtextxy(140, 200, " Press 'R' to Restart ");
            outtextxy(290, 240, "OR");
            outtextxy(210, 290, "Press E to exit");

            while (true) {
                if (_kbhit()) {
                    char ch = _getch();
                    ofstream gamefile;
                    gamefile.open("Game(WAS3).txt", ios::app);

                    if (ch == 'E' || ch == 'e') {
                        closegraph();
//                        if (difficulty == 1) {
//                            SetConsoleTextAttribute(h, 10);
//                            cout << "Your score " << score << endl;
//                            gamefile << "Your score " << score << endl;
//                        } else if (difficulty == 2) {
//                            SetConsoleTextAttribute(h, 6);
//                            cout << "Your score " << score << endl;
//                            gamefile << "Your score " << score << endl;
//                        } else if (difficulty == 3) {
//                            SetConsoleTextAttribute(h, 4);
//                            cout << "Your score " << score << endl;
//                            gamefile << "Your score " << score << endl;
//                        } else if (difficulty == 4) {
//                            SetConsoleTextAttribute(h, 1);
//                            cout << "Your score " << score << endl;
//                            gamefile << "Your score " << score << endl;
//                        }
//						cout<<"Your score "<<score;
                        exit(0);
                    } else if (ch == 'R' || ch == 'r') {
                        initialize();
                        gameWon = false;
                        break;
                    }
                }
            }
        }
    }

    return 0;
}


