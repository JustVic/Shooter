
#ifndef DSL_LIB_TEXTURES_H
#define DSL_LIB_TEXTURES_H

#include <SFML/Graphics/Texture.hpp>
#include <vector>

#define nullptr 0
#define CONST_TEXTURE_NARUTO_TEST 1
#define CONST_RECT_ITACHI_TELEPORT1 1
#define CONST_RECT_ITACHI_TELEPORT2 2
#define CONST_RECT_ITACHI_TELEPORT3 3
#define CONST_RECT_ITACHI_TELEPORT4 0
#define CONST_RECT_ITACHI_TELEPORT5 3
#define CONST_RECT_ITACHI_TELEPORT6 2
#define CONST_RECT_ITACHI_TELEPORT7 1

class LibTextures
{
private:
	std::vector<sf::Texture*> vect;
	std::vector<sf::IntRect*> m_vec_rects;
public:
	// Инстанцировать библиотеку текстур (всегда одну и ту же)
	static LibTextures& Instance()
	{
		static LibTextures lib;
		return lib;
	} 	
	// Вернуть ссылку на текстуру
	sf::Texture& getTexture(int id)
	{
		if(0 <= id && id < vect.size() && vect[id])
			return *vect[id];
		else
			return *vect[0];
	}
	// Вернуть указатель на текстуру
	sf::Texture* getTexturePtr(int id)
	{
		if(0 <= id && id < vect.size())
			return vect[id];
		else
			return 0;
	}
	// Вернуть квадрат текстуры
	sf::IntRect getIntRect(int p_rect_id)
	{
		if(0 < p_rect_id && p_rect_id < m_vec_rects.size() && m_vec_rects[p_rect_id])
			return *m_vec_rects[p_rect_id];
		else
			return sf::IntRect(0,0,32,32);
	}
private:
	// Конструктор
	LibTextures()
	{ 
		for(int i=0; i<1000; i++) 
			vect.push_back(nullptr);
			
		for(int i=0; i<1000; i++) 
			m_vec_rects.push_back(nullptr);
			
		load(); 
	}
	
	// Деструктор - удаляет все текстуры
	~LibTextures()
	{
		// Высвобождаем память вектора текстур
		for(int i=0; i<vect.size(); ++i)
			if(vect[i]) 
				delete vect[i];
		vect.clear();
		
		// Высвобождаем память вектора квадратов текстур
		for(int i=0; i<m_vec_rects.size(); ++i)
			if(m_vec_rects[i]) 
				delete m_vec_rects[i];	
		m_vec_rects.clear();
	}
	
	// Загрузить данные
	void load()
	{
		// Нулевая текстурка
		addTexture( 0, "src/images/null.png");
		
		// Тестовые анимации
		addTexture( 1, "src/images/test_animations/test1.png");
		addTexture( 2, "src/images/test_animations/test2.png");
		
		// Добавляем квадраты текстур
		addIntRect(0,   0,   0, 32, 32);
		addIntRect(1,   5, 145, 30, 55);
		addIntRect(2,   5, 226, 30, 52);
		addIntRect(3,  43, 226, 30, 52);
		
		// Run
		addIntRect(4,  24, 315, 48, 50);
		addIntRect(5,  84, 315, 56, 45);
		addIntRect(6, 152, 317, 57, 47);
		addIntRect(7, 224, 316, 45, 48);
		addIntRect(8, 284, 315, 54, 45);
		addIntRect(9, 349, 317, 58, 47);
		
		// Stance Right
		addIntRect(10, 326, 145, 18, 54);
		addIntRect(11, 350, 145, 18, 54);
		addIntRect(12, 375, 145, 20, 54);
		addIntRect(13, 402, 145, 18, 54);
		
		// Stance Top
		addIntRect(14, 162, 147, 27, 52);
		addIntRect(15, 195, 147, 27, 52);
		addIntRect(16, 227, 147, 28, 52);
		addIntRect(17, 261, 147, 27, 52);
		
		// Stance Down
		addIntRect(18,   5, 147, 27, 52);
		addIntRect(19,  38, 147, 27, 52);
		addIntRect(20,  71, 147, 28, 52);
		addIntRect(21, 105, 147, 27, 52);
		
		// Stance Left
		addIntRect(22, 326+18, 145, -18, 54);
		addIntRect(23, 350+18, 145, -18, 54);
		addIntRect(24, 375+20, 145, -20, 54);
		addIntRect(25, 402+18, 145, -18, 54);
		
		// Move left
		addIntRect(26,  24+48, 315, -48, 50);
		addIntRect(27,  84+56, 315, -56, 45);
		addIntRect(28, 152+57, 317, -57, 47);
		addIntRect(29, 224+45, 316, -45, 48);
		addIntRect(30, 284+54, 315, -54, 45);
		addIntRect(31, 349+58, 317, -58, 47);
	}
	
	// Функция добавления текстуры по ID и имени файла
	void addTexture(int id, std::string filename)
	{
		sf::Texture *v_texture = new sf::Texture();
		v_texture->loadFromFile(filename);
		vect[id] = v_texture;
	}
	// Добавить квадрат текстуры
	void addIntRect(int p_rect_id, int p_x, int p_y, int p_w, int p_h)
	{
		if (0 <= p_rect_id && p_rect_id <= m_vec_rects.size())
		{
			m_vec_rects[p_rect_id] = new sf::IntRect(p_x, p_y, p_w, p_h);
		}
		else
		{
			std::cout << "WARNING: LibTextures::addIntRect("
			          << p_rect_id << ", " 
					  << p_x << ", " 
					  << p_y << ", " 
					  << p_w << ", " 
					  << p_h << "). Не удалось добавить квадрат текстуры.";
		}
	}
};

#endif



