#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1


#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4CrystalBravaisLattices.h"
#include "G4CrystalLatticeSystems.h"
#include "G4CrystalExtension.hh"
#include "G4LogicalCrystalVolume.hh"
#include "G4ExtendedMaterial.hh"





class G4VPhysicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
  /// constructor
  DetectorConstruction();
  /// destructor
  virtual ~DetectorConstruction();
  
public:
  /// Defines detector geom. and returns a pointer to the physical World Volume
  virtual G4VPhysicalVolume* Construct();
  
private:  
  G4bool  fCheckOverlaps;
};

//...............................................................................
/*class G4CrystalUnitCell

{
public:
  G4CrystalUnitCell(G4double sizeA,
		    G4double sizeB,
		    G4double sizeC,
		    G4double alpha,
		    G4double beta,
		    G4double gamma,
		    G4int spacegroup);
  virtual ~G4CrystalUnitCell();
    
private:
  G4int theSpaceGroup; //
public:
  inline G4int GetSpaceGroup() const {return theSpaceGroup;};
  inline void SetSpaceGroup(G4int aInt)  {SetSpaceGroup=aInt;};

  private:
  theLatticeSystemType GetLatticeSystem(G4int aGroup);
  theBravaisLatticeType GetBravaisLattice(G4int aGroup);
  
   public:
  theLatticeSystemType GetLatticeSystem(){
     return GetLatticeSystem(theSpaceGroup);
   }
  theBravaisLatticeType GetBravaisLattice(){
     return GetBravaisLattice(theSpaceGroup);
   }
   };*/


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

