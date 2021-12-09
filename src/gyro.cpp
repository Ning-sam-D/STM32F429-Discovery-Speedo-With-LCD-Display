#include <mbed.h>
#include "gyro.h"

SPI gyro_spi(PF_9, PF_8, PF_7); // MOSI, MISO, SCK
DigitalOut gyro_cs(PC_1);

void write_register(int address, int data)
{
    gyro_cs = 0;
    gyro_spi.write(0x00 | address);
    gyro_spi.write(data);
    gyro_cs = 1;
}

int read_register(int address)
{
    gyro_cs = 0;
    gyro_spi.write(0x80 | address);
    int data = gyro_spi.write(0x00); // write in random value
    gyro_cs = 1;
    return data;
}

void gyro_setup()
{
    gyro_cs = 1;
    gyro_spi.format(8, 3);
    gyro_spi.frequency(1000000);
    write_register(FIFO_CTRL_REG, 0);     // bypass mode
    write_register(GYRO_CTRL_REG1, 0x8F); // power on, output data rate 400 Hz, Cutoff 20
    write_register(GYRO_CTRL_REG4, 0);    // setup 4-wire interface
    write_register(GYRO_CTRL_REG5, 0x08); // set up low pass filter
}

void read_data(float &x, float &y, float &z)
{
    int X_L = read_register(GYRO_X_L);
    int X_H = read_register(GYRO_X_H);
    int16_t reading_X = (X_H << 8) | (X_L);

    int Y_L = read_register(GYRO_Y_L);
    int Y_H = read_register(GYRO_Y_H);
    int16_t reading_Y = (Y_H << 8) | (Y_L);

    int Z_L = read_register(GYRO_Z_L);
    int Z_H = read_register(GYRO_Z_H);
    int16_t reading_Z = (Z_H << 8) | (Z_L);

    x = (float) (reading_X / 1000 * 8.75);
    y = (float) (reading_Y / 1000 * 8.75);
    z = (float) (reading_Z / 1000 * 8.75);

    // int x = (int) (reading_X / 1000 * 8.75);
    // int y = (int) (reading_Y / 1000 * 8.75);
    // int z = (int) (reading_Z / 1000 * 8.75);

    printf("%d, %d, %d\n", x, y, z);
    // printf("%3.2f, %3.2f, %3.2f\n", x, y, z);
    // printf("0x%X, 0x%X, 0x%X\n", reading_X, reading_Y, reading_Z);
}

