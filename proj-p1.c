#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int exist(char *filename) {
    struct stat buffer;
    return (stat (filename, &buffer) == 0);
}

int isFile(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

int isNode(const char *path) {
    if (exist(path)) return !isFile(path);
    return 0;
}

long int charnum(FILE* file) {
    fseek(file, 0, SEEK_END);
    return ftell(file);
}

char input_function_name_temp[40];
char fileinput[400];
char strinput[500];
char filecontent[6000];
char clipboard[3000];
char str1input[500];
char str2input[500];
long int newline_count;
long int* newline_count_Ptr = &newline_count;
long int newlines[1500];
long int poschar;
char file1input[400];
char file2input[400];
char filesinput[50][400];
char pos_temp[200];
int pos[2];
int fb_status;
int depth;
long int size;
int at_status;
int *at_status_Ptr = &at_status;
int at_value;
int count_status;
int *count_status_ptr = &count_status;
int byword_status;
int *byword_status_ptr = &byword_status;
int all_status;
int *all_status_ptr = &all_status;
int c_status;
int l_status;
long int findcount;
long int finds[1000][2];

int createfile(char *address) {
    int lastslash = -1, k;
    char subaddress[50];
    int length = strlen(address), tmp;

    while (1 == 1) {
        k = 0;
        tmp = lastslash;
        while (1 == 1) {
            if (lastslash < length - 1) {
                lastslash++;
                if (address[lastslash + 1] == '/') {
                    if (k != 0) break;
                }
                else if (address[lastslash + 1] != '.') {
                    k++;
                }
            }
            else break;
        }
        if (lastslash - tmp == 0) break;
        if (address[0] == '.')  {
            strncpy(subaddress, address, lastslash + 1);
            subaddress[lastslash + 1] = '\0';
        }
        else {
            subaddress[0] = '.';
            if (address[1] == '/') {
                strncpy(subaddress + 1, address, lastslash + 1);
                subaddress[lastslash + 2] = '\0';
            }
            else {
                subaddress[1] = '/';
                strncpy(subaddress + 2, address, lastslash + 1);
                subaddress[lastslash + 3] = '\0';
            }
        }

        if (exist(subaddress)) {
            if (strlen(subaddress) < length) {
                if (isNode(subaddress)) {
                    continue;
                }
                else {
                    printf("invalid address\n");
                    return 0;
                }
            }
            else {
                printf("file exists\n");
                return 0;
            }
        }
        else {
            if (strlen(subaddress) < length) {
                mkdir(subaddress);
            }
            else {
                if (fopen(subaddress, "w") != 0) return 1;
                else {
                    printf("invalid address\n");
                    return 0;
                }
            }
        }
    }
}

int cat() {
    if (exist(fileinput) && !isNode(fileinput)) {
        FILE* file = fopen(fileinput, "r+");
        char text[2000], tmp[2000];
        while (1) {
            fgets(text, 2000, file);
            if (!strcmp(text, tmp)) break;
            else printf("%s", text);
            strcpy(tmp, text);
        }
        printf("\n");
        fclose(file);
        return 1;
    }
    else {
        printf("invalid address\n");
        return 0;
    }
}

int insertstr() {
    if (exist(fileinput) && !isNode(fileinput)) {
        FILE* file = fopen(fileinput, "a");
        fprintf(file, "%s", strinput);
        fclose(file);
        return 1;
    }
    else {
        printf("invalid address\n");
        return 0;
    }
}

void filenewlines() {
    FILE *file = fopen(fileinput, "r+");
    int counter, newline_count;
    counter = 0;
    newline_count = 0;
    for (char c = getc(file); c != EOF; c = getc(file)) {
        if (c == '\n') {
            newlines[newline_count] = counter;
            newline_count++;
        }
        counter++;
    }
    fclose(file);
    (*newline_count_Ptr) = newline_count;
}

long int filetostring() {
    FILE *file = fopen(fileinput, "r+");
    int count = 0;
    for (char c = getc(file); c != EOF; c = getc(file)) {
        filecontent[count] = c;
        count++;
    }
    filecontent[count] = '\0';
    fclose(file);
    return count;
}

int filetoremovedstr() {
    long int length = filetostring(), count = 0, i = length;
    for (i = length - 1 ; count < size ; i--) {
        if (filecontent[i] != '\n') count++;
    }
    filecontent[i + 1] = '\0';
}


int postocharnum() {
    filenewlines();
    if (pos[0] > 0 && pos[0] <= newline_count) {
        if (pos[0] == 1) {
            if (pos[1] <= newlines[0]) {
                poschar = pos[1]-1;
                return 1;
            }
            else return 0;
        }
        else if (pos[1] < newlines[pos[0]-1] - newlines[pos[0]-2]) {
            poschar = (newlines[pos[0] - 2] + pos[1] + 1);
            return 1;
        }
        else return 0;
    }
    else return 0;
}

int copystr() {
    if (!exist(fileinput) || isNode(fileinput)) {
        printf("invalid address\n");
        return 0;
    }
    if (postocharnum() == 0) {
        printf("invalid position\n");
        return 0;
    }
    else {
        if (fb_status == 1) {
            if (poschar > newlines[0]) {
                if (poschar + size < strlen(filecontent) + 2) strncpy(clipboard, filecontent + poschar, size);
                else {
                    printf("invalid size, the mode is on forward\n");
                    return 0;
                }
            }
            else {
                if (poschar + size < strlen(filecontent) + 1) strncpy(clipboard, filecontent + poschar + 1, size);
                else {
                    printf("invalid size, the mode is on forward\n");
                    return 0;
                }
            }
        }
        else {
            if (poschar > newlines[0]) {
                if (poschar + size < strlen(filecontent) + 2) strncpy(clipboard, filecontent + poschar - size + 1, size);
                else {
                    printf("invalid size, the mode is on backward\n");
                    return 0;
                }
            }
            else {
                if (poschar + size < strlen(filecontent) + 1) strncpy(clipboard, filecontent + poschar - size + 2, size);
                else {
                    printf("invalid size, the mode is on backward\n");
                    return 0;
                }
            }
        }
    }
}
int filetoeditedstr(char *str) {
    long int length = filetostring(fileinput, str), count = 0, i = length;
    for (i = length - 1 ; count < size ; i--) {
        if (str[i] != '\n') count++;
    }
    str[i + 1] = '\0';
}

int removestr() {
    if (!exist(fileinput) || isNode(fileinput)) {
        printf("invalid address\n");
        return 0;
    }
    if (postocharnum() == 0) {
        printf("invalid position\n");
        return 0;
    }
    else {
        if (fb_status == 1) {
            if (poschar > newlines[0]) {
                if (poschar + size < strlen(filecontent) + 2) {
                    for (int i = poschar ; i < strlen(filecontent) - size ; i++) {
                        filecontent[i] = filecontent[i + size];
                    }
                    filecontent[strlen(filecontent) - size] = '\0';
                    FILE *file = fopen(fileinput, "w");
                    fprintf(file, filecontent);
                    fclose(file);
                }
                else {
                    printf("invalid size, the mode is on forward\n");
                    return 0;
                }
            }
            else {
                if (poschar + size < strlen(filecontent) + 1) {
                    for (int i = poschar + 1 ; i < strlen(filecontent) - size ; i++) {
                        filecontent[i] = filecontent[i + size];
                    }
                    filecontent[strlen(filecontent) - size] = '\0';
                    FILE *file = fopen(fileinput, "w");
                    fprintf(file, filecontent);
                    fclose(file);
                }
                else {
                    printf("invalid size, the mode is on forward\n");
                    return 0;
                }
            }
        }
        else {
            if (poschar > newlines[0]) {
                if (poschar + size < strlen(filecontent) + 2) {
                    for (int i = poschar - size + 1 ; i < strlen(filecontent) - size ; i++) {
                        filecontent[i] = filecontent[i + size];
                    }
                    filecontent[strlen(filecontent) - size] = '\0';
                    FILE *file = fopen(fileinput, "w");
                    fprintf(file, filecontent);
                    fclose(file);
                }
                else {
                    printf("invalid size, the mode is on backward\n");
                    return 0;
                }
            }
            else {
                if (poschar + size < strlen(filecontent) + 1) {
                    for (int i = poschar - size + 2 ; i < strlen(filecontent) - size ; i++) {
                        filecontent[i] = filecontent[i + size];
                    }
                    filecontent[strlen(filecontent) - size] = '\0';
                    FILE *file = fopen(fileinput, "w");
                    fprintf(file, filecontent);
                    fclose(file);
                }
                else {
                    printf("invalid size, the mode is on backward\n");
                    return 0;
                }
            }
        }
    }
}

int cutstr() {
    if (copystr() == 0) return 0;
    else if (removestr() == 0) return 0;
    else return 1;
}

int pastestr() {
    if (exist(fileinput) && !isNode(fileinput)) {
        if (strlen(clipboard) == 0) {
            printf("your clipboard is empty\n");
            return 0;
        }
        else {
            long int len_file = strlen(filecontent), len_clip = strlen(clipboard);
            filecontent[len_file] = 'a';
            filecontent[len_file + len_clip] = '\0';
            for (int i = len_file - 1 ; i >= poschar ; i--) filecontent[i + len_clip] = filecontent[i];
            for (int i = poschar ; i < poschar + len_clip ; i++) filecontent[i] = clipboard[i - poschar];
            FILE *file = fopen(fileinput, "w");
            fprintf(file, filecontent);
            fclose(file);
        }
    }
    else {
        printf("invalid file address\n");
        return 0;
    }
}

int chartoword(long int charnum) {
    int spacecount = 1;
    for (int i = 0 ; i <= charnum ; i++) {
        if (filecontent[i] == '\n' || filecontent[i] == ' ') spacecount++;
    }
    return spacecount;
}



int find() {
    findcount = 0;
    int k = 0;
    all_status = 0;
    at_status = 0;
    if (exist(fileinput)&& !isNode(fileinput)) {
        if (strlen(strinput) == 0) {
            printf("invalid string\n");
            return 0;
        }
        else {
            int len_str = strlen(strinput);
            long int len_file = strlen(filecontent);
            int i;
            for (i = 0 ; i < len_str && (strinput[i] != '*' ||(strinput[i-1] == '\\' && i > 0)) ; i++);
            if (i == 0) {
                for (int j = 1 ; j < len_str - 1 ; j++) {
                    if (strinput[j] == '\\' && strinput[j+1] == '*') {
                        strinput[j] = '*';
                        for (int h = j+1 ; h < len_str - 1 ; h++) strinput[h] = strinput[h+1];
                        strinput[len_str - 1] = '\0';
                        len_str = strlen(strinput);
                    }
                }
                for (int j = 0 ; j <= len_file - len_str ; j++) {
                    for (k = 0 ; k < len_str - 1 && strinput[k+1] == filecontent[j + k] ; k++);
                    if (k == len_str - 1) {
                        int t;
                        for (t = j ; t >= 0 && filecontent[t] != ' ' && filecontent[t] != '\n' ; t--);
                        finds[findcount][0] = t+1;
                        finds[findcount][1] = j + len_str - 2;
                        findcount++;
                    }
                }
            }
            else if (i == len_str - 1) {
                for (int j = 0 ; j < len_str - 2 ; j++) {
                    if (strinput[j] == '\\' && strinput[j+1] == '*') {
                        strinput[j] = '*';
                        for (int h = j+1 ; h < len_str - 1 ; h++) strinput[h] = strinput[h+1];
                        strinput[len_str - 1] = '\0';
                        len_str = strlen(strinput);
                    }
                }
                for (int j = 0 ; j <= len_file - len_str ; j++) {
                    for (k = 0 ; k < len_str - 1 && strinput[k] == filecontent[j + k] ; k++);
                    if (k == len_str - 1) {
                        int t;
                        for (t = j + len_str - 2 ; t < len_file && filecontent[t] != ' ' && filecontent[t] != '\n' ; t++);
                        finds[findcount][0] = j;
                        finds[findcount][1] = t - 1;
                        findcount++;
                    }
                }
            }
            else if (i != len_str) {
                for (int j = 0 ; j < len_str - 1 ; j++) {
                    if (strinput[j] == '\\' && strinput[j+1] == '*') {
                        strinput[j] = '*';
                        for (int h = j+1 ; h < len_str - 1 ; h++) strinput[h] = strinput[h+1];
                        strinput[len_str - 1] = '\0';
                        len_str = strlen(strinput);
                        if (j < i) i--;
                    }
                }
                for (int j = 0 ; j <= len_file - len_str + 1 ; j++) {
                    for (k = 0 ; k < i && strinput[k] == filecontent[j + k] ; k++);
                    if (k == i) {
                        int p;
                        for (p = j + i ; p <= len_file - len_str + i ; p++) {
                            if (filecontent[p] == '\n' || filecontent[p] == ' ') break;
                            int l;
                            for (l = p ; strinput[l - p + i + 1] == filecontent[l] ; l++);
                            if (l == p + len_str - i - 1) {
                                finds[findcount][0] = j;
                                finds[findcount][1] = p + len_str - i - 2;
                                findcount++;
                            }
                        }
                    }
                }
            }
            else {
                for (int j = 0 ; j < len_str - 1 ; j++) {
                    if (strinput[j] == '\\' && strinput[j+1] == '*') {
                        strinput[j] = '*';
                        for (int h = j+1 ; h < len_str - 1 ; h++) strinput[h] = strinput[h+1];
                        strinput[len_str - 1] = '\0';
                        len_str = strlen(strinput);
                    }
                }
                for (int j = 0 ; j <= len_file - len_str ; j++) {
                    for (k = 0 ; k < len_str && strinput[k] == filecontent[j + k] ; k++);
                    if (k == len_str) {
                        finds[findcount][0] = j;
                        finds[findcount][1] = j + len_str - 1;
                        findcount++;
                    }
                }
            }

            if (count_status == 0) {
                if (at_status == 0) {
                    if (byword_status == 0) {
                        if (all_status == 0) {
                            if (findcount == 0) {
                                printf("-1\n");
                                return 0;
                            }
                            else printf("%d\n", finds[0][0]);
                        }
                        else if (all_status == 1) {
                            for (int i = 0 ; i < findcount - 1 ; i++) printf("%d, ", chartoword(finds[i][0]));
                            printf("%d\n", chartoword(finds[findcount - 1][0]));
                        }
                    }
                    else if (byword_status == 1) {
                        if (all_status == 0) {
                            printf("%d\n" ,chartoword(finds[0][0]));
                        }
                        else if (all_status == 1) {
                            if (findcount == 0) {
                                printf("-1\n");
                                return 0;
                            }
                            for (int i = 0 ; i < findcount - 1 ; i++) printf("%d, ", chartoword(finds[i][0]));
                            printf("%d\n", chartoword(finds[findcount - 1][0]));
                        }
                    }
                }
                else if (at_status == 1) {
                    if (byword_status == 0) {
                        if (all_status == 0) {
                            if (at_value > findcount) {
                                printf("-1\n");
                                return 0;
                            }
                            else printf("%d\n", finds[at_value - 1][0]);
                        }
                        else if (all_status == 1) {
                            printf("at and all can't come together\n");
                            return 0;
                        }
                    }
                    else if (byword_status == 1) {
                        if (all_status == 0) {
                            if (at_value > findcount) {
                                printf("-1\n");
                                return 0;
                            }
                            else printf("%d\n",chartoword(finds[at_value - 1][0]));
                        }
                        else if (all_status == 1) {
                            if (findcount == 0) {
                                printf("-1\n");
                                return 0;
                            }
                            for (int i = 0 ; i < findcount - 1 ; i++) printf("%d, ", chartoword(finds[i][0]));
                            printf("%d\n", chartoword(finds[findcount - 1][0]));
                            printf("info given by all contains info given by byword and at\n");
                        }
                    }
                }
            }
            else if (count_status == 1) {
                if (at_status == 0 && byword_status == 0 && all_status == 0) printf("%d\n", findcount);
                else {
                    printf("count can't come with other options\n");
                    return 0;
                }
            }

        }
    }
}

int replace() {
    findcount = 0;
    int k = 0;
    all_status = 0;
    at_status = 0;
    if (exist(fileinput)&& !isNode(fileinput)) {
        if (strlen(str1input) == 0) {
            printf("invalid string\n");
            return 0;
        }
        else {
            int len_str = strlen(str1input);
            long int len_file = strlen(filecontent);
            int i;
            for (i = 0 ; i < len_str && (str1input[i] != '*' ||(str1input[i-1] == '\\' && i > 0)) ; i++);
            if (i == 0) {
                for (int j = 1 ; j < len_str - 1 ; j++) {
                    if (str1input[j] == '\\' && str1input[j+1] == '*') {
                        str1input[j] = '*';
                        for (int h = j+1 ; h < len_str - 1 ; h++) str1input[h] = str1input[h+1];
                        str1input[len_str - 1] = '\0';
                        len_str = strlen(str1input);
                    }
                }
                for (int j = 0 ; j <= len_file - len_str ; j++) {
                    for (k = 0 ; k < len_str - 1 && str1input[k+1] == filecontent[j + k] ; k++);
                    if (k == len_str - 1) {
                        int t;
                        for (t = j ; t >= 0 && filecontent[t] != ' ' && filecontent[t] != '\n' ; t--);
                        finds[findcount][0] = t+1;
                        finds[findcount][1] = j + len_str - 2;
                        findcount++;
                    }
                }
            }
            else if (i == len_str - 1) {
                for (int j = 0 ; j < len_str - 2 ; j++) {
                    if (str1input[j] == '\\' && str1input[j+1] == '*') {
                        str1input[j] = '*';
                        for (int h = j+1 ; h < len_str - 1 ; h++) str1input[h] = str1input[h+1];
                        str1input[len_str - 1] = '\0';
                        len_str = strlen(str1input);
                    }
                }
                for (int j = 0 ; j <= len_file - len_str ; j++) {
                    for (k = 0 ; k < len_str - 1 && str1input[k] == filecontent[j + k] ; k++);
                    if (k == len_str - 1) {
                        int t;
                        for (t = j + len_str - 2 ; t < len_file && filecontent[t] != ' ' && filecontent[t] != '\n' ; t++);
                        finds[findcount][0] = j;
                        finds[findcount][1] = t - 1;
                        findcount++;
                    }
                }
            }
            else if (i != len_str) {
                for (int j = 0 ; j < len_str - 1 ; j++) {
                    if (str1input[j] == '\\' && str1input[j+1] == '*') {
                        str1input[j] = '*';
                        for (int h = j+1 ; h < len_str - 1 ; h++) str1input[h] = str1input[h+1];
                        str1input[len_str - 1] = '\0';
                        len_str = strlen(str1input);
                        if (j < i) i--;
                    }
                }
                for (int j = 0 ; j <= len_file - len_str + 1 ; j++) {
                    for (k = 0 ; k < i && str1input[k] == filecontent[j + k] ; k++);
                    if (k == i) {
                        int p;
                        for (p = j + i ; p <= len_file - len_str + i ; p++) {
                            if (filecontent[p] == '\n' || filecontent[p] == ' ') break;
                            int l;
                            for (l = p ; str1input[l - p + i + 1] == filecontent[l] ; l++);
                            if (l == p + len_str - i - 1) {
                                finds[findcount][0] = j;
                                finds[findcount][1] = p + len_str - i - 2;
                                findcount++;
                            }
                        }
                    }
                }
            }
            else {
                for (int j = 0 ; j < len_str - 1 ; j++) {
                    if (str1input[j] == '\\' && str1input[j+1] == '*') {
                        str1input[j] = '*';
                        for (int h = j+1 ; h < len_str - 1 ; h++) str1input[h] = str1input[h+1];
                        str1input[len_str - 1] = '\0';
                        len_str = strlen(str1input);
                    }
                }
                for (int j = 0 ; j <= len_file - len_str ; j++) {
                    for (k = 0 ; k < len_str && str1input[k] == filecontent[j + k] ; k++);
                    if (k == len_str) {
                        finds[findcount][0] = j;
                        finds[findcount][1] = j + len_str - 1;
                        findcount++;
                    }
                }
            }

            if (at_status == 0) {
                if (all_status == 0) {

                    for (int i = finds[0][0] ; i < len_file + finds[0][0] - finds[0][1] - 1 ; i++) filecontent[i] = filecontent[i+finds[0][1]-finds[0][0]+1];
                    filecontent[len_file + finds[0][0] - finds[0][1] - 1] = '\0';
                    len_file = len_file + finds[0][0] - finds[0][1] - 1;

                    if (strlen(str2input) == 0) {
                        printf("invalid string\n");
                        return 0;
                    }
                    else {
                        long int len_clip = strlen(str2input);
                        filecontent[len_file] = 'a';
                        filecontent[len_file + len_clip] = '\0';
                        for (int i = len_file - 1 ; i >= finds[0][0] ; i--) filecontent[i + len_clip] = filecontent[i];
                        for (int i = finds[0][0] ; i < finds[0][0] + len_clip ; i++) filecontent[i] = str2input[i - finds[0][0]];
                        FILE *file = fopen(fileinput, "w");
                        fprintf(file, filecontent);
                        fclose(file);
                    }

                }
                else {
                    for (int j = 0 ; j < findcount ; j++) {
                        for (int i = finds[j][0] ; i < len_file + finds[j][0] - finds[j][1] - 1 ; i++) filecontent[i] = filecontent[i+finds[j][1]-finds[j][0]+1];
                        filecontent[len_file + finds[j][0] - finds[j][1] - 1] = '\0';
                        len_file = len_file + finds[j][0] - finds[j][1] - 1;

                        if (strlen(str2input) == 0) {
                            printf("invalid string\n");
                            return 0;
                        }
                        else {
                            long int len_clip = strlen(str2input);
                            filecontent[len_file] = 'a';
                            filecontent[len_file + len_clip] = '\0';
                            for (int i = len_file - 1 ; i >= finds[j][0] ; i--) filecontent[i + len_clip] = filecontent[i];
                            for (int i = finds[j][0] ; i < finds[j][0] + len_clip ; i++) filecontent[i] = str2input[i - finds[j][0]];
                            FILE *file = fopen(fileinput, "w");
                            fprintf(file, filecontent);
                            fclose(file);
                        }
                    }
                }
            }
            else {
                if (all_status == 0) {

                        for (int i = finds[at_value - 1][0] ; i < len_file + finds[at_value - 1][0] - finds[at_value - 1][1] - 1 ; i++) filecontent[i] = filecontent[i+finds[at_value - 1][1]-finds[at_value - 1][0]+1];
                        filecontent[len_file + finds[at_value - 1][0] - finds[at_value - 1][1] - 1] = '\0';
                        len_file = len_file + finds[at_value - 1][0] - finds[at_value - 1][1] - 1;

                        if (strlen(str2input) == 0) {
                            printf("invalid string\n");
                            return 0;
                        }
                        else {
                            long int len_clip = strlen(str2input);
                            filecontent[len_file] = 'a';
                            filecontent[len_file + len_clip] = '\0';
                            for (int i = len_file - 1 ; i >= finds[at_value - 1][0] ; i--) filecontent[i + len_clip] = filecontent[i];
                            for (int i = finds[at_value - 1][0] ; i < finds[at_value - 1][0] + len_clip ; i++) filecontent[i] = str2input[i - finds[at_value - 1][0]];
                            FILE *file = fopen(fileinput, "w");
                            fprintf(file, filecontent);
                            fclose(file);
                        }

                }
                else printf("at and all can't come together\n");
            }

        }
    }
}

int read_cmd(char* cmd_line) {
    char cmd[25];
    int length = strlen(cmd_line),i;
    for (i=1 ; i<length ; i++) {
        if (cmd_line[i]==' ') {
            for (int j = 0 ; j < i ; j++) {
                cmd[j] = cmd_line[j];
            }
            cmd[i] = '\0';
            break;
        }
    }
            if (!strcmp(cmd, "createfile")) {
                if (read_cmd_inputs(cmd_line, i+1, 1) == 0) return 0;
                filenewlines();
                filetostring();
                postocharnum();
                if (createfile(fileinput) == 0) return -1;
                else return 1;
            }
            if (!strcmp(cmd, "insertstr")) {
                if (read_cmd_inputs(cmd_line, i+1, 1) == 0) return 0;
                filenewlines();
                filetostring();
                postocharnum();
                if (insertstr() == 0) return -1;
                else return 1;
            }
            if (!strcmp(cmd, "cat")) {
                if (read_cmd_inputs(cmd_line, i+1, 1) == 0) return 0;
                filenewlines();
                filetostring();
                postocharnum();
                    if (cat() == 0) return -1;
                    else return 1;
            }
            if (!strcmp(cmd, "removestr") || !strcmp(cmd, "copystr") || !strcmp(cmd, "cutstr")) {
                if (read_cmd_inputs(cmd_line, i+1, 1) == 0) return 0;
                filenewlines();
                filetostring();
                postocharnum();
                    if (!strcmp(cmd, "removestr")) {
                        if (removestr() == 0) return -1;
                        else return 1;
                    }
                    else if (!strcmp(cmd, "copystr")) {
                        if (copystr() == 0) return -1;
                        else return 1;
                    }
                    else {
                        if (cutstr() == 0) return -1;
                        else return 1;
                    }
            }
            if (!strcmp(cmd, "pastestr")) {
                if (read_cmd_inputs(cmd_line, i+1, 1) == 0) return 0;
                filenewlines();
                filetostring();
                postocharnum();
                if (pastestr() == 0) return -1;
                else return 1;
            }
            if (!strcmp(cmd, "find")) {
                if (read_cmd_inputs(cmd_line, i+1, 1) == 0) return 0;
                filenewlines();
                filetostring();
                postocharnum();
                if (find() == 0) return -1;
                else return 1;
            }
            if (!strcmp(cmd, "replace")) {
                if (read_cmd_inputs(cmd_line, i+1, 1) == 0) return 0;
                filenewlines();
                filetostring();
                postocharnum();
                    if (replace() == 0) return -1;
                    else return 1;
            }
            if (!strcmp(cmd, "grep")) {
                if (read_cmd_inputs(cmd_line, i+1, 1) == 0) return 0;
                printf("undeveloped\n");
                return 1;
                filenewlines();
                filetostring();
                postocharnum();
            }
            if (!strcmp(cmd, "undo")) {
                if (read_cmd_inputs(cmd_line, i+1, 1) == 0) return 0;
                printf("undeveloped\n");
                return 1;
                filenewlines();
                filetostring();
                postocharnum();
            }
            if (!strcmp(cmd, "auto-indent")) {
                if (read_cmd_inputs(cmd_line, i+1, 1) == 0) return 0;
                printf("undeveloped\n");
                return 1;
                filenewlines();
                filetostring();
                postocharnum();
            }
            if (!strcmp(cmd, "compare")) {
                if (read_cmd_inputs(cmd_line, i+1, 1) == 0) return 0;
                printf("undeveloped\n");
                return 1;
                filenewlines();
                filetostring();
                postocharnum();
            }
            if (!strcmp(cmd, "tree")) {
                if (read_cmd_inputs(cmd_line, i+1, 1) == 0) return 0;
                printf("undeveloped\n");
                return 1;
                filenewlines();
                filetostring();
                postocharnum();
            }
}


int read_cmd_inputs(char* cmd_line, int start_pt) {
    int length = strlen(cmd_line), reader = start_pt + 2, input_return, k=1;
    *at_status_Ptr = 0;
    *count_status_ptr = 0;
    c_status = 0;
    l_status = 0;
    while (start_pt < length) {
        if (cmd_line[start_pt] != '-') return 0;
        reader = start_pt + 2;
            for (reader = start_pt + 1 ; cmd_line[reader] != ' ' && reader < length ; reader++);
            if (cmd_line[start_pt + 1] == '-') start_pt++;
            for (int i = start_pt + 1 ; i < reader ; i++) {
                input_function_name_temp[i - (start_pt + 1)] = cmd_line[i];
            }
            input_function_name_temp[reader - start_pt - 1] = '\0';

        if (!strcmp(input_function_name_temp, "file")) {
            input_return = read_file(reader+1, cmd_line);
            if (input_return == 0) return 0;
            start_pt = input_return;
        }
        else if (!strcmp(input_function_name_temp, "str")) {
            input_return = read_str(reader+1, cmd_line);
            if (input_return == 0) return 0;
            start_pt = input_return;
        }
        else if (!strcmp(input_function_name_temp, "str1")) {
            input_return = read_str1(reader+1, cmd_line);
            if (input_return == 0) return 0;
            start_pt = input_return;
        }
        else if (!strcmp(input_function_name_temp, "str2")) {
            input_return = read_str2(reader+1, cmd_line);
            if (input_return == 0) return 0;
            start_pt = input_return;
        }
        else if (!strcmp(input_function_name_temp, "file1")) {
            input_return = read_file1(reader+1, cmd_line);
            if (input_return == 0) return 0;
            start_pt = input_return;
        }
        else if (!strcmp(input_function_name_temp, "file2")) {
            input_return = read_file2(reader+1, cmd_line);
            if (input_return == 0) return 0;
            start_pt = input_return;
        }
        else if (!strcmp(input_function_name_temp, "pos")) {
            input_return = read_pos(reader+1, cmd_line);
            if (input_return == 0) return 0;
            start_pt = input_return;
        }
        else if (!strcmp(input_function_name_temp, "f") || !strcmp(input_function_name_temp, "b")) {
            if (!strcmp(input_function_name_temp, "f")) fb_status = 1;
            else fb_status = 0;
            start_pt = reader + 1;
        }
        else if (!strcmp(input_function_name_temp, "size")) {
            input_return = read_size(reader+1, cmd_line);
            if (input_return == 0) return 0;
            start_pt = input_return;
        }
        else if (!strcmp(input_function_name_temp, "depth")) {
            input_return = read_depth(reader+1, cmd_line);
            if (input_return == 0) return 0;
            start_pt = input_return;
        }
        else if (!strcmp(input_function_name_temp, "at")) {
            *at_status_Ptr = 1;
            input_return = read_at(reader+1, cmd_line);
            if (input_return == 0) return 0;
            start_pt = input_return;
        }
        else if (!strcmp(input_function_name_temp, "count")) {
            *count_status_ptr = 1;
            start_pt = reader + 1;
        }
        else if (!strcmp(input_function_name_temp, "byword")) {
            *byword_status_ptr = 1;
            start_pt = reader + 1;
        }
        else if (!strcmp(input_function_name_temp, "all")) {
            *all_status_ptr = 1;
            start_pt = reader + 1;
        }
        else if (!strcmp(input_function_name_temp, "files")) {
            input_return = read_filesandstrs(reader+1, cmd_line, 0);
            if (input_return == 0) return 0;
            start_pt = input_return;
        }
        else if (!strcmp(input_function_name_temp, "c")) {
            c_status = 1;
            start_pt = reader + 1;
        }
        else if (!strcmp(input_function_name_temp, "l")) {
            l_status = 1;
            start_pt = reader + 1;
        }
        else return 0;

         k++;

    }

    return 1;

}

int read_filesandstrs(int start_pt, char *cmd_line, int file_count) {
    int length = strlen(cmd_line);
    if (start_pt >= length - 1) return 0;
    if (cmd_line[start_pt] == ' ') return 0;
    int reader = start_pt;
    if (start_pt < length && cmd_line[start_pt] != '-') {
        if (cmd_line[start_pt] == 34) {
            for (reader = start_pt + 1 ; cmd_line[reader]!=34 ; reader++);
            for (int i = start_pt+1 ; i < reader ; i++) {
                filesinput[file_count][i - start_pt - 1] = cmd_line[i];
            }
            filesinput[file_count][reader-start_pt-1] = '\0';
            if (reader + 2 != '-' && reader + 2 < length) return read_filesandstrs(reader + 2, cmd_line, file_count + 1);
            else return (reader + 2);
        }
        if (cmd_line[start_pt] != 34) {
            for (reader = start_pt + 1 ; cmd_line[reader]!=' ' ; reader++) {
                if (reader >= length - 1) {
                    for (int i = start_pt ; i <= reader ; i++) filesinput[file_count][i-start_pt] = cmd_line[i];
                    filesinput[file_count][reader-start_pt+1] = '\0';
                    if (reader + 2 != '-' && reader + 2 < length) return read_filesandstrs(reader + 2, cmd_line, file_count + 1);
                    else return (reader + 2);
                }
            }
            if (reader == length - 1) return 0;
            for (int i = start_pt ; i < reader ; i++) filesinput[file_count][i-start_pt] = cmd_line[i];
            filesinput[file_count][reader-start_pt] = '\0';
            if (cmd_line[reader + 1] != '-' && reader + 1 < length) return read_filesandstrs(reader + 1, cmd_line, file_count + 1);
            else return (reader + 1);
        }
    }
    else return start_pt;
}


int read_fileandstr(int start_pt, char *cmd_line, char *inputarray) {
    int length = strlen(cmd_line);
    if (start_pt >= length - 1) return 0;
    if (cmd_line[start_pt] == ' ') return 0;
    int reader = start_pt;
    if (cmd_line[start_pt] == 34) {
        for (reader = start_pt + 1 ; cmd_line[reader]!=34 ; reader++) {
            if (reader >= length - 1);
        }
        for (int i = start_pt+1 ; i < reader ; i++) {
            inputarray[i-start_pt-1] = cmd_line[i];
        }
        inputarray[reader-start_pt-1] = '\0';
        return reader+2;
    }
    if (cmd_line[start_pt] != 34) {
        for (reader = start_pt + 1 ; cmd_line[reader]!=' ' ; reader++) {
            if (reader >= length - 1) {
                for (int i = start_pt ; i <= reader ; i++) inputarray[i-start_pt] = cmd_line[i];
                inputarray[reader-start_pt+1] = '\0';
                return reader+2;
            }
        }
        if (reader == length - 1) return 0;
        for (int i = start_pt ; i < reader ; i++) inputarray[i-start_pt] = cmd_line[i];
        inputarray[reader-start_pt] = '\0';
        return reader+1;
    }
}

int read_file(int start_pt, char *cmd_line) {
    int returnc = read_fileandstr(start_pt, cmd_line, fileinput);
    if (returnc != 0) {
        for (int i = 0 ; i < strlen(fileinput) ; i++) {
            if (fileinput[i] != '.' && fileinput[i] != '/') {
                int j;
                for (j = i ; j < strlen(fileinput) && fileinput[j] != '/' ; j++);
                char firstdir[20];
                strncpy(firstdir, fileinput + i, j - i);
                if (fileinput[0] != '.') {
                    int len_file = strlen(fileinput);
                    fileinput[len_file] = 'a';
                    for (int i = len_file ; i >= 1 ; i--) fileinput[i] = fileinput[i-1];
                    fileinput[0] = '.';
                    fileinput[len_file+1] = '\0';
                }
                if (!strcmp(firstdir, "root")) return returnc;
                else return 0;
            }
        }
    }
    else return 0;
}
int read_str(int start_pt, char *cmd_line) {
    int a = read_fileandstr(start_pt, cmd_line, strinput);
    int len_s = strlen(strinput);
    for (int i = 0 ; i < len_s ; i++) {
        if (strinput[i] == '\\' && strinput[i+1] == 'n') {
            strinput[i] = '\n';
            for (int j = i+1 ; j < len_s - 1 ; j++) strinput[j] = strinput[j+1];
            strinput[len_s-1] = '\0';
            len_s--;
        }
        else if (strinput[i] == '\\' && strinput[i+1] == '\\' && strinput[i+2] == 'n') {
            strinput[i] = '\\';
            strinput[i+1] = 'n';
            for (int j = i+2 ; j < len_s - 1 ; j++) strinput[j] = strinput[j+1];
            strinput[len_s-1] = '\0';
            len_s--;
        }
    }
    return a;
}
int read_str1(int start_pt, char *cmd_line) {
    return read_fileandstr(start_pt, cmd_line, str1input);
}
int read_str2(int start_pt, char *cmd_line) {
    return read_fileandstr(start_pt, cmd_line, str2input);
}
int read_file1(int start_pt, char *cmd_line) {
    return read_fileandstr(start_pt, cmd_line, file1input);
}
int read_file2(int start_pt, char *cmd_line) {
    return read_fileandstr(start_pt, cmd_line, file2input);
}

int read_number(int start_pt, char *cmd_line, int *numberPtr) {
    int length = strlen(cmd_line);
    if (start_pt > length - 1) return 0;
    if (cmd_line[start_pt] == ' ') return 0;
    int reader = start_pt;
    if (cmd_line[start_pt] < '0' || cmd_line[start_pt] > '9') return 0;
    for (reader = start_pt + 1 ; cmd_line[reader]!=' ' && reader < length ; reader++) {
            if (cmd_line[reader] < '0' || cmd_line[reader] > '9') return 0;
            if (reader >= length - 1) {
                    for (int i = start_pt ; i <= reader ; i++) pos_temp[i-start_pt] = cmd_line[i];
                    pos_temp[reader-start_pt+1] = '\0';
                    sscanf(pos_temp,"%d",numberPtr);
                    return reader+2;
            }
        }
        if (reader == length - 1) return 0;
        for (int i = start_pt ; i < reader ; i++) pos_temp[i-start_pt] = cmd_line[i];
        pos_temp[reader-start_pt] = '\0';
        sscanf(pos_temp,"%d",numberPtr);
        return reader+1;
}

int read_size(int start_pt, char *cmd_line) {
    return read_number(start_pt, cmd_line, &size);
}

int read_at(int start_pt, char *cmd_line) {
    return read_number(start_pt, cmd_line, &at_value);
}

int read_depth(int start_pt, char *cmd_line) {
    return read_number(start_pt, cmd_line, &depth);
}

int read_pos(int start_pt, char *cmd_line) {
    int k = 0, length = strlen(cmd_line);
    if (start_pt >= length - 1) return 0;
    if (cmd_line[start_pt] == ' ') return 0;
    int reader = start_pt;
    if (!(cmd_line[start_pt] >= '0' && cmd_line[start_pt] <= '9')) return 0;
        for (reader = start_pt + 1 ; cmd_line[reader]!=' ' ; reader++) {
            if (!(cmd_line[reader] == ':' || (cmd_line[reader]>='0' && cmd_line[reader]<='9'))) return 0;
            if (k != 0 && cmd_line[reader] == ':') return 0;
            if (cmd_line[reader] == ':') k++;
            if (reader >= length - 1) {
                    for (int i = start_pt ; i <= reader ; i++) pos_temp[i-start_pt] = cmd_line[i];
                    pos_temp[reader-start_pt+1] = '\0';
                    sscanf(pos_temp,"%d:%d",&pos[0],&pos[1]);
                    return reader+2;
            }
        }
        if (reader == length - 1) return 0;
        for (int i = start_pt ; i < reader ; i++) pos_temp[i-start_pt] = cmd_line[i];
        pos_temp[reader-start_pt] = '\0';
        sscanf(pos_temp,"%d:%d",&pos[0],&pos[1]);
        return reader+1;
}

int main() {
    if (!exist("./root") || !isNode("./root")) mkdir("./root");
    while (1) {
        char source[150];
        gets(source);
        if (read_cmd(source) == 0) printf("input error\n");
    }
}
