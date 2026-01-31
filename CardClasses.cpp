// CardClasses.cpp
#pragma hdrstop
#include "CardClasses.h"
#pragma package(smart_init)

// Constructor
CardClasses::CardClasses(Game* g, TImage* img, int index)
    : game(g), image(img), SlotIndex(index)
{
    currentState = asNone; // Stare inițială
}

// Destructor
CardClasses::~CardClasses() {
}

// Setarea stării
void CardClasses::SetState(ActionState newState) {
    currentState = newState;
}

// Verificarea cărții
bool CardClasses::CheckRightCard() {
    if (!game || SlotIndex >= (int)game->GetCards().size()) return false;

    int myID = game->GetCards()[SlotIndex].id;
    int pendingID = game->GetPendingCardID();

    int myRank = myID % 13;
    int pendingRank = pendingID % 13;

    // Verificăm doar numărul (Rank)
    if (myRank == pendingRank) {
        return true;
    }
    return false;
}

// Funcția PlayCard (setează starea și execută)
void CardClasses::PlayCard() {
    this->SetState(asPlay);
    this->OnButtonClick();
}


