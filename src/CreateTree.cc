#include "CreateTree.hh"

using namespace std ;

CreateTree* CreateTree::fInstance = NULL;



CreateTree::CreateTree(TString name, bool energy_fiber, bool init_data, bool pos_fiber, bool opPhotons, bool timing)
{
  if( fInstance )
  {
    return;
  }
  
  this -> ENERGY_FIBER 	= energy_fiber;
  this -> INIT_DATA    	= init_data;
  this -> POS_FIBER    	= pos_fiber;
  this -> OPPHOTONS     = opPhotons;
  this -> TIMING        = timing;
  
  this -> fInstance = this;
  this -> fname     = name;
  this -> ftree     = new TTree(name,name);
  
  this->GetTree()->Branch("Event",&this->Event,"Event/I");
  
  this->GetTree()->Branch("Fiber_radius",		   &this->Fiber_radius,			 "Fiber_radius/F");
  this->GetTree()->Branch("Fiber_length",		   &this->Fiber_length,			 "Fiber_length/F");
  this->GetTree()->Branch("Coupling",			   &this->Coupling,			 "Coupling/F");
  this->GetTree()->Branch("SurfaceType",		   &this->SurfaceType,			 "SurfaceType/F");
  this->GetTree()->Branch("DetDistance",		   &this->DetDistance,			 "DetDistance/F");
  
  if( this -> INIT_DATA )
  {
    this->GetTree()->Branch("InitialEnergy",&this->InitialEnergy,"InitialEnergy/F"); 
    
    this->GetTree()->Branch("InitialPositionX",&this->InitialPositionX,"InitialPositionX/F");
    this->GetTree()->Branch("InitialPositionY",&this->InitialPositionY,"InitialPositionY/F");
    this->GetTree()->Branch("InitialPositionZ",&this->InitialPositionZ,"InitialPositionZ/F");
    
    this->GetTree()->Branch("InitalMomentumDirectionX",&this->InitalMomentumDirectionX,"InitalMomentumDirectionX/F");
    this->GetTree()->Branch("InitalMomentumDirectionY",&this->InitalMomentumDirectionY,"InitalMomentumDirectionY/F");
    this->GetTree()->Branch("InitalMomentumDirectionZ",&this->InitalMomentumDirectionZ,"InitalMomentumDirectionZ/F");

    this->GetTree()->Branch("Total_energy_world",&this->Total_energy_world,"Total_energy_world/F");
    this->GetTree()->Branch("Total_ion_energy_world",&this->Total_ion_energy_world,"Total_ion_energy_world/F");	 
  }
    
  if( this -> ENERGY_FIBER)
  {
    //this->GetTree()->Branch("Total_ion_energy_att",&this->Total_ion_energy_att,"Total_ion_energy_att[11]/F");	  

    this->GetTree()->Branch("Total_energy_fib",&this->Total_energy_fib,"Total_energy_fib/F");
    this->GetTree()->Branch("Total_ion_energy_fib",&this->Total_ion_energy_fib,"Total_ion_energy_fib/F");	  

    this->GetTree()->Branch("Total_energy_abs",&this->Total_energy_abs,"Total_energy_abs/F");
    this->GetTree()->Branch("Total_ion_energy_abs",&this->Total_ion_energy_abs,"Total_ion_energy_abs/F");	  
  }
  
  if( this -> POS_FIBER)
  { 
    this->GetTree()->Branch("depositionX",&depositionX);
    this->GetTree()->Branch("depositionY",&depositionY);
    this->GetTree()->Branch("depositionZ",&depositionZ);  
    this->GetTree()->Branch("Energy_deposited",&Energy_deposited);
    this->GetTree()->Branch("Time_deposit",&Time_deposit);
  }
  
  if( this -> OPPHOTONS)
  {
    this->GetTree()->Branch("opPhoton_n",&this->opPhoton_n,"opPhoton_n/I");
    this->GetTree()->Branch("opPhoton_n_type",&this->opPhoton_n_type);
    
    this->GetTree()->Branch("opPhoton_n_ext",&this->opPhoton_n_ext,"opPhoton_n_ext/I");
    this->GetTree()->Branch("opPhoton_n_ext_type",&this->opPhoton_n_ext_type);
    
    this->GetTree()->Branch("opPhoton_n_det",&this->opPhoton_n_det,"opPhoton_n_det/I");
    this->GetTree()->Branch("opPhoton_n_det_type",&this->opPhoton_n_det_type);
       
    this->GetTree()->Branch("opPhoton_process_det",&opPhoton_process_det);
    this->GetTree()->Branch("opPhoton_waveLength_det",&opPhoton_waveLength_det);
    this->GetTree()->Branch("opPhoton_time_det",&opPhoton_time_det);
    this->GetTree()->Branch("opPhoton_trackLength_det",&opPhoton_trackLength_det);
  }
  
  this->Clear();
}



CreateTree::~CreateTree()
{}



bool CreateTree::Write()
{
  TString filename = this->GetName();
  filename+=".root";
  TFile* file = new TFile(filename,"RECREATE");
  this->GetTree()->Write();
  file->Write();
  file->Close();
  return true;
}



void CreateTree::Clear()
{
  Event	= 0;
  
  Total_energy_world        = 0;
  Total_ion_energy_world    = 0;

  
  if( this->INIT_DATA )
  {
    InitialEnergy = 0;
    
    InitialPositionX = 0;
    InitialPositionY = 0;
    InitialPositionZ = 0;
    
    InitalMomentumDirectionX = 0;
    InitalMomentumDirectionY = 0;
    InitalMomentumDirectionZ = 0;
  }
  
  if( this->ENERGY_FIBER )
  {
    
        Total_energy_fib     = 0;
        Total_ion_energy_fib = 0;	  		    	  
        Total_energy_abs     = 0;
        Total_ion_energy_abs = 0;	  		    	  

	for (int j = 0; j< 10; j++) Total_ion_energy_att[j] = 0; 
  }
  
  if( this->POS_FIBER )
  {
    depositionX.clear();		
    depositionY.clear();		
    depositionZ.clear();
    Energy_deposited.clear();
    Time_deposit.clear();
  }
  
  if( this->OPPHOTONS )
  {
    
//     opPhoton_process.clear();
//     opPhoton_trackID.clear();
//     opPhoton_parentTrackID.clear();
//     opPhoton_fiberIX.clear();
//     opPhoton_fiberIZ.clear();
//     opPhoton_energy.clear();
//     opPhoton_waveLength.clear();
//     opPhoton_time.clear();
//     opPhoton_vertexX.clear();
//     opPhoton_vertexY.clear();
//     opPhoton_vertexZ.clear();
//     opPhoton_pX.clear();
//     opPhoton_pY.clear();
//     opPhoton_pZ.clear();

//     for (int i = 0; i < 25; i++) {
      opPhoton_n = 0;
      opPhoton_n_type.clear();
      
      opPhoton_n_ext = 0;
      opPhoton_n_ext_type.clear();
      
      opPhoton_n_det = 0;
      opPhoton_n_det_type.clear();
      
//     }
    
    
    opPhoton_process_det.clear();

    opPhoton_waveLength_det.clear();
    opPhoton_time_det.clear();
    opPhoton_trackLength_det.clear();
  }
}
