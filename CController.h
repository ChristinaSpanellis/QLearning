#ifndef CCONTROLLER_H
#define CCONTROLLER_H

//------------------------------------------------------------------------
//
//	Name: CController.h
//
//  Author: Mat Buckland 2002
//
//  Desc: Controller class for the 'Smart Sweeper' example 
//
//------------------------------------------------------------------------
#include <vector>
#include <sstream>
#include <string>
#include <fstream>
#include <windows.h>
#include "utils.h"
#include "C2DMatrix.h"
#include "SVector2D.h"
#include "CParams.h"
#include "CCollisionObject.h"
#include "CMinesweeper.h"
#include <algorithm>
//these hold the geometry of the sweepers and the mines
const int	 NumSweeperVerts = 16;
const SPoint sweeper[NumSweeperVerts] = {SPoint(-1, -1),
                                         SPoint(-1, 1),
                                         SPoint(-0.5, 1),
                                         SPoint(-0.5, -1),

                                         SPoint(0.5, -1),
                                         SPoint(1, -1),
                                         SPoint(1, 1),
                                         SPoint(0.5, 1),
                                         
                                         SPoint(-0.5, -0.5),
                                         SPoint(0.5, -0.5),

                                         SPoint(-0.5, 0.5),
                                         SPoint(-0.25, 0.5),
                                         SPoint(-0.25, 1.75),
                                         SPoint(0.25, 1.75),
                                         SPoint(0.25, 0.5),
                                         SPoint(0.5, 0.5)};



const int NumMineVerts = 4;
const SPoint mine[NumMineVerts] = {SPoint(-1, -1),
                                   SPoint(-1, 1),
                                   SPoint(1, 1),
                                   SPoint(1, -1)};

class CController
{

protected:
	int					         m_NumSweepers;

	int					         m_NumMines;
	int					         m_NumSuperMines;
	int					         m_NumRocks;

	//vertex buffer for the sweeper shape's vertices
	vector<SPoint>		   m_SweeperVB;

	//vertex buffer for the mine shape's vertices
	vector<SPoint>		   m_MineVB;

	//stores the average MinesGathered per iteration for use 
	//in graphing.
	vector<double>		   m_vecAvMinesGathered;

	//stores the most MinesGathered per iteration
	vector<double>		   m_vecMostMinesGathered;

	//keep track of number of deaths per iteration
	vector<double>		   m_vecDeaths;


	//pens we use for the stats
	HPEN				m_RedPen;
	HPEN				m_BluePen;
	HPEN				m_GreenPen;
	HPEN				m_OldPen;
	
	//handle to the application window
	HWND				m_hwndMain;

	//toggles the speed at which the simulation runs
	bool				m_bFastRender;
	
	//cycles per iteration
	int					m_iTicks;

	//iteration counter
	int					m_iIterations;

  //window dimensions
  int         cxClient, cyClient;

  //this function plots a graph of the average and best MinesGathered
  //over the course of a run
  void   PlotStats(HDC surface);

public:

	CController(HWND hwndMain);

	virtual ~CController();

	
	template<typename pos_type>
	void WorldTransform(vector<SPoint> &VBuffer, SVector2D<pos_type> vPos)
	{
		//create the world transformation matrix
		C2DMatrix matTransform;
	
		//scale
		matTransform.Scale(CParams::dMineScale, CParams::dMineScale);
	
		//translate
		matTransform.Translate(vPos.x, vPos.y);

		//transform the ships vertices
		matTransform.TransformSPoints(VBuffer);
	}

	void Initialize();
	virtual void Render(HDC surface) = 0;
	virtual bool Update() = 0;
	virtual void InitializeLearningAlgorithm(void) = 0;
	virtual void InitializeSweepers(void) = 0;
	virtual void InitializeMines(void) = 0;
	virtual void InitializeSuperMines(void) = 0;
	virtual void InitializeRocks(void) = 0;
	//accessor methods
	bool		FastRender()const	  {return m_bFastRender;}
	void		FastRender(bool arg){m_bFastRender = arg;}
	void		FastRenderToggle()  {m_bFastRender = !m_bFastRender;}

};


#endif
	
