#include <avr/io.h>
#include "communication/spi_master.hpp"

namespace SPI
{
    void initialize_master()
    {
        /* Set MOSI and SCK output */
        DDRB |= (1 << DDB3) | (1 << DDB5);
        /* Enable SPI, Master, set clock rate fck/2, SPI_MODE_3 */ 
        SPCR = (1 << SPE) | (1 << MSTR) | (1 << CPOL) | (1 << CPHA);
        SPSR |= (1 << SPI2X);
    }

    void send(uint8_t byte)
    {
        /* Start transmission */
        SPDR = byte;
        // A small delay that can actually make it faster (from Arduino source code)
        asm volatile("nop");
        /* Wait for transmission complete */
        while (!(SPSR & (1 << SPIF)))
            ;
    }
}