#include "mymath.h"

float getAngle(sf::Vector2f kto, sf::Vector2f kuda){
	return -1*atan2(kuda.y-kto.y,kuda.x-kto.x)*180/3.1415;
}

float getAngle(float dx, float dy)
{
	return -1*atan2(dy,dx)*180/3.1415;
}
