#ifndef CmdYourTurnH
#define CmdYourTurnH
#include "ServerCommandBase.h"

class CmdYourTurn : public ServerCommand {
public:
    void Execute(TForm3* form, UnicodeString rawMsg) override;
};
#endif
