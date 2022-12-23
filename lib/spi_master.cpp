#include <avr/io.h>

namespace SPI
{
    void initialize_master(bool set_cpol, bool set_cpha)
    {
        /* Set MOSI and SCK output, all others input */
        DDRB = (1 << DDB3) | (1 << DDB5);
        /* Enable SPI, Master, set clock rate fck/2 */
        SPCR =  (1 << SPE) | 
                (1 << MSTR) | 
                (1 << SPI2X) | 
                set_cpol ? (1 << CPOL) : 0 | 
                set_cpha ? (1 << CPHA) : 0;
    }

    void send(char byte)
    {
        /* Start transmission */
        SPDR = byte;
        /* Wait for transmission complete */
        while (!(SPSR & (1 << SPIF)))
            ;
    }
}
