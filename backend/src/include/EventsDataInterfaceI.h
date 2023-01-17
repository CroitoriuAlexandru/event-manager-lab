#pragma once
#include "EventsDataSession.h"
#include "AuthInterfaceI.h"
#include "EventDataModule.h"

#include <Ice/Ice.h>
#include "AuthSession.h"
#include <Wt/Dbo/FixedSqlConnectionPool.h>

using namespace std;


class EventsDataInterfaceI : public EventDataModule::EventsDataInterface
{
public:
	EventsDataInterfaceI(std::unique_ptr<dbo::SqlConnection> conn, std::shared_ptr<AuthInterfaceI> authInterface);

	virtual void registerEvent(EventDataModule::StructRegisterEvent structRegisterEvent, const Ice::Current&) override;



private:
	EventsDataSession eventsDataSession_;
	std::shared_ptr<AuthInterfaceI> authInterface_;
};