#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct Pixel{r, g, b};

//Pixel *buffer;

void numarg(int);
void exists(char *);
void check_format(int);
void check_filetype(FILE *);

int main(int argc, char *argv[]){
    int arguments = argc;
    char *format = argv[1];
    int int_format = atoi(format);
    char *input_file = argv[2];
    char *output_file = argv[3];
    FILE *fh = fopen(input_file, "r");

    numarg(arguments);
    exists(input_file);
    check_format(int_format);
    check_filetype(fh);

    if(int_format == 3){
        conv_to_p3(fh);
    }else{
        conv_to_p6(fh);
    }
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

void check_format(int format){
    if((format != 3) & (format != 6)){
        fprintf(stderr, "Error: This is not a correct magical number");
        exit(1);
    }
}

void check_filetype(FILE *fh){
    int type = fgetc(fh);
    if (type != 'P'){
        fprintf(stderr, "Error: This is not a PPM file.");
        fclose(fh);
        exit(1);
    }
}

void conv_to_p3(FILE *fh){

}

void conv_to_p6(FILE *fh){

}
