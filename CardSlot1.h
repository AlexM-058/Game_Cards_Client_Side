#ifndef CardSlot1H
#define CardSlot1H

#include "CardClasses.h"

class CardSlot1 : public CardClasses {
public:

    CardSlot1(Game* g, TImage* img, int index);


    void OnButtonClick() override;
    void PlayCard() override;
};

#endif
