#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include "config.h"
#include "cipher.h"

#define BUFLEN 1024

char BUF[BUFLEN];

struct arguments {
    bool error;
    bool help;
    bool version;
    bool decrypt;
    char* subkey;
};

void print_usage(FILE* out) {
    out = !out ? stdout : out;
    fprintf(out, "%s -- Chiffre de playfair. Lis l'entrée standard et produit le cryptogramme\n", PRGNAME);
    fprintf(out, "%*ssur la sortie standard.\n\n", (int)strlen(PRGNAME)+4, "");
    fprintf(out, "SYNOPSYS\n");
    fprintf(out, "    %s [-hv] [-d]\n", PRGNAME);
    fprintf(out, "    \n");
    fprintf(out, "OPTIONS\n");
    fprintf(out, "    -h|--help\n");
    fprintf(out, "        Affiche ce texte d'aide.\n");
    fprintf(out, "    -v|--version\n");
    fprintf(out, "        Affiche la version du programme.\n");
    fprintf(out, "    -d|--decrypt\n");
    fprintf(out, "        Déchiffre le message.\n");
}

struct arguments parse_options(int argc, char* const argv[]) {
    struct arguments args = {};
    int choice;
    while (true) {
        static struct option long_options[] = {
            {"version", no_argument,	0,	'v'},
            {"help",	no_argument,	0,	'h'},
            {"decrypt", no_argument,    0,  'd'},
            {0,0,0,0}
        };

        int option_index = 0;
        choice = getopt_long( argc, argv, "vhd", long_options, &option_index);

        if (choice == -1)
            break;

        switch(choice) {
            case 'v':
                args.version = true;
                break;

            case 'h':
                args.help = true;
                break;

            case 'd':
                args.decrypt = true;
                break;

            default:
                /* Not sure how to get here... */
                return (struct arguments) {.error=true};
        }
    }

    /* Deal with non-option arguments here */
    if (optind < argc)
        args.subkey = argv[optind];

    return args;
}

int main(int argc, char *argv[]) {
    if (argc < 2)
        return 1;

    struct arguments args = parse_options(argc, argv);
    if (args.error) {
        fprintf(stderr, "erreur innattendue... Terminaison.\n");
        return 2;
    }
    if (args.help) {
        print_usage(stdout);
        return EXIT_SUCCESS;
    }
    else if (args.version) {
        printf("%s version %f\n", PRGNAME, PRGVERSION);
        return EXIT_SUCCESS;
    }

    if (!args.subkey) {
        fprintf(stderr, "erreur: la clef doit être spécifiée !\n");
        print_usage(stderr);
        return EXIT_FAILURE;
    }

    fill_key(args.subkey);

#if DEBUG==1
    const char* key = getkey();
    for (size_t i = 0; i < 25; ++i) {
        fprintf(stderr, "%c ", key[i]);
        if (i%5 == 4)
            fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
#endif

    char* text = malloc(sizeof(char));
    text[0] = '\0';
    while (fgets(BUF, BUFLEN, stdin)) {
        text = realloc(text, BUFLEN);
        strcpy(text, BUF);
    }

#if DEBUG==1
    fprintf(stderr, "Texte clair:\n");
    fprintf(stderr, "%s\n\n", text);
#endif

    if (args.decrypt) {
        printf("%s", decrypt(text));
    } else {
        convert_for_scheme_input(&text);
#if DEBUG==1
        fprintf(stderr, "Texte clair converti:\n");
        fprintf(stderr, "%s\n\n", text);
#endif
        printf("%s\n", encrypt(text));
    }

    return EXIT_SUCCESS;
}

/* vim: set sts=4 ts=4 sw=4 tw=120 et :*/

