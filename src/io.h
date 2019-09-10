/**
 * @file
 * In diesem Modul wird der Dateizugriff realisiert. Die zu lesenden und
 * zu schreibenden Daten werden gepuffert, so dass blockweise gelesen und 
 * geschrieben wird. Das Modul bietet Funktionen an, um bit- und byteweise 
 * zu lesen und zu schreiben.
 *
 * @author Ulrike Griefahn
 * @date 2017-12-01
 */

#ifndef IO_H
#define	IO_H
/* ------------------------------------------------------------------------- */

/* ============================================================================
 * Funktions-Prototypen
 * ========================================================================= */

/**
 * Oeffnet die uebergebene Datei zum Lesen
 * 
 * @param filename zu oeffnende Datei
 * @return  liefert den Eingabestrom oder bricht das Programm ab, wenn die 
 *          Datei nicht geoeffnet werden konnte.
 */
extern void open_infile(char filename[]);

/**
 * Schliesst die zum Lesen geoeffnete Datei oder bricht das Programm ab, wenn die 
 *          Datei nicht geschlossen werden konnte.
 * @return 
 */
extern void close_infile(void);

/**
 * Oeffnet die uebergebene Datei zum Schreiben
 * 
 * @param filename zu oeffnende Datei
 * @return  liefert den Ausgabestrom oder bricht das Programm ab, wenn die 
 *          Datei nicht geoeffnet werden konnte.
 */
extern void open_outfile(char filename[]);

/**
 * Schliesst die zum Schreiben geoeffnete Datei oder bricht das Programm ab, 
 *          wenn die Datei nicht geschlossen werden konnte.
 * @return 
 */
extern void close_outfile(void);

/**
 * Liefert true, wenn noch mindestens ein weiteres Zeichen vorhanden ist.
 * 
 * @return 
 */
extern bool has_next_char(void);

/**
 * Liefert das nächste Zeichen aus dem Eingabestrom
 * @return 
 */
extern unsigned char read_char(void);

/**
 * Schreibt das Zeichen in den Ausgabestrom
 * 
 * @param c 
 */
extern void write_char(unsigned char c);

/**
 * Liefert true, wenn noch mindestens ein weiteres Bit vorhanden ist.
 * 
 * @return 
 */
extern bool has_next_bit(void);

/**
 * Liefert das nächste Bit aus dem Eingabestrom
 * @return 
 */
extern BIT read_bit(void);

/**
 * Schreibt das Zeichen c, das nur den Zahlwert 0 oder 1 haben darf in den 
 * Ausgabestrom
 * 
 * @param c 
 */
extern void write_bit(BIT);

/* ------------------------------------------------------------------------- */
#endif	/* IO_H */
