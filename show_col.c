#include "helpers.h"
#include "uart1.h"
#include <time.h>
#define ARR_SIZE(arr) ( sizeof((arr)) / sizeof((arr[0])) )
#define MAX_SEQ 31
#define NO_INPUT -1
#define TIME_OUT 3
#define RED  0;
#define GREEN 1;
#define BLUE 2;
#define YELLOW 3;
#define MAGENTA 4;
#define CYAN 5;
#define WHITE 6;
uint8_t colours[4] = {0,1,2,3};
uint8_t simon_seq[MAX_SEQ];
uint8_t current_lvl;
uint8_t current_col;
char state = 0xFF;
char last_state = 0xFF;
int pos = 0;
int line = 0;
time_t seed;
__attribute__((constructor))  static void init()
{
	serial_init1();
	uart1_init();
	SYSTICK_InternalInit(1);
	SYSTICK_IntCmd(ENABLE);
	SYSTICK_Cmd(ENABLE);
	seed=time(NULL);
	srand(seed);	
}
int main(void)
{
	
	setup();
	//int key=key_pressed();
	/* Catches digits that are not valid */
	/*if((!(key <= 6)) || key == -1)
	{
		lcd_init();
		lcd_write_str("Please Enter a  Valid digit!",0,0,sizeof("please enter a  valid digita"));
	wait(2);
	lcd_init();
	lcd_write_str("Please Restart  to try again", 0,0, sizeof("please restart  to try again"));
	}*/
	show_seq();
}

void setup()
{
	/*  Basic setup of monitor, Keyboard, LCD Monitor, Lighting Module */
	lcd_init();
	lcd_write_str("Hello User", 0,0, sizeof("hello user"));
	memset(simon_seq, '2', MAX_SEQ);
}

void show_col(uint8_t col, uint8_t time)
	/* Displays requested colour for desired duration */
{
	switch(col)
	{
		case 0:
			dmx_write(255,0,0);
			wait(time);
			dmx_write(0,0,0);
			wait(time);
			break;
		case 1:
			dmx_write(0,255,0);
			wait(time);
			dmx_write(0,0,0);
			wait(time);
			break;
		case 2:
			dmx_write(0,0,255);
			wait(time);
			dmx_write(0,0,0);
			wait(time);
			break;
		case 3:
			dmx_write(255,255,0);
			wait(time);
			dmx_write(0,0,0);
			wait(time);
			break;
		case 4:
			dmx_write(255,0,255);
			wait(time);
			dmx_write(0,0,0);
			wait(time);
			break;
		case 5:
			dmx_write(0,255,255);
			wait(time);
			dmx_write(0,0,0);
			wait(time);
			break;
		case 6:
			dmx_write(255,255,255);
			wait(time);
			dmx_write(0,0,0);
			wait(time);
			break;
	}
}

int key_pressed()
{
	int key=-1;
	while(keypad_uint8_t_decode(last_state)==keypad_uint8_t_decode(state) && keypad_uint8_t_decode(state)=='G')
	{
		state=last_state;
		state=read_buttons();
	}
	char r = keypad_uint8_t_decode(state);
	if(!isdigit(r))
	{
		return key;
	}
 	key = r - '0';
	return key;
}

void show_seq()
{
	// Adjusts time based on level player is on
	int i;
	for (i=0;i<16; i++) //seq_len
	{
		simon_seq[i] = colours[rand() % 4 ;  
	}
	// simon_seq has array of colours(0to3)
	current_lvl=12;
	uint8_t time;
	if(current_lvl <= 5)
	{
		time=2;
	}
	else if(current_lvl <= 13)
	{
		time=1;
	}
	else
	{
		time=0.5;
	}
	int j;
	for(j=0;j<current_lvl; j++)
	{
		current_col=simon_seq[j]; // saves colour to be shown in variable
		show_col(simon_seq[j], time);
		wait(time);
	}
}







