#pragma hdrstop
#include "GameModes.h"
#include "Unit3.h"
#pragma package(smart_init)

// --- NORMAL MODE ---
void NormalMode::OnCardClicked(int slotIndex) {
    if (form->mySlots[slotIndex]) {
        form->mySlots[slotIndex]->OnButtonClick();
    }
}

UnicodeString NormalMode::GetDescription() {
    return "Asteapta randul...";
}

// --- SWAP MODE ---
void SwapMode::OnCardClicked(int slotIndex) {
    if (!form->mySlots[slotIndex]) return;

    int idDeAtac = form->game.GetCards()[slotIndex].id;

    form->mySlots[slotIndex]->SetState(asSwap);
    form->mySlots[slotIndex]->OnButtonClick();

    form->UpdateDebugLabels();

    TImage* img = form->GetImageForSlot(slotIndex);
    if (img) img->Repaint();

    form->SendPlayerMove(idDeAtac, true);

    form->SetMode(new NormalMode(form));
}

UnicodeString SwapMode::GetDescription() {
    return "MOD SWAP ACTIV: Alege o carte din mana pentru a face schimb.";
}

// --- PEEK MODE ---
void PeekMode::OnCardClicked(int slotIndex) {
    if (!form->mySlots[slotIndex]) return;

    int idDeVazut = form->game.GetCards()[slotIndex].id;
    int idRegina = form->game.GetPendingCardID();

    TImage* currentImage = form->GetImageForSlot(slotIndex);
    form->imgToFlipBack = currentImage;

    form->SendPlayerMove(idRegina, false);

  UnicodeString resName = form->game.GetCardResourceName(idDeVazut);

// 2. Incarcam imaginea folosind Managerul de Resurse
TPngImage* img = form->ResManager->GetCardImage(resName);
    if (img && currentImage) {
        currentImage->Picture->Assign(img);
        delete img;
        currentImage->Visible = true;
        currentImage->BringToFront();
        currentImage->Repaint();
    }

    form->TimerPeek->Enabled = true;
    form->LabelText->Caption = "Memoreaza cartea (5 sec)...";
}

UnicodeString PeekMode::GetDescription() {
    return "MOD PEEK ACTIV: Apasa pe o carte pe care vrei sa o vezi.";
}

// --- ATTACK MODE ---
void AttackMode::OnCardClicked(int slotIndex) {
    if (!form->mySlots[slotIndex]) return;

    auto* CarteaMea = form->mySlots[slotIndex];
    TImage* currentImage = form->GetImageForSlot(slotIndex);
    TButton* currentButton = form->GetButtonForSlot(slotIndex);

    if (CarteaMea->CheckRightCard()) {
        CarteaMea->PlayCard();

        const_cast<Card&>(form->game.GetCards()[slotIndex]).id = 0;

        if (currentImage) {
            currentImage->Picture->Assign(nullptr);
            currentImage->Visible = false;
            currentImage->Enabled = false;
        }
        if (currentButton) {
            currentButton->SendToBack();
        }
    } else {
        ShowMessage("Ai gresit cartea! Ai avut o singura sansa.");
    }

    form->TCPClient1->Socket->WriteLn("DONEAT");
    form->Skip->Visible = false;
    form->LabelText->Color = clBtnFace;
    form->CurrentCard->Visible = false;

    form->SetMode(new NormalMode(form));
    form->ResetControls();
}

UnicodeString AttackMode::GetDescription() {
    return "MOD ATTACK ACTIV: Apasa pe cartea care stii ca are acelasi numar.";
}
