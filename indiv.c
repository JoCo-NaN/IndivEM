#include "helpers.h"
#include "uart1.h"
#define ARR_SIZE(arr) ( sizeof((arr)) / sizeof((arr[0])) )
#define MAX_SEQ 31
#define NO_INPUT -1
#define TIME_OUT 3
#define DEBOUNCE 250 //Or use wait()
#define THRESHOLD 10
#define RED  0
#define GREEN 1
#define BLUE 2
#define YELLOW 3
#define MAGENTA 4
#define CYAN 5
#define WHITE 6
uint8_t simon_seq[MAX_SEQ];
uint8_t current_lvl;
uint8_t current_col;
uint8_t colours[4] = {0,1,2,3};
int difficulty=1; // Set to 1 by default
uint8_t seq_len;
unsigned long init_guess_time;  //Change later
int8_t guess;

//Can try to create a struct that has keypressed, colour and then frequency for sound

//Use arr[rand() % ARR_SIZE(arr)] to get random element from array where arr is name of array
char state = 0xFF;
char last_state = 0xFF;
int pos = 0;
int line = 0;
__attribute__((constructor))  static void init()
{
	serial_init1();
	uart1_init();
	SYSTICK_InternalInit(1);
	SYSTICK_IntCmd(ENABLE);
	SYSTICK_Cmd(ENABLE);
}
int main(void)
{
	setup();

}

void setup()
{
	/*  Basic setup of monitor, Keyboard, LCD Monitor, Lighting Module */
	// Will play sound  'hello' through speaker of some sort and cycle through  all 4 colours of the game
	lcd_init();
	lcd_write_str("Hello User", 0,0, sizeof("hello user")
	// May include voice over as oppose to displaying text, additonally may have lights synchronise with sound
	dmx_write(255,0, 0);
	wait(0.25);
	dmx_write(0,255,0);
	wait(0.25);
	dmx_write(0,0,255);
	wait(0.25);
	dmx_write(255,255,0);
	simon_game(); 
}

void simon_game()
{
	lcd_init();
	lcd_write_str("Please pick a difficulty from 1-4",0,0,sizeof("please pick a difficulty from 1-4");
	//Keyboard  detects key press
	state=read_buttons();
	while(keypad_uint8_t_decode(last_state)==keypad_uint8_t_decode(state) && keypad_uint8_t_decode(state)=='G')
	{
		last_state=state;
		state=read_buttons();
	} //Possible error here in while loop
	char c = keypad_uint8_t_decode(state);
	if(!isdigit(c))
	{
		lcd_init();
		lcd_write_str("Please enter a valid digit",0,0,sizeof("please enter a valid digit"));
		setup(); // Restarts Game, can say if more time saves state of game, etc.
	}
	difficulty = c-'0';
	switch(difficulty) 
	{
		case 1:
			seq_len =  8 ;
			break;
		case 2:
			seq_len = 14;
			break;
		case 3:
			seq_len = 20;
			break;
		case 4:
			seq_len = 31;
			break;

	}
	
	//Creates pattern based on difficulty chosen, higher difficulty --> goes up to a longer sequence
	for (int i=0; i<seq_len; i++) 
	{
		simon_seq[i] = colours[rand % ARR_SIZE(colours)];  
	}
	// simon_seq has array of colours(0to3)
	current_lvl=1;

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

void show_seq()
{
	// Adjusts time based on level player is on
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

	for(int i=0; i<current_lvl; i++)
	{
		current_col=simon_seq[i]; // saves colour to be shown in variable
		show_col(simon_seq[i], time);
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
void lost(uint8_t key)
{
	lcd_write_char(current_col,0,0);
	show_col(current_col, 5); // Need to adjust code to account for no 0 on keyboard
	//Play a losing game sound
	setup(); //Resets game
}

void won()
{
	while(1)
	{
		// Cycles through 4 colours , add victory sound
		for (int i=0; i<4; i++)
		{
			show_col(i,0.5);
		}
		state=read_buttons();
		while(keypad_uint8_t_decode(last_state)==keypad_uint8_t_decode(state) && keypad_uint8_t_decode(state)=='G')
		{
			last_state=state;
			state=read_buttons();
		}
		char w=keypad_uint8_t_decode(state);
		if(w == '#')
		{
			break;
		}
	}
	setup(); // Restarts game
}

void loop()
{
	// Displays seq that was configured i.e.  Computer sends pattern
	show_seq();
	
	// Player's input  read and processed
	for (int s=0; s<current_lvl; s++)
	{
		//	init_guess_time = ;
		guess = NO_INPUT;
		guess=key_pressed();
		/* Catches digits that are not valid */
		if((!(key <= 6)) || key == -1)
		{
			lcd_init();
			lcd_write_str("Please Enter a  Valid digit!",0,0,sizeof("please enter a  valid digita"));
			wait(2);
			lcd_init();
			lcd_write_str("Please Restart  to try again", 0,0, sizeof("please restart  to try again"));
		}
		if(guess != simon_seq[s])
		{
			lost(simon_seq[s]); // or use current_col
		}
	}
	current_lvl++;
	if(current_lvl > seq_len)
	{
		//wait(1);
		won();
	}
}







