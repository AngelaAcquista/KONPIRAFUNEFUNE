#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include "WelcomeWindow.hpp"

using std::map;
using std::string;
using std::vector;
using std::cerr;
using std::endl;
using sf::Texture;
using std::ifstream;
using sf::RenderWindow;
using std::to_string;
using sf::Event;
using sf::Sprite;

map<string, Texture> TextureLoad(){
    map<string, Texture> TextureMap;
    vector<string> TextureNames = {"geisha.jpg", "open_hand_button.png", "open_hand_computer.png", "fist_button.png","background.png","capture_object_button.png","hand_holding_the_thing.png","object_in_center.png","computer_fist_on_table.png"};

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
		RenderWindow window(sf::VideoMode(800, 600), "Konpira");

		map<string, Texture> TextureMap = TextureLoad();
		Sprite OpenHandButton(TextureMap.at("open_hand_button.png")), FistButton(TextureMap.at("fist_button.png")), CaptureButton(TextureMap.at("capture_object_button.png"));
	
		OpenHandButton.setPosition(static_cast<float>(32), static_cast<float>(5));
		FistButton.setPosition(static_cast<float>(32), static_cast<float>(20));
		CaptureButton.setPosition(static_cast<float>(32), static_cast<float>(40));
	
		while(window.isOpen()){
			Event event;
			while(window.pollEvent(event)){
				if(event.type == Event::Closed){
					window.close();
				}
	
			}
	
			window.clear();
	
			window.draw(OpenHandButton);
			window.draw(FistButton);
			window.draw(CaptureButton);
	
			window.display();
		}
	}

    return 0;
}