#ifndef BOOKINGCONTEXT_H
#define BOOKINGCONTEXT_H

#include "spot.h"

struct BookingContext {

    BookingContext() {
        current_spot = std::make_shared<TRAFFIK::Spot>();
    }

    void init_rules() {
    }

    std::shared_ptr<TRAFFIK::Spot> current_spot;
    std::map<QString, QWizardPage*> pages;
    std::map<QString, std::function<std::unique_ptr<QWizardPage>()>> page_types;

};


#endif