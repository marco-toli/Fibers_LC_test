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
   G4LogicalVolume* thePostLV = thePostPV->GetLogicalVolume();
   G4String thePreLVName  = ""; thePreLVName  = thePreLV  -> GetName();
//    G4String thePostLVName = ""; if( thePostLV ) thePostLVName = thePostLV -> GetName();


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
      }
      
      // extracted photons
      if (thePrePVName == "Fiber_phys" && thePostPVName == "Grease_phys" ) {
	CreateTree::Instance()->opPhoton_n_ext ++;
	if (processName == "Cherenkov") CreateTree::Instance()->opPhoton_n_ext_type.push_back(0);
	if (processName == "Scintillation") CreateTree::Instance()->opPhoton_n_ext_type.push_back(1);
      }
      
      //----------------------------------------------
      // detected photons
      if (thePrePVName == "Det_layer_phys" && thePostPVName == "Det_phys") {	// detected at photodetector iCrystal

	CreateTree::Instance()->opPhoton_n_det++;
	if (processName == "Cherenkov") CreateTree::Instance()->opPhoton_n_det_type.push_back(0);
	if (processName == "Scintillation") CreateTree::Instance()->opPhoton_n_det_type.push_back(1);
	CreateTree::Instance()->opPhoton_waveLength_det.push_back(MyMaterials::fromEvToNm(theTrack->GetTotalEnergy()/eV) );
	CreateTree::Instance()->opPhoton_time_det.push_back( theTrack->GetGlobalTime()/nanosecond );
	CreateTree::Instance()->opPhoton_trackLength_det.push_back( theTrack->GetTrackLength()/m );
	
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
    
    
    float delta = thePrePoint->GetTotalEnergy()/GeV - thePostPoint->GetTotalEnergy()/GeV;
    float energy = theStep->GetTotalEnergyDeposit()/GeV;
    float time_dep =  theTrack->GetGlobalTime()/nanosecond ;
    
    float ion_energy = theStep->GetTotalEnergyDeposit()/GeV - theStep->GetNonIonizingEnergyDeposit()/GeV;
    float nonion_energy = theStep->GetNonIonizingEnergyDeposit()/GeV;
    
    if( delta > 0  && thePrePVLogName == "Fiber_log")
    {	   

      G4ThreeVector pos = thePostPoint -> GetPosition();
      CreateTree::Instance()->Total_energy      += energy;
      CreateTree::Instance()->Total_ion_energy    += ion_energy;
      
     // define parameter for effective attenuation curve along z (crystal length)
     
     
//      cout << " att coeff = " <<  fAtt(par0, par1, par2, pos[2])/par0 << endl;
     for (int iAtt = 0; iAtt < nATT; iAtt++) {
	 CreateTree::Instance()->Total_ion_energy_att[iAtt]    += ion_energy* fAtt(par0[iAtt], par1[iAtt], par2[iAtt], pos[2] + 110);// if normalization / fAtt(par0[iAtt], par1[iAtt], par2[iAtt], 220);
     }
     

      
      if( CreateTree::Instance()->Pos_fiber() )
      {
        	
        CreateTree::Instance() -> depositionX.push_back(pos[0]);		
        CreateTree::Instance() -> depositionY.push_back(pos[1]);
        CreateTree::Instance() -> depositionZ.push_back(pos[2]);
        CreateTree::Instance() -> Energy_deposited.push_back(energy);
	CreateTree::Instance() -> Time_deposit.push_back(time_dep);
// 	cout << " depositing ... ("  << pos[0] << "," << pos[1] << "," << pos[2] << ") :: energy = " << energy << endl;
      }
    }  //inside crystal
  } // non optical photon
  
}
