#include "include/LoginObj.h"
#include <Wt/WApplication.h>

Login::Login()
{
	user_.loginResponse = AuthModule::LoginResponse::NotIdentified;
	user_.userName = "";
	user_.userToken = "";
}

void Login::login(AuthModule::StructLoginReturn structLoginReturn)
{
	std::cout << "\n\n Login obj here to tell you that the user LOGGED IN succesfully :D \n\n";

	user_.loginResponse = structLoginReturn.loginResponse;
	user_.userName = structLoginReturn.userName;
	user_.userToken = structLoginReturn.userToken;
	changed_.emit();
}

void Login::logout()
{
	std::cout << "\n\n Login obj here to tell you that the user LOGGED OUT succesfully :D \n\n";
	user_.loginResponse = AuthModule::LoginResponse::NotIdentified;
	user_.userName = "";
	user_.userToken = "";
	changed_.emit();
}

bool Login::isLoggedIn()
{
	auto status = user_.loginResponse;
	if (status == AuthModule::LoginResponse::LoggedIn)
	{
		std::cout << "\n\n isLOggedIn true \n\n";
		return true;
	}
	else
	{
		std::cout << "\n\n isLOggedIn false \n\n";
		return false;
	}
}
