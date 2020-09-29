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
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"       
#include "G4VisAttributes.hh"

class G4LogicalVolume;
class G4Material;
class DetectorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction();

  public:
  
    virtual G4VPhysicalVolume* Construct();

    G4Material* 
    MaterialWithSingleIsotope(G4String, G4String, G4double, G4int, G4int);
         
    void SetWorldSize     (G4double);                        

  public:
     
     void               PrintParameters();
                       
  public:
  
     // Defined Materials 
     G4Material* fAir;   
     G4Material* fNitrogen;  
     G4Material* fGlasswool;
     G4Material* fVacuum;
     G4Material* fConcrete;
     G4Material* fPolyethylene;
     G4Material* fBPolyethylene;
     G4Material* fNeutronShield_mat;
     G4Material* fGammaShield_mat;
     G4Material* fPolyurethane;
     G4Material* fPolyurethane_light;
     G4Material* fProtoduneFoam;
     G4Material* fLiquidArgon;
     G4Material* fStainlessSteel;
     G4Material* fLead;
     G4Material* fIron;
     
     // Logical Volume     
     G4LogicalVolume*   fWorld_l;
     G4LogicalVolume*   fWall_l;
     G4LogicalVolume*   fPool_l;
     G4LogicalVolume*   fSteelPlate_l;
     G4LogicalVolume*   fFoam_l;
     G4LogicalVolume*   fNitrogenBW_l;
     G4LogicalVolume*   fGlasswoolBW_l;
     G4LogicalVolume*   fFoamBW_l;
     G4LogicalVolume*   fDDtube_l;
     G4LogicalVolume*   fDDelectronics_l;
     G4LogicalVolume*   fSourceVolume_l;
     G4LogicalVolume*   fNeutronShield_l;
     G4LogicalVolume*   fGammaShield_l;
     G4LogicalVolume*   fPlatform_l;
     G4LogicalVolume*   fTestPlane1_l;
     G4LogicalVolume*   fTestPlane2_l;
     G4LogicalVolume*   fTestPlane3_l;
     G4LogicalVolume*   fTestPlane4_l;
     G4LogicalVolume*   fTestPlane5_l;
     G4LogicalVolume*   fTestPlane6_l;
     
     // Physical Volume
     G4VPhysicalVolume* fWorld_p;
     G4VPhysicalVolume* fDDtube_p;
     G4VPhysicalVolume* fDDelectronics_p;
     G4VPhysicalVolume* fNeutronShield_p;
     
     // World
     G4double fWorldSize_x, fWorldSize_y, fWorldSize_z; 
     
     // EHN1 Wall
     G4double fWallThickness; 
     G4double fSurrConcrete_x, fSurrConcrete_y, fSurrConcrete_z;
     G4double fPitBox_x, fPitBox_y, fPitBox_z;
     
     // Crystat Steel plate
     G4double fSteelPlateThickness;
     G4double fSteelPlate_x, fSteelPlate_y, fSteelPlate_z;
     
     // Crystat Foam
     G4double fFoamThickness;
     G4double fFoam_x, fFoam_y, fFoam_z;
     
     
     // Liquid argon
     G4double fPool_x, fPool_y, fPool_z;
     
     
     // beam plug
     G4double fBeamPlugRadius;
     
     // DD tube
     G4double fDDtubeLength;
     G4double fDDtubeRadius;
     
     // DD electronics
     G4double fDDelectrnoics_x, fDDelectrnoics_y, fDDelectrnoics_z;
     
     // Neutron shield
     G4double fNeutronShieldThickness;
     
     // Gamma shield
     G4double fGammaShieldThickness;     
     
     // UI commands
     DetectorMessenger* fDetectorMessenger;

  private:
  	
     // Divide Construction Into Steps:
     void DefineMaterials();
     void ConstructWall();
     void ConstructCryostat();
     void ConstructPlatform();
     void ConstructDDGenerator(); 
     void ConstructNeutronShield();
     void ConstructGammaShield();
     void ConstructTestPlanes();
     void SetNeutronShieldMaterial(G4String value);
     
     // color
     G4VisAttributes* blue_;		
     G4VisAttributes* green_;
     G4VisAttributes* red_;		
     G4VisAttributes* grey_;		
     G4VisAttributes* silver_;
     G4VisAttributes* orange_;
     G4VisAttributes* copper_;
     G4VisAttributes* cyan_; 	
     G4VisAttributes* yellow_;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


#endif

