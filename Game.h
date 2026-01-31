#ifndef GameH
#define GameH

#include <vector>
#include <string>
#include <vcl.h> // Doar pentru UnicodeString daca e cazul, sau std::string

// Structura Card ramane la fel
struct Card {
    int id;
    UnicodeString imgName; // Numele resursei (ex: "c_2")
    int value;

    Card(int i, UnicodeString n, int v) : id(i), imgName(n), value(v) {}
};

class Game {
private:
    std::vector<Card> MyCards;      // Pachetul complet (Baza de date)
    std::vector<Card> GameCardsVector; // Mana jucatorului
    int pendingCardId;

    void InitCards();

public:
    Game();

    // Functii de logica pura
    void AddCard(int id);
    void ClearCurrentHand();
    void SwapCardInHand(int index, const Card& newCard);
    void ModifyCard(int index, int newId);
    bool IsQueen(int cardId);
    void SetPendingCard(int id);
    int GetPendingCardID() { return pendingCardId; }
    void PerformSwap(int handSlotIndex);
    int CalculateTotalScore();

    // Getters pentru date
    const std::vector<Card>& GetCards() const { return GameCardsVector; }

    // --- FUNCTIA NOUA ---
    // In loc sa returneze poza, returneaza NUMELE resursei ("h_K", "c_2", etc.)
    UnicodeString GetCardResourceName(int id);
};

#endif
