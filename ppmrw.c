#include <stdio.h>      //next 2 libraries are standard
#include <stdlib.h>
#include <unistd.h>     //I use this to check if a file exists without opening it

#define MAX_COLOR 255   //max color definition for later use

typedef struct Pixel{unsigned int r;    //create a pixel struct like discussed in class
unsigned int g;
unsigned int b;
}Pixel;

typedef struct Image{   //image struct that keeps track of the data in the input file
    int width;
    int height;
    int color;
    unsigned char *data;
} Image;

Image *buffer;  //implemented buffer like requested

void numarg(int);                   //function declarations
void exists(char *);
void check_format(int, char *);
Image read_data(char *);
int write_data(int, char *);

int main(int argc, char *argv[]){
    int arguments = argc;               //saving command line input into variables
    char *format = argv[1];
    int int_format = atoi(format);
    char *input_file = argv[2];
    char *output_file = argv[3];

    numarg(arguments);                      //functions called that do work
    exists(input_file);
    read_data(input_file);
    check_format(int_format, output_file);

    return 0;   //return no errors
}

void numarg(int arguments){     //checks if number of arguments is correct
    if (arguments != 4){
        fprintf(stderr, "Error: There needs to be 4 arguments\n");
        exit(1);
    }
}

void exists(char *input_file){      //checks if input file exists
    if(access (input_file, R_OK) == -1){
        fprintf(stderr, "Error: The input file does not exist\n");
        exit(1);
    }
}

Image read_data(char *input_file){
    buffer = (Image*)malloc(sizeof(Image));     //allocates memory for buffer
    FILE* ifp = fopen(input_file, "rb");        //opens file for reading
    int current = fgetc(ifp);                   //sets variable equal to first character in file
    if(current != 'P'){
        fprintf(stderr, "Error: This is not a PPM file!\n");    //checks if file is a ppm file
        fclose(ifp);
        exit(1);
    }

    current = fgetc(ifp);                                       //checks if p3 or p6 file or neither
    int ppm_type = current;
    if((ppm_type != '3') & (ppm_type != '6')){
        fprintf(stderr, "Error: Only P3/P6 files allowed!\n");
        fclose(ifp);
        exit(1);
    }

    while(current != '\n'){     //skips newlines
        current = fgetc(ifp);
    }
    current = fgetc(ifp);

    while(current == '#'){      //skips comments
        while(current != '\n'){
            current = fgetc(ifp);
        }
        current = fgetc(ifp);
    }
    ungetc(current, ifp);

    int dimensions = fscanf(ifp, "%d %d", &buffer->width, &buffer->height);     //saves image width and height in buffer
    if(dimensions != 2){                                                        //checks if dimensions are correct amount
        fprintf(stderr, "Error: Not correct dimensions of width and height!\n");
        fclose(ifp);
        exit(1);
    }

    int max_color = fscanf(ifp, "%d", &buffer->color);      //checks if max color value is in correct range
    if((max_color < 1) | (max_color > MAX_COLOR)){
        fprintf(stderr, "Error: The color value range is 1 through 255\n");
        fclose(ifp);
        exit(1);
    }

    buffer->data = (unsigned char*)malloc((buffer->width) * (buffer->height) * (sizeof(Pixel)));    //allocates memory for the buffer data
    if(buffer == NULL){
        fprintf(stderr, "Error: memory allocation unsuccessful\n");
        fclose(ifp);
        exit(1);
    }

    if(ppm_type == '3'){        //if the image is a p3 file this for loop writes the data into the pixels struct for rgb like discussed in class
        int i, j;
        for(i=0; i<buffer->height; i++){
            for(j=0; j<buffer->width; j++){
                Pixel *pixels = (Pixel*)malloc(sizeof(Pixel));
                fscanf(ifp, "%d %d %d", &pixels->r, &pixels->g, &pixels->b);
                buffer->data[i*buffer->width * 3 + j * 3] = pixels->r;
                buffer->data[i*buffer->width * 3 + j * 3 + 1] = pixels->g;
                buffer->data[i*buffer->width * 3 + j * 3 + 2] = pixels->b;
            }
        }
    }
    else{
        size_t s = fread(buffer->data, sizeof(Pixel), buffer->width*buffer->height, ifp);   //saves p6 data into buffer
        if(s < buffer->width*buffer->height){
            fprintf(stderr, "Error: Can't convert!");
            fclose(ifp);
            exit(1);
        }
    }
    fclose(ifp);
    return *buffer;
}

void check_format(int int_format, char *output_file){   //checks what format the file is being converted to
    if((int_format != 3) & (int_format != 6)){
        fprintf(stderr, "Error: This is not a correct magical number");
        exit(1);
    }
    write_data(int_format, output_file);
}

int write_data(int int_format, char *output_file){ //function that writes data to new file
    FILE *ofp = fopen(output_file, "wb");   //opens output file for writing

    char *extension = "# output.ppm";   //adds extension to output file to make it readable
    int width = buffer->width;      //saves dimensions from buffer into variables
    int height = buffer->height;

    fprintf(ofp, "P%i\n%s\n%d %d\n%d\n", int_format, extension, width, height, MAX_COLOR);  //prints correct header for new output file

    if(int_format == 6){        //writes data if being converted to P6
        fwrite(buffer->data, sizeof(Pixel), buffer->width*buffer->height, ofp);
        printf("The file converted successfully!\n");
    }
    else if(int_format == 3){   //writes data if being converted to P3
        int i, j;
        for(i=0; i<buffer->height; i++){
            for(j=0; j<buffer->width; j++){
                fprintf(ofp, "%d %d %d ", buffer->data[i*buffer->width*3+j*3], buffer->data[i*buffer->width+j*3+1], buffer->data[i*buffer->width*3+2]);
            }
            fprintf(ofp, "\n");
        }
        printf("Image converted successfully!\n");
    }
    return(0);
}
