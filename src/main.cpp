#include <mbed.h>
#include "gyro.h"

void wait_ms(uint32_t ms)
{
	wait_us(1000 * ms);
}

// counting how many peaks are in a cycle
// storing the average peak value.
float degree_to_velocity(float leg_length, float sum)
{
	float len = leg_length;
	float average_angle = sum / 500 * 0.0174533; // average radian shift

  	// distance equation: 2 * leg_length*sin(angle/2);
	float sin_value = std::sin(average_angle / 2);
  	// printf("sin value: %d\n", (int)(100 * sin_value)); // to read the value of sin
	float distance = 2 * len * sin_value;
	return distance;
}

float distance_traveled = 0;

int main()
{
	gyro_setup();

	while (1)
  	{
		int sum = sum_of_degree();
		float len = degree_to_velocity(1.2, sum); // setup leg length
		distance_traveled += len * 0.5;
		printf("Speed: %d cm/s, Distance: %d cm\n", (int)(100 * len), (int)(100*distance_traveled));
  	}
}