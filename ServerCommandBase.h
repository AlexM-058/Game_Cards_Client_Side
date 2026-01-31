#ifndef ServerCommandBaseH
#define ServerCommandBaseH

#include <System.Classes.hpp>
#include <Vcl.Forms.hpp>

class TForm3;

class ServerCommand {
public:
    virtual ~ServerCommand() {}
    virtual void Execute(TForm3* form, UnicodeString rawMsg) = 0;
};

#endif
