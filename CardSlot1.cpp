// CardSlot1.cpp
#pragma hdrstop
#include "CardSlot1.h"
#pragma package(smart_init)


CardSlot1::CardSlot1(Game* g, TImage* img, int index)
    : CardClasses(g, img, index)
{
}


void CardSlot1::PlayCard()
{
    this->SetState(asPlay);
    this->OnButtonClick();
}


void CardSlot1::OnButtonClick()
{

    if (!game) return;

    switch(currentState)
    {
        case asPlay:
            // Folosim SlotIndex (care știe singur dacă e 0, 1, 2 sau 3)
            // Modificăm în memorie la -1 (carte jucată)
            game->ModifyCard(SlotIndex, -1);

            // Ascundem imaginea asociată acestui slot
            if(image) image->Visible = false;
            break;

        case asSwap:
            // Logica de schimb
            game->PerformSwap(SlotIndex);
            break;

        case asPeek:
            // Aici nu facem nimic logic, e doar vizual în Unit3
            break;

        default:
            break;
    }

    // Resetăm starea
    currentState = asNone;
}
