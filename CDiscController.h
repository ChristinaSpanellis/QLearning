#pragma once
#include "ccontroller.h"
#include "CDiscCollisionObject.h"
#include "CDiscMinesweeper.h"
#include <assert.h>
class CDiscController :
	public CController
{
protected:
	//and the minesweepers
    vector<CDiscMinesweeper*> m_vecSweepers;

	//and the mines
	vector<CDiscCollisionObject*> m_vecObjects;
public:
	CDiscController(HWND hwndMain);
	virtual ~CDiscController(void);
	virtual void Render(HDC surface);
	virtual bool Update(void);
	virtual void InitializeLearningAlgorithm(void);
	virtual void InitializeSweepers(void);
	virtual void InitializeMines(void);
	virtual void InitializeSuperMines(void);
	virtual void InitializeRocks(void);
	virtual bool SpawnCheck(SVector2D<int>);
};

