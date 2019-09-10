/* ============================================================================
 * Includes
 * ========================================================================= */

#include "huffman_common.h"
#include "io.h"
#include "huffman.h"


/* ============================================================================
 * Funktions-Prototypen
 * ========================================================================= */

/**
 * Kopiert den Inhalt der Eingabedatei in die Ausgabedatei (byteweise)
 */
static void do_copy_chars(void);

/**
 * Kopiert den Inhalt der Eingabedatei in die Ausgabedatei (bitweise)
 */
static void do_copy_bits(void);


/* ============================================================================
 * Funktions-Definitionen
 * ========================================================================= */

extern void compress(char in_filename[], char out_filename[])
{
    open_infile(in_filename);
    open_outfile(out_filename);
    
    do_copy_chars();
    
    close_infile();
    close_outfile();
}

extern void decompress(char in_filename[], char out_filename[])
{
    open_infile(in_filename);
    open_outfile(out_filename);
    
    do_copy_bits();
    
    close_infile();
    close_outfile();
}

static void do_copy_chars(void)
{
    unsigned char c;
    
    while (has_next_char())
    {
        c = read_char();
        write_char(c);
    }
}

static void do_copy_bits(void)
{
    int bit;
    
    while (has_next_bit())
    {
        bit = read_bit();    
        write_bit(bit);
    }
}
