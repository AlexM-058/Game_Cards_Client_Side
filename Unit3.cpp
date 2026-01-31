
#include <vcl.h>
#pragma hdrstop
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <memory>
#include <System.Classes.hpp>
#include <JPEG.hpp>
#include <pngimage.hpp>

#include "Unit3.h"
#include "CardClasses.h"
#include "CardSlot1.h"
#include "GameModes.h"
#include "ResourceManager.h"

// Include commands
#include "Cmd/CmdAttack.h"
#include "Cmd/CmdYourTurn.h"
#include "Cmd/CmdReady.h"
#include "Cmd/CmdWait.h"
#include "Cmd/CmdComingDeck.h"
#include "Cmd/CmdGameFinished.h"
#include "Cmd/CmdWinner.h"

using std::string;
using std::vector;
using std::unique_ptr;

#pragma package(smart_init)
#pragma resource "*.dfm"

TForm3 *Form3;

//---------------------------------------------------------------------------
// 1. INITIALIZE COMMANDS
//---------------------------------------------------------------------------
void TForm3::InitCommands() {
    CommandsMap["READY"]        = new CmdReady();
    CommandsMap["COMINGDECK"]   = new CmdComingDeck();
    CommandsMap["YOURTURN"]     = new CmdYourTurn();
    CommandsMap["WAIT"]         = new CmdWait();
    CommandsMap["ATTACK"]       = new CmdAttack();
    CommandsMap["GAME_FINISHED_DECK_EMPTY"] = new CmdGameFinished();
    CommandsMap["WINNERIS"]     = new CmdWinner();
}

//---------------------------------------------------------------------------
// 2. CONSTRUCTOR
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
    : TForm(Owner)
{
    // 1. Stop timers
    TimerPeek->Enabled = false;
    Timer1->Enabled = false;
    imgToFlipBack = nullptr;

    // 2. INITIALIZE RESOURCES (CRITICAL)
    try {
        this->ResManager.reset(new class ResourceManager());
    } catch (const Exception &e) {
		ShowMessage("CRITICAL ERROR: Failed to create ResourceManager: " + e.Message);
    }

    if (this->ResManager) {
        this->ResManager->LoadBackground(Image1);
    } else {
        ShowMessage("WARNING: ResManager is NULL after initialization.");
    }


    mySlots[0] = new CardSlot1(&game, Image2, 0);
    mySlots[1] = new CardSlot1(&game, Image3, 1);
    mySlots[2] = new CardSlot1(&game, Image4, 2);
    mySlots[3] = new CardSlot1(&game, Image5, 3);

    // 4. Initialize Mode and Commands
	CurrentMode.reset(new NormalMode(this));
	InitCommands();

    // 5. Configure UI
    ButtonFinish->Visible = false;
    TDisconnect->Visible = false;
    EFirstNumber->Visible = false;
    Skip->Visible = false;

    Card1->Visible = false; Card2->Visible = false;
	Card3->Visible = false; Card4->Visible = false;

    Finish->Visible = false;
    CurrentCard->Visible = true;
    Play->Visible = false;
    LabelPlayer->Visible = false;

    // Configure Images
    Image1->Align = alClient;
    Image1->Stretch = true; Image1->Proportional = false;

    TImage* imgs[] = {Image2, Image3, Image4, Image5, Image6, CurrentCard};
    for(auto img : imgs) {
        img->Stretch = true;
        img->Proportional = true;
    }

    Playfirst->Visible = false; Swap->Visible = false; Peek->Visible = false;
    Edit1->Visible = false;
    QuickChange1->Visible = false; QuickChange2->Visible = false;
    QuickChange3->Visible = false; QuickChange4->Visible = false;

    // 6. Start Timer
    Timer1->Enabled = true;
}

__fastcall TForm3::~TForm3() {
    for (auto const& [key, cmd] : CommandsMap) {
        delete cmd;
    }
    CommandsMap.clear();
}

//---------------------------------------------------------------------------
// 3. RECEIVE CARDS LOGIC (DEBUG VERSION)
//---------------------------------------------------------------------------


void __fastcall TForm3::ReceiveCardsFromServer()
{
    if (!TCPClient1->Connected()) return;

    try {
        Label2->Caption = "Se impart cartile...";

        // 1. Curățăm bufferul și cerem cărți
        TCPClient1->Socket->InputBuffer->Clear();
        TCPClient1->Socket->WriteLn("PRIMITCARTI");

        game.ClearCurrentHand();

        // 2. Citim linia (Timeout 3 secunde)
        TCPClient1->Socket->ReadTimeout = 3000;
        UnicodeString line = TCPClient1->Socket->ReadLn();

       

        // Convertim în std::string pentru parsare sigură C++
        std::string rawLine = AnsiString(line).c_str();


        std::stringstream ss(rawLine);
        std::string segment;
        String debugLog = "Carti adaugate: ";

        while(std::getline(ss, segment, ',')) {
			// Curățăm spațiile eventuale
            segment.erase(0, segment.find_first_not_of(" \r\n"));
            segment.erase(segment.find_last_not_of(" \r\n") + 1);

            if (segment.empty()) continue;

            int id = std::stoi(segment); // Convertim string în int

            if (id > 0) {
                // A. ADĂUGĂM ÎN JOC (VECTOR)
                game.AddCard(id);
                debugLog += IntToStr(id) + " ";

                // B. TESTĂM IMEDIAT EXTRAGEREA DIN DLL (Doar test, nu afișăm încă)
                if (this->ResManager) {

                    UnicodeString resName = game.GetCardResourceName(id);

                    // Încercăm să încărcăm din DLL
                    TPngImage* testImg = this->ResManager->GetCardImage(resName);
                    if (testImg) {
                        delete testImg; // A mers, o ștergem momentan
                    } else {
                        ShowMessage("CRITIC: Nu pot extrage din DLL cartea ID: " + IntToStr(id) + " (" + resName + ")");
                    }
                }
            }
        }



        // 4. ACTUALIZARE UI (Afișăm Spatele Cărților)
        TThread::Synchronize(nullptr, [this]() {
            int count = game.GetCards().size();

            if (count < 4) {
                 ShowMessage("EROARE: Am doar " + IntToStr(count) + " carti! Ceva nu a mers la parsare.");
                 return;
            }

			UpdateDebugLabels();

            // Încărcăm spatele cărților (BackCard) din DLL
            if (this->ResManager) {
                TPngImage* back = this->ResManager->GetBackImage();
                if (back) {
                    Image2->Picture->Assign(back);
                    Image3->Picture->Assign(back);
                    Image4->Picture->Assign(back);
                    Image5->Picture->Assign(back);
                    delete back;
                } else {
                    ShowMessage("Nu pot incarca BackCard din DLL!");
                }
            }

            // Facem sloturile vizibile
            Image2->Visible = true;
            Image3->Visible = true;
            Image4->Visible = true;
            Image5->Visible = true;

            Label2->Caption = "Carti primite (" + IntToStr(count) + ")";
        });
    }
    catch (const Exception &e) {
        ShowMessage("Eroare CRITICA: " + e.Message);
    }
}//---------------------------------------------------------------------------
// 4. REST OF IMPLEMENTATION
//---------------------------------------------------------------------------

void __fastcall TForm3::ShowTwoCardsFor10Seconds()
{
    Image6->Visible = false;

    if (game.GetCards().size() < 2) return;

    int id1 = game.GetCards()[0].id;
    int id2 = game.GetCards()[1].id;

    UnicodeString name1 = game.GetCardResourceName(id1);
    UnicodeString name2 = game.GetCardResourceName(id2);

    // ShowMessage("DEBUG: Showing cards: " + name1 + ", " + name2);

    if (this->ResManager) {
        TPngImage* img1 = this->ResManager->GetCardImage(name1);
        TPngImage* img2 = this->ResManager->GetCardImage(name2);

        if (img1) { Image2->Picture->Assign(img1); delete img1; }
        else ShowMessage("DEBUG ERROR: Failed to load image: " + name1);

        if (img2) { Image3->Picture->Assign(img2); delete img2; }
        else ShowMessage("DEBUG ERROR: Failed to load image: " + name2);
    }

	TThread::CreateAnonymousThread([this]() {
		Sleep(3000);
		TThread::Synchronize(nullptr, [this]() {
			if(!this->ResManager) return;

			TPngImage* backImg = this->ResManager->GetBackImage();
			if (backImg) {
				Image2->Picture->Assign(backImg);
				Image3->Picture->Assign(backImg);
				delete backImg;
				Image6->Visible = true;
			}
		});
	})->Start();
}

void __fastcall TForm3::ProcessCards(const UnicodeString &cardData)
{
	std::unique_ptr<TStringList> list(new TStringList());
	list->CommaText = cardData;
	if (list->Count >= 4) {
		Card1->Caption = list->Strings[0]; Card2->Caption = list->Strings[1];
		Card3->Caption = list->Strings[2]; Card4->Caption = list->Strings[3];

		if (this->ResManager) {
			TPngImage* back = this->ResManager->GetBackImage();
			if (back) {
				Image5->Picture->Assign(back); Image2->Picture->Assign(back);
				Image3->Picture->Assign(back); Image4->Picture->Assign(back);
				delete back;
			}
		}
	}
}

void TForm3::SendPlayerMove(int cardID, bool endTurn)
{
    TCPClient1->Socket->WriteLn("STARTPLAYINGTHERIGHTCARD");
    TCPClient1->Socket->Write(cardID);

    ResetControls();
    SetMode(new NormalMode(this));
    LabelText->Caption = "Se asteapta sincronizarea...";
}

// --- NETWORK CONNECTION ---
void __fastcall TForm3::Login(TObject *Sender)
{
    TCPClient1->Host = EHost->Text;
    if (TCPClient1->Connected()) {
        try { TCPClient1->Disconnect(); } catch (...) {}
	}
	try {
        TCPClient1->Connect();
        int myID = TCPClient1->Socket->ReadInt32();
        LabelPlayer->Caption = "Conectat ca Player: " + IntToStr(myID);
        TConnect->Visible = false; TDisconnect->Visible = true;
        Play->Visible = true; LabelPlayer->Visible = true;
        Label1->Visible = false; EHost->Visible = false;
    }
    catch (const Exception &e) {
		ShowMessage("Eroare conectare: " + e.Message);
    }
}

void __fastcall TForm3::TConnectClick(TObject *Sender) { Login(Sender); }

void __fastcall TForm3::TDisconnectClick(TObject *Sender)
{
    if (TCPClient1->Connected()) {
        try { TCPClient1->Socket->WriteLn("DECONECTARE"); Sleep(50); } catch (...) {}
    }
    try {
        if (TCPClient1->Connected()) { TCPClient1->Disconnect(); Sleep(100); }
    } catch (...) {}
    Close();
}

void __fastcall TForm3::FormClose(TObject *Sender, TCloseAction &Action)
{
    if (TCPClient1->Connected()) {
        try {
            TCPClient1->Socket->WriteLn("SECRET_KEY_DISCONNECT");
            TCPClient1->Disconnect();
        } catch (...) {}
    }
}

// --- DISPECER MESAJE ---
void __fastcall TForm3::Timer1Timer(TObject *Sender)
{
    if (!TCPClient1->Connected()) return;

    while (!TCPClient1->Socket->InputBufferIsEmpty())
    {
        UnicodeString msg = TCPClient1->Socket->ReadLn();

        if (CommandsMap.count(msg)) {
			CommandsMap[msg]->Execute(this, msg);
        }
        else if (msg.Pos("PRIMITCARTI") == 1) {
            UnicodeString cardData = msg.SubString(11, msg.Length() - 10);
            ProcessCards(cardData);
        }
    }
}

void __fastcall TForm3::TimerPeekTimer(TObject *Sender)
{
    TimerPeek->Enabled = false;

    if (imgToFlipBack != nullptr) {
        if (this->ResManager) {
            TPngImage* backImg = this->ResManager->GetBackImage();
            if (backImg) {
                try {
                    imgToFlipBack->Picture->Assign(backImg);
                    imgToFlipBack->Repaint();
                } catch (...) {}
                delete backImg;
            }
        }
    }
	imgToFlipBack = nullptr;
    if (TCPClient1->Connected())
    {
	   // Trimitem comanda de "Joc cartea"
        TCPClient1->Socket->WriteLn("STARTPLAYINGTHERIGHTCARD");


        int queenID = game.GetPendingCardID();
        TCPClient1->Socket->Write(queenID);
    }

   // -----------------------------------------------------------
	// 3. INTRAM IN STATE-UL DE ATTACK
	// -----------------------------------------------------------
	Peek->Visible = false;
    Image6->Visible = true;


	SetMode(new AttackMode(this));


    Skip->Visible = true;


	LabelText->Caption = "Regina jucata! Poti ataca, pune peste sau apasa 'skip'.";
}


void TForm3::UpdateDebugLabels()
{
    if (game.GetCards().size() >= 4) {
        Card1->Caption = IntToStr(game.GetCards()[0].id);
        Card2->Caption = IntToStr(game.GetCards()[1].id);
        Card3->Caption = IntToStr(game.GetCards()[2].id);
        Card4->Caption = IntToStr(game.GetCards()[3].id);
        Card1->Visible = true; Card2->Visible = true;
        Card3->Visible = true; Card4->Visible = true;
    }
}

// --- UI HELPERS & EVENTS ---
void TForm3::SetMode(GameMode* newMode) {
    CurrentMode.reset(newMode);
    LabelText->Caption = CurrentMode->GetDescription();
}

TImage* TForm3::GetImageForSlot(int index) {
    switch (index) {
        case 0: return Image2; case 1: return Image3;
        case 2: return Image4; case 3: return Image5;
        default: return nullptr;
    }
}

TButton* TForm3::GetButtonForSlot(int index) {
    switch (index) {
        case 0: return QuickChange1; case 1: return QuickChange2;
        case 2: return QuickChange3; case 3: return QuickChange4;
        default: return nullptr;
    }
}

void __fastcall TForm3::PlayClick(TObject *Sender) {
    if (TCPClient1->Connected()) { try { TCPClient1->Socket->WriteLn("READY"); Play->Visible = false; } catch (...) {} }
}

void __fastcall TForm3::PlayDirectClick(TObject *Sender) {
    int pendingID = game.GetPendingCardID();
    SendPlayerMove(pendingID);
    ResetControls();
}

void __fastcall TForm3::ButtonFinishClick(TObject *Sender) {
    TCPClient1->Socket->WriteLn("DONE"); ButtonFinish->Enabled = false; Image6->Visible = false;
}

void __fastcall TForm3::SwapClick(TObject *Sender) {
    SetMode(new SwapMode(this));
    QuickChange1->Caption = "Alege"; QuickChange2->Caption = "Alege";
    QuickChange3->Caption = "Alege"; QuickChange4->Caption = "Alege";
    QuickChange1->Visible = true; QuickChange2->Visible = true;
    QuickChange3->Visible = true; QuickChange4->Visible = true;
}

void __fastcall TForm3::PeekClick(TObject *Sender) {
    SetMode(new PeekMode(this));
    QuickChange1->Caption = "Peek"; QuickChange1->Visible = true;
    QuickChange2->Caption = "Peek"; QuickChange2->Visible = true;
    QuickChange3->Caption = "Peek"; QuickChange3->Visible = true;
    QuickChange4->Caption = "Peek"; QuickChange4->Visible = true;
}

void __fastcall TForm3::EnableAttackMode() { SetMode(new AttackMode(this)); }

void TForm3::ResetControls() {
    Playfirst->Visible = false; Swap->Visible = false; Peek->Visible = false;
    QuickChange1->Visible = false; QuickChange2->Visible = false;
    QuickChange3->Visible = false; QuickChange4->Visible = false;
    Finish->Visible = false;
    LabelText->Caption = "Asteapta randul...";
}

void TForm3::PrepareForPlay() {
    for(int i = 0; i < 4; i++) { if(mySlots[i]) mySlots[i]->SetState(asPlay); }
    QuickChange1->Caption = "Play Card"; QuickChange1->Visible = true;
    QuickChange2->Caption = "Play Card"; QuickChange2->Visible = true;
    QuickChange3->Caption = "Play Card"; QuickChange3->Visible = true;
    QuickChange4->Caption = "Play Card"; QuickChange4->Visible = true;
    LabelText->Caption = "Daca stii ca ai aceeasi carte, joac-o"; LabelText->Visible = true;
}

void __fastcall TForm3::PrepareForPeek() {
    for(int i = 0; i < 4; i++) { mySlots[i]->SetState(asPeek); }
    QuickChange1->Caption = "Peek"; QuickChange2->Caption = "Peek";
    QuickChange3->Caption = "Peek"; QuickChange4->Caption = "Peek";
}

void TForm3::HandleSlotClick(int index)
{
    if (index < 0 || index > 3 || !mySlots[index]) return;

    if (Peek->Visible == true)
    {
        this->imgToFlipBack = GetImageForSlot(index);

        TimerPeek->Interval = 5000;
        TimerPeek->Enabled = true;

        int cardID = game.GetCards()[index].id;
        UnicodeString resName = game.GetCardResourceName(cardID);

        if (this->ResManager) {
            TPngImage* face = this->ResManager->GetCardImage(resName);
            if (face) {
                this->imgToFlipBack->Picture->Assign(face);
                delete face;
            }
        }
        return;
    }

    if (CurrentMode) {
        CurrentMode->OnCardClicked(index);
    }
}
void __fastcall TForm3::QuickChange1Click(TObject *Sender) { HandleSlotClick(0); }
void __fastcall TForm3::QuickChange2Click(TObject *Sender) { HandleSlotClick(1); }
void __fastcall TForm3::QuickChange3Click(TObject *Sender) { HandleSlotClick(2); }
void __fastcall TForm3::QuickChange4Click(TObject *Sender) { HandleSlotClick(3); }

void __fastcall TForm3::BGetSumClick(TObject *Sender) { TCPClient1->Socket->WriteLn("CALCULEAZA"); }

void __fastcall TForm3::SkipClick(TObject *Sender) {
    TCPClient1->Socket->WriteLn("DONEAT"); SetMode(new NormalMode(this)); LabelText->Color = clBtnFace; ResetControls(); Skip->Visible = false; CurrentCard->Visible = false;
}

void __fastcall TForm3::FinishClick(TObject *Sender) {
  if (TCPClient1->Connected()) {
        TCPClient1->Socket->WriteLn("PLAYER_CALLED_FINISH");
        ButtonFinish->Enabled = false;
        LabelText->Caption = "Ai cerut incheierea jocului...";
    }
}
