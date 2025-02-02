#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>

#include "eff_word_list.h"

// underlying RNG functions

static size_t GetRandom(void)
{
    size_t result;
    getrandom(&result, sizeof(size_t), GRND_RANDOM);
    return result;
}

static unsigned int RandomInt(unsigned int range)
{
    size_t maxUniform = SIZE_MAX - (SIZE_MAX % range);
    
    size_t random;
    do
    {
        random = GetRandom();
    }
    while (random >= maxUniform);

    return (unsigned int) (random % range);
}

// random passwords

static void PrintRandomDigits(unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        char digit = '0' + RandomInt(10);
        printf("%c", digit);
    }
}

static void PrintRandomLetters(unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        char letter = 'a' + RandomInt(26);
        printf("%c", letter);
    }
}

static void PrintRandomAlphanumerics(unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        int random = RandomInt(36);
        
        char alphanum;
        if (random < 10)
        {
            alphanum = '0' + random;
        }
        else
        {
            int letterIndex = random - 10;
            alphanum = 'a' + letterIndex;
        }

        printf("%c", alphanum);
    }
}

static void PrintRandomWords(unsigned int count)
{
    for (unsigned int i = 0; i < count; i++)
    {
        if (i > 0)
        {
            printf(" ");
        }

        int index = RandomInt(WORD_COUNT);
        const char *word = wordList[index];
        printf("%s", word);
    }
}

// main

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: pass-gen <type> <count>\n");
        return -1;
    }

    enum {NUMERIC, LETTERS, ALPHANUMERIC, WORDS} passType;
    if ((strcmp(argv[1], "digits") == 0)
        || (strcmp(argv[1], "num") == 0)
        || (strcmp(argv[1], "numerals") == 0)
        || (strcmp(argv[1], "numeric") == 0))
    {
        passType = NUMERIC;
    }
    else if ((strcmp(argv[1], "alpha") == 0)
             || (strcmp(argv[1], "alphabet") == 0)
             || (strcmp(argv[1], "letters") == 0))
    {
        passType = LETTERS;
    }
    else if ((strcmp(argv[1], "alphanum") == 0)
             || (strcmp(argv[1], "alphanumeric") == 0))
    {
        passType = ALPHANUMERIC;
    }
    else if (strcmp(argv[1], "words") == 0)
    {
        passType = WORDS;
    }
    else
    {
        fprintf(stderr, "pass-gen: unrecognised type. Valid types are\n");
        fprintf(stderr, "\tdigits, num, numerals, numeric\n");
        fprintf(stderr, "\talpha, alphabet, letters\n");
        fprintf(stderr, "\talphanum, alphanumeric\n");
        fprintf(stderr, "\twords\n");
        return -1;
    }

    char *endptr;
    long count = strtol(argv[2], &endptr, 10);
    if (endptr[0] != '\0')
    {
        fprintf(stderr, "pass-gen: count must be a valid positive integer.\n");
        return -1;
    }

    if (count <= 0)
    {
        fprintf(stderr, "pass-gen: count must be a positive integer.\n");
        return -1;
    }

    if ((count == LONG_MAX) || (count > UINT_MAX))
    {
        fprintf(stderr, "pass-gen: count is out of bounds.\n");
        return -1;
    }

    switch (passType)
    {
        case NUMERIC: PrintRandomDigits(count); break;
        case LETTERS: PrintRandomLetters(count); break;
        case ALPHANUMERIC: PrintRandomAlphanumerics(count); break;
        case WORDS: PrintRandomWords(count); break;
    }

    printf("\n");
    return 0;
}
