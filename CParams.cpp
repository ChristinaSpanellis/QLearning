#include "CParams.h"

// trim from start
static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
}

//because we will always be loading in the settings from an ini file
//we can just initialize everything to zero
double CParams::dPi                 = 3.14159265358979;
double CParams::dHalfPi             = dPi / 2;
double CParams::dTwoPi              = dPi * 2;
int CParams::WindowWidth            = 400;
int CParams::WindowHeight           = 400;
int CParams::iFramesPerSecond       = 0;
double CParams::dMaxTurnRate        = 0;
double CParams::dMaxSpeed           = 0;
int CParams::iSweeperScale          = 0;
int CParams::iNumSweepers           = 0;
int CParams::iNumMines              = 0;
int CParams::iNumSuperMines         = 0;
int CParams::iNumRocks              = 0;
int CParams::iNumTicks              = 0;
double CParams::dMineScale          = 0;
int CParams::iNumElite              = 0;
int CParams::iNumCopiesElite        = 0;
bool CParams::bDiscreteGrid			= false;
int CParams::iGridCellDim			= 10;
std::string CParams::sTrainingFilename	= "training.txt";
//this function loads in the parameters from a given file name. Returns
//false if there is a problem opening the file.
bool CParams::LoadInParameters(char* szFileName)
{
  using namespace std;	
  ifstream grab(szFileName);

  //check file exists
  if (!grab)
  {
    return false;
  }

  //load in from the file
  char ParamDescription[40];

  grab >> ParamDescription;
  grab >> iFramesPerSecond;
  grab >> ParamDescription;
  grab >> dMaxTurnRate;
  grab >> ParamDescription;
  grab >> dMaxSpeed;
  grab >> ParamDescription;
  grab >> iSweeperScale;
  grab >> ParamDescription;
  grab >> iNumMines;
  grab >> ParamDescription;
  grab >> iNumSuperMines;
  grab >> ParamDescription;
  grab >> iNumRocks;
  grab >> ParamDescription;
  grab >> iNumSweepers;
  grab >> ParamDescription;
  grab >> iNumTicks;
  grab >> ParamDescription;
  grab >> dMineScale;
  grab >> ParamDescription;
  grab >> iNumElite;
  grab >> ParamDescription;
  grab >> iNumCopiesElite;
  grab >> ParamDescription;
  grab >> bDiscreteGrid;
  grab >> ParamDescription;
  grab >> iGridCellDim;
  grab >> ParamDescription;  
  getline(grab,sTrainingFilename);
  sTrainingFilename = trim(sTrainingFilename);
  return true;
}
 




  
  
