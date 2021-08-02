#ifndef TParallelepiped_h
#define TParallelepiped_h

#include "TObject3D.h"

class TParallelepiped : public TObject3D
{
	float m_vertex[8];
public:
	TParallelepiped(float p_size_x, float p_size_y, float p_size_z)
	{
		setSize (p_size_x, p_size_y, p_size_z);
	}	
	void draw(sf::Vector3f m_view_pos)
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

#endif
