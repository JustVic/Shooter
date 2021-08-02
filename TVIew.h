#ifndef TView_h
#define TView_h

//#include "TObject3D.h"

class TView 
{
private:
	float        m_view_speed_rot;
	float        m_view_speed;
	float        m_view_angle; 
	float        m_view_dist;
	sf::Vector3f m_view_movement;
	sf::Vector3f m_view_center_pos;
	sf::Vector3f m_view_pos;
	sf::Vector3f m_view_rot;
	
public:
	TView(float speed_rot, float speed, float angle, float dist, sf::Vector3f movement, sf::Vector3f center_pos, sf::Vector3f pos, sf::Vector3f rot)
	: m_view_speed_rot   	(speed_rot)
	, m_view_speed      	(speed)
	, m_view_angle      	(angle) 
	, m_view_dist       	(dist)
	, m_view_movement		(movement)
	, m_view_center_pos		(center_pos)
	, m_view_pos			(pos)
	, m_view_rot			(rot)
	{
		//
	}
	
	float getRotationSpeed ()  {  return m_view_speed_rot; }
	float getSpeed ()  {  return m_view_speed; }
	float getAngle ()  {  return m_view_angle; }
	float getDist ()  {  return m_view_dist; }
	sf::Vector3f getMovement ()  {  return m_view_movement; }
	sf::Vector3f getCenterPos ()  {  return m_view_center_pos; }
	sf::Vector3f getRotation ()  {  return m_view_rot; }
	sf::Vector3f getPosition ()  {  return m_view_pos; }
	
	void setRotationSpeed (float speed_rot)  {  m_view_speed_rot = speed_rot; }
	void setSpeed (float speed)  {  m_view_speed = speed; }
	void setAngle(float angle)  {  m_view_angle = angle; }
	void setDist (float dist)  {  m_view_dist = dist; }
	void setMovement (sf::Vector3f movement)  {  m_view_movement = movement; }
	void setCenterPos (sf::Vector3f center_pos)  {  m_view_center_pos = center_pos; }
	void setRotation (sf::Vector3f rot)  {  m_view_rot = rot; }
	void setPosition (sf::Vector3f pos)  {  m_view_pos = pos; }
	
	
	void rotate(sf::Vector2f p_offset)
	{
		m_view_rot.x += p_offset.y * 0.2f;
	//  m_view_rot.y += p_offset.x * m_view_speed_rot; // Повороты камеры вокруг своей оси
		m_view_rot.z += p_offset.x * 0.2f;
	}

	void update()
	{
		glTranslatef (0.0f, 0.0f, m_view_dist);
		glRotatef (m_view_rot.x, 1.f, 0.f, 0.f);
		glRotatef (m_view_rot.y, 0.f, 1.f, 0.f);
		glRotatef (m_view_rot.z, 0.f, 0.f, 1.f);	
	}

	void print(bool p_is_print)
	{
		if (!p_is_print)
			return;
		
		std::cout << "m_view_pos=("
	     	<< int(m_view_pos.x) << ":" 
		 	<< int(m_view_pos.y) << ":" 
		 	<< int(m_view_pos.z) << ") "
		 	<< "m_view_rot=(" 
		 	<< int(m_view_rot.x) << ":" 
		 	<< int(m_view_rot.y) << ":" 
		 	<< int(m_view_rot.z) << ")"
		 	<< std::endl;	
	}
};

#endif
