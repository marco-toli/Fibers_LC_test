{


  #include <cstdlib>
//   #include <iostream>
  #include <fstream>
  #include <stdio.h>
  #include <string.h>
  #include <vector>
  #include <math.h>
  #include <sellmeier.h>

  using namespace std;


  ///************** INDEX OF REFRACTION VS WAVELENGTH ************************** ////
  // magic formula for pwo refractive index obtained from fit of experimental data: see CMS TN/95-184
  TF1 *  SellmeierEMP = new TF1 ("SellmeierEMP", "sqrt(3.5158 + 1.22372/(1-73239/x/x) )", 300, 900); 	// defined between 300 and 900 nm only
  SellmeierEMP->SetLineColor(kBlack);
  
  // Cauchy approximation using 5 parameters
  TF1 *  Cauchy = new TF1 ("Cauchy", "2.1941 - 6250.6/pow(x,2) + 1.4280e10/pow(x,4) - 2.2215e15/pow(x,6) + 1.5114e20/pow(x,8)", 300, 900); 	// defined between 300 and 900 nm only
  Cauchy->SetLineColor(kRed);
  
  // Sellmeier formula
  TF1 *  Sellmeier = new TF1 ("Sellmeier", "sqrt (1 + pow(1.5861,2) + pow(1.1062,2)/(1 - pow(270.63/x,2)))", 300, 900); 	// defined between 300 and 900 nm only
  Sellmeier->SetLineColor(kBlue);
  
  // Extraordinary wave and index of refraction
  TF1 * fExt = new TF1 ("fExt", "fExtra(x)-0.26", 300, 900);
  fExt->SetLineColor(kGreen+2);
  
  // Dispersion formula for air from
  TF1 * AirDispersion = new TF1 ("AirDispersion" , "1 + 5792105e-8/(238.0185-pow(x,-2)) + 167917e-8/(57.362-pow(x,-2))", 300, 900);
  AirDispersion->SetLineColor(kBlue-2);
  
  
  TCanvas * cRefractiveIndexPWO = new TCanvas ("cRefractiveIndexPWO", "cRefractiveIndexPWO");
  SellmeierEMP->Draw();
  Cauchy->Draw("same");
  Sellmeier->Draw("same");
  fExt->Draw("same");
  SellmeierEMP->GetXaxis()->SetTitle("Wavelength (nm)");
  SellmeierEMP->GetYaxis()->SetTitle("n_{ord}");	// ordinary refractive index
  SellmeierEMP->GetYaxis()->SetRangeUser(1.5,3.5);	// ordinary refractive index
  gPad->SetGrid();

  TCanvas * cRefractiveIndexAir = new TCanvas ("cRefractiveIndexAir", "cRefractiveIndexAir");
  AirDispersion->Draw();
  AirDispersion->GetXaxis()->SetTitle("Wavelength (nm)");
  AirDispersion->GetYaxis()->SetTitle("n_{ord}");	// ordinary refractive index
  gPad->SetGrid();

  

  ///************** REFLECTION AND TRASMITTANCE VS WAVELENGTH ************************** ////
  
  double theta_inc = 0;
  double theta_refr;	// snell law: sin(theta_inc)/sin(theta_refr) = n_2/n_1 with n_1 refr index of first medium and n_2 refr index of second medium 

    // reflectance for s-polarized light as a function of wavelength x
    TF1 * fRef_s = new TF1 ("fRef_s", "pow( (AirIndexRefr(x)*cos([0]) - IndexRefr(x)*sqrt(1-pow(AirIndexRefr(x)/IndexRefr(x)*sin([0]),2))) / (AirIndexRefr(x)*cos([0]) + IndexRefr(x)*sqrt(1-pow(AirIndexRefr(x)/IndexRefr(x)*sin([0]),2))),2)", 300, 900);
    fRef_s->SetParameter(0, theta_inc);
    fRef_s->SetLineColor(kRed);
    
    // reflectance for p-polarized light as a function of wavelength x
    TF1 * fRef_p = new TF1 ("fRef_p", "pow( (AirIndexRefr(x)*sqrt(1-pow(AirIndexRefr(x)/IndexRefr(x)*sin([0]),2))  - IndexRefr(x)*cos([0])) / (AirIndexRefr(x)*sqrt(1-pow(AirIndexRefr(x)/IndexRefr(x)*sin([0]),2))  + IndexRefr(x)*cos([0])),2)", 300, 900);
    fRef_p->SetParameter(0, theta_inc);
    fRef_p->SetLineColor(kBlue);
    
    TF1 * fRef_SUM = new TF1 ("fRef_SUM", "0.5*pow( (AirIndexRefr(x)*sqrt(1-pow(AirIndexRefr(x)/IndexRefr(x)*sin([0]),2))  - IndexRefr(x)*cos([0])) / (AirIndexRefr(x)*sqrt(1-pow(AirIndexRefr(x)/IndexRefr(x)*sin([0]),2))  + IndexRefr(x)*cos([0])),2) + pow( (AirIndexRefr(x)*cos([0]) - IndexRefr(x)*sqrt(1-pow(AirIndexRefr(x)/IndexRefr(x)*sin([0]),2))) / (AirIndexRefr(x)*cos([0]) + IndexRefr(x)*sqrt(1-pow(AirIndexRefr(x)/IndexRefr(x)*sin([0]),2))),2)*0.5", 300, 900);
    fRef_SUM->SetParameter(0, theta_inc);
    fRef_SUM->SetLineColor(kBlack);
    
    /* ATTENTION:: THIS IS A VERY CPU CONSUMING FUNCTION, BETTER NOT TO USE AND TO USE TGRAPH INSTEAD
    TF1 * PWO_maxTransmission = new TF1 ("PWO_maxTransmission", "pow(1-ReflectanceSUM_Air_PWO(x,[0]),2)/(1-pow(ReflectanceSUM_Air_PWO(x,[0]),2))", 300, 900);
    PWO_maxTransmission->SetParameter(0, theta_inc);
    PWO_maxTransmission->SetLineColor(kGreen + 2);*/
    
    TGraphErrors * gTransmission = new TGraphErrors();
    for (int iW = 300; iW<900; iW++) {
      gTransmission->SetPoint(iW, iW, pow(1-fRef_SUM(iW),2)/(1-pow(fRef_SUM(iW),2)));
    }
    
  //   double
    
    TCanvas * cReflectance_S = new TCanvas ("cReflectance_S", "cReflectance_S");
    fRef_s->Draw();
    fRef_p->Draw("same");
    fRef_SUM->Draw("same");
    fRef_s->GetXaxis()->SetTitle("Wavelength (nm)");
    fRef_s->GetYaxis()->SetTitle("Reflectance");	// ordinary refractive index
    fRef_s->GetYaxis()->SetRangeUser(0,1);
    gPad->SetGrid();

    TCanvas * cTransmission = new TCanvas ("cTransmission", "cTransmission");
    gTransmission->Draw("ALPE");
    gTransmission->SetLineColor(kGreen+2);
    gTransmission->SetMarkerColor(kGreen+2);
    gTransmission->SetLineWidth(2);
    
    gTransmission->GetXaxis()->SetTitle("Wavelength (nm)");
    gTransmission->GetYaxis()->SetTitle("Transmission");	// ordinary refractive index
    gTransmission->GetXaxis()->SetRangeUser(300,900);
    gTransmission->GetYaxis()->SetRangeUser(0,1);
    gPad->SetGrid();
    
    /*
    TCanvas * cTransmission = new TCanvas ("cTransmission", "cTransmission");
    PWO_maxTransmission->Draw();
    
    PWO_maxTransmission->GetXaxis()->SetTitle("Wavelength (nm)");
    PWO_maxTransmission->GetYaxis()->SetTitle("Transmission");	// ordinary refractive index
    PWO_maxTransmission->GetYaxis()->SetRangeUser(0,1);
    gPad->SetGrid();*/
}


