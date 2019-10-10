#ifndef CDiscMINESWEEPER_H
#define CDiscMINESWEEPER_H

//------------------------------------------------------------------------
//
//	Name: CMineSweeper.h
//
//  Author: Mat Buckland 2002
//
//  Desc: Class to create a minesweeper object 
//
//------------------------------------------------------------------------
#include <vector>
#include <math.h>
#include "utils.h"
#include "C2DMatrix.h"
#include "SVector2D.h"
#include "CParams.h"
#include "CDiscCollisionObject.h"
#include "CMinesweeper.h"
using namespace std;

enum ROTATION_DIRECTION {NORTH=1, SOUTH=3, EAST=0, WEST=2};
class CDiscMinesweeper:public CMinesweeper
{

private:
	//its position in the world
	SVector2D<int>		m_vPosition;
	SVector2D<int>		m_vPrevPosition;
	//direction sweeper is facing
	SVector2D<int>		m_vLookAt;

	//its rotation (surprise surprise)
	ROTATION_DIRECTION	m_dRotation;
	
	//sets the internal closest object variables for the 3 types of objects
	void GetClosestObjects(vector<CDiscCollisionObject*> &objects);
public:

	void setRotation(ROTATION_DIRECTION rotForce);
	ROTATION_DIRECTION getRotation();

	CDiscMinesweeper();
	
	//updates the information from the sweepers enviroment
	bool			Update(vector<CDiscCollisionObject*> &objects);

	//used to transform the sweepers vertices prior to rendering
	void			WorldTransform(vector<SPoint> &sweeper);

	//checks to see if the minesweeper has 'collected' a mine
	int       CheckForObject(vector<CDiscCollisionObject*> &objects, int size);

	void			Reset();
  

	//-------------------accessor functions
	SVector2D<int>	Position()const{return SVector2D<int>(m_vPosition.x,m_vPosition.y);}
	SVector2D<int>	PrevPosition()const{return SVector2D<int>(m_vPrevPosition.x,m_vPrevPosition.y);}
  
};


#endif

	
	