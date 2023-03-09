
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
    //This lets us dynamically allocate memory for the texts
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
    char ch;
    int index = 0;
    while((ch = fgetc(input)) != EOF) {
        if(!isprint(ch)){break;}
        inputText[index] = ch;
        index++;
    }
    if(strlen(key) < strlen(inputText)) {

        char* newKey = (char*)malloc(sizeof(char) * (inputSize + 1));
        for(int i = 0; i < strlen(inputText); i++) {
           newKey[i] = key[i % strlen(key)];
        }
        key = newKey;
    }

    isEncrypt ? printf("...Encrypting now...\n") : printf("...Decrypting now...\n");
    printf("Key: %s %ld\nPlaintext %s %ld\n",key,strlen(key),inputText,strlen(inputText));
    fprintf(output,"%s +++ %s\n",key,inputText);
    fclose(input);
    fclose(output);
    /* Your program should exit zero if all goes well otherwise 1 */
    return 0;
}

int decrypt(char *key, char *ciphertext, char *result) {

    /* Add your code here */

    /* Your function should exit zero if all goes well otherwise 1 */
    return 1;
}

int encrypt(char *key, char *plaintext, char *result) {

    /* Add your code here */

    /* Your function should exit zero if all goes well otherwise 1 */
    return 1;
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
