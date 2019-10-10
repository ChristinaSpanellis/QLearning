#include "CMinesweeper.h"

CMinesweeper::~CMinesweeper(void)
{
}

void CMinesweeper::Reset(){
	this->m_dMinesGathered = 0;
	this->m_bDead = false;
}
void CMinesweeper::die(){
	this->m_bDead = true;
}
bool CMinesweeper::isDead() const{
	return m_bDead;
}
int	CMinesweeper::getClosestMine()
{
	return m_iClosestMine;
}
int	CMinesweeper::getClosestRock()
{
	return m_iClosestRock;
}
int	CMinesweeper::getClosestSupermine()
{
	return m_iClosestSupermine;
}