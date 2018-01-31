#include "helpers.h"
#define ARR_SIZE(arr) ( sizeof((arr)) / sizeof((arr[0])) )
#define MAX_SEQ 31
#define NO_INPUT -1
#define DEBOUNCE 250 //Or use wait()
#define THRESHOLD 10
/* See if possible to do this
#define RED dmx_write(255,0,0);
#define GREEN dmx_write(0,255,0);
#define BLUE dmx_write(0,0,255);
#define YELLOW dmx_write(255,255,0);*/
uint8_t simon_seq[MAX_SEQ];
uint8_t current_lvl;
unsigned long init_guess_time;  //Change later
int8_t guess;
//Can try to create a struct that has keypressed, colour and then frequency for sound

//Use arr[rand() % ARR_SIZE(arr)] to get random element from array where arr is name of array
int difficulty=1; // Set to 1 by default
char state = 0xFF;
char last_state = 0xFF;
int pos = 0;
int line = 0;
__attribut__((constructor))  static void init() 
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
	state = read_buttons(); // Keeps track of key pressed will use array to store and eventually will be used to decode pressed pattern
	dmx_write(255,0, 0);
	wait(0.25);
	dmx_write(0,255,0);
	wait(0.25);
	dmx_write(0,0,255);
	wait(0.25);
	dmx_write(255,255,0);
	simon_game();
	// configure random
	
	
 
}

void simon_game()
{
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
	
	//Creates pattern based on difficulty chosen
	for (int i=0; i<seq_len; i++) 
	{
		simon_seq[i] = random(4);
	}
	
	

}

void loop()
{
	// Displays seq that was configured i.e.  Computer sends pattern
	show_seq();
	
	// Player's input  read and processed
	for (int s=0; s<current

}







