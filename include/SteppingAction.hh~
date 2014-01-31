// Martin Goettlich @ DESY
//

#ifndef SteppingAction_H
#define SteppingACtion_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"
#include "DetectorConstruction.hh"
#include <iostream>

#include "TFile.h"
#include "TF1.h"
#include "TTree.h"
#include "TString.h"
#include <vector>



  const int nATT = 11;
  double par0[nATT], par1[nATT], par2[nATT];

  //class SteppingMessenger;
  
class SteppingAction : public G4UserSteppingAction
{
public:

  SteppingAction();
  ~SteppingAction();
  virtual void UserSteppingAction(const G4Step*);
  

  

  double fAtt (double par0, double par1, double par2, double z) {
    
    double att = par0*exp((z-220)/par1) + par2;
    return att;
    
  }
  
     
  //void SetOneStepPrimaries(G4bool b){oneStepPrimaries=b;}
  //G4bool GetOneStepPrimaries(){return oneStepPrimaries;}
  
// private:

//   G4bool oneStepPrimaries;
//   SteppingMessenger* steppingMessenger;
};

#endif
