#include <cstdlib>
#include <iostream>
#include <TROOT.h>
#include <TH1.h>
#include <TChain.h>
#include <TCanvas.h>
#include "TH2.h"
#include <cstring>
#include <TLatex.h>
#include <TDatabasePDG.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <fstream>
#include <cmath>
#include <chrono>
#include <TBenchmark.h>

using namespace std;

double sq(double a){
  return a*a;
}

bool sample(double t, TF1 tdist){
  double r=tdist.GetRandom(0,7);
  if(r>t){
    return true;
  }  else {
    return false;
  }
}

void eventGen(){

  TH1F ht("t","t",100,0,4);
  TH1F hMM("MM","; e' pi+ Missing Mass",100,0.5,1.5);
  TH1F hPpip("Ppip","pi+ P",100,0,10);
  TH1F hPel("Pel","el P",100,0,10);
  TH1F hPn("Pn","n P",100,0,10);
  TH1F hThetapip("Thpip","pi+ Theta",100,0,3.5);
  TH1F hThetael("Thel","el Theta",100,0,3.5);
  TH1F hThetan("Thn","n Theta",100,0,3.5);
  TH1F hPhipip("Phpip","pi+ Phi",100,-3.5,3.5);
  TH1F hPhiel("Phel","el Phi",100,-3.5,3.5);
  TH1F hPhin("Phn","n Phi",100,-3.5,3.5);

  double beamE=10.6;

  double Mp = 0.938272;
  double Mn = 0.939565;
  double Mpi = 0.1395;
  double Mel = 0.511e-3;   

  int nEv_pFile=1000;
  int nFiles=1;

  TString dir="./out/";
  TString filename=dir+"epipn_";

  std::vector<double> masses = {Mn,Mel ,Mpi};

  TLorentzVector target(0,0,0,Mp);
  TLorentzVector beam(0,0,beamE,sqrt(sq(beamE)+sq(Mel)));
  TLorentzVector W = beam + target;
  TGenPhaseSpace event;

  event.SetDecay(W, masses.size(), &masses[0],"Fermi");

  TF1 tdist("tdist","expo",0,10);
  tdist.SetParameters(3,-1.);

  for (int j = 0; j < nFiles; j++) 
	{
		cout << "FileNb: "<< j << endl;

    FILE* f;
	  TString name;
		name = filename + Form("%.4d.txt", j);
		f = fopen(name.Data(), "w");

    Long64_t evInFile=0;

		while(evInFile<nEv_pFile)
		{
      auto weight = event.Generate();

      TLorentzVector n_out=*event.GetDecay(0);
      TLorentzVector el_out=*event.GetDecay(1);
      TLorentzVector pi_out=*event.GetDecay(2);

      double t = -1*(n_out-target).M2();

      if(sample(t,tdist)){

        //header
			  fprintf(f, "%d %d %d %d %d %d %.6f %d %d %.6f\n",3,1,1,0,0,11,beamE,2212,0, 1.0);
        //e-
			  fprintf(f, "%d %d %d %d %d %d %.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f\n",1, 1, 1,11,0,0, el_out.X(), el_out.Y(), el_out.Z(), el_out.E(),el_out.M(),0.0,0.0,0.0);
			  //pi+
			  fprintf(f, "%d %d %d %d %d %d %.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f\n",2, 1, 1,211,0,0, pi_out.X(), pi_out.Y(), pi_out.Z(), pi_out.E(),pi_out.M(),0.0,0.0,0.0);
        //n
			  fprintf(f, "%d %d %d %d %d %d %.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f\n",3, 1, 1,2112,0,0, n_out.X(), n_out.Y(), n_out.Z(), n_out.E(),n_out.M(),0.0,0.0,0.0);

        double MM = (beam + target - ( el_out + pi_out )).M();

        //don't need full stats for diagnostic plots
        if(j<5){

          ht.Fill(t);
          hMM.Fill(MM);

          hPpip.Fill(pi_out.P());
          hThetapip.Fill(pi_out.Theta());
          hPhipip.Fill(pi_out.Phi());

          hPel.Fill(el_out.P());
          hThetael.Fill(el_out.Theta());
          hPhiel.Fill(el_out.Phi());

          hPn.Fill(n_out.P());
          hThetan.Fill(n_out.Theta());
          hPhin.Fill(n_out.Phi());

        }


        evInFile++;
      }

    }
    
    fclose(f);
  }

  TFile *fout = TFile::Open(dir+"plots.root", "recreate");
 
  ht.Write();
  hMM.Write();
  hPpip.Write();
  hThetapip.Write();
  hPhipip.Write();
  hPel.Write();
  hThetael.Write();
  hPhiel.Write();
  hPn.Write();
  hThetan.Write();
  hPhin.Write();


  fout->Close();
  
}
