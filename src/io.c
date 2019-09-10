/* ============================================================================
 * Includes
 * ========================================================================= */

#include <stdlib.h>
#include <stdio.h>

/* Definiere Variablen, damit sie in dieser Datei für Splint bekannt sind. Sie
 * werden in errno.h definiert. */
#define ENOENT 0
#define EIO 0
#define EMFILE 0
#include <errno.h>

#include "huffman_common.h"
#include "io.h"


/* ============================================================================
 * Symbolische Konstanten
 * ========================================================================= */

/**
 * Blockgröße, mit der aus der Eingabedatei gelesen und in die Ausgabedatei 
 * geschrieben wird.
 */
#define BUF_SIZE 4096


/* ============================================================================
 * Makros
 * ========================================================================= */

/**
 * Liefert das Bit von C an der Position POS. Ist POS gleich 0, wird  
 * das höchstwertigste Bit geliefert, für POS gleich 7 wird das 
 * niederwertigste Bit geliefert. Es wird davon ausgegangen, dass POS einen
 * Wert zwischen 0 und 7 hat. Ansonsten ist die Rückgabe undefiniert.
 */
#define GET_BIT(C, POS) ((C) >> (7 - (POS)) & (unsigned char) 0x01)

/**
 * Schreibt das Bit BIT an der Position POS in das Zeichen C. Ist POS 
 * gleich 0, wird das höchstwertigste Bit geliefert, für POS gleich 7 wird  
 * das niederwertigste Bit geliefert. Es wird davon ausgegangen, dass POS 
 * einen Wert zwischen 0 und 7 hat. Ansonsten ist die Rückgabe undefiniert.
 */
#define PUT_BIT(C, BIT, POS) ((C) | ((unsigned char) (BIT) << (7 - (POS))))


/* ============================================================================
 * Funktions-Prototypen
 * ========================================================================= */

/**
 * Gibt den Fehler aus, der beim Versuch eine Datei zu oeffnen oder zu schliessen,
 * aufgetreten ist, und bricht dann das Programm ab.
 */
static void report_error_and_exit(void);


/* ============================================================================
 * Globale Variablen
 * ========================================================================= */

/** Eingabestrom */
static FILE *in_stream;

/** Puffer für den Eingabestrom */
static unsigned char in_buffer[BUF_SIZE];

/** Enthält die erste freie Position des Puffers nach dem letzten Zeichen */
static int last_in_pos;

/** Aktuelle Position im Eingabepuffer */
static int curr_in_pos;

/** Aktuelle Bit-Position im aktuellen Zeichen */
static int curr_pos_in_bit;

/** Ausgabestrom */
static FILE *out_stream;

/** Puffer für den Ausgabestrom */
static unsigned char out_buffer[BUF_SIZE];

/** Nächste freie Position im Ausgabepuffer */
static int last_out_pos;


/* ============================================================================
 * Funktions-Definitionen
 * ========================================================================= */

/* ----------------------------------------------------------------------------
 * Oeffnen und Schliessen von Dateien
 * ------------------------------------------------------------------------- */

extern void open_infile(char filename[])
{
    errno = 0;
    in_stream = fopen(filename, "rb");
    if (in_stream == NULL)
    {
        report_error_and_exit();
    }
    last_in_pos = (int) fread(in_buffer, sizeof (unsigned char),
                                     BUF_SIZE, in_stream);
    curr_in_pos = 0;
    curr_pos_in_bit = 8;
}

extern void close_infile(void)
{
    errno = 0;
    if (fclose(in_stream) == EOF)
    {
        report_error_and_exit();
    };
}

extern void open_outfile(char filename[])
{
    errno = 0;
    out_stream = fopen(filename, "wb");
    if (out_stream == NULL)
    {
        report_error_and_exit();
    }
    last_out_pos = 0;
}

extern void close_outfile(void)
{
    errno = 0;
    (void) fwrite(out_buffer, sizeof(unsigned char), (size_t) last_out_pos, out_stream);
    if (fclose(out_stream) == EOF)
    {
        report_error_and_exit();
    };
}


/* ----------------------------------------------------------------------------
 * Byteweises Lesen und Schreiben
 * ------------------------------------------------------------------------- */

extern bool has_next_char(void)
{
    /* Buffer erneut füllen, falls letztes Zeichen ausgelesen */
    if (curr_in_pos >= last_in_pos)
    {
        last_in_pos = (int) fread(in_buffer, sizeof(unsigned char), 
                                         BUF_SIZE, in_stream);
        curr_in_pos = 0;
    }
    
    return curr_in_pos < last_in_pos;
}

extern unsigned char read_char(void)
{
    /* Nächstes Zeichen aus dem Buffer lesen */
    unsigned char c = in_buffer[curr_in_pos];
    curr_in_pos++;

    return c;
}

extern void write_char(unsigned char c)
{
    /* 
     * Schreibt das Zeichen in den Puffer, bis dieser voll ist. Ist dieser voll,
     * wird der Inhalt des Puffers in die Ausgabedatei geschrieben und der 
     * Puffer erneut gefuellt.
     */

    /* Zeichen an nächste freie Pufferposition schreiben */
    out_buffer[last_out_pos] = c;
    last_out_pos++;

    /* Vollen Puffer zuerst schreiben */
    if (last_out_pos >= BUF_SIZE)
    {
        (void) fwrite(out_buffer, sizeof(unsigned char), BUF_SIZE, out_stream);
        last_out_pos = 0;
    }
}

/* ----------------------------------------------------------------------------
 * Bitweises Lesen und Schreiben
 * ------------------------------------------------------------------------- */

extern bool has_next_bit(void)
{
    return curr_pos_in_bit < 8 || has_next_char();
}

extern BIT read_bit(void)
{
    /* 
     * Liest so lange aus dem Puffer, bis dieser leer ist. Ist dieser leer,
     * wird der Inhalt des Puffers aus der Eingabedatei erneut gefuellt.
     */

    /* Das aktuelle Zeichen */
    static int c = EOF;

    /* das aktuelle Bit */
    BIT bit;
    
    /* das allererste Zeichen holen, weil statische Variable nicht mit 
     * Rückgabe einer Funktion initialisiert werden kann */
    if (curr_pos_in_bit % 8 == 0 && has_next_char())
    {
        c = read_char();
        curr_pos_in_bit = 0;
    }
    
    /* Bit aus dem aktuellen Zeichen auslesen und weitersetzen */
    bit = GET_BIT(c, curr_pos_in_bit);
    curr_pos_in_bit++;
    
    return bit;
}

extern void write_bit(BIT bit)
{
    /* 
     * Schreibt so lange in den Puffer, bis dieser voll ist. Ist dieser voll,
     * wird der Inhalt des Puffers in die Ausgabedatei geschrieben und der 
     * Puffer erneut gefüllt.
     */

    /* das aktuelle Zeichen */
    static unsigned char c;
    
    /* die aktuelle Bit-Position im aktuellen Zeichen */
    static int bit_pos = 0;
    
    /* Bit im aktuellen Zeichen setzen */
    c = PUT_BIT(c, bit, bit_pos);
    bit_pos++;
    
    /* Wenn Byte voll, dann Zeichen schreiben */
    if (bit_pos == 8)
    {
        write_char((unsigned char) c);
        bit_pos = 0;
        c = '\0';
    }
}

/* ----------------------------------------------------------------------------
 * Fehlerbehandlung
 * ------------------------------------------------------------------------- */

static void report_error_and_exit(void)
{
    switch (errno)
    {
    case ENOENT:
        fprintf(stderr, "[ERROR] (%d): No such file or directory.\n", errno);
        break;
    case EIO:
        fprintf(stderr, "[ERROR] (%d): I/O error.\n", errno);
        break;
    case EMFILE:
        fprintf(stderr, "[ERROR] (%d): Too many open files.\n", errno);
        break;
    default:
        fprintf(stderr, "[ERROR] (%d): unknown error.\n", errno);
        break;
    }

    exit(EXIT_IO_ERROR); /* Todo Richtigen Fehlercode eintragen */
}
