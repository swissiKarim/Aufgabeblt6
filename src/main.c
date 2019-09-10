/**
 * @mainpage Huffman-Komprimierung
 * Dieses Projekt realisiert die Huffman-Kodierung. Es können beliebige Dateien
 * komprimiert und wieder dekompriniert werden.
 */

/**
 * @file
 * Hauptmodul der Huffman-Komprimierung
 * 
 * @author Ulrike Griefahn
 * @date 2017-12-01
 */


/* ===========================================================================
 * Includes 
 * ======================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Splint definiert S_SPLINT_S. Da die Splint-Prüfung von stat.h zum parse
 * error und Abbruch führt, wird der folgende Codeteil von der Splint-Prüfung 
 * ausgeklammert. 
 */
#ifndef S_SPLINT_S
#ifdef __unix__
#include <sys/stat.h>
#else
#include <sys\stat.h>
#endif
#endif

#include "huffman_common.h"
#include "huffman.h"
#include "io.h"

/* ===========================================================================
 * Datentypen
 * ======================================================================== */

/**
 * Ausführungsmodi
 */
typedef enum
{
    NO_MODE,
    HELP,
    COMPRESS,
    DECOMPRESS
} MODE;


/* ===========================================================================
 * Symbolische Konstanten
 * ======================================================================== */

/** Kommandozeilen-Option fuer das Komprimieren */
#define COMPRESS_OPTION "-c"

/** Kommandozeilen-Option fuer das Dekomprimieren */
#define DECOMPRESS_OPTION "-d"

/** Kommandozeilen-Option für die Ausgabedatei */
#define OUTFILE_OPTION "-o"

/** Kommandozeilen-Option für die Wahl der Komprimierungsstärke */
#define LEVEL_OPTION "-l"

/** Kommandozeilen-Option für die Ausgabe von Informationen */
#define VERBOSE_OPTION "-v"

/** Kommandozeilen-Option für die Unterdrückung des Ausgabe von Informationen */
#define HELP_OPTION "-h"

/** Minimaler Level für Komprimierung */
#define MIN_LEVEL 1

/** Maximaler Level für Komprimierung */
#define MAX_LEVEL 7

/** ---------------------------------------------------------------------- */
/** Dateiendung fuer die Ergebnisdatei, je nach Modus 'hc' oder 'hd' */
#define GET_STD_SUFFIX(MODE) (((MODE) == COMPRESS) ? ".hc" : ".hd")

/** Dateiendung fuer die Ergebnisdatei, je nach Modus 'hc' oder 'hd' */
#define GET_STD_SUFFIX_LEN(MODE) (((MODE) == COMPRESS) ? 3 : 3)

/** Standard-Level für Komrimierung */
#define STD_LEVEL 2

/** ---------------------------------------------------------------------- */

/** Fehlermeldung wenn Komprimierungsrichtung fehlt */
#define EMSG_MODE_MISSSING "Es wurde keine Komprimierungsrichtung angegeben."

/** Fehlermeldung wenn Ausgabedatei nicht angegeben wurde */
#define EMSG_OUTFILE_MISSING "Es wurde keine Ausgabedatei angegeben."

/** Fehlermeldung wenn Ausgabedatei nicht angegeben wurde */
#define EMSG_INVALID_OUTFILE "Namen von Ein- und Ausgabedatei sind gleich."

/** Fehlermeldung wenn Ausgabedatei nicht angegeben wurde */
#define EMSG_MISSING_LEVEL "Level für Komprimierung fehlt."

/** Fehlermeldung wenn Ausgabedatei nicht angegeben wurde */
#define EMSG_INVALID_LEVEL "Ungueltiger Level für Komprimierung."

/** Fehlermeldung fuer unbekannte Option */
#define EMSG_UNKNOWN_OPTION "Unbekannte Option."

/** Fehlermeldung fuer fehlende Pflichtoption */
#define EMSG_MISSING_OPTION "Zu wenige Parameter."


/* ===========================================================================
 * Globale Variablen
 * ======================================================================== */

/**
 * Name der Eingabedatei 
 */
static char in_filename[MAX_FILENAME + 1] = "";

/**
 * Name der Ausghabedatei, 3 Zeichen mehr, weil ggf. noch Platz für die 
 * Standard-Endung .hc bzw. .hd vorhanden sein muss.
 */
static char out_filename[MAX_FILENAME + 4] = "";

/**
 * Modus, in der das Programm ausgeführt werden soll
 */
static MODE mode = NO_MODE;

/**
 * Flag, über das festgelegt wird, ob eine Ausgabe zu Dateigrößen und 
 * Laufzeit erfolgen soll.
 */
static bool verbose = false;

/**
 * Level der Komprimierung, wird derzeit nicht verwendet.
 */
static int level = STD_LEVEL;


/* ===========================================================================
 * Funktionsprototypen
 * ======================================================================== */

/**
 * Reads and checks the given arguments
 * 
 * @param argc  number of given arguments 
 * @param argv  given arguments
 * @return      EXIT_SUCCESS if all arguments are correct, #EXIT_OPTION_ERROR 
 *              if the options include an error.
 */
static int read_arguments(int argc, char **argv);

/**
 * Prints usage information 
 */
static void print_help();

/**
 * If verbose is switched on, this function prints information about run time
 * and size of input and output files. If switched off, nothing ist done.
 * 
 * @param verbose   info is printed if set to true, 
 * @param prg_start cpu time at program start
 */
static void print_info(bool verbose, clock_t prg_start);


/* ===========================================================================
 * Funktionsdefinitionen
 * ======================================================================== */

/**
 * Hauptfunktion des Programms
 * 
 * @param argc
 * @param argv
 * @return 
 */
int main(int argc, char** argv)
{
    clock_t prg_start = clock();
    int exit_status = EXIT_SUCCESS;

    exit_status = read_arguments(argc, argv);

    if (exit_status == EXIT_SUCCESS)
    {
        switch (mode)
        {
        case COMPRESS:
            compress(in_filename, out_filename); /* level nicht verwendet */
            print_info(verbose, prg_start);
            break;

        case DECOMPRESS:
            decompress(in_filename, out_filename);
            print_info(verbose, prg_start);
            break;

        default:
            print_help();
            break;
        }
    }
    else
    {
        print_help();
    }

    return (exit_status);
}

static int read_arguments(int argc, char **argv)
{
    int i;
    int exit_status = EXIT_SUCCESS;
    bool resume = true;

    /* Sind genügend Argumente vorhanden? (mind. Eingabedatei) */
    if (argc < 2)
    {
        /* Nicht genügend Argumente*/
        fprintf(stderr, "[ERROR]: %s\n\n", EMSG_MISSING_OPTION);
        exit_status = EXIT_OPTION_ERROR;
    }
    else
    {
        /* Das letzte Argument ist der Name der Eingabedatei */
        strncpy(in_filename, argv[argc - 1], MAX_FILENAME);
        argc--;

        /* Argumente durchlaufen vom zweiten bis zum vorletzten */
        i = 1;
        while (i < argc && resume)
        {
            if (strcmp(argv[i], HELP_OPTION) == 0)
            {
                /* HELP_OPTION gefunden: Parameterprüfung beenden */
                mode = HELP;
                resume = false;
            }
            else if (strcmp(argv[i], OUTFILE_OPTION) == 0)
            {
                /* OUTFILE_OPTION gefunden, nächster Parameter ist Name der Datei */
                if (i + 1 < argc)
                {
                    strncpy(out_filename, argv[i + 1], MAX_FILENAME);
                    i++;
                }
                else
                {
                    fprintf(stderr, "[ERROR]: %s\n\n", EMSG_OUTFILE_MISSING);
                    exit_status = EXIT_OPTION_ERROR;
                }
            }
            else if (strcmp(argv[i], COMPRESS_OPTION) == 0)
            {
                mode = COMPRESS;
            }
            else if (strcmp(argv[i], DECOMPRESS_OPTION) == 0)
            {
                mode = DECOMPRESS;
            }
            else if (strcmp(argv[i], VERBOSE_OPTION) == 0)
            {
                verbose = true;
            }
            else if (strncmp(argv[i], LEVEL_OPTION, 2) == 0)
            {
                /* LEVEL_OPTION: nächste Zeichen bilden die Zahl des Levels */
                level = atoi(argv[i] + 2);

                /* Fehler bei Umwandlung der Zeichenkette */
                if (level == 0)
                {
                    fprintf(stderr, "[ERROR]: %s\n\n", EMSG_MISSING_LEVEL);
                    exit_status = EXIT_OPTION_ERROR;
                }
                else if (level < MIN_LEVEL || level > MAX_LEVEL)
                {
                    fprintf(stderr, "[ERROR]: %s\n\n", EMSG_INVALID_LEVEL);
                    exit_status = EXIT_OPTION_ERROR;
                }
            }
            else
            {
                fprintf(stderr, "[ERROR]: %s: %s\n\n", EMSG_UNKNOWN_OPTION, argv[i]);
                exit_status = EXIT_OPTION_ERROR;
            }

            i++;
        }

        if (mode == NO_MODE)
        {
            fprintf(stderr, "[ERROR]: %s\n\n", EMSG_MODE_MISSSING);
            exit_status = EXIT_OPTION_ERROR;
        }
        else
        {
            /* Standard-Ausgabedateinamen erstellen */
            if (strcmp(out_filename, "") == 0
                    && strlen(in_filename) < MAX_FILENAME - strlen(GET_STD_SUFFIX(mode)))
            {
                strncpy(out_filename, in_filename, MAX_FILENAME);
                strncat(out_filename, GET_STD_SUFFIX(mode), GET_STD_SUFFIX_LEN(mode));
            }

            /* Ein- und Ausgabedateiname vergleichen */
            if (strcmp(in_filename, out_filename) == 0)
            {
                fprintf(stderr, "[ERROR]: %s\n\n", EMSG_INVALID_OUTFILE);
                exit_status = EXIT_OPTION_ERROR;
            }
        }
    }

    SPRINT(in_filename);
    SPRINT(out_filename);
    DPRINT(mode);
    DPRINT(verbose);
    DPRINT(level);

    return exit_status;
}

static void print_help()
{
    printf("Usage: huffman <options> infilename\n"
           "  depending on options compresses oder decompresses infilename\n");

    printf("Options are:\n");
    printf("  -c           compress file (mandatory) \n");
    printf("  -d           decompress file (mandatory) \n"
           "                  if options -c and -d are both given, the latter\n"
           "                  determines the mode of execution\n");
    printf("  -l<level>    level (1-7) of compression (optional, default: 2) \n");
    printf("  -v           prints size of outfile and used time to de-/compress (optional) \n");
    printf("  -o <outfile> name of output file (optional)\n"
           "                  if option -o is not given, a standard suffix is added\n"
           "                  to the infilename: 'hc' in case of compression, 'hd' in\n"
           "                  case of decompression\n");
    printf("  -h           prints this help \n");
    printf("\n");

    printf("Exit states are:\n");
    printf("  0:           successful execution\n");
    printf("  1:           unspecified error\n");
    printf("  2:           error in some option\n");
    printf("  3:           file error\n");
    printf("  4:           error caused by compression/decompression\n\n");
}

static void print_info(bool verbose, clock_t prg_start)
{
#ifndef S_SPLINT_S

    if (verbose)
    {
        struct stat attribut;
        clock_t prg_end = clock();

        printf("\nAusfuehrungsstatistik\n");

        stat(in_filename, &attribut);
        printf(" - Groesse der Eingabedatei %s (byte): %lu\n",
               in_filename, (unsigned long) attribut.st_size);

        stat(out_filename, &attribut);
        printf(" - Groesse der Ausgabedatei %s (byte): %lu\n",
               out_filename, (unsigned long) attribut.st_size);

        printf(" - Die Programmlaufzeit betrug %.2f Sekunden\n",
               (float) (prg_end - prg_start) / CLOCKS_PER_SEC);

        printf("\n");
    }
#endif
}
