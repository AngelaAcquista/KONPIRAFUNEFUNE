#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include "WelcomeWindow.hpp"

using std::map; using std::string; using std::vector; using std::cerr; using std::endl; using std::ifstream; using std::to_string;
using std::istreambuf_iterator; using std::random_device; using std::uniform_int_distribution; using std::mt19937;
using sf::Texture; using sf::RenderWindow; using sf::Event; using sf::Sprite; using sf::VideoMode; using sf::Font; using sf::Text; using sf::Mouse;
using sf::SoundBuffer; using sf::Sound;

void FileReader(const string& filename, vector<unsigned char>& FontData){
	ifstream file;

	file.open("files/" + filename, std::ios::binary);

	if(!file.is_open()) {
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

void TextureLoad(map<string, Texture>& TextureMap){
    vector<string> TextureNames = {"tablewithoutblock.png", "welcome_title.png", "tablewithblock.png", "welcome_button.png","geisha.png", "open_hand_computer.png","background.png","object_in_center.png","computer_fist_on_table.png"};

    for(const string& name : TextureNames){
        Texture texture;

        if(texture.loadFromFile("files/" + name)) TextureMap[name] = texture;
        else cerr<<"Failed to open files/" + name<<endl;
    }
}

int main(){
	WelcomeWindow welcome_window;
	welcome_window.Run();

	if(welcome_window.ShowVideo){
		// Play the demo video
	}
	if(welcome_window.GameStart){
		// Start the game
		RenderWindow window(VideoMode(1600, 1200), "Konpira");

		random_device rand;
		mt19937 gen(rand());
		int count = 0, computermove = 0;
		bool GameOver, Won, Captured, ComputerCaptured, PlayerTurn = false;
		uniform_int_distribution<> move(0, 2);
		SoundBuffer buffer;
		Sound sound;
		map<string, Texture> TextureMap;
		vector<unsigned char> FontData;
		Font font;

		FileReader("MorganChalk-L3aJy.ttf", FontData);

		if(!font.loadFromMemory(FontData.data(), FontData.size())) cerr<<"Failed to open font."<<endl;

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
		SetText(LostText, 800.0f, 280.0f);
		SetText(WonText, 800.0f, 280.0f);
		SetText(TryAgainText, 850.0f, 710.0f);

		CaptureText.setScale(1.5, 1.8);
		CloseText.setScale(2.4, 1.5);
		OpenText.setScale(2.4, 1.5);
		LostText.setScale(10.0, 10.0);
		WonText.setScale(10.0, 10.0);
		TryAgainText.setScale(2.4, 1.5);

		if(!buffer.loadFromFile("files/KonpiraFuneFune_soundtrack.wav")) cerr<<"Failed to load soundtrack"<<endl;

		sound.setBuffer(buffer);
		sound.play();

		TextureLoad(TextureMap);

		Sprite GameOverSign(TextureMap.at("welcome_title.png")), TableWithoutBlock(TextureMap.at("tablewithoutblock.png")), TableWithBlock(TextureMap.at("tablewithblock.png")), Geisha(TextureMap.at("geisha.png")), TryAgainButton(TextureMap.at("welcome_button.png")), Background(TextureMap.at("background.png")), OpenHandButton(TextureMap.at("welcome_button.png")), FistButton(TextureMap.at("welcome_button.png")), CaptureButton(TextureMap.at("welcome_button.png"));

		Background.setScale(1.6, 1.6);
		OpenHandButton.setScale(0.5, 0.5);
		FistButton.setScale(0.5, 0.5);
		CaptureButton.setScale(0.5, 0.5);
		TryAgainButton.setScale(0.9, 0.9);
		GameOverSign.setScale(0.6, 0.8);

		OpenHandButton.setPosition(700.0f, 900.0f);
		FistButton.setPosition(70.0f, 900.0f);
		TryAgainButton.setPosition(600.0f, 480.0f);
		CaptureButton.setPosition(1300.0f, 900.0f);
		Geisha.setPosition(600.0f, 250.0f);
		TableWithBlock.setPosition(780.0f, 630.0f);
		TableWithoutBlock.setPosition(780.0f, 630.0f);
		Background.setOrigin(0, 220.f);
		GameOverSign.setPosition(200.f, -500.f);

		while(window.isOpen()){
			Event event;

			while(window.pollEvent(event)){
				if(event.type == Event::Closed) window.close();

				if(sound.getStatus() == Sound::Stopped) {
					sound.play();
				}
				if(!PlayerTurn){
					PlayerTurn = true;

					if(!Captured && count % 5 == 0) ComputerCaptured = true;
					else{
						Captured = false;
						computermove = move(gen);
						if(computermove == 0){ //fist or close option
							GameOver = true;
							Won = true;
						}
					}
					count++;
				}
				if(event.type == Event::MouseButtonPressed){
					Vector2i MousePos = Mouse::getPosition(window);

					if(PlayerTurn && !GameOver){
						if(OpenHandButton.getGlobalBounds().contains(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y))){
							if(ComputerCaptured){
								GameOver = true;
								Won = false;
							}
							ComputerCaptured = false;
							PlayerTurn = false;
							count++;
						}else if(FistButton.getGlobalBounds().contains(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y))){
							if(!ComputerCaptured){
								GameOver = true;
								Won = false;
							}
							ComputerCaptured = false;
							PlayerTurn = false;
							count++;
						}else if(CaptureButton.getGlobalBounds().contains(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y))){
							Captured = true;
							PlayerTurn = false;
							count++;
						}
					}
					if(TryAgainButton.getGlobalBounds().contains(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y))){
						GameOver = false;
						Won = false;
						PlayerTurn = false;
						sound.play();
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

			if(!ComputerCaptured && !Captured){
				window.draw(CaptureButton);
				window.draw(CaptureText);
				window.draw(TableWithBlock);
			}else window.draw(TableWithoutBlock);

			if(GameOver){
				window.draw(GameOverSign);

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