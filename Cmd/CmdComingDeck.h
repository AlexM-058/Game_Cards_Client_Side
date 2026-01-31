#ifndef CmdComingDeckH
#define CmdComingDeckH
#include "ServerCommandBase.h"

class CmdComingDeck : public ServerCommand {
public:
    void Execute(TForm3* form, UnicodeString rawMsg) override;
};
#endif
