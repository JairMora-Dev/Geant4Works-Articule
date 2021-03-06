//Construcción del target, detector

#include "DetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

///////////////////Librerias añadidas///////////////////////////////////
#include "G4Material.hh"
#include "G4CrystalUnitCell.hh"
#include "G4CrystalExtension.hh"
#include "G4ExtendedMaterial.hh"
#include "G4LogicalCrystalVolume.hh"

#include "MaterialExtensionData.hh"





//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
  : G4VUserDetectorConstruction(),
    fCheckOverlaps(true)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }


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
  G4double world_sizeXY = 30.0*mm;
  G4double world_sizeZ  = 30.0*mm;
  
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

  //AMORFO
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  //Material LiF
  G4Material* LiF1 = man-> FindOrBuildMaterial("G4_LITHIUM_FLUORIDE");
  

  
  //Volumen Solido
  G4Box* LiFBoxAmorfo = new G4Box ("amorfo_LiF",
			       1.*mm,
			       6.*mm,
			       1.*mm);

  //Logical volume
  G4LogicalVolume* logicAmorfo =                         
    new G4LogicalVolume(LiFBoxAmorfo,          //its solid
                        LiF1,         //its material
                        "LogAmorfo");            //its name
  
   //VolumenFisicoAmorfo.........................
   new G4PVPlacement(0,                       //Sin Rotación del F.Volumen
		     G4ThreeVector(),         //Hubicación (0,0,0)
		     logicAmorfo,      //VolumenLogicoAsociado
		     "physicAmorfo",        //NombreVPhysics
		     logicWorld,              //VolumenMadre
		     false,                   //No Boolean Operations
		     0,                       //Copy Number
		     fCheckOverlaps);         // checking overlaps
  
  
  //CRISTAL
  //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  
  //VolumenSolidoCristal.......................................
  G4Box* LiFBoxCristal = new G4Box ("CristalLiF",
			       1.*mm,
			       6.*mm,
			       1.*mm);
  
  
  //0000000000000oooooooooooo Material Cristal oooooooooooo0000000
  //Material del Cristal (LiF con impurezas de Mn)
  
     
  G4double z, a, fractionmass, density;
  G4String name, symbol;
  G4int ncomponents, natoms;
  
  //COMBINACIÓNSIMPLE-----------------------------LiF
  /* a = 18.9984*g/mole;
  G4Element* F = new G4Element(name="Fluor",symbol="F" , z= 10., a);
  //density = 0.001696*g/cm3;

  a = 6.941*g/mole;
  G4Element* Li = new G4Element(name="Litio",symbol="Li" , z= 2., a);
  //density = 0.534*g/cm3;

  
  a = 54.93*g/mole;
  G4Element* Mn = new G4Element(name="Manganeso",symbol="Mn" , z= 25., a);
  //density = 7.44*g/cm3;

  density= 2.64*g/cm3; 
  G4Material* LiFconMn = new G4Material(name="LiF_con_impureza_Mn",density,ncomponents=3);
  LiFconMn->AddElement(Li, fractionmass=26.7*perCent);
  LiFconMn->AddElement(F, fractionmass=73.2*perCent);
  LiFconMn->AddElement(Mn, fractionmass=0.02*perCent);*/

  
  //COMBINACIÓN MOLECULAR --------------------------LiF
   a = 18.9984*g/mole;
  G4Element* F = new G4Element(name="Fluor",symbol="F" , z= 9., a);
  //density = 0.001696*g/cm3;

  a = 6.941*g/mole;
  G4Element* Li = new G4Element(name="Litio",symbol="Li" , z= 3., a);
  //density = 0.534*g/cm3;

  density = 2.53*g/cm3;
  G4Material* LiF_Mol = new G4Material(name="LiF_Molecula", density, ncomponents=2);
  LiF_Mol->AddElement(F, natoms=1);
  LiF_Mol->AddElement(Li, natoms=1);
  
  a = 54.93*g/mole;
  G4Element* Mn = new G4Element(name="Manganeso",symbol="Mn" , z= 25., a);
  //density = 7.44*g/cm3;


  
  density= 2.64*g/cm3; 
  G4Material* LiFconMn = new G4Material(name="LiF_con_impureza_Mn",density,ncomponents=2);
  LiFconMn->AddMaterial(LiF_Mol, fractionmass=99.98*perCent);
  LiFconMn->AddElement(Mn, fractionmass=0.02*perCent);
  
  G4ExtendedMaterial* MaterialCrystal =
      new G4ExtendedMaterial("crystal.material",LiFconMn);
   
   //imprima Material
  G4cout << LiF_Mol;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

   
   //ContruccionCristal0000000000000000000000000000000OOOOOOOOooooooooooooooooooooo

   // DEfinición Celda Unitaria...............................................................................
   MaterialCrystal->RegisterExtension(std::unique_ptr<G4CrystalExtension>(
								  new G4CrystalExtension(MaterialCrystal)));
   
   G4CrystalExtension* crystalExtension = (G4CrystalExtension*)MaterialCrystal->RetrieveExtension("crystal");
   
   crystalExtension->SetUnitCell(
				 new G4CrystalUnitCell(2.887 * CLHEP::angstrom,     //Tamaño de Red en X             
						       2.887 * CLHEP::angstrom,     //Tamaño de Red en Y         
						       2.887 * CLHEP::angstrom,     //Tamaño de Red en Z         
						       CLHEP::pi/3,                 //Angulo de red en Alpha         
						       CLHEP::pi/3,                 //Angulo de red en Beta        
						       CLHEP::pi/3,                 //Angulo de red en Gamma        
						       225));                       //EspacioDeGrupo  CubicaCentrada en la Cara        
   
   //Conexion ExtendedMaterialData para info cristal de LiF con Mn......................................................................
    MaterialCrystal->RegisterExtension(std::unique_ptr<MaterialExtensionData>(new MaterialExtensionData("ExtendedData")));
    MaterialExtensionData* materialExtension = (MaterialExtensionData*)MaterialCrystal->RetrieveExtension("ExtendedData");
    materialExtension->SetValue(57.);
  
      
   //VolumenLogicoCristal.........................
   G4LogicalCrystalVolume* LiFBoxCristalLogic =
     new G4LogicalCrystalVolume(LiFBoxCristal,            //VolumenSolidoAsociado
				MaterialCrystal,          //ExtendedMaterialAsociado
				"crystal.logic");         //NombreVLogico

   
   //VolumenFisicoCristal.........................
   new G4PVPlacement(0,                       //Sin Rotación del F.Volumen
		     G4ThreeVector(0., 0., 2.*mm),         //Hubicación (0,0,2 mm)
		     LiFBoxCristalLogic,      //VolumenLogicoAsociado
		     "crystal.physic",        //NombreVPhysics
		     logicWorld,              //VolumenMadre
		     false,                   //No Boolean Operations
		     0,                       //Copy Number
		     fCheckOverlaps);         // checking overlaps
   
   
   
   
   
   
   //retorna objeto apuntador a la clase G4VPhysicalVolume
   return physWorld; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

