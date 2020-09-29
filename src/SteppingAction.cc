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
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"
#include "Run.hh"
#include "HistoManager.hh"

#include "G4RunManager.hh"
                           
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(EventAction* evt, TrackingAction* TrAct)
: G4UserSteppingAction(),fEventAction(evt), fTrackingAction(TrAct)
{ 
  //obtain the detector (needed for volumes)
  fDetector = static_cast<const DetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction()); 	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  // Get process information 
  const G4StepPoint* endPoint = step->GetPostStepPoint();
  const G4VProcess* process   = endPoint->GetProcessDefinedStep();
  G4String processName = process->GetProcessName();  
    
  // Count processes  
  Run* run = static_cast<Run*>(
        G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  run->CountProcesses(process);
    
  // Get step information
  const G4StepPoint* pre = step->GetPreStepPoint();   
  const G4StepPoint* post = step->GetPostStepPoint();
  const G4VPhysicalVolume* prePhysical = pre->GetPhysicalVolume();
  const G4VPhysicalVolume* postPhysical = post->GetPhysicalVolume();
  G4double x = post->GetPosition().x(), y = post->GetPosition().y(), z = post->GetPosition().z(); 		
  
  // Get track information
  G4Track* track = step->GetTrack();
  G4double ekin = track->GetKineticEnergy();
  G4double trackl = track->GetTrackLength();
  G4double time = track->GetLocalTime(); 
   
  // Sanity checks
  if(prePhysical == 0 || postPhysical == 0) return;  // The track does not exist  
  if(prePhysical->GetCopyNo() == -1 && postPhysical->GetCopyNo() == -1) return; // Both steps are in the World
  
  // Get logical volume
  const G4LogicalVolume* preLogical = pre->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  const G4LogicalVolume* postLogical = post->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
  	
  // Get particle name
  G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName();
  
  // Neutron passing through boundary
  if(particleName == "neutron" && post->GetStepStatus() == fGeomBoundary) {
  	
  	// Neutrons exiting collimator window
    if(preLogical == fDetector->fSourceVolume_l && postLogical == fDetector->fTestPlane1_l)	{
    	fEventAction->fCount_neutron_source2nitrogenBW++;
    	if(fEventAction->fCount_neutron_source2nitrogenBW==1) G4AnalysisManager::Instance()->FillH1(1,ekin);
    }
  	
  	// Neutrons exiting shield (from shield to world)
    if(preLogical == fDetector->fGammaShield_l && postLogical == fDetector->fWorld_l)	{
    	fEventAction->fCount_neutron_shield2world++;
    	if(fEventAction->fCount_neutron_shield2world==1) G4AnalysisManager::Instance()->FillH1(2,ekin);
    }
    
    // Neutrons entering liquid argon
    if(postLogical == fDetector->fPool_l)	{
    	fEventAction->fCount_neutron_all2argon++;
    	if(fEventAction->fCount_neutron_all2argon==1) G4AnalysisManager::Instance()->FillH1(3,ekin);
    }
    
    // Neutrons exiting cryostat (from cryostat to world)
    if(preLogical == fDetector->fSteelPlate_l && postLogical == fDetector->fWorld_l)	{
    	fEventAction->fCount_neutron_cryostat2world++;
    	if(fEventAction->fCount_neutron_cryostat2world==1) {
    		G4AnalysisManager::Instance()->FillH1(4,ekin);
    		G4AnalysisManager::Instance()->FillNtupleDColumn(0, 0, x/1000); // ID, column, value
    		G4AnalysisManager::Instance()->FillNtupleDColumn(0, 1, y/1000); // ID, column, value
    		G4AnalysisManager::Instance()->FillNtupleDColumn(0, 2, z/1000); // ID, column, value
    		G4AnalysisManager::Instance()->FillNtupleIColumn(0, 3, 0); //ID, column, tag
        G4AnalysisManager::Instance()->AddNtupleRow(0); 	
    	}
    }
    
    // Neutrons entering world
    if(postLogical == fDetector->fWorld_l)	{
    	fEventAction->fCount_neutron_all2world++;
    	if(fEventAction->fCount_neutron_all2world==1) G4AnalysisManager::Instance()->FillH1(5,ekin);
    }
    
    // Neutron entering test planes  
    if(postLogical == fDetector->fTestPlane1_l) G4AnalysisManager::Instance()->FillH2(0, x, y);
    if(postLogical == fDetector->fTestPlane2_l) G4AnalysisManager::Instance()->FillH2(1, x, y);
    if(postLogical == fDetector->fTestPlane3_l) G4AnalysisManager::Instance()->FillH2(2, x, y);      
    if(postLogical == fDetector->fTestPlane4_l) G4AnalysisManager::Instance()->FillH2(3, x, y); 
    if(postLogical == fDetector->fTestPlane5_l) G4AnalysisManager::Instance()->FillH2(4, x, z);
    if(postLogical == fDetector->fTestPlane6_l) G4AnalysisManager::Instance()->FillH2(5, x, z); 
      
  } 
  
  // Neutron capture
  if(particleName == "neutron" && processName == "nCapture" && postLogical == fDetector->fPool_l ) {
  	G4AnalysisManager::Instance()->FillH1(11,time); 	
    G4AnalysisManager::Instance()->FillNtupleDColumn(1, 0, x/1000); // ID, column, value
    G4AnalysisManager::Instance()->FillNtupleDColumn(1, 1, y/1000); // ID, column, value
    G4AnalysisManager::Instance()->FillNtupleDColumn(1, 2, z/1000); // ID, column, value
    G4AnalysisManager::Instance()->FillNtupleDColumn(1, 3, time); // ID, column, value
    G4AnalysisManager::Instance()->FillNtupleIColumn(1, 4, 0); //ID, column, tag
    G4AnalysisManager::Instance()->AddNtupleRow(1); 	
  }
  
  // Gammas passing through boundary
  if(particleName == "gamma" && post->GetStepStatus() == fGeomBoundary ) {
  	
  	// Gammas entering shield
  	if(preLogical == fDetector->fNeutronShield_l && postLogical == fDetector->fGammaShield_l)	{
    	G4AnalysisManager::Instance()->FillH1(6,ekin);
    }
  	
    // Gammas exiting shield
    if(preLogical == fDetector->fGammaShield_l && postLogical == fDetector->fWorld_l)	{
    	G4AnalysisManager::Instance()->FillH1(7,ekin);
    }
    
    // Gammas entering liquid argon
    if(postLogical == fDetector->fPool_l)	{
    	G4AnalysisManager::Instance()->FillH1(8,ekin);
    }
    
    // Gammas exiting cryostat
    if(preLogical == fDetector->fSteelPlate_l && postLogical == fDetector->fWorld_l)	{
    	G4AnalysisManager::Instance()->FillH1(9,ekin);
    	G4AnalysisManager::Instance()->FillNtupleDColumn(2, 0, x/1000); // ID, column, value
    	G4AnalysisManager::Instance()->FillNtupleDColumn(2, 1, y/1000); // ID, column, value
    	G4AnalysisManager::Instance()->FillNtupleDColumn(2, 2, z/1000); // ID, column, value
    	G4AnalysisManager::Instance()->FillNtupleIColumn(2, 3, 0); //ID, column, tag
      G4AnalysisManager::Instance()->AddNtupleRow(2); 	
    }
    
    // Gammas entering world
    if(postLogical == fDetector->fWorld_l)	{
    	G4AnalysisManager::Instance()->FillH1(10,ekin);
    }
    
    // Gamma test planes
    if(postLogical == fDetector->fTestPlane1_l) G4AnalysisManager::Instance()->FillH2(6, x, y);
    if(postLogical == fDetector->fTestPlane2_l) G4AnalysisManager::Instance()->FillH2(7, x, y);
    if(postLogical == fDetector->fTestPlane3_l) G4AnalysisManager::Instance()->FillH2(8, x, y);
    if(postLogical == fDetector->fTestPlane4_l) G4AnalysisManager::Instance()->FillH2(9, x, y);
    if(postLogical == fDetector->fTestPlane5_l) G4AnalysisManager::Instance()->FillH2(10, x, z);
    if(postLogical == fDetector->fTestPlane6_l) G4AnalysisManager::Instance()->FillH2(11, x, z);
    
  }
  
  // incident neutron
  //
  if (step->GetTrack()->GetTrackID() == 1) {     
    fTrackingAction->UpdateTrackInfo(ekin,trackl,time);
  }    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


