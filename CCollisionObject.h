#pragma once
class CCollisionObject
{
public:
	enum ObjectType { Mine, SuperMine, Rock };
protected:
	ObjectType m_ObjectType;
	bool m_bDead;
public:
	CCollisionObject(ObjectType objectType);
	virtual ~CCollisionObject(void);
	void setType(ObjectType objectType);
	ObjectType getType();
	void Reset();
	void die();
	bool isDead() const;
};

