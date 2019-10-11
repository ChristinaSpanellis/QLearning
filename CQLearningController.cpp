/**
         (                                      
   (     )\ )                                   
 ( )\   (()/(   (    ) (        (        (  (   
 )((_)   /(_)) ))\( /( )(   (   )\  (    )\))(  
((_)_   (_))  /((_)(_)|()\  )\ |(_) )\ )((_))\  
 / _ \  | |  (_))((_)_ ((_)_(_/((_)_(_/( (()(_) 
| (_) | | |__/ -_) _` | '_| ' \)) | ' \)) _` |  
 \__\_\ |____\___\__,_|_| |_||_||_|_||_|\__, |  
                                        |___/   

Refer to Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
for a detailed discussion on Q Learning
*/
#include "CQLearningController.h"


CQLearningController::CQLearningController(HWND hwndMain):
	CDiscController(hwndMain),
	_grid_size_x(CParams::WindowWidth / CParams::iGridCellDim +1),
	_grid_size_y(CParams::WindowHeight / CParams::iGridCellDim +1)
{
}
/**
 The update method should allocate a Q table for each sweeper (this can
 be allocated in one shot - use an offset to store the tables one after the other)

 You can also use a boost multiarray if you wish
*/
void CQLearningController::InitializeLearningAlgorithm(void)
{ 	
	CDiscController::InitializeLearningAlgorithm();
	// This is my "reward" matrix
	// a 2D matrix of dim x by y that stores the reward
	// if there is a supermine at x,y then R[x,y] = -100, if there is a mine at x,y then R[x,y] = 100, if there is a rock at x,y then R[x,y] = -100

	// This initialises the QTables
	// a 4D vector, the first level is for each mine sweeper
	// then it basically represents the X by Y grid world, where each entry
	// is a vector with four entries, one for left/right/up/down with all values set to 0.0
	QTables = new double*** [m_vecSweepers.size()];
	QTableCounter = new double*** [m_vecSweepers.size()];
	for (int i = 0;i < m_vecSweepers.size();++i) {
		QTables[i] = new double** [_grid_size_x];
		QTableCounter[i] = new double** [_grid_size_x];
		for (int x = 0;x < _grid_size_x;++x) {
			QTables[i][x] = new double* [_grid_size_y];
			QTableCounter[i][x] = new double* [_grid_size_y];
			for (int j = 0; j < _grid_size_y;++j) {
				QTables[i][x][j] = new double[4];
				QTableCounter[i][x][j] = new double[4];
				for (int a = 0; a < 4; ++a) {
					QTables[i][x][j][a] = 0.0f;
					QTableCounter[i][x][j][a] = 0.0f;

				}
			}

		}
	}
	//TODO
}
/**
 The immediate reward function. This computes a reward upon achieving the goal state of
 collecting all the mines on the field. It may also penalize movement to encourage exploring all directions and 
 of course for hitting supermines/rocks!
*/
double CQLearningController::R(uint x,uint y, uint sweeper_no){
	//TODO: roll your own here!
		int collidableObjectIndex = m_vecSweepers[sweeper_no]->CheckForObject(m_vecObjects, CParams::dMineScale);
		if (collidableObjectIndex > -1) {
			if (m_vecObjects[collidableObjectIndex]->getType() == CCollisionObject::Mine) {
				return 100;
			}
			else if (m_vecObjects[collidableObjectIndex]->getType() == CCollisionObject::SuperMine) {
				return -500;
			}
		}		
		else if (QTableCounter[sweeper_no][x/10][y/10][0] > 0 || QTableCounter[sweeper_no][x/10][y/10][1] > 0 || QTableCounter[sweeper_no][x/10][y/10][2] > 0 || QTableCounter[sweeper_no][x/10][y/10][3] >0 ) {
			return -200; // discount it more if it has already explored this area
		}
		return -100;
}
/**
The update method. Main loop body of our Q Learning implementation
See: Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
*/
bool CQLearningController::Update(void)
{
	if (m_iTicks == 1) {
		epsilon *= 0.9;
	}
	//write data to a file for the table
	if (m_iIterations == 51) {
		ofstream out;
		out.open("test1.txt");
		int mostMinesGathered = 0;
		double averageMines = 0.0f;
		for (int i = 0; i < 50; ++i) {
			if (m_vecMostMinesGathered[i] > mostMinesGathered) {
				mostMinesGathered = m_vecMostMinesGathered[i];
			}
			averageMines += m_vecAvMinesGathered[i];
		}
		out << mostMinesGathered << " " << averageMines/50 << endl;
		out.close();
	}
	
	//m_vecSweepers is the array of minesweepers
	//everything you need will be m_[something] ;)
	uint cDead = std::count_if(m_vecSweepers.begin(),
							   m_vecSweepers.end(),
						       [](CDiscMinesweeper * s)->bool{
								return s->isDead();
							   });
	if (cDead == CParams::iNumSweepers){
		printf("All dead ... skipping to next iteration\n");
		m_iTicks = CParams::iNumTicks;
	}
	

	for (uint sw = 0; sw < CParams::iNumSweepers; ++sw){
		if (m_vecSweepers[sw]->isDead()) continue;
		/**
		Q-learning algorithm according to:
		Watkins, Christopher JCH, and Peter Dayan. "Q-learning." Machine learning 8. 3-4 (1992): 279-292
		*/
		//1:::Observe the current state:
		//TODO
		//2:::Select action with highest historic return:
		int finalAction;
		// this is e-greedy, chooses a random action if random<epsilon, else chooses the optimal action
		double random = RandFloat();
		if (random < epsilon) {
			finalAction = RandInt(0, 3);
			m_vecSweepers[sw]->setRotation((ROTATION_DIRECTION)finalAction);
		}
		else {
			int action = RandInt(0, 3);
			double maxReward = QTables[sw][m_vecSweepers[sw]->Position().x / 10][m_vecSweepers[sw]->Position().y / 10][action];
			for (int i = 0; i < 4; ++i) {
				double reward = QTables[sw][m_vecSweepers[sw]->Position().x / 10][m_vecSweepers[sw]->Position().y / 10][i];
				if (reward > maxReward) {
					maxReward = reward;
				}
			}
			// need to check if they're all the same, then choose a random one
			vector<int> actions;
			for (int i = 0; i < 4; ++i) {
				if (QTables[sw][m_vecSweepers[sw]->Position().x / 10][m_vecSweepers[sw]->Position().y / 10][i] == maxReward) {
					actions.push_back(i);
				}
			}
			finalAction = RandInt(0, actions.size() - 1);
			m_vecSweepers[sw]->setRotation((ROTATION_DIRECTION)actions[finalAction]);
		}
		m_vecSweepers[sw]->setRotation((ROTATION_DIRECTION)RandInt(0,3));

		//enum ROTATION_DIRECTION {NORTH=1, SOUTH=3, EAST=0, WEST=2};
		//TODO
		//now call the parents update, so all the sweepers fulfill their chosen action
	}
	
	CDiscController::Update(); //call the parent's class update. Do not delete this.
	
	for (uint sw = 0; sw < CParams::iNumSweepers; ++sw){
		if (m_iTicks == 1) {
			m_vecSweepers[sw]->beenDead= false;
		}
		if (m_vecSweepers[sw]->isDead()) {
			if (m_vecSweepers[sw]->beenDead) { 
				continue; }
			m_vecSweepers[sw]->beenDead = true;
		}
		//TODO:compute your indexes.. it may also be necessary to keep track of the previous state
		
		//enum ROTATION_DIRECTION {NORTH=1, SOUTH=3, EAST=0, WEST=2};
		//3:::Observe new state:

		//TODO
		//4:::Update _Q_s_a accordingly:
		SVector2D<int> prevState = m_vecSweepers[sw]->PrevPosition();
		SVector2D<int> currentState = m_vecSweepers[sw]->Position();
		double nextImmReward = R(prevState.x, prevState.y, sw);
		// uses a decaying learning rate calculated by
		// 1/(1+visits(s,a))
		QTables[sw][prevState.x / 10][prevState.y / 10][m_vecSweepers[sw]->getRotation()] +=  (1/(1+QTableCounter[sw][prevState.x / 10][prevState.y / 10][m_vecSweepers[sw]->getRotation()]))*(nextImmReward+discountFactor*(getMaxQValue(currentState.x, currentState.y, sw))- QTables[sw][prevState.x / 10][prevState.y / 10][m_vecSweepers[sw]->getRotation()]);
		++QTableCounter[sw][prevState.x / 10][prevState.y / 10][m_vecSweepers[sw]->getRotation()];

		//TODO
	}
	return true;
}

CQLearningController::~CQLearningController(void)
{
	for (int i = 0; i < m_vecSweepers.size(); ++i) {
		for (int j = 0; j < _grid_size_x; ++j) {
			for (int x = 0; x < _grid_size_y; ++x) {
				delete[] QTables[i][j][x];
				delete[] QTableCounter[i][j][x];
			}
			delete[] QTables[i][j];
			delete[] QTableCounter[i][j];
		}
		delete[] QTables[i];
		delete[] QTableCounter[i];


	}
	delete[] QTables;
	delete[] QTableCounter;
	//TODO: dealloc stuff here if you need to	
}

double CQLearningController::getMaxQValue(int x, int y, int sw) {
	double max = QTables[sw][x / 10][y / 10][RandInt(0,3)];
	for (int i = 0; i < 4; ++i) {
		double curr = QTables[sw][x / 10][y / 10][i];
		if (curr > max) {
			max = curr;
		}
	}
	return max;
}
