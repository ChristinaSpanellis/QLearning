#include "CDiscCollisionObject.h"
#include "SVector2D.h"
CDiscCollisionObject::CDiscCollisionObject(): CCollisionObject(ObjectType::Mine)
{
	m_vPosition = new SVector2D<int>(0,0);
}
CDiscCollisionObject::CDiscCollisionObject(ObjectType objectType, SVector2D<int> position):CCollisionObject(objectType)
{
	m_vPosition = new SVector2D<int>(position.x,position.y);
}
CDiscCollisionObject::~CDiscCollisionObject()
{
	delete m_vPosition;
}

void CDiscCollisionObject::setPosition(SVector2D<int> position)
{
	m_vPosition->x = position.x;
	m_vPosition->y = position.y;	
}
SVector2D<int> CDiscCollisionObject::getPosition() const
{
	return *m_vPosition;
}
