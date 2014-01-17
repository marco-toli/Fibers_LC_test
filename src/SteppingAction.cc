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
    if (processName == "Cerenkov") CreateTree::Instance()->Tot_phot_cer += 1;
    if (processName == "Scintillation") CreateTree::Instance()->Tot_phot_scint += 1;
    
    if( //(CreateTree::Instance()->Timing() && processName != "Cerenkov") ||
        (!CreateTree::Instance()->Timing()) )
    theTrack->SetTrackStatus(fKillTrackAndSecondaries);
    
    // do this cycle only IF info on optical photons is needed
    if (CreateTree::Instance()->OpPhotons())
    {
      //---------------------------------------------------------
      // storing time, energy and position of all optical photons

//       cout << " tracing optical photons ... " << endl;
      
      G4int nStep = theTrack -> GetCurrentStepNumber();
      
      if( (theTrack->GetLogicalVolumeAtVertex()->GetName() == "EE_log") && (nStep == 1) )
      {
//  	cout << "storing optical photons informations ... " << endl;
//         G4String processName = theTrack->GetCreatorProcess()->GetProcessName();
//         if     ( processName == "Cerenkov" )      CreateTree::Instance()->opPhoton_process.push_back( +1 );
//         else if( processName == "Scintillation" ) CreateTree::Instance()->opPhoton_process.push_back( +2 );
//         else                                      CreateTree::Instance()->opPhoton_process.push_back( -1 );

        for (int iCrystal = 0; iCrystal < 25; iCrystal++) {
	  char name1[50];
	  sprintf(name1, "EE_phys_%d", iCrystal);
	  char name2[50];
	  sprintf(name2, "Grease_rear_%d", iCrystal);
	  
	  if (theTrack->GetVolume()->GetName() == name1) CreateTree::Instance()->opPhoton_n[iCrystal] ++;
	  if (thePrePVName == name1 && thePostPVName == name2 ) CreateTree::Instance()->opPhoton_n_ext[iCrystal] ++;
	}
//         CreateTree::Instance()->opPhoton_waveLength.push_back( MyMaterials::fromEvToNm(theTrack->GetTotalEnergy()/eV) );
//         CreateTree::Instance()->opPhoton_time.push_back( thePrePoint->GetGlobalTime()/nanosecond );
//         CreateTree::Instance()->opPhoton_vertexX.push_back( thePrePoint->GetPosition().x()/cm );
//         CreateTree::Instance()->opPhoton_vertexY.push_back( thePrePoint->GetPosition().y()/cm );
//         CreateTree::Instance()->opPhoton_vertexZ.push_back( thePrePoint->GetPosition().z()/cm );
//         CreateTree::Instance()->opPhoton_pX.push_back( theTrack->GetVertexMomentumDirection().x() );
//         CreateTree::Instance()->opPhoton_pY.push_back( theTrack->GetVertexMomentumDirection().y() );
//         CreateTree::Instance()->opPhoton_pZ.push_back( theTrack->GetVertexMomentumDirection().z() );
      }
      
      
      //----------------------------------------------
      // storing time, energy and position at detector
// 
//      cout << " thePreLVName = " << thePreLVName << endl;
      if(thePreLVName == "Det_layer_log") {// && thePostPVNamee() == "Det_log" ){
//       if(thePreLVName == "EE_log") {
       
      for (int iCrystal = 0; iCrystal < 25; iCrystal++) {
	 char name1[50];
	 char name2[50];
	 sprintf(name1, "Det_layer_rear_%d", iCrystal);
	 sprintf(name2, "Det_rear_%d", iCrystal);

// 	 cout << " thePrePVName = " << thePrePVName << " :: thePostPVName = " << thePostPVName << endl;
	 if (thePrePVName == name1 && thePostPVName == name2) {	// detected at photodetector iCrystal

	  CreateTree::Instance()->opPhoton_n_det[iCrystal]++;
//         if( theTrack->GetCreatorProcess() )
//         {
//           G4String processName = theTrack->GetCreatorProcess()->GetProcessName();
//           if     ( processName == "Cerenkov" )      CreateTree::Instance()->opPhoton_process_det.push_back( +1 );
//           else if( processName == "Scintillation" ) CreateTree::Instance()->opPhoton_process_det.push_back( +2 );
//           else                                      CreateTree::Instance()->opPhoton_process_det.push_back( -1 );
//         }
        
	if (iCrystal == 12) CreateTree::Instance()->opPhoton_n_det_central++;
	if (iCrystal == 6  || iCrystal == 7  || iCrystal == 8  || iCrystal == 11 || iCrystal == 12 || iCrystal == 13 || iCrystal == 16 || iCrystal == 17 || iCrystal == 18 ) CreateTree::Instance()->opPhoton_n_det_3x3 ++;
	
	CreateTree::Instance()->opPhoton_n_det_5x5 ++;
	CreateTree::Instance()->opPhoton_waveLength_det.push_back(MyMaterials::fromEvToNm(theTrack->GetTotalEnergy()/eV) );
	CreateTree::Instance()->opPhoton_time_det.push_back( theTrack->GetGlobalTime()/nanosecond );
	CreateTree::Instance()->opPhoton_trackLength_det.push_back( theTrack->GetTrackLength()/m );

	 }//if in iCrystal
       }// cycle over crystals
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
    float ion_energy = theStep->GetTotalEnergyDeposit()/GeV - theStep->GetNonIonizingEnergyDeposit()/GeV;
    float nonion_energy = theStep->GetNonIonizingEnergyDeposit()/GeV;
    
    if( delta > 0 )
    {	
//       CreateTree::Instance()->Total_delta_world         += delta;
      CreateTree::Instance()->Total_energy_world        += energy;
      CreateTree::Instance()->Total_ion_energy_world    += ion_energy;
      CreateTree::Instance()->Total_nonion_energy_world += nonion_energy;
      
      
      if( thePrePVLogName == "EE_log" )
      {
//         CreateTree::Instance()->Total_delta_5x5         += delta;
        CreateTree::Instance()->Total_energy_5x5        += energy;
        CreateTree::Instance()->Total_ion_energy_5x5    += ion_energy;
        CreateTree::Instance()->Total_nonion_energy_5x5 += nonion_energy;
      
	for (int iCrystal = 0; iCrystal < 25; iCrystal++) {
	  char name[20];
	  sprintf(name, "EE_phys_%d", iCrystal);
	  if (thePrePV->GetName() == name) {
	    CreateTree::Instance()->Total_energy[iCrystal] += energy;	
	    CreateTree::Instance()->Total_nonion_energy[iCrystal] += nonion_energy;	               
	  }
	}
	
      }
      
      if( CreateTree::Instance()->Pos_fiber() )
      {
        G4ThreeVector pos = thePostPoint -> GetPosition();	
        CreateTree::Instance() -> depositionX.push_back(pos[0]);		
        CreateTree::Instance() -> depositionY.push_back(pos[1]);
        CreateTree::Instance() -> depositionZ.push_back(pos[2]);
        CreateTree::Instance() -> Energy_deposited.push_back(energy);
// 	cout << " depositing ... ("  << pos[0] << "," << pos[1] << "," << pos[2] << ") :: energy = " << energy << endl;
      }
    }  
  } // non optical photon
  
}
