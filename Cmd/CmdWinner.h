#ifndef CmdWinnerH
#define CmdWinnerH
#include "ServerCommandBase.h"

class CmdWinner : public ServerCommand {
public:
    void Execute(TForm3* form, UnicodeString rawMsg) override;
};
#endif
