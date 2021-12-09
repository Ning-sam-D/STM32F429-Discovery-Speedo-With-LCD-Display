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
    float x,y,z;
    read_data(x, y, z);

    wait_ms(100);
  }
}
