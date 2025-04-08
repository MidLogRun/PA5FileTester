#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    FILE *file;
    char *filename;
} my_file_t;


int reader(my_file_t *fp) {
    if (!fp->file) {
        printf("File pointer is null or not opened %s\n", fp->filename);
        return 1;
    }

    int input_buffer_size = 1024;
    char buffer[input_buffer_size];
    printf("Enter number of bytes to read: ");

    int num_bytes;
    do {
        if (!fgets(buffer, 1024, stdin)) {
            return 1;
        }
        num_bytes = atoi(buffer);
    } while (num_bytes == 0); //repeat until valid number

    char readBuffer[num_bytes + 1];

    int i = 0, c;

    while (i < num_bytes && (c = getc(fp->file)) != EOF) {
        readBuffer[i++] = (char) c;
    }
    readBuffer[i] = '\0';
    printf("%s\n", readBuffer);

    return 0;
}

int writer(my_file_t *fp) {
    if (!fp->file) {
        printf("filepointer is null  %s\n", fp->filename);
        return 1;
    }

    char buffer[1024];
    printf("Enter the data you want to write:");
    fgets(buffer, 1024, stdin);


    if (fprintf(fp->file, "%s", buffer) < 0) {
        printf("Error writing to file\n");
        fclose(fp->file);
        return 1;
    }

    //Put a null character after the input
    if (fputc('\0', fp->file) == EOF) {
        printf("Error writing null terminator to file\n");
        fclose(fp->file);
        return 1;
    }

    return 0;
}

int seeker(my_file_t *fp) {
    if (!fp->file) {
        printf("filepointer is null  %s\n", fp);
        return 1;
    }

    int input_buffer_size = 1024;
    char buffer[input_buffer_size];

    int valid;

    do {
        printf("Enter an offset value (bytes): ");
        valid = 1;
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            return 1;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        if (buffer[0] == '\0') {
            valid = 0;
        }

        for (int i = 0; buffer[i] != '\0'; i++) {
            if (!isdigit(buffer[i])) {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf("Invalid offset value. Please enter an integer value.\n");
        }
    } while (!valid); //repeat until valid number (for offset)

    int offset = atoi(buffer);

    int whence;
    int whence_input;

    do {
        valid = 1;
        printf("Enter a whence value (bytes): ");
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            return 1;
        }

        buffer[strcspn(buffer, "\n")] = '\0';

        if (buffer[0] == '\0') {
            valid = 0;
        }
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (!isdigit(buffer[i])) {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("Invalid whence value. Please enter an integer value.\n");
            continue;
        }
        whence_input = atoi(buffer);
        switch (whence_input) {
            case 0:
                whence = SEEK_SET;
            break;
            case 1:
                whence = SEEK_CUR;
            break;
            case 2:
                whence = SEEK_END;
            break;
            default:
                printf("Invalid whence value. Please enter an integer value.\n");
            valid = 0;
            break;
        }
    } while (!valid);


    if (whence == SEEK_END && offset != 0) {
        offset = 0; //offset cannot be more than 0 if whence is end of file
    }

    fseek(fp->file, offset, whence);
    return 0;
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    FILE *f = fopen(filename, "r+"); //open the file once and, upon exit, close the file
    my_file_t *my_file = malloc(sizeof(my_file_t));
    my_file->file = f;
    my_file->filename = filename;


    while (1) {
        if (f == NULL) {
            printf("Error opening file %s\n", filename);
            free(my_file);
            return 1;
        }

        printf("Option (r for read, w for write, s for seek): \n");
        const char choice = getchar();

        if (choice == EOF) {
            printf("Exiting... \n");
            fclose(f);
            free(my_file);
            exit(0);
        }

        while (getchar() != '\n'); //consume leftover characters

        switch (choice) {
            case 'r':
                int reader_result = reader(my_file);

                if (reader_result != 0) {
                    printf("Error reading file %s\n", my_file->filename);
                }

                break;
            case 'w':
                const int writer_result = writer(my_file);
                if (writer_result != 0) {
                    printf("Error writing file %s\n", my_file->filename);
                }

                break;
            case 's':
                const int seeker_result = seeker(my_file);
                if (seeker_result != 0) {
                    printf("Error seeking file %s\n", my_file->filename);
                }

                break;
            default:
                printf("Invalid input.\n");
        }
    }


}
