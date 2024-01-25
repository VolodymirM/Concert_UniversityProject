#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#define NAMES_LENGTH 26
#define DATES_LENGTH 11
#define TIMES_LENGTH 9
#define MAX_LINES 999

typedef struct Concert {
    unsigned short line_number;
    char band[NAMES_LENGTH + 1];
    char city[NAMES_LENGTH + 1];
    char date[DATES_LENGTH + 1];
    char time[TIMES_LENGTH + 1];
    double index;
} Concert;

void press_clearScreen() { // Function that asks user to press any button to clear a terminal
    printf("\nPress any key to continue.\n");
    getch();
    system("cls");
}

void clearScreen() { // Function that clears a terminal
    system("cls");
}

void filenameCheck(char *name) { // Function that allows user to input a file's name and check it for match a ".bin" format
    clearScreen();
    char files_name[100] = {};
    bool flag = true;
    while (flag) {
        memset(files_name, 0, strlen(files_name));
        // Outputting a menu
        printf("Enter binary file's (.bin) name where it is expected to store data (up to %d symbols).\n", NAMES_LENGTH - 1);
        printf("The file will be opened, if it exists in the program's folder, or created, if it is not.\n");
        printf("\nFile's name: ");
        scanf("%s", files_name);
        // Checking the entered file name to be a .bin file
        for (int i = 4; i <= NAMES_LENGTH - 1; ++i){
            if (files_name[i] == '\0' && files_name[i - 1] == 'n' && files_name[i - 2] == 'i' && files_name[i - 3] == 'b' && files_name[i - 4] == '.') {
                 flag = 0;
                break;
            }
        }
        if (files_name[NAMES_LENGTH - 1] != 0 || files_name[NAMES_LENGTH - 1] != '\0')
            flag = 1;
        if (flag == 0){
            clearScreen();
            break;
         }
        // User inputs the file name one more time if it is not a .bin file
        printf("\n!ERROR! The file name must end with \".bin\" (maybe it there is more of %d symbols). Please, enter the file name again.\n", NAMES_LENGTH - 1);
        press_clearScreen();
    }
    strcpy(name, files_name); // Copying users entered filename to a given string
}

void fileCheck(FILE* file, char files_name[]) {
    if (file == NULL) {
        printf("There was no \"%s\" file in program's folder.\n", files_name);
        file = fopen(files_name, "wb+");
        printf("\nFile \"%s\" is successfully created.\n", files_name);
        press_clearScreen();
    }
    else {
        printf("The file \"%s\" is successfully opened.\n", files_name);
        press_clearScreen();
    }
}

void readLine(Concert *table, char string[], unsigned short line_number) { // Function that allows to convert a string with each table's line to a structure
    table->line_number = line_number;
    // Reading a name of band from the string
    memset(table->band, 0, NAMES_LENGTH);
    for (int i = 3; i < 3 + NAMES_LENGTH; ++i) {
        table->band[i - 3] = string[i];
    }
    table->band[NAMES_LENGTH] = '\0';
    // Reading a city from the string
    memset(table->city, 0, NAMES_LENGTH);
    for (int i = 3 + NAMES_LENGTH; i < 3 + NAMES_LENGTH * 2; ++i) {
        table->city[i - (3 + NAMES_LENGTH)] = string[i];
    }
    table->city[NAMES_LENGTH] = '\0';
    // Reading a date from the string
    memset(table->date, 0, DATES_LENGTH);
    for (int i = 3 + NAMES_LENGTH * 2; i < 3 + NAMES_LENGTH * 2 + DATES_LENGTH; ++i) {
        table->date[i - (3 + NAMES_LENGTH * 2)] = string[i];
    }
    table->date[DATES_LENGTH] = '\0';
    // Reading a time when the concert starts from the string
    memset(table->time, 0, TIMES_LENGTH);
    for (int i = 3 + NAMES_LENGTH * 2 + DATES_LENGTH; i < 3 + NAMES_LENGTH * 2 + DATES_LENGTH + TIMES_LENGTH; ++i) {
        table->time[i - (3 + NAMES_LENGTH * 2 + DATES_LENGTH)] = string[i];
    }
    table->time[TIMES_LENGTH] = '\0';
}

void countIndex(Concert *table) { // Function that allows to count an index for a structure
    double year, month, day, hour, minute, sec;
    year = ((table->date[4] - 48) + (table->date[3] - 48) * 10 + (table->date[2] - 48) * 100 + (table->date[1] - 48) * 1000);
    month = ((table->date[7] - 48) + (table->date[6] - 48) * 10);
    day = ((table->date[10] - 48) + (table->date[9] - 48) * 10);
    hour = ((table->time[2] - 48) + (table->time[1] - 48) * 10);
    minute = ((table->time[5] - 48) + (table->time[4] - 48) * 10);
    sec = ((table->time[8] - 48) + (table->time[7] - 48) * 10);
    table->index = year * pow(10,10) + month * pow(10,8) + day*pow(10,6) + hour * pow(10,4) + minute * 100 + sec ;
}

void readTable(char files_name[], Concert table[], unsigned short *line_number) {
    FILE* fp;
    fp = fopen(files_name, "rb+");
    fileCheck(fp, files_name);
    for (int i = 0; i < MAX_LINES; ++i) {
        char string[4 + 2 * NAMES_LENGTH + DATES_LENGTH + TIMES_LENGTH];
        memset(string, 0, strlen(string));
        fgets(string, 4 + 2 * NAMES_LENGTH + DATES_LENGTH + TIMES_LENGTH, fp);
        if (string[0] == '\0') {
            break;
        }
        else {
            if (i % 2 == 0){
            readLine(&table[*line_number], string, *line_number + 1); // Reading each line from the file
            countIndex(&table[*line_number]); // Counting an index of each line
            ++(*line_number);
            }
        }
    }
    fclose(fp);
}

void saveTable(Concert table[], unsigned short line_number, char files_name[]) { // Function that rewrites a table to a document
    FILE* file;
    file = fopen(files_name, "wb");
    for (int i = 0; i < line_number; ++i) {
        fprintf(file, "%3d%s%s%s%s\n", table[i].line_number, table[i].band, table[i].city, table[i].date, table[i].time);
    }
    fclose(file);
    clearScreen();
    printf("Your table is successfully saved into \"%s\".\n", files_name);
    press_clearScreen();
}