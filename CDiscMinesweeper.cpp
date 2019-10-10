#include "CDiscMinesweeper.h"

//-----------------------------------constructor-------------------------
//
//-----------------------------------------------------------------------
CDiscMinesweeper::CDiscMinesweeper():
							 CMinesweeper(),
                             m_dRotation((ROTATION_DIRECTION)RandInt(0,3))
{
	//create a random start position
	
	m_vPosition = SVector2D<int>(RandInt(0,CParams::WindowWidth/CParams::iGridCellDim)*CParams::iGridCellDim, 
					             RandInt(0,CParams::WindowHeight/CParams::iGridCellDim)*CParams::iGridCellDim);
}

//-------------------------------------------Reset()--------------------
//
//	Resets the sweepers position, MinesGathered and rotation
//
//----------------------------------------------------------------------
void CDiscMinesweeper::Reset()
{

	//reset the sweepers positions
	m_vPosition = SVector2D<int>(RandInt(0,CParams::WindowWidth/CParams::iGridCellDim)*CParams::iGridCellDim, 
					             RandInt(0,CParams::WindowHeight/CParams::iGridCellDim)*CParams::iGridCellDim);
	
	CMinesweeper::Reset();

	//and the rotation
	m_dRotation = (ROTATION_DIRECTION)RandInt(0,3);
	//m_dRotation = ROTATION_DIRECTION::SOUTH;
	return;
}

//---------------------WorldTransform--------------------------------
//
//	sets up a translation matrix for the sweeper according to its
//  scale, rotation and position. Returns the transformed vertices.
//-------------------------------------------------------------------
void CDiscMinesweeper::WorldTransform(vector<SPoint> &sweeper)
{
	//create the world transformation matrix
	C2DMatrix matTransform;
	
	//scale
	matTransform.Scale(m_dScale, m_dScale);
	
	//rotate
	matTransform.Rotate((m_dRotation-1) * CParams::dHalfPi);
	
	//and translate
	matTransform.Translate(m_vPosition.x, m_vPosition.y);
	
	//now transform the ships vertices
	matTransform.TransformSPoints(sweeper);
}
#include <assert.h>
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
bool CDiscMinesweeper::Update(vector<CDiscCollisionObject*> &objects)
{
	GetClosestObjects(objects);
	//update Look At 
	m_vLookAt.x = cos(m_dRotation * CParams::dHalfPi);
	m_vLookAt.y = sin(m_dRotation * CParams::dHalfPi);

	//update position
	m_vPrevPosition = m_vPosition;
	m_vPosition += (m_vLookAt * CParams::iGridCellDim);

	//wrap around window limits
	if (m_vPosition.x >= CParams::WindowWidth) m_vPosition.x = 0;
	if (m_vPosition.x < 0) m_vPosition.x = CParams::WindowWidth - CParams::iGridCellDim;
	if (m_vPosition.y >= CParams::WindowHeight) m_vPosition.y = 0;
	if (m_vPosition.y < 0) m_vPosition.y = CParams::WindowHeight - CParams::iGridCellDim;
	return true;
}


//----------------------GetClosestObject()---------------------------------
//
//	returns the vector from the sweeper to the closest mine
//
//-----------------------------------------------------------------------
void CDiscMinesweeper::GetClosestObjects(vector<CDiscCollisionObject*> &objects)
{
	double			closest_mine_so_far = 99999, closest_rock_so_far = 99999, closest_super_mine_so_far = 99999;

	SVector2D<int>		vClosestObject(0, 0);

	//cycle through mines to find closest
	for (int i=0; i<objects.size(); i++)
	{
		if (objects[i]->isDead()) continue; //skip if object was destroyed earlier
		double len_to_object = Vec2DLength<int>(objects[i]->getPosition() - m_vPosition);

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
//----------------------------- CheckForObject -----------------------------
//
//  this function checks for collision with its closest mine (calculated
//  earlier and stored in m_iClosestMine)
//-----------------------------------------------------------------------
int CDiscMinesweeper::CheckForObject(vector<CDiscCollisionObject*> &objects, int size)
{
	SVector2D<int> DistToObject = m_vPosition - objects[m_iClosestMine]->getPosition();
		
	if (Vec2DLength<int>(DistToObject) < size+5)
	{
			return m_iClosestMine;
	}

	// commented out so that setting number of rocks to 0 does not cause error
	/*DistToObject = m_vPosition - objects[m_iClosestRock]->getPosition();
		
	if (Vec2DLength<int>(DistToObject) < size+5)
	{
			return m_iClosestRock;
	}*/

	DistToObject = m_vPosition - objects[m_iClosestSupermine]->getPosition();
		
	if (Vec2DLength<int>(DistToObject) < size+5)
	{
			return m_iClosestSupermine;
	}

  return -1;
}
//-----------------------------------------------------------------------
// Getters and setters for rotation force and speed
//-----------------------------------------------------------------------
void CDiscMinesweeper::setRotation(ROTATION_DIRECTION dir)
{
	m_dRotation = dir;
}
ROTATION_DIRECTION CDiscMinesweeper::getRotation()
{
	return m_dRotation;
}

