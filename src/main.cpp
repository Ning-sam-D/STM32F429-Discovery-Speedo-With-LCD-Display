#include <mbed.h>
#include <stdio.h>
#include "gyro.h"
#include "SPI_TFT_ILI9341.h"
#include "Arial12x12.h"


volatile bool startOrPause = false;
bool print_distance = false;
float distance_traveled = 0;
int entry_count = 0;

InterruptIn button(BUTTON1);
SPI_TFT_ILI9341 TFT(PF_9, PF_8, PF_7, PC_2, PC_12, PD_13); // mosi, miso, sclk, cs, reset, dc

EventQueue *queue = mbed_event_queue();
// global variable for location on screen 
int lcd_x = 0;
int lcd_y = 1;

void lcd_setup()
{
	TFT.claim(stdout); // send stdout to the TFT display
	TFT.background(Black);					   // set background to black
	TFT.foreground(White);					   // set chars to white
	TFT.cls();								   // clear the screen
	TFT.set_font((unsigned char *)Arial12x12); // select the font
	TFT.set_orientation(0);
}

void onOff()
{
	if (startOrPause)
	{
		print_distance = true;
		startOrPause = false;
	}
	else
	{
		print_distance = false;
		distance_traveled = 0; // wipe the value, restart
		entry_count = 0;
		startOrPause = true;

		lcd_x = 0;
		lcd_y = 11;
	}
}

// counting how many peaks are in a cycle
// storing the average peak value.
float degree_to_velocity(float leg_length, float sum)
{
	float len = leg_length;
	float average_angle = sum / 500 * 0.0174533; // average radian shift

	// distance equation: 2 * leg_length*sin(angle/2);
	float sin_value = std::sin(average_angle / 2);
	float distance = 2 * len * sin_value;
	return distance;
}

int main()
{
	lcd_setup();
	gyro_setup();

	button.rise(&onOff);
	while (1)
	{
		if (startOrPause)
		{
			if (entry_count < 40)
			{
				if(entry_count == 0) TFT.cls();
				entry_count++;

				int sum = sum_of_degree();
				float len = degree_to_velocity(1.2, sum) - (3/100); // setup leg length
				distance_traveled += len * 0.5;

				if ((lcd_y + 13) < 320)
				{
					TFT.locate(lcd_x, lcd_y);
					TFT.printf(" Speed:%3dcm/s", (int)(100 * len));
					lcd_y = lcd_y + 13;
				}

				if (print_distance || (entry_count == 40))
				{
					TFT.locate(50, 300);
					// lcd_y = lcd_y + 26;
					TFT.printf(" Distance:%3dcm", (int)(100 * distance_traveled));
					// lcd_y = lcd_y + 13;
				}

				if (lcd_y >= 299 || (entry_count == 20))
				{
					lcd_x = 120;
					lcd_y = 10;
				}
			}
		}
	}
}

