#ifndef TObject3D_h
#define TObject3D_h

//#include "TObject3D.h"

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
	, m_pos				(0.0f, 0.0f, 0.0f)
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

#endif
