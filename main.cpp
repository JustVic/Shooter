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

#define nullptr           0
#define GAMEMODE_REDACTOR 0
#define GAMEMODE_PLAYER   1

using namespace std;

class TObject3D;

int          m_gamemode = 0; // 0 - redactor // 1 - player 

float        m_view_speed_rot  = 180.0f;
float        m_view_speed      = 100.0f;
float        m_view_angle      =   0.0f; 
float        m_view_dist       = -100.0f;
sf::Vector3f m_view_movement   = sf::Vector3f (   0.0f, 0.0f,   0.0f);
sf::Vector3f m_view_center_pos = sf::Vector3f (   0.0f, 0.0f,   0.0f);
sf::Vector3f m_view_pos        = sf::Vector3f (   0.0f, 0.0f,   0.0f);
sf::Vector3f m_view_rot        = sf::Vector3f (- 30.0f, 0.0f, m_view_angle - 90.0f);
TObject3D    *v_obj_ctrl       = nullptr; // Объект контроля, вокруг которого бегает камера

void view_rotate(sf::Vector2f p_offset)
{
	m_view_rot.x += p_offset.y * 0.2f;
//  m_view_rot.y += p_offset.x * m_view_speed_rot; // Повороты камеры вокруг своей оси
	m_view_rot.z += p_offset.x * 0.2f;
}

void view_update()
{
	glTranslatef (0.0f, 0.0f, m_view_dist);
	glRotatef (m_view_rot.x, 1.f, 0.f, 0.f);
	glRotatef (m_view_rot.y, 0.f, 1.f, 0.f);
	glRotatef (m_view_rot.z, 0.f, 0.f, 1.f);	
}

void view_print (bool p_is_print)
{
	if (!p_is_print)
		return;
		
	cout << "m_view_pos=(" 
	     << int(m_view_pos.x) << ":" 
		 << int(m_view_pos.y) << ":" 
		 << int(m_view_pos.z) << ") "
		 << "m_view_rot=(" 
		 << int(m_view_rot.x) << ":" 
		 << int(m_view_rot.y) << ":" 
		 << int(m_view_rot.z) << ")"
		 << endl;	
}

void draw_grid (
	float   p_size_grid     = 2000.0f, 
    int   	p_count_cell    = 100,
	bool 	p_draw_axis_x   = true,
	bool 	p_draw_axis_y   = true,
	bool  	p_draw_axis_z   = true
)
{
	int   v_max_i      =    p_count_cell/2;
	int   v_min_i      = -1*p_count_cell/2;
	float v_max_lenght =    p_size_grid/2;
	float v_min_lenght = -1*p_size_grid/2;
	float v_cell       = (v_max_lenght - v_min_lenght) / (v_max_i - v_min_i);
		
	glBegin(GL_LINES);	
	
	glLineWidth (2);
	if (p_draw_axis_x || p_draw_axis_y) 
	{
		glColor3d(0,1,0);
		glVertex3d(m_view_pos.x + 0.0f, m_view_pos.y + v_min_lenght, m_view_pos.z + 0.0f);
		glVertex3d(m_view_pos.x + 0.0f, m_view_pos.y + v_max_lenght, m_view_pos.z + 0.0f);
	}
	if (p_draw_axis_x || p_draw_axis_z)
	{
		glColor3d(1,0,0);
		glVertex3d(m_view_pos.x + v_min_lenght, m_view_pos.y + 0.0f, m_view_pos.z + 0.0f);
		glVertex3d(m_view_pos.x + v_max_lenght, m_view_pos.y + 0.0f, m_view_pos.z + 0.0f);
	}
	if (p_draw_axis_y || p_draw_axis_z)
	{
		glColor3d(0,0,1);
		glVertex3d(m_view_pos.x + 0.0f, m_view_pos.y + 0.0f, m_view_pos.z + v_min_lenght);
		glVertex3d(m_view_pos.x + 0.0f, m_view_pos.y + 0.0f, m_view_pos.z + v_max_lenght);	
	}	
	
	glColor3d(0,0,0);
	glLineWidth (1);
	
	if (p_draw_axis_x)
	{
		for (int i = v_min_i; i<=v_max_i; ++i)
		{
			glVertex3d (m_view_pos.x +     i*v_cell, m_view_pos.y + v_min_lenght, m_view_pos.z + 0.0f);
			glVertex3d (m_view_pos.x +     i*v_cell, m_view_pos.y + v_max_lenght, m_view_pos.z + 0.0f);
			glVertex3d (m_view_pos.x + v_min_lenght, m_view_pos.y + i*v_cell    , m_view_pos.z + 0.0f);
			glVertex3d (m_view_pos.x + v_max_lenght, m_view_pos.y + i*v_cell    , m_view_pos.z + 0.0f);	
		}
	}
	if (p_draw_axis_y)
	{
		for (int i = v_min_i; i<=v_max_i; ++i)
		{
			glVertex3d(m_view_pos.x + 0.0f, m_view_pos.y + v_min_lenght, m_view_pos.z +     i*v_cell);
			glVertex3d(m_view_pos.x + 0.0f, m_view_pos.y + v_max_lenght, m_view_pos.z +     i*v_cell);
			glVertex3d(m_view_pos.x + 0.0f, m_view_pos.y +     i*v_cell, m_view_pos.z + v_min_lenght);
			glVertex3d(m_view_pos.x + 0.0f, m_view_pos.y +     i*v_cell, m_view_pos.z + v_max_lenght);		
		}
	}
	if (p_draw_axis_z)
	{
		for (int i = v_min_i; i<=v_max_i; ++i)
		{
			glVertex3d(m_view_pos.x + m_view_pos.y + v_min_lenght, 0.0f, m_view_pos.z +     i*v_cell);
			glVertex3d(m_view_pos.x + m_view_pos.y + v_max_lenght, 0.0f, m_view_pos.z +     i*v_cell);
			glVertex3d(m_view_pos.x + m_view_pos.y +     i*v_cell, 0.0f, m_view_pos.z + v_min_lenght);
			glVertex3d(m_view_pos.x + m_view_pos.y +     i*v_cell, 0.0f, m_view_pos.z + v_max_lenght);
		}
	}
	
	glEnd();
}

class TObject3D
{
protected:
	bool           m_is_rot;
	sf::Vector3f   m_pos; // ?????????? ?????? ???????
	sf::Vector3f   m_size;
	sf::Vector3f   m_rotate;
	sf::Vector3f   m_offset; // ???????? ????? ???????????? ?????? ???????
	sf::Vector3f   m_rotate_speed;
public:
	TObject3D() 
	: m_is_rot 			(false)
	, m_pos    			(0.0f, 0.0f, 0.0f)
	, m_size   			(0.0f, 0.0f, 0.0f)
	, m_rotate 			(0.0f, 0.0f, 0.0f)
	, m_offset 			(0.0f, 0.0f, 0.0f)
	, m_rotate_speed 	(0.0f, 0.0f, 0.0f)
	{
		//
	}
	void rotate      (sf::Vector3f p_rotate) { m_rotate += p_rotate; m_is_rot = true; }
	void move        (float p_pos_x,    float p_pos_y,    float p_pos_z)    { m_pos    += sf::Vector3f(p_pos_x,    p_pos_y,    p_pos_z   ); }
	void setPosition (float p_pos_x,    float p_pos_y,    float p_pos_z)    { m_pos     = sf::Vector3f(p_pos_x,    p_pos_y,    p_pos_z   ); }
	void rotate      (float p_angle_x,  float p_angle_y,  float p_angle_z)  { m_rotate += sf::Vector3f(p_angle_x,  p_angle_y,  p_angle_z ); m_is_rot = true; }
	void setRotation (float p_angle_x,  float p_angle_y,  float p_angle_z)  { m_rotate  = sf::Vector3f(p_angle_x,  p_angle_y,  p_angle_z ); m_is_rot = true; }
	void setSize     (float p_size_x,   float p_size_y,   float p_size_z)   { m_size    = sf::Vector3f(p_size_x,   p_size_y,   p_size_z  ); }
	void setOffset   (float p_offset_x, float p_offset_y, float p_offset_z) { m_offset  = sf::Vector3f(p_offset_x, p_offset_y, p_offset_z); }
	// ?????????? ???????? ????????
	void setRotateSpeed (float p_angle_x, float p_angle_y, float p_angle_z) 
	{ 
		m_rotate_speed = sf::Vector3f(p_angle_x, p_angle_y, p_angle_z);
		m_is_rot = true; 
	}
	void move_forward (float p_time)
	{
		/*
		cout << "object3d m_pos(x:y:z) = (" 
		     << int(m_pos.x) << ":" 
			 << int(m_pos.y) << ":" 
			 << int(m_pos.z) << ") "
			 << "m_rotate(x:y:z) = (" 
			 << int(m_rotate.x) << ":" 
			 << int(m_rotate.y) << ":" 
			 << int(m_rotate.z) << ") "
			 << endl;
		*/	 
		move (
			100.0f * p_time * cos (m_rotate.z / 180.0f * 3.1415f), 
		    100.0f * p_time * sin (m_rotate.z / 180.0f * 3.1415f),
			0.0f 
		);
	}
	void move_back (float p_time)
	{
		move (
			-100.0f * p_time * cos (m_rotate.z / 180.0f * 3.1415f), 
		    -100.0f * p_time * sin (m_rotate.z / 180.0f * 3.1415f),
			0.0f 
		);
	}
	void rotate_left (float p_time)
	{
		rotate ( 0.0f, 0.0f,  200.0f * p_time);
	}
	void rotate_right (float p_time)
	{
		rotate ( 0.0f, 0.0f, -200.0f * p_time);
	}
	// ??????? ????????? 3D-???????
	void behavior (float p_time)
	{
		// ???? ?????? ?????? ?????????
		if (m_rotate_speed.x || m_rotate_speed.y || m_rotate_speed.z)	
		{
			// ??????? ??????
			rotate (m_rotate_speed * p_time);
		}
	}
	
	void begin_draw()
	{
		glTranslatef (
			(m_pos.x + m_offset.x), 
            (m_pos.y + m_offset.y), 
			(m_pos.z + m_offset.z)
		);
		if(m_is_rot)
		{
			glRotatef(m_rotate.x, 1.f, 0.f, 0.f);
			glRotatef(m_rotate.y, 0.f, 1.f, 0.f);
			glRotatef(m_rotate.z, 0.f, 0.f, 1.f);
		}
	}
	void end_draw()
	{
		glTranslatef (
			-1*(m_pos.x + m_offset.x),
			-1*(m_pos.y + m_offset.y),
			-1*(m_pos.z + m_offset.z)
		);
		if(m_is_rot)
		{
			glRotatef(-1*m_rotate.x, 1.f, 0.f, 0.f);
			glRotatef(-1*m_rotate.y, 0.f, 1.f, 0.f);
			glRotatef(-1*m_rotate.z, 0.f, 0.f, 1.f);
		}	
	}
	sf::Vector3f* getPositionPtr() { return &m_pos;    }
	sf::Vector3f* getRotationPtr() { return &m_rotate; }
};

class TParallelepiped : public TObject3D
{
	float m_vertex[8];
public:
	TParallelepiped(float p_size_x, float p_size_y, float p_size_z)
	{
		setSize (p_size_x, p_size_y, p_size_z);
	}	
	void draw()
	{
		begin_draw();
		//==================
		glBegin(GL_QUADS);
        if (true)
        {
        	//glBegin (GL_LINES);	
        	//glColor3d (0,0,0); // Black
        	//glLineWidth (1); // ??????? 1
			//glVertex3d(-1*m_size.x, v_min_lenght, 0.0f);
			//glVertex3d(0.0f, v_max_lenght, 0.0f);
        	//glEnd ();
		}
		glColor3i(0,1,1);
		glVertex3f(m_view_pos.x - m_size.x, m_view_pos.y - m_size.y, m_view_pos.z - m_size.z);
        glVertex3f(m_view_pos.x - m_size.x, m_view_pos.y + m_size.y, m_view_pos.z - m_size.z);
        glVertex3f(m_view_pos.x + m_size.x, m_view_pos.y + m_size.y, m_view_pos.z - m_size.z);
        glVertex3f(m_view_pos.x + m_size.x, m_view_pos.y - m_size.y, m_view_pos.z - m_size.z);
		glColor3f(0,0,1);
        glVertex3f(m_view_pos.x - m_size.x, m_view_pos.y - m_size.y, m_view_pos.z + m_size.z);
        glVertex3f(m_view_pos.x - m_size.x, m_view_pos.y + m_size.y, m_view_pos.z + m_size.z);
        glVertex3f(m_view_pos.x + m_size.x, m_view_pos.y + m_size.y, m_view_pos.z + m_size.z);
        glVertex3f(m_view_pos.x + m_size.x, m_view_pos.y - m_size.y, m_view_pos.z + m_size.z);
		glColor3f(1,0,1);
        glVertex3f(m_view_pos.x - m_size.x, m_view_pos.y - m_size.y, m_view_pos.z - m_size.z);
        glVertex3f(m_view_pos.x - m_size.x, m_view_pos.y + m_size.y, m_view_pos.z - m_size.z);
        glVertex3f(m_view_pos.x - m_size.x, m_view_pos.y + m_size.y, m_view_pos.z + m_size.z);
        glVertex3f(m_view_pos.x - m_size.x, m_view_pos.y - m_size.y, m_view_pos.z + m_size.z);
		glColor3f(0,1,0);
        glVertex3f(m_view_pos.x + m_size.x, m_view_pos.y - m_size.y, m_view_pos.z - m_size.z);
        glVertex3f(m_view_pos.x + m_size.x, m_view_pos.y + m_size.y, m_view_pos.z - m_size.z);
        glVertex3f(m_view_pos.x + m_size.x, m_view_pos.y + m_size.y, m_view_pos.z + m_size.z);
        glVertex3f(m_view_pos.x + m_size.x, m_view_pos.y - m_size.y, m_view_pos.z + m_size.z);
		glColor3f(1,1,0);
        glVertex3f(m_view_pos.x - m_size.x, m_view_pos.y - m_size.y, m_view_pos.z + m_size.z);
        glVertex3f(m_view_pos.x - m_size.x, m_view_pos.y - m_size.y, m_view_pos.z - m_size.z);
        glVertex3f(m_view_pos.x + m_size.x, m_view_pos.y - m_size.y, m_view_pos.z - m_size.z);
        glVertex3f(m_view_pos.x + m_size.x, m_view_pos.y - m_size.y, m_view_pos.z + m_size.z);
		glColor3f(1,0,0);
        glVertex3f(m_view_pos.x - m_size.x, m_view_pos.y + m_size.y, m_view_pos.z + m_size.z);
        glVertex3f(m_view_pos.x - m_size.x, m_view_pos.y + m_size.y, m_view_pos.z - m_size.z);
        glVertex3f(m_view_pos.x + m_size.x, m_view_pos.y + m_size.y, m_view_pos.z - m_size.z);
        glVertex3f(m_view_pos.x + m_size.x, m_view_pos.y + m_size.y, m_view_pos.z + m_size.z);
        glEnd();
		//==================
		end_draw();
	}
};

class TCube : public TObject3D
{
public:
	TCube() { }
	// ???????? ?? ????
	void rotate (float p_angle_x , float p_angle_y, float p_angle_z)
	{
		m_rotate += sf::Vector3f(p_angle_x, p_angle_y, p_angle_z);
		glRotatef(m_rotate.x, 1.f, 0.f, 0.f);
		glRotatef(m_rotate.y, 0.f, 1.f, 0.f);
		glRotatef(m_rotate.z, 0.f, 0.f, 1.f);
	}
	void draw()
	{
		//Draw a cube
        glBegin(GL_QUADS);//draw some squares
        
		glColor3i(0,1,1);
		glVertex3f(-1*m_size.x, -1*m_size.y, -1*m_size.z);
        glVertex3f(-1*m_size.x,    m_size.y, -1*m_size.z);
        glVertex3f(   m_size.x,    m_size.y, -1*m_size.z);
        glVertex3f(   m_size.x, -1*m_size.y, -1*m_size.z);

		glColor3f(0,0,1);
        glVertex3f(-1*m_size.x, -1*m_size.y,    m_size.z);
        glVertex3f(-1*m_size.x,    m_size.y,    m_size.z);
        glVertex3f(   m_size.x,    m_size.y,    m_size.z);
        glVertex3f(   m_size.x, -1*m_size.y,    m_size.z);

		glColor3f(1,0,1);
        glVertex3f(-1*m_size.x, -1*m_size.y, -1*m_size.z);
        glVertex3f(-1*m_size.x,    m_size.y, -1*m_size.z);
        glVertex3f(-1*m_size.x,    m_size.y,    m_size.z);
        glVertex3f(-1*m_size.x, -1*m_size.y,    m_size.z);

		glColor3f(0,1,0);
        glVertex3f(   m_size.x, -1*m_size.y, -1*m_size.z);
        glVertex3f(   m_size.x,    m_size.y, -1*m_size.z);
        glVertex3f(   m_size.x,    m_size.y,    m_size.z);
        glVertex3f(   m_size.x, -1*m_size.y,    m_size.z);

		glColor3f(1,1,0);
        glVertex3f(-1*m_size.x, -1*m_size.y,    m_size.z);
        glVertex3f(-1*m_size.x, -1*m_size.y, -1*m_size.z);
        glVertex3f(   m_size.x, -1*m_size.y, -1*m_size.z);
        glVertex3f(   m_size.x, -1*m_size.y,    m_size.z);

		glColor3f(1,0,0);
        glVertex3f(-1*m_size.x,    m_size.y,    m_size.z);
        glVertex3f(-1*m_size.x,    m_size.y, -1*m_size.z);
        glVertex3f(   m_size.x,    m_size.y, -1*m_size.z);
        glVertex3f(   m_size.x,    m_size.y,    m_size.z);
            
        glEnd();
	}	
};
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
	v_cube->setRotation      ( 30.0f,  0.0f,  0.0f);
	v_cube->setPosition      (100.0f,  0.0f,  0.0f);
	v_cube->setRotateSpeed   ( 90.0f,  0.0f,  0.0f);
	v_obj_control->setSize   ( 25.0f, 25.0f, 25.0f);
	v_obj_control->setOffset (  0.0f,  0.0f, 25.0f);
	v_obj_control->setPosition (100.0f, 100.0f, 100.0f);
	v_obj_ctrl = v_obj_control;
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
					view_rotate (v_mouse_pos - v_mouse_prev_pos);
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
		view_update();
	
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
			draw_grid(2000.0f, 200, true, true, true);
		}
		//=====================================================
		// D R A W    O B J E C T S  
		//=====================================================
		v_cube->draw();
		v_obj_control->draw();
		//=====================================================
		// D R A W    O B J E C T S  
		//=====================================================
        App.display();
    }
    delete v_cube;
    delete v_obj_control;
    
    return EXIT_SUCCESS;
}
