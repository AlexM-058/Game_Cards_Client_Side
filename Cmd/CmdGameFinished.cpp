#pragma hdrstop

#include "CmdGameFinished.h"
#include "../Unit3.h"

#pragma package(smart_init)

void CmdGameFinished::Execute(TForm3* form, UnicodeString rawMsg) {
    form->ResetControls();
    form->SetMode(new NormalMode(form));
    form->TimerPeek->Enabled = false;

    form->Image6->Visible = false;
    form->CurrentCard->Visible = false;
    form->ButtonFinish->Visible = false;
    form->Skip->Visible = false;

    form->LabelText->Caption = "PACHET GOL! Se trimite scorul...";
    form->LabelText->Color = clRed;

    // Calculăm scorul folosind funcția din Game
    int myFinalScore = form->game.CalculateTotalScore();

    // Trimitem comanda și scorul separat (conform protocolului serverului)
    form->TCPClient1->Socket->WriteLn("MYPOINTS");
    form->TCPClient1->Socket->Write(myFinalScore);

    ShowMessage("Joc terminat! Ai acumulat: " + IntToStr(myFinalScore) + " puncte.");
}
