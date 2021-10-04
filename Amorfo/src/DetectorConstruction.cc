//Construcción del target, detector

#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//...........Librerias añadidas para estructura Cristalina................
#include "G4Material.hh"
#include "G4CrystalExtension.hh"
#include "G4ExtendedMaterial.hh"
#include "G4LogicalCrystalVolume.hh"
//........................................................................

//SensitiveDetector.......................................................
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSFlatSurfaceCurrent.hh"
#include "G4SDParticleFilter.hh"
#include "G4PSTrackLength.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4PSNofSecondary.hh"
//........................................................................
#include "G4SubtractionSolid.hh"

//Step size
#include "G4UserLimits.hh"

//......................De-exitation.....................................
#include "G4EmParameters.hh"
#include "G4EmLowEParameters.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
    fCheckOverlaps(true)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{delete fStepLimit; }


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Llamado a la base de datos del NIST
  G4NistManager* man = G4NistManager::Instance();

  //Definición del "vacío"
  G4double presion, temperatura, densidad;
  densidad     = universe_mean_density;    //from PhysicalConstants.h
  presion    = 3.e-18*pascal;
  temperatura = 2.73*kelvin;
  G4Material* Vacuum   = new G4Material("Vacuum",
                                        1., 1.01*g/mole, densidad,
                                        kStateGas,temperatura,presion);

  


  // World ()
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  G4double world_sizeXY = 10*cm;
  G4double world_sizeZ  = 16*cm;
  
  //Solid volume
  G4Box* solidWorld = new G4Box("SolWorld",
				0.5*world_sizeXY,
				0.5*world_sizeXY,
				0.5*world_sizeZ); //its size
  
  //Logical volume
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        Vacuum,         //its material
                        "LogWorld");            //its name
  
  //Physical volume
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "PhysWorld",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      fCheckOverlaps);       // checking overlaps
    
    
  //........................................CONSTRUCCION CRISTAL................................................
    
  
  //0000000000000oooooooooooo Material Cristal oooooooooooo0000000
  //Material del Cristal (NaI con impurezas de Tl)
      
    G4double z, a, fractionmass, density;
    G4String name, symbol;
    G4int ncomponents;
      
      
    G4double zLi, aLi;
    G4String nameLi, symboLi;
      
    G4double zMn, aMn;
    G4String nameMn, symbolMn;
      
    G4String nameCris;
    
      
    ////////Compuesto sin impureza //////////////////////
      
    G4Material* LiF = man->FindOrBuildMaterial("G4_LITHIUM_FLUORIDE");
      
      
    ////////Compuesto con impureza para la celda unitaria////////////
    
    a=18.9984*g/mole;
    G4Element* F = new G4Element (name="floruro", symbol="F", z=9, a);
    
    aLi=6.941*g/mole;
    G4Element* Li = new G4Element (nameLi="Litio", symboLi="Li", zLi=3, aLi);
    
    aMn=54.938*g/mole;
    G4Element* Mn = new G4Element (nameMn="Manganeso", symbolMn="Mn", zMn=24 , aMn);

    density = 2.64*g/cm3;
    G4Material* LiF_Mn = new G4Material(name="LiF(Mn)", density, ncomponents=3);
    LiF_Mn->AddElement(F, fractionmass=61.88*perCent);
    LiF_Mn->AddElement(Li, fractionmass=24.35*perCent);
    LiF_Mn->AddElement(Mn, fractionmass=13.77*perCent);
    
      
    /////////Compuesto total para el cristal//////////////////////
      
    G4double temperaturaF;
    temperaturaF = 573*kelvin;
    G4Material* TLD_LiF_Mn = new G4Material (nameCris="Compuesto_LiF(Mn)", density, ncomponents=2, kStateSolid, temperaturaF);
        TLD_LiF_Mn->AddMaterial(LiF, fractionmass=99.990*perCent);
        TLD_LiF_Mn->AddMaterial(LiF_Mn, fractionmass=0.001*perCent);

  
   //imprima Material

    G4cout << *(G4Material::GetMaterialTable()) << G4endl;

    //VolumenSolidoCristal.......................................
    G4Tubs* Cristal1 = new G4Tubs ("Cristal.LiFMn",
                                   0.00*cm,
                                   0.635*cm,
                                   1.27*cm,
                                   0*CLHEP::pi,
                                   2*CLHEP::pi);
      
      G4Box* Cristal2 = new G4Box ("Cristal.LiFMn2",
                       0.2*mm,
                       0.2*mm,
                       1.27*cm);
      
      G4SubtractionSolid* CrisSubtraction =
      new G4SubtractionSolid("CristalGrande", Cristal1, Cristal2);
    
    
  
    //VolumenLogicoCristal.........................
    G4LogicalVolume* crystalLogic =
      new G4LogicalVolume(CrisSubtraction,
                          TLD_LiF_Mn,
                          "Crystal_logic");
    
    //VolumenFisicoCristal.........................
         new G4PVPlacement(0,
                      G4ThreeVector(),
                      crystalLogic,
                      "Crystal_physical",
                      logicWorld,
                      false,
                           0);
    
    //.....................FINALIZACION CRISTAL...................................................................
    
    
    //.......................CONSTRUCCION NUCLEO DEL CRISTAL......................................................
    
    //Volumen Solido nucleo del cristal..................
    
    G4Box* NucleoCristal = new G4Box ("Cristal.LiFMn3",
                                      Crystal_x,
                                      Crystal_y,
                                      Crystal_z);

    
    //VolumenLogicoCristal.........................
    G4LogicalVolume* crystalLogic2 =
      new G4LogicalVolume(NucleoCristal,
                          TLD_LiF_Mn,
                                 "Crystal_logic2");
    
    //VolumenFisicoCristal.........................
         new G4PVPlacement(0,
                      G4ThreeVector(),
                      crystalLogic2,
                      "Crystal_physical2",
                      logicWorld,
                      false,
               0);
    //.......................FINALIZACION NUCLEO DEL CRISTAL......................................................
    
    
    //......................PLACA CONTADORA DE FLUJO DE PARTICULAS INCIDENTES..........................
    //Volumen solido placa contadora
    G4Box* Placa = new G4Box ("PlacaCont",
                              3.0*mm,
                              3.0*mm,
                              0.001*mm);
    
    //Volumen logico Placa
    G4LogicalVolume* LogPlaca =
        new G4LogicalVolume(Placa,
                            Vacuum,
                            "Placa_Logic");
    
    //Volumen fisico Placa
        new G4PVPlacement(0,
                          G4ThreeVector(0.0, 0.0, -2.10*cm),
                          LogPlaca,
                          "Phys_Placa",
                          logicWorld,
                          false,
                          0);
    
    //...................FIN DE PLACA CONTADORA DE FLUJO DE PARTICULAS INCIDENTES......................
    

 

  //retorna objeto apuntador a la clase G4VPhysicalVolume
  return physWorld; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{

  //...................................SDetectores cristal.........................................
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
  //auto absDetector = new G4MultiFunctionalDetector("CDetector");
  G4MultiFunctionalDetector* CrisDetector = new G4MultiFunctionalDetector("CDetector");
  G4SDManager::GetSDMpointer()->AddNewDetector(CrisDetector);
    
  //..........Contador(EnergiaDepositada)...................
  G4PSEnergyDeposit* scorer = new G4PSEnergyDeposit("EneDep");
  CrisDetector->RegisterPrimitive(scorer);

  //Hacer Sencible el Detector
  SetSensitiveDetector("Crystal_logic",CrisDetector);
    
  //..................................SDetectores Placa............................................
    G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
    G4MultiFunctionalDetector* PlacaDetector = new G4MultiFunctionalDetector("SD_Placa");
    G4SDManager::GetSDMpointer()->AddNewDetector(PlacaDetector);
    //............Contador Flujo en la placa...............
    G4PSFlatSurfaceCurrent* PlacaScorer = new G4PSFlatSurfaceCurrent("Flux", fCurrent_In);
    PlacaScorer->DivideByArea(false);
    PlacaDetector->RegisterPrimitive(PlacaScorer);
    
    //............HacerSencible el Placadetector
    SetSensitiveDetector("Placa_Logic", PlacaDetector);

  }









  
