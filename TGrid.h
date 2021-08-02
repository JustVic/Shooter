#include "TObject3D.h"

class TGrid : public TObject3D
{
private:
	float   p_size_grid; 
    int   	p_count_cell;
	bool 	p_draw_axis_x;
	bool 	p_draw_axis_y;
	bool  	p_draw_axis_z;
	
	int   v_max_i;
	int   v_min_i;
	float v_max_lenght;
	float v_min_lenght;
	float v_cell;
	
public:
	TGrid(float size_grid, int count_cell, bool draw_axis_x, bool draw_axis_y, bool draw_axis_z)
	: p_size_grid	(size_grid)
    , p_count_cell	(count_cell)
	, p_draw_axis_x	(draw_axis_x)
	, p_draw_axis_y	(draw_axis_y)
	, p_draw_axis_z	(draw_axis_z)
	{
		//
	}
	
	void draw(sf::Vector3f m_view_pos)
	{
		v_max_i      =    p_count_cell/2;
		v_min_i      = -1*p_count_cell/2;
		v_max_lenght =    p_size_grid/2;
		v_min_lenght = -1*p_size_grid/2;
		v_cell       = (v_max_lenght - v_min_lenght) / (v_max_i - v_min_i);
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
};
