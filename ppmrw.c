#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*typedef struct Pixel{unsigned char r;
unsigned char g;
unsigned char b;} Pixel;

typedef struct Image{int width;
int height;
int color;
Pixel *data;} Image;

Image *buffer;*/

void numarg(int);
void exists(char *);
void check_format(int);
void check_filetype(char[], int, FILE *);
void conv_to_p3(FILE *, char[]);
void conv_to_p6(FILE *, char[]);

int main(int argc, char *argv[]){
    int arguments = argc;
    char *format = argv[1];
    int int_format = atoi(format);
    char *input_file = argv[2];
    char *output_file = argv[3];
    FILE *fh = fopen(input_file, "r");
    char line1[2];
    fscanf(fh, "%s", line1);

    numarg(arguments);
    exists(input_file);
    check_format(int_format);
    check_filetype(line1, int_format, fh);

    return 0;
}

void numarg(int arguments){
    if (arguments != 4){
        fprintf(stderr, "Error: There needs to be 4 arguments");
        exit(1);
    }
}

void exists(char *input_file){
    if(access (input_file, R_OK) == -1){
        fprintf(stderr, "Error: The input file does not exist");
        exit(1);
    }
}

void check_format(int int_format){
    if((int_format != 3) & (int_format != 6)){
        fprintf(stderr, "Error: This is not a correct magical number");
        exit(1);
    }
}

void check_filetype(char line1[], int int_format, FILE *fh){
    if ((strcmp(line1, "P3") == 1) & (strcmp(line1, "P6") == 1)){
        fprintf(stderr, "Error: This is not a PPM file.");
        fclose(fh);
        exit(1);
    }
    if (int_format == 3){
        conv_to_p3(fh, line1);
    }else{
        conv_to_p6(fh, line1);
    }
}

void conv_to_p3(FILE *fh, char line1[]){
    if (strcmp(line1, "P3") == 0){
        printf("P3 to P3 conversion");
    }else{
        printf("P6 to P3 conversion");
    }
}

void conv_to_p6(FILE *fh, char line1[]){
    if (strcmp(line1, "P3") == 0){
        printf("P3 to P6 conversion");
    }else {
        printf("P6 to P6 conversion");
    }
}
