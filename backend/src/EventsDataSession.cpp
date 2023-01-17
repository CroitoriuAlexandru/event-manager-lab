#include "include/EventsDataSession.h"
#include "include/Event.h"


#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <Wt/WText.h>
#include <Wt/WDateTime.h>

#include "Wt/Auth/Dbo/AuthInfo.h" // i dont know why but i need this ????? for mapping tables to session

using namespace Wt;

namespace dbo = Wt::Dbo;


EventsDataSession::EventsDataSession(std::unique_ptr<dbo::SqlConnection> conn)
{
	session_.setConnection(std::move(conn));

	configureSession();
}

EventsDataSession::~EventsDataSession()
{
}

void EventsDataSession::configureSession()
{
	std::cout << "\n\n Configure Events Data Session STARTED \n\n";



	session_.mapClass<Event>("event");
	session_.mapClass<Client>("client");
	session_.mapClass<Service>("service");

	dbo::Transaction transaction(session_);

	try {
		session_.createTables();
		log("info") << "Database created";
	}
	catch (...) {
		log("info") << "Using existing database";
	}
	transaction.commit();


	std::cout << "\n\n Configure Events Data Session  ENDED \n\n";
}

void EventsDataSession::registerEventData(EventDataModule::StructRegisterEvent eventData)
{
	auto date = Wt::WDate().fromString(eventData.date, "dd/MM/yyyy");
	auto time = Wt::WTime().fromString(eventData.startHour, "HH:mm AP");
	auto dateTime = Wt::WDateTime(date, time);
	//std::cout << "\n\n -----" << duration.minute() << " ----- \n\n";

	dbo::Transaction transaction(session_);

	std::unique_ptr<Event> event{ new Event() };
	std::unique_ptr<Client> client{ new Client() };

	event->userId = eventData.userId;
	event->dateTime = dateTime.toTimePoint();
	event->duration = eventData.duration;
	event->location = eventData.location;
	event->observations = eventData.observations; 
	
	client->name = eventData.client.name;
	client->phone = eventData.client.phone;
	
	auto eventPtrDbo = session_.add(std::move(event));
	auto clientPtrDbo = session_.add(std::move(client));

	eventPtrDbo.modify()->client = clientPtrDbo;
	auto& eventServices = eventData.services;
	std::cout << "\n\n TOTAL SERVICES" << eventServices.size() << "\n\n";

	for (int i = 0; i < eventServices.size(); ++i) {

		auto& serviceData = eventServices.at(i);

		std::cout << "\n SERVICE " << i << "\n\n";
		std::cout << "\n providerIdentity = " << serviceData.providerIdentity;
		std::cout << "\n duration = " << serviceData.duration;
		std::cout << "\n cost = " << serviceData.cost;
		std::cout << "\n description = " << serviceData.description;
		std::cout << "\n observations = " << serviceData.observations;

		std::unique_ptr<Service> service{ new Service() };

		service->event = eventPtrDbo;
		service->providerIdentity = serviceData.providerIdentity;
		service->duration = serviceData.duration;
		service->cost = serviceData.cost;
		service->description = serviceData.description;
		service->observations = serviceData.observations;


		auto servicePtrDbo = session_.add(std::move(service));

		
	}

	transaction.commit();
}