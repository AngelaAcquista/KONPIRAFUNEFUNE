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

map<string, Texture> TextureLoad(){
    map<string, Texture> TextureMap;
    vector<string> TextureNames = {"Geisha.png", "open hand button.png", "open hand computer.png", "fist button.png","Background.png","capture object button.png","Hand holding the thing.png","object in center.png","computer fist on table.png"};

    for(const string& name : TextureNames){
        Texture texture;
        if(texture.loadFromFile("files/" + name)){
            TextureMap[name] = texture;
        }else cerr<<"Failed to open files/" + name<<endl;
    }
    return TextureMap;
}

int main() {
    RenderWindow window(sf::VideoMode(800, 600), "Konpira");

    while(window.isOpen()) {
        Event event;
        while(window.pollEvent(event)) {
            if(event.type == Event::Closed) {
                window.close();
            }

        }

        window.clear();

        window.display();
    }
    return 0;
}