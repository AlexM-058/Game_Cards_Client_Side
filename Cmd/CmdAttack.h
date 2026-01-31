#ifndef CmdAttackH
#define CmdAttackH
#include "ServerCommandBase.h"

class CmdAttack : public ServerCommand {
public:
    void Execute(TForm3* form, UnicodeString rawMsg) override;
};
#endif
