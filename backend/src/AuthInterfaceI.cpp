#include "include/AuthInterfaceI.h"


AuthInterfaceI::AuthInterfaceI(std::unique_ptr<dbo::SqlConnection> conn)
	: authSession_(std::move(conn))
{

}

StructLoginReturn AuthInterfaceI::tryLogin(StructLoginInfo structLoginInfo, const Ice::Current&)
{
	std::cout << "\n\n" << "AuthInterfaceI::tryToLogIn(userLoginInfo) STARTED" << "\n\n";

	// std::cout << "User Name     : " << loginUserInfo.userName << "\n";
	// std::cout << "User Password : " << userLoginInfo.userPassword << "\n\n";
	// 
	// 
	StructLoginReturn userResponse = authSession_.tryLoginUser(structLoginInfo);
	// 
	// 
	// 
	// std::cout << "\n User Name : " << 	userResponse.userName;
	// std::cout << "\n User Token : " << 	userResponse.userToken;
	// std::cout << "\n User Status : " << userResponse.status;

	std::cout << "\n\n" << "AuthInterfaceI::tryToLogIn(userLoginInfo) ENDED" << "\n\n";
	return userResponse;
}

StructRegistrationReturn AuthInterfaceI::tryRegister(StructRegistrationInfo structRegistrationInfo, const Ice::Current&)
{
	std::cout << "\n\n  AuthInterfaceI::tryRegister STARTED \n\n";

	StructRegistrationReturn userResponse;
	userResponse = authSession_.tryRegisterNewUser(structRegistrationInfo);
	// std::cout << "\n User Name : " << 	userResponse.loginReturnUserData.userName.toUTF8();
	// std::cout << "\n User Token : " << 	userResponse.loginReturnUserData.userToken.toUTF8();
	// std::cout << "\n User Status : " << userResponse.loginReturnUserData.status.toUTF8();
	// std::cout << "\n User reg response : " << userResponse.userRegistrationResponse;

	std::cout << "\n\n  AuthInterfaceI::tryRegister STARTED \n\n";
	return userResponse;
}

string AuthInterfaceI::getUserName(string userToken, const Ice::Current&)
{
	Wt::WString userTokenWS(userToken);
	auto userName = authSession_.processUserTokenForName(userTokenWS).toUTF8();

	return userName;
}