// @ansty
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
//#include "stdafx.h"
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

class TObject3D
{
protected:
	sf::Vector3f m_vertex[4];
	sf::Vector3f m_angles; 
	sf::Vector3f m_center; // ���������� ������ �������
	sf::Vector3f m_size;
	sf::Color    m_color;
public:
	// �����������
	TObject3D() 
	: m_angles(0.0f, 0.0f, 0.0f)
	{
		
	}
	
	// �������� �� ����
	void rotate (float p_angle_x , float p_angle_y, float p_angle_z)
	{
		m_angles += sf::Vector3f(p_angle_x, p_angle_y, p_angle_z);
		glRotatef(m_angles.x, 1.f, 0.f, 0.f);
		glRotatef(m_angles.y, 0.f, 1.f, 0.f);
		glRotatef(m_angles.z, 0.f, 0.f, 1.f);
	}
	
	void setSize3D(float p_size_x, float p_size_y, float p_size_z)
	{
		m_size.x = p_size_x;
		m_size.y = p_size_y;
		m_size.z = p_size_z;
	}
	void setColor(sf::Color p_color)
	{
		m_color = p_color;
	}
};

class TCube : public TObject3D
{
	
public:
	TCube()
	{
		setSize3D(10.0f, 10.0f, 100.0f);
		setColor(sf::Color::Red);
	}
	
	void draw()
	{
		//Draw a cube
        glBegin(GL_QUADS);//draw some squares
        
		//glColor3i(0,0,1);
        glVertex3f(-m_size.x/2, -m_size.y/2, -m_size.z/2);
        glVertex3f(-m_size.x/2,  m_size.y/2, -m_size.z/2);
        glVertex3f( m_size.x/2,  m_size.y/2, -m_size.z/2);
        glVertex3f( m_size.x/2, -m_size.y/2, -m_size.z/2);
		
		glColor3f (float (m_color.r) / 255,
		           float (m_color.g) / 255,
				   float (m_color.b) / 255 );
        
		glVertex3f(-m_size.x/2, -m_size.y/2, m_size.z/2);
        glVertex3f(-m_size.x/2,  m_size.y/2, m_size.z/2);
        glVertex3f( m_size.x/2,  m_size.y/2, m_size.z/2);
        glVertex3f( m_size.x/2, -m_size.y/2, m_size.z/2);

		glColor3f (float (m_color.r) / 255,
		           float (m_color.g) / 255,
				   float (m_color.b) / 255 );

		//glColor3f(1,0,1);
        glVertex3f(-m_size.x/2, -m_size.y/2, -m_size.z/2);
        glVertex3f(-m_size.x/2,  m_size.y/2, -m_size.z/2);
        glVertex3f(-m_size.x/2,  m_size.y/2,  m_size.z/2);
        glVertex3f(-m_size.x/2, -m_size.y/2,  m_size.z/2);
glColor3f (float (m_color.r) / 255,
		           float (m_color.g) / 255,
				   float (m_color.b) / 255 );
		//glColor3f(0,1,0);
        glVertex3f(m_size.x/2, -m_size.y/2, -m_size.z/2);
        glVertex3f(m_size.x/2,  m_size.y/2, -m_size.z/2);
        glVertex3f(m_size.x/2,  m_size.y/2,  m_size.z/2);
        glVertex3f(m_size.x/2, -m_size.y/2,  m_size.z/2);
glColor3f (float (m_color.r) / 255,
		           float (m_color.g) / 255,
				   float (m_color.b) / 255 );
		//glColor3f(1,1,0);
        glVertex3f(-m_size.x/2, -m_size.y/2,  m_size.z/2);
        glVertex3f(-m_size.x/2, -m_size.y/2, -m_size.z/2);
        glVertex3f( m_size.x/2, -m_size.y/2, -m_size.z/2);
        glVertex3f( m_size.x/2, -m_size.y/2,  m_size.z/2);
glColor3f (float (m_color.r) / 255,
		           float (m_color.g) / 255,
				   float (m_color.b) / 255 );
		//glColor3f(1,0,0);
        glVertex3f(-m_size.x/2, m_size.y/2,  m_size.z/2);
        glVertex3f(-m_size.x/2, m_size.y/2, -m_size.z/2);
        glVertex3f( m_size.x/2, m_size.y/2, -m_size.z/2);
        glVertex3f( m_size.x/2, m_size.y/2,  m_size.z/2);
        
        glColor3f (float (m_color.r) / 255,
		           float (m_color.g) / 255,
				   float (m_color.b) / 255 );
		//glColor3i(0,0,1);
        
		glEnd();
	}	
};

////////////////////////////////////////////////////////////
/// Entry point of application
////////////////////////////////////////////////////////////
int main()
{
	TCube *v_cube = new TCube();
	
    // Create the main window
    sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML OpenGL");

    // Create a clock for measuring time elapsed
    sf::Clock clock;
	float v_time = 0.0f;
	
    //prepare OpenGL surface for HSR
	glClearDepth(1.f);
    glClearColor(0.3f, 0.3f, 0.3f, 0.f);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    //// Setup a perspective projection & Camera position
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar



    bool rotate=true;
	float angle;
	
	// Start game loop
	while (App.isOpen())
    {
    	v_time = clock.getElapsedTime().asSeconds();
    	
		clock.restart();
    	
        // Process events
        sf::Event Event;
        while (App.pollEvent(Event))
        {
            // Close window : exit
            if (Event.type == sf::Event::Closed)
                App.close();

            // Escape key : exit
            if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
                App.close();

			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::A)){
				rotate=!rotate;
			}
 
		}
        
        //Prepare for drawing
		// Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Apply some transformations for the cube
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.f, 0.f, -200.f);	
		
		v_cube->rotate(50.0f*v_time, 30.0f*v_time, 90.0f*v_time);
		v_cube->draw();

        // Finally, display rendered frame on screen
        App.display();
    }
    
    // ������ ������
    if (v_cube)
		delete v_cube;
	
    return EXIT_SUCCESS;
}
