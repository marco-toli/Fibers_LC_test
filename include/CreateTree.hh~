#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"



class CreateTree
{
private:
  
  TTree*  ftree;
  TString fname;
  
public:
  
  bool ENERGY_FIBER;
  bool INIT_DATA; 
  bool POS_FIBER;
  bool OPPHOTONS;
  bool TIMING;
  
public:
  
  CreateTree(TString name, bool energy_fiber, bool init_data, bool pos_fiber, bool opPhotons, bool timing);
  ~CreateTree();
  
  TTree*             GetTree() const { return ftree; };
  TString            GetName() const { return fname; };
  int                Fill() { return this->GetTree()->Fill(); };
  bool               Write();
  void               Clear();
  static CreateTree* Instance() { return fInstance; };
  static CreateTree* fInstance;
  
  bool Energy_fiber() const { return this -> ENERGY_FIBER; };
  bool Init_data()    const { return this -> INIT_DATA;    };
  bool Pos_fiber()    const { return this -> POS_FIBER;    };
  bool OpPhotons()    const { return this -> OPPHOTONS;    };
  bool Timing()       const { return this -> TIMING;       };
  
  int Event;
  
  float InitialEnergy;
  
  float InitialPositionX;
  float InitialPositionY;
  float InitialPositionZ;
  
  float InitalMomentumDirectionX;
  float InitalMomentumDirectionY;
  float InitalMomentumDirectionZ;
  
  std::vector<float> depositionX;
  std::vector<float> depositionY;
  std::vector<float> depositionZ;
  std::vector<float> Energy_deposited;
  std::vector<float> Time_deposit;

  float Tot_phot_cer;
  float Tot_phot_scint;
  
  float Total_delta_world;
  float Total_energy_world;
  float Total_ion_energy_world;
  float Total_nonion_energy_world;
  
  
  float Total_energy;
  float Total_ion_energy;
  float Total_ion_energy_att[10];
  
  
  std::vector<int> opPhoton_process;
  std::vector<int> opPhoton_trackID;
  std::vector<int> opPhoton_parentTrackID;
  std::vector<int> opPhoton_fiberIX;
  std::vector<int> opPhoton_fiberIZ;
  std::vector<float> opPhoton_energy;
  std::vector<float> opPhoton_waveLength;
  std::vector<float> opPhoton_time;
  std::vector<float> opPhoton_vertexX;
  std::vector<float> opPhoton_vertexY;
  std::vector<float> opPhoton_vertexZ;
  std::vector<float> opPhoton_pX;
  std::vector<float> opPhoton_pY;
  std::vector<float> opPhoton_pZ;

  int opPhoton_n;
  int opPhoton_n_ext;
  int opPhoton_n_det;

  int opPhoton_n_det_central;
  int opPhoton_n_det_3x3;
  int opPhoton_n_det_5x5;
  
  std::vector<int> opPhoton_process_det;
  std::vector<int> opPhoton_trackID_det;
  std::vector<int> opPhoton_parentTrackID_det;
  std::vector<int> opPhoton_fiberIX_det;
  std::vector<int> opPhoton_fiberIZ_det;
  std::vector<float> opPhoton_energy_det;
  
  std::vector<float> opPhoton_waveLength_det;
  std::vector<float> opPhoton_time_det;
  std::vector<float> opPhoton_trackLength_det;
  
};