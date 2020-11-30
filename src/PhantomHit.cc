#include "PhantomHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
                
G4Allocator<PhantomHit> PhantomHitAllocator;
                        
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
                        
PhantomHit::PhantomHit():
Ekin(0),
Time(0),
ParticleID(0),
Position(TVector3(0,0,0)),
Momentum(TVector3(0,0,0))

{
}                       
                        
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
                
PhantomHit::~PhantomHit(){}
                
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
PhantomHit::PhantomHit(const PhantomHit& right){
	Ekin=right.Ekin;
	Time=right.Time;
	ParticleID=right.ParticleID;
	Position=right.Position;
	Momentum=right.Momentum;



}               
                
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
const PhantomHit& PhantomHit::operator=(const PhantomHit& right){

	Ekin=right.Ekin;
	Time=right.Time;
	ParticleID=right.ParticleID;
	Position=right.Position;
	Momentum=right.Momentum;
        return *this;
}
