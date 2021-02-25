#include "PhantomHit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
                
G4Allocator<PhantomHit> PhantomHitAllocator;
                        
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
                        
PhantomHit::PhantomHit():
Ekin(0),
Time(0),
ParticleID(0),
Position(TVector3(0,0,0)),
Momentum(TVector3(0,0,0)),
CreatorProcess(""),
MotherEkin(0),
MotherParticleID(0),
MotherMomentum(TVector3(0,0,0))
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
	CreatorProcess=right.CreatorProcess;
	MotherEkin=right.MotherEkin;
	MotherParticleID=right.MotherParticleID;
	MotherMomentum=right.MotherMomentum;
}               
                
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
    
const PhantomHit& PhantomHit::operator=(const PhantomHit& right){

	Ekin=right.Ekin;
	Time=right.Time;
	ParticleID=right.ParticleID;
	Position=right.Position;
	Momentum=right.Momentum;
	CreatorProcess=right.CreatorProcess;
	MotherEkin=right.MotherEkin;
	MotherParticleID=right.MotherParticleID;
	MotherMomentum=right.MotherMomentum;
        return *this;
}
void PhantomHit::Print(){
	std::cout << "Ekin " << Ekin << std::endl;
	std::cout << "Time " << Time << std::endl;
	std::cout << "ParticleId " << ParticleID << std::endl;
	std::cout << "Process of creation " << CreatorProcess<< std::endl;
	std::cout << "Position " << Position.X() << ", " << Position.Y() << ", " << Position.Z() << std::endl;
	std::cout << "Momentum " << Momentum.X() << ", " << Momentum.Y() << ", " << Momentum.Z() << std::endl;
	std::cout << "MotherEkin " << MotherEkin << std::endl;
	std::cout << "MotherParticleId " << MotherParticleID << std::endl;
	std::cout << "MotherMomentum " << MotherMomentum.X() << ", " << MotherMomentum.Y() << ", " << MotherMomentum.Z() << std::endl;
}
