
#ifndef DSL_LIB_TEXTURES_H
#define DSL_LIB_TEXTURES_H

#include <SFML/Graphics/Texture.hpp>
#include <vector>
#include "txtparser.h"

class LibJam {
private:
	std::vector<sf::Image*>   veci;
	std::vector<sf::Texture*> vect;
public:
	static LibJam& Instance(){
		static LibJam lib;
		return lib;
	} 	
	sf::Image& get_image(int id){
		if(0 <= id && id < veci.size() && veci[id])
			return *veci[id];
		else
			return *veci[0];
	}
	sf::Texture& get_texture(int id){
		if(0 <= id && id < vect.size() && vect[id])
			return *vect[id];
		else
			return *vect[0];
	}
	sf::Texture* get_texture_ptr(int id){
		if(0 <= id && id < vect.size())
			return vect[id];
		else
			return 0;
	}
private:
	LibJam() { 
		for(int i=0; i<2000; i++) veci.push_back(0);
		for(int i=0; i<2000; i++) vect.push_back(0);
		load(); 
	}
	~LibJam(){
		for(int i=0; i<veci.size(); ++i)
			if(veci[i]) delete veci[i];
		for(int i=0; i<vect.size(); ++i)
			if(vect[i]) delete vect[i];
	}
	void load(){
		add( 0, "src/images/null.png", false);
		add( 1, "src/images/delete.png", false);
		add( 2, "src/images/map1.png", false);
		add( 3, "src/images/target.png", false);
		add( 4, "src/images/bullet.png", false);
		add( 5, "src/images/man.png");
		
		add( 8, "src/images/man1.png", false);
		add( 9, "src/images/man2.png", false);
		add(10, "src/images/t3.png", false);
		add(11, "src/images/t1.png", false);
		add(12, "src/images/Save.png", false);
		//=====================================
		std::vector< std::vector<std::string> > vvs = get_strings_obj();
		for(int i=0; i<vvs.size(); ++i){
			if(vvs[i].size() < 2) continue;
			add( atoi( vvs[i][0].c_str() ), vvs[i][1]);	
			if(vvs[i].size() < 4) continue;
			int dx = atoi( vvs[i][2].c_str() );
			int dy = atoi( vvs[i][3].c_str() );
		}
	}
	void add(int id, std::string filename, bool flag=true){
		sf::Image *img = new sf::Image();
		if(img->loadFromFile(filename)){
			sf::Texture *texture = new sf::Texture();
			texture->loadFromImage(*img);
			if(veci[id]) delete veci[id];
			if(vect[id]) delete vect[id];
			veci[id] = img;
			vect[id] = texture;
		}
		//=================================
		if(!flag) return;
		filename.insert( filename.size() - 4, "_mask" );
		sf::Image *img_mask = new sf::Image();
		if(img_mask->loadFromFile(filename))
		{
			sf::Texture *texture_mask = new sf::Texture();
			texture_mask->loadFromImage(*img_mask);
			if(veci[id+1000]) delete veci[id];
			if(vect[id+1000]) delete vect[id];
			veci[id+1000] = img_mask;
			vect[id+1000] = texture_mask;
		}
	}
};

/*
class LibTextures 
{
public:
	static LibTextures& Instance(){
		static LibTextures lib;
		return lib;
	} 
	sf::Texture& get(int id){
		if(0 <= id && id < vec.size())
			return *vec[id];
		else
			return *vec[0];
	}	
private:
	std::vector<sf::Texture*> vec;
	LibTextures() { load(); }
	~LibTextures(){
		for(int i=0; i<vec.size();++i)
			delete vec[i];
		vec.clear();
	}
	void load(){
		add("src/images/null.png");
		add("src/images/image0.png");
		add("src/images/ice1.png");
	}
	void add(std::string filename){
		sf::Texture* t = new sf::Texture();
		t->loadFromFile(filename);
		vec.push_back(t);
	}
};
*/

class LibTextures2
{
public:
	static LibTextures2& Instance(){
		static LibTextures2 lib;
		return lib;
	} 
	sf::Texture& get(int id) {
		if(0 <= id && id < vec.size() && vec[id])
			return *vec[id];
		else
			return *vec[0];
	}	
private:
	std::vector<sf::Texture*> vec;
	LibTextures2() { 
		for(int i=0; i<2000; i++)
			vec.push_back(0);
		load(); 
	}
	~LibTextures2(){
		for(int i=0; i<vec.size();++i)
			if(vec[i]) delete vec[i];
		vec.clear();
	}
	void load(){
		add( 0, "src/images/null.png");
		add( 1, "src/images/delete.png");
		add( 2, "src/images/map1.png");
		add( 8, "src/images/man1.png");
		add( 9, "src/images/man2.png");
		/*add(10, "src/images/auto1.png");
		add(11, "src/images/auto2.png");
		add(12, "src/images/auto3.png");
		add(13, "src/images/auto4.png");
		add(20, "src/images/house1.png");
		add(21, "src/images/house2.png");
		add(22, "src/images/house3.png");
		add(23, "src/images/house4.png");
		add(30, "src/images/wood1.png");
		add(31, "src/images/wood2.png");
		add(32, "src/images/wood3.png");
		add(33, "src/images/wood4.png");
		add(40, "src/images/obj1.png");*/
		std::vector< std::vector<std::string> > vvs = get_strings_obj();
		for(int i=0; i<vvs.size(); ++i){
			if(vvs[i].size() < 2) continue;
			add( atoi( vvs[i][0].c_str() ), vvs[i][1]);	
			if(vvs[i].size() < 4) continue;
			int dx = atoi( vvs[i][2].c_str() );
			int dy = atoi( vvs[i][3].c_str() );
		}
	}
	void add(int id, std::string filename){
		cout << "add(" << id << ',' << filename << endl;
		sf::Texture* t = new sf::Texture();
		t->loadFromFile(filename);
		if(vec[id]) delete vec[id];
		vec[id] = t;
	}
};

class LibImages
{
public:
	static LibImages& Instance(){
		static LibImages lib;
		return lib;
	} 
	sf::Image& get(int id) {
		if(0 <= id && id < veci.size() && veci[id])
			return *veci[id];
		else
			return *veci[0];
	}
	sf::Texture& get_texture(int id) {
		if(0 <= id && id < vect.size() && vect[id])
			return *vect[id];
		else
			return *vect[0];
	}	
private:
	std::vector<sf::Image*>   veci;
	std::vector<sf::Texture*> vect;
	LibImages() { 
		for(int i=0; i<1000; i++) veci.push_back(0);
		for(int i=0; i<1000; i++) vect.push_back(0);
		load(); 
	}
	~LibImages(){
		for(int i=0; i<veci.size(); ++i)
			if(veci[i]) delete veci[i];
		for(int i=0; i<vect.size(); ++i)
			if(vect[i]) delete vect[i];
	}
	void load(){
		add( 0, "src/images/null.png");
		add( 1, "src/images/circle.png");
		add( 2, "src/images/map1.png");
		add( 8, "src/images/man1.png");
		add( 9, "src/images/man2.png");
		
		add(110, "src/images/auto1.png");
		add(110, "src/images/auto2.png");
		add(120, "src/images/auto3.png");
		add(130, "src/images/auto4.png");
		
		add(120, "src/images/house1.png");
		add(121, "src/images/house2.png");
		add(122, "src/images/house3.png");
		add(123, "src/images/house4.png");
		
		add(130, "src/images/wood1.png");
		add(131, "src/images/wood2.png");
		add(32, "src/images/wood3.png");
		add(33, "src/images/wood4.png");
		
		add(140, "src/images/obj1.png");
	}
	void add(int id, std::string filename){
		sf::Image *img = new sf::Image();
		if(img->loadFromFile(filename)){
			sf::Texture *texture = new sf::Texture();
			texture->loadFromImage(*img);
			if(veci[id]) delete veci[id];
			if(vect[id]) delete vect[id];
			veci[id] = img;
			vect[id] = texture;
		}	
	}
};

class LibImageMasks
{
public:
	static LibImageMasks& Instance(){
		static LibImageMasks lib;
		return lib;
	} 
	sf::Image& get(int id) {
		if(0 <= id && id < veci.size() && veci[id])
			return *veci[id];
		else
			return *veci[0];
	}
	sf::Texture& get_texture(int id) {
		if(0 <= id && id < vect.size() && vect[id])
			return *vect[id];
		else
			return *vect[0];
	}	
private:
	std::vector<sf::Image*>   veci;
	std::vector<sf::Texture*> vect;
	
	LibImageMasks() { 
		for(int i=0; i<1000; i++)
			veci.push_back(0);
		for(int i=0; i<1000; i++)
			vect.push_back(0);
		load(); 
	}
	~LibImageMasks(){
		for(int i=0; i<veci.size(); ++i)
			if(veci[i]) delete veci[i];
		for(int i=0; i<vect.size(); ++i)
			if(vect[i]) delete vect[i];
	}
	void load(){
		add( 0, "src/images/null.png");
		add( 1, "src/images/circle_mask.png");
		add( 2, "src/images/map1_mask.png");
		add( 8, "src/images/man1_mask.png");
		add( 9, "src/images/man2_mask.png");
		
		add(10, "src/images/auto1_mask.png");
		add(11, "src/images/auto2_mask.png");
		add(12, "src/images/auto3_mask.png");
		add(13, "src/images/auto4_mask.png");
		
		add(20, "src/images/house1_mask.png");
		add(21, "src/images/house2_mask.png");
		add(22, "src/images/house3_mask.png");
		add(23, "src/images/house4_mask.png");
		
		add(30, "src/images/wood1_mask.png");
		add(31, "src/images/wood2_mask.png");
		add(32, "src/images/wood3_mask.png");
		add(33, "src/images/wood4_mask.png");
		
		add(40, "src/images/obj1_mask.png");
	}
	void add(int id, std::string filename){
		sf::Image *img = new sf::Image();
		if(img->loadFromFile(filename)){
			sf::Texture *texture = new sf::Texture();
			texture->loadFromImage(*img);
			if(veci[id]) delete veci[id];
			if(vect[id]) delete vect[id];
			veci[id] = img;
			vect[id] = texture;
		}
	}
};

#endif



