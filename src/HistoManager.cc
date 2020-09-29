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
/// \file HistoManager.cc
/// \brief Implementation of the HistoManager class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "HistoManager.hh"
#include "G4UnitsTable.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::HistoManager()
  : fFileName("Hadr04")
{
  Book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::Book()
{
  // Create or get analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in HistoManager.hh
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetFileName(fFileName);
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetActivation(true);     //enable inactivation of histograms
   

  // Default values (to be reset via /analysis/h1/set command)               
  G4int nbins = 100;
  G4double vmin = 0.;
  G4double vmax = 100.;

  // Create all histograms as inactivated 
  
  // 1D histograms

  // ID=0
  G4int ih = analysisManager->CreateH1("neutronEnergy_gen", "DD neutrons", nbins, vmin, vmax); 
  analysisManager->SetH1Activation(ih, false);
  
  // ID=1
  ih = analysisManager->CreateH1("neutronEnergy_exitWindow", "Neutrons exiting collimator window", nbins, vmin, vmax); 
  analysisManager->SetH1Activation(ih, false);
  
  // ID=2
  ih = analysisManager->CreateH1("neutronEnergy_exitShield", "neutrons exiting shield", nbins, vmin, vmax); 
  analysisManager->SetH1Activation(ih, false);
  
  // ID=3
  ih = analysisManager->CreateH1("neutronEnergy_enterArgon", "neutrons entering argon", nbins, vmin, vmax); 
  analysisManager->SetH1Activation(ih, false);
  
  // ID=4
  ih = analysisManager->CreateH1("neutronEnergy_exitCryostat", "neutrons exiting Cryostat", nbins, vmin, vmax); 
  analysisManager->SetH1Activation(ih, false);
  
  // ID=5
  ih = analysisManager->CreateH1("neutronEnergy_enterWorld", "neutrons entering World", nbins, vmin, vmax); 
  analysisManager->SetH1Activation(ih, false);
  
  // ID=6
  ih = analysisManager->CreateH1("gammaEnergy_enterShield", "gammas entering shield", nbins, vmin, vmax); 
  analysisManager->SetH1Activation(ih, false);
  
  // ID=7
  ih = analysisManager->CreateH1("gammaEnergy_exitShield", "gammas exiting shield", nbins, vmin, vmax); 
  analysisManager->SetH1Activation(ih, false);
  
  // ID=8
  ih = analysisManager->CreateH1("gammaEnergy_enterArgon", "gammas entering argon", nbins, vmin, vmax); 
  analysisManager->SetH1Activation(ih, false);
  
  // ID=9
  ih = analysisManager->CreateH1("gammaEnergy_exitCryostat", "gammas exiting Cryostat", nbins, vmin, vmax); 
  analysisManager->SetH1Activation(ih, false);
  
  // ID=10
  ih = analysisManager->CreateH1("gammaEnergy_enterWorld", "gammas entering World", nbins, vmin, vmax); 
  analysisManager->SetH1Activation(ih, false);
  
  // ID=11
  ih = analysisManager->CreateH1("capture_time", "neutron capture time",nbins,vmin,vmax);
  analysisManager->SetH1Activation(ih, false);
  
  // 2D histograms
  
  // ID=0
  ih = analysisManager->CreateH2("neutronPlane1", "neutrons on test plane#1 (side view, y:x)",nbins,vmin,vmax, nbins,vmin,vmax);
  analysisManager->SetH2Activation(ih, false);
  
  // ID=1
  ih = analysisManager->CreateH2("neutronPlane2", "neutrons on test plane#2 (sid view, y:x)",nbins,vmin,vmax, nbins,vmin,vmax);
  analysisManager->SetH2Activation(ih, false);
  
  // ID=2
  ih = analysisManager->CreateH2("neutronPlane3", "neutrons on test plane#3 (side view, y:x)",nbins,vmin,vmax, nbins,vmin,vmax);
  analysisManager->SetH2Activation(ih, false);
  
  // ID=3
  ih = analysisManager->CreateH2("neutronPlane4", "neutrons on test plane#4 (sid view, y:x)",nbins,vmin,vmax, nbins,vmin,vmax);
  analysisManager->SetH2Activation(ih, false);
  
  // ID=4
  ih = analysisManager->CreateH2("neutronPlane5 ", "neutrons on test plane#5 (top view, z:x)",nbins,vmin,vmax, nbins,vmin,vmax);
  analysisManager->SetH2Activation(ih, false);
  
  // ID=5
  ih = analysisManager->CreateH2("neutronPlane6 ", "neutrons on test plane#6 (top view, z:x)",nbins,vmin,vmax, nbins,vmin,vmax);
  analysisManager->SetH2Activation(ih, false);
  
  
  
  
  
  // ID=6
  ih = analysisManager->CreateH2("gammaPlane1", "gammas on test plane#1 (side view, y:x)",nbins,vmin,vmax, nbins,vmin,vmax);
  analysisManager->SetH2Activation(ih, false);
  
  // ID=7
  ih = analysisManager->CreateH2("gammaPlane2", "gammas on test plane#2 (sid view, y:x)",nbins,vmin,vmax, nbins,vmin,vmax);
  analysisManager->SetH2Activation(ih, false);
  
  
  // ID=8
  ih = analysisManager->CreateH2("gammaPlane3", "gammas on test plane#3 (side view, y:x)",nbins,vmin,vmax, nbins,vmin,vmax);
  analysisManager->SetH2Activation(ih, false);
  
  // ID=9
  ih = analysisManager->CreateH2("gammaPlane4", "gammas on test plane#4 (side view, y:x)",nbins,vmin,vmax, nbins,vmin,vmax);
  analysisManager->SetH2Activation(ih, false);
  
  // ID=10
  ih = analysisManager->CreateH2("gammaPlane5", "gammas on test plane#5 (top view, z:x)",nbins,vmin,vmax, nbins,vmin,vmax);
  analysisManager->SetH2Activation(ih, false);
  
  // ID=11
  ih = analysisManager->CreateH2("gammaPlane6", "gammas on test plane#6 (top view, z:x)",nbins,vmin,vmax, nbins,vmin,vmax);
  analysisManager->SetH2Activation(ih, false);
  
  
  
  // ntuple
  
  
  // ID=0, neutron transport
  analysisManager->CreateNtuple("ntransport", "Neutrons crossing boundary"); //id = 0
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleIColumn("tag"); 
  analysisManager->FinishNtuple(); 
  
  // ID=1, neutron capture
  analysisManager->CreateNtuple("ncapture", "Neutron captures"); //id = 1
  analysisManager->CreateNtupleDColumn("x");       //column 0
  analysisManager->CreateNtupleDColumn("y");       //column 1
  analysisManager->CreateNtupleDColumn("z");       //column 2
  analysisManager->CreateNtupleDColumn("t");       //column 3
  analysisManager->CreateNtupleIColumn("tag");  //column 4 
  analysisManager->FinishNtuple();  
  
  // ID=2, gamma tranport
  analysisManager->CreateNtuple("gammatransport", "Gammas crossing boundary"); //id = 2
  analysisManager->CreateNtupleDColumn("x");       //column 0
  analysisManager->CreateNtupleDColumn("y");       //column 1
  analysisManager->CreateNtupleDColumn("z");       //column 2
  analysisManager->CreateNtupleIColumn("tag");  //column 3 
  analysisManager->FinishNtuple();  
  
  // ID=3, neutrons hitting test planes
  analysisManager->CreateNtuple("neutronflux", "Neutrons hitting testplanes"); //id = 3
  analysisManager->CreateNtupleDColumn("x");       //column 0
  analysisManager->CreateNtupleDColumn("y");       //column 1
  analysisManager->CreateNtupleDColumn("z");       //column 2
  analysisManager->CreateNtupleIColumn("tag");  //column 3 
  analysisManager->FinishNtuple();  
  
  // ID=3, gammas hitting test planes
  analysisManager->CreateNtuple("gammaflux", "Gammas hitting testplanes"); //id = 4
  analysisManager->CreateNtupleDColumn("x");       //column 0
  analysisManager->CreateNtupleDColumn("y");       //column 1
  analysisManager->CreateNtupleDColumn("z");       //column 2
  analysisManager->CreateNtupleIColumn("tag");  //column 3 
  analysisManager->FinishNtuple();  
  
  analysisManager->SetNtupleActivation(true); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
