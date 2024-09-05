#ifndef EVENT_ACTION_HH
#define EVENT_ACTION_HH

#include "G4UserEventAction.hh"
#include "G4THitsCollection.hh"
#include "MyHit.hh"

class EventAction : public G4UserEventAction {
public:
    EventAction();
    virtual ~EventAction() = default;

    virtual void EndOfEventAction(const G4Event* event) override;

private:
    G4int GetHitsCollectionID(const G4String& colName) const;
};

#endif // EVENT_ACTION_HH
