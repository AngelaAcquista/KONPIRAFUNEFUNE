#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include "WelcomeWindow.hpp"

using std::map; using std::string; using std::vector; using std::cerr; using std::endl; using std::ifstream; using std::to_string;
using sf::Texture; using sf::RenderWindow; using sf::Event; using sf::Sprite; using sf::VideoMode;

map<string, Texture> TextureLoad(){
    map<string, Texture> TextureMap;
    vector<string> TextureNames = {"welcome_button.png","geisha.png", "open_hand_computer.png","background.png","hand_holding_the_thing.png","object_in_center.png","computer_fist_on_table.png"};

    for(const string& name : TextureNames){
        Texture texture;
        if(texture.loadFromFile("files/" + name)){
            TextureMap[name] = texture;
        }else cerr<<"Failed to open files/" + name<<endl;
    }
    return TextureMap;
}

int main(){
	WelcomeWindow welcome_window;
	welcome_window.Run();
	
	if (welcome_window.ShowVideo){
		// Play the demo video
	}

	if (welcome_window.GameStart){
		// Start the game
		RenderWindow window(VideoMode(1600, 1200), "Konpira");

		map<string, Texture> TextureMap = TextureLoad();
		Sprite Geisha(TextureMap.at("geisha.png")), Background(TextureMap.at("background.png")), OpenHandButton(TextureMap.at("welcome_button.png")), FistButton(TextureMap.at("welcome_button.png")), CaptureButton(TextureMap.at("welcome_button.png"));

		Background.setScale(1.6, 1.6);
		OpenHandButton.setScale(0.5, 0.5);
		FistButton.setScale(0.5, 0.5);
		CaptureButton.setScale(0.5, 0.5);

		OpenHandButton.setPosition(static_cast<float>(700), static_cast<float>(900));
		FistButton.setPosition(static_cast<float>(70), static_cast<float>(900));
		CaptureButton.setPosition(static_cast<float>(1300), static_cast<float>(900));
		Geisha.setPosition(static_cast<float>(600), static_cast<float>(250));
		Background.setOrigin(0, 220.f);
	
		while(window.isOpen()){
			Event event;
			while(window.pollEvent(event)){
				if(event.type == Event::Closed){
					window.close();
				}
	
			}
	
			window.clear();

			window.draw(Background);
			window.draw(Geisha);
			window.draw(OpenHandButton);
			window.draw(FistButton);
			window.draw(CaptureButton);
	
			window.display();
		}
	}

    return 0;
}