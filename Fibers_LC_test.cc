//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: exampleN06.cc,v 1.18 2010-10-23 19:33:55 gum Exp $
// GEANT4 tag $Name: not supported by cvs2svn $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// Description: Test of Continuous Process G4Cerenkov
//              and RestDiscrete Process G4Scintillation
//              -- Generation Cerenkov Photons --
//              -- Generation Scintillation Photons --
//              -- Transport of optical Photons --
// Version:     5.0
// Created:     1996-04-30
// Author:      Juliet Armstrong
// mail:        gum@triumf.ca
//     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "TString.h"
#include "TRandom3.h"
#include "TCint.h"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4PhysListFactory.hh"
#include "G4EmUserPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4VModularPhysicsList.hh"

#include "LHEP.hh"
#include "QGSP_BERT.hh"

#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "TrackingAction.hh"
#include "SteppingAction.hh"
#include "EventAction.hh"
#include "SteppingVerbose.hh"
#include "CreateTree.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif



long int CreateSeed();



int main(int argc,char** argv)
{
  gInterpreter -> GenerateDictionary("vector<float>","vector");
  
  
  if (argc != 4 && argc != 3)
  {
    cout << "Syntax for exec: crystal <configuration file> <gps file> <output file>" << endl;
    cout << "Syntax for viz:  crystal <configuration file> <gps file>" << endl;
    return 0;
  }
  
  string file;
  string filename;
  
  TFile* outfile = NULL;
  
  if(argc == 4) 
  {
    cout << "Starting exec mode..." << endl; 
    file = argv[3];
    filename = file + ".root";
    G4cout << "Writing data to file '" << filename << "' ..." << G4endl;
    
    outfile = new TFile((TString)filename,"RECREATE");
    outfile -> cd();
  }
  
  if (argc == 3)
  {
    cout << "Starting viz mode..." << endl; 
  }
  
  
  
  cout<<"\n"<<endl;
  cout<<"######################################################"<<endl;  
  cout<<"#                                                    #"<<endl;
  cout<<"#  GEANT4 simulation of sampling calorimeter         #"<<endl;  
  cout<<"#         based on LuAG crystal fibers.              #"<<endl;  
  cout<<"#  Author: Marco Lucchini, CERN, 2013                #"<<endl;
  cout<<"#                                                    #"<<endl;  
  cout<<"######################################################"<<endl;
  cout<<"\n\n"<<endl;
  cout<<"=====>   C O N F I G U R A T I O N   <====\n"<<endl;
  
  G4cout << "Configuration file: '" << argv[1] << "'" << G4endl;
  ConfigFile config(argv[1]);

  char gpsfile[100];
  strcpy (gpsfile, argv[2]);
  G4cout << "GPS file: '" << gpsfile << G4endl;
  
  char executeGPS [100];
  strcpy (executeGPS, "/control/execute ");
  strcat (executeGPS, argv[2]);
  
  /// initialize attenuation function parameters
  
  
  ifstream fp_Att_func("att_input_parameters.txt");
  for (int iAtt = 0; iAtt < nATT; iAtt++) fp_Att_func >> par0[iAtt] >> par1[iAtt] >> par2[iAtt];
  
  
  // Seed the random number generator manually
  //
  G4long myseed = config.read<long int>("seed");
  if( myseed == -1 )
  {
    G4cout << "Creating random seed..." << G4endl;
    myseed = CreateSeed();
  }
  G4cout << "Random seed : " << myseed << G4endl;
  CLHEP::HepRandom::setTheSeed(myseed);
  
  
  G4bool energy_data = 1;
  G4bool init_data   = 1;
  G4bool pos_fiber   = 0;
  G4bool opPhotons   = 1;
  G4bool timing      = 1;
  
  CreateTree* mytree = new CreateTree("tree", energy_data, init_data, pos_fiber, opPhotons, timing);
  
  
  // User Verbose output class
  //
  G4VSteppingVerbose* verbosity = new SteppingVerbose;
  G4VSteppingVerbose::SetInstance(verbosity);
  
  // Run manager
  //
  G4RunManager* runManager = new G4RunManager;
  
  
  //Physics list defined using PhysListFactory
  //
  std::string physName("");
  
  G4PhysListFactory factory;
  const std::vector<G4String>& names = factory.AvailablePhysLists();
  for(unsigned n = 0; n != names.size(); n++)
    G4cout << "PhysicsList: " << names[n] << G4endl;
  
  if( physName == "")
  {
    char* path = getenv("PHYSLIST");
    if( path ) physName = G4String(path);
  }
  
  if ( physName == "" || factory.IsReferencePhysList(physName))
  {
    physName = "FTFP_BERT";
  }
  
  std::cout << "Using physics list: " << physName << std::endl; 
  
  
  // UserInitialization classes - mandatory
  //
  
  G4cout << ">>> Define physics list::begin <<<" << G4endl; 
  G4VModularPhysicsList* physics = factory.GetReferencePhysList(physName);
  physics->RegisterPhysics(new G4EmUserPhysics(0));
  runManager-> SetUserInitialization(physics);
  G4cout << ">>> Define physics list::end <<<" << G4endl; 
  
  G4cout << ">>> Define DetectorConstruction::begin <<<" << G4endl; 
  DetectorConstruction* detector = new DetectorConstruction(argv[1]);
  runManager-> SetUserInitialization(detector);
  G4cout << ">>> Define DetectorConstruction::end <<<" << G4endl; 
  
  G4cout << ">>> Define PrimaryGeneratorAction::begin <<<" << G4endl; 
  G4ThreeVector posCentre(0.*m,0.*m,-1.*(detector->GetAbsorber_z()/m)/2.*m);
  G4VUserPrimaryGeneratorAction* gen_action = new PrimaryGeneratorAction(posCentre);
  runManager->SetUserAction(gen_action);
  G4cout << ">>> Define PrimaryGeneratorAction::end <<<" << G4endl; 
  
  // UserAction classes
  //
  
  G4cout << ">>> Define RunAction::begin <<<" << G4endl; 
  G4UserRunAction* run_action = new RunAction;
  runManager->SetUserAction(run_action);  
  G4cout << ">>> Define RunAction::end <<<" << G4endl; 
  
  G4cout << ">>> Define EventAction::begin <<<" << G4endl; 
  G4UserEventAction* event_action = new EventAction;
  runManager->SetUserAction(event_action);
  G4cout << ">>> Define EventAction::end <<<" << G4endl; 
  
  G4cout << ">>> Define TrackingAction::begin <<<" << G4endl; 
  TrackingAction* tracking_action = new TrackingAction;
  runManager->SetUserAction(tracking_action); 
  G4cout << ">>> Define TrackingAction::end <<<" << G4endl;
  
  G4cout << ">>> Define SteppingAction::begin <<<" << G4endl; 
  SteppingAction* stepping_action = new SteppingAction;
  runManager->SetUserAction(stepping_action); 
  G4cout << ">>> Define SteppingAction::end <<<" << G4endl;
  
  
  if (argc == 3)   // Define UI session for interactive mode
  {   
    // Initialize G4 kernel
    //
    runManager -> Initialize();
    
    #ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive;
    visManager -> Initialize();
    #endif
    
    G4UImanager* UImanager = G4UImanager::GetUIpointer(); 
    #ifdef G4UI_USE
    G4UIExecutive * ui = new G4UIExecutive(argc,argv);
    #ifdef G4VIS_USE
    
    UImanager -> ApplyCommand(executeGPS);
    UImanager -> ApplyCommand("/control/execute vis.mac");     
    #endif
    ui -> SessionStart();
    delete ui;
    #endif 
   
    #ifdef G4VIS_USE
    delete visManager;
    #endif  
  }
  else
  {
    runManager -> Initialize();
    G4UImanager* UImanager = G4UImanager::GetUIpointer(); 
    
//     UImanager -> ApplyCommand("/control/execute gps.mac");
    UImanager -> ApplyCommand(executeGPS);
  } 
  
  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not
  // be deleted in the main() program !
  
  delete runManager;
  delete verbosity;
  
  if(argc == 4) 
  {
    G4cout << "Writing tree to file " << filename << " ..." << G4endl;
    
    mytree -> GetTree() -> Write();
    outfile -> Close();
  }
  
  return 0;
}



long int CreateSeed()
{
  TRandom3 rangen;
  long int s = time(0);
  cout<<"Time : "<<s<<endl;
  s+=getpid();
  cout<<"PID  : "<<getpid()<<endl;
  
  FILE * fp = fopen ("/proc/uptime", "r");
  int uptime,upsecs;
  if (fp != NULL)
  {
    char buf[BUFSIZ];
    int res;
    char *b = fgets (buf, BUFSIZ, fp);
    if (b == buf)
    {
      /* The following sscanf must use the C locale.  */
      setlocale (LC_NUMERIC, "C");
      res = sscanf (buf, "%i", &upsecs);
      setlocale (LC_NUMERIC, "");
      if (res == 1) uptime = (time_t) upsecs;
    }
    fclose (fp);
  }
  
  cout<<"Uptime: "<<upsecs<<endl;
  s+=upsecs;
  
  cout<<"Seed for srand: "<<s<<endl;
  srand(s);
  rangen.SetSeed(rand());
  long int seed = round(1000000*rangen.Uniform());
  return seed;
}
