#include <mbed.h>
#include "gyro.h"

void wait_ms(uint32_t ms)
{
  wait_us(1000 * ms);
}

void degree_to_radian();

int main()
{
  gyro_setup();

  while (1)
  {
    for (int a = 0; a <= 1000; a++){
      // float x,y,z;
      // read_data(x, y, z);
      // read_data();
      float x = read_data_x();
      float y = read_data_y();
      float z = read_data_z();
      printf("Main: %d, %d, %d\n", (int) x, (int) y, (int)z);
      wait_ms(100);
    }
  }
}
