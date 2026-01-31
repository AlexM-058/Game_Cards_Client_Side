#ifndef GameModesH
#define GameModesH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

class TForm3;

class GameMode {
protected:
    TForm3* form;
public:
    GameMode(TForm3* f) : form(f) {}
    virtual ~GameMode() {}

    virtual void OnCardClicked(int slotIndex) = 0;
    virtual UnicodeString GetDescription() = 0;
};

class NormalMode : public GameMode {
public:
    NormalMode(TForm3* f) : GameMode(f) {}
    void OnCardClicked(int slotIndex) override;
    UnicodeString GetDescription() override;
};

class SwapMode : public GameMode {
public:
    SwapMode(TForm3* f) : GameMode(f) {}
    void OnCardClicked(int slotIndex) override;
    UnicodeString GetDescription() override;
};

class PeekMode : public GameMode {
public:
    PeekMode(TForm3* f) : GameMode(f) {}
    void OnCardClicked(int slotIndex) override;
    UnicodeString GetDescription() override;
};

class AttackMode : public GameMode {
public:
    AttackMode(TForm3* f) : GameMode(f) {}
    void OnCardClicked(int slotIndex) override;
    UnicodeString GetDescription() override;
};

#endif
