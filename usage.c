
#include <stdio.h>
#include <unistd.h>

int decrypt(char *, char *, char *);
int encrypt(char *, char *, char *);
void show_usage(void);

int main(int argc, char **argv) {

    /* Add your code here to parse the command line arguments */

    show_usage();

    for (int i = 0; i < argc; i++) {

        printf("This is argument #%d: %s\n",i,argv[i]);
    }
    /* Your program should exit zero if all goes well otherwise 1 */
    return 1;
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
