#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include <iostream>
#include <cmath>

#include "TGrid.h"
#include "TView.h"
#include "TParallelepiped.h"
#include "TCube.h"

#define nullptr           0
#define GAMEMODE_REDACTOR 0
#define GAMEMODE_PLAYER   1

using namespace std;

class TObject3D;

int          m_gamemode = 0; // 0 - redactor // 1 - player 

TObject3D    *v_obj_ctrl       = nullptr; // Объект контроля, вокруг которого бегает камера


////////////////////////////////////////////////////////////
/// Entry point of application
////////////////////////////////////////////////////////////
int main()
{
	//================================================================
	// O B J E C T S
	//================================================================
	TParallelepiped *v_cube        = new TParallelepiped(10, 300, 10);
	TParallelepiped *v_obj_control = new TParallelepiped(100, 50, 10);
	TGrid *v_grid = new TGrid(2000.0f, 100, true, true, true);
	TView *v_view = new TView(180.0f, 100.0f, 0.0f, -100.0f, sf::Vector3f (   0.0f, 0.0f,   0.0f), sf::Vector3f (   0.0f, 0.0f,   0.0f), sf::Vector3f (   0.0f, 0.0f,   0.0f), sf::Vector3f (- 30.0f, 0.0f, 0.0f - 90.0f));
	v_cube->setRotation      ( 30.0f,  0.0f,  0.0f);
	v_cube->setPosition      (100.0f,  0.0f,  0.0f);
	v_cube->setRotateSpeed   ( 90.0f,  0.0f,  0.0f);
	v_obj_control->setSize   ( 25.0f, 25.0f, 25.0f);
	v_obj_control->setOffset (  0.0f,  0.0f, 25.0f);
	v_obj_control->setPosition (100.0f, 100.0f, 100.0f);
	v_obj_ctrl = v_obj_control;
	
	float        m_view_speed_rot  = v_view->getRotationSpeed();
	float        m_view_speed      = v_view->getSpeed();
	float        m_view_angle      =   v_view->getAngle(); 
	float        m_view_dist       = v_view->getDist();
	sf::Vector3f m_view_movement   = v_view->getMovement();
	sf::Vector3f m_view_center_pos = v_view->getCenterPos();
	sf::Vector3f m_view_pos        = v_view->getPosition();
	sf::Vector3f m_view_rot        = v_view->getRotation();
	
	//================================================================
	// SFML
    //================================================================
	sf::RenderWindow App(sf::VideoMode(800, 600, 32), "DSLEngine SFML/OpenGL 3D test");
    sf::Clock clock;
	float v_time = 0.0f;
	sf::Vector2f v_mouse_offset;
	sf::Vector2f v_mouse_prev_pos;
	sf::Vector2f v_mouse_pos;
	bool mouse[5];
	for (int i=0; i<5; ++i)
		mouse[i] = false;
	bool keys[101];
	for (int i=0; i<101; ++i)
		keys[i] = false;
	m_view_movement.x = m_view_speed * cos (m_view_angle / 180.0f * 3.1415f);
	m_view_movement.y = m_view_speed * sin (m_view_angle / 180.0f * 3.1415f);
	//================================================================
	// OpenGL
	//================================================================
	glClearDepth(1.f); //prepare OpenGL surface for HSR
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glMatrixMode(GL_PROJECTION); // Setup a perspective projection & Camera position
    glLoadIdentity();
    gluPerspective (
		90.f,  // fov
		1.f,   // aspect
		1.f,   // zNear
		10000.0f // zFar - ?????????? ?? ??????? ???????? ?????????
	);
	
	
	//================================================================
	while (App.isOpen())
    {
    	v_time = clock.getElapsedTime().asSeconds();
		clock.restart();
        sf::Event v_event;
        while (App.pollEvent(v_event))
        {
            if (v_event.type == sf::Event::Closed)
			{
            	 App.close();
			}
			else if (v_event.type == sf::Event::MouseMoved)
			{
				v_mouse_pos = sf::Vector2f (v_event.mouseMove.x, v_event.mouseMove.y);
				if (mouse[sf::Mouse::Right])
				{
					v_view->rotate (v_mouse_pos - v_mouse_prev_pos);
				}
				v_mouse_prev_pos = v_mouse_pos;
			}
			else if (v_event.type == sf::Event::MouseButtonPressed)
			{
				if (v_event.mouseButton.button == sf::Mouse::Right)
				{
					v_mouse_prev_pos = v_mouse_pos;
				}
				mouse[v_event.mouseButton.button] = true;
			}
			else if (v_event.type == sf::Event::MouseButtonReleased)
			{
				
				mouse[v_event.mouseButton.button] = false;
			}
			else if (v_event.type == sf::Event::MouseWheelMoved)
			{
				m_view_dist += v_event.mouseWheel.delta * 10;
			}
            else if (v_event.type == sf::Event::KeyPressed)
			{
				if (v_event.key.code == sf::Keyboard::Tilde)
				{
					if (m_gamemode == GAMEMODE_REDACTOR)
						m_gamemode = GAMEMODE_PLAYER;
					else 
						m_gamemode = GAMEMODE_REDACTOR;
				}
				
				if (false && m_gamemode == GAMEMODE_REDACTOR)
				{
					 	 if (v_event.key.code == sf::Keyboard::W) m_view_pos.x += m_view_speed * v_time;	
					else if (v_event.key.code == sf::Keyboard::A) m_view_pos.x -= m_view_speed * v_time;
					else if (v_event.key.code == sf::Keyboard::S) m_view_pos.y += m_view_speed * v_time;
					else if (v_event.key.code == sf::Keyboard::D) m_view_pos.y -= m_view_speed * v_time;
					else if (v_event.key.code == sf::Keyboard::Q) m_view_pos.z += m_view_speed * v_time;
					else if (v_event.key.code == sf::Keyboard::E) m_view_pos.z -= m_view_speed * v_time;
				}
				     
				keys[v_event.key.code] = true;
			} 
			else if (v_event.type == sf::Event::KeyReleased)
			{
				//...
				keys[v_event.key.code] = false;
			}
		}
		//view_print (true); // Возможно уже не актуальная инфа о камере вида
        if (keys[sf::Keyboard::W])
		{
			m_view_pos.x += m_view_movement.x * v_time;
			m_view_pos.y += m_view_movement.y * v_time;
			//v_obj_control->move_forward (v_time);
		}
	    if (keys[sf::Keyboard::S])
		{
			m_view_pos.x -= m_view_movement.x * v_time;
			m_view_pos.y -= m_view_movement.y * v_time;
			//v_obj_control->move_back (v_time);
		}
		if (keys[sf::Keyboard::A])
		{
			m_view_angle += m_view_speed_rot * v_time;
			m_view_rot.z -= m_view_speed_rot * v_time;
			m_view_movement.x = m_view_speed * cos (m_view_angle / 180.0f * 3.1415f);
			m_view_movement.y = m_view_speed * sin (m_view_angle / 180.0f * 3.1415f);
			//v_obj_control->rotate_left (v_time);		
		}
		if (keys[sf::Keyboard::D])
		{
			m_view_angle -= m_view_speed_rot * v_time;
			m_view_rot.z += m_view_speed_rot * v_time;
			m_view_movement.x = m_view_speed * cos (m_view_angle / 180.0f * 3.1415f);
			m_view_movement.y = m_view_speed * sin (m_view_angle / 180.0f * 3.1415f);
			//v_obj_control->rotate_right (v_time);
		}
		
		//=====================================================
		// B E H A V I O R    O B J E C T S
		//=====================================================
        v_cube->behavior (v_time);
        v_obj_control->behavior(v_time);
        //=====================================================
        // B E H A V I O R    O B J E C T S
        //=====================================================
        
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Prepare for drawing // Clear color and depth buffer
        glMatrixMode(GL_MODELVIEW); // Apply some transformations for the cube
        glLoadIdentity();
		v_view->update();
	
		glBegin(GL_LINES);	
		
		// Цветной крестик
		glLineWidth (2);
		glColor3d(0,1,0);
		glVertex3d(m_view_pos.x -   0.0f, m_view_pos.y + 100.0f, m_view_pos.z -   0.0f);
		glVertex3d(m_view_pos.x -   0.0f, m_view_pos.y - 100.0f, m_view_pos.z -   0.0f);
		glColor3d(1,0,0);
		glVertex3d(m_view_pos.x + 100.0f, m_view_pos.y -   0.0f, m_view_pos.z -   0.0f);
		glVertex3d(m_view_pos.x - 100.0f, m_view_pos.y -   0.0f, m_view_pos.z -   0.0f);
		glColor3d(0,0,1);
		glVertex3d(m_view_pos.x -   0.0f, m_view_pos.y -   0.0f, m_view_pos.z + 100.0f);
		glVertex3d(m_view_pos.x -   0.0f, m_view_pos.y -   0.0f, m_view_pos.z - 100.0f);		
	
		// Белый крестик
		glRotatef( m_view_rot.x, 1.f, 0.f, 0.f);
		glRotatef( m_view_rot.y, 0.f, 1.f, 0.f);
		glRotatef( m_view_rot.z, 0.f, 0.f, 1.f);
		glLineWidth (5);
		glColor3d(1,1,1);
		glVertex3d(0.0f, -100.0f, 0.0f);
		glVertex3d(0.0f, +100.0f, 0.0f);
		glColor3d(1,1,1);
		glVertex3d(-100.0f, 0.0f, 0.0f);
		glVertex3d( 100.0f, 0.0f, 0.0f);
		glColor3d(1,1,1);
		glVertex3d(0.0f, 0.0f, -100.0f);
		glVertex3d(0.0f, 0.0f, +100.0f);
		glRotatef( -m_view_rot.x, 1.f, 0.f, 0.f);
		glRotatef( -m_view_rot.y, 0.f, 1.f, 0.f);
		glRotatef( -m_view_rot.z, 0.f, 0.f, 1.f);
							
		glEnd();
		if (m_gamemode == GAMEMODE_REDACTOR)
		{
			// Рисуем сетку
			v_grid->draw(m_view_pos);
			//draw_grid(2000.0f, 200, true, true, true);
		}
		//=====================================================
		// D R A W    O B J E C T S  
		//=====================================================
		v_cube->draw(m_view_pos);
		v_obj_control->draw(m_view_pos);
		//=====================================================
		// D R A W    O B J E C T S  
		//=====================================================
        App.display();
    }
    delete v_cube;
    delete v_obj_control;
    delete v_grid;
    delete v_view;
    
    return EXIT_SUCCESS;
}
