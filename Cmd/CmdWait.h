#ifndef CmdWaitH
#define CmdWaitH
#include "ServerCommandBase.h"

class CmdWait : public ServerCommand {
public:
    void Execute(TForm3* form, UnicodeString rawMsg) override;
};
#endif
