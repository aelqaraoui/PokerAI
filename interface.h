#ifndef POKERAI_INTERFACE_H
#define POKERAI_INTERFACE_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "cards.h"

class LTexture{
	public:
		SDL_Renderer* m_renderer;
		SDL_Texture* m_texture;
		SDL_Rect vect;

	public:
		LTexture(SDL_Renderer* renderer, SDL_Texture* texture, SDL_Rect pos_dim);
		LTexture(SDL_Renderer* renderer, std::string path, SDL_Rect pos_dim);
		LTexture(SDL_Renderer* renderer, std::string text, SDL_Rect pos_dim, TTF_Font* font, SDL_Color color);
		~LTexture();
		void setBlend(SDL_BlendMode blending);
		void setAlpha(Uint8 alpha);
		void render();
		void render(SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip);

};

#define CLICKED 0
#define NOT_CLICKED 1

class LButton {
	public:
		SDL_Renderer* m_renderer;
		LTexture* m_text;
		SDL_Rect pos;
		int current;

	public:
		LButton(SDL_Renderer* renderer, std::string text, SDL_Rect pos_dim, TTF_Font* font);
		~LButton();
		bool handleEvent(SDL_Event* e);
		void render();

};

class interface {
public:
	int action;
	int raise;
	const int SCREEN_WIDTH;
	const int SCREEN_HEIGHT;
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Event event;

	LTexture* background;
	LButton* btns[3];
	SDL_Texture* cards[52];
	SDL_Texture* back;
	LTexture* hands[4];
	LTexture* community[5];
	int indexCommunity = 0;
	SDL_Rect cardsPos[9];
	SDL_Color textColor;
	TTF_Font* font;

public:
	interface(int width, int height);
	~interface();

	void resetGame();
	SDL_Texture* loadTexture(std::string path);

//	SDL_Texture* loadText(std::string text);
	void renderBackground();
	void show();
	void setPlayerHand(std::pair<int, int> hand);
	void showPlayerHand(std::pair<int, int> hand);
//	void renderButton(std::string text, SDL_Rect pos);
	void resetButtons();
	void setOpponentHand(std::pair<int, int> hand);
	void showOpponentHand();
	void celebrateWinner(std::string name, int pot);
	void addCommunityCard(int card);
	void showCommunityCards();

	void setAction();
	int getAction();
	void setRaise();
	int getRaise();

};

#endif //POKERAI_INTERFACE_H
