#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>

#define rows 25
#define columns 80

void output(int field[rows][columns]);
void fieldInit(int field[rows][columns]);
void import(int field[rows][columns], char name[]);
int checkNb(int field[rows][columns], int i, int j);
int checkCell(int field[rows][columns], int field_next[rows][columns]);
void resetField(int field[rows][columns]);
void swap(int field[rows][columns], int field_next[rows][columns]);
int fieldDead(int field[rows][columns]);
int fieldStatic(int field[rows][columns], int field_next[rows][columns]);
void changeFPS(int *speed_koef, char key);

int main() {
    int field[rows][columns];
    int field_next[rows][columns];
    char key = ' ';
    int speed_koef = 9;

    resetField(field);
    resetField(field_next);

    initscr();
    fieldInit(field);
    noecho();
    nodelay(stdscr, TRUE);

    while (key != 033) {
        usleep(1000 * 500 / speed_koef);
        printw("Press ESC to quit | Press [1..9] to change speed. Current speed: %d\n", speed_koef);
        if (checkCell(field, field_next)) {
            output(field);
            printw("\nLIFE IS GONE! GAME OVER! <PRESS ESC TO QUIT>");
            nodelay(stdscr, FALSE);
        } else {
            output(field);
        }
        key = getch();
        changeFPS(&speed_koef, key);
        clear();
    }

    endwin();

    return 0;
}

void import(int field[rows][columns], char name[]) {
    FILE *file = fopen(name, "r");
    int i = 0;
    int j = 0;
    char c;
    while ((c = fgetc(file)) != EOF) {
        if (c == '\n') {
            i++;
            j = 0;
        } else {
            if (c == '0') {
                field[i][j] = 0;
            }
            if (c == '1') {
                field[i][j] = 1;
            }
            j++;
        }
    }
}

void fieldInit(int field[rows][columns]) {
    int choice = 0;

    printw("Input number of sample\n");

    int check = 0;

    do {
        scanw("%d", &choice);
        if (choice > 6 || choice < 1) {
            printw("No such file. Try again: ");
        } else {
            check = 1;
        }

    } while (check == 0);

    if (choice == 1) {
        char name[] = "1.txt";
        import(field, name);
    } else if (choice == 2) {
        char name[] = "2.txt";
        import(field, name);
    } else if (choice == 3) {
        char name[] = "3.txt";
        import(field, name);
    } else if (choice == 4) {
        char name[] = "4.txt";
        import(field, name);
    } else if (choice == 5) {
        char name[] = "5.txt";
        import(field, name);
    } else if (choice == 6) {
        char name[] = "6.txt";
        import(field, name);
    }
}

int checkNb(int field[rows][columns], int i, int j) {
    int nbCheck = 0;

    nbCheck += field[i - 1 < 0 ? rows - 1 : i - 1][j - 1 < 0 ? columns - 1 : j - 1];
    nbCheck += field[i - 1 < 0 ? rows - 1 : i - 1][j];
    nbCheck += field[i - 1 < 0 ? rows - 1 : i - 1][(j + 1) % columns];
    nbCheck += field[i][j - 1 < 0 ? columns - 1 : j - 1];
    nbCheck += field[i][(j + 1) % columns];
    nbCheck += field[(i + 1) % rows][j - 1 < 0 ? columns - 1 : j - 1];
    nbCheck += field[(i + 1) % rows][j];
    nbCheck += field[(i + 1) % rows][(j + 1) % columns];

    return nbCheck;
}

void output(int field[rows][columns]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            printw("%d", field[i][j]);
        }
        printw("\n");
    }
}

int checkCell(int field[rows][columns], int field_next[rows][columns]) {
    int endGame = 0;
    int nbLevel;
    resetField(field_next);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            nbLevel = checkNb(field, i, j);
            if (field[i][j] == 0 && nbLevel == 3) {
                field_next[i][j] = 1;
            } else if (field[i][j] == 1 && (nbLevel == 2 || nbLevel == 3)) {
                field_next[i][j] = 1;
            }
        }
    }
    if (fieldDead(field_next) || fieldStatic(field, field_next)) {
        endGame = 1;
    }

    swap(field, field_next);

    return endGame;
}

void resetField(int field[rows][columns]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            field[i][j] = 0;
        }
    }
}

void swap(int field[rows][columns], int field_next[rows][columns]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            field[i][j] = field_next[i][j];
        }
    }
}

int fieldDead(int field[rows][columns]) {
    int alive = 0;
    int dead = 1;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            alive += field[i][j];
        }
    }

    if (alive != 0) {
        dead = 0;
    }

    return dead;
}

int fieldStatic(int field[rows][columns], int field_next[rows][columns]) {
    int equal = 1;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if (field[i][j] != field_next[i][j]) {
                equal = 0;
            }
        }
    }

    return equal;
}

void changeFPS(int *speed_koef, char key) {
    if (key >= '1' && key <= '9') {
        switch (key) {
            case '1':
                *speed_koef = 1;
                break;
            case '2':
                *speed_koef = 2;
                break;
            case '3':
                *speed_koef = 3;
                break;
            case '4':
                *speed_koef = 4;
                break;
            case '5':
                *speed_koef = 5;
                break;
            case '6':
                *speed_koef = 6;
                break;
            case '7':
                *speed_koef = 7;
                break;
            case '8':
                *speed_koef = 8;
                break;
            case '9':
                *speed_koef = 9;
                break;
        }
    }
}
