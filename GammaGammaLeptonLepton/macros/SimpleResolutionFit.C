/*
 * Simple 1-d fit to Gaussian+polynomial, taking as input the histograms produced by the "Dimuons2017Macro" macro
 * Fits to the mean and sigma of data and MC for the 2 arms separately, and combined. Binning, fit ranges, and initial values 
 * may need to be adjusted as necessary.
 *
 * For single-RP hitograms: reco=1, for multi-RP histograms: reco=2
 */
void SimpleResolutionFit(Int_t reco = 1)
{
  TFile *f1 = TFile::Open("MoreDimuons2017AllWithMultSingleTrack_BCDEF_single_RC.root"); // Input data histograms
  TFile *f2 = TFile::Open("MoreDimuons2017AllWithMultMC.root"); // Input MC histograms

  TH1F *h1; 
  if(reco == 1)
    h1 = (TH1F *)f1->Get("hres45");
  if(reco == 2)
    h1 = (TH1F *)f1->Get("hres45mult");

  TH1F *h2; 
  if(reco == 1)
    h2 = (TH1F *)f1->Get("hres56");
  if(reco == 2)
    h2 = (TH1F *)f1->Get("hres56mult");

  TH1F *h3;
  if(reco == 1)
    h3 = (TH1F *)f2->Get("hres45");
  if(reco == 2)
    h3 = (TH1F *)f2->Get("hres45mult");

  TH1F *h4;
  if(reco == 1)
    h4 = (TH1F *)f2->Get("hres56");
  if(reco == 2)
    h4 = (TH1F *)f2->Get("hres56mult");


  h1->Sumw2();
  h2->Sumw2();
  TH1F *hsum = (TH1F *)h1->Clone("hsum");
  hsum->Sumw2();
  hsum->Add(h2);

  h3->Sumw2();
  h4->Sumw2();
  TH1F *hsummc = (TH1F *)h3->Clone("hsum");
  hsummc->Add(h4);

  TCanvas *c1 = new TCanvas("c1","c1");
  //  c1->Divide(1,2);

  // Adjust binning as needed
  Int_t rebinfact = 5;

  c1->Divide(3,1);
  c1->cd(1);
  h3->Rebin(rebinfact);
  h1->Rebin(rebinfact);
  h3->Scale(1.0/20.0);
  h3->SetLineColor(4); h3->SetLineWidth(3);
  h1->SetMaximum(100);
  h1->Draw("e");
  h3->Draw("histsame");
  TF1 *fitmc1 = new TF1("fitmc1","gaus(0)",-1,1);
  TFitResultPtr fitrmc3 = h3->Fit("fitmc1","LEMVS","",-1,1);

  h1->SetStats(1);
  h1->SetMarkerStyle(20); h1->SetLineWidth(3); h1->SetLineColor(1);
  h1->SetMaximum(100);
  h1->SetTitle("2017BCDEF, 45");
  h1->SetXTitle("1 - #xi(p)/#xi(#mu#mu)");
  h1->Draw("esame");
  TF1 *fit1 = new TF1("fit1", "gaus(0)+pol2(3)",-5,1);
  fit1->SetParName(0,"Norm.");
  fit1->SetParName(1,"Mean");
  fit1->SetParName(2,"Sigma");
  fit1->SetParName(3,"background p0");
  fit1->SetParName(4,"background p1");
  fit1->SetParName(5,"background p2");
  fit1->SetParameter(0,50);
  fit1->SetParameter(1,0);
  fit1->SetParameter(2,0.1);

  fit1->SetParLimits(0,0.1,1000);
  fit1->SetParLimits(1,-0.5,0.5);
  fit1->SetParLimits(2,0.03,0.2);
  fit1->SetLineColor(2);
  TFitResultPtr fitr = h1->Fit("fit1","LEMVS","",-5,1);
  std::cout << fitr->MinFcnValue() << std::endl;

  c1->cd(2);
  h2->Rebin(rebinfact);
  h4->Rebin(rebinfact);
  h4->Scale(1.0/20.0);
  h4->SetLineColor(4); h4->SetLineWidth(3);
  h4->SetMaximum(100);
  h2->Draw("e");
  h4->Draw("histsame");
  TF1 *fitmc2 = new TF1("fitmc2","gaus(0)",-1,1);
  TFitResultPtr fitrmc4 = h4->Fit("fitmc2","LEMVS","",-1,1);

  h2->SetStats(1);
  h2->SetMarkerStyle(20); h2->SetLineWidth(3); h2->SetLineColor(1);
  h2->SetMaximum(100);
  h2->SetTitle("2017BCDEF, 56");
  h2->SetXTitle("1 - #xi(p)/#xi(#mu#mu)");
  h2->Draw("esame");
  TF1 *fit2 = new TF1("fit2", "gaus(0)+pol2(3)",-5,1);
  fit2->SetParName(0,"Norm.");
  fit2->SetParName(1,"Mean");
  fit2->SetParName(2,"Sigma");
  fit2->SetParName(3,"background p0");
  fit2->SetParName(4,"background p1");
  fit2->SetParName(5,"background p2");
  fit2->SetParameter(0,50);
  fit2->SetParameter(1,0);
  fit2->SetParameter(2,0.1);

  fit2->SetParLimits(0,0.1,1000);
  fit2->SetParLimits(1,-0.5,0.5);
  fit2->SetParLimits(2,0.03,0.2);
  fit2->SetLineColor(2);
  TFitResultPtr fitr2 = h2->Fit("fit2","LEMVS","",-5,1);
  std::cout << fitr2->MinFcnValue() << std::endl;
						   
  c1->cd(3);
  hsum->Rebin(rebinfact);
  hsummc->Rebin(rebinfact);
  hsummc->Scale(1.0/20.0);
  hsummc->SetLineColor(4); hsummc->SetLineWidth(3);
  hsummc->SetMaximum(100);
  hsum->Draw("e");
  hsummc->Draw("histsame");
  TF1 *fitmcsum = new TF1("fitmcsum","gaus(0)",-1,1);
  TFitResultPtr fitrmcsum = hsummc->Fit("fitmcsum","LEMVS","",-1,1);

  hsum->SetStats(1);
  hsum->SetMarkerStyle(20); hsum->SetLineWidth(3); hsum->SetLineColor(1);
  hsum->SetMaximum(100);
  hsum->SetTitle("2017BCDEF, 45+56");
  hsum->SetXTitle("1 - #xi(p)/#xi(#mu#mu)");
  hsum->Draw("esame");
  TF1 *fitsum = new TF1("fitsum", "gaus(0)+pol2(3)",-5,1);
  fitsum->SetParName(0,"Norm.");
  fitsum->SetParName(1,"Mean");
  fitsum->SetParName(2,"Sigma");
  fitsum->SetParName(3,"background p0");
  fitsum->SetParName(4,"background p1");
  fitsum->SetParName(5,"background p2");
  fitsum->SetParameter(0,50);
  fitsum->SetParameter(1,0);
  fitsum->SetParameter(2,0.10);

  fitsum->SetParLimits(0,0.000001,1000);
  fitsum->SetParLimits(1,-0.9,0.9);
  fitsum->SetParLimits(2,0.03,0.2);
  fitsum->SetLineColor(2);
  TFitResultPtr fitrs = hsum->Fit("fitsum","LEMVS","",-5,1);
  std::cout << fitrs->MinFcnValue() << std::endl;

  std::cout << "Data:" << std::endl;
  std::cout << "\t45 mean = " << fit1->GetParameter(1) << " +- " << fit1->GetParError(1) 
  	    << ", sigma = " << fit1->GetParameter(2) << " +- " << fit1->GetParError(2) << std::endl;
  std::cout << "\t56 mean = " << fit2->GetParameter(1) << " +- " << fit2->GetParError(1)
	    << ", sigma = " << fit2->GetParameter(2) << " +- " << fit2->GetParError(2) << std::endl;
  std::cout << "\tsum mean = " << fitsum->GetParameter(1) << " +- " << fitsum->GetParError(1)
            << ", sigma = " << fitsum->GetParameter(2) << " +- " << fitsum->GetParError(2) << std::endl;

  std::cout << "MC:" << std::endl;
  std::cout << "\t45 mean = " << fitmc1->GetParameter(1) << " +- " << fitmc1->GetParError(1)
	    << ", sigma = " << fitmc1->GetParameter(2) << " +- " << fitmc1->GetParError(2) << std::endl;
  std::cout << "\t56 mean = " << fitmc2->GetParameter(1) << " +- " << fitmc2->GetParError(1)
  	    << ", sigma = " << fitmc2->GetParameter(2) << " +- " << fitmc2->GetParError(2) << std::endl;
  std::cout << "\tsum mean = " << fitmcsum->GetParameter(1) << " +- " << fitmcsum->GetParError(1)
  	    << ", sigma = " << fitmcsum->GetParameter(2) << " +- " << fitmcsum->GetParError(2) << std::endl;

  std::cout << "\tsum RMS = " << hsummc->GetRMS() << std::endl;

}


