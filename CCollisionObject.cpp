#include "CCollisionObject.h"


CCollisionObject::CCollisionObject(ObjectType objectType = ObjectType::Mine) : 
									m_ObjectType(objectType),
									m_bDead(false)
{
}


CCollisionObject::~CCollisionObject(void)
{
}

void CCollisionObject::setType(ObjectType objectType)
{
	m_ObjectType = objectType;
}

CCollisionObject::ObjectType CCollisionObject::getType()
{
	return m_ObjectType;
}
void CCollisionObject::Reset()
{
	m_bDead = false;
}
void CCollisionObject::die(){
	this->m_bDead = true;
}
bool CCollisionObject::isDead() const{
	return m_bDead;
}