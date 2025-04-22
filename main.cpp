#include <iostream>
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include <fstream>

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
    vector<string> TextureNames = {"Geisha.png", "OpenHandButton.png", "OpenHandComputer.png", "FistButton.png","Background.png","CaptureObjectButton.png","HandHoldingTheThing.png","ObjectInCenter.png","ComputerFistOnTable.png"};

    for(const string& name : TextureNames){
        Texture texture;
        if(texture.loadFromFile("files/" + name)){
            TextureMap[name] = texture;
        }else cerr<<"Failed to open files/" + name<<endl;
    }
    return TextureMap;
}

int main(){
    RenderWindow window(sf::VideoMode(800, 600), "Konpira");

    map<string, Texture> TextureMap = TextureLoad();
    Sprite OpenHandButton(TextureMap.at("OpenHandButton.png")), FistButton(TextureMap.at("FistButton.png")), CaptureButton(TextureMap.at("CaptureObjectButton.png"));

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
    return 0;
}