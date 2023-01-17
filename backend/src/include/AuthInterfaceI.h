#pragma once
#include "AuthModule.h"
#include <Ice/Ice.h>
#include "AuthSession.h"
#include <Wt/Dbo/FixedSqlConnectionPool.h>

using namespace std;
using namespace AuthModule;

class AuthInterfaceI : public AuthInterface
{
public:
	AuthInterfaceI(std::unique_ptr<dbo::SqlConnection> conn);

	virtual StructLoginReturn tryLogin(StructLoginInfo structLoginInfo, const Ice::Current&) override;
	virtual StructRegistrationReturn tryRegister(StructRegistrationInfo structRegistrationInfo, const Ice::Current&) override;
	virtual string getUserName(string userToken, const Ice::Current&) override;


	AuthSession authSession_;
private:

	
};