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

  const int nAtt= 11;
  TFile * RunFile[nAtt];
  cout << "almeno fin qui ..." << endl;


  RunFile[0] = new TFile("data/50GeV_full_sim_0mind.root","READ");   //
  RunFile[1] = new TFile("data/50GeV_full_sim_1mind.root","READ");   //
  RunFile[2] = new TFile("data/50GeV_full_sim_2mind.root","READ");   //
  RunFile[3] = new TFile("data/50GeV_full_sim_3mind.root","READ");   //
  RunFile[4] = new TFile("data/50GeV_full_sim_5mind.root","READ");   //
  
  RunFile[5] = new TFile("data/50GeV_full_sim_7mind.root","READ");   //
  RunFile[6] = new TFile("data/50GeV_full_sim_10mind.root","READ");   //
  RunFile[7] = new TFile("data/50GeV_full_sim_12mind.root","READ");   //
  RunFile[8] = new TFile("data/50GeV_full_sim_15mind.root","READ");   //
  RunFile[9] = new TFile("data/50GeV_full_sim_20mind.root","READ");   //
  RunFile[10] = new TFile("data/50GeV_full_sim_30mind.root","READ");   //
   
  int NBINS = 220;
  double maxZ = 220;
  double maxX = 14;
  
  TGraphErrors * grLO = new TGraphErrors ();
  TGraphErrors * grLO_RMS = new TGraphErrors ();
  
  TH1F * hLongitudinalEff[nAtt];
  TH1F * hLongitudinalInEff[nAtt];
  TH1F * hEfficiency[nAtt]; 
  
  TH1F * hLightOutput[nAtt];
  TH1F * hTranverseXInEff[nAtt];
  TH1F * hEfficiencyX[nAtt];
  TH1F * hPulseShape[nAtt];
  TH1F * hPulseShapePMT[nAtt];
  
  TProfile * pTOF [nAtt];
  TH1F * hInputAbsorption[nAtt];
  
  for (int iAtt = 0; iAtt < nAtt; iAtt ++) {
    char name[50];
//     sprintf (name, "hInputAbsorption_%d", iAtt);
//     hInputAbsorption[iAtt] = new TH1F (name, name, 700, 200, 900);
  
    sprintf (name, "hLightOutput_%d", iAtt);
    hLightOutput[iAtt] = new TH1F (name, name, 1000000, 0, 100000);
    
    sprintf (name, "hTranverseXInEff_%d", iAtt);
    hTranverseXInEff[iAtt] = new TH1F (name, name, NBINS, -maxX, maxX);
    sprintf (name, "hEfficiencyX_%d", iAtt);
    hEfficiencyX [iAtt] = new TH1F (name, name, NBINS, -maxX, maxX);
    
    sprintf (name, "hPulseShape_%d", iAtt);
    hPulseShape [iAtt] = new TH1F (name, name, 500, -10, 100);
    sprintf (name, "hPulseShapePMT_%d", iAtt);
    hPulseShapePMT [iAtt] = new TH1F (name, name, 500, -10, 100);
    
    sprintf (name, "pTOF_%d", iAtt);
    pTOF [iAtt] = new TProfile (name, name, NBINS, 0, maxZ);
  }
  
  /// defining tree variables
  
    Float_t InitialPositionX;
    Float_t InitialPositionY;
    Float_t InitialPositionZ;
  
    Float_t Total_energy_5x5;
    Float_t Total_ion_energy_5x5;
    Float_t Tot_phot_cer;
    Float_t Tot_phot_scint;
  
    Float_t Total_energy ;
    Float_t Total_nonion_energy ;
    Int_t opPhoton_n ;
    Int_t opPhoton_n_ext ;
    Int_t opPhoton_n_det ;
    std::vector<float> * opPhoton_n_det_type ;
    std::vector<float> * opPhoton_time_det ;

  ///******************************************///
  /// Run over different beam position
  ///******************************************///

  for (int iAtt = 0; iAtt < nAtt; iAtt++) {


    TTree* TreeRun = (TTree*) RunFile[iAtt].Get("tree");
    hInputAbsorption[iAtt] = (TH1F*) RunFile[iAtt].Get("hAttenuation");

    TreeRun->SetBranchAddress("InitialPositionX",&InitialPositionX);
    TreeRun->SetBranchAddress("InitialPositionY",&InitialPositionY);
    TreeRun->SetBranchAddress("InitialPositionZ",&InitialPositionZ);
    
    TreeRun->SetBranchAddress("Tot_phot_cer",&Tot_phot_cer);
    TreeRun->SetBranchAddress("Tot_phot_scint",&Tot_phot_scint);
    TreeRun->SetBranchAddress("Total_energy",&Total_energy);
    
//     TreeRun->SetBranchAddress("opPhoton_n",&opPhoton_n);
//     TreeRun->SetBranchAddress("opPhoton_n_ext",&opPhoton_n_ext);
    TreeRun->SetBranchAddress("opPhoton_n_det",&opPhoton_n_det);
    TreeRun->SetBranchAddress("opPhoton_n_det_type",&opPhoton_n_det_type);
    TreeRun->SetBranchAddress("opPhoton_time_det",&opPhoton_time_det);
  

      ///******************************************///
      ///		 Run over events		///
      ///******************************************///

      int NEVENTS = TreeRun->GetEntries();
      cout << "iAtt = " << iAtt << " :: nEvents = " << NEVENTS << endl;
      for (Int_t iEvt= 0; iEvt < NEVENTS && iEvt<56; iEvt++) {//&& iEvt < 5

	TreeRun->GetEntry(iEvt);
// 	cout << " iEvt = " << iEvt << " :: photons = " << opPhoton_time_det.size() << endl; 	
	double scint_phot = 0;
// 	 cout << " opPhoton_time_det.size = " << opPhoton_time_det.size() << endl;
	 for (int i = 0; i< opPhoton_time_det.size(); i++) {
	   if (opPhoton_n_det_type.at(i) == 1) {
//    	    if (opPhoton_time_det.at(i)!=0) cout << "opPhoton_n_det_time = " << opPhoton_time_det.at(i) << endl;

	    hPulseShape[iAtt]->Fill(opPhoton_time_det.at(i));
	    hPulseShapePMT[iAtt]->Fill(gRandom->Gaus(opPhoton_time_det.at(i), 1.6));
	    scint_phot++;
	   }
 	  }
	
	hLightOutput[iAtt]->Fill(scint_phot);
// 	hPhotonsProd[iCrys]->Fill(opPhoton_n[iCrys]);
// 	hPhotonsExt[iCrys]->Fill(opPhoton_n_ext[iCrys]);
// 	hPhotonsDet[iCrys]->Fill(opPhoton_n_det[iCrys]);

      }	   //end of events loop
      
      
//       hEfficiency[iAtt]->Scale (1./hEfficiency[iAtt]->GetMaximum());
//       hEfficiencyX[iAtt]->Scale (1./hEfficiencyX[iAtt]->GetMaximum());
/*
    TF1 * fAtt = new TF1 ("fAtt", " [0]*exp((x-220)/[1]) + [2]", 0, 220);
    
    fAtt->SetParameters(0.1, 20, 0.01);
//     fAtt->FixParameter(1,220);
    fAtt->SetLineColor(iAtt+1);
    for (int j=0; j< 10; j++) hEfficiency[iAtt]->Fit("fAtt", "QR");
    
    
    TF1 * fDecayTime = new TF1 ("fDecayTime", " [0]*exp(-x/[1])", 0, 100);
    fDecayTime->SetParameters(100, 20);
    fDecayTime->SetLineColor(iAtt+1);
    for (int j = 0; j< 10; j++) hPulseShape[iAtt]->Fit("fDecayTime", "QR");
  
    */
    hPulseShape[iAtt]->Scale(1./hPulseShape[iAtt]->GetMaximum());
    hPulseShapePMT[iAtt]->Scale(1./hPulseShapePMT[iAtt]->GetMaximum());
    
    hInputAbsorption[iAtt]->SetLineColor(iAtt+1);
    hPulseShape[iAtt]->SetLineColor(iAtt+1);
    hPulseShapePMT[iAtt]->SetLineColor(iAtt+1);
    hLightOutput[iAtt]->SetLineColor(iAtt+1);
    
    if (iAtt == 0) double initialLY = hLightOutput[0]->GetMean();
    grLO->SetPoint(iAtt, 1000/hInputAbsorption[iAtt]->GetBinContent(13), hLightOutput[iAtt]->GetMean()/initialLY);
    grLO->SetPointError(iAtt, 0, hLightOutput[iAtt]->GetMeanError());
    
//     grLO_RMS->SetPoint(iAtt, 1000/hInputAbsorption[iAtt]->GetBinContent(13),  hPulseShapePMT[iAtt]->GetRMS());
//     grLO_RMS->SetPointError(iAtt, 0,  hPulseShapePMT[iAtt]->GetRMSError());
    grLO_RMS->SetPoint(iAtt, 1000/hInputAbsorption[iAtt]->GetBinContent(13),  hPulseShape[iAtt]->GetRMS());
    grLO_RMS->SetPointError(iAtt, 0,  hPulseShape[iAtt]->GetRMSError());
    
    cout << " mu_ind = " << 1000/hInputAbsorption[iAtt]->GetBinContent(13) << " [m^-1] :: LO = " << hLightOutput[iAtt]->GetMean() << " phe" << endl;
    
    }  //end of energy loop
    

  

    TCanvas * cInputAbsorption = new TCanvas ("cInputAbsorption", "cInputAbsorption", 600, 600);
    hInputAbsorption[0]->Draw();
    hInputAbsorption[0]->GetXaxis()->SetTitle("Wavelength (nm)");
    hInputAbsorption[0]->GetYaxis()->SetTitle("Absorption Length (mm)");
    for (int iAtt = 1; iAtt < nAtt; iAtt++){
      hInputAbsorption[iAtt]->Draw("same");
    }
    gPad->SetGrid();
    
    
    TCanvas * cPulseShape = new TCanvas ("cPulseShape", "cPulseShape", 600, 600);
    hPulseShape[0]->Draw();
    hPulseShape[0]->GetXaxis()->SetTitle("Time at photodetector (ns)");
    hPulseShape[0]->GetYaxis()->SetTitle("Normalized Amplitude");
    for (int iAtt = 1; iAtt < nAtt; iAtt++){
      hPulseShape[iAtt]->Draw("same");
    }
    gPad->SetGrid();
    
    TCanvas * cPulseShapePMT = new TCanvas ("cPulseShapePMT", "cPulseShapePMT", 600, 600);
    hPulseShapePMT[0]->Draw();
    hPulseShapePMT[0]->GetXaxis()->SetTitle("Time after PMT (ns)");
    hPulseShapePMT[0]->GetYaxis()->SetTitle("Normalized Amplitude");
    for (int iAtt = 1; iAtt < nAtt; iAtt++){
      hPulseShapePMT[iAtt]->Draw("same");
    }
    gPad->SetGrid();
    
    
    TCanvas * cLO = new TCanvas ("cLO", "cLO", 600, 600);
    grLO->SetMarkerStyle (21);
    grLO->GetXaxis()->SetTitle("#mu_{ind} (m^{-1})");
    grLO->GetYaxis()->SetTitle("Light Output");
    grLO->Draw("ALPE");
    gPad->SetGrid();

    TCanvas * cLO_RMS = new TCanvas ("cLO_RMS", "cLO_RMS", 600, 600);
    grLO_RMS->SetMarkerStyle (21);
    grLO_RMS->GetXaxis()->SetTitle("#mu_{ind} (m^{-1})");
    grLO_RMS->GetYaxis()->SetTitle("RMS pulse shape (ns)");
    grLO_RMS->GetXaxis()->SetRangeUser(0, 12);
    grLO_RMS->GetYaxis()->SetRangeUser(0, 11);
    grLO_RMS->Draw("ALPE");
    gPad->SetGrid();

}


