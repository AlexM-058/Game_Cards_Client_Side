#pragma hdrstop
#include "CmdAttack.h"
#include "../Unit3.h" // Path ajustat pentru parinte

#pragma package(smart_init)

void CmdAttack::Execute(TForm3* form, UnicodeString rawMsg) {
    int cardIndex = form->TCPClient1->Socket->ReadInt32();
    form->game.SetPendingCard(cardIndex);

    form->EnableAttackMode();

    form->ResetControls();
    form->Skip->Visible = true;
    form->Skip->BringToFront();

    UnicodeString resName = form->game.GetCardResourceName(cardIndex);
    TPngImage* img = form->ResManager->GetCardImage(resName);

    if (img) {
        form->CurrentCard->Picture->Assign(img);
        delete img;
    }

    form->PrepareForPlay();
    form->Image6->Visible = false;
    form->CurrentCard->Visible = true;
    form->CurrentCard->BringToFront();
    form->CurrentCard->Repaint();
}
