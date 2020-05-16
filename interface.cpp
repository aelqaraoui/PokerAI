#include "interface.h"

LTexture::LTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect pos_dim){

	m_renderer = renderer;
	m_texture = texture;
	vect = pos_dim;

}

LTexture::LTexture(SDL_Renderer* renderer, std::string path, SDL_Rect pos_dim){

	m_renderer = renderer;
	m_texture = NULL;
	vect = pos_dim;

	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL)
		std::cout << "Unable to load image " << path.c_str() << "! SDL_Image ERROR : " << IMG_GetError() << std::endl;
	else{
	
		m_texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if(m_texture == NULL)
			std::cout << "Unable to create texture from " << path.c_str() << "! SDL ERROR : " << SDL_GetError() << std::endl;

		SDL_FreeSurface(loadedSurface);
	
	}


}

LTexture::LTexture(SDL_Renderer* renderer, std::string text, SDL_Rect pos_dim, TTF_Font* font, SDL_Color color){

	m_renderer = renderer;
	m_texture = NULL;
	vect = pos_dim;

	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	if(textSurface == NULL)
		std::cout << "Unable to render text surface! SDL_TTF ERROR : " << TTF_GetError() << std::endl;
	else{
	
		m_texture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if(m_texture == NULL)
			std::cout << "Unable to create texture from rendered text! SDL ERROR : " << SDL_GetError() << std::endl;

		SDL_FreeSurface(textSurface);

	}

}

LTexture::~LTexture(){

}

void LTexture::setBlend(SDL_BlendMode blending){

	SDL_SetTextureBlendMode(m_texture, blending);

}

void LTexture::setAlpha(Uint8 alpha){

	SDL_SetTextureAlphaMod(m_texture, alpha);

}

void LTexture::render(){

	SDL_RenderCopy(m_renderer, m_texture, NULL, &vect);

}

void LTexture::render(SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip){

	SDL_Rect renderQuad = vect;

	if(clip != NULL){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopyEx(m_renderer, m_texture, clip, &renderQuad, angle, center, flip);

}

LButton::LButton(SDL_Renderer* renderer, std::string text, SDL_Rect pos_dim, TTF_Font* font){

	SDL_Color color = {0, 0, 0};

	SDL_Rect quad = {
		pos_dim.x + pos_dim.w / 10,
        	pos_dim.y + pos_dim.h / 10,
		pos_dim.w - pos_dim.w * 2 / 10,
		pos_dim.h - pos_dim.h *2 /10	
	};

	m_renderer = renderer;

	m_text = new LTexture(renderer, text, quad, font, color);
	pos = pos_dim;
	current = NOT_CLICKED;
	

}

LButton::~LButton(){

	SDL_DestroyTexture(m_text->m_texture);
	delete m_text;

}

bool LButton::handleEvent(SDL_Event* event){

	bool last = false;
	if(event->type == SDL_MOUSEBUTTONDOWN || event->type == SDL_MOUSEBUTTONUP){
	
		int x, y;
		SDL_GetMouseState(&x, &y);

		bool inside = (x >= pos.x && x <= pos.x + pos.w && y >= pos.y && y <= pos.y + pos.h);

		if(inside){
		
			if(event->type == SDL_MOUSEBUTTONDOWN){

				if(current == NOT_CLICKED){
					current = CLICKED;
					last = true;
				}else	current = NOT_CLICKED;
			
			}
		
		} 

	}
	return last;

}

void LButton::render(){

	if(current == NOT_CLICKED)
		SDL_SetRenderDrawColor(m_renderer, 220, 220, 220, 255);
	else
		SDL_SetRenderDrawColor(m_renderer, 105, 105, 105, 255);
	for(int i = pos.x; i < pos.x + pos.w; i++)
		for(int j = pos.y; j < pos.y + pos.h; j++){

			double radius = pos.h * 5 / 10;
			if(i < pos.x + radius && j < pos.y + radius){
				double pf = (double)(pow(i - (pos.x + radius) , 2) + (double)pow(j - (pos.y + radius), 2))/(double)pow(radius, 2); 
				if(pf <= 1)	SDL_RenderDrawPoint(m_renderer, i, j);
			}else if(i > pos.x + pos.w - radius && j > pos.y + pos.h - radius){
				double pf = (double)(pow(i - (pos.x + pos.w - radius) , 2) + (double)pow(j - (pos.y + pos.h - radius), 2))/(double)pow(radius, 2); 
				if(pf <= 1)	SDL_RenderDrawPoint(m_renderer, i, j);
			}else if(i < pos.x + radius && j > pos.y + pos.h - radius){
				double pf = (double)(pow(i - (pos.x + radius) , 2) + (double)pow(j - (pos.y + pos.h - radius), 2))/(double)pow(radius, 2); 
				if(pf <= 1)	SDL_RenderDrawPoint(m_renderer, i, j);
			}else if(i > pos.x + pos.w - radius && j < pos.y + radius){
				double pf = (double)(pow(i - (pos.x + pos.w -radius) , 2) + (double)pow(j - (pos.y + radius), 2))/(double)pow(radius, 2); 
				if(pf <= 1)	SDL_RenderDrawPoint(m_renderer, i, j);
			}else 	SDL_RenderDrawPoint(m_renderer, i, j);

		}

	SDL_Rect quad = {
		pos.x + pos.w / 10,
        	pos.y + pos.h / 10,
		pos.w - pos.w * 2 / 10,
		pos.h - pos.h *2 /10	
	};
	SDL_RenderCopy(m_renderer, m_text->m_texture, NULL, &quad);

}

interface::interface(int width, int height) : action(-1), raise(-1), replay(-1), SCREEN_WIDTH(width), SCREEN_HEIGHT(height){

	window = NULL;
	renderer = NULL;
	
	for(int i = 0; i < 2; i++)	cardsPos[i] = {SCREEN_WIDTH*2/9 + i * SCREEN_WIDTH/9 , SCREEN_HEIGHT/3 + SCREEN_HEIGHT*3/12, 50, 80};
	for(int i = 0; i < 2; i++)	cardsPos[i+2] = {SCREEN_WIDTH*5/9 + i * SCREEN_WIDTH/9, SCREEN_HEIGHT/3 + SCREEN_HEIGHT*3/12, 50, 80};
	for(int i = 0; i < 5; i++)	cardsPos[i+4] = {SCREEN_WIDTH*2/9 + i * SCREEN_WIDTH/9 , SCREEN_HEIGHT/3 + SCREEN_HEIGHT*1/15, 50, 80};

	textColor = {0, 0, 0};

	if(SDL_Init( SDL_INIT_VIDEO) < 0)
		std::cout << "SDL couldn't initialize! SDL_ERROR : " << SDL_GetError() << std::endl;
	else{

		if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			std::cout << "Warning : Linear tecture filtering not enabled!" << std::endl;
	
		window = SDL_CreateWindow("Poker Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
		if(window == NULL)
			std::cout << "Window couldn't be created! SDL_ERROR : " << SDL_GetError() << std::endl;
		else{
		
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if(renderer == NULL)
				std::cout << "Renderer couldn't be created! SDL_ERROR : " << SDL_GetError() << std::endl;
			else{
			
				SDL_SetRenderDrawColor(renderer, 47, 79, 79, 255);
				int imgFlag = IMG_INIT_PNG;
				if(!(IMG_Init(imgFlag) & imgFlag))
					std::cout << "SDL_Image couldn't initialize! SDL_Image ERROR : " << IMG_GetError() << std::endl;
			
				if(TTF_Init() == -1)
					std::cout << "SDL_TTF couldn't initialize! SDL_TTF ERROR : " << TTF_GetError() << std::endl;
				 
				font = TTF_OpenFont("fonts/amatic_regular.ttf", 28);
				if(font == NULL)
					std::cout << "SDL_TTF couldn't open amatic_regular.ttf! SDL_TTF ERROR : " << TTF_GetError() << std::endl;	

				//Initialize Background
				SDL_Rect pos = {0, SCREEN_HEIGHT / 6, SCREEN_WIDTH, SCREEN_HEIGHT * 4 / 6};
				background = new LTexture(renderer, "background.png", pos);
				pos = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
				orchid = new LTexture(renderer, "black-orchid.png", pos);

				pos = {0, 0, 100, 50};
				replaybtn = new LButton(renderer, "Replay", pos, font);
				pos = {0, 150, 100, 50};
				quitbtn = new LButton(renderer, "Quit", pos, font);

				//Initialize Buttons
 				pos = {SCREEN_WIDTH / 4, SCREEN_HEIGHT * 8 / 10 + SCREEN_HEIGHT / 20, 100, 50};
				btns[0] = new LButton(renderer, "Fold", pos, font);
				pos = {SCREEN_WIDTH / 4 + SCREEN_WIDTH / 6, SCREEN_HEIGHT * 8 / 10 + SCREEN_HEIGHT / 20, 100, 50};
				btns[1] = new LButton(renderer, "Call", pos, font);
				pos = {SCREEN_WIDTH / 4 + SCREEN_WIDTH * 2 / 6, SCREEN_HEIGHT * 8 / 10 + SCREEN_HEIGHT / 20, 100, 50};
				btns[2] = new LButton(renderer, "Raise", pos, font);


				//Initialize textures for cards
				for(int i = 0; i < 52; i++){
				
					std::string pl =  "cards/" + readHand(i) + ".png";
					cards[i] = loadTexture(pl);
				
				}
				back = loadTexture("cards/back.png");
			}

		}
	
	}

}

interface::~interface(){

	SDL_DestroyTexture(background->m_texture);
	delete background;
	SDL_DestroyTexture(back);
	delete replaybtn;
	delete quitbtn;
	for(int i = 0; i < 3; i++)	delete btns[i];
	for(int i = 0; i < 52; i++)	SDL_DestroyTexture(cards[i]);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	window = NULL;
	renderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

}

void interface::resetGame(){
	
	for(int i = 0; i < 4; i++)	delete hands[i];
	for(int i = 0; i < indexCommunity; i++)	delete community[i];
	indexCommunity = 0;

}

SDL_Texture* interface::loadTexture(std::string path){

	SDL_Texture *loadedTexture = NULL;

	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if(loadedSurface == NULL)
		std::cout << "Unable to load image " << path.c_str() << "! SDL_Image ERROR : " << IMG_GetError() << std::endl;
	else{
	
		loadedTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if(loadedTexture == NULL)
			std::cout << "Unable to create texture from " << path.c_str() << "! SDL ERROR : " << SDL_GetError() << std::endl;

		SDL_FreeSurface(loadedSurface);
	
	}

	return loadedTexture;

}

void interface::renderBackground(){

	SDL_RenderClear(renderer);
		
	background->render();

	int last = -1;
	while(SDL_PollEvent(&event)){
	
		for(int i = 0; i < 3; i++)	
			if(btns[i]->handleEvent(&event) == true)	last = i;
	
	}

	if(last != -1){	
		for(int i = 0; i < 3; i++){	
			if(i != last){
				btns[i]->current = NOT_CLICKED;
			}else	action = i;
		}
	}

	for(int i = 0; i < 3; i++){

		btns[i]->render();
	
	}

}

void interface::show(){

	SDL_SetRenderDrawColor(renderer, 47, 79, 79, 255);
	SDL_RenderPresent(renderer);

}

void interface::setPlayerHand(std::pair<int, int> hand){

	hands[0] = new LTexture(renderer, cards[hand.first], cardsPos[0]);
	hands[1] = new LTexture(renderer, cards[hand.second], cardsPos[1]);
	hands[2] = new LTexture(renderer, back, cardsPos[2]);
	hands[3] = new LTexture(renderer, back, cardsPos[3]);

}

void interface::resetButtons(){

	for(int i = 0; i < 3; i++)	btns[i]->current = NOT_CLICKED;
	replaybtn->current = NOT_CLICKED;
	quitbtn->current = NOT_CLICKED;

}

void interface::setOpponentHand(std::pair<int, int> hand){

	hands[2]->m_texture = cards[hand.first];
	hands[3]->m_texture = cards[hand.second];

}

void interface::celebrateWinner(std::string name, int pot){

//	std::cout << name << " won " << pot << std::endl;

	SDL_Rect pos = {0, 0, SCREEN_WIDTH / 2, 100};
	winner = new LTexture(renderer, name + " won.", pos, font, textColor);
		
	while(replay == -1){

		SDL_RenderClear(renderer);
		
		background->render();

		while(SDL_PollEvent(&event)){
			if(replaybtn->handleEvent(&event) == true)	replay = 0;
			else if(quitbtn->handleEvent(&event) == true)	replay = 1;
		}

		if(replay == 0)	quitbtn->current = NOT_CLICKED;
		if(replay == 1)	replaybtn->current = NOT_CLICKED;

		for(int i = 0; i < 3; i++)	btns[i]->render();
	
		for(int i = 0; i < 4; i++)	hands[i]->render();

		showCommunityCards();

		orchid->render();
		winner->render();
		replaybtn->render();
		quitbtn->render();

		show();

	}

}

void interface::addCommunityCard(int card){

	community[indexCommunity] = new LTexture(renderer, cards[card], cardsPos[4+indexCommunity]);
	indexCommunity++;

}

void interface::showCommunityCards(){

	for(int i = 0; i < indexCommunity; i++)	community[i]->render();

}

void interface::setAction(){

	while(action == -1){
		
		renderBackground();
		
		for(int i = 0; i < 4; i++)	hands[i]->render();

		showCommunityCards();
		
		show();

	}

}

int interface::getAction(){

	int pl = action;
	action = -1;
	return pl;

}

void interface::setRaise(){

	std::cout << "Enter bet: ";
	cin >> raise;

}

int interface::getRaise(){

	int pl = raise;
	raise = -1;
	return pl;

}

int interface::getReplay(){

	int pl = replay;
	replay = -1;
	return pl;

}
