#ifndef CDiscCOLLISIONOBJECT_H
#define CDiscCOLLISIONOBJECT_H
#include "SVector2D.h"
#include "CCollisionObject.h"
class CDiscCollisionObject: public CCollisionObject
{
public:
	CDiscCollisionObject();
	CDiscCollisionObject(ObjectType objectType, SVector2D<int> position);
	virtual ~CDiscCollisionObject();

	void setPosition(SVector2D<int> position);
	SVector2D<int> getPosition() const;
	
private:
	SVector2D<int> * m_vPosition;
};

#endif