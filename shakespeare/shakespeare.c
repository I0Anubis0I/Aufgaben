/**
 * shakespeare.c
 * Stefanie Schatz
 * Date:05.03.2025
 * 
 * Aufgabe 02
 * Program can be used as a filter or with one file as input argument.
 * The output is controlled by option arguments 
 * -h outputs usage
 * -f <inputfile> is optional, <inputfile> is sufficient
 * -s <n> n=search string length, default=1
 * -l <n> length of output, default=number of characters read
 **/

//Includes
#include <stdio.h> //c standard
#include <getopt.h> //programm argument processing
#include <string.h> //string copy
#include <stdlib.h> //for constants
#include <unistd.h> //filemanagement
#include <time.h> //time for randomfunction

#ifndef BINTREE
#include "binarytree.h"
#endif

//Macros
#define FALSE 0
#define TRUE !(FALSE)
#define ERROR_FILEREAD(message) fprintf(stderr,"%s is not a valid input file name!\n",message)
#define ERROR_FILEOPEN(message) fprintf(stderr,"An error occured while opening %s!\n",message)
#define ERROR_USAGE(message) fprintf(stderr,"Usage: %s [-h] [-s <n>] [-l <m>] [-f <filename>]\n",message)
#define ERROR_NAMELESS(message) fprintf(stderr,"There is no name %s\n",message)


//Main function
int main(int argc, char **argv) {
    int mainReturn = EXIT_SUCCESS;
    int option; /* contains one option letter */
    char filename[255] = ""; 
    int filenameValid = FALSE;
    FILE* inputFile;
    size_t substring_length = 1;
    size_t outputstring_length = 0;
    char firststring[substring_length +1];
    memset(firststring, 0, sizeof(firststring));
    char *output_string = malloc(outputstring_length + 1);
    TNode* treeRoot = NULL;


    enum {OFF, ON};

    struct { 
        unsigned int h: 1;
        unsigned int f: 1;
        unsigned int s: 1;
        unsigned int l: 1;
    } optionflags = {OFF, OFF, OFF, OFF};

    inputFile = stdin; /* for the case: openInputFile() is never used */

    //Reading the options and arguments and setting flags
    while ((option =getopt(argc, argv, "hf:s:l:")) != EOF) {
        switch (option)
        {
        case 'h':
            optionflags.h = ON;
            usage(filename);
            break;
        
        case 'f':
            optionflags.f = ON;
            printf("Dateiname aus -f: %s\n", optarg);
            strncpy(filename, optarg, 255);
            filenameValid = isReadableFile(filename);
            break;

        case 's':
                int input_string_length = atoi(optarg);
                
                if (input_string_length > 0 && input_string_length <= 10) {
                    optionflags.s = ON;
                    substring_length = (size_t)input_string_length;
                } 
                else {
                    fprintf(stderr, "Fehler: Der Wert für -s muss eine positive Zahl zwischen 1 und 10 sein (eingegeben: %d)\n", input_string_length);
                    exit(EXIT_FAILURE);
                }
            
            break;

        case 'l':
                int input_text_length = atoi(optarg);
                
                if (input_text_length > 0) {
                    optionflags.l = ON;
                    outputstring_length = (size_t)input_text_length;
                } 
                else {
                    fprintf(stderr, "Fehler: Der Wert für -l muss eine positive Zahl sein (eingegeben: %d)\n", input_text_length);
                    exit(EXIT_FAILURE);
                }
            break;

        default:
            usage(filename);
            exit(EXIT_FAILURE);
        }
    }



    //looking for a further parameterwithout option
    if (optind < argc && !optionflags.f) {
        strncpy(filename,argv[optind], 255);
        filenameValid = isReadableFile(filename);
    }

    if (filenameValid == FALSE || (optionflags.h == OFF && optionflags.f == OFF && optionflags.s == OFF && optionflags.l == OFF)) {
        usage(filename);
        exit(EXIT_FAILURE); /* no arguments or wrong filename */
    }

    mainReturn = ((inputFile = openInputFile(filename)) == NULL) ? EXIT_FAILURE : EXIT_SUCCESS;

    if (mainReturn== EXIT_SUCCESS) {
    
        if (optionflags.s == ON || optionflags.l == ON) {
            inputFile = openInputFile(filename);   
            build_tree_from_file(inputFile, &treeRoot, substring_length);
            rewind(inputFile);

            if (optionflags.s == ON && optionflags.l == OFF) {
                fseek(inputFile, 0, SEEK_END);
                outputstring_length = ftell(inputFile);
                rewind(inputFile);
            }

            fread(firststring, 1, substring_length, inputFile);  // Read up to substring_length characters
            firststring[substring_length] = '\0';
            output_string = generate_text(treeRoot, substring_length, outputstring_length, inputFile);
            printf("<%s>%s\n",firststring, output_string);
        }
        
        fflush(stdout);
        }
        else {
            ERROR_FILEOPEN(filename);
    }
    


    fclose(inputFile);
    return mainReturn;
}