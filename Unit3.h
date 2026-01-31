//---------------------------------------------------------------------------
#ifndef Unit3H
#define Unit3H
//---------------------------------------------------------------------------

// 1. Includeri Standard si Biblioteci
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Graphics.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

// 2. Includeri Specifice C++
#include <memory>
#include <vector>
#include <string>
#include <map>           // Necesar pentru CommandsMap
#include <pngimage.hpp>

// 3. Includeri Module Proprii
#include "Game.h"
#include "CardClasses.h"
#include "CardSlot1.h"
#include "GameModes.h"
#include "ResourceManager.h"       // Managerul de resurse grafice
#include "ServerCommandBase.h" // Interfata pentru comenzi

//---------------------------------------------------------------------------

class TForm3 : public TForm
{
__published:    // --- COMPONENTE VIZUALE (Gestionate de IDE) ---
    // Retea
    TIdTCPClient *TCPClient1;

    // Controale Conexiune
    TLabel *Label1;
    TLabel *LabelPlayer;
    TEdit *EFirstNumber;
    TEdit *EHost;
    TEdit *Edit1;
    TButton *TConnect;
    TButton *TDisconnect;

    // Interfata Joc
    TImage *Image1;
    TImage *Image2;
    TImage *Image3;
    TImage *Image4;
    TImage *Image5;
    TImage *Image6;
    TImage *CurrentCard;

    TLabel *Card1;
    TLabel *Card2;
    TLabel *Card3;
    TLabel *Card4;
    TLabel *LabelText;
    TLabel *Label2;

    // Butoane
    TButton *Play;
    TButton *ButtonFinish;
    TButton *Finish;
    TButton *Playfirst;
    TButton *Swap;
    TButton *Peek;
    TButton *Skip;

    // Butoane Sloturi
    TButton *QuickChange1;
    TButton *QuickChange2;
    TButton *QuickChange3;
    TButton *QuickChange4;

    // Timere
    TTimer *Timer1;
    TTimer *TimerPeek;
	TLabel *Label3;

    // --- EVENIMENTE GENERATE DE IDE ---
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);

    // Evenimente Retea
    void __fastcall Login(TObject *Sender);
    void __fastcall TConnectClick(TObject *Sender);
    void __fastcall TDisconnectClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);

    // Evenimente Gameplay
    void __fastcall PlayClick(TObject *Sender);
    void __fastcall PlayDirectClick(TObject *Sender);
    void __fastcall ButtonFinishClick(TObject *Sender);
    void __fastcall FinishClick(TObject *Sender);
    void __fastcall SkipClick(TObject *Sender);

    // Evenimente Moduri
    void __fastcall SwapClick(TObject *Sender);
    void __fastcall PeekClick(TObject *Sender);

    // Redenumit pentru a nu intra in conflict cu clasa AttackMode
    void __fastcall EnableAttackMode();

    // Evenimente Sloturi
    void __fastcall QuickChange1Click(TObject *Sender);
    void __fastcall QuickChange2Click(TObject *Sender);
    void __fastcall QuickChange3Click(TObject *Sender);
    void __fastcall QuickChange4Click(TObject *Sender);

    // Diverse
    void __fastcall BGetSumClick(TObject *Sender);
    void __fastcall TimerPeekTimer(TObject *Sender);

public:     // --- LOGICA DE JOC SI ARHITECTURA ---

    __fastcall TForm3(TComponent* Owner);
    __fastcall ~TForm3(); // Destructor pentru curatarea map-ului

    // 1. Obiecte Core
    Game game;
    CardSlot1* mySlots[4];

    // 2. Managere (Smart Pointers)
    std::unique_ptr<GameMode> CurrentMode;
    std::unique_ptr<ResourceManager> ResManager;

    // 3. Command Pattern (Retea)
    std::map<String, ServerCommand*> CommandsMap;
    void InitCommands();

    // 4. Helper Functions (GameMode)
    void SetMode(GameMode* newMode);
    void HandleSlotClick(int index);

    // Accesori UI pentru clase externe
    TImage* GetImageForSlot(int index);
    TButton* GetButtonForSlot(int index);

    // 5. Functii Utilitare Joc
    void SendPlayerMove(int cardID, bool endTurn = true);

    // Functii publice pentru a fi apelate de Comenzile din Cmd/
    void ProcessCards(const UnicodeString &cardData);
    void UpdateDebugLabels();
    void ReceiveCardsFromServer();       // <--- Aceasta lipseste sau e privata!
    void __fastcall ShowTwoCardsFor10Seconds();
    // 6. Control UI
    void ResetControls();
    void PrepareForPeek();
    void PrepareForPlay();

    // Variabile publice
    TImage* imgToFlipBack;
};

//---------------------------------------------------------------------------
extern PACKAGE TForm3 *Form3;
//---------------------------------------------------------------------------
#endif
