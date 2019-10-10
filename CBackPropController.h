#pragma once
#include "ccontcontroller.h"
#include "CNeuralNet.h"
#include <assert.h>
class CBackPropController :
	public CContController
{
protected:
	CNeuralNet* _neuralnet;
public:
	CBackPropController(HWND hwndMain);
	virtual void InitializeLearningAlgorithm(void);
	virtual bool Update(void);
	~CBackPropController(void);
};

