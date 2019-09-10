/**
 * @file
 * In diesem Modul werden die Huffman-Komprimierung und die 
 * Huffman-Dekomprimierung realisiert.
 *
 * @author Ulrike Griefahn
 * @date 2017-01-12
 */

#ifndef HUFFMAN_H
#define	HUFFMAN_H
/* ------------------------------------------------------------------------- */


/* ============================================================================
 * Funktions-Prototypen
 * ========================================================================= */

/**
 * Komprimiert den Inhalt der Eingabedatei in_filename und schreibt das 
 * Ergebnis in die Ausgabedatei out_filename. Bei einem Fehler wird das 
 * Programm abgebrochen.
 * 
 * @param in_filename   Name der Eingabedatei
 * @param out_filename  Name der Ausgabedatei
 */
extern void compress(char in_filename[], char out_filename[]);

/**
 * Dekomprimiert den Inhalt der Eingabedatei in_filename und 
 * schreibt das Ergebnis in die Ausgabedatei out_filename. Bei einem Fehler
 * wird das Programm abgebrochen.
 * 
 * @param in_filename   Name der Eingabedatei
 * @param out_filename  Name der Ausgabedatei
 */
extern void decompress(char in_filename[], char out_filename[]);

/* ------------------------------------------------------------------------- */
#endif	/* HUFFMAN_H */

