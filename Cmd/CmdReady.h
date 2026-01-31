#ifndef CmdReadyH
#define CmdReadyH
#include "ServerCommandBase.h"

class CmdReady : public ServerCommand {
public:
    void Execute(TForm3* form, UnicodeString rawMsg) override;
};
#endif
