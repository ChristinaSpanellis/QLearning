#include "CController.h"

//---------------------------------------constructor---------------------
//
//	initilaize the sweepers and their brains
//
//-----------------------------------------------------------------------
void CController::Initialize()
{
	//let's create the mine sweepers
	InitializeSweepers();

	//initialize mines in random positions within the application window
	InitializeMines();
	InitializeSuperMines();
	InitializeRocks();

	//initialize the learning algoritm in child classes
	InitializeLearningAlgorithm();
}

CController::CController(HWND hwndMain): m_NumSweepers(CParams::iNumSweepers),
										                     m_bFastRender(false),
										                     m_iTicks(0),
										                     m_NumMines(CParams::iNumMines),
															 m_NumSuperMines(CParams::iNumSuperMines),
															 m_NumRocks(CParams::iNumRocks),
										                     m_hwndMain(hwndMain),
										                     m_iIterations(0),
                                         cxClient(CParams::WindowWidth),
                                         cyClient(CParams::WindowHeight)
{
	//create a pen for the graph drawing
	m_BluePen  = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	m_RedPen   = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_GreenPen = CreatePen(PS_SOLID, 1, RGB(0, 150, 0));

	m_OldPen	= NULL;

	//fill the vertex buffers
	for (int i=0; i<NumSweeperVerts; ++i)
	{
		m_SweeperVB.push_back(sweeper[i]);
	}

	for (int i=0; i<NumMineVerts; ++i)
	{
		m_MineVB.push_back(mine[i]);
	}

}


//--------------------------------------destructor-------------------------------------
//
//--------------------------------------------------------------------------------------
CController::~CController()
{
	DeleteObject(m_BluePen);
	DeleteObject(m_RedPen);
	DeleteObject(m_GreenPen);
	DeleteObject(m_OldPen);
}

//--------------------------PlotStats-------------------------------------
//
//  Given a surface to draw on this function displays stats and a crude
//  graph showing best and average MinesGathered
//------------------------------------------------------------------------
void CController::PlotStats(HDC surface)
{
	
	string s = "Most MinesGathered last round:       " + ftos((m_vecMostMinesGathered.size() > 0) ? m_vecMostMinesGathered[m_vecMostMinesGathered.size()-1] : 0.0);
	TextOut(surface, 5, 20, s.c_str(), s.size());

    s = "Average MinesGathered last round: " + ftos((m_vecAvMinesGathered.size() > 0) ? m_vecAvMinesGathered[m_vecAvMinesGathered.size()-1] : 0.0);
	TextOut(surface, 5, 40, s.c_str(), s.size());
    
	s = "Number of deaths last round: " + ftos((m_vecDeaths.size() > 0) ? m_vecDeaths[m_vecDeaths.size()-1] : 0.0);
	TextOut(surface, 5, 60, s.c_str(), s.size());
    //render the graph
    float HSlice = (float)cxClient/(m_iIterations+1);
	float VSlice = (float)cyClient/((1)*2);

    //plot the graph for the best MinesGathered
    float x = 0;
    
    m_OldPen = (HPEN)SelectObject(surface, m_RedPen);

    MoveToEx(surface, 0, cyClient, NULL);
    
    for (int i=0; i<m_vecMostMinesGathered.size(); ++i)
    {
       LineTo(surface, x, cyClient - VSlice*m_vecMostMinesGathered[i]);

       x += HSlice;
    }

    //plot the graph for the average MinesGathered
    x = 0;

    SelectObject(surface, m_BluePen);

    MoveToEx(surface, 0, cyClient, NULL);
    
    for (int i=0; i<m_vecAvMinesGathered.size(); ++i)
    {
       LineTo(surface, (int)x, (int)(cyClient - VSlice*m_vecAvMinesGathered[i]));

       x += HSlice;
    }

    //replace the old pen
    SelectObject(surface, m_OldPen);
}

