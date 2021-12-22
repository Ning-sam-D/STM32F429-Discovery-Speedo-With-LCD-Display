#include <mbed.h>
#include <stdio.h>
#include "gyro.h"
/*----- uncomment this -----*/
// #include "SPI_TFT_ILI9341.h"
// #include "Arial12x12.h"
// SPI_TFT_ILI9341 TFT(PF_9, PF_9, PF_7, PC_2, PE_5, PE_5);
/*----- uncomment end -----*/

InterruptIn button(USER_BUTTON);
volatile bool startOrPause = false;

void onOff()
{
	if (startOrPause)
	{
		startOrPause = false;
		distance_traveled = 0;
	}
	else startOrPause = true;
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
	/*----- uncomment this -----*/
	// TFT.claim(stdout);      // send stdout to the TFT display 
    // //TFT.claim(stderr);      // send stderr to the TFT display
    // TFT.background(Black);    // set background to black
    // TFT.foreground(White);    // set chars to white
    // TFT.cls();                // clear the screen
    // TFT.set_font((unsigned char*) Arial12x12);  // select the font
    // TFT.set_orientation(0);
    // printf("  Hello Mbed 0");
	/*----- uncomment end -----*/


	gyro_setup();

	while (1)
	{
		if (startOrPause)
		{
			int sum = sum_of_degree();
			float len = degree_to_velocity(1.2, sum); // setup leg length
			distance_traveled += len * 0.5;
			printf("Speed: %d cm/s, Distance: %d cm\n", (int)(100 * len), (int)(100 * distance_traveled));
    	}
  	}
}