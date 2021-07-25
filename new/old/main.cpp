#include "stdafx.h"
#include "LibTextures.h"
#include "mymath.h" // getAngle()
#include "txtparser.h" // struct parser_JamProject 

float win_scale = 3.0f; 
float win_w = 1000;
float win_h = 700;
float view_w = 0;
float view_h = 0;
float view_x = 0;
float view_y = 0;

struct Object : public sf::Sprite {
	bool is_mirror;
	int img_id;
	sf::Image *mask; // маска для определения столкновения
	sf::Sprite s_mask;
	//=================================
	
	Object() : mask(0), img_id(0), is_mirror(false) { }
	
	Object(Object* obj, int id) : mask(0), img_id(0), is_mirror(false) {
		img_id = id;
		setTexture(LibJam::Instance().get_texture(id));
		mask   =  &LibJam::Instance().get_image(id+1000);
		setPosition(obj->getPosition());
		s_mask.setTexture(LibJam::Instance().get_texture(id+1000));
		s_mask.setColor(sf::Color(255,255,255,50));
		int w = getTexture()->getSize().x;
		int h = getTexture()->getSize().y;
		//cout << "size=" << -w/2 << ':' << -h/2 << endl;
		move(-w/2, -h/2);
		if(obj->is_mirror) mirror();
	}
	
	Object(int id, int x, int y, bool mirrored)  : mask(0), img_id(0), is_mirror(false)
	{	
		img_id = id;
		setTexture(LibJam::Instance().get_texture(id));
		mask   =  &LibJam::Instance().get_image(id+1000);
		setPosition(x,y);
		s_mask.setTexture(LibJam::Instance().get_texture(id+1000));
		s_mask.setColor(sf::Color(255,255,255,50));
		if(mirrored) mirror();
	}
	
	void mirror()
	{
		// Был баг... WTF?
		if(is_mirror == true){
			is_mirror = false;
		} else {
			is_mirror = true;
		}
		//is_mirror = !is_mirror;
		
		sf::Vector2u size  =        getTexture()->getSize();
		sf::Vector2u size2 = s_mask.getTexture()->getSize();
		if(is_mirror){
			       setTextureRect(sf::IntRect( size.x,0, -size.x,size.y));
			s_mask.setTextureRect(sf::IntRect(size2.x,0,-size2.x,size2.y));
		} else {
			       setTextureRect(sf::IntRect(0, 0, size.x, size.y));
			s_mask.setTextureRect(sf::IntRect(0, 0, size2.x, size2.y));
		}
	}
	virtual void draw(sf::RenderWindow &win){
		win.draw(*this);
	}
	virtual void draw_mask(sf::RenderWindow &win){
		win.draw(s_mask);
	}
	bool check1(sf::Vector2f &pos){
		sf::Image img = LibJam::Instance().get_image(img_id);
		float dx = pos.x - getPosition().x;
		float dy = pos.y - getPosition().y;
		
		if(0 < dx && dx < img.getSize().x && 0 < dy && dy < img.getSize().y){
			sf::Color pxl = img.getPixel(dx,dy);
			if(pxl.a > 0) 
				return true; 	 
		}		
		return false;
	}
	void setPosition(float x, float y){
		sf::Sprite::setPosition(x,y);
		s_mask.setPosition(x,y);
	}
	void setPosition(sf::Vector2f pos){
		sf::Sprite::setPosition(pos);
		s_mask.setPosition(pos);
	}
	void move(float x, float y){
		sf::Sprite::move(x,y);
		s_mask.move(x,y);
	}
	void move(sf::Vector2f pos){
		sf::Sprite::move(pos);
		s_mask.move(pos);
	}
};

struct Unit : Object {
	sf::Vector2f prev_prev_pos;
	sf::Vector2f prev_pos;
	sf::Vector2f target_move_pos;
	bool is_move;
	float speed;
	float speedX;
	float speedY;
	float angle;
	float dist;
	//-------------------------
	Unit() : is_move(false),
	         speed(100),
		     speedY(0),
			 speedX(0),
			 angle(0),
			 dist(0) 
	{
		if(rand()%2) setTexture(LibJam::Instance().get_texture(9));
		        else setTexture(LibJam::Instance().get_texture(8));
		mask = &LibJam::Instance().get_image(1005);
		//setPosition(x,y);
		s_mask.setTexture(LibJam::Instance().get_texture(1005));
		s_mask.setColor(sf::Color(255,255,255,50));
		setTextureRect(sf::IntRect(0,0,50,100));
		setOrigin(25,90);
		s_mask.setOrigin(25,90);
		setScale(1,1);
	}
	void setMoveTarget(sf::Vector2f pos){
		target_move_pos = pos;
		is_move  = true;
		float dx = pos.x - getPosition().x;
		float dy = pos.y - getPosition().y;
		dist     = sqrt(dx*dx + dy*dy);
		angle    = getAngle(getPosition(), pos); 
		speedX   = speed*dx/dist;
		speedY   = speed*dy/dist;
	}
	void setMoveRand(int min_w, int min_h, int max_w, int max_h){
		sf::Vector2f pos;
		pos.x = getPosition().x + rand()%100 - 50;
		pos.y = getPosition().y + rand()%100 - 50;
		if(pos.x < 0) pos.x = 0;
		if(pos.y < 0) pos.y = 0;
		if(max_w < pos.x) pos.x = max_w;
		if(max_h < pos.y) pos.y = max_h;
		
		setMoveTarget(pos);
	}
	void behavior(float time){
		if(is_move){
			if(dist > speed*time){
				prev_prev_pos = prev_pos;
				prev_pos = getPosition();
				move(speedX*time, speedY*time);
				dist -= speed*time;
			} else {
				setPosition(target_move_pos);
				is_move = false;
			}
	 	}	
	}
	void stop_move(int value){
		is_move = false;
		setPosition(prev_prev_pos);
		move(-value*speedX/speed, -value*speedY/speed);
	}
};

struct Player : Unit {
	Player(){ }
};

struct Bullet {
	float x, y, speed, speedX, speedY, timer;
	sf::Sprite *sprite;
	
	Bullet(sf::Sprite *sprite) : x(0), y(0), speed(300), speedX(0), speedY(0), 
	                            timer(0), sprite(sprite) { }
	
	void start(sf::Vector2f pos, sf::Vector2f target){
		float dx = target.x - pos.x;
		float dy = target.y - pos.y;
		float dist = sqrt(dx*dx + dy*dy); 
		speedX = speed*dx/dist;
		speedY = speed*dy/dist;
		x = pos.x + speedX * 0.01f;
		y = pos.y + speedY * 0.01f;
	}
	
	bool behavior(float time){
		x += speedX*time;
		y += speedY*time;
		timer += time;
		if(timer > 2.0f)
			return true; // нужно удалять пулю
		return false;
	}
	
	void draw(sf::RenderWindow &win){
		sprite->setPosition(x, y);
		win.draw(*sprite);
	}
};

struct Map : Object {
	Map(){
		setTexture(LibJam::Instance().get_texture(2));
		mask   =  &LibJam::Instance().get_image(2);
	}
};

int collision(Object *obj, Object* unit){
	if(!obj || !unit || !obj->mask) return false;
	float dx = unit->getPosition().x - obj->getPosition().x;
	float dy = unit->getPosition().y - obj->getPosition().y;
	sf::Image *mask = obj->mask;
	
	//cout << "unit.pos=" << unit->getPosition().x << ':' << unit->getPosition().y
	//     << " obj.pos=" <<  obj->getPosition().x << ':' <<  obj->getPosition().y
	//     << " dx:dy= "  << dx << ':' << dy 
	//	   << ", size="   << mask->getSize().x << ':' << mask->getSize().y << endl;
		
	if(0 < dx && dx < mask->getSize().x && 0 < dy && dy < mask->getSize().y){
		if(!obj->is_mirror){
			sf::Color pxl = mask->getPixel(dx, dy);
			if(pxl.r == 255 && pxl.a > 0)
	   			return true;
		} else {
			sf::Color pxl = mask->getPixel(mask->getSize().x - dx, dy);
			if(pxl.r == 255 && pxl.a > 0)
	   			return true;
		}	
	}
		
	   		
	return false;
}

// определяет, накладываются ли картинки друг на друга по координатам
bool collision1_obj(Object *obj1, Object *obj2){
	if(!obj1 || !obj2) return false;
	sf::Vector2u size1 = obj1->getTexture()->getSize();
	sf::Vector2u size2 = obj2->getTexture()->getSize();
	sf::Vector2f pos1 = obj1->getPosition();
	sf::Vector2f pos2 = obj2->getPosition();
	pos1.x += size1.x/2.0f;
	pos1.y += size1.y/2.0f;
	pos2.x += size2.x/2.0f;
	pos2.y += size2.y/2.0f;
	
	float dx = abs(pos2.x - pos1.x);
	float dy = abs(pos2.y - pos1.y);
	float minx = size1.x/2.0f + size2.x/2.0f;
	float miny = size1.y/2.0f + size2.y/2.0f;
	
	//cout << "dx:dy=" << dx << ':' << dy;
	
	if( dx < minx && dy < miny) return true;
	return false;
}

// определяет по пикселям
int collision2_obj(Object *obj1, Object *obj2){
	//return 1; // Первый    за вторым
	//return 2; // Первый перед вторым
	sf::Image *mask1 = obj1->mask;
	sf::Image *mask2 = obj2->mask;
	if(!mask1 || !mask2) return 0;
	// Данные для расчётов
	sf::Vector2f pos1 = obj1->getPosition();
	sf::Vector2f pos2 = obj2->getPosition();
	sf::Vector2f start_pos;
	sf::Vector2f end_pos;
	int w1 = mask1->getSize().x;
	int h1 = mask1->getSize().y;
	int w2 = mask2->getSize().x;
	int h2 = mask2->getSize().y;	
	//----------------------------
	float dx = pos2.x - pos1.x;
	float dy = pos2.y - pos1.y;
	//----------------------------
	for(int i=0; i<w1; ++i){
		for(int j=0; j<h1; ++j){
			sf::Color pxl = mask1->getPixel(i,j);
			if(pxl.a == 0) continue;
			if(i-dx <  0) continue;
			if(j-dy <  0) continue;
			if(i-dx >= w2) continue;
			if(j-dy >= h2) continue;
			//cout << "w1:h1="  << w1 << ':' << h1 
			//     << " i:j="   << i  << ':' << j  
			//	 << " dx:dy=" << dx << ':' << dy
			//	 << " i-dx="  << i-dx
			//	 << " j-dy="  << j-dy
			//	 << endl;
			sf::Color pxl2 = mask2->getPixel(i-dx,j-dy);
			if(pxl2.a == 0) continue;
			if(pxl.r == 255 && pxl2.g == 255) return 1; // RA
			if(pxl.g == 255 && pxl2.r == 255) return 2; // AR
		}
	}
	
	return 0; // не соприкасаются вообще
}

void draw_collision(std::vector<Object*> &vec){
	if(vec.size() == 0) return;
	// Проходим по объектам
	for(int i=0; i<vec.size(); ++i){
	for(int j=i; j<vec.size(); ++j){
	if(i == j) continue;
		// если этот и следующий объекты накладываются друг на друга
		if( collision1_obj(vec[i], vec[j]) == true ){
			int res = collision2_obj(vec[i], vec[j]);
			if(res == 2){
				Object *obj = vec[i];
				vec[i] = vec[j];
				vec[j] = obj;
				return;
			}
		}
	}
	}
}

void collision_bullet(std::vector<Bullet*> &bullets,
                      std::vector<Object*> &vec,
					  std::vector<Unit*>   &units)
{
	for(int i=0; i<bullets.size(); ++i){
		Bullet* b = bullets[i];
		if(!b) continue;
		for(int j=0; j<vec.size(); ++j){
			Object* obj = vec[j];
			if(!obj) continue;
			float dx = b->x - obj->getPosition().x;
			float dy = b->y - obj->getPosition().y;
			sf::Image *mask = obj->mask;
			if(!mask) continue;
			if(0 < dx && dx < mask->getSize().x-1 
			&& 0 < dy && dy < mask->getSize().y-1)
			{
				bool flag = false;
				if(!obj->is_mirror){
					sf::Color pxl = mask->getPixel(dx, dy);
					if(pxl.r == 255 && pxl.a > 0)
			   			flag = true;
				} else {
					sf::Color pxl = mask->getPixel(mask->getSize().x - dx, dy);
					if(pxl.r == 255 && pxl.a > 0)
			   			flag = true;
				}	
				if(flag == true){					
					Bullet* buff = bullets[i];
					bullets[i] = bullets[bullets.size()-1];
					bullets[bullets.size()-1] = buff;
					delete buff;
					bullets.pop_back();
					i--;
				}
			}
		}
	}
	// UNITS
	for(int i=0; i<bullets.size(); ++i){
		Bullet* b = bullets[i];
		if(!b) continue;
		for(int j=0; j<units.size(); ++j){
			Object* obj = units[j];
			if(!obj) continue;
			float dx = b->x - obj->getPosition().x + obj->getOrigin().x;
			float dy = b->y - obj->getPosition().y + obj->getOrigin().y;
			sf::Image *mask = obj->mask;
			if(!mask) continue;
			if(0 < dx && dx < mask->getSize().x-1 
			&& 0 < dy && dy < mask->getSize().y-1)
			{
				bool flag = false;
				if(!obj->is_mirror){
					sf::Color pxl = mask->getPixel(dx, dy);
					if( pxl.a > 0)
			   			flag = true;
				} else {
					sf::Color pxl = mask->getPixel(mask->getSize().x - dx, dy);
					if( pxl.a > 0)
			   			flag = true;
				}	
				if(flag == true){					
					Bullet* buff = bullets[i];
					bullets[i] = bullets[bullets.size()-1];
					bullets[bullets.size()-1] = buff;
					delete buff;
					bullets.pop_back();
					i--;
					
					Unit* unit = units[j];
					units[j] = units[units.size()-1];
					units[units.size()-1] = unit;
					delete unit;
					units.pop_back();
					j--;
				}
			}
		}
	}
}

struct GUIElement : public sf::Sprite {
	float offsetX;
	float offsetY;
	
	GUIElement() : offsetX(0), offsetY(0) { }
	GUIElement(sf::Texture *t) : offsetX(0), offsetY(0) { 
		setTexture(*t); 
	}
	virtual bool check1(sf::Vector2f &pos){
		sf::Vector2u size = getTexture()->getSize();
		sf::Vector2f p = getPosition();
		if(p.x < pos.x && pos.x < p.x + size.x*win_scale
		&& p.y < pos.y && pos.y < p.y + size.y*win_scale)
			return true;
		return false;
	}
	virtual void update(){
		setScale(win_scale, win_scale);
		setPosition(view_x+offsetX*view_w, view_y+offsetY*view_h);
	}
	void setOffset(float x, float y){
		offsetX = x;
		offsetY = y;
	}
};

struct Button : GUIElement {
	Button(){}
	Button(sf::Texture *t, int x, int y){ 
		setTexture(*t); 
		setPosition(x,y);
	}
};

int main()
{	
	setlocale(LC_ALL, "Russian");
	
	float timer_update_coallise_cur = 0.0f;
	float timer_update_coallise_max = 0.5f;
	
	// Interface
	bool is_tutorial = false;
	GUIElement sprite_tutorial(LibJam::Instance().get_texture_ptr(11)); 
	sprite_tutorial.setOffset(-0.5f,0.5f);
	sprite_tutorial.setOrigin(0,562);
	
	Button button_tutorial( LibJam::Instance().get_texture_ptr(10), 0, 0);
	button_tutorial.setOffset(-0.48f,0.4f);
	
	Button button_save( LibJam::Instance().get_texture_ptr(12), 0, 0);
	button_save.setOffset(-0.44f,0.4f);
	
	// Map
	Map *map = new Map();
	int map_w = map->getTexture()->getSize().x;
	int map_h = map->getTexture()->getSize().y;
	
	// poll_objects
	bool is_new_obj = false;
	Object* new_obj = new Object(101,0,0,0);
	sf::Vector2u size = new_obj->getTexture()->getSize();
	new_obj->setTextureRect(sf::IntRect(0,0,size.x,size.y));
	new_obj->setOrigin(size.x/2,size.y/2);
	std::vector<int>          textures_ids;
	std::vector<sf::Texture*> textures;
	int id_texture = 0;
	int cur_i = 0;
	for(int i=100; i<1000; ++i){
		sf::Texture *texture = LibJam::Instance().get_texture_ptr(i);
		if(texture){
			textures_ids.push_back(i);
			textures.push_back(texture);
		}
	}
	
	// Objects
	std::vector<Object*> vec;
	Object *cur_obj = 0;
	parser_JamProject parser;
	//parser.debug = true;
	parser.start();
	for(int i=0; i<parser.numbers.size(); ++i){
		if(parser.numbers[i].size() < 3) break;
		int id = parser.numbers[i][0];
		int x  = parser.numbers[i][1];
		int y  = parser.numbers[i][2];
		bool m = parser.numbers[i][3];
		vec.push_back( new Object(id, x, y, m) );
	}
	
	// Units
	std::vector<Unit*> vecUnits;
	for(int i=0; i<100; ++i){
		Unit *unit = new Unit();
		unit->setPosition(rand()%map_w, rand()%map_h);
		vecUnits.push_back(unit);
	}
	
	// Координаты пуль
	std::vector<Bullet*> bullets;
	sf::Sprite bullet( *LibJam::Instance().get_texture_ptr(4) );
	bullet.setOrigin(4,4);
	
	// Player
	Player *player = new Player();
	player->setPosition(1500,1500);
	
	// Cursor
	sf::Sprite cursor_target(LibJam::Instance().get_texture(3));
	sf::Sprite cursor_delete(LibJam::Instance().get_texture(1));
	cursor_target.setOrigin(16,16);
	cursor_delete.setOrigin(16,16);
	bool is_delete = false;
	bool is_target = false;
	
	// Mouse
	sf::Vector2f mouse_pos_win;
	sf::Vector2f mouse_pos;
	sf::Vector2f mouse_prev_pos;
	sf::Vector2f mouse_pos_click;
	bool mouse[5];
	for(int i=0; i<5; i++)
		mouse[i] = false;
		
	// Time
	sf::Clock clock;
	float time = 0.0f;
	// Timers
	float timer_shot_cur = 0.0f;
	float timer_shot_max = 0.5f;
	
	sf::RenderWindow win(sf::VideoMode(1000,700), "Восстание города");
	
	// View
	sf::View view;
	view_w = win_w * win_scale;
	view_h = win_h * win_scale;
	view_x = map->getTexture()->getSize().x / 2.0f;
	view_y = map->getTexture()->getSize().y / 2.0f;
	view.setSize(view_w, view_h);
	view.setCenter(view_x, view_y);
	win.setView(view);
	
	// Mask
	bool is_mask = false;
	
	//------------------------------------------------------------
	while(win.isOpen()){
		time = clock.getElapsedTime().asSeconds(); 
		clock.restart();
		sf::Event event;
		while(win.pollEvent(event)){
			// Закрытие окна
			if(event.type == sf::Event::Closed) win.close();
			// Изменение размера окна
			else if(event.type == sf::Event::Resized){
				win_w = win.getSize().x;
				win_h = win.getSize().y;
				view_w = win_w*win_scale;
				view_h = win_h*win_scale;
				view.setSize(view_w, view_h);
				win.setView(view);
			}
			// Движение мыши
			else if(event.type == sf::Event::MouseMoved){
				mouse_pos_win.x = event.mouseMove.x;
				mouse_pos_win.y = event.mouseMove.y;
				//=====================================
				mouse_prev_pos = mouse_pos;
				mouse_pos.x = view_x - view_w / 2
				            + sf::Mouse::getPosition(win).x * win_scale;
				mouse_pos.y = view_y - view_h / 2
				            + sf::Mouse::getPosition(win).y * win_scale; 
				
				if(cur_obj){
					cur_obj->move(mouse_pos - mouse_prev_pos);
					timer_update_coallise_cur += time;
					if(timer_update_coallise_cur >= timer_update_coallise_max){
						timer_update_coallise_cur -= timer_update_coallise_max;
						draw_collision(vec);
					}
				}
					
				
				if(new_obj) new_obj->setPosition(mouse_pos);
					
				if(mouse[sf::Mouse::Right])
					player->setMoveTarget(mouse_pos);
				
				cursor_delete.setPosition(mouse_pos);
				cursor_target.setPosition(mouse_pos);				
				//cout << "mouse_pos=" << mouse_pos.x 
				//     << ":" << mouse_pos.y 
				//     << "/" << int(mouse_pos.x*scroll/100.0f)
				//     << ":" << int(mouse_pos.y*scroll/100.0f) 
				//     << endl;			             
			} 
			// Нажатие кнопки мыши
			else if(event.type == sf::Event::MouseButtonPressed){ 
				if(event.mouseButton.button == sf::Mouse::Left)
					if(!is_delete)
						for(int i=0; i<vec.size(); ++i)
							if(vec[i]->check1(mouse_pos)){
								cur_obj = vec[i];
								break;
							}		
				mouse[event.mouseButton.button] = true;
			} 
			// Отжатие кнопки мыши
			else if(event.type == sf::Event::MouseButtonReleased) {
				if(button_tutorial.check1(mouse_pos)){
					if(is_tutorial) is_tutorial = false;
				               else is_tutorial = true;
				} else if( button_save.check1(mouse_pos) ){
					std::ofstream out("src/map1_info.txt");
				    if (out.is_open()){
				    	int mirror;
				        for (int i=0; i<vec.size(); i++){
				        	if(vec[i]->is_mirror == true) mirror = 1;
							                         else mirror = 0;
				            out << vec[i]->img_id << " " 
							    << int(vec[i]->getPosition().x) << ' '
								<< int(vec[i]->getPosition().y) << ' '
								<< mirror << ' ' << 0 << '\n';
				        }
				    }
				    out.close();
				} else if(is_delete){
					for(int i=0; i<vec.size(); ++i)
						if(vec[i]->check1(mouse_pos)){
							delete vec[i];
							vec.erase(vec.begin()+ i);
							break;
						}	
				} else {
					if(event.mouseButton.button == sf::Mouse::Right){
						if(cur_obj){
							Object* obj = new Object(*cur_obj);
							vec.push_back(obj);
						} else if(is_new_obj) {
							Object* obj = new Object(new_obj, textures_ids[cur_i]);
							vec.push_back(obj);
						} else {
							player->setMoveTarget(mouse_pos);	
						}	
					}
						
					if(event.mouseButton.button == sf::Mouse::Left){
						
						// Поворот добавляемого объекта
						if(is_new_obj) new_obj->mirror();
						// поворот объекта под курсором
						else if(cur_obj && mouse_pos_click == mouse_pos) 
							cur_obj->mirror();
							
						mouse_pos_click = mouse_pos;	
						cur_obj = 0;
					}		
				}
				mouse[event.mouseButton.button] = false;	
			} 
			else if(event.type == sf::Event::KeyPressed) { } 
			// Отжатие кнопки клавиатуры
			else if(event.type == sf::Event::KeyReleased){         
				if(event.key.code == sf::Keyboard::Tab){
					is_mask = !is_mask;
					if(is_mask){
						for(int i=0; i<vec.size(); i++)
							vec[i]->setColor(sf::Color(255,255,255,50));
					} else {
						for(int i=0; i<vec.size(); i++)
							vec[i]->setColor(sf::Color::White);
					}
				}
					
				if(event.key.code == sf::Keyboard::Num1){
					// Если не включен режим выставл
					if(!is_delete){
						is_delete = true;
						is_new_obj = false;
						cur_obj = 0;
					} else {
						is_delete = false;
					}	
				}
				
				if(event.key.code == sf::Keyboard::Num2){
					if(!is_target){
						is_target = true;
					} else {
						is_target = false;
					}
				}
				
				if(event.key.code == sf::Keyboard::Tilde){
					if(!is_new_obj){
						is_new_obj = true;
						is_delete = false;
						new_obj->setPosition(mouse_pos);
					} else {
						is_new_obj = false;
					}
				}	
				if(event.key.code == sf::Keyboard::Escape){
					is_new_obj = false;
					is_delete = false;
					if(is_tutorial == false){
						is_tutorial = false;
					} else {
						is_tutorial = true;
					}
				}
			} 
			
			// Колёсико мыши
			else if(event.type == 7){
				// Прокрутка выбора объекта
				if(is_new_obj){
					cur_i -= event.mouseWheel.delta;
					if(cur_i < 0) cur_i = textures.size()-1;
					if(textures.size() <= cur_i) cur_i = 0;
					new_obj->setTexture( *textures[cur_i] );
					sf::Vector2u size = new_obj->getTexture()->getSize();
					new_obj->setTextureRect(sf::IntRect(0,0,size.x,size.y));
					new_obj->setOrigin(size.x/2,size.y/2);
				}
				// Масштабирование карты
				else if(!cur_obj){
					win_scale -= 0.03 * event.mouseWheel.delta;
					if(win_scale < 0.5f) win_scale = 0.5f;
					if(win_scale > 5.0f) win_scale = 5.0f;
					view_w = win_w * win_scale;
					view_h = win_h * win_scale;
					view.setSize(view_w, view_h);
					win.setView(view);
				} 
				// Поворот объекта
				else {
					//cur_obj->mirror();
				}
				
			}
		}
		//=====================
		//== U P D A T E ======
		//=====================
		sprite_tutorial.update();
		button_tutorial.update();
		button_save.update();
		cursor_delete.setScale(win_scale,win_scale);
		cursor_target.setScale(win_scale,win_scale);
		if(is_target){
			// UPDATE VIEW
			view_x = player->getPosition().x;
			view_y = player->getPosition().y;
			view.setCenter(view_x, view_y);
			win.setView(view);	
			// UPDATE MOUSE
			mouse_prev_pos = mouse_pos;
			mouse_pos.x = view_x - view_w / 2
				        + sf::Mouse::getPosition(win).x * win_scale;
			mouse_pos.y = view_y - view_h / 2
				        + sf::Mouse::getPosition(win).y * win_scale; 
			cursor_target.setPosition(mouse_pos);	
			// TRA-TA-TA-TA-TA-TA-TA!!!!
			timer_shot_cur += time;
			if(mouse[sf::Mouse::Left]){
				if(timer_shot_cur >= timer_shot_max){
					timer_shot_cur -= timer_shot_max;
					// SHOT
					Bullet *b = new Bullet(&bullet);
					b->start(player->getPosition() + 
					sf::Vector2f(0,-50), mouse_pos);
					bullets.push_back(b);
				}
			} else {
				// Пассивная охлаждение
				if(timer_shot_cur >= timer_shot_max) 
					timer_shot_cur = timer_shot_max;
			}
		}
		
		//=====================
		//== V I E W ==========
		//=====================
		if(mouse_pos_win.x < 25 || win_w-25 < mouse_pos_win.x
		|| mouse_pos_win.y < 25 || win_h-25 < mouse_pos_win.y)
		{
			// LEFT
			if(mouse_pos_win.x < 25){
				view_x -= 200 * time * win_scale;
				if(view_x < view_w/2-10) view_x = view_w/2-10;
			} 
			// TOP
			if(mouse_pos_win.y < 25){
				view_y -= 200 * time * win_scale;
				if(view_y < view_h/2-10) view_y = view_h/2-10;
			} 
			// RIGHT
			if(mouse_pos_win.x > win_w-25){
				view_x += 200 * time * win_scale;
				if(view_x > map_w - view_w/2 + 10)
					view_x = map_w - view_w/2 + 10;
			} 
			// DOWN
			if(mouse_pos_win.y > win_h-25){
				view_y += 200 * time * win_scale;
				if(view_y > map_h - view_h/2 + 10)
					view_y = map_h - view_h/2 + 10;
			} 
			// UPDATE VIEW
			view.setCenter(view_x, view_y);
			win.setView(view);	
		}
		//=====================
		
		
		//=====================
		//== B E H A V I O R ==
		//=====================
		player->behavior(time);
		for(int i=0; i<vecUnits.size(); ++i){
			if(!vecUnits[i]->is_move) 
				vecUnits[i]->setMoveRand(0,0,map_w,map_h);
			vecUnits[i]->behavior(time);
		}
		// bullets
		for(int i=0; i<bullets.size(); ++i){
			if( bullets[i]->behavior(time) ){
				Bullet* b = bullets[bullets.size()-1];
				bullets[bullets.size()-1] = bullets[i];
				bullets[i] = b;
				bullets.pop_back();
			}
		}
		collision_bullet(bullets, vec, vecUnits);
			
		//=====================
		for(int i=0; i<vec.size(); i++)
			if(collision(vec[i], player))
				player->stop_move(3);
				
		for(int i=0; i<vec.size(); i++)
			for(int j=0; j<vecUnits.size(); j++)
				if( collision(vec[i], vecUnits[j]) )
					vecUnits[j]->stop_move(3);
		
		//=====================
		//== D R A W ==========
		//=====================
		win.clear();
		map->draw(win);
		
		for(int i=0; i<vec.size(); i++)
			vec[i]->draw(win);
		
		if(is_new_obj)
			new_obj->draw(win);
		for(int i=0; i<vecUnits.size(); ++i)
			vecUnits[i]->draw(win);	
		if(is_mask){
			for(int i=0; i<vec.size(); i++)
				vec[i]->draw_mask(win);
			for(int i=0; i<vecUnits.size(); ++i)
				vecUnits[i]->draw_mask(win);
			// TODO
			//if(is_new_obj)
			//	new_obj->draw_mask(win);
		}
		player->draw(win);
		// Bullets
		for(int i=0; i<bullets.size(); ++i)
			bullets[i]->draw(win);
		// Cursors
		if(is_delete) win.draw(cursor_delete);
		if(is_target) win.draw(cursor_target);
		// Interface
		if(is_tutorial) win.draw(sprite_tutorial);
		win.draw(button_tutorial);
		win.draw(button_save);
		win.display();
	}
	
	//=============================
	//== S A V E   O B J E C T S ==
	//=============================
	std::ofstream out("src/map1_info.txt");
    if (out.is_open()){
    	int mirror;
        for (int i=0; i<vec.size(); i++){
        	if(vec[i]->is_mirror == true) mirror = 1;
			                         else mirror = 0;
            out << vec[i]->img_id << " " 
			    << int(vec[i]->getPosition().x) << ' '
				<< int(vec[i]->getPosition().y) << ' '
				<< mirror << ' ' << 0 << '\n';
        }
    }
    out.close();
	//=============================
	
	//=============================
	//== C L E A R   M E M O R Y ==
	//=============================
	for(int i=0; i<vec.size(); i++)
		delete vec[i];
	for(int i=0; i<vecUnits.size(); ++i)
		delete vecUnits[i];
	delete player;
	delete map;
	//=============================
	
	return 0;
}
