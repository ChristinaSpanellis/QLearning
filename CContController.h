#pragma once
#include "ccontroller.h"
#include "CContCollisionObject.h"
#include "CContMinesweeper.h"
#include <algorithm>
class CContController :
	public CController
{
protected:
	//and the minesweepers
    vector<CContMinesweeper*> m_vecSweepers;

	//and the mines
	vector<CContCollisionObject*> m_vecObjects;
public:
	CContController(HWND hwndMain);
	virtual ~CContController(void);
	virtual void Render(HDC surface);
	virtual bool Update(void);
	virtual void InitializeLearningAlgorithm(void);
	virtual void InitializeSweepers(void);
	virtual void InitializeMines(void);
	virtual void InitializeSuperMines(void);
	virtual void InitializeRocks(void);
};

