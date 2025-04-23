#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include "WelcomeWindow.hpp"

using std::map; using std::string; using std::vector; using std::cerr; using std::endl; using std::ifstream; using std::to_string;
using std::istreambuf_iterator;
using sf::Texture; using sf::RenderWindow; using sf::Event; using sf::Sprite; using sf::VideoMode; using sf::Font; using sf::Text;

void FileReader(const string& filename, vector<unsigned char>& FontData){
	ifstream file;

	file.open("files/" + filename, std::ios::binary);

	if(!file.is_open()){
		cerr<<"Failed to load: files/"<<filename<<endl;
		return;
	}
	FontData.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	file.close();
}

void SetText(Text& text, float x, float y){
	FloatRect TextRect = text.getLocalBounds();

	text.setOrigin(TextRect.left + TextRect.width/2.0f,TextRect.top + TextRect.height/2.0f);
	text.setPosition(Vector2f(x, y));
}

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
	vector<unsigned char> FontData;
	Font font;

	WelcomeWindow welcome_window;
	welcome_window.Run();

	FileReader("MorganChalk-L3aJy.ttf", FontData);

	if(!font.loadFromMemory(FontData.data(), FontData.size())){
		cerr<<"Failed to open font."<<endl;
	}
	Text CaptureText("", font);
	Text CloseText("", font);
	Text OpenText("", font);

	CaptureText.setString("Capture");
	CloseText.setString("Close");
	OpenText.setString("Open");

	CaptureText.setCharacterSize(24);
	CloseText.setCharacterSize(24);
	OpenText.setCharacterSize(24);

	CaptureText.setFillColor(Color::Black);
	CloseText.setFillColor(Color::Black);
	OpenText.setFillColor(Color::Black);

	SetText(CaptureText, 1443.0f, 1030.0f);
	SetText(CloseText, 210.0f, 1030.0f);
	SetText(OpenText, 840.0f, 1030.0f);

	CaptureText.setScale(1.5, 1.8);
	CloseText.setScale(2.4, 1.5);
	OpenText.setScale(2.4, 1.5);

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

		OpenHandButton.setPosition(700.0f, 900.0f);
		FistButton.setPosition(70.0f, 900.0f);
		CaptureButton.setPosition(1300.0f, 900.0f);
		Geisha.setPosition(600.0f, 250.0f);
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
			window.draw(OpenText);
			window.draw(FistButton);
			window.draw(CloseText);
			window.draw(CaptureButton);
			window.draw(CaptureText);
	
			window.display();
		}
	}

    return 0;
}