#include<stdio.h>
#include <stdlib.h>
#include <string.h>



// val, toDeci, reVal, strev and fromDeci are functions from https://www.geeksforgeeks.org/convert-base-decimal-vice-versa/ 
//with few modifications


int val(char c)
{
    return (int)c - 'a';
}

int toDeci(char* str, int base)
{
    int len = 3;
    int power = 1; // Initialize power of base
    int num = 0;  // Initialize result
    int i;

    // Decimal equivalent is str[len-1]*1 +
    // str[len-2]*base + str[len-3]*(base^2) + ...
    for (i = len - 1; i >= 0; i--)
    {
        // A digit in input number must be
        // less than number's base
        if (val(str[i]) >= base)
        {
            printf("Invalid Number");
            return -1;
        }

        num += val(str[i]) * power;
        power = power * base;
    }

    return num;
}

char reVal(int num)
{
    return (char)(num + 'a');
}

void strev(char* str)
{

    printf("%d", strlen(str));
    int len = 3;
    int i;
    for (i = 0; i < len / 2; i++)
    {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void fromDeci(char* res, int base, int inputNum)
{
    int index = 0;  // Initialize index of result
    printf("New is %d\n", inputNum);
    // Convert input number is given base by repeatedly
    // dividing it by base and taking remainder

    res[0] = 'a';
    res[1] = 'a';
    res[2] = 'a';

    while (inputNum > 0)
    {
        res[index++] = reVal(inputNum % base);
        inputNum /= base;
    }

    //res[3] = '\0';
    // Reverse the result
    strev(res);
}


//create a binary file from the txt to be able to stream data (not to save all the text in memory)
void createBinary(char* fileName) {
    FILE* fp, * f;
    char c;
    int data;
    fp = fopen("p059_cipher.txt", "r");
    f = fopen("junk", "w");
    if (!f || !fp)
        exit(1);
    while (!feof(fp)) {
        fscanf(fp, "%d%c", &data, &c);
        fwrite(&data, sizeof(int), 1, f);
        //printf("%d", data);
    }

    fclose(fp);
    fclose(f);
}

//helper function to add one lowerletter character on the key we are testing aaa -> aab -> aac ...
void iterate3characterstring(char* c) {
    printf("KEY IS %d", strlen(c));
    //change the three leter key to a number in base 26, add one, then change it back
    printf("from iterate Character c is\n");
    int deci = toDeci(c, 26);
    printf("%d\n", deci);
    if (deci >= (17576 - 1)) { //zzz
        perror("no key worked");
        exit(1);
    }
    deci++;
    fromDeci(c, 26, deci);
}


int main() {
    //create the binary file
    createBinary("p059_cipher.txt");

    int WORD_SIZE = 4;
    char word[4] = "what";


    //loop over all 3 letter keys and in each itteration we save the decrypted file in a new binary file (decrypted) and then we loop over the words to check if we can read it.
    FILE* f, * d;
    f = fopen("junk", "r+");
    d = fopen("decrypted", "w+");
    char key[3] = "aaa";

    int data[3];

    int result = 0;
    //printf("finished");
    while (result == 0) {

        printf("Trying key: %s\n", key);
        printf("\n");

        while (!feof(f)) {

            fread(&data, sizeof(int), 3, f);
            printf("%d %d %d\n", data[0], data[1], data[2]);
            for (int i = 0;i < 3;i++) {
                data[i] = data[i] ^ key[i];
            }
            fwrite(&data, sizeof(int), 3, d);
        }

        int data2[WORD_SIZE];


        fseek(d, 0L, SEEK_SET);
        fread(&data2, sizeof(int), WORD_SIZE, d);
        //printf("| %d %d %d\n", data2[0], data2[1], data2[2]);


        int count = 0;
        while (!feof(d))
        {
            result = 1;
            for (int i = 0; i < WORD_SIZE; i++) {
                if ((int)word[i] != data2[(i + count) % WORD_SIZE]) {
                    result = 0;
                }
            }
            if (result == 1) {
                //we found a word with the key then we translate the decrypted file to a message 
                printf("we found the word with key:");
                puts(key);
                FILE* ot;
                ot = fopen("originalText", "w+");
                while (!feof(d)) {
                    int ascii;
                    fread(&ascii, sizeof(int), 1, d);
                    fputc(ascii, ot);
                }
            }

            fread(&data2[count], sizeof(int), 1, d);
            //printf("|| %c %c %c\n", data2[0], data2[1], data2[2]);
            count++;
            count %= WORD_SIZE;
        }

        printf("%s is not the key\n", key);
        iterate3characterstring(key);
    }




    fclose(f);
    fclose(d);
    return 0;
}

