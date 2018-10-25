#include "PhantomHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
                
G4Allocator<PhantomHit> PhantomHitAllocator;
                        
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
                        
PhantomHit::PhantomHit(){
	Position = new TVector3(0,0,0);
	Momentum = new TVector3(0,0,0);
}                       
                        
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
                
PhantomHit::~PhantomHit(){}
                
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
PhantomHit::PhantomHit(const PhantomHit& right){
	Ekin=right.Ekin;
	ParticleID=right.ParticleID;
	Position=right.Position;
	Momentum=right.Momentum;
}               
                
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
const PhantomHit& PhantomHit::operator=(const PhantomHit& right){

	Ekin=right.Ekin;
	ParticleID=right.ParticleID;
	Position=right.Position;
	Momentum=right.Momentum;
        return *this;
}
