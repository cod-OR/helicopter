// 6 lines

void loadloadingimage(){
	load = SDL_LoadBMP("Images/loading.bmp");
	load_tex = SDL_CreateTextureFromSurface(renderer,load);
	SDL_FreeSurface(load);
	SDL_RenderCopy(renderer,load_tex,NULL,NULL);
	SDL_RenderPresent(renderer);
	SDL_Delay(2000);
}
void loadgameoverimage(){
	gameover_img = SDL_LoadBMP("Images/gameover.bmp");
	gameover_tex = SDL_CreateTextureFromSurface(renderer,gameover_img);
	SDL_FreeSurface(gameover_img);
	SDL_RenderCopy(renderer,gameover_tex,NULL,NULL);
	SDL_RenderPresent(renderer);
	SDL_Delay(2000);
}

bool loadpremenu(){
	pre_menu = SDL_LoadBMP("Images/pre_menu.bmp");
	pre_menu_tex = SDL_CreateTextureFromSurface(renderer,pre_menu);
	SDL_FreeSurface(pre_menu);
	SDL_RenderCopy(renderer,pre_menu_tex,NULL,NULL);
	SDL_RenderPresent(renderer);
	SDL_Delay(1500);
	return 1;
}


// generic -
bool loadandrender(const char* name){
	SDL_Surface* x = SDL_LoadBMP(name);
	SDL_Texture* x_tex = SDL_CreateTextureFromSurface(renderer,x);
	SDL_FreeSurface(x);
	SDL_RenderCopy(renderer,x_tex,NULL,NULL);
	SDL_RenderPresent(renderer);
	SDL_Delay(1750);
	SDL_DestroyTexture(x_tex);
	return 1;	
}



// 3 lines

bool loadplayerimage(){
	player = SDL_LoadBMP("Images/player.bmp");
	player_tex = SDL_CreateTextureFromSurface(renderer,player);
	SDL_FreeSurface(player);
	return 1;
}

bool loadobstacleimage(){
	obstacle = SDL_LoadBMP("Images/obstacle.bmp");
	obstacle_tex = SDL_CreateTextureFromSurface(renderer,obstacle);
	SDL_FreeSurface(obstacle);
	return 1;
}

void loadhighscoreimage(){
	highscore_img = SDL_LoadBMP("Images/highscore.bmp");
	highscore_tex = SDL_CreateTextureFromSurface(renderer,highscore_img);
	SDL_FreeSurface(highscore_img);
}

void loadinstrimage(){
	instruction = SDL_LoadBMP("Images/instruction.bmp");
	instruction_tex = SDL_CreateTextureFromSurface(renderer,instruction);
	SDL_FreeSurface(instruction);
}

bool loadbackground(){
	background = SDL_LoadBMP("Images/background.bmp");
	background_tex = SDL_CreateTextureFromSurface(renderer,background);
	SDL_FreeSurface(background);
	return 1;
}

// generic -
SDL_Texture* loadimage(const char* name){
	SDL_Surface* x= SDL_LoadBMP(name);
	SDL_Texture* x_tex = SDL_CreateTextureFromSurface(renderer,x);
	SDL_FreeSurface(x);
	return x_tex;
}



// 4 Lines -

bool loadmenuimage(){
	menu = SDL_LoadBMP("Images/menu.bmp");
	menu_tex = SDL_CreateTextureFromSurface(renderer,menu);
	SDL_FreeSurface(menu);
	SDL_RenderCopy(renderer,menu_tex,NULL,NULL);
	return 1;
}




