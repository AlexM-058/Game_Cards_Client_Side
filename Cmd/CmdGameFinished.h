#ifndef CmdGameFinishedH
#define CmdGameFinishedH
#include "ServerCommandBase.h"

class CmdGameFinished : public ServerCommand {
public:
    void Execute(TForm3* form, UnicodeString rawMsg) override;
};
#endif
