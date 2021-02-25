#ifndef PhantomHit_h
#define PhantomHit_h 1

#include <G4VHit.hh>
#include <G4THitsCollection.hh>
#include <G4Allocator.hh>
#include <G4ThreeVector.hh>
#include <TVector3.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PhantomHit : public G4VHit {
public:

        PhantomHit(); 
        ~PhantomHit();
        PhantomHit(const PhantomHit&);
        const PhantomHit& operator=(const PhantomHit&);
        int operator==(const PhantomHit&) const;

        inline void* operator new(size_t);
        inline void  operator delete(void*);

public:

    void inline SetEkin(G4double E){ Ekin = E; };
    void inline SetTime(G4double T){ Time = T; };
    void inline SetParticleID(G4int pid) { ParticleID = pid; };
    void inline SetPosition(TVector3 xyz){ Position.SetXYZ(xyz.X(),xyz.Y(),xyz.Z());};
    void inline SetCreatorProcess(G4String name){ CreatorProcess=name;};
    void inline SetMomentum(TVector3 xyz){ Momentum.SetXYZ(xyz.X(),xyz.Y(),xyz.Z());};
    void inline SetMotherEkin(G4double E){ MotherEkin = E; };
    void inline SetMotherParticleID(G4int pid) { MotherParticleID = pid; };
    void inline SetMotherMomentum(TVector3 xyz){ MotherMomentum.SetXYZ(xyz.X(),xyz.Y(),xyz.Z());};
        
    G4double inline GetEkin() { return Ekin; };
    G4double inline GetTime() { return Time; };
    G4int    inline GetParticleID() { return ParticleID ;};
    TVector3 inline GetPosition() { return Position; };
    TVector3 inline GetMomentum() { return Momentum; };
    G4String inline GetCreatorProcess() { return CreatorProcess; };
    
    G4double inline GetMotherEkin(){ return MotherEkin; };
    G4int    inline GetMotherParticleID(){ return MotherParticleID ;};
    TVector3 inline GetMotherMomentum() { return MotherMomentum; };

    void Print();

private:

    G4double      Ekin;
    G4double      Time;
    G4int         ParticleID;
    TVector3	  Position;
    TVector3	  Momentum;
    G4String 	  CreatorProcess;

    G4double      MotherEkin;
    G4int         MotherParticleID;
    TVector3	  MotherMomentum;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<PhantomHit> PhantomHitsCollection;

extern G4Allocator<PhantomHit> PhantomHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* PhantomHit::operator new(size_t) {
        void* aHit;
        aHit = (void*) PhantomHitAllocator.MallocSingle();
        return aHit;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PhantomHit::operator delete(void* aHit) {
        PhantomHitAllocator.FreeSingle((PhantomHit*) aHit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


