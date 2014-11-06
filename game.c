#include <stdlib.h>
#include "SDL.h"
#include "SDL_image.h"
#include "graphics.h"
#include "glib.h"
#include "vector2.h"
#include "entity.h"
#include "controller.h"
#include "font.h"
#include "audio.h"

extern SDL_Surface* screen;
extern SDL_Surface* buffer; //pointer to the draw buffer
extern SDL_Rect Camera;

void InitAll();
void InitFighters();
void InitPlayer(char* str, float x, float y);
void InitPlayer(char* str, float x, float y, int c);
void CleanupAll();
void ProcessInput();
void CountDown(char* string, Uint32 color);

int quit = 0;

/**
 * this program must be run from the directory directly below images and src, not from within src
 * notice the default arguments for main.  SDL expects main to look like that, so don't change it
 */
int main(int argc, char** argv)
{
	SDL_Surface* temp;
	SDL_Surface* bg;
	int frame = 0;
	int time = 9999;
	char timechar[2];

	InitAll();

	temp = IMG_Load("images/fire_and_ice.jpg");
	
	if(temp != NULL) bg = SDL_DisplayFormat(temp); // ALWAYS check your pointers before you use them

	SDL_FreeSurface(temp);

	if(bg != NULL) SDL_BlitSurface(bg, NULL, buffer, NULL);

	CountDown(" ", Red_);
	CountDown(" ", Red_);
	CountDown(" ", Red_);
	PlaySound(THREE_);
	FrameDelay(200);
	CountDown("3", Red_);
	PlaySound(TWO_);
	CountDown("2", Orange_);
	PlaySound(ONE_);
	CountDown("1", Yellow_);
	PlaySound(GO_);

	PlayMusic(BATTLEFIELD);
	//PlayMusic(FINALDESTINATION);
	while(!quit)
	{
		//BEGIN Draw Events
		ResetBuffer ();
		DrawEntityList();
 
		DrawTextCentered(itoa(time/100,timechar,10),screen, ResolutionX_/2 + 24, ResolutionY_/2 - 300, Green_, 0xdeadbeef, F_Large);
		if(frame++ < 500) DrawTextCentered("GO!",screen, ResolutionX_/2 + 60, ResolutionY_/2 - 200, Green_, 0xdeadbeef, F_Large);

		NextFrame();
		//END Draw Events


		//fprintf(stderr, "%x\n", green);
		//Input and Action Events
		ProcessInput();
		ThinkEntityList();
		PopulateQuadtrees();
		UpdateEntityList();
		//End Input and Action Events

		time--;
		if(time < 0) goto timeout;
	}

	exit(0);   //technically this will end the program, but the compiler likes all functions that can return a value TO return a value

	timeout:

	PlaySound(TIME_);
	CountDown("Time!", Blue_);
	CountDown("Time!", Blue_);
	CountDown("Time!", Blue_);
	CountDown("Time!", Blue_);
	CountDown("Time!", Blue_);

	wincond:

	PlaySound(GAME_);
	CountDown("Game!", Red_);
	CountDown("Game!", Red_);
	CountDown("Game!", Red_);
	CountDown("Game!", Red_);
	CountDown("Game!", Red_);

	exit(0);

	return 0;
}

/**
 * Conjecture: everything required for game clean-up, saving needs to go here
 */
void CleanUpAll()
{
	CloseSprites();
	CloseEntityList();
	//any other cleanup functions can be added here
}

/**
 * Conjecture: everything required for game set-up
 */
void InitAll()
{
	InitGraphics();
	LoadFonts();
	InitAudio();
	LoadSounds();
	InitControllers();
	InitEntityList();
	InitFighters();
	InitMouse();
	//InitController();
	atexit(CleanUpAll);
}

void InitFighters()
{
	InitPlayer("images/hypersphere.png", 200, 600);
	InitPlayer("images/hypersphere2.png", 800, 600);
}

void InitPlayer(char* str, float x, float y)
{
	InitPlayer(str, x, y, -1);
}

void InitPlayer(char* str, float x, float y, int c)
{
	Fighter* fighter = (Fighter*) GetEntity(FIGHTER);

	//fprintf(stderr,"Worked I guess");

	fighter->sprite = LoadSprite(str, 64, 64, 192, c, c, c);
	fighter->frame = 0;
	fighter->pos[0] = x;
	fighter->pos[1] = y;
}

void CountDown(char* string, Uint32 color)
{
	ResetBuffer ();
	DrawEntityList();
 
	DrawTextCentered(string,screen, ResolutionX_/2 + 7, ResolutionY_/2 - 200, color, 0xdeadbeef, F_Large);

	NextFrame();
	FrameDelay(1000);
}