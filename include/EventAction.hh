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
/// \file EventAction.hh
/// \brief Definition of the EventAction class
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "RunAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction*);
   ~EventAction();

  public:
    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);  
    
    // boundary crossing counters
    G4int fCount_neutron_source2nitrogenBW;
    G4int fCount_neutron_shield2world;    
    G4int fCount_neutron_all2argon;
    G4int fCount_neutron_cryostat2world;
    G4int fCount_neutron_all2world;
                
  private:                  
  	RunAction* fRun;
  	
  	// event variables:
    G4double neutronEnergy_gen;  // DD neutron energy
    G4double neutronEnergy_exitshield; // neutrons exiting shield
    G4double neutronEnergy_enterArgon; // neutrons entering liquid argon
    G4double neutronEnergy_exitCryostat; // neutrons exiting cryostat
    
    //vector<G4double> gammaEnergy_exitshield; // gammas exiting shield
    //vector<G4double> gammaEnergy_enterArgon; // gammas entering liquid argon
    //vector<G4double> gammaEnergy_exitCryostat; // gammas exiting cryostat

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
