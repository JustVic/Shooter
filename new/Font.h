
#ifndef DSL_CLIENT_FONT_H
#define DSL_CLIENT_FONT_H

#include "stdafx.h"

class myFont : public sf::Font
{
public:
	static myFont& Instance()
	{
		static myFont font;
		return font;
	}
	
	myFont()
	{
		loadFromFile("src/fonts/myFont.ttf");
	}
};

#endif
