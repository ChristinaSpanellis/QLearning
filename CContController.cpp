#include "CContController.h"


CContController::CContController(HWND hwndMain):
	CController(hwndMain)
{
	
}

CContController::~CContController(void)
{
	for (auto i = m_vecObjects.begin(); i != m_vecObjects.end(); ++i)
		delete *i;
	for (auto i = m_vecSweepers.begin(); i != m_vecSweepers.end(); ++i)
		delete *i;
}

void CContController::InitializeLearningAlgorithm(void)
{
	/**
	This is the basic continious environment so there are no learning algorithms
	to initialize yet...
	*/
}

void CContController::InitializeSweepers(void)
{
	for (int i=0; i<m_NumSweepers; ++i)
	{
		m_vecSweepers.push_back(new CContMinesweeper());
	}
}

void CContController::InitializeMines(void)
{
	for (int i=0; i<m_NumMines; ++i)
	{
		m_vecObjects.push_back(new CContCollisionObject(CCollisionObject::Mine, SVector2D<double>(RandFloat() * cxClient,
                                   RandFloat() * cyClient)));
	}
}

void CContController::InitializeSuperMines(void)
{
	for (int i=0; i<m_NumSuperMines; ++i)
	{
		m_vecObjects.push_back(new CContCollisionObject(CCollisionObject::SuperMine, SVector2D<double>(RandFloat() * cxClient,
                                   RandFloat() * cyClient)));
	}
}

void CContController::InitializeRocks(void)
{
	for (int i=0; i<m_NumRocks; ++i)
	{
		m_vecObjects.push_back(new CContCollisionObject(CCollisionObject::Rock, SVector2D<double>(RandFloat() * cxClient,
                                   RandFloat() * cyClient)));
	}
}

//-------------------------------------Update-----------------------------
//
//	This is the main workhorse. The entire simulation is controlled from here.
//
//	The comments should explain what is going on adequately.
//-------------------------------------------------------------------------
bool CContController::Update()
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
			if (m_vecSweepers[i]->isDead()) continue; //skip if dead
			//update the position
			if (!(m_vecSweepers[i])->Update(m_vecObjects))
			{
				//error in processing the learning algorithm
				MessageBox(m_hwndMain, "An error occured while processing!", "Error", MB_OK);

				return false;
			}
				
			//see if it's found a mine
			int GrabHit = (m_vecSweepers[i])->CheckForObject(m_vecObjects,
													CParams::dMineScale);

			if (GrabHit >= 0)
			{
				switch(m_vecObjects[GrabHit]->getType()){
				case CContCollisionObject::Mine:
					{
					//we have discovered a mine so increase MinesGathered
					(m_vecSweepers[i])->IncrementMinesGathered();

					//mine found so replace the mine with another at a random 
					//position
					CContCollisionObject* oldObject = m_vecObjects[GrabHit];
					m_vecObjects[GrabHit] = new CContCollisionObject(oldObject->getType(),SVector2D<double>(RandFloat() * cxClient,
											RandFloat() * cyClient));
					delete oldObject;
					break;
					}
				case CContCollisionObject::Rock:
					{
					//destroy the sweeper until it reincarnates in the next round
					//CContCollisionObject* oldObject = m_vecObjects[GrabHit];
					//oldObject->die();
					(m_vecSweepers[i])->die();
					break;
					}
				case CContCollisionObject::SuperMine:
					{
					//destroy both the sweeper and the supermine until they reincarnate in the next round
					CContCollisionObject* oldObject = m_vecObjects[GrabHit];
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
		//reset supermine
		for (int i = 0; i < m_NumMines+m_NumRocks+m_NumSuperMines; ++i)
		{
			if (m_vecObjects[i]->getType() == CCollisionObject::ObjectType::SuperMine)
				m_vecObjects[i]->Reset();
		}
	}
	return true;
}

//------------------------------------Render()--------------------------------------
//
//----------------------------------------------------------------------------------
void CContController::Render(HDC surface)
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
			if ( (m_vecObjects[i])->getType() == CCollisionObject::Mine)
			{
				SelectObject(surface, m_GreenPen);
			}
			else if ( (m_vecObjects[i])->getType() == CCollisionObject::Rock)
			{
				SelectObject(surface, m_BluePen );
			}
			else if ( (m_vecObjects[i])->getType() == CCollisionObject::SuperMine)
			{
				SelectObject(surface, m_RedPen);
			}
			//grab the vertices for the mine shape
			vector<SPoint> mineVB = m_MineVB;

			WorldTransform<double>(mineVB, (m_vecObjects[i])->getPosition());

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