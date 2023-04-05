#include "Solid.hpp"
#include <OficinaFramework/ScreenSystem.hpp>


Solid::Solid(vec2 position, vec2 size, SolidType type, bool* showstuff)
{
	SetName("Solid");
	m_position = position;
	m_size = size;
	this->type = type;
	m_showstuff = showstuff;
}


Solid::~Solid()
{
}

void Solid::Initialize()
{
}

void Solid::Update()
{
}

void Solid::LoadContent()
{
}

void Solid::UnloadContent()
{
}

void Solid::Draw()
{
	// glPushMatrix();
	// glTranslatef(m_position.x, m_position.y, 0.0f);

	// OficinaFramework::RenderingSystem::glColorM(GREY90, 0.7f);
	// switch (type)
	// {
	// case SolidType::RECT:
	// 	glBegin(GL_QUADS);
	// 		glVertex2f(0.0f, 0.0f);
	// 		glVertex2f(m_size.x, 0.0f);
	// 		glVertex2f(m_size.x, m_size.y);
	// 		glVertex2f(0.0f, m_size.y);
	// 	glEnd();
	// 	break;
	// case SolidType::SLOPE_L:
	// 	glBegin(GL_TRIANGLES);
	// 		glVertex2f(0.0f, 0.0f);
	// 		glVertex2f(m_size.x, m_size.y);
	// 		glVertex2f(0.0f, m_size.y);
	// 	glEnd();
	// 	break;
	// case SolidType::SLOPE_R:
	// 	glBegin(GL_TRIANGLES);
	// 		glVertex2f(m_size.x, 0.0f);
	// 		glVertex2f(m_size.x, m_size.y);
	// 		glVertex2f(0.0f, m_size.y);
	// 	glEnd();
	// 	break;
	// }
	// glPopMatrix();

	if ((m_showstuff != nullptr) && (*m_showstuff) == true)
	{
		//AABBs
		if (!GetProperty(1))
		{
			OficinaFramework::RenderingSystem::glColorM(YELLOW);
			glBegin(GL_LINE_LOOP);
			glVertex2f(Left(), Top(Left()));
			glVertex2f(Right(), Top(Right()));
			glVertex2f(Right(), Bottom());
			glVertex2f(Left(), Bottom());
			glEnd();
		}
		else
		{
			OficinaFramework::RenderingSystem::glColorM(MAGENTA);
			glBegin(GL_LINES);
			glVertex2f(Left(), Top(Left()));
			glVertex2f(Right(), Top(Right()));
			glEnd();
			OficinaFramework::RenderingSystem::glColorM(GREEN);
			glBegin(GL_LINE_STRIP);
			glVertex2f(Right(), Top(Right()));
			glVertex2f(Right(), Bottom());
			glVertex2f(Left(), Bottom());
			glVertex2f(Left(), Top(Left()));
			glEnd();
		}
	}
}

float Solid::angularCoefficient(float XPosition) {
	if (type == SolidType::RECT) return 0.0f;
	else return (m_size.y / m_size.x) * (type == SolidType::SLOPE_L ? 1.0f : -1.0f);
}

float Solid::Top(float XPosition) {
	if (XPosition < Left()) return Top(Left());
	if (XPosition > Right()) return Top(Right());

	if (type == SolidType::RECT) return m_position.y;
	else if(type == SolidType::SLOPE_L)
		return m_position.y + (angularCoefficient(XPosition) * (XPosition - m_position.x));
	else return (m_position.y + m_size.y) + (angularCoefficient(XPosition) * (XPosition - m_position.x));
}

float Solid::Left()
{
	return m_position.x;
}

float Solid::Right()
{
	return m_position.x + m_size.x;
}

float Solid::Bottom()
{
	return m_position.y + m_size.y;
}

vec2 Solid::Middle()
{
	return vec2(m_position.x + (m_size.x / 2.0f),
		m_position.y + (m_size.y / 2.0f));
}

int Solid::getType() {
	return (int)type;
}

bool Solid::Intersects(vec2 Position, vec2 size)
{
	float boxRight = Position.x + size.x,
		boxLeft = Position.x,
		boxTop = Position.y,
		boxBottom = Position.y + size.y;
	if (boxRight < Left() || boxLeft > Right() ||
		boxTop > Bottom() || boxBottom < m_position.y)
		return false;
	return true;
}

bool Solid::Linecast(vec2 Position, vec2 direction, float size)
{
	vec2 end(Position + (direction * size));

	float lineLeft = fmin(Position.x, end.x),
		lineRight = fmax(Position.x, end.x),
		lineTop = fmin(Position.y, end.y),
		lineBottom = fmax(Position.y, end.y);

	// In case of no intersection between the AABBs, then
	// return false
	if (lineRight < Left() || lineLeft > Right() ||
		lineTop > Bottom() || lineBottom < m_position.y)
		return false;

	// If inside, however...
	if (Position.x >= m_position.x &&
	Position.x <= Right() &&
	Position.y >= Top(Position.x) &&
	Position.y <= Bottom())
		return true;

	vec2 nearestPoint = Position;
	clamp(nearestPoint.x, Left(), Right());
	clamp(nearestPoint.y, Top(Position.x), Bottom());

	vec2 nearestPointDirection = nearestPoint - Position;
	nearestPointDirection.x /= abs(nearestPointDirection.x);
	nearestPointDirection.y /= abs(nearestPointDirection.y);

	if (nearestPointDirection != direction) return false;

	return (vec2::squareDistance(Position, end) >= vec2::squareDistance(Position, nearestPoint));
}
