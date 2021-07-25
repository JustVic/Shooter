#include <iostream>
#include <vector>
#include <iterator>
#include <string> 
#include <sstream>
#include <math.h> // sqrt()
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#define nullptr 0

using namespace std;
using namespace sf;

typedef sf::Vector2f  vec2f;
typedef sf::Vector2i  vec2i;
typedef sf::Vector2u  vec2u;

// Для работы to_string() в Dev-C++
std::string to_string(int val);
int from_string(const std::string& s) ;
