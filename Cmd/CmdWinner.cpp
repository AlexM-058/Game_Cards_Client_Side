#pragma hdrstop
#include "CmdWinner.h"
#include "../Unit3.h"
#pragma package(smart_init)

void CmdWinner::Execute(TForm3* form, UnicodeString rawMsg) {
    UnicodeString winnerInfo = form->TCPClient1->Socket->ReadLn();
    form->LabelText->Caption = "CASTIGATOR: " + winnerInfo;
    form->LabelText->Color = clYellow;

    ShowMessage("JOCUL S-A INCHEIAT!\n\nCastigatorul este: " + winnerInfo);

    if (form->TCPClient1->Connected()) {
        form->TCPClient1->Disconnect();
    }
    form->Close();
}
