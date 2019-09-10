/**
 * @file
 * Dieses Modul stellt allgemeine Definitionen zur Verfügung
 *
 * @author  Ulrike Griefahn
 * @date    2015-08-28
 */

#ifndef HUFFMAN_COMMON_H
#define HUFFMAN_COMMON_H
/* ------------------------------------------------------------------------ */

/* ===========================================================================
 * Includes
 * ======================================================================== */

#include <stdbool.h>
#include <stdlib.h>


/* ===========================================================================
 * Symbolische Konstanten
 * ======================================================================== */

/** Maixmale Länge des Dateinamens */
#define MAX_FILENAME 255


/* ===========================================================================
 * Aufzählungstypen
 * ======================================================================== */

/**
 * Der Typ ist auf 0 und 1 beschränkt.
 */
typedef enum
{
    BIT0 = 0,
    BIT1 = 1
} BIT;

/**
 *  0 = erfolgreiche Durchführung
 *  1 = unbekannter Fehler
 *  2 = Fehler beim Aufruf
 *  3 = Ein-/Ausgabefehler
 *  4 = Fehler beim Komprimieren oder dekomprimieren
 */
enum {
    /* EXIT_SUCCESS, */
    /* EXIT_FAILURE, */
    EXIT_OPTION_ERROR = 2,
    EXIT_IO_ERROR,
    EXIT_DC_ERROR
};


/* ===========================================================================
 * Debug-Anweisungen
 * ======================================================================== */

#ifdef DEBUG
/** Präprozessor-Makro für Debugausgaben */
#include <stdio.h>
#define CPRINT(E) printf(#E " = %c\n", E)
#define DPRINT(E) printf(#E " = %d\n", E)
#define FPRINT(E) printf(#E " = %f\n", E)
#define SPRINT(E) printf(#E " = %s\n", E)
#define XPRINT(E) printf(#E " = %x\n", E)
#else
/** Präprozessor-Makro für Debugausgaben (nicht definiert) */
#define CPRINT(E)
/** Praeprozessor-Makro fuer Debugausgaben (decimal) */
#define DPRINT(E)
/** Praeprozessor-Makro fuer Debugausgaben (float) */
#define FPRINT(E)
/** Praeprozessor-Makro fuer Debugausgaben (string) */
#define SPRINT(E)
/** Praeprozessor-Makro fuer Debugausgaben (hex) */
#define XPRINT(E)
#endif

/* ------------------------------------------------------------------------ */
#endif	/* HUFFMAN_COMMON_H */

