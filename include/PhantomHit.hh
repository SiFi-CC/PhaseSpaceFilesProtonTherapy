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

    void SetEkin(G4double E){ Ekin = E; };
    void SetTime(G4double T){ Time = T; };
    void SetParticleID(G4int pid) { ParticleID = pid; };
    void SetPosition(TVector3 xyz){ Position.SetXYZ(xyz.X(),xyz.Y(),xyz.Z());};
    void SetMomentum(TVector3 xyz){ Momentum.SetXYZ(xyz.X(),xyz.Y(),xyz.Z());};
        
    G4double      GetEkin()            { return Ekin; };
    G4double      GetTime()            { return Time; };
    G4int 	  GetParticleID()     { return ParticleID ;};
    TVector3	  GetPosition()           { return Position; };
    TVector3	  GetMomentum()           { return Momentum; };


private:

    G4double      Ekin;
    G4double      Time;
    G4int         ParticleID;
    TVector3	  Position;
    TVector3	  Momentum;

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


