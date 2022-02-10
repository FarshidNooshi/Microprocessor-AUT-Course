#include <SPI.h>
#include <Wire.h>
#include <math.h>

#define uint_64 unsigned long long

#define MISO 3 // shift clock          is PB3
#define MOSI 2 // Master In Slave Out  is PB2
#define SCK 1  // Master Out Slave In  is PB1
#define SS 0   // Slave Select         is PB0

#define SLAVE_SELECT 53

char buf[256] = {'\0'}, reader[256] = {'\0'}, wr_buf[256] = {'o', 'k', '\0'};
volatile boolean process = false;
volatile int pos = 0;

void spi_begin_slave()
{
    DDRB &= ~(1 << MOSI) & ~(1 << SCK) & ~(1 << SS); // 1 -> output, 0 -> input
    DDRB |= (1 << MISO);
    SPCR = (1 << SPE) | (1 << SPIE); // slave, spi
}

void setup()
{
    pinMode(MISO, OUTPUT);
    Serial.begin(9600);
    Serial.println("SPI SLAVE_2:");
    spi_begin_slave();
}

void move_buffer_data()
{
    memset(reader, 0, sizeof(reader));
    sprintf(reader, "%s", buf);
    memset(buf, 0, sizeof(buf));
    pos = 0;
    process = true;
}

ISR(SPI_STC_vect)
{
    if (digitalRead(SLAVE_SELECT) == LOW)
    {
        buf[pos] = SPDR; // data received from master is stored in SPDR
        pos++;

        if (pos < sizeof(buf))
        {
            if (buf[pos - 1] == '\n')
            {
                move_buffer_data();
            }
        }
        else
        {
            move_buffer_data();
        }

        SPDR = wr_buf[pos - 1];
        while (!(SPSR & (1 << SPIF))) // ta zamani ke khali nashode write kone, status register 1 -> rikhte, 0 -> narikhte
            ;
    }
}

void loop()
{

    if (process)
    {
        Serial.println(reader);
        process = false;
    }
}
