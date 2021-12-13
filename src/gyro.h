#ifndef GYRO_H_INCLUDED
#define GYRO_H_INCLUDED

#define GYRO_ADDR 0b110100
#define GYRO_WHO_AM_I 0x0F
#define GYRO_CTRL_REG1 0x20
#define GYRO_CTRL_REG2 0x21
#define GYRO_CTRL_REG3 0x22
#define GYRO_CTRL_REG4 0x23
#define GYRO_CTRL_REG5 0x24
#define FIFO_CTRL_REG 0x2E
#define GYRO_X_L 0x28
#define GYRO_X_H 0x29
#define GYRO_Y_L 0x2A
#define GYRO_Y_H 0x2B
#define GYRO_Z_L 0x2C
#define GYRO_Z_H 0x2D

void gyro_setup();
int read_register(int address);
void write_register(int address, int data);

void read_data();

float read_data_x();
float read_data_y();
float read_data_z();

// float* read_data_array();

#endif
