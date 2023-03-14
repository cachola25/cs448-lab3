
/*Author: Jayven Cachola*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define FAILED 1

int decrypt(char *, char *, char *);
int encrypt(char *, char *, char *);
void show_usage(void);

int main(int argc, char **argv) {

    bool isEncrypt = false;

    //Check if there are the correct # of arguments
    if(argc != 8) {

        printf("Invalid number of parameters\n");
        return FAILED;

    }

    //Checks if switch is valid
    if(strcmp("-E",argv[1]) == 0) {

        printf("Beginning encryption...\n");
        isEncrypt = true;

    }else if(strcmp("-D",argv[1]) == 0) {

        printf("Beginning decrpytion...\n");
    } else {
        printf("Invalid switch, use -E or -D\n");
        return FAILED;
    }

    //Check if key is specified correctly
    if(strcmp("-k",argv[2]) != 0) {

        printf("Invalid switch detected\n");
        return FAILED;
    }

    char* key = argv[3];

    //Switch checking
    if(strcmp("-i",argv[4]) != 0) {

        printf("Invalid switch detected\n");
        return FAILED;
    }

    //Get input file
    FILE* input = fopen(argv[5],"r");
    if(input == NULL){

        printf("%s could not be accessed\n", argv[5]);
        return FAILED;
    }

    //Get the size of the file
    //This lets us dynamically allocate memory for the text
    //Searched up how to do this online
    fseek(input,0,SEEK_END);
    int inputSize = ftell(input);
    rewind(input);

    //Switch checking
    if(strcmp("-o",argv[6]) != 0) {

        printf("Invalid switch detected\n");
        return FAILED;
    }

    //Get output file
    FILE* output = fopen(argv[7],"w");
    if(output == NULL) {

        printf("%s could not be accessed\n",argv[7]);
        return FAILED;
    }

    //Check if key is valid
    int keyLength = strlen(key);
    for(int i = 0; i < keyLength; i++) {

        if(!isupper(key[i]) && !isdigit(key[i])) {

            printf("Invalid key detected\n");
            return FAILED;
        }
    }

    char* inputText = (char*)malloc(sizeof(char) * (inputSize + 1));
    char ch = '{';

    //Check if input file is empty
    if(fgetc(input) == EOF) {

            printf("Empty file detected\n");
            return FAILED;
    }
    rewind(input);

    //Initialize array that holds the contents of the input file
    int index = 0;
    while((ch = fgetc(input)) != EOF) {
        //Check if the current character is a printable one
        if(!isprint(ch)){break;}
        inputText[index] = ch;
        index++;
    }

    //Make a new key array if key is shorter than the input text
    //Don't need to worry about longer key
    int inputLength = strlen(inputText);
    if(keyLength < inputLength) {

        char* newKey = (char*)malloc(sizeof(char) * (inputSize + 1));
        for(int i = 0; i < strlen(inputText); i++) {
           newKey[i] = key[i % strlen(key)];
        }
        key = newKey;
    }

    char* res = (char*)malloc(sizeof(char) * (inputSize + 1));

    int success = isEncrypt ? encrypt(key,inputText,res) : decrypt(key,inputText,res);
    if(success == FAILED){
        printf("...Process Failed...\n");
        return FAILED;
    }
    //Write result to output file
    fprintf(output,"%s\n",res);
    fclose(input);
    fclose(output);
    return 0;
}

int decrypt(char *key, char *ciphertext, char *result) {

    //Check if something went wrong with memory
    if(key == NULL || ciphertext == NULL || result == NULL){
        printf("Decryption failed\n");
        return FAILED;
    }
    printf("...Decrypting...\n");
    int len = strlen(ciphertext);
    int cipherNums[len];
    int keyNums[len];

    //Convert the char arrays into int arrays
    //so it is easier to make the alphabet
    //shifts
    for(int i = 0; i < len; i++) {

        //Number values will hold values 26-35
        //Letters will hold values 0-25
        if(isdigit(ciphertext[i])) {
            cipherNums[i] = ciphertext[i] - '0' + 26;
        } else {
            cipherNums[i] = ciphertext[i] - 'A';
        }

        if(isdigit(key[i])) {
            keyNums[i] = key[i] - '0' + 26;
        } else {
            keyNums[i] = key[i] - 'A';
        }
    }

    for(int i = 0; i < len; i++) {

        //Mod by 36 since that's how long our alphabet is
        int num = (cipherNums[i] - keyNums[i]) % 36;

        //Shift values based on num
        if(num < 0) {
            num += 36;
        }
        if(num < 26) {
            result[i] = num + 'A';
        } else {
            result[i] = num - 26 + '0';
        }
    }

    return 0;
}

int encrypt(char *key, char *plaintext, char *result) {

    //Check if something went wrong with memory
    if(key == NULL || plaintext == NULL || result == NULL){
        printf("Encrypt failed\n");
        return FAILED;
    }

    printf("...Encrypting...\n");
    int len = strlen(plaintext);
    int plainNums[len];
    int keyNums[len];

    //Convert the char arrays into int arrays
    //so it is easier to make the alphabet
    //shifts
    for(int i = 0; i < len; i++) {

        //Number values will hold values 26-35
        //Letters will hold values 0-25
        if(isdigit(plaintext[i])) {
            plainNums[i] = plaintext[i] - '0' + 26;
        } else {
            plainNums[i] = plaintext[i] - 'A';
        }

        if(isdigit(key[i])) {
            keyNums[i] = key[i] - '0' + 26;
        } else {
            keyNums[i] = key[i] - 'A';
        }
    }

    for(int i = 0; i < len; i++) {

        //Mod by 36 since that's how long our alphabet is
        int num = (plainNums[i] + keyNums[i]) % 36;

        //Shift values based off num
        if(num < 26) {
            result[i] = num + 'A';
        } else {
            result[i] = num - 26 + '0';
        }
    }

    return 0;
}

void show_usage() {

    printf("\nThis is a cipher program that can encrypt and decrypt\
\nASCII text using the Vigenère substitution method.\
\n\nUsage: vigenere\
\n  -E Encrypt\
\n  -D Decrypt\
\n  -k Secret Key\
\n  -i Input file (-E plaintext | -D ciphertext)\
\n  -o Output file (-E ciphertext | -D plaintext)\
\n\nExamples:\
\n  vigenere -E -k 'MYKEY' -i 'plaintext.txt' -o 'ciphertext.txt'\
\n  vigenere -D -k 'MYKEY' -i 'ciphertext.txt' -o 'plaintext.txt'\n");

    return;
}
