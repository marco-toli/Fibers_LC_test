#include "SteppingAction.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "EventAction.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4UnitsTable.hh"
#include "CreateTree.hh"
#include "MyMaterials.hh"

#include <iostream>
#include <fstream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TF1.h"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "globals.hh"


using namespace std;
using namespace CLHEP;

SteppingAction::SteppingAction()
{}



SteppingAction::~SteppingAction()
{}



void SteppingAction::UserSteppingAction(const G4Step * theStep)
{
  G4Track* theTrack = theStep->GetTrack();
  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  
  
  G4StepPoint* thePrePoint  = theStep->GetPreStepPoint();
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume* thePrePV  = thePrePoint ->GetPhysicalVolume();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
  G4String thePrePVName  = ""; if( thePrePV )  thePrePVName  = thePrePV  -> GetName();
  G4String thePostPVName = ""; if( thePostPV ) thePostPVName = thePostPV -> GetName();

   G4LogicalVolume* thePreLV  = thePrePV ->GetLogicalVolume();
   G4String thePreLVName  = ""; thePreLVName  = thePreLV  -> GetName();



//    cout << "defining step volumes ... " << endl;
    

  // optical photon
  if( particleType == G4OpticalPhoton::OpticalPhotonDefinition() )
  {
//      cout << "optical photon ! " << endl;
    G4String processName = " "; if (theTrack->GetCreatorProcess())  processName = theTrack->GetCreatorProcess()->GetProcessName();
//     if (processName == "Cerenkov") CreateTree::Instance()->Tot_phot_cer += 1;
//     if (processName == "Scintillation") CreateTree::Instance()->Tot_phot_scint += 1;
    if( !CreateTree::Instance()->Timing() || processName == "Cerenkov")
      theTrack->SetTrackStatus(fKillTrackAndSecondaries);
    
    // do this cycle only IF info on optical photons is needed
    if (CreateTree::Instance()->OpPhotons() && processName != "Cerenkov")
    {
      //---------------------------------------------------------
      // storing time, energy and position of all optical photons

//       cout << " tracing optical photons ... " << endl;
      
      G4int nStep = theTrack -> GetCurrentStepNumber();
      
      // produced photons
      // 	  if (thePostPVName == name2 ) cout << "pre PV name = " << thePrePVName << " :: the post PV = " << thePostPVName  << endl;
      if (theTrack->GetVolume()->GetName() == "Fiber_phys" && (nStep == 1)) {//
	CreateTree::Instance()->opPhoton_n ++;
	if (processName == "Cerenkov") CreateTree::Instance()->opPhoton_n_type.push_back(0);
	if (processName == "Scintillation") CreateTree::Instance()->opPhoton_n_type.push_back(1);
	CreateTree::Instance()->opPhoton_time.push_back(theTrack->GetGlobalTime()/nanosecond);
//	cout << "created photon"  << endl;
      }
      
      // extracted photons
      if (thePrePVName == "Grease_phys_layer" && thePostPVName == "Grease_phys" ) {
	CreateTree::Instance()->opPhoton_n_ext ++;
	if (processName == "Cherenkov")     CreateTree::Instance()->opPhoton_n_ext_type.push_back(0);
	if (processName == "Scintillation") CreateTree::Instance()->opPhoton_n_ext_type.push_back(1);
//	cout << "photon in grease"  << endl;
      }
      
      //----------------------------------------------
      // detected photons
      if (thePrePVName == "Win_phys_layer" && thePostPVName == "Win_phys") {	// detected at photodetector iCrystal

	CreateTree::Instance()->opPhoton_n_det++;
	if (processName == "Cherenkov") CreateTree::Instance()->opPhoton_n_det_type.push_back(0);
	if (processName == "Scintillation") CreateTree::Instance()->opPhoton_n_det_type.push_back(1);//
	CreateTree::Instance()->opPhoton_waveLength_det.push_back(MyMaterials::fromEvToNm(theTrack->GetTotalEnergy()/eV) );//
	CreateTree::Instance()->opPhoton_time_det.push_back( theTrack->GetGlobalTime()/nanosecond );
	CreateTree::Instance()->opPhoton_trackLength_det.push_back( theTrack->GetTrackLength()/m );
//	cout << "photon in quartz"  << endl;
        theTrack->SetTrackStatus(fKillTrackAndSecondaries);
	
      }//definition of detected photon
      
    } // do this cycle only IF info on optical photons is needed
  } // optical photon
  
  
  // non optical photon
  else
  {
    G4TouchableHandle thePreTouch = thePrePoint -> GetTouchableHandle();
    G4VPhysicalVolume* thePreVolume = thePreTouch -> GetVolume();
    G4LogicalVolume* thePrePVLog = thePreVolume -> GetLogicalVolume();
    G4String thePrePVLogName = thePrePVLog -> GetName();
    
    
    float delta = thePrePoint->GetTotalEnergy()/MeV - thePostPoint->GetTotalEnergy()/MeV;
    float energy = theStep->GetTotalEnergyDeposit()/MeV;
    float time_dep =  theTrack->GetGlobalTime()/nanosecond ;
    
    float ion_energy = theStep->GetTotalEnergyDeposit()/MeV - theStep->GetNonIonizingEnergyDeposit()/MeV;
    
  
    if( delta > 0)
    {	   

      CreateTree::Instance()->Total_energy_world        += energy;
      CreateTree::Instance()->Total_ion_energy_world    += ion_energy;      

      if (thePrePVLogName == "Fiber_log") 
      {
         CreateTree::Instance()->Total_energy_fib        += energy;
         CreateTree::Instance()->Total_ion_energy_fib    += ion_energy;      
      }
      else if (thePrePVLogName == "Absorber_L") 
      {
         CreateTree::Instance()->Total_energy_abs        += energy;
         CreateTree::Instance()->Total_ion_energy_abs    += ion_energy;      
      }
      
      if( CreateTree::Instance()->Pos_fiber() )
      {
        G4ThreeVector pos = thePostPoint -> GetPosition();        	
        CreateTree::Instance() -> depositionX.push_back(pos[0]);		
        CreateTree::Instance() -> depositionY.push_back(pos[1]);
        CreateTree::Instance() -> depositionZ.push_back(pos[2]);
        CreateTree::Instance() -> Energy_deposited.push_back(energy);
	CreateTree::Instance() -> Time_deposit.push_back(time_dep);
// 	cout << " depositing ... ("  << pos[0] << "," << pos[1] << "," << pos[2] << ") :: energy = " << energy << endl;
      }
    }
  } // non optical photon
  
}
