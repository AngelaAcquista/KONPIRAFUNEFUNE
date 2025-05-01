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
using sf::SoundBuffer; using sf::Sound; using sf::RenderTexture;

void FileReader(const string& filename, vector<unsigned char>& FontData){
	/*
	Pass in a file path and a vector of unsigned char by reference
	No return, but read the data from the file into the vector of unsigned char for later use
	*/

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
	/*
	Pass in a sf::Text object by reference and two floats representing the position to put the text
	No return, but set the position of the sf::Text object
	*/

	FloatRect TextRect = text.getLocalBounds();

	text.setOrigin(TextRect.left + TextRect.width/2.0f,TextRect.top + TextRect.height/2.0f);
	text.setPosition(Vector2f(x, y));
}

void TextureLoad(map<string, Texture>& TextureMap){
	/*
	Pass in a map with file names as the keys and their corresponding sf::Texture objects as the values by reference
	No return, but load the sf::Texture objects from the local .png files
	*/

    vector<string> TextureNames = {"tryagainbutton.png","capturebutton.png", "openbutton.png", "tablewithoutblock.png", "closebutton.png", "backgroundforgame.png", "welcome_title.png", "tablewithblock.png"};

    for(const string& name : TextureNames){
        Texture texture;

        if(texture.loadFromFile("files/" + name)) TextureMap[name] = texture;
        else cerr<<"Failed to open files/" + name<<endl;
    }
}

int main(){
	// Launch the welcome window
	WelcomeWindow welcome_window;
	welcome_window.Run();
	
	if(welcome_window.GameStart){
		// Start the game
		RenderWindow window(VideoMode(1600, 1200), "Konpira");

		RenderTexture staticLayerTexture;

		if(!staticLayerTexture.create(1600, 1200)) cerr << "Failed to create static layer texture" << endl;

		staticLayerTexture.clear(Color::Transparent);
		random_device rand;
		mt19937 gen(rand());
		int count = 0, computermove = 0;
		bool GameOver, Won, Captured, ComputerCaptured, PlayerTurn = false;
		uniform_int_distribution<> move(0, 10);
		SoundBuffer buffer;
		Sound sound;
		map<string, Texture> TextureMap;
		vector<unsigned char> FontData;
		Font font;

		FileReader("MorganChalk-L3aJy.ttf", FontData);

		if(!font.loadFromMemory(FontData.data(), FontData.size())) cerr<<"Failed to open font."<<endl;
		//these are the endgame texts and their respective positions and size
		Text LostText("", font), WonText("", font);

		LostText.setString("You Lost!");
		WonText.setString("You Won!");

		LostText.setCharacterSize(24);
		WonText.setCharacterSize(24);

		LostText.setFillColor(Color::Black);
		WonText.setFillColor(Color::Black);

		SetText(LostText, 800.0f, 280.0f);
		SetText(WonText, 800.0f, 280.0f);

		LostText.setScale(10.0, 10.0);
		WonText.setScale(10.0, 10.0);
		//starts sound
		if(!buffer.loadFromFile("files/KonpiraFuneFune_soundtrack.wav")) cerr<<"Failed to load soundtrack"<<endl;

		sound.setBuffer(buffer);
		sound.play();
		//loads textures
		TextureLoad(TextureMap);
		//makes sprites and their respective postitions
		Sprite GameOverSign(TextureMap.at("welcome_title.png")), TableWithoutBlock(TextureMap.at("tablewithoutblock.png")), TableWithBlock(TextureMap.at("tablewithblock.png")), TryAgainButton(TextureMap.at("tryagainbutton.png")), Background(TextureMap.at("backgroundforgame.png")), OpenHandButton(TextureMap.at("openbutton.png")), FistButton(TextureMap.at("closebutton.png")), CaptureButton(TextureMap.at("capturebutton.png"));

		Background.setScale(1.1, 1.1);
		GameOverSign.setScale(0.6, 0.8);

		OpenHandButton.setPosition(700.0f, 915.0f);
		FistButton.setPosition(70.0f, 920.0f);
		TryAgainButton.setPosition(600.0f, 480.0f);
		CaptureButton.setPosition(1300.0f, 895.0f);
		TableWithBlock.setPosition(830.0f, 680.0f);
		TableWithoutBlock.setPosition(800.0f, 670.0f);
		GameOverSign.setPosition(200.f, -500.f);
		//everything that remains unchanging during the game I set as one sprite so that it doesn't have to redraw it as individual stuff every iteration
		staticLayerTexture.draw(Background);
		staticLayerTexture.draw(OpenHandButton);
		staticLayerTexture.draw(FistButton);

		staticLayerTexture.display();

		Sprite staticLayer(staticLayerTexture.getTexture());
		//game loop
		while(window.isOpen()){
			Event event;

			while(window.pollEvent(event)){
				if(event.type == Event::Closed) window.close();

				if(sound.getStatus() == Sound::Stopped) {
					sound.play();
				}
				//computer only plays if it's not the player's turn
				if(!PlayerTurn){
					PlayerTurn = true;

					if(!Captured && count % 7 == 0) ComputerCaptured = true;
					else{
						Captured = false;
						computermove = move(gen);
						//random mess up option
						if(computermove == 0 && count > 5){
							GameOver = true;
							Won = true;
						}
					}
					count++;
				}
				if(event.type == Event::MouseButtonPressed){
					Vector2i MousePos = Mouse::getPosition(window);
					//makes sure player can't make a move when it's not their turn or when the game is over
					if(PlayerTurn && !GameOver){
						if(OpenHandButton.getGlobalBounds().contains(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y))){
							//when the player plays an open hand while the computer has it captured it ends the game and the player loses
							if(ComputerCaptured){
								GameOver = true;
								Won = false;
							}
							ComputerCaptured = false;
							PlayerTurn = false;
							count++;
						}else if(FistButton.getGlobalBounds().contains(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y))){
							//when the player plays a closed hand while the computer doesn't have it captured it ends the game and the player loses
							if(!ComputerCaptured){
								GameOver = true;
								Won = false;
							}
							ComputerCaptured = false;
							PlayerTurn = false;
							count++;
						}else if(!ComputerCaptured && CaptureButton.getGlobalBounds().contains(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y))){
							//allows player to capture when it's not captured by the computer
							Captured = true;
							PlayerTurn = false;
							count++;
						}
					}
					if(TryAgainButton.getGlobalBounds().contains(static_cast<float>(MousePos.x), static_cast<float>(MousePos.y))){
						//at the end of the game it resets everything when pressed and allows the player to start a new game if they want
						GameOver = false;
						Won = false;
						PlayerTurn = false;
						sound.play();
						count = 0;
					}
				}
			}
			window.clear();

			window.draw(staticLayer);
			//the capture button and table with the block in center only show up if the block hasn't been captured by either the player or the computer
			if(!ComputerCaptured && !Captured){
				window.draw(CaptureButton);
				window.draw(TableWithBlock);
			}else window.draw(TableWithoutBlock);
			//only drawn if the game is over
			if(GameOver){
				window.draw(GameOverSign);
				//if the player won it shows the winner text, otherwise it draws the loser text
				if(Won) window.draw(WonText);
				else window.draw(LostText);

				window.draw(TryAgainButton);
			}
			window.display();
		}
	}
    return 0;
}