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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4RunManager.hh"

#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:G4VUserDetectorConstruction(),
 fWorld_p(0), fWorld_l(0), fDetectorMessenger(0)
{
	// Dimensions
  fWorldSize_x = 60*m;
  fWorldSize_y = 60*m;
  fWorldSize_z = 60*m;
//  fWorldSize_x = 200*m;
//  fWorldSize_y = 200*m;
//  fWorldSize_z = 200*m;
  fWallThickness = 1.0*m;
  fSurrConcrete_x = 29.404*m;
  fSurrConcrete_y = 18.0*m;
  fSurrConcrete_z = 29.404*m;
  fPitBox_x = 16.50*m;
  fPitBox_y = 9.0*m;
  fPitBox_z = 22.0*m;
  fSteelPlateThickness = 1.0*cm;
  fSteelPlate_x = 10.16*m;
  fSteelPlate_y = 9.52*m;
  fSteelPlate_z = 10.16*m;
//  fSteelPlate_x = 100*m;
//  fSteelPlate_y = 100*m;
//  fSteelPlate_z = 100*m;
  fFoamThickness = 90.0*cm;
  fFoam_x = fSteelPlate_x - 2*fSteelPlateThickness;
  fFoam_y = fSteelPlate_y - 2*fSteelPlateThickness;
  fFoam_z = fSteelPlate_z - 2*fSteelPlateThickness;
  fPool_x = fFoam_x - 2*fFoamThickness;
  fPool_y = fFoam_y - 2*fFoamThickness;
  fPool_z = fFoam_z - 2*fFoamThickness;
  fDDtubeLength = 55.2*cm;
  fDDtubeRadius = 6.0*cm;
  fDDelectrnoics_x = 14.0*cm;
  fDDelectrnoics_y = 14.0*cm;
  fDDelectrnoics_z = 37.5*cm;
  fBeamPlugRadius = 11.0*cm;
  
  //shield thickness
  fNeutronShieldThickness = 15.0*cm;
  fGammaShieldThickness = 0.000001*cm;
  
  // materials
  DefineMaterials(); 
  
  // colors
	blue_		= new G4VisAttributes(G4Colour(0, 0.4, 0.8));
	green_	= new G4VisAttributes(G4Colour(0, 0.6, 0.3));
	red_		= new G4VisAttributes(G4Colour(0.82, 0.1, 0.1));
	grey_		= new G4VisAttributes(G4Colour(0.545, 0.533, 0.470));
	silver_	= new G4VisAttributes(G4Colour(0.625, 0.625, 0.625));
	orange_	= new G4VisAttributes(G4Colour(1,     0.549, 0));
	copper_	= new G4VisAttributes(G4Colour(0.722, 0.451, 0.2));
	cyan_ 	= new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
	yellow_ = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));
	
	// detector messenger
  fDetectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ delete fDetectorMessenger;}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
  // specific element name for thermal neutronHP
  // (see G4ParticleHPThermalScatteringNames.cc)
  
  G4NistManager* man = G4NistManager::Instance();
  G4Element* C = man->FindOrBuildElement("C");
  G4Element* H = man->FindOrBuildElement("H");
  G4Element* N = man->FindOrBuildElement("N");
  G4Element* O = man->FindOrBuildElement("O");
  G4Element* B = man->FindOrBuildElement("B");
  G4Element* Si = man->FindOrBuildElement("Si");  	
  G4Element* Al = man->FindOrBuildElement("Al"); 
  G4Element* Ca = man->FindOrBuildElement("Ca"); 
  G4Element* Mg = man->FindOrBuildElement("Mg"); 
  G4Element* Na = man->FindOrBuildElement("Na"); 
  // Air
  fAir = man->FindOrBuildMaterial("G4_AIR"); 
  // Nitrogen
  fNitrogen = man->FindOrBuildMaterial("G4_N"); 
  // Vacuum
  G4double density = fAir->GetDensity() * (1.0E-11);
  fVacuum = new G4Material("vacuum", density, 1);
  fVacuum->AddMaterial(fAir, 1.0); 
  
  // glasswool
  // SiO2
  G4Material* SiO2 = new G4Material("SiO2", 2.65*g/cm3, 2, kStateSolid);
  SiO2->AddElement(Si, 1);
  SiO2->AddElement(O, 2);
  //Al2O3
  G4Material* Al2O3 = new G4Material("Al203", 3.987*g/cm3, 2, kStateSolid);
  Al2O3->AddElement(Al, 2);
  Al2O3->AddElement(O, 3);
  // CaO
  G4Material* CaO = new G4Material("CaO", 3.35*g/cm3, 2, kStateSolid);
  CaO->AddElement(Ca, 1);
  CaO->AddElement(O, 1);
  // MgO
  G4Material* MgO = new G4Material("MgO", 3.58*g/cm3, 2, kStateSolid);
  MgO->AddElement(Mg, 1);
  MgO->AddElement(O, 1);
  // Na2O
  G4Material* Na2O = new G4Material("Na2O", 2.27*g/cm3, 2, kStateSolid);
  Na2O->AddElement(Na, 2);
  Na2O->AddElement(O, 1);
  
  fGlasswool = new G4Material("Glasswool", 0.035*g/cm3, 5);
  fGlasswool->AddMaterial(SiO2, 65*perCent);
  fGlasswool->AddMaterial(Al2O3, 9*perCent);
  fGlasswool->AddMaterial(CaO, 7*perCent);
  fGlasswool->AddMaterial(MgO, 3*perCent);
  fGlasswool->AddMaterial(Na2O, 16*perCent);
  
//  <material name="GlassWool">
//   <D value="0.035" unit="g/cm3"/>
//   <fraction n="0.65" ref="SiO2"/>
//   <fraction n="0.09" ref="Al2O3"/>
//   <fraction n="0.07" ref="CaO"/>
//   <fraction n="0.03" ref="MgO"/>
//   <fraction n="0.16" ref="Na2O"/>
//  </material>
   
  // Lead
  fLead = man->FindOrBuildMaterial("G4_Pb"); 
  
  // Iron
  fIron = man->FindOrBuildMaterial("G4_Fe");
  
  // Concrete
  fConcrete = man->FindOrBuildMaterial("G4_CONCRETE");  
  
  // Polyethylene
  fPolyethylene = man->FindOrBuildMaterial("G4_POLYETHYLENE"); 
   
  // Borated Polyethylene
  fBPolyethylene = new G4Material("BPolyethylene", 0.92*g/cm3, 2);
  G4Material* Boron = man->FindOrBuildMaterial("G4_B");
  fBPolyethylene->AddMaterial (Boron, 5*perCent);
  fBPolyethylene->AddMaterial (fPolyethylene, 95*perCent);
  
  // Polyurethane (foam Foam)
  fPolyurethane = new G4Material("Polyurethane", 0.088*g/cm3, 4);
  fPolyurethane->AddElement(C, 17);
  fPolyurethane->AddElement(H, 16);
  fPolyurethane->AddElement(N, 2);
  fPolyurethane->AddElement(O, 4); 
  
  // light Polyurethane
  fPolyurethane_light = new G4Material("Polyurethane_light", 0.009*g/cm3, 4);
  fPolyurethane_light->AddElement(C, 17);
  fPolyurethane_light->AddElement(H, 16);
  fPolyurethane_light->AddElement(N, 2);
  fPolyurethane_light->AddElement(O, 4); 
  
  // protodune foam
  fProtoduneFoam = new G4Material("ProtoduneFoam", 0.135*g/cm3, 4);
  fProtoduneFoam->AddElement(C, 17);
  fProtoduneFoam->AddElement(H, 16);
  fProtoduneFoam->AddElement(N, 2);
  fProtoduneFoam->AddElement(O, 4); 

//  from protodune_v5.gdml  

//  <material name="PolyurethaneFoam">
//   <D value="0.088" unit="g/cm3"/>
//   <composite n="17" ref="carbon"/>
//   <composite n="16" ref="hydrogen"/>
//   <composite n="2" ref="nitrogen"/>
//   <composite n="4" ref="oxygen"/>
//  </material>
//
//   <material name="ProtoDUNEFoam">
//   <D value="0.135" unit="g/cm3"/>
//   <composite n="17" ref="carbon"/>
//   <composite n="16" ref="hydrogen"/>
//   <composite n="2" ref="nitrogen"/>
//   <composite n="4" ref="oxygen"/>
//   </material>
//
//  <material name="LightPolyurethaneFoam">
//   <D value="0.009" unit="g/cm3"/>
//   <composite n="17" ref="carbon"/>
//   <composite n="16" ref="hydrogen"/>
//   <composite n="2" ref="nitrogen"s/>
//   <composite n="4" ref="oxygen"/>
//  </material>
  
  // Liquid argon
  fLiquidArgon = man->FindOrBuildMaterial("G4_lAr"); 
   
  // Stainless steel
  fStainlessSteel= man->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  
  // Neutron shield 
  fNeutronShield_mat = man->FindOrBuildMaterial("G4_POLYETHYLENE");  
  
  //G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Cleanup old geometry
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  G4Box* sBox = new G4Box("Container", fWorldSize_x/2,fWorldSize_y/2,fWorldSize_z/2);
  fWorld_l = new G4LogicalVolume(sBox, fAir, "World_l");
  fWorld_p = new G4PVPlacement(0, G4ThreeVector(), fWorld_l, "World_p", 0, false, 0);
  fWorld_l->SetVisAttributes(G4VisAttributes::GetInvisible());
  
  // Construct geometry
  ConstructWall();
  ConstructCryostat();
  ConstructPlatform();
  ConstructGammaShield();
  ConstructNeutronShield();
  ConstructDDGenerator();
  ConstructTestPlanes();
    
  return fWorld_p;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void DetectorConstruction::PrintParameters()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetWorldSize(G4double value)
{
  fWorldSize_z = value;
  G4RunManager::GetRunManager()->ReinitializeGeometry();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ConstructWall()
{
	// position
  G4double pos_x=0., pos_y=0., pos_z=0.;
  // Wall
	G4Box* sSurrConcrete = new G4Box("SurrConcrete", fSurrConcrete_x/2,fSurrConcrete_y/2, fSurrConcrete_z/2);  
  G4Box* sPitBox = new G4Box("PitBox", fPitBox_x/2,fPitBox_y/2+1.0, fPitBox_z/2); 
  G4ThreeVector zTransPitBox(1.33*m, 4.5*m,  -3.72*m);  
  G4SubtractionSolid* sWall = new G4SubtractionSolid("Wall_s", sSurrConcrete, sPitBox, 0, zTransPitBox); 
	pos_y = -fSteelPlate_y/2;
  fWall_l = new G4LogicalVolume(sWall, fConcrete, "Wall_l");
  new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), fWall_l, "Wall_p", fWorld_l, false, 0);
  fWall_l->SetVisAttributes(silver_); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructPlatform()
{
	// position
  G4double pos_x, pos_y, pos_z;
  
  // dimension
  G4double fPlatform_x = 2.0*m; //width
  G4double fPlatform_y = 3.0*cm; //thickness
  G4double fPlatform_z = 3.0*m; //length
  
  G4Box* sPlatform = new G4Box("DPlatform_s", fPlatform_x/2,fPlatform_y/2,fPlatform_z/2);
  fPlatform_l = new G4LogicalVolume(sPlatform, fStainlessSteel, "Platform_l");
  pos_x = 0;
  pos_y =  - 1.0*m;
  pos_z = -fSteelPlate_z/2-fPlatform_z/2;
  new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), 
                                       fPlatform_l, "Platform_p", fWorld_l, false, 0);
  fPlatform_l->SetVisAttributes(yellow_);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructCryostat()
{
  
  // Steel plate
  G4Box* sSteelPlate_outer = new G4Box("SteelPlate_outer", fSteelPlate_x/2,fSteelPlate_y/2, fSteelPlate_z/2);  
  G4Box* sSteelPlate_inner = new G4Box("SteelPlate_inner", fSteelPlate_x/2-fSteelPlateThickness,fSteelPlate_y/2-fSteelPlateThickness, fSteelPlate_z/2-fSteelPlateThickness); 
  G4SubtractionSolid* sSteelPlate1 = new G4SubtractionSolid("SteelPlate1", sSteelPlate_outer, sSteelPlate_inner); 
  G4Tubs* sSteelPlate_port = new G4Tubs("SteelPlate_port",0, fBeamPlugRadius, fSteelPlateThickness/2+10*cm, 0.,CLHEP::twopi );  
  G4ThreeVector zTransSteelPlate(0, 0, -fSteelPlate_z/2 + fSteelPlateThickness/2); 
  G4SubtractionSolid* sSteelPlate2 = new G4SubtractionSolid("SteelPlate2", sSteelPlate1, sSteelPlate_port, 0, zTransSteelPlate); 
  fSteelPlate_l = new G4LogicalVolume(sSteelPlate2, fStainlessSteel, "SteelPlate_l"); 
  new G4PVPlacement(0,G4ThreeVector(0, 0, 0), fSteelPlate_l, "SteelPlate_p", fWorld_l, false, 0); 
  fSteelPlate_l->SetVisAttributes(cyan_);
  
  // Foam insulator
  G4Box* sFoam_block = new G4Box("Foam_block", fFoam_x/2,fFoam_y/2, fFoam_z/2);  
  G4Tubs* sNitrogen_BW = new G4Tubs("Nitrogen_BW",0, fBeamPlugRadius, fFoamThickness/4, 0.,CLHEP::twopi );  
  G4ThreeVector zTransFoam(0, 0, -fFoam_z/2 + fFoamThickness/4); 
  G4SubtractionSolid* sFoam = new G4SubtractionSolid("Foam", sFoam_block, sNitrogen_BW, 0, zTransFoam); 
  fFoam_l = new G4LogicalVolume(sFoam, fProtoduneFoam, "Foam_l"); 
  new G4PVPlacement(0,G4ThreeVector(0, 0, 0), fFoam_l, "Foam_p", fWorld_l, false, 0); 
  fFoam_l->SetVisAttributes(cyan_);
  
  // Beam window nitrogen
  G4double NitrogenBW_z = fFoamThickness/2 + fSteelPlateThickness;
  G4Tubs* sNitrogenBW = new G4Tubs("NitrogenBW",0, fBeamPlugRadius, NitrogenBW_z/2, 0.,CLHEP::twopi );  
  fNitrogenBW_l = new G4LogicalVolume(sNitrogenBW, fNitrogen, "NitrogenBW_l"); 
  new G4PVPlacement(0,G4ThreeVector(0, 0, -fSteelPlate_z/2 + NitrogenBW_z/2), fNitrogenBW_l, "NitrogenBW_p", fWorld_l, false, 0); 
  fNitrogenBW_l->SetVisAttributes(red_);
  
  // Beam window glasswool
  G4double GlasswoolBW_z = 10.12*cm;
  G4Tubs* sGlasswoolBW = new G4Tubs("GlasswoolBW",0, fBeamPlugRadius, GlasswoolBW_z/2, 0.,CLHEP::twopi );  
  fGlasswoolBW_l = new G4LogicalVolume(sGlasswoolBW, fGlasswool, "GlasswoolBW_l");
  new G4PVPlacement(0,G4ThreeVector(0, 0, NitrogenBW_z/2-GlasswoolBW_z/2), fGlasswoolBW_l, "GlasswoolBW_p", fNitrogenBW_l, false, 0); 
  fGlasswoolBW_l->SetVisAttributes(yellow_);
  
  // Beam window foam
  G4Tubs* sFoamBW = new G4Tubs("FoamBW",0, fBeamPlugRadius, fFoamThickness/4, 0.,CLHEP::twopi );  
  fFoamBW_l = new G4LogicalVolume(sFoamBW, fPolyurethane_light, "FoamBW_l"); 
  new G4PVPlacement(0,G4ThreeVector(0, 0, -fFoam_z/2 + fFoamThickness*3/4), fFoamBW_l, "FoamBW_p", fFoam_l, false, 0); 
  fFoamBW_l->SetVisAttributes(cyan_);
  
  // Liquid Argon volumes
  G4Box* sLarPool = new G4Box("Larpool", fPool_x/2,fPool_y/2,fPool_z/2);
  fPool_l = new G4LogicalVolume(sLarPool, fLiquidArgon, "LarPool_l");
  new G4PVPlacement(0,G4ThreeVector(0, 0, 0), fPool_l, "LarPool_p", fFoam_l, false, 0);          
  fPool_l->SetVisAttributes(blue_);
  
  // Gaseous argon
  
  // ActiveTPC
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructGammaShield()
{
  // set default material
	//fGammaShield_mat = fLead;
	fGammaShield_mat = fAir;
	//fGammaShield_mat = fIron;
	
	// position
  G4double pos_x, pos_y, pos_z;	
  
  // gamma shield Dimension
  G4double GammaShield_x = std::max(fBeamPlugRadius*2, fDDelectrnoics_x) + fNeutronShieldThickness*2 + fGammaShieldThickness*2;
  G4double GammaShield_y = std::max(fDDtubeRadius, fBeamPlugRadius)*2 + fDDelectrnoics_y + fNeutronShieldThickness*2 + fGammaShieldThickness*2;
  G4double GammaShield_z = std::max(fDDtubeLength, fDDelectrnoics_z) + fNeutronShieldThickness*2 + fGammaShieldThickness*2;
  	
  G4cout<<"gamma shield size: "<<"x, y, z = "<<GammaShield_x/cm<<", "<<GammaShield_y/cm<<", "<<GammaShield_z/cm<<G4endl;
  
  // Neutron source volume                                      
  G4double fTestPlane1_z = 0.1*cm;
  G4Box* sSourceVolume = new G4Box("SourceVolume", GammaShield_x/2, GammaShield_y/2, GammaShield_z/2);
  fSourceVolume_l = new G4LogicalVolume(sSourceVolume, fAir, "SourceVolume_l");
  pos_x = 0;
  pos_y = fDDelectrnoics_y/2;
  pos_z = -fSteelPlate_z/2-GammaShield_z/2-fTestPlane1_z;
  new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), fSourceVolume_l, "SourceVolume_p", fWorld_l, false, 0);
  fSourceVolume_l->SetVisAttributes(grey_);
   
  // Gamma shield
  G4Box* gammaShield_block = new G4Box("GammaShield_block", GammaShield_x/2, GammaShield_y/2, GammaShield_z/2);
  G4double gammaport_z = fNeutronShieldThickness+fGammaShieldThickness;
  //G4Box* gammaport = new G4Box("GammaShield_port", fBeamPlugRadius, fBeamPlugRadius, gammaport_z/2+1.0*cm);
  G4Tubs* gammaport = new G4Tubs("GammaShield_port", 0, fDDtubeRadius, gammaport_z/2+1.0*cm, 0., CLHEP::twopi);
  G4ThreeVector zTransShield(0, -fDDelectrnoics_y/2, fDDtubeLength/2 + gammaport_z/2); 
  G4SubtractionSolid * sGammaShield1 = new G4SubtractionSolid("GammaShield1_s",gammaShield_block, gammaport, 0, zTransShield); 

  G4double NeutronShield_x = std::max(fBeamPlugRadius*2, fDDelectrnoics_x) + fNeutronShieldThickness*2;
  G4double NeutronShield_y = std::max(fDDtubeRadius, fBeamPlugRadius)*2 + fDDelectrnoics_y + fNeutronShieldThickness*2;
  G4double NeutronShield_z = std::max(fDDtubeLength, fDDelectrnoics_z) + fNeutronShieldThickness*2;
  G4Box* outer = new G4Box("NeutronShield_outer", NeutronShield_x/2, NeutronShield_y/2, NeutronShield_z/2);
  G4SubtractionSolid * sGammaShield = new G4SubtractionSolid("GammaShield_s",sGammaShield1,outer); 
   
  fGammaShield_l = new G4LogicalVolume(sGammaShield, fLead, "GammaShield_l");
  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), fGammaShield_l, "GammaShield_p", fSourceVolume_l, false, 0);
  fGammaShield_l->SetVisAttributes(orange_);  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructNeutronShield()
{
	// set default material
	//fNeutronShield_mat = fBPolyethylene;
	fNeutronShield_mat = fPolyethylene;
	//fNeutronShield_mat = fAir;
	
	// position
  G4double pos_x, pos_y, pos_z;
  
  // neutron shield material
  //SetNeutronShieldMaterial(fPolyethylene);
  
  // neutron shield dimension
  G4double NeutronShield_x = std::max(fBeamPlugRadius*2, fDDelectrnoics_x) + fNeutronShieldThickness*2;
  G4double NeutronShield_y = std::max(fDDtubeRadius, fBeamPlugRadius)*2 + fDDelectrnoics_y + fNeutronShieldThickness*2;
  G4double NeutronShield_z = std::max(fDDtubeLength, fDDelectrnoics_z) + fNeutronShieldThickness*2;
  G4double GammaShield_z = std::max(fDDtubeLength, fDDelectrnoics_z) + fNeutronShieldThickness*2 + fGammaShieldThickness*2;
  	
  // neutron shield geometry
  
  // shield plastic
  G4Box* outer = new G4Box("NeutronShield_outer", NeutronShield_x/2, NeutronShield_y/2, NeutronShield_z/2);
  G4Box* inner = new G4Box("NeutronShield_inner", NeutronShield_x/2-fNeutronShieldThickness, 
                            NeutronShield_y/2-fNeutronShieldThickness, NeutronShield_z/2-fNeutronShieldThickness);
  G4SubtractionSolid * nshield_s1 = new G4SubtractionSolid("NeutronShield_s1",outer,inner);
  //G4Box* port = new G4Box("NeutronShield_port", fBeamPlugRadius, fBeamPlugRadius, fNeutronShieldThickness/2+1.0*cm);
  G4Tubs* port = new G4Tubs("NeutronShield_port", 0, fDDtubeRadius, fNeutronShieldThickness/2+1.0*cm, 0., CLHEP::twopi);
  G4ThreeVector zTransShield(0, -fDDelectrnoics_y/2, fDDtubeLength/2+fNeutronShieldThickness/2); 
  G4SubtractionSolid * nshield_s2 = new G4SubtractionSolid("NeutronShield_s2",nshield_s1, port, 0, zTransShield);  
  fNeutronShield_l = new G4LogicalVolume(nshield_s2, fNeutronShield_mat, "NeutronShield_l");
  fNeutronShield_p = new G4PVPlacement(0, G4ThreeVector(0,0,0), fNeutronShield_l, "NeutronShield_p", fSourceVolume_l, false, 0);
  fNeutronShield_l->SetVisAttributes(grey_);
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructDDGenerator()
{
  // position
  G4double pos_x, pos_y, pos_z;
  
  // neutron DD generator tube                    
  G4Tubs* sDDtube = new G4Tubs("DDtube_s", 0, fDDtubeRadius, 0.5*fDDtubeLength, 0., CLHEP::twopi);
  fDDtube_l = new G4LogicalVolume(sDDtube, fVacuum, "DDtube_l");
  pos_x = 0;
  pos_y = -fDDelectrnoics_y/2;
  pos_z = 0.;
  fDDtube_p = new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), fDDtube_l, "DDtube_p", fSourceVolume_l, false, 0);
  fDDtube_l->SetVisAttributes(green_); 
  
  // neutron DD generator electronics box
  G4Box* sDDelectrnoics = new G4Box("DDelectrnoics_s", fDDelectrnoics_x/2,fDDelectrnoics_y/2,fDDelectrnoics_z/2);
  fDDelectronics_l = new G4LogicalVolume(sDDelectrnoics, fAir, "DDelectrnoics_l");
  pos_x = 0;
  pos_y = -fDDelectrnoics_y/2 + fDDtubeRadius + fDDelectrnoics_y/2;
  pos_z = 0.;
  fDDelectronics_p = new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), 
                                       fDDelectronics_l, "DDelectrnoics_p", fSourceVolume_l, false, 0);
  fDDelectronics_l->SetVisAttributes(green_); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructTestPlanes()
{
	// position
  G4double pos_x, pos_y, pos_z;
  
  // Test plane1, at the neutron exit
   G4double fPlatform_x = 2.0*m; //width
   G4double fPlatform_y = 3.0*cm; //thickness
   G4double fPlatform_z = 3.0*m; //length
   G4double fTestPlane1_x = 16.5*m;
   G4double fTestPlane1_y = fSteelPlate_y - 10*cm;
   G4double fTestPlane1_z = 0.1*cm;
   G4Box* sTestPlane1_1 = new G4Box("TestPlane1_s1", fTestPlane1_x/2,fTestPlane1_y/2,fTestPlane1_z/2);
   G4Box* sPlatform = new G4Box("DPlatform_s", fPlatform_x/2,fPlatform_y/2,fPlatform_z/2);
   G4ThreeVector zTransPlane1(-1.33*m, -1*m,  0); 
   G4SubtractionSolid* sTestPlane1 = new G4SubtractionSolid("TestPlane1_s", sTestPlane1_1, sPlatform, 0, zTransPlane1); 
   fTestPlane1_l = new G4LogicalVolume(sTestPlane1, fAir, "TestPlane1_l");
   pos_x = 1.33*m;
   pos_y = 0.*m;
   pos_z = -fSteelPlate_z/2-fTestPlane1_z/2;
   new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), 
                                        fTestPlane1_l, "TestPlane1_p", fWorld_l, false, 0);
   fTestPlane1_l->SetVisAttributes(red_); 
   
   // Test plane2, right behind the shield
   G4double GammaShield_z = std::max(fDDtubeLength, fDDelectrnoics_z) + fNeutronShieldThickness*2 + fGammaShieldThickness*2;
   G4double fTestPlane2_x = 16.5*m;
   G4double fTestPlane2_y = fSteelPlate_y - 10*cm;
   G4double fTestPlane2_z = 0.1*cm;
   G4Box* sTestPlane2_1 = new G4Box("TestPlane2_s1", fTestPlane2_x/2,fTestPlane2_y/2,fTestPlane2_z/2);
   G4ThreeVector zTransPlane2(-1.33*m, -1*m,  0); 
    
   G4SubtractionSolid* sTestPlane2 = new G4SubtractionSolid("TestPlane2_s", sTestPlane2_1, sPlatform, 0, zTransPlane2); 
   fTestPlane2_l = new G4LogicalVolume(sTestPlane2, fAir, "TestPlane2_l");
   pos_x = 1.33*m;
   pos_y = 0.*m;
   pos_z = -fSteelPlate_z/2-GammaShield_z-fTestPlane2_z/2;
   new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), 
                                       fTestPlane2_l, "TestPlane2_p", fWorld_l, false, 0);
   fTestPlane2_l->SetVisAttributes(red_); 
  
  // Test plane3, 3m from the cryostat
  G4double fTestPlane3_x = 16.5*m;
  G4double fTestPlane3_y = fSteelPlate_y - 10.0*cm;
  G4double fTestPlane3_z = 0.1*cm;
  G4Box* sTestPlane3 = new G4Box("TestPlane3_s", fTestPlane3_x/2,fTestPlane3_y/2,fTestPlane3_z/2);
  fTestPlane3_l = new G4LogicalVolume(sTestPlane3, fAir, "TestPlane3_l");
  pos_x = 1.33*m;
  pos_y = 0.*m;
  pos_z = -fSteelPlate_z/2-fTestPlane3_z/2-3.0*m; //3m from the cryostat
  new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), 
                                       fTestPlane3_l, "TestPlane3_p", fWorld_l, false, 0);
  fTestPlane3_l->SetVisAttributes(red_); 
  
  // Test plane4, 9m from the cryostat
  G4double fTestPlane4_x = 16.5*m;
  G4double fTestPlane4_y = fSteelPlate_y - 10.0*cm;
  G4double fTestPlane4_z = 0.1*cm;
  G4Box* sTestPlane4 = new G4Box("TestPlane4_s", fTestPlane4_x/2,fTestPlane4_y/2,fTestPlane4_z/2);
  fTestPlane4_l = new G4LogicalVolume(sTestPlane4, fAir, "TestPlane4_l");
  pos_x = 1.33*m;
  pos_y = 0.*m;
  pos_z = -fSteelPlate_z/2-fTestPlane4_z/2-9.0*m; //9 m from the cryostat
  new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), 
                                       fTestPlane4_l, "TestPlane4_p", fWorld_l, false, 0);
  fTestPlane4_l->SetVisAttributes(red_); 
  
  // Test plane5
  G4double fTestPlane5_x = 16.5*m;
  G4double fTestPlane5_y = 0.1*cm;
  G4double fTestPlane5_z = 9.0*m;
  G4Box* sTestPlane5 = new G4Box("TestPlane5_s", fTestPlane5_x/2,fTestPlane5_y/2,fTestPlane5_z/2);
  fTestPlane5_l = new G4LogicalVolume(sTestPlane5, fAir, "TestPlane5_l");
  pos_x = 1.33*m;
  pos_y = -fSteelPlate_y/2 + fTestPlane5_y/2;
  pos_z = -fSteelPlate_z/2-fTestPlane5_z/2; 
  new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z), 
                                       fTestPlane5_l, "TestPlane5_p", fWorld_l, false, 0);
  fTestPlane5_l->SetVisAttributes(red_); 
  
  // Test plane6
  G4double fTestPlane6_x = 16.5*m;
  G4double fTestPlane6_y = 0.1*cm;
  G4double fTestPlane6_z = fSteelPlate_z;
  G4Box* sTestPlane6 = new G4Box("TestPlane6_s", fTestPlane6_x/2,fTestPlane6_y/2,fTestPlane6_z/2);
  fTestPlane6_l = new G4LogicalVolume(sTestPlane6, fAir, "TestPlane6_l");
  pos_x = 1.33*m;
  pos_y = fSteelPlate_y/2 + fTestPlane6_y/2 + 5*cm;
  pos_z = 0.;
  new G4PVPlacement(0, G4ThreeVector(pos_x, pos_y, pos_z),
                                       fTestPlane6_l, "TestPlane6_p", fWorld_l, false, 0);
  fTestPlane6_l->SetVisAttributes(red_); 
 
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......