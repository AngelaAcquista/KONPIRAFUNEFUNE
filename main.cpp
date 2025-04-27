#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include "WelcomeWindow.hpp"

using std::map; using std::string; using std::vector; using std::cerr; using std::endl; using std::ifstream; using std::to_string;
using std::istreambuf_iterator; using std::random_device; using std::uniform_int_distribution; using std::mt19937;
using sf::Texture; using sf::RenderWindow; using sf::Event; using sf::Sprite; using sf::VideoMode; using sf::Font; using sf::Text; using sf::Mouse;

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
    vector<string> TextureNames = {"tablewithblock.png", "welcome_button.png","geisha.png", "open_hand_computer.png","background.png","object_in_center.png","computer_fist_on_table.png"};

    for(const string& name : TextureNames){
        Texture texture;
        if(texture.loadFromFile("files/" + name)){
            TextureMap[name] = texture;
        }else cerr<<"Failed to open files/" + name<<endl;
    }
    return TextureMap;
}

int main(){
	random_device rand;
	mt19937 gen(rand());
	uniform_int_distribution<> move(0, 2);
	vector<unsigned char> FontData;
	Font font;
	bool GameOver, Won, Captured, ComputerCaptured, PlayerWent;

	WelcomeWindow welcome_window;
	welcome_window.Run();

	FileReader("MorganChalk-L3aJy.ttf", FontData);

	if(!font.loadFromMemory(FontData.data(), FontData.size())){
		cerr<<"Failed to open font."<<endl;
	}
	Text CaptureText("", font), CloseText("", font), OpenText("", font), TryAgainText("", font), LostText("", font), WonText("", font);

	CaptureText.setString("Capture");
	CloseText.setString("Close");
	OpenText.setString("Open");
	LostText.setString("You Lost!");
	WonText.setString("You Won!");
	TryAgainText.setString("Try Again?");

	CaptureText.setCharacterSize(24);
	CloseText.setCharacterSize(24);
	OpenText.setCharacterSize(24);
	LostText.setCharacterSize(24);
	WonText.setCharacterSize(24);
	TryAgainText.setCharacterSize(24);

	CaptureText.setFillColor(Color::Black);
	CloseText.setFillColor(Color::Black);
	OpenText.setFillColor(Color::Black);
	LostText.setFillColor(Color::Black);
	WonText.setFillColor(Color::Black);
	TryAgainText.setFillColor(Color::Black);

	SetText(CaptureText, 1443.0f, 1030.0f);
	SetText(CloseText, 210.0f, 1030.0f);
	SetText(OpenText, 840.0f, 1030.0f);
	SetText(LostText, 800.0f, 300.0f);
	SetText(WonText, 800.0f, 300.0f);
	SetText(TryAgainText, 850.0f, 710.0f);

	CaptureText.setScale(1.5, 1.8);
	CloseText.setScale(2.4, 1.5);
	OpenText.setScale(2.4, 1.5);
	LostText.setScale(10.0, 10.0);
	WonText.setScale(10.0, 10.0);
	TryAgainText.setScale(2.4, 1.5);

	if(welcome_window.ShowVideo){
		// Play the demo video
	}
	if(welcome_window.GameStart){
		// Start the game
		RenderWindow window(VideoMode(1600, 1200), "Konpira");

		map<string, Texture> TextureMap = TextureLoad();

		Sprite TableWithBlock(TextureMap.at("tablewithblock.png")), Geisha(TextureMap.at("geisha.png")), TryAgainButton(TextureMap.at("welcome_button.png")), Background(TextureMap.at("background.png")), OpenHandButton(TextureMap.at("welcome_button.png")), FistButton(TextureMap.at("welcome_button.png")), CaptureButton(TextureMap.at("welcome_button.png"));

		Background.setScale(1.6, 1.6);
		OpenHandButton.setScale(0.5, 0.5);
		FistButton.setScale(0.5, 0.5);
		CaptureButton.setScale(0.5, 0.5);
		TryAgainButton.setScale(0.9, 0.9);

		OpenHandButton.setPosition(700.0f, 900.0f);
		FistButton.setPosition(70.0f, 900.0f);
		TryAgainButton.setPosition(600.0f, 480.0f);
		CaptureButton.setPosition(1300.0f, 900.0f);
		Geisha.setPosition(600.0f, 250.0f);
		TableWithBlock.setPosition(780.0f, 630.0f);
		Background.setOrigin(0, 220.f);
	
		while(window.isOpen()){
			Event event;

			while(window.pollEvent(event)){
				if(event.type == Event::Closed) window.close();
				int computermove;
				if(PlayerWent){
					PlayerWent = false;
					if(Captured) while(computermove == 2) computermove = move(gen);

					if(computermove == 2) ComputerCaptured = true; //capture

					if(computermove == 0){
						//open
						if(Captured){
							GameOver = true;
							Won = true;
						}
						Captured = false;
					}
					if(computermove == 1){
						//close
						if(!Captured){
							GameOver = true;
							Won = true;
						}
						Captured = false;
					}
				}
				if(event.type == Event::MouseButtonPressed) {
					Vector2i MousePos = Mouse::getPosition(window);

					if(!GameOver && OpenHandButton.getGlobalBounds().contains(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y))){
						if(ComputerCaptured){
							GameOver = true;
							Won = false;
						}
						ComputerCaptured = false;
						PlayerWent = true;
					}
					if(!GameOver && FistButton.getGlobalBounds().contains(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y))){
						if(!ComputerCaptured){
							GameOver = true;
							Won = false;
						}
						ComputerCaptured = false;
						PlayerWent = true;
					}
					if(!GameOver && CaptureButton.getGlobalBounds().contains(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y))){
						Captured = true;
						PlayerWent = true;
					}
					if(TryAgainButton.getGlobalBounds().contains(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y))){
						GameOver = false;
						Won = false;
						PlayerWent = false;
					}
				}
			}
			window.clear();

			window.draw(Background);
			window.draw(Geisha);
			window.draw(OpenHandButton);
			window.draw(OpenText);
			window.draw(FistButton);
			window.draw(CloseText);
			window.draw(TableWithBlock);

			if(!ComputerCaptured){
				window.draw(CaptureButton);
				window.draw(CaptureText);
			}
			if(GameOver){
				if(Won) window.draw(WonText);
				else window.draw(LostText);
				window.draw(TryAgainButton);
				window.draw(TryAgainText);
			}
			window.display();
		}
	}
    return 0;
}