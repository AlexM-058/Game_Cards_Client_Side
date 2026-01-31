#pragma hdrstop
#include "Game.h"

Game::Game()
{
    MyCards.clear();
    InitCards();
}

void Game::AddCard(int id)
{
  for (const Card& c : MyCards)
    {
        if (c.id == id)
        {
            GameCardsVector.push_back(c);
            break;
        }
    }
}

void Game::ClearCurrentHand()
{
    GameCardsVector.clear();
}

void Game::InitCards()
{
    // AICI RAMANE INITIALIZAREA EXACTA A CARTILOR TALE
    // (Copy-paste la lista ta lunga cu MyCards.push_back...)
    // ...
    MyCards.clear();
    int id = 1;
    // Clubs...
    MyCards.push_back(Card(id++, "c_2", 2));
    // ===== CLUBS =====
    MyCards.push_back(Card(id++, "c_2", 2));
    MyCards.push_back(Card(id++, "c_3", 3));
    MyCards.push_back(Card(id++, "c_4", 4));
    MyCards.push_back(Card(id++, "c_5", 5));
    MyCards.push_back(Card(id++, "c_6", 6));
    MyCards.push_back(Card(id++, "c_7", 7));
    MyCards.push_back(Card(id++, "c_8", 8));
    MyCards.push_back(Card(id++, "c_9", 9));
    MyCards.push_back(Card(id++, "c_10", 10));
	MyCards.push_back(Card(id++, "c_J", 11));
	MyCards.push_back(Card(id++, "c_Q", 12));
	MyCards.push_back(Card(id++, "c_K", 13));
	MyCards.push_back(Card(id++, "c_A", 1));

	// ===== HEARTS =====
	MyCards.push_back(Card(id++, "h_2", 2));
	MyCards.push_back(Card(id++, "h_3", 3));
	MyCards.push_back(Card(id++, "h_4", 4));
	MyCards.push_back(Card(id++, "h_5", 5));
	MyCards.push_back(Card(id++, "h_6", 6));
	MyCards.push_back(Card(id++, "h_7", 7));
	MyCards.push_back(Card(id++, "h_8", 8));
	MyCards.push_back(Card(id++, "h_9", 9));
	MyCards.push_back(Card(id++, "h_10", 10));
	MyCards.push_back(Card(id++, "h_J", 11));
	MyCards.push_back(Card(id++, "h_Q", 12));
	MyCards.push_back(Card(id++, "h_K", -1));
	MyCards.push_back(Card(id++, "h_A", 1));

	// ===== SPADES =====
	MyCards.push_back(Card(id++, "s_2", 2));
	MyCards.push_back(Card(id++, "s_3", 3));
	MyCards.push_back(Card(id++, "s_4", 4));
	MyCards.push_back(Card(id++, "s_5", 5));
	MyCards.push_back(Card(id++, "s_6", 6));
	MyCards.push_back(Card(id++, "s_7", 7));
	MyCards.push_back(Card(id++, "s_8", 8));
	MyCards.push_back(Card(id++, "s_9", 9));
	MyCards.push_back(Card(id++, "s_10", 10));
	MyCards.push_back(Card(id++, "s_J", 11));
	MyCards.push_back(Card(id++, "s_Q", 12));
	MyCards.push_back(Card(id++, "s_K", 13));
	MyCards.push_back(Card(id++, "s_A", 1));

	// ===== DIAMONDS =====
	MyCards.push_back(Card(id++, "d_2", 2));
	MyCards.push_back(Card(id++, "d_3", 3));
	MyCards.push_back(Card(id++, "d_4", 4));
	MyCards.push_back(Card(id++, "d_5", 5));
	MyCards.push_back(Card(id++, "d_6", 6));
	MyCards.push_back(Card(id++, "d_7", 7));
	MyCards.push_back(Card(id++, "d_8", 8));
	MyCards.push_back(Card(id++, "d_9", 9));
	MyCards.push_back(Card(id++, "d_10", 10));
	MyCards.push_back(Card(id++, "d_J", 11));
	MyCards.push_back(Card(id++, "d_Q", 12));
	MyCards.push_back(Card(id++, "d_K", -1));
	MyCards.push_back(Card(id++, "d_A", 1));
}

// --- FUNCTIA SIMPLIFICATA ---
UnicodeString Game::GetCardResourceName(int id)
{
    if (id < 1 || id > (int)MyCards.size())
        return "";


    return MyCards[id - 1].imgName;
}


void Game::SwapCardInHand(int index, const Card& newCard) {
    if (index >= 0 && index < (int)GameCardsVector.size()) {
		GameCardsVector[index] = newCard;
    }
}

void Game::ModifyCard(int index, int newId) {
    if (index >= 0 && index < (int)MyCards.size()) {
        MyCards[index].id = newId;
        if (newId == -1) {
			MyCards[index].value = 0;
        }
    }
}



void Game::SetPendingCard(int id) {
	this->pendingCardId = id;
}
	bool Game::IsQueen(int cardId)
{
    // Cautam cartea in pachetul complet MyCards
    for(const auto& c : MyCards) {
        if(c.id == cardId) {

			if (c.imgName.Pos("_Q") > 0 || c.imgName.Pos("_J") > 0 )
            {
                return true;
            }
        }
    }
    return false;
}
void Game::PerformSwap(int handSlotIndex) {
    if (handSlotIndex < 0 || handSlotIndex >= (int)GameCardsVector.size()) return;
    GameCardsVector[handSlotIndex].id = pendingCardId;

    for(const auto& c : MyCards) {
        if(c.id == pendingCardId) {
            GameCardsVector[handSlotIndex].imgName = c.imgName;
            GameCardsVector[handSlotIndex].value = c.value;
            break;
        }
    }
}

int Game::CalculateTotalScore() {
    int total = 0;
    for (const auto& card : GameCardsVector) {
        if (card.id == 0) continue;
        total += card.value;
    }
    return total;
}
