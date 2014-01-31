{


  #include <cstdlib>
//   #include <iostream>
  #include <fstream>
  #include <stdio.h>
  #include <string.h>
  #include <vector>
  #include <math.h>
  #include <TH1F.h>

  using namespace std;

  gROOT->Reset();
  gROOT->SetStyle("Plain");

  // DEFINING FILE AND TREE VARIABLES

  const int NENERGIES= 1;
  const int nATT = 10;
  
  TFile * RunFile[NENERGIES];
  

  RunFile[0] = new TFile("data/1GeV_test_Attenuation_curves.root","READ");   //
//   RunFile[1] = new TFile("data/50GeV_full_sim_1mind.root","READ");   //
//   RunFile[2] = new TFile("data/50GeV_full_sim_2mind.root","READ");   //
//   RunFile[3] = new TFile("data/50GeV_full_sim_3mind.root","READ");   //
//   RunFile[4] = new TFile("data/50GeV_full_sim_5mind.root","READ");   //  
//   RunFile[5] = new TFile("data/50GeV_full_sim_7mind.root","READ");   //
  
  double input_energy[NENERGIES];
  input_energy[0] = 1;
//   input_energy[1] = 20;
//   input_energy[2] = 50;
//   input_energy[3] = 100;
//   input_energy[4] = 120;
//   input_energy[5] = 150;
  
  double input_att[nATT];
  input_att[0] = 0.117681;
  input_att[1] = 0.0637243;
  input_att[2] = 0.0439325;
  input_att[3] = 0.0321204;
  input_att[4] = 0.0192074;
  input_att[5] = 0.0124216;
  input_att[6] = 0.00660631;
  input_att[7] = 0.00431397;
  input_att[8] = 0.00229227;
  input_att[9] = 0.000864042;
  
//   input_att[10] = 0.001;
  
  cout << "almeno fin qui ..." << endl;
  
  TGraphErrors * grResolution = new TGraphErrors ();
  TGraphErrors * grLinearity = new TGraphErrors ();
  
  TGraphErrors * grResolutionAtt [nATT];
  TGraphErrors * grLinearityAtt [nATT];
  
  for (int iAtt = 0; iAtt < nATT; iAtt++) {
    grResolutionAtt[iAtt] = new TGraphErrors ();
    grLinearityAtt[iAtt] = new TGraphErrors ();	
    
  }
  
  cout << "almeno fin qui ..." << endl;
  
  TH1F * hDepoEn[NENERGIES];
  TH1F * hAttEn[NENERGIES][nATT];
  TH1F * hAttCalib_En[NENERGIES][nATT];
  
  TH1F * hInputAbsorption[NENERGIES];
  
  int NBINS = 1000;
  char name[50];
  
  for (int iEn = 0; iEn < NENERGIES; iEn ++) {  
    
    sprintf (name, "hDepoEn_%d", iEn);
    hDepoEn [iEn] = new TH1F (name, name, NBINS, 0.3*input_energy[iEn], input_energy[iEn]);
    
    for (int iAtt = 0; iAtt < nATT; iAtt++){
      sprintf (name, "hAttEn_%d_ATT_%d", iEn, iAtt);
//       hAttEn[iEn][iAtt] = new TH1F (name, name, NBINS, 0.3*input_energy[iEn]*input_att[iAtt], input_energy[iEn]*input_att[iAtt]);
      hAttEn[iEn][iAtt] = new TH1F (name, name, NBINS, 0, input_energy[iEn]*input_att[iAtt]*2);
      
      sprintf (name, "hAttCalib_En_%d_ATT_%d", iEn, iAtt);
      hAttCalib_En[iEn][iAtt] = new TH1F (name, name, NBINS, 0.3*input_energy[iEn], input_energy[iEn]*2);
    }
  }
  
  /// defining tree variables
  
    Float_t InitialPositionX;
    Float_t InitialPositionY;
    Float_t InitialPositionZ;
  
    Float_t Total_energy ;
    Float_t Total_ion_energy ;
    Float_t Total_ion_energy_att[nATT] ;


  ///******************************************///
  /// Run over different beam position
  ///******************************************///

  for (int iEn = 0; iEn < NENERGIES; iEn++) {


    TTree* TreeRun = (TTree*) RunFile[iEn].Get("tree");
    hInputAbsorption[iEn] = (TH1F*) RunFile[iEn].Get("hAttenuation");

    TreeRun->SetBranchAddress("InitialPositionX",&InitialPositionX);
    TreeRun->SetBranchAddress("InitialPositionY",&InitialPositionY);
    TreeRun->SetBranchAddress("InitialPositionZ",&InitialPositionZ);

    TreeRun->SetBranchAddress("Total_energy",&Total_energy);
    TreeRun->SetBranchAddress("Total_ion_energy",&Total_ion_energy);
    TreeRun->SetBranchAddress("Total_ion_energy_att",&Total_ion_energy_att);


      ///******************************************///
      ///		 Run over events	    ///
      ///******************************************///

      int NEVENTS = TreeRun->GetEntries();
      cout << "iEn = " << iEn << " :: nEvents = " << NEVENTS << endl;
      
      for (Int_t iEvt= 0; iEvt < NEVENTS; iEvt++) {//&& iEvt < 5

	TreeRun->GetEntry(iEvt);
	cout << " iEvt = " << iEvt << endl;
	hDepoEn[iEn]->Fill(Total_ion_energy);
	
	for (int iAtt = 0; iAtt < nATT; iAtt++){
	  cout << "input_att[" << iAtt << "] = " << Total_ion_energy_att[iAtt]/0.84 <<";" << endl;
	  hAttEn[iEn][iAtt]->Fill(Total_ion_energy_att[iAtt]);
	}
	
      }	   //end of events loop
      
      
/*
    TF1 * fEnergies = new TF1 ("fEnergies", " [0]*exp((x-220)/[1]) + [2]", 0, 220);
    
    fEnergies->SetParameters(0.1, 20, 0.01);
//     fEnergies->FixParameter(1,220);
    fEnergies->SetLineColor(iEn+1);
    for (int j=0; j< 10; j++) hEfficiency[iEn]->Fit("fEnergies", "QR");
    
    
    TF1 * fDecayTime = new TF1 ("fDecayTime", " [0]*exp(-x/[1])", 0, 100);
    fDecayTime->SetParameters(100, 20);
    fDecayTime->SetLineColor(iEn+1);
    for (int j = 0; j< 10; j++) hPulseShape[iEn]->Fit("fDecayTime", "QR");
  
    */
    
    hInputAbsorption[iEn]->SetLineColor(iEn+1);
    hDepoEn[iEn]->SetLineColor(iEn+1);
    for (int iAtt = 0; iAtt < nATT; iAtt++) hAttEn[iEn][iAtt]->SetLineColor(iEn+1);
    
    
//     if (iEn == 0) double initialLY = hLightOutput[0]->GetMean();
    grResolution->SetPoint(iEn, input_energy[iEn], hDepoEn[iEn]->GetRMS()/hDepoEn[iEn]->GetMean());
    grLinearity->SetPoint(iEn, input_energy[iEn], hDepoEn[iEn]->GetMean()/input_energy[iEn]);
    
    for (int iAtt = 0; iAtt < nATT; iAtt++){
      grResolutionAtt[iAtt]->SetPoint(iEn, input_energy[iEn], hAttEn[iEn][iAtt]->GetRMS()/hAttEn[iEn][iAtt]->GetMean());
//       grResolution[iAtt]->SetPointError(iEn, 0, hLightOutput[iEn]->GetMeanError());
      grLinearityAtt[iAtt]->SetPoint(iEn, input_energy[iEn], hAttEn[iEn][iAtt]->GetMean()/input_energy[iEn]);
    }
    
   }  //end of energy loop
    

  /// ********* DRAWING ***************** ///

    TCanvas * cInputAbsorption = new TCanvas ("cInputAbsorption", "cInputAbsorption", 600, 600);
    hInputAbsorption[0]->Draw();
    hInputAbsorption[0]->GetXaxis()->SetTitle("Wavelength (nm)");
    hInputAbsorption[0]->GetYaxis()->SetTitle("Absorption Length (mm)");
    for (int iEn = 1; iEn < NENERGIES; iEn++){
      hInputAbsorption[iEn]->Draw("same");
    }
    gPad->SetGrid();
    
    
    TCanvas * cDepoEn = new TCanvas ("cDepoEn", "cDepoEn", 600, 600);
    hDepoEn[0]->Draw();
    hDepoEn[0]->GetXaxis()->SetTitle("Deposited Energy (GeV)");
    hDepoEn[0]->GetYaxis()->SetTitle("Counts");
    for (int iEn = 1; iEn < NENERGIES; iEn++){
      hDepoEn[iEn]->Draw("same");
    }
    gPad->SetGrid();
    
    
    int selEn = 0;
    TCanvas * cAttEn = new TCanvas ("cAttEn", "cAttEn", 600, 600);
//     cAttEn->Divide(2,3);
    hAttEn[selEn][0]->Draw();
    hAttEn[selEn][0]->GetXaxis()->SetTitle("Measured Energy (GeV)");
    hAttEn[selEn][0]->GetYaxis()->SetTitle("Counts");
    
    for (int iAtt = 1; iAtt < nATT; iAtt++){
      hAttEn[selEn][iAtt]->Draw("same");
    }
    gPad->SetGrid();
    
    
    TCanvas * cResolution = new TCanvas ("cResolution", "cResolution", 600, 600);
    grResolution->SetMarkerStyle (21);
    grResolution->GetXaxis()->SetTitle("Energy (GeV)");
    grResolution->GetYaxis()->SetTitle("#sigma_{E}/E");
    grResolution->Draw("ALPE");
    gPad->SetGrid();

    TCanvas * cLinearity = new TCanvas ("cLinearity", "cLinearity", 600, 600);
    grLinearity->SetMarkerStyle (21);
    grLinearity->GetXaxis()->SetTitle("Energy (GeV)");
    grLinearity->GetYaxis()->SetTitle("Linearity");
    grLinearity->GetXaxis()->SetRangeUser(0, 12);
    grLinearity->GetYaxis()->SetRangeUser(0, 11);
    grLinearity->Draw("ALPE");
    gPad->SetGrid();

}


