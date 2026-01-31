#pragma hdrstop
#include "CmdWait.h"
#include "../Unit3.h"
#pragma package(smart_init)

void CmdWait::Execute(TForm3* form, UnicodeString rawMsg) {
    form->ButtonFinish->Enabled = false;
    form->Label2->Caption = "Asteapta...";
    form->Label2->Color = clBtnFace;
}
