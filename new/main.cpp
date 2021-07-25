#include "stdafx.h"
#include "Font.h"

class TLibTextures 
{
public: 
	static TLibTextures& Instance()
	{
		static TLibTextures lib;
		return lib;
	}
	sf::Texture* getTexturePtr(int p_texture_id)
	{
		if(0 < p_texture_id && p_texture_id < m_vec_textures.size())
		{
			return m_vec_textures[p_texture_id];
		}
		else 
		{
			return m_vec_textures[0];
		}
	}
private:
	TLibTextures()
	{
		load();
	}
	~TLibTextures()
	{
		for (int i=0; i<m_vec_textures.size(); ++i)
			if (m_vec_textures[i])
				delete m_vec_textures[i];
		m_vec_textures.clear();	
	}
	void load()
	{
		// Нулевая текстура
		// Нужна для того, чтобы возвращаться при ненайденной текстурке
		sf::Texture* v_texture = new sf::Texture();
		v_texture->loadFromFile("src/images/null.png");
		m_vec_textures.push_back(v_texture);
		
		v_texture = new sf::Texture();
		v_texture->loadFromFile("src/images/1.png");
		m_vec_textures.push_back(v_texture);
	}
	
	void addTexture (int p_id, std::string p_filename)
	{
		sf::Texture* v_texture = new sf::Texture();
		v_texture->loadFromFile( p_filename );
		m_vec_textures[p_id] = v_texture;
	}
private:
	std::vector <sf::Texture*> m_vec_textures;
};

// Базовый объект для GUI-элементов
class TObject 
{
public:
	TObject()
	{
		//
	}
	~TObject()
	{
		//
	}
	sf::Vector2f getSize() { sf::Vector2f(0,0); }
	sf::Vector2f getPosition () { sf::Vector2f(0,0); }
	// Проверить, входит ли объект в координаты
	virtual bool check1(sf::Vector2f p_pos)
	{
		return false;
	}
private:
	//...
};

// Кнопка
class TButton : public TObject
{
public:
	TButton() : m_sprite (nullptr), m_text(nullptr) { }
	
	TButton( int p_type_id ) : m_sprite (nullptr), m_text(nullptr)
	{
		// p_type_id - ID тип кнопки
		m_sprite = new sf::Sprite (*TLibTextures::Instance().getTexturePtr(1) );
		m_sprite->setPosition( 5+p_type_id*(64+5), 0 );
		setText("Кнопка");
		m_text->setPosition( 10+p_type_id*(64+5), 5 );
	}
	
	~TButton()
	{
		// Возможно удаление будет в другом месте (Фабрике объектов)
		if (m_sprite) 
			delete m_sprite;
	}
	
	// Поведение кнопки
	void behavior(float time)
	{
		//...
	}
	
	// Отрисовка кнопки
	void draw(sf::RenderWindow &window)
	{
		if (m_sprite) 
		{
			window.draw(*m_sprite);
			window.draw(*m_text);
		}
	}
	
	// Задать текст для кнопки
	void setText(std::string p_string)
	{
		if (m_text)
		{
			m_text->setString(p_string);
		}
		else
		{
			m_text = new sf::Text();
			m_text->setFont(myFont::Instance());
			m_text->setString(p_string);
			m_text->setCharacterSize(16);
			m_text->setColor(sf::Color::Black);
		}
	}
	
	void move(float p_x, float p_y)
	{
		if(m_sprite)
			m_sprite->move(p_x, p_y);
			
		if(m_text)
			m_text->move(p_x, p_y);
	}
	
	virtual bool check1(sf::Vector2f p_pos)
	{
		if( m_sprite.getPosition );
	}
	
private:
	sf::Sprite *m_sprite;
	sf::Text   *m_text;
};



int main()
{
	TObject* m_select_object        = nullptr; // Выделенный объект
	TObject* m_drag_and_drop_object = nullptr; // Передвигаемый объект
	
	std::vector <TButton*> vecButtons;
	
	for (int i=0; i<10; ++i)
	{
		TButton *v_button = new TButton ( i );
		//...
		vecButtons.push_back( v_button );
	}
	
	
	// Mouse
	sf::Vector2f mouse_pos;
	bool mouse[5];
	for(int i=0; i<5; ++i) mouse[i] = false;
	
	// Keyboard
	bool keys[101];
	for(int i=0; i<101; ++i) keys[i] = false;
	
	// Window
    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML works!");
    
    // Time
    sf::Clock clock;
    float time = 0.0f;
    

    while (window.isOpen())
    {
    	time = clock.getElapsedTime().asSeconds();
    	clock.restart();
    	
        sf::Event event;
        while (window.pollEvent(event))
        {
        	// Событие закрытия окна
            if (event.type == sf::Event::Closed)
            {
            	window.close();
			}
                
            // Событие перемещения мыши
            else if (event.type == sf::Event::MouseMoved)
            {
            	// Запоминаем координаты мыши
				mouse_pos.x = event.mouseMove.x;
				mouse_pos.y = event.mouseMove.y;
			}
			
			// Событие прокрутки колёсика
			else if(event.type == 7)
			{
				// 1 - вверх / -1 - вниз
				int delta = event.mouseWheel.delta;
				// cout << "delta = " << delta << endl;
				vecButtons[0]->move(0, -1*delta);
			}
			
			// Событие нажатия кнопки мыши
			else if(event.type == sf::Event::MouseButtonPressed)
			{
				int button = event.mouseButton.button;
				mouse[event.mouseButton.button] = true;
				
			}
			
			// Событие отжатия кнопки мыши
			else if(event.type == sf::Event::MouseButtonReleased)
			{
				mouse[event.mouseButton.button] = false;
			}
			
        }
        
		//cout << time << endl;
		
        window.clear();
        for (int i=0; i<vecButtons.size(); ++i)
        {
        	vecButtons[i]->draw(window);
		}
        window.display();
    }

    return 0;
}
