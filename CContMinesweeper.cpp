#include "CContMinesweeper.h"

//-----------------------------------constructor-------------------------
//
//-----------------------------------------------------------------------
CContMinesweeper::CContMinesweeper():
							 CMinesweeper(),
                             m_dRotation(RandFloat()*CParams::dTwoPi),
							 m_dSpeed(MAX_SPEED_IN_PIXELS)
{
	//create a random start position
	m_vPosition = SVector2D<double>((RandFloat() * CParams::WindowWidth), 
					                (RandFloat() * CParams::WindowHeight));
}

//-------------------------------------------Reset()--------------------
//
//	Resets the sweepers position, MinesGathered and rotation
//
//----------------------------------------------------------------------
void CContMinesweeper::Reset()
{

	//reset the sweepers positions
	m_vPosition = SVector2D<double>((RandFloat() * CParams::WindowWidth), 
					                (RandFloat() * CParams::WindowHeight));
	
	CMinesweeper::Reset();

	//and the rotation
	m_dRotation = RandFloat()*CParams::dTwoPi;

	return;
}
//---------------------WorldTransform--------------------------------
//
//	sets up a translation matrix for the sweeper according to its
//  scale, rotation and position. Returns the transformed vertices.
//-------------------------------------------------------------------
void CContMinesweeper::WorldTransform(vector<SPoint> &sweeper)
{
	//create the world transformation matrix
	C2DMatrix matTransform;
	
	//scale
	matTransform.Scale(m_dScale, m_dScale);
	
	//rotate
	matTransform.Rotate(m_dRotation - CParams::dHalfPi);
	
	//and translate
	matTransform.Translate(m_vPosition.x, m_vPosition.y);
	
	//now transform the ships vertices
	matTransform.TransformSPoints(sweeper);
}

//-------------------------------Update()--------------------------------
//
//	First we take sensor readings. These are then fed into the learning algorithm
//
//	The inputs are:
//	
//	A vector to the closest mine (x, y)
//	The sweepers 'look at' vector (x, y)
//	So given a force we calculate the resultant rotation 
//	and acceleration. This is then applied to current velocity vector.
//
//-----------------------------------------------------------------------
bool CContMinesweeper::Update(vector<CContCollisionObject*> &objects)
{
	//update Look At 
	m_vLookAt.x = cos(m_dRotation);
	m_vLookAt.y = sin(m_dRotation);

	//update position
	m_vPosition += (m_vLookAt * m_dSpeed);

	//wrap around window limits
	if (m_vPosition.x > CParams::WindowWidth) m_vPosition.x = 0;
	if (m_vPosition.x < 0) m_vPosition.x = CParams::WindowWidth;
	if (m_vPosition.y > CParams::WindowHeight) m_vPosition.y = 0;
	if (m_vPosition.y < 0) m_vPosition.y = CParams::WindowHeight;
	GetClosestObjects(objects);	
	return true;
}


//----------------------GetClosestObject()---------------------------------
//
//	returns the vector from the sweeper to the closest mine
//
//-----------------------------------------------------------------------
void CContMinesweeper::GetClosestObjects(vector<CContCollisionObject*> &objects)
{
	double			closest_mine_so_far = 99999, closest_rock_so_far = 99999, closest_super_mine_so_far = 99999;

	SVector2D<double>		vClosestObject(0, 0);

	//cycle through mines to find closest
	for (int i=0; i<objects.size(); i++)
	{
		if (objects[i]->isDead()) continue; //skip if object was destroyed earlier
		double len_to_object = Vec2DLength<double>(objects[i]->getPosition() - m_vPosition);

		switch(objects[i]->getType()){
		case CCollisionObject::ObjectType::Mine:
			if (len_to_object < closest_mine_so_far)
			{
				closest_mine_so_far	= len_to_object;
				vClosestObject	= objects[i]->getPosition()-m_vPosition;
				m_iClosestMine = i;
			}
			break;
		case CCollisionObject::ObjectType::Rock:
			if (len_to_object < closest_rock_so_far)
			{
				closest_rock_so_far	= len_to_object;
				vClosestObject	= objects[i]->getPosition()-m_vPosition;
				m_iClosestRock = i;
			}
			break;
		case CCollisionObject::ObjectType::SuperMine:
			if (len_to_object < closest_super_mine_so_far)
			{
				closest_super_mine_so_far = len_to_object;
				vClosestObject	= objects[i]->getPosition()-m_vPosition;
				m_iClosestSupermine = i;
			}
			break;
		}
	}
}
//----------------------------- CheckForMine -----------------------------
//
//  this function checks for collision with its closest mine (calculated
//  earlier and stored in m_iClosestMine)
//-----------------------------------------------------------------------
int CContMinesweeper::CheckForObject(vector<CContCollisionObject*> &objects, double size)
{
	SVector2D<double> DistToObject = m_vPosition - objects[m_iClosestMine]->getPosition();
		
	if (Vec2DLength<double>(DistToObject) < (size + 5))
	{
			return m_iClosestMine;
	}

	DistToObject = m_vPosition - objects[m_iClosestRock]->getPosition();
		
	if (Vec2DLength<double>(DistToObject) < (size + 5))
	{
			return m_iClosestRock;
	}

	DistToObject = m_vPosition - objects[m_iClosestSupermine]->getPosition();
		
	if (Vec2DLength<double>(DistToObject) < (size + 5))
	{
			return m_iClosestSupermine;
	}

  return -1;
}
//-----------------------------------------------------------------------
// Getters and setters for speed
// speed_factor_of_full_throttle should be between 0.0 and 1.0
//-----------------------------------------------------------------------
void CContMinesweeper::setSpeed(double speed_factor_of_full_throttle)
{
	m_dSpeed = speed_factor_of_full_throttle * MAX_SPEED_IN_PIXELS;
}
double CContMinesweeper::getSpeed() const
{
	return m_dSpeed;
}
//-----------------------------------------------------------------------
//Accessor to the current look vector of the sweeper (this is normalized
//already)
//-----------------------------------------------------------------------
SVector2D<double> CContMinesweeper::getLookAt(void) const 
{
	return m_vLookAt;
}
//----------------------------- turn -----------------------------
//
//  Lets the minesweeper turn towards / away from the current direction
//  to pt, at rate_factor * MAX_TURNING_RATE_IN_DEGREES
// 
//-----------------------------------------------------------------------
void CContMinesweeper::turn(SPoint pt, double rate_factor, bool towards)
{
	double aclockRotRads = m_dRotation + (rate_factor*MAX_TURNING_RATE_IN_DEGREES)*CParams::dPi/180;
	double clockRotRads = m_dRotation - (rate_factor*MAX_TURNING_RATE_IN_DEGREES)*CParams::dPi/180;
	SVector2D<double> vLookAC(cos(aclockRotRads),sin(aclockRotRads));
	SVector2D<double> vLookC(cos(clockRotRads),sin(clockRotRads));
	//get the vector to the point from the sweepers current position:
	SVector2D<double> vObj(SVector2D<double>(pt.x,pt.y) - m_vPosition);
	Vec2DNormalize<double>(vObj);
	//remember (MAM1000 / CSC3020) the dot product between two normalized vectors returns
	//1 if the two vectors point in the same direction
	//0 if the two vectors are perpendicular
	//-1 if the two vectors are pointing in opposite directions
	//therefore let's work out which if ACW rotation or CW rotation brings us closer to 1:
	double dot_aclockW = Vec2DDot<double>(vLookAC,vObj);
	double dot_clockW = Vec2DDot<double>(vLookC,vObj);
	if (towards)
		m_dRotation = (abs(1 - dot_aclockW) < abs(1 - dot_clockW)) ? aclockRotRads : clockRotRads;
	else
		m_dRotation = (abs(1 - dot_aclockW) < abs(1 - dot_clockW)) ? clockRotRads : aclockRotRads;
}

