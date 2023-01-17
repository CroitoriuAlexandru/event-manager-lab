#pragma once
#include "EventDataModule.h"

#include <Wt/WString.h>

#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/ptr.h>

#include <Wt/Dbo/SqlConnection.h>

class EventsDataSession
{
public:
    EventsDataSession(std::unique_ptr<Wt::Dbo::SqlConnection> conn);
    ~EventsDataSession();

    void configureSession();
    void registerEventData(EventDataModule::StructRegisterEvent structRegisterEvent);

private:
    mutable Wt::Dbo::Session session_;


};

