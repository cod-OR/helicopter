#include<bits/stdc++.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_timer.h>
#include<SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

#define SCREEN_WIDTH 1500 
#define SCREEN_HEIGHT 1000
#define SPEED 400 
#define FPS 60 
#define numberofbullets 20
#define bulletdelay 1000

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font *font = NULL;

SDL_Texture*  bullet_tex = NULL;
SDL_Texture* life_tex = NULL;
SDL_Texture*  player_tex = NULL;
SDL_Surface* obstacle = NULL;
SDL_Texture*  obstacle_tex = NULL;
SDL_Texture*  background_tex = NULL;
SDL_Texture*  load_tex = NULL;
SDL_Surface* menu = NULL;
SDL_Texture*  menu_tex = NULL;
SDL_Texture* highscore_tex = NULL;
SDL_Texture* instruction_tex = NULL;
SDL_Texture*  pre_menu_tex = NULL;
SDL_Texture* gameover_tex = NULL;

int spawn[SCREEN_WIDTH];
SDL_Rect destO[SCREEN_WIDTH];
int lives = 3;
int up = 0;
int down = 0;
int left = 0;
int right = 0;
SDL_Rect dest;
float x_pos = (SCREEN_WIDTH - dest.w)/2.0;
float y_pos = (SCREEN_HEIGHT - dest.h);
float x_vel = 0;
float y_vel = 0;
int currentscore=0;

class bullet{
	public:
        void init();        
        void render();
        void destroy();
        SDL_Rect bullet;

    private:
        int xpos , ypos;       
};
void bullet::render(){
	bullet.h= 10;
	bullet.w= 5;
	bullet.y -= 6;
	
	SDL_Surface* bullet_img = SDL_LoadBMP("Images/bullet.bmp");
	bullet_tex = SDL_CreateTextureFromSurface(renderer,bullet_img);
	SDL_FreeSurface(bullet_img);
	SDL_RenderCopy(renderer,bullet_tex,NULL,&bullet);
}
int random_obstacle(int x, int rand ,SDL_Rect p ,bullet B[]){
	if(rand==2){
		spawn[x]=1;
	    destO[x].x = x;
	}
	for(int i=0;i<(SCREEN_WIDTH-50);i++){
		if(spawn[i]==1){
			SDL_RenderCopy(renderer,obstacle_tex,NULL,&destO[i]);
	    	destO[i].y+=2.5;
	    //obstacle gone
	    if(destO[i].y>=SCREEN_HEIGHT){
	       spawn[i]=0;
	       destO[i].y=0;
	       lives--;
	       printf("Life: %d\n" ,lives);
	    }

	    if(p.y>=(destO[i].y-p.h) && p.y<=(destO[i].y+destO[i].h)){

		    if(p.x<=(destO[i].x+destO[i].w) && p.x>=destO[i].x){
		    	spawn[i]=0;
		    	destO[i].y=0;
		    	lives--;
		    	printf("Life: %d\n" ,lives);
		    }
		    
		    if((p.x+p.w)<=(destO[i].x+destO[i].w) && (p.x+p.w)>=destO[i].x){
		    	spawn[i]=0;
		    	destO[i].y=0;
		    	lives--;
		    	printf("Life: %d\n" ,lives);
		    }

	    }

        if(p.x>=(destO[i].x+p.x) && p.x<=(destO[i].x+destO[i].w)){
		    if(p.y<=(destO[i].y+destO[i].h) && p.y>=destO[i].y){
		    	spawn[i]=0;
		    	destO[i].y=0;
		    	lives--;
		    	printf("Life: %d" ,lives);
		    }
		    if((p.y+p.h)<=(destO[i].y+destO[i].h) && (p.y+p.h)>=destO[i].y){
		    	spawn[i]=0;
		    	destO[i].y=0;
		    	lives--;
		    	printf("Life: %d\n" ,lives);
		    }
	    }
	    for(int j=0;j<numberofbullets;j++){
            if(B[j].bullet.x>=destO[i].x && (B[j].bullet.x+B[j].bullet.w)<=(destO[i].x+destO[i].w)){
                if(B[j].bullet.y<=(destO[i].y+destO[i].h)){
                    spawn[i] = 0;
                    B[j].bullet.x = SCREEN_WIDTH+100;
                    B[j].bullet.y = SCREEN_HEIGHT+1000000;
                }
            }
	    }
      }
	}
}
void highscore(int t){
	FILE *fp;
	fp = fopen("highscore.txt","r");
	std::vector<int> a(5);
	int i=0;
	while(fscanf(fp,"%d",&a[i])==1){
		i++;
		if(i==5)  break;
	}
	if(t>a[4])  a[4]=t;
	std::sort(a.begin(), a.end());
	FILE *fw=fopen("highscore.txt","w");
	fprintf(fw,"%d\n%d\n%d\n%d\n%d\n",a[0],a[1],a[2],a[3],a[4]);
	fclose(fp);
	fclose(fw);
}

void fps_optimize(Uint32 starting_tick){
	if((1000/FPS)>SDL_GetTicks()-starting_tick){
		SDL_Delay(1000/FPS-(SDL_GetTicks()-starting_tick));
	}
}

bool createWindow(){
	window = SDL_CreateWindow("HELI",
							   SDL_WINDOWPOS_CENTERED,
							   SDL_WINDOWPOS_CENTERED,
							   SCREEN_WIDTH,SCREEN_HEIGHT,
							   SDL_WINDOW_RESIZABLE);
	if(window == NULL){
		printf("Error Creating Window: %s\n",SDL_GetError());
		SDL_Quit();
		return 0;
	}
	return 1;
}
bool createrenderer(){
	Uint32 render_flags = SDL_RENDERER_ACCELERATED	;
	renderer = SDL_CreateRenderer(window,-1,render_flags);
	if(renderer == NULL){
		printf("Error Creating Renderer: %s\n",SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 0;
	}
	return 1;
}
bool loadandrender(const char* name){
	SDL_Surface* x = SDL_LoadBMP(name);
	SDL_Texture* x_tex = SDL_CreateTextureFromSurface(renderer,x);
	SDL2L_FreeSurface(x);
	SDL_RenderCopy(renderer,x_tex,NULL,NULL);
	SDL_RenderPresent(renderer);
	SDL_Delay(1750);
	SDL_DestroyTexture(x_tex);
	return 1;	
}
SDL_Texture* loadimage(const char* name){
	SDL_Surface* x= SDL_LoadBMP(name);
	SDL_Texture* x_tex = SDL_CreateTextureFromSurface(renderer,x);
	SDL_FreeSurface(x);
	return x_tex;
}
bool loadmenuimage(){
	menu = SDL_LoadBMP("Images/menu.bmp");
	menu_tex = SDL_CreateTextureFromSurface(renderer,menu);
	SDL_FreeSurface(menu);
	SDL_RenderCopy(renderer,menu_tex,NULL,NULL);
	return 1;
}
void loadlifeimage(SDL_Rect &life){
	SDL_Surface* life_img = SDL_LoadBMP("Images/life.bmp");
	life_tex = SDL_CreateTextureFromSurface(renderer,life_img);
	SDL_FreeSurface(life_img);
	SDL_RenderCopy(renderer,life_tex,NULL,&life);
}
void playaudio(){
	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1048);
	Mix_Music* backgroundSound=Mix_LoadMUS("Audio/gamesong.mp3");
	Mix_PlayMusic(backgroundSound, -1);
}
int showtillclose(SDL_Texture* m){
	SDL_Event event;
	bool running = true;
	while(running){
		while(SDL_PollEvent(&event)){
			if(event.key.keysym.scancode==SDL_SCANCODE_ESCAPE){	
				return 1;
			}
			switch(event.type){
				case SDL_QUIT:
					running = false;
					break;
			}
		}
		SDL_RenderCopy(renderer,m,NULL,NULL);
		SDL_RenderPresent(renderer);

	}
	return 0;
}

int mainmenu(){
	std::cout<<"Loading menu\n";
	if(!loadmenuimage()){
		std::cout<<"Failed \n";
		return 0;
	}
	std::cout<<"Menu loaded\n";

	/*Menu Text Type*/
	{
		font  = TTF_OpenFont("Images/con.ttf",1000);	
		SDL_Texture *text[4];
		const char* options[4]={"NEW GAME", "HIGHSCORE", "INSTRUCTIONS","QUIT"};
		SDL_Color text_color = {255,255,255,255};

		for(int i=0;i<4;i++){
			SDL_Surface* menu_surface = TTF_RenderText_Solid(font,options[i],text_color);
			text[i] = SDL_CreateTextureFromSurface(renderer,menu_surface);
			SDL_FreeSurface(menu_surface);
		}

		SDL_Rect text_position;
		int i,j;

		for(i=0,j=0;i<5;i++,j+=250){
			text_position.x = 1000;
			text_position.y = j;
			text_position.w = 200;
			text_position.h = 150;
			SDL_RenderCopy(renderer,text[i],NULL,&text_position);
		}
		SDL_RenderPresent(renderer);

		SDL_Event e;
		bool menu_run = true;
		while(menu_run){
			if(SDL_PollEvent(&e)){
				switch(e.type){
					case SDL_QUIT :
						menu_run =false;
						break;
					case SDL_MOUSEBUTTONDOWN:
					{
						for(int i=0;i<5;i++){
							if(e.motion.x>=1000&&e.motion.x<=1200 && e.motion.y>=(i*150) && e.motion.y<(i+1)*150){
								return i;
								break;
							}
						}
					}
					default:
					break;
				}
			}
		} 
	}
}

bool startplay(){
	Uint32 starting_tick;
	printf("Life: %d\n" ,lives);
	
	SDL_Rect life;
	loadandrender("Images/loading.bmp");
	playaudio();
		
	bullet B[numberofbullets];
	int initial = 0;
	int time=0;
		
	SDL_Event event;
	bool running = true;
	while(running){
		if(lives==0){
			loadandrender("Images/gameover.bmp");
			Mix_HaltMusic();
			return 1;
		}
		currentscore = SDL_GetTicks()/1000;
		while(SDL_PollEvent(&event)){
			/*Initializing Bullet*/
			if(event.key.keysym.scancode==SDL_SCANCODE_SPACE && event.key.repeat == 0){
				time++;
				if(time>1){
				initial++;
				if(initial>=numberofbullets){ 
					initial=0;
				}
				printf("Bullets till reset = %d\n", numberofbullets-initial);
				B[initial].bullet.x = x_pos + dest.w/2 -2;
				B[initial].bullet.y = y_pos;
				time =0;
				}
			}
			if(event.key.keysym.scancode==SDL_SCANCODE_ESCAPE){	
				Mix_HaltMusic();
				return 1;
			}
			switch(event.type){
				case SDL_QUIT:
					running = false;
					return 0;
				case SDL_KEYDOWN:
					switch(event.key.keysym.scancode){
						case SDL_SCANCODE_W:
						case SDL_SCANCODE_UP:
							up = 1;
							break;
						case SDL_SCANCODE_A:
						case SDL_SCANCODE_LEFT:
							left = 1;
							break;
						case SDL_SCANCODE_S:
						case SDL_SCANCODE_DOWN:
							down = 1;
							break;
						case SDL_SCANCODE_D:
						case SDL_SCANCODE_RIGHT:
					 		right = 1;
					 		break;
					}
					break; 
				case SDL_KEYUP:
					switch(event.key.keysym.scancode){
						case SDL_SCANCODE_W:
						case SDL_SCANCODE_UP:
							up = 0;
							break;
						case SDL_SCANCODE_A:
						case SDL_SCANCODE_LEFT:
							left = 0;
							break;
						case SDL_SCANCODE_S:
						case SDL_SCANCODE_DOWN:
							down = 0;
							break;
						case SDL_SCANCODE_D:
						case SDL_SCANCODE_RIGHT:
					 		right = 0;
					 		break;
				}
				break;  // decide up or down or left or right
			}
		}
		/*Determine the Velocity*/
		x_vel = 0;
		y_vel = 0;
		if(up && !down) y_vel = -SPEED;
		if(down && !up) y_vel = SPEED;
		if(left && !right) x_vel = -SPEED;
		if(right && !left) x_vel = SPEED;

		/*Update Positions*/
		x_pos += x_vel/60;
		y_pos +=y_vel/60;

		/*Collision Detection with Borders*/
		if(x_pos<=0) x_pos = 0;
		if(y_pos<=0) y_pos = 0;
		if(x_pos>=SCREEN_WIDTH - dest.w) x_pos = SCREEN_WIDTH - dest.w;
		if(y_pos>=SCREEN_HEIGHT - dest.h) y_pos = SCREEN_HEIGHT - dest.h;

		/*Set the Positions in the Structure*/
		dest.y = (int) y_pos;
		dest.x = (int) x_pos;

		/*Clear the Window*/
		SDL_RenderClear(renderer);

		/*Draw the Background to the Window*/
		SDL_RenderCopy(renderer,background_tex,NULL,NULL);


		/*Draw the Bullet*/
		for(int start=0;start<=initial;start++){
			if(B[start].bullet.y>0)
			B[start].render();
		}

		/*Draw the Player to the Window*/
		SDL_RenderCopy(renderer,player_tex,NULL,&dest);

		/*Draw the Obstacle*/
		int x=rand()%(SCREEN_WIDTH-50);
		int p=rand()%60;
    	random_obstacle(x ,p ,dest,B);


		SDL_Delay(1000/FPS);
		{
				if(lives>0)	{
					life.x = 100;
					life.y = 50;
					life.w = 50;
					life.h = 50;
				loadlifeimage(life);      
				}
				if(lives>1){
					life.x = 160;
		     	loadlifeimage(life); 
				}
				if(lives>2){
					life.x = 220;
		    	loadlifeimage(life); 
				}}		// Show lives remaining	

		SDL_RenderPresent(renderer);

		/*Using the Optimizing Function*/
		fps_optimize(starting_tick);
	}
 	
 	printf("Score : %d\n", currentscore);
 	Mix_HaltMusic();
 	return 0;
}

void game(){
	std::cout<<"Starting the game...\n";
	currentscore=0;
 	int option = mainmenu();
 	
 	if(option==0){
		int next=startplay();
		if(next==1)game();
 	}
 	if(option==1){
  		highscore_tex = loadimage("Images/highscore.bmp");
		int next= showtillclose(highscore_tex);
		if(next==1)game();	
 	}
 	else if(option==2){
  		instruction_tex = loadimage("Images/instruction.bmp");
 		int next = showtillclose(instruction_tex);	
 		if(next==1)game();
 	}
 	else return;
}

void close(){
	SDL_DestroyTexture(life_tex);
	SDL_DestroyTexture(bullet_tex);
	SDL_DestroyTexture(obstacle_tex);
	SDL_DestroyTexture(player_tex);
	SDL_DestroyTexture(background_tex);
	SDL_DestroyTexture(load_tex);
	SDL_DestroyTexture(instruction_tex);
	SDL_DestroyTexture(menu_tex);
	SDL_DestroyTexture(pre_menu_tex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	highscore(currentscore);
}

bool init(){
	for(int i=0;i<(SCREEN_WIDTH-50);i++){
		destO[i].w=50;
		destO[i].h=60;
	}
	memset(spawn,0,sizeof(spawn[0])*1000);
	if(SDL_Init(	SDL_INIT_VIDEO|SDL_INIT_TIMER)!=0){
		printf("Error Initializing SDL: %s\n",SDL_GetError());
		return 0;
	}
	if(TTF_Init()==-1){
    	printf("Error Initializing TTF: %s\n", TTF_GetError());
    	return 0;
	}
	if(!createWindow())return 0;
	if(!createrenderer())return 0;;
	if(!loadandrender("Images/pre_menu.bmp"))return 0;
	background_tex = loadimage("Images/background.bmp");
	player_tex = loadimage("Images/player.bmp");
	obstacle_tex = loadimage("Images/obstacle.bmp");
	SDL_QueryTexture(player_tex,NULL,NULL,&dest.w,&dest.h);
	return 1;
}

int main(int argc,char* args[]){
	if(!init())return 0;
 	game();
 	close();
	return 0;
}
