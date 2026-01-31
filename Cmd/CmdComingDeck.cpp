#pragma hdrstop
#include "CmdComingDeck.h"
#include "../Unit3.h"
#pragma package(smart_init)

void CmdComingDeck::Execute(TForm3* form, UnicodeString rawMsg) {
	form->Label2->Caption = "Primul pas";
    form->Label3->Caption = "Am ajuns in cmd";
    TThread::Synchronize(nullptr, [&, form]() {
        form->ReceiveCardsFromServer();
        form->ShowTwoCardsFor10Seconds();
    });
}
