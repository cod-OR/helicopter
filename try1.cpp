#include<bits/stdc++.h>
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include "SDL2/SDL_timer.h"

#define SCREEN_WIDTH   1500 
#define SCREEN_HEIGHT  1000
#define FPS              60
#define numberofbullets  50
#define bulletdelay     500

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

void createwindow(){
	window = SDL_CreateWindow("Attack on Sith",
							   SDL_WINDOWPOS_CENTERED,
							   SDL_WINDOWPOS_CENTERED,
							   SCREEN_WIDTH,SCREEN_HEIGHT,
							   SDL_WINDOW_RESIZABLE);	}
void createrenderer(){renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);}

bool loadandrender(const char* name){
	SDL_Surface* x= SDL_LoadBMP(name);
	if(x == NULL){
		printf("Error Loading image from %s\n %s", name, SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
	}
	SDL_Texture* x_tx = SDL_CreateTextureFromSurface(renderer, x);
	SDL_FreeSurface(x);
	SDL_RenderCopy(renderer, x_tx, NULL, NULL);
	SDL_RenderPresent(renderer);
	SDL_Delay(1750);
	SDL_DestroyTexture(x_tx);
	return 1;
}
bool init(){
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER)!=0){
		printf("Error Initializing SDL: %s\n",SDL_GetError());
		return 1;
	}
	if(TTF_Init()==-1){
    	printf("Error Initializing TTF: %s\n", TTF_GetError());
    	return 1;
	}
	createwindow();
	if(window == NULL){
		printf("Error Creating Window: %s\n",SDL_GetError());
		SDL_Quit();
		return 1;
	}
	createrenderer();
	if(renderer == NULL){
		printf("Error Creating Renderer: %s\n",SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	if(!loadandrender("Images/pre_menu.bmp"))return 0;
	return 1;
}
void close(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
bool loadimage(const char* name){
	SDL_Surface* x= SDL_LoadBMP(name);
	if(x == NULL){
		printf("Error Loading image from %s\n %s", name, SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
	}
	SDL_Texture* x_tx = SDL_CreateTextureFromSurface(renderer,x);
	SDL_FreeSurface(x);
	SDL_RenderCopy(renderer, x_tx, NULL, NULL);
	SDL_DestroyTexture(x_tx);
	return 1;
}
int startmenu(){
	if(!loadimage("Images/menu.bmp"))return -1;

	printf("Menu image load complete\n");

	TTF_Font *font = NULL;
	font= TTF_OpenFont("Images/con.ttf",1000);
	const char* options[4]={"NEW GAME", "HIGHSCORE", "INSTRUCTIONS","QUIT"};
	SDL_Color text_color = {255,255,255,255};

	SDL_Texture *text[4];
	for(int i=0;i<4;i++){
		SDL_Surface* menu_surface = TTF_RenderText_Solid(font,options[i],text_color);
		text[i] = SDL_CreateTextureFromSurface(renderer,menu_surface);
		SDL_FreeSurface(menu_surface);
		
	}
	SDL_Rect text_position;
	text_position.x=1000;
	text_position.w=200;
	text_position.h=150;
	printf("Text load complete\n");
	for(int i=0, j=10;i<5;i++, j+=250){
		text_position.y=j;
		SDL_RenderCopy(renderer, text[i], NULL, &text_position);
	}
	SDL_RenderPresent(renderer);
	printf("Text render complete\n");
	// SDL_DestroyTexture(text[i]);
	SDL_Event e;
	bool running = true;
	while(running){
		if(SDL_PollEvent(&e)){
			switch(e.type){
				case SDL_QUIT :
					running=false;
					break;
				case SDL_MOUSEBUTTONDOWN:
				{	for(int i=0;i<4;i++){
						if(e.motion.x>=1000 && e.motion.x<=1200 && e.motion.y>=i*250 && e.motion.y<=(i+1)*250){
							return i;
							break;
						}
					}
					break;
				}
				default:
				break;
			}
		}
	}
}
bool showtillesc(const char* name){
	SDL_Surface* x= SDL_LoadBMP(name);
	if(x == NULL){
		printf("Error Loading image from %s\n %s", name, SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
	}
	SDL_Texture* x_tx = SDL_CreateTextureFromSurface(renderer,x);
	SDL_FreeSurface(x);
	SDL_RenderCopy(renderer, x_tx, NULL, NULL);
	SDL_RenderPresent(renderer);
	bool showing =1;
	SDL_Event e;
	while(showing){
		while(SDL_PollEvent(&e)){
			if(e.key.keysym.scancode==SDL_SCANCODE_ESCAPE){	
				showing=0;
			}	
			if(e.type==SDL_QUIT){
				showing=0;
				return 0;
			}
		}		
	}
	return 1;
}
void playaudio(){
	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1048);
	Mix_Music* backgroundSound=Mix_LoadMUS("Audio/gamesong.mp3");
	Mix_PlayMusic(backgroundSound, -1);
}

class bullet{
private:	
	SDL_Rect b, time;
public:
	bullet(int pos){
		b.h=15;
		b.w=8;
		b.y=SCREEN_HEIGHT-125;
		b.x=pos;
	}
	void render(){
		
		// if(abs(b.y) > SCREEN_HEIGHT )return;
		SDL_Surface* x = SDL_LoadBMP("Images/bullet.bmp");
		SDL_Texture* x_tex = SDL_CreateTextureFromSurface(renderer,x);
		SDL_FreeSurface(x);
		SDL_RenderCopy(renderer,x_tex,NULL,&b);
		SDL_DestroyTexture(x_tex);
		b.y-=120;
	}
};
class obstacle{
	SDL_Rect ob;
	int curr_y;
public:	
	obstacle(){
		ob.y=0;
		ob.x= rand()% SCREEN_WIDTH;
		ob.h=50;
		ob.w=50;
		curr_y=0;
	}
	void render(){
		
		// if(abs(ob.y) >= SCREEN_HEIGHT )return;
		SDL_Surface* x = SDL_LoadBMP("Images/obstacle.bmp");
		SDL_Texture* x_tex = SDL_CreateTextureFromSurface(renderer,x);
		SDL_FreeSurface(x);
		SDL_RenderCopy(renderer,x_tex,NULL,&ob);
		SDL_DestroyTexture(x_tex);
		ob.y+=100;

	}
};


SDL_Texture* loadimagetexture(const char* name){
	SDL_Surface* x= SDL_LoadBMP(name);
	SDL_Texture* x_tex = SDL_CreateTextureFromSurface(renderer,x);
	SDL_FreeSurface(x);
	return x_tex;
}

bool play(){
	// playaudio();
	loadandrender("Images/loading.bmp");
	loadimage("Images/background.bmp");
	SDL_Texture* player = loadimagetexture("Images/player.bmp");
	SDL_Rect player_pos;
	player_pos.x= rand()% SCREEN_WIDTH;
	player_pos.y=SCREEN_HEIGHT -100;
	player_pos.h=100;
	player_pos.w=80;
	SDL_RenderCopy(renderer,player,NULL,&player_pos);
	bullet bu(player_pos.x + 38);
	obstacle one;
	bu.render();
	one.render();
	SDL_RenderPresent(renderer);
	
	// bool showing =1;
	// SDL_Event e;
	// while(showing){
	// 	while(SDL_PollEvent(&e)){
	// 		if(e.key.keysym.scancode==SDL_SCANCODE_ESCAPE){	
	// 			showing=0;
	// 		}	
	// 		if(e.type==SDL_QUIT){
	// 			showing=0;
	// 			return 0;
	// 		}
	// 	}		
	// }
	// return 1;

	return 0;
	
	/*

	1. show the remaining number of lives
	2. Load the player
	3. Load the background image
	4. Load bullets
	5. Load obstacles


	6. Give mmotion to obstacles
	7. Give motion to player

	8. Define collision conditions

	9. Write scoring system

	*/
}


void game(){
	int option = startmenu();
	printf("%d \n",option );;
	// switch(option){
		
	// 	case -1:
	// 	return;
	// 	case 0:
	// 	if(!play())return;
	// 	case 1:
	// 	if(!showtillesc("Images/highscore.bmp"))return;
	// 	break;
	// 	case 2:
	// 	if(!showtillesc("Images/instruction.bmp"))return;
	// 	break;
	// 	case 3:
	// 	return;
	// }
	// game();
	return;
}

int main(int argc,char* args[]){
	srand(time(0));
	if(init())
		game();
		// play();
	close();
	return 1;
}

