#ifndef HEADER_FILE
#define HEADER_FILE

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

void press_clearScreen();
void clearScreen();
void filenameCheck(char *name);
void fileCheck(FILE* file, char files_name[]);
void readLine(Concert *table, char string[], unsigned short line_number);
void countIndex(Concert *table);
void readTable(char files_name[], Concert table[], unsigned short *line_number);
void saveTable(Concert table[], unsigned short line_number, char files_name[]);

#endif