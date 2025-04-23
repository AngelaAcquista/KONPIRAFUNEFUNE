#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

using std::istreambuf_iterator; using std::string; using std::vector; using std::ifstream; using std::cerr; using std::endl;
using sf::FloatRect; using sf::Text; using sf::Vector2f; using sf::VideoMode; using sf::Color; using sf::Event; using sf::Vector2i;
using sf::Mouse; using sf::Font; using sf::Texture; using sf::Sprite; using sf::RenderWindow;


void ReaderFile(const string& filename, vector<unsigned char>& FontData){
	ifstream file;

	file.open("files/" + filename, std::ios::binary);

	if(!file.is_open()){
		cerr<<"Failed to load: files/"<<filename<<endl;
		return;
	}
	FontData.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	file.close();
}

void setText(Text &text, float x, float y){
	FloatRect textRect = text.getLocalBounds();
	text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
	text.setPosition(Vector2f(x, y));
}

class WelcomeWindow{
public:
	bool ShowVideo;
	bool GameStart;

	WelcomeWindow(){
		window_.create(VideoMode(1600, 1200), "Konpira");
		// Set up the background
		background_texture.loadFromFile("files/background.png");
		background_sprite.setTexture(background_texture);
		background_sprite.setScale(1.6, 1.6);
		background_sprite.setOrigin(0, 220.f);

		// Set up the title
		title_texture.loadFromFile("files/welcome_title.png");
		title_sprite.setTexture(title_texture);
		title_sprite.setScale(0.6, 0.6); //1200*1200
		title_sprite.setPosition(200.f, -250.f);

		// Set up the two buttons
		buttons_texture.loadFromFile("files/welcome_button.png");
		button1_sprite.setTexture(buttons_texture);
		button2_sprite.setTexture(buttons_texture);
		button1_sprite.setScale(0.6, 0.6); //300*300
		button2_sprite.setScale(0.6, 0.6); //300*300
		button1_sprite.setPosition(450.f, 650.f);
		button2_sprite.setPosition(850.f, 650.f);

		// Set up the texts
		vector<unsigned char> FontData;

		ReaderFile("MorganChalk-L3aJy.ttf", FontData);

		if(!font.loadFromMemory(FontData.data(), FontData.size())){
			cerr<<"Failed to open font."<<endl;
		}
		title_text.setFont(font);
		title_text.setString("Konpira Fune Fune!");
		title_text.setCharacterSize(56);
		title_text.setStyle(Text::Bold);
		title_text.setFillColor(Color::Black);
		setText(title_text, 800.0f, 320.0f);
		title_text.setScale(2.4, 1.8);
		ShowVideo = false;
		GameStart = false;
	}

	void Run(){
		// Window loop
		while (window_.isOpen()){
			// Handle event
			Event event;
			while (window_.pollEvent(event)){
				if (event.type == Event::Closed){
					window_.close();
				}
				
				if (event.type == Event::MouseButtonPressed){
					if (Mouse::isButtonPressed(Mouse::Left)){
						Vector2i mouse_position = Mouse::getPosition(window_);
						Vector2f mouse_position_f(static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y));
						// Show video button is clicked
						if (button1_sprite.getGlobalBounds().contains(mouse_position_f)){
							window_.close();
							ShowVideo = true;
						} 
						// Game start button is clicked
						else if (button2_sprite.getGlobalBounds().contains(mouse_position_f)){
							window_.close();
							GameStart = true;
						}
					}
				}
			}
			
			// Draw the sprites
			window_.draw(background_sprite);
			window_.draw(title_sprite);
			window_.draw(button1_sprite);
			window_.draw(button2_sprite);

			//Draw text
			window_.draw(title_text);

			window_.display();
		}
	}

private:
	RenderWindow window_;
	Font font;

	Texture background_texture;
	Sprite background_sprite;

	Texture title_texture;
	Sprite title_sprite;
	Text title_text;

	Texture buttons_texture;
	Sprite button1_sprite;
	Sprite button2_sprite;
};