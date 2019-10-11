#include "CDiscController.h"


CDiscController::CDiscController(HWND hwndMain):
	CController(hwndMain)
{
	assert(CParams::WindowHeight % CParams::iGridCellDim == 0);
	assert(CParams::WindowWidth % CParams::iGridCellDim == 0);
}

CDiscController::~CDiscController(void)
{
	for (auto i = m_vecSweepers.begin(); i != m_vecSweepers.end(); ++i)
		delete *i;
	for (auto i = m_vecObjects.begin(); i != m_vecObjects.end(); ++i)
		delete *i;
}

void CDiscController::InitializeLearningAlgorithm(void)
{
	/**
	This is the basic discrete environment so there are no learning algorithms
	to initialize yet...
	*/
	//CController::InitializeLearningAlgorithm();
	
}

void CDiscController::InitializeSweepers(void)
{
	for (int i=0; i<m_NumSweepers; ++i)
	{
		m_vecSweepers.push_back(new CDiscMinesweeper());
	}
}

void CDiscController::InitializeMines(void)
{
	for (int i=0; i<m_NumMines; ++i)
	{
		while (true)
		{
			SVector2D<int> newPos(RandInt(0, CParams::WindowWidth / CParams::iGridCellDim)*CParams::iGridCellDim,
				RandInt(0, CParams::WindowHeight / CParams::iGridCellDim)*CParams::iGridCellDim);

			if (SpawnCheck(newPos))
			{
				m_vecObjects.push_back(new CDiscCollisionObject(CCollisionObject::Mine, newPos));
				break;
			}
		}
	}
}

void CDiscController::InitializeSuperMines(void)
{
	for (int i=0; i<m_NumSuperMines; ++i)
	{
		while (true)
		{
			SVector2D<int> newPos(RandInt(0, CParams::WindowWidth / CParams::iGridCellDim)*CParams::iGridCellDim,
				RandInt(0, CParams::WindowHeight / CParams::iGridCellDim)*CParams::iGridCellDim);

			if (SpawnCheck(newPos))
			{
				m_vecObjects.push_back(new CDiscCollisionObject(CCollisionObject::SuperMine, newPos));
				break;
			}
		}
	}
}

bool CDiscController::SpawnCheck(SVector2D<int> newPos)
{
	for (int j = 0; j < m_vecObjects.size(); ++j)
	{
		SVector2D<int> existingPos = m_vecObjects[j]->getPosition();
		if (abs(existingPos.x - newPos.x) <= (CParams::iSweeperScale * 2) &&
			abs(existingPos.y - newPos.y) <= (CParams::iSweeperScale * 2))
		{
			return false;
		}
	}
	return true;
}

void CDiscController::InitializeRocks(void)
{
	for (int i=0; i<m_NumRocks; ++i)
	{
		m_vecObjects.push_back(new CDiscCollisionObject(CCollisionObject::Rock, 
								 SVector2D<int>(RandInt(0,CParams::WindowWidth/CParams::iGridCellDim)*CParams::iGridCellDim, 
					             RandInt(0,CParams::WindowHeight/CParams::iGridCellDim)*CParams::iGridCellDim)));
	}
}

//-------------------------------------Update-----------------------------
//
//	This is the main workhorse. The entire simulation is controlled from here.
//
//	The comments should explain what is going on adequately.
//-------------------------------------------------------------------------
bool CDiscController::Update()
{
	//run the sweepers through CParams::iNumTicks amount of cycles. During
	//this loop each sweeper is constantly updated with the appropriate
	//information from its surroundings. The output from the learning algorithm is obtained
	//and the sweeper is moved. If it encounters a mine its MinesGathered is
	//updated appropriately,
	if (m_iTicks++ < CParams::iNumTicks)
	{
		for (int i=0; i<m_NumSweepers; ++i)
		{
			if ((m_vecSweepers[i])->isDead()) continue;
			//update the position
			if (!(m_vecSweepers[i])->Update(m_vecObjects))
			{
				//error in processing the learning algorithm
				MessageBox(m_hwndMain, "An error occured while processing!", "Error", MB_OK);

				return false;
			}
				
			//see if it's found a mine
			int GrabHit = ((m_vecSweepers[i])->CheckForObject(m_vecObjects,
													CParams::dMineScale));

			if (GrabHit >= 0)
			{
				switch(m_vecObjects[GrabHit]->getType()){
				case CDiscCollisionObject::Mine:
					{
					//we have discovered a mine so increase MinesGathered
					if (!m_vecObjects[GrabHit]->isDead())
					{
						(m_vecSweepers[i])->IncrementMinesGathered();
						CDiscCollisionObject* oldObject = m_vecObjects[GrabHit];
						oldObject->die();
					}
					break;
					}
				case CDiscCollisionObject::Rock:
					{
					//destroy the sweeper until it reincarnates in the next round
					//CDiscCollisionObject* oldObject = m_vecObjects[GrabHit];
					//oldObject->die();
					(m_vecSweepers[i])->die();
					break;
					}
				case CDiscCollisionObject::SuperMine:
					{
					//destroy both the sweeper and the supermine until both reincarnate in the next round
					CDiscCollisionObject* oldObject = m_vecObjects[GrabHit];
					oldObject->die();
					(m_vecSweepers[i])->die();
					break;
					}
				}
			}
		}
	}
	//Time to update the sweepers for the next iteration
	else
	{
		//update the stats to be used in our stat window
		double sum = 0;
		int maxMines = -1;
		int deaths = 0;
		for (auto i = m_vecSweepers.begin(); i != m_vecSweepers.end(); ++i){
			sum += (*i)->MinesGathered();
			if ((*i)->isDead())
				deaths++;
			maxMines = max((*i)->MinesGathered(),maxMines);
		}

		m_vecAvMinesGathered.push_back(sum/float(m_vecSweepers.size()));
		m_vecMostMinesGathered.push_back(maxMines);
		m_vecDeaths.push_back(deaths);

		//increment the iteration counter
		++m_iIterations;

		//reset cycles
		m_iTicks = 0;
	
		//reset the sweepers positions etc
		for (int i=0; i<m_NumSweepers; ++i)
		{
			(m_vecSweepers[i])->Reset();
		}

		//reset the objects:
		for (auto i = m_vecObjects.begin(); i != m_vecObjects.end(); ++i)
			(*i)->Reset();
	}
	return true;
}


//------------------------------------Render()--------------------------------------
//
//----------------------------------------------------------------------------------
void CDiscController::Render(HDC surface)
{

	//render the stats
	string s = "Iteration:          " + itos(m_iIterations);
	TextOut(surface, 5, 0, s.c_str(), s.size());

	//do not render if running at accelerated speed
	if (!m_bFastRender)
	{
		//keep a record of the old pen
		m_OldPen = (HPEN)SelectObject(surface, m_GreenPen);
		
		//render the mines
		for (int i=0; i<m_NumMines+m_NumSuperMines+m_NumRocks; ++i)
		{
			if ((m_vecObjects[i])->isDead()) continue; //skip if dead
			if ( m_vecObjects[i]->getType() == CCollisionObject::Mine)
			{
				SelectObject(surface, m_GreenPen);
			}
			else if ( m_vecObjects[i]->getType() == CCollisionObject::Rock)
			{
				SelectObject(surface, m_BluePen );
			}
			else if ( m_vecObjects[i]->getType() == CCollisionObject::SuperMine)
			{
				SelectObject(surface, m_RedPen);
			}
			//grab the vertices for the mine shape
			vector<SPoint> mineVB = m_MineVB;

			WorldTransform<int>(mineVB, m_vecObjects[i]->getPosition());

			//draw the mines
			MoveToEx(surface, (int)mineVB[0].x, (int)mineVB[0].y, NULL);

			for (int vert=1; vert<mineVB.size(); ++vert)
			{
				LineTo(surface, (int)mineVB[vert].x, (int)mineVB[vert].y);
			}

			LineTo(surface, (int)mineVB[0].x, (int)mineVB[0].y);
			
		}
       		
		//we want some sweepers displayed in red
		SelectObject(surface, m_RedPen);

		//render the sweepers
		for (int i=0; i<m_NumSweepers; i++)
		{
			if (i == CParams::iNumElite)
			{
				SelectObject(surface, m_OldPen);
			}
			if (m_vecSweepers[i]->isDead()) continue; //skip if dead

			//grab the sweeper vertices
			vector<SPoint> sweeperVB = m_SweeperVB;

			//transform the vertex buffer
			(m_vecSweepers[i])->WorldTransform(sweeperVB);

			//draw the sweeper left track
			MoveToEx(surface, (int)sweeperVB[0].x, (int)sweeperVB[0].y, NULL);

			for (int vert=1; vert<4; ++vert)
			{
				LineTo(surface, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y);
			}

			LineTo(surface, (int)sweeperVB[0].x, (int)sweeperVB[0].y);

			//draw the sweeper right track
			MoveToEx(surface, (int)sweeperVB[4].x, (int)sweeperVB[4].y, NULL);

			for (int vert=5; vert<8; ++vert)
			{
				LineTo(surface, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y);
			}

			LineTo(surface, (int)sweeperVB[4].x, (int)sweeperVB[4].y);

			MoveToEx(surface, (int)sweeperVB[8].x, (int)sweeperVB[8].y, NULL);
			LineTo(surface, (int)sweeperVB[9].x, (int)sweeperVB[9].y);

			MoveToEx(surface, (int)sweeperVB[10].x, (int)sweeperVB[10].y, NULL);

			for (int vert=11; vert<16; ++vert)
			{
				LineTo(surface, (int)sweeperVB[vert].x, (int)sweeperVB[vert].y);
			}

		}

		//put the old pen back
		SelectObject(surface, m_OldPen);

	}//end if

  else
  {
    PlotStats(surface);
  }

}