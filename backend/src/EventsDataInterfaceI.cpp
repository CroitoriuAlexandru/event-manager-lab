#include "include/EventsDataInterfaceI.h"


EventsDataInterfaceI::EventsDataInterfaceI(std::unique_ptr<dbo::SqlConnection> conn, std::shared_ptr<AuthInterfaceI> authInterface)
	: eventsDataSession_(std::move(conn)),
	authInterface_(authInterface)
{

}

void EventsDataInterfaceI::registerEvent(EventDataModule::StructRegisterEvent eventData, const Ice::Current&)
{
	Wt::WString userToken(eventData.userToken);
	eventData.userId = authInterface_->authSession_.getUserIdFromToken(userToken);
	std::cout << "\n\n EventsDataInterfaceI::registerEvent \n\n";

	eventsDataSession_.registerEventData(eventData);
	
}