// CardClasses.h
#ifndef CardClassesH
#define CardClassesH

#include <vcl.h>
#include <pngimage.hpp>
#include "Game.h"

// Definim stările posibile
enum ActionState {
    asNone,
    asPlay,
    asSwap,
    asPeek,
    asAttack
};

class CardClasses {
protected:
    Game* game;
    TImage* image;
    int SlotIndex;
    ActionState currentState; // Variabila de stare

public:
    // Constructor
    CardClasses(Game* g, TImage* img, int index);

    // Destructor virtual
    virtual ~CardClasses();

    // Funcții virtuale (care pot fi suprascrise)
    virtual void OnButtonClick() = 0; // Virtuală pură (obligatoriu de implementat în copii)

    // Funcții concrete (de bază)
    void SetState(ActionState newState);
    bool CheckRightCard();

    // Funcția PlayCard trebuie să fie virtuală pentru a putea fi apelată din copii
	virtual void PlayCard();

};

#endif
