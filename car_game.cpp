#include <iostream>
#include <ctime>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <ncurses.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

using namespace std;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char car[4][4] = { ' ','|','|',' ',
                    '|','|','|','|',
                    ' ','|','|',' ',
                    '|','|','|','|' };

int carPos = WIN_WIDTH / 2;
int score = 0;

void drawBorder() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        mvaddch(i, 0, '|');
        mvaddch(i, WIN_WIDTH, '|');
    }
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        mvaddch(i, SCREEN_WIDTH, '|');
    }
}

void genEnemy(int ind) {
    enemyX[ind] = 17 + rand() % (33);
}

void drawEnemy(int ind) {
    if (enemyFlag[ind] == true) {
        mvaddstr(enemyY[ind], enemyX[ind], "****");
        mvaddstr(enemyY[ind] + 1, enemyX[ind], " ** ");
        mvaddstr(enemyY[ind] + 2, enemyX[ind], "****");
        mvaddstr(enemyY[ind] + 3, enemyX[ind], " ** ");
    }
}

void eraseEnemy(int ind) {
    if (enemyFlag[ind] == true) {
        mvaddstr(enemyY[ind], enemyX[ind], "    ");
        mvaddstr(enemyY[ind] + 1, enemyX[ind], "    ");
        mvaddstr(enemyY[ind] + 2, enemyX[ind], "    ");
        mvaddstr(enemyY[ind] + 3, enemyX[ind], "    ");
    }
}

void resetEnemy(int ind) {
    eraseEnemy(ind);
    enemyY[ind] = 1;
    genEnemy(ind);
}

void drawCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mvaddch(i + 22, j + carPos, car[i][j]);
        }
    }
}

void eraseCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            mvaddch(i + 22, j + carPos, ' ');
        }
    }
}


int collision() {
    if (enemyY[0] + 4 >= 23) {
        if (enemyX[0] + 4 - carPos >= 0 && enemyX[0] + 4 - carPos < 9) {
            return 1;
        }
    }
    return 0;
}

void gameover() {
    clear();
    mvprintw(10, 30, "--------------------------");
    mvprintw(11, 30, "-------- Game Over -------");
    mvprintw(12, 30, "--------------------------");
    mvprintw(14, 30, "Press any key to go back to menu.");
    refresh();
    getch();
}

void updateScore() {
    mvprintw(5, WIN_WIDTH + 7, "Score: %d", score);
    refresh();
}

void instructions() {
    clear();
    mvprintw(0, 30, "Instructions");
    mvprintw(1, 30, "----------------");
    mvprintw(2, 30, "Avoid Cars by moving left or right.");
    mvprintw(4, 30, "Press 'a' to move left");
    mvprintw(5, 30, "Press 'd' to move right");
    mvprintw(6, 30, "Press 'escape' to exit");
    mvprintw(8, 30, "Press any key to go back to menu.");
    refresh();
    getch();
}

void play() {
    carPos = -1 + WIN_WIDTH / 2;
    score = 0;
    enemyFlag[0] = 1;
    enemyFlag[1] = 0;
    enemyY[0] = enemyY[1] = 1;

    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);

    drawBorder();
    updateScore();
    genEnemy(0);
    genEnemy(1);

    mvprintw(2, WIN_WIDTH + 7, "Car Game");
    mvprintw(4, WIN_WIDTH + 6, "----------");
    mvprintw(6, WIN_WIDTH + 6, "----------");
    mvprintw(12, WIN_WIDTH + 7, "Control ");
    mvprintw(13, WIN_WIDTH + 7, "-------- ");
    mvprintw(14, WIN_WIDTH + 2, " A Key - Left");
    mvprintw(15, WIN_WIDTH + 2, " D Key - Right");

    mvprintw(5, 18, "Press any key to start");
    refresh();
    getch();
    mvprintw(5, 18, "                      ");
    refresh();

    while (1) {
        int ch = getch();
        if (ch == 'a' || ch == 'A') {
            if (carPos > 18)
                carPos -= 4;
        }
        if (ch == 'd' || ch == 'D') {
            if (carPos < 50)
                carPos += 4;
        }
        if (ch == 27) {
            break;
        }

        drawCar();
        drawEnemy(0);
        drawEnemy(1);
        if (collision() == 1) {
            gameover();
            endwin();
            return;
        }
        this_thread::sleep_for(chrono::milliseconds(50));
        eraseCar();
        eraseEnemy(0);
        eraseEnemy(1);

        if (enemyY[0] == 10)
            if (enemyFlag[1] == 0)
                enemyFlag[1] = 1;

        if (enemyFlag[0] == 1)
            enemyY[0] += 1;

        if (enemyFlag[1] == 1)
            enemyY[1] += 1;

        if (enemyY[0] > SCREEN_HEIGHT - 4) {
            resetEnemy(0);
            score++;
            updateScore();
        }
        if (enemyY[1] > SCREEN_HEIGHT - 4) {
            resetEnemy(1);
            score++;
            updateScore();
        }
    }


    endwin();
}

int main() {
    srand(static_cast<unsigned>(time(NULL)));

    while (1) {
        clear();
        mvprintw(5, 30, " -------------------------- ");
        mvprintw(6, 30, " |        Car Game        | ");
        mvprintw(7, 30, " --------------------------");
        mvprintw(9, 30, "1. Start Game");
        mvprintw(10, 30, "2. Instructions");
        mvprintw(11, 30, "3. Quit");
        mvprintw(13, 30, "Select option: ");
        int op = getch();

        if (op == '1')
            play();
        else if (op == '2')
            instructions();
        else if (op == '3')
            break;
    }

    return 0;
}
