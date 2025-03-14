#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int position;
};

int reader(char *filename) {
    // printf("Reading bytes file %s\n", filename);
    int num_bytes;
    char buffer[1024];
    printf("How many bytes do you want to read?\n");

    do {
        if (!fgets(buffer, 1024, stdin)) {
            return 1;
        }
        num_bytes = atoi(buffer);
    } while (num_bytes == 0); //repeat until valid number

    char readBuffer[num_bytes + 1];
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s\n", filename);
        return 1;
    }

    int i = 0, c;

    while (i < num_bytes && (c = getc(file)) != EOF) {
        readBuffer[i++] = (char) c;
    }
    readBuffer[i] = '\0';

    fclose(file);
    printf("%s\n", readBuffer);
    return 0;
}

int writer(char *filename) {
    char buffer[1024];
    printf("Enter the data you want to write:\n");
    fgets(buffer, 1024, stdin);

    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Could not open file %s\n", filename);
        return 1;
    }
    fprintf(file, "%s", buffer); //write to the file

    fclose(file); //close the file
    return 0;

}

int seeker(char *filename) {
    //fseek(FILE *filePointer, long offset, int origin);

}


int main(int argc, const char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    FILE *f = fopen(filename, "r"); //

    if (f == NULL) {
        printf("Error opening file %s\n", filename);
        return 1;
    }

    while (1) {
        printf("Option (r for read, w for write, s for seek): \n");
        char choice = getchar();
        while (getchar() != '\n'); //consume leftover characters

        switch (choice) {
            case 'r':
                int reader_result = reader(filename);

                if (reader_result != 0) {
                    printf("Error reading file %s\n", filename);
                }

                break;
            case 'w':
                int writer_result = writer(filename);
                if (writer_result != 0) {
                    printf("Error writing file %s\n", filename);
                }

                break;
            case 's':
                printf("Seeking to file %s\n", filename);
                break;
            default:
                printf("Invalid input.\n");
        }
    }

    return 0;
}


/*
 *we will use fgets()
 *and strcspn() to remove end line
 *and strtol() to get numbers
 *
 *
 */
