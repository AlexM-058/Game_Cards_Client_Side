#pragma hdrstop
#include "CmdReady.h"
#include "../Unit3.h"
#pragma package(smart_init)

void CmdReady::Execute(TForm3* form, UnicodeString rawMsg) {
    TThread::Synchronize(nullptr, [&, form]() {
        form->LabelText->Caption = "Alt jucator este gata!";
    });
}
