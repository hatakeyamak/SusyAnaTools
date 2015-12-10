#include "Weights.h"
#include "NTupleReader.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <ctime>
#include <iomanip>
#include <cmath>



Pileup_Sys::Pileup_Sys()
{


}


Pileup_Sys::~Pileup_Sys()
{
 //dtor
}


void Pileup_Sys::getPileup_Sys(NTupleReader& tr)
{
//Calling the Jasn file from RA2b currently
TFile Pileup_Jasn("PileupHistograms_1117.root");

//These are the historgrams currently in the Jasn file
TH1F *pu_central  = (TH1F*)Pileup_Jasn.Get("pu_weights_central");
TH1F *pu_up  = (TH1F*)Pileup_Jasn.Get("pu_weights_up");
TH1F *pu_down  = (TH1F*)Pileup_Jasn.Get("pu_weights_down");

 double _PUweightFactor = 1.;
 double _PUSysUp = 1.;
 double _PUSysDown = 1.;

 double trueint; 
 double w = 1.;


//These are all the pileup varaibles the only one that is needed is the first
    const double &tru_npv  = tr.getVar<double>("tru_npv");
    //const int    BX       = tr.getVar<int>("BX");
    //const int    nm1      = tr.getVar<int>("nm1");
    //const int    n0       = tr.getVar<int>("n0");
    //const int    np1      = tr.getVar<int>("np1");
    //const int    npv      = tr.getVar<int>("npv");

	//This is going through and calculating the wieght 
  if (tru_npv < pu_central->GetBinLowEdge(pu_central->GetNbinsX()+1)) {
        _PUweightFactor = pu_central->GetBinContent(pu_central->GetXaxis()->FindBin(tru_npv));
  } else {
    std::cerr << "WARNING in WeightProcessor::getPUWeight: Number of interactions = " << tru_npv
              << " out of histogram binning." << std::endl;
    _PUweightFactor = pu_central->GetBinContent(pu_central->GetNbinsX());
  }

  if (tru_npv < pu_up->GetBinLowEdge(pu_up->GetNbinsX()+1)) {
        _PUSysUp = pu_up->GetBinContent(pu_up->GetXaxis()->FindBin(tru_npv));
  } else {
    std::cerr << "WARNING in WeightProcessor::getPUWeight: Number of interactions = " << tru_npv
              << " out of histogram binning." << std::endl;
    _PUSysUp = pu_up->GetBinContent(pu_up->GetNbinsX());
  }
    

  if (tru_npv < pu_down->GetBinLowEdge(pu_down->GetNbinsX()+1)) {
        _PUSysDown = pu_down->GetBinContent(pu_down->GetXaxis()->FindBin(tru_npv));
  } else {
    std::cerr << "WARNING in WeightProcessor::getPUWeight: Number of interactions = " << tru_npv
              << " out of histogram binning." << std::endl;
    _PUSysDown = pu_down->GetBinContent(pu_down->GetNbinsX());
  }


          tr.registerDerivedVar("_PUweightFactor", _PUweightFactor);
          tr.registerDerivedVar("_PUSysUp", _PUSysUp);
          tr.registerDerivedVar("_PUSysDown", _PUSysDown);

        
}

void Pileup_Sys::operator()(NTupleReader& tr)
{
 //
    getPileup_Sys(tr);
}

