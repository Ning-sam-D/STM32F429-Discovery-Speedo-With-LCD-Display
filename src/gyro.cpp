#include <mbed.h>
#include "gyro.h"
#include <math.h>

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

void read_data()
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

    int x = (float)(reading_X / 1000 * 8.75);
    int y = (float)(reading_Y / 1000 * 8.75);
    int z = (float)(reading_Z / 1000 * 8.75);

    printf("%d, %d, %d\n", x, y, z);
}

float read_data_x()
{
    int X_L = read_register(GYRO_X_L);
    int X_H = read_register(GYRO_X_H);
    int16_t reading_X = (X_H << 8) | (X_L);
    // float x = reading_X * 8.75 / 1000;
    return (float)(reading_X * 8.75 / 1000);
}

float read_data_y()
{
    int Y_L = read_register(GYRO_Y_L);
    int Y_H = read_register(GYRO_Y_H);
    int16_t reading_Y = (Y_H << 8) | (Y_L);
    // float y = reading_Y * 8.75 / 1000;
    return (float)(reading_Y * 8.75 / 1000);
}

float read_data_z()
{
    int Z_L = read_register(GYRO_Z_L);
    int Z_H = read_register(GYRO_Z_H);
    int16_t reading_Z = (Z_H << 8) | (Z_L);
    // float z = reading_Z * 8.75 / 1000;
    return (float)(reading_Z * 8.75 / 1000);
}

float sum_of_degree()
{
    float sum = 0;
    for (int i = 0; i < 500; i++)
    {
        float x = read_data_x();
        float abs_value = std::abs(x);
        sum += abs_value;
        wait_us(1000);
    }
    return sum;
}