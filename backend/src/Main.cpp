#include "include/AuthInterfaceI.h"
#include "include/EventsDataInterfaceI.h"

#include <Wt/Dbo/FixedSqlConnectionPool.h>
#include <Wt/Dbo/backend/Sqlite3.h>

int main(int argc, char *argv[])
{

	/*
	Create Connection pool to a dbo and gives every interface a connection
	*/
	auto myPQconnectdb = std::make_unique<dbo::backend::Sqlite3>("myDbo.db");
	myPQconnectdb->setProperty("show-queries", "true");
	auto fixedConnPool = std::make_shared<dbo::FixedSqlConnectionPool>(std::move(myPQconnectdb), 2);

	try
	{
		Ice::CommunicatorHolder ich(argc, argv);
		auto adapter = ich->createObjectAdapterWithEndpoints("EventManagetAdapter", "default -p 10000");

		// Authentification Interface HERE
		auto authServant = make_shared<AuthInterfaceI>(std::move(fixedConnPool->getConnection()));

		// Events Data Interface HERE
		auto eventDataServant = make_shared<EventsDataInterfaceI>(std::move(fixedConnPool->getConnection()), authServant);

		// Add Servants to the adaptor
		adapter->add(authServant, Ice::stringToIdentity("Authentification"));
		adapter->add(eventDataServant, Ice::stringToIdentity("EventData"));

		adapter->activate();
		ich->waitForShutdown();
	}
	catch (const std::exception &e)
	{
		cerr << e.what() << endl;
		return 1;
	}
	return 0;
}
