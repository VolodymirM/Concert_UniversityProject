#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include "iodata.h"

void menuOutput() { // Function that displays information about the program an main menu
    clearScreen();
    printf("This program allows to store and edit binary (.bin) file with data about upcoming bands' concerts.\n\n");
    printf("Storage's format:\n");
    printf("Band's name - City where the concert is about to be - Date of the concert - Time when the concert is about to start\n\n");
    printf("[1] Start\n");
    printf("[2] Quit\n");
}

void printTable(Concert table[], unsigned short line_number) { // Function that displays whole table
    printf("Your table:\n\n");
    if (line_number == 0)
        printf("\t!EMPTY!\n");
    else {
        for (int i = 0; i < line_number; ++i) {
            printf("%3d%s%s%s%s\n", table[i].line_number, table[i].band, table[i].city, table[i].date, table[i].time);
        }
    }
}

void outputActions() { // Function that outputs all available actions to do with the table
        printf("\nPress a key for action:\n");
        printf("\t[1] Add a new line(The line will be chosen automatically according to concert's date and start time)\n");
        printf("\t[2] Change line's data\n");
        printf("\t[3] Delete a line\n");
        printf("\t[4] Clear the table (it will delete all lines in the table)\n");
        printf("\t[5] Save Changes\n");
        printf("\t[6] Exit\n");
        printf("\n!DO NOT FORGET TO SAVE CHANGES BEFORE LEAVING THIS PAGE. OTHERWISE YOUR TABLE WILL NOT BE CHANGED!\n");
}

char makeAction(Concert table[], unsigned short line_number) { // User inputs an action to make with the table
    char action;
    while (1) {
        printTable(table, line_number);
        outputActions();
        action = getch();
        clearScreen();
        if (action == '1' || action == '2' || action == '3' || action == '4' || action == '5' || action == '6')
            return action;
    }
}

void fullTable(Concert table[], unsigned short line_number) { // Displaying a message that the table is full
    printTable(table, line_number);
    printf("!Impossible to add a new line - the table is full. Please, delete any line of the table before adding a new one!\n");
    press_clearScreen();
}

void line_numScan(unsigned short *number, Concert table[], unsigned short line_number, char action) { // User inputs a lines' number to make action with
    char line[MAX_LINES];
    bool flag = false;
    while (1) {
        *number = MAX_LINES + 1;
        memset(line, 0, strlen(line));
        line[3] = '\0';
        printTable(table, line_number);
        if (flag) // Outputting a message that entered line does not matches the format or greater than all existing lines in the table
            printf("\n!ERROR! Incorrect format of number or the number was greater, than a number of lines existing in this table, or not positive.\n");
        if (action == '3') // Checking if the function is running for deleting a line
            printf("\nEnter a number if the line (up to %d) that you would like to delete: ", MAX_LINES);
        if (action == '2') // Checking if the function is running for changing a line
            printf("\nEnter a number if the line (up to %d) that you would like to change: ", MAX_LINES);
        scanf("%s", &line); // User enters a line's number
        clearScreen();
        if (line[3] == '\0') { // Checking if the number entered properly
            if ((line[0] < 58 && line[0] > 47) && (line[1] < 58 && line[1] > 47) && (line[2] < 58 && line[2] > 47) && (line[3] == '\0'))
                *number = ((line[2] - 48) + (line[1] - 48) * 10 + (line[0] - 48) * 100);
            if ((line[0] < 58 && line[0] > 47) && (line[1] < 58 && line[1] > 47) && (line[2] == 0) && (line[3] == '\0'))
                *number = ((line[1] - 48) + (line[0] - 48) * 10);
            if ((line[0] < 58 && line[0] > 47) && (line[1] == 0) && (line[2] == 0) && (line[3] == '\0'))
                *number = (line[0] - 48);
            if (*number <= line_number && *number > 0)
                return ;
            else { // Marking that entered line does not matches the format or greater than all existing lines in the table
                flag = true;
            }
        }
        else {
            flag = true;
        }
    }
}

bool dateCheck(char date[]) { // Function that check a date for matching date's format
    bool year = false, month = false, day = false;
    if ((date[0] > 47 && date[0] < 58) && (date[1] > 47 && date[1] < 58) && (date[2] > 47 && date[2] < 58) && (date[3] > 47 && date[3] < 58))
        year = true;
    if ((date[5] > 47 && date[5] < 58) && (date[6] > 47 && date[6] < 58)) {
        if (date[5] == '0' && date[6] != '0')
            month = true;
        if (date[5] == '1' && date[6] < 51)
            month = true;
    }
    if ((date[8] > 47 && date[8] < 58) && (date[9] > 47 && date[9] < 58)) {
        if (date[8] == '0' && date[9] != '0')
            day = true;
        if (date[8] == '1')
            day = true;
        if (date[8] == '2')
            day = true;
        if (date[8] == '3' && date[9] < 50)
            day = true;
    }
    if (year && month && day && date[4] == '-' && date[7] == '-')
        return true;
    else
        return false;
}

bool timeCheck(char time[]) { // Function that check a time for matching time's format
    bool hour = false, minute = false, second = false;
    if ((time[0] > 47 && time[0] < 58) && (time[1] > 47 && time[1] < 58)){
        if(time[0] < 50)
            hour = true;
        if(time[0] == '2' && time[1] < 52)
            hour = true;
    }
    if ((time[3] > 47 && time[3] < 58) && (time[4] > 47 && time[4] < 58)){
        if(time[3] < 54)
            minute = true;
    }
    if ((time[6] > 47 && time[6] < 58) && (time[7] > 47 && time[7] < 58)){
        if(time[6] < 54)
            second = true;
    }
    if (hour && minute && second && time[2] == ':' && time[5] == ':')
        return true;
    else
        return false;
}

void changeLines_data(Concert *table) { // Function that changes a line
    char action;
    char name[100] = {};
    char date[100] = "0000-00-00";
    char time[100] = "00:00:00";
    //memset(name, 0, strlen(name));
    name[NAMES_LENGTH - 1] = '\0';
    while (1) {
        // Outputting a menu of available line's elements to change
        printf("The line:");
        printf("%s%s%s%s\n\n", table->band, table->city, table->date, table->time);
        printf("Press:\n");
        printf("\t[1] New band's name\n");
        printf("\t[2] New the city where the band is about to have concert\n");
        printf("\t[3] New concert's date\n");
        printf("\t[4] New concert's start time\n");
        printf("\t[5] Exit\n");
        action = getch(); // User chooses the element
        clearScreen();
        switch (action)
        {
        case '1': // Changing band's name
            while (1) {
                clearScreen();
                printf("The line:");
                printf("%s%s%s%s\n\n", table->band, table->city, table->date, table->time);
                if (name[NAMES_LENGTH - 1] != '\0') {
                    printf("!ERROR! Band's name should consist of no more than %d symbols.\n\n", NAMES_LENGTH - 1);
                }
                memset(name, 0, strlen(name));
                name[NAMES_LENGTH - 1] = '\0';
                printf("New band's name: ");
                scanf(" %[^\n]s", name);
                if (name[NAMES_LENGTH - 1] == '\0') {
                    for (int i = 0; i < NAMES_LENGTH - 1; ++i) {
                        if (name[i] == '\0'){
                            for (int t = i; t < NAMES_LENGTH - 1; ++t)
                                name[t] = ' ';
                            name[NAMES_LENGTH - 1] = '\0';
                        }
                    }
                    for (int i = 0; i < NAMES_LENGTH - 1; ++i) {
                        table->band[i + 1] = name[i];
                    }
                    break;
                }
            }
            clearScreen();
            break;
        case '2': // Changing city where a concert is about to take place
            while (1) {
                clearScreen();
                printf("The line:");
                printf("%s%s%s%s\n\n", table->band, table->city, table->date, table->time);
                if (name[NAMES_LENGTH - 1] != '\0') {
                    printf("!ERROR! City's name should consist of no more than %d symbols.\n\n", NAMES_LENGTH - 1);
                }
                memset(name, 0, strlen(name));
                name[NAMES_LENGTH - 1] = '\0';
                printf("New city's name: ");
                scanf(" %[^\n]s", name);
                if (name[NAMES_LENGTH - 1] == '\0') {
                    for (int i = 0; i < NAMES_LENGTH - 1; ++i) {
                        if (name[i] == '\0'){
                            for (int t = i; t < NAMES_LENGTH - 1; ++t)
                                name[t] = ' ';
                            name[NAMES_LENGTH - 1] = '\0';
                        }
                    }
                    for (int i = 0; i < NAMES_LENGTH - 1; ++i) {
                        table->city[i + 1] = name[i];
                    }
                    break;
                }
            }
            clearScreen();
            break;
        case '3': // Changing a concert's date
            while (1) {
                clearScreen();
                printf("The line:");
                printf("%s%s%s%s\n\n", table->band, table->city, table->date, table->time);
                memset(date, 0, strlen(date));
                date[DATES_LENGTH - 1] = '\0';
                printf("New concert's date: ");
                scanf("%s", &date);
                if (dateCheck(date) && date[DATES_LENGTH - 1] == '\0') {
                    for (int i = 0; i < DATES_LENGTH - 1; ++i) {
                        table->date[i + 1] = date[i];
                    }
                    break;
                }
                else {
                    clearScreen();
                    printf("!ERROR! Wrong date's format. The date's format should be: YYYY-MM-DD.\n", NAMES_LENGTH - 1);
                    press_clearScreen();
                }
            }
            clearScreen();
            break;
        case '4': // Changing a time when a concert is about to start
            while (1) {
                clearScreen();
                printf("The line:");
                printf("%s%s%s%s\n\n", table->band, table->city, table->date, table->time);
                memset(time, 0, strlen(time));
                time[TIMES_LENGTH - 1] = '\0';
                printf("New concert's start time: ");
                scanf("%s", &time);
                if (timeCheck(time) && time[TIMES_LENGTH - 1] == '\0') {
                    for (int i = 0; i < TIMES_LENGTH - 1; ++i) {
                        table->time[i + 1] = time[i];
                    }
                    break;
                }
                else {
                    clearScreen();
                    printf("!ERROR! Wrong time's format. The time's format should be: HH:MM:SS.\n", TIMES_LENGTH - 1);
                    press_clearScreen();
                }
            }
            clearScreen();
            break;
        case '5':
            clearScreen();
            return ;
            break;
        }
    }
}

void clearLine(Concert *table) { // Function that deletes all information from a line
    table->line_number = 0;
    table->band[0] = '\0';
    table->city[0] = '\0';
    table->date[0] = '\0';
    table->time[0] = '\0';
    table->index = 0;
}

void emptyLine(Concert *table, unsigned short line) { // Function that replaces all information of a line to  space " " symbols
    table->line_number = line;
    for (int i = 0; i < NAMES_LENGTH - 1; ++i) {
        table->band[i] = ' ';
        table->city[i] = ' ';
    }
    table->band[NAMES_LENGTH - 1] = '\0';
    table->city[NAMES_LENGTH - 1] = '\0';
    for (int i = 0; i < DATES_LENGTH - 1; ++i) {
        table->date[i] = ' ';
    }
    table->date[DATES_LENGTH - 1] = '\0';
    for (int i = 0; i < TIMES_LENGTH - 1; ++i) {
        table->time[i] = ' ';
    }
    table->time[TIMES_LENGTH - 1] = '\0';
}

void swapTables(Concert *table1, Concert *table2) {
    Concert copy;
    copy = *table1;
    *table1 = *table2;
    *table2 = copy;
    unsigned short line_numberCopy = table1->line_number;
    table1->line_number = table2->line_number;
    table2->line_number = line_numberCopy;
}

void addLine(unsigned short *line_number, Concert table[]) {
    unsigned short line;
    if (!(*line_number < MAX_LINES - 1))
        fullTable(table, *line_number);
    else {
        ++(*line_number);
        line = *line_number;
        emptyLine(&table[line - 1], line);
        changeLines_data(&table[line - 1]);
        countIndex(&table[line - 1]);
        for (int i = 0; i < *line_number; ++i) {
            for (int t = 0; t < *line_number - 1; ++t) {
                if (table[t].index > table[t + 1].index) {
                    Concert copy;
                    copy = table[t];
                    table[t] = table[t + 1];
                    table[t + 1] = copy;
                    table[t].line_number = t + 1;
                    table[t + 1].line_number = t + 2;
                }
            }
        }
        clearScreen();
        printf("The line is successfully added.\n");
        press_clearScreen();                    
    }
}

void changeLine(unsigned short *line_number, Concert table[], char action) {
    unsigned short line;
    if (*line_number != 0) {
    line_numScan(&line, table, *line_number, action);
    changeLines_data(&table[line - 1]);
    countIndex(&table[line - 1]);
    for (int i = 0; i < *line_number; ++i) {
        for (int t = 0; t < *line_number - 1; ++t) {
            if (table[t].index > table[t + 1].index)
                swapTables(&table[t], &table[t + 1]);
        }
    }
    clearScreen();
    printf("The line is successfully changed.\n");
    press_clearScreen();
    }
    else {
        printf("!ERROR! There is no such line in the table to change.\n");
        press_clearScreen();
    }
}

void deleteLine(unsigned short *line_number, Concert table[], char action) {
    unsigned short line;
    if (line_number != 0) { // Checking if there are any lines in the table
        line_numScan(&line, table, *line_number, action); // User inputs a number of the line that has to be changed
        for (int i = line - 1; i < *line_number - 1; ++i) {  // Repalcing all lines that come after the one that user entered
            table[i] = table[i + 1];
            --table[i].line_number;
        }
        clearLine(&table[*line_number - 1]); // Creating an empty line
        --(*line_number);
        clearScreen();
        printf("The line is successfully deleted.\n");
        press_clearScreen();
    }
    else {
        printf("!ERROR! There is no line in the table to delete.\n");
        press_clearScreen();
    }
}

void clearTable(unsigned short *line_number, Concert table[]) {
    for (int i  = 0; i < *line_number; ++i) {
        clearLine(&table[i]);     // Deleting data from different lines separately
    }
    *line_number = 0; // Changing a number of lines to zero
    clearScreen();
    printf("Your table is successfully cleared - all information is deleted.\n");
    press_clearScreen();
}

int main()
{
    char files_name[NAMES_LENGTH];
    char action;
    while (1) {
        // Outputting main information about the program
        menuOutput();
        action = getch();
        // User enters name of the file where all the data is about to store
        switch (action)
        {
            case '1': // Running the program
            filenameCheck(files_name);
            // Opening/creating a file where data is about to store
            Concert table[MAX_LINES];
            unsigned short line_number = 0;
            // Reading a table from a file
            readTable(files_name, table, &line_number);
            while (1) {
                // Outputting the data and asking user an action to do with it
                action = makeAction(table, line_number);
                // Running user's entered action
                switch (action)
                {
                case '1':
                    // Adding a new line to a table
                    addLine(&line_number, table);
                    break;
                case '2':
                    // Changing a line of a table
                    changeLine(&line_number, table, action);
                    break;
                case '3':
                    // Deleting a line from a table
                    deleteLine(&line_number, table, action);
                    break;
                case '4':
                    // Deleting all data from a table
                    clearTable(&line_number, table);
                    break;
                case '5':
                    // Saving a changed table
                    saveTable(table, line_number, files_name);
                    break;
                }
                // Escaping a menu
                if (action == '6')
                    break;
            }
            break;
            case '2':
                // Quiting programs run
                return 0;
                break;
        }
    }
}
