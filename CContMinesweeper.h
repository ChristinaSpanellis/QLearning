#ifndef CContMINESWEEPER_H
#define CContMINESWEEPER_H

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
#include "CContCollisionObject.h"
#include "CMinesweeper.h"
#define MAX_TURNING_RATE_IN_DEGREES 2.0
#define MAX_SPEED_IN_PIXELS 0.5
using namespace std;


class CContMinesweeper:public CMinesweeper
{

private:
	//its position in the world
	SVector2D<double>		m_vPosition;

	//direction sweeper is facing
	SVector2D<double>		m_vLookAt;

	//its rotation (surprise surprise)
	double			m_dRotation;

	double			m_dSpeed;
	//sets the internal closest object variables for the 3 types of objects
	void GetClosestObjects(vector<CContCollisionObject*> &objects);
public:
	
	void setSpeed(double speed);
	double getSpeed() const;
	SVector2D<double> getLookAt(void) const;
	CContMinesweeper();
	
	//updates the information from the sweepers enviroment
	bool			Update(vector<CContCollisionObject*> &objects);

	//used to transform the sweepers vertices prior to rendering
	void			WorldTransform(vector<SPoint> &sweeper);

	//checks to see if the minesweeper has 'collected' a mine
	int       CheckForObject(vector<CContCollisionObject*> &objects, double size);

	void			Reset();
  

	//-------------------accessor functions
	SVector2D<double>	Position()const{return m_vPosition;}
	
	//turs towards/away from the specified point at a specified rate
	void turn(SPoint pt, double rate_factor, bool towards = true);
};


#endif

	
	