#pragma once
#include <Wt/WTemplate.h>
#include "LoginObj.h"
#include "EventForms.h"

class AddEventView : public Wt::WTemplate
{
public:
    AddEventView(std::shared_ptr<Login> login);

    std::shared_ptr<Login> login_;

private:
    EventFormView *eventForm_;

    void processFormsData();
    void addService();
};