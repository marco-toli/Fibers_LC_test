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
  ofstream out_stream_att ("att_input_parameters_NEW.txt");
  
  TFile * RunFile[nAtt];
  cout << "almeno fin qui ..." << endl;


  RunFile[0] = new TFile("data/LC_sim_0mind.root","READ");   //
  RunFile[1] = new TFile("data/LC_sim_1mind.root","READ");   //
  RunFile[2] = new TFile("data/LC_sim_2mind.root","READ");   //
  RunFile[3] = new TFile("data/LC_sim_3mind.root","READ");   //
  RunFile[4] = new TFile("data/LC_sim_5mind.root","READ");   //
  
  RunFile[5] = new TFile("data/LC_sim_7mind.root","READ");   //
  RunFile[6] = new TFile("data/LC_sim_10mind.root","READ");   //
  RunFile[7] = new TFile("data/LC_sim_12mind.root","READ");   //
  RunFile[8] = new TFile("data/LC_sim_15mind.root","READ");   //
  RunFile[9] = new TFile("data/LC_sim_20mind.root","READ");   //
  RunFile[10] = new TFile("data/LC_sim_30mind.root","READ");   //
   
  int NBINS = 220;
  double maxZ = 220;
  double maxX = 14;
  
  TGraphErrors * grLO = new TGraphErrors ();
  TGraphErrors * gr_par0 = new TGraphErrors ();
  TGraphErrors * gr_par1 = new TGraphErrors ();
  TGraphErrors * gr_par2 = new TGraphErrors ();
  TGraphErrors * gr_FNUF = new TGraphErrors ();
  
  TH1F * hLightOutput[nAtt];
  
  TH1F * hLongitudinalEff[nAtt];
  TH1F * hLongitudinalInEff[nAtt];
  TH1F * hEfficiency[nAtt]; 
  
  TH1F * hTranverseXEff[nAtt];
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
    
    sprintf (name, "hLongitudinalEff_%d", iAtt);
    hLongitudinalEff[iAtt] = new TH1F (name, name, NBINS, 0, maxZ);
    sprintf (name, "hLongitudinalInEff_%d", iAtt);
    hLongitudinalInEff[iAtt] = new TH1F (name, name, NBINS, 0, maxZ);
    sprintf (name, "hEfficiency_%d", iAtt);
    hEfficiency[iAtt] = new TH1F (name, name, NBINS, 0, maxZ);
  
    sprintf (name, "hTranverseXEff_%d", iAtt);
    hTranverseXEff[iAtt] = new TH1F (name, name, NBINS, -maxX, maxX);
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

  for (int iAtt = 0; iAtt < nAtt; iAtt++) { // && iAtt < 5


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
      
      for (Int_t iEvt= 0; iEvt < NEVENTS ; iEvt++) {//&& iEvt < 5&& iEvt < 1000000

	TreeRun->GetEntry(iEvt);
// 	cout << " iEvt = " << iEvt << endl; //" :: photons = " << opPhoton_time_det.size() << endl; 	
	
	// filling efficiencies
	hLongitudinalInEff[iAtt]->Fill(InitialPositionZ + 110);
	hTranverseXInEff[iAtt]->Fill(InitialPositionX);
	  
	if (opPhoton_n_det == 1) {
// 	  cout << "det!" << endl;
	  hLongitudinalEff[iAtt]->Fill(InitialPositionZ + 110);
	  hTranverseXEff[iAtt]->Fill(InitialPositionX);
	}
	
	
// 	 cout << " opPhoton_time_det.size = " << opPhoton_time_det.size() << endl;
	 for (int i = 0; i< opPhoton_time_det.size(); i++) {
// 	   if (opPhoton_n_det_type.at(i) == 1) {
//    	    if (opPhoton_time_det.at(i)!=0) cout << "opPhoton_n_det_time = " << opPhoton_time_det.at(i) << endl;

	    hPulseShape[iAtt]->Fill(opPhoton_time_det.at(i));
	    hPulseShapePMT[iAtt]->Fill(gRandom->Gaus(opPhoton_time_det.at(i), 1.6));
// 	    scint_phot++;
// 	   }
 	  }
	
	
	

      }	   //end of events loop
      
      for (int iBin = 0; iBin < NBINS; iBin ++){
	hEfficiency[iAtt]->SetBinContent(iBin+1, hLongitudinalEff[iAtt]->GetBinContent(iBin+1)/hLongitudinalInEff[iAtt]->GetBinContent(iBin+1));
	hEfficiencyX[iAtt]->SetBinContent(iBin+1, hTranverseXEff[iAtt]->GetBinContent(iBin+1)/hTranverseXInEff[iAtt]->GetBinContent(iBin+1));
      }
      
//       hEfficiency[iAtt]->Scale (1./hEfficiency[iAtt]->GetMaximum());
//       hEfficiencyX[iAtt]->Scale (1./hEfficiencyX[iAtt]->GetMaximum());

    TF1 * fAtt = new TF1 ("fAtt", " [0]*exp((x-220)/[1]) + [2]", 0, 220);    
    fAtt->SetParameters(0.1, 20, 0.01);
//     fAtt->FixParameter(1,220);
    fAtt->SetLineColor(iAtt+1);
    for (int j=0; j< 10; j++) hEfficiency[iAtt]->Fit("fAtt", "QR");
    gr_par0->SetPoint(iAtt, 1000/hInputAbsorption[iAtt]->GetBinContent(13), fAtt->GetParameter(0));
    gr_par1->SetPoint(iAtt, 1000/hInputAbsorption[iAtt]->GetBinContent(13), fAtt->GetParameter(1));
    gr_par2->SetPoint(iAtt, 1000/hInputAbsorption[iAtt]->GetBinContent(13), fAtt->GetParameter(2));
//     gr_FNUF->SetPoint(iAtt, 1000/hInputAbsorption[iAtt]->GetBinContent(13), fAtt->Eval(114.4) - fAtt->Eval(35.2));	// fnuf tra 13 X_0 e 4 X_0 con X_0 PWO = 8.8 mm
    out_stream_att << fAtt->GetParameter(0) << " " << fAtt->GetParameter(1) << " " << fAtt->GetParameter(2) << endl;
    
    TF1 * fFNUF = new TF1 ("fFNUF", " [0]*x + [1]", 35.2, 114.4);    	// fnuf: slope of linear fit between 13 X_0 e 4 X_0 con X_0 PWO = 8.8 mm
    fFNUF->SetParameters(1, 1);
    hEfficiency[iAtt]->Fit("fFNUF", "QR");
    gr_FNUF->SetPoint(iAtt, 1000/hInputAbsorption[iAtt]->GetBinContent(13), fFNUF->GetParameter(0)*100/0.0088);	// fnuf tra 13 X_0 e 4 X_0 con X_0 PWO = 8.8 mm
    
    
    
    TF1 * fDecayTime = new TF1 ("fDecayTime", " [0]*exp(-x/[1])", 0, 100);
    fDecayTime->SetParameters(100, 20);
    fDecayTime->SetLineColor(iAtt+1);
    for (int j = 0; j< 10; j++) hPulseShape[iAtt]->Fit("fDecayTime", "QR");
    
    hPulseShape[iAtt]->Scale(1./hPulseShape[iAtt]->GetMaximum());
    hPulseShapePMT[iAtt]->Scale(1./hPulseShapePMT[iAtt]->GetMaximum());
    
    hInputAbsorption[iAtt]->SetLineColor(iAtt+1);
    hPulseShape[iAtt]->SetLineColor(iAtt+1);
    hPulseShapePMT[iAtt]->SetLineColor(iAtt+1);
    hLightOutput[iAtt]->SetLineColor(iAtt+1);
    hEfficiency[iAtt]->SetLineColor(iAtt+1);
    hEfficiencyX[iAtt]->SetLineColor(iAtt+1);
    
    grLO->SetPoint(iAtt, 1000/hInputAbsorption[iAtt]->GetBinContent(13), hLightOutput[iAtt]->GetMean());
    grLO->SetPointError(iAtt, 1, hLightOutput[iAtt]->GetMeanError());
    
    cout << " mu_ind = " << 1000/hInputAbsorption[iAtt]->GetBinContent(13) << " [m^-1] :: LO = " << hLightOutput[iAtt]->GetMean() << " phe" << endl;
    
    }  //end of energy loop
    

  
    TCanvas * cInputAbsorption = new TCanvas ("cInputAbsorption", "cInputAbsorption", 600, 600);
    hInputAbsorption[0]->Draw();
    for (int iAtt = 1; iAtt < nAtt; iAtt++){
      hInputAbsorption[iAtt]->Draw("same");
    }
    gPad->SetGrid();
    
    
    TCanvas * cPulseShape = new TCanvas ("cPulseShape", "cPulseShape", 600, 600);
    hPulseShape[0]->Draw();
    for (int iAtt = 1; iAtt < nAtt; iAtt++){
      hPulseShape[iAtt]->Draw("same");
    }
    gPad->SetGrid();
    
    TCanvas * cPulseShapePMT = new TCanvas ("cPulseShapePMT", "cPulseShapePMT", 600, 600);
    hPulseShapePMT[0]->Draw();
    for (int iAtt = 1; iAtt < nAtt; iAtt++){
      hPulseShapePMT[iAtt]->Draw("same");
    }
    gPad->SetGrid();
    
    /*
    TCanvas * cLO = new TCanvas ("cLO", "cLO", 600, 600);
    grLO->SetMarkerStyle (21);
    grLO->Draw("ALPE");
    gPad->SetGrid();*/
    
    TCanvas * cLongitudinalEff = new TCanvas ("cLongitudinalEff", "cLongitudinalEff", 600, 600);
    hEfficiency[0]->Draw();
    hEfficiency[0]->GetYaxis()->SetRangeUser(1e-5,0.13);
    for (int iAtt = 1; iAtt < nAtt; iAtt++){
      hEfficiency[iAtt]->Draw("same");
    }
    gPad->SetGrid();
    gPad->SetLogy();
    
    TCanvas * cXEff = new TCanvas ("cXEff", "cXEff", 600, 600);
    hEfficiencyX[0]->Draw();
    hEfficiencyX[0]->GetYaxis()->SetRangeUser(1e-5,0.13);
    for (int iAtt = 1; iAtt < nAtt; iAtt++){
      hEfficiencyX[iAtt]->Draw("same");
    }
    gPad->SetGrid();
    gPad->SetLogy();
  
    
    TCanvas * c_par0 = new TCanvas ("c_par0", "c_par0", 600, 600);
    gr_par0->SetMarkerStyle (21);
    gr_par0->GetXaxis()->SetTitle("#mu_{ind} (m^{-1})");
    gr_par0->GetYaxis()->SetTitle("par0");
//     gr_par0->GetXaxis()->SetRangeUser(0, 12);
    gr_par0->GetYaxis()->SetRangeUser(0, 0.1);
    gr_par0->Draw("ALPE");
    gPad->SetGrid();

    TCanvas * c_par1 = new TCanvas ("c_par1", "c_par1", 600, 600);
    gr_par1->SetMarkerStyle (21);
    gr_par1->GetXaxis()->SetTitle("#mu_{ind} (m^{-1})");
    gr_par1->GetYaxis()->SetTitle("par1");
//     gr_par1->GetXaxis()->SetRangeUser(0, 12);
//     gr_par1->GetYaxis()->SetRangeUser(0, 0.1);
    gr_par1->Draw("ALPE");
    gPad->SetGrid();

    TCanvas * c_par2 = new TCanvas ("c_par2", "c_par2", 600, 600);
    gr_par2->SetMarkerStyle (21);
    gr_par2->GetXaxis()->SetTitle("#mu_{ind} (m^{-1})");
    gr_par2->GetYaxis()->SetTitle("par2");
//     gr_par2->GetXaxis()->SetRangeUser(0, 12);
    gr_par2->GetYaxis()->SetRangeUser(0, 0.1);
    gr_par2->Draw("ALPE");
    gPad->SetGrid();

    TCanvas * c_FNUF = new TCanvas ("c_FNUF", "c_FNUF", 600, 600);
    gr_FNUF->SetMarkerStyle (21);
    gr_FNUF->GetXaxis()->SetTitle("#mu_{ind} (m^{-1})");
    gr_FNUF->GetYaxis()->SetTitle("FNUF (%/X_{0})");
//     gr_FNUF->GetXaxis()->SetRangeUser(0, 12);
//     gr_FNUF->GetYaxis()->SetRangeUser(0, 11);
    gr_FNUF->Draw("ALPE");
    gPad->SetGrid();

}


