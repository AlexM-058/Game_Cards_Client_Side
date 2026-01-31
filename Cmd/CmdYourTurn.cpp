#pragma hdrstop
#include "CmdYourTurn.h"
#include "Unit3.h"
#include <vcl.h>

#pragma package(smart_init)

void CmdYourTurn::Execute(TForm3* form, UnicodeString rawMsg)
{
    // 1. Resetam controalele
    form->SetMode(new NormalMode(form));
    form->ResetControls();

    form->Finish->Visible = true;
    form->Finish->Enabled = true;
    form->Label2->Caption = "E randul tau!";
    form->Label2->Color = clYellow;

    // 2. Citim cartea
    int cardIndex = form->TCPClient1->Socket->ReadInt32();
    form->game.SetPendingCard(cardIndex);

    // ---------------------------------------------------------
    // 3. AFISAM CARTEA (FOLOSIND IMAGE6)
    // ---------------------------------------------------------
    UnicodeString resName = form->game.GetCardResourceName(cardIndex);

    if (form->ResManager) {
        TPngImage* img = form->ResManager->GetCardImage(resName);
        if (img) {
            // AICI AM MODIFICAT: Folosim Image6 in loc de CurrentCard
            form->Image6->Picture->Assign(img);
            delete img;
        }
    }
    form->Image6->Visible = true; // O facem vizibila
    // ---------------------------------------------------------

    // 4. Logica de joc (Regina/Normal)
    if (form->game.IsQueen(cardIndex))
    {
        form->Peek->Visible = true;
        form->Swap->Visible = false;
        form->Playfirst->Visible = false;
        form->LabelText->Caption = "Ai tras o REGINA! Poti doar sa folosesti PEEK.";
    }
    else
    {
        form->Peek->Visible = false;
        form->Swap->Visible = true;
        form->Playfirst->Visible = true;
        form->LabelText->Caption = "Alege: Joaca cartea (Play) sau Schimba (Swap).";
    }
}
