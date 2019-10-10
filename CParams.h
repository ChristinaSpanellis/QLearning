#ifndef CPARAMS_H
#define CPARAMS_H
//------------------------------------------------------------------------
//
//	Name: CParams.h
//
//  Author: Mat Buckland 2002
//
//  Desc: Class to hold all the parameters used in this project. The values
//        are loaded in from an ini file when an instance of the class is
//        created.
//       
//
//------------------------------------------------------------------------
#include <fstream>
#include <windows.h>
#include <string>
#include <algorithm>
#include <functional> 
#include <cctype>
#include <locale>

class CParams
{

public:

  //------------------------------------general parameters
  static double dPi;
  static double dHalfPi;
  static double dTwoPi;

  static int    WindowWidth;
  static int    WindowHeight;

  static int    iFramesPerSecond;
  
  //--------------------------------------used to define the sweepers

  //limits how fast the sweepers can turn
  static double dMaxTurnRate;

  static double dMaxSpeed;

  //for controlling the size
  static int   iSweeperScale;


  //--------------------------------------controller parameters
  static int    iNumSweepers;

  static int    iNumMines;
  static int    iNumSuperMines;
  static int    iNumRocks;

  //number of time steps we allow for each generation to live
  static int    iNumTicks;

  //scaling factor for mines
  static double dMineScale;
    
  //used for elitism
  static int    iNumElite;
  static int    iNumCopiesElite;
  
  static bool		  bDiscreteGrid;
  static int		  iGridCellDim;
  static std::string  sTrainingFilename;

  //ctor
  CParams()
  {
    if(!LoadInParameters("params.ini"))
    {
      MessageBox(NULL, "Cannot find ini file!", "Error", 0);
    }

  }

  bool LoadInParameters(char* szFileName);
};






#endif