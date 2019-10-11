#pragma once
#include "cdisccontroller.h"
#include "CParams.h"
#include "CDiscCollisionObject.h"
#include <cmath>
#include <vector>
#include <fstream>
#include <ctime>
#include <cstdlib>

typedef unsigned int uint;
//enum ROTATION_DIRECTION {NORTH=1, SOUTH=3, EAST=0, WEST=2};
enum ACTIONS {UP = 1, RIGHT = 0, DOWN = 3, LEFT = 2};
class CQLearningController :
	public CDiscController
{
private:
	uint _grid_size_x;
	uint _grid_size_y;
	double **** QTables; // Q tables
	double **** QTableCounter; // counts the number of times each state action pair was visited
	double epsilon = 0.9;
	double discountFactor = 0.8;
public:
	CQLearningController(HWND hwndMain);
	virtual void InitializeLearningAlgorithm(void);
	double R(uint x, uint y, uint sweeper_no);
	virtual bool Update(void);
	virtual ~CQLearningController(void);
	double getMaxQValue(int x, int y, int sw);
	
};

