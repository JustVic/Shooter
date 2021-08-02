#ifndef TCube_h
#define TCube_h

#include "TObject3D.h"

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
	void draw(sf::Vector3f m_view_pos)
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

#endif
