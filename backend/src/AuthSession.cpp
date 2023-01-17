#include "include/AuthSession.h"
#include <Wt/WApplication.h>
#include "Wt/Auth/AuthService.h"
#include "Wt/Auth/HashFunction.h"
#include "Wt/Auth/PasswordService.h"
#include "Wt/Auth/PasswordStrengthValidator.h"
#include "Wt/Auth/PasswordVerifier.h"
#include "Wt/Auth/GoogleService.h"
#include "Wt/Auth/FacebookService.h"
#include "Wt/Auth/Dbo/AuthInfo.h"
#include <Wt/Auth/Token.h>
#include <Wt/WRandom.h>

#include <Wt/Dbo/backend/Sqlite3.h>
#include <Wt/Dbo/backend/Postgres.h>

#include <Wt/Dbo/FixedSqlConnectionPool.h>

using namespace Wt;


namespace dbo = Wt::Dbo;

namespace {

	Auth::AuthService myAuthService;
	Auth::PasswordService myPasswordService(myAuthService);

}

AuthSession::AuthSession(std::unique_ptr<dbo::SqlConnection> conn)
{
	session_.setConnection(std::move(conn));
	configureAuth();
	configureSession();
}

AuthSession::~AuthSession()
{
}

void AuthSession::configureAuth()
{
	myAuthService.setAuthTokensEnabled(true);
	myAuthService.setAuthTokenUpdateEnabled(false);
	myAuthService.setAuthTokenValidity(300);


	std::unique_ptr<Auth::PasswordVerifier> verifier = std::make_unique<Auth::PasswordVerifier>();
	verifier->addHashFunction(std::make_unique<Auth::BCryptHashFunction>(7));

	myPasswordService.setVerifier(std::move(verifier));
	myPasswordService.setStrengthValidator(std::make_unique<Auth::PasswordStrengthValidator>());
	myPasswordService.setAttemptThrottlingEnabled(true);
}

void AuthSession::configureSession()
{
	std::cout << "\n\n Configure Session STARTED \n\n";

	session_.mapClass<User>("user");
	session_.mapClass<AuthInfo>("auth_info");
	session_.mapClass<AuthInfo::AuthIdentityType>("auth_identity");
	session_.mapClass<AuthInfo::AuthTokenType>("auth_token");

	users_ = std::make_unique<UserDatabase>(session_);
	users_->setMaxAuthTokensPerUser(5);

	dbo::Transaction transaction(session_);
	try {
		session_.createTables();
		log("info") << "Database created";
	}
	catch (...) {
		log("info") << "Using existing database";
	}
	transaction.commit();

	std::cout << "\n\n Configure Session ENDED \n\n";
}

StructRegistrationReturn AuthSession::tryRegisterNewUser(StructRegistrationInfo structRegistrationInfo)
{

	std::cout << "\n\n Register New User STARTED \n\n";
	dbo::Transaction transaction(session_);

	// RETURNED RegistrationReturnUserData Object
	StructRegistrationReturn resultUser;
	resultUser.structLoginReturn.userName = structRegistrationInfo.userName;
	resultUser.structLoginReturn.loginResponse = LoginResponse::NotIdentified;
	resultUser.structLoginReturn.userToken = "";

	auto userAlreadyExist = users_->findWithEmail(structRegistrationInfo.structLoginInfo.userEmail);
	if (userAlreadyExist.isValid()) {

		std::cout << "\n\n User Alredy exists in dbo \n\n";
		resultUser.registrationResponse = RegistrationResponse::UserEmailAlreadyExists;
		resultUser.structLoginReturn.loginResponse = LoginResponse::Identified;
		resultUser.structLoginReturn.userName = userAlreadyExist.identity(Auth::Identity::LoginName).toUTF8();

		return resultUser;
	}
	std::cout << "\n\n No User found with current user data resource. Starting registration process \n\n";

	std::cout << "\n\n creating user for User Table \n\n";
	std::unique_ptr<User> newUserPtr{ new User() };
	newUserPtr->name = structRegistrationInfo.userName;
	dbo::ptr<User> userPtr = session_.add(std::move(newUserPtr));

	std::cout << "\n\n Creating a new user_info record \n\n";
	// Crteate a new AuthInfo record and returns a Wt::Auth::User
	auto newUser = users_->registerNew();
	// Create a new User
	std::cout << "\n\n gettin user info, for linking user refferance from User Table \n\n";
	auto userInfo = users_->find(newUser);
	std::cout << "\n\n link user to auth info \n\n";
	userInfo.modify()->setUser(userPtr);
	std::cout << "\n\n set identity \n\n";
	newUser.setIdentity(Auth::Identity::LoginName, structRegistrationInfo.structLoginInfo.userEmail);
	std::cout << "\n\n set email \n\n";
	newUser.setEmail(structRegistrationInfo.structLoginInfo.userEmail);
	std::cout << "\n\n set password \n\n";
	myPasswordService.updatePassword(newUser, structRegistrationInfo.structLoginInfo.userPassword);
	std::cout << "\n\n creating Token for user to log in \n\n";
	auto token = myAuthService.createAuthToken(newUser);


	resultUser.structLoginReturn.loginResponse= LoginResponse::LoggedIn;
	resultUser.structLoginReturn.userToken = token;
	resultUser.registrationResponse = RegistrationResponse::UserRegistrationSuccessful;

	transaction.commit();

	std::cout << "\n\n Register New User ENDED \n\n";
	return resultUser;
}

StructLoginReturn AuthSession::tryLoginUser(StructLoginInfo structLoginInfo)
{
	std::cout << "\n\n Try To Login STARTED \n\n";

	// RETURNED LoginReturnUserData Obj;
	StructLoginReturn resultUser;
	resultUser.loginResponse = LoginResponse::NotIdentified;
	resultUser.userName = "";
	resultUser.userToken = "";

	dbo::Transaction transaction(session_);

	auto user = users_->findWithEmail(structLoginInfo.userEmail);


	if (user.isValid()) {
		resultUser.loginResponse = LoginResponse::Identified;
		resultUser.userName = user.identity(Auth::Identity::LoginName).toUTF8();

		auto userToken = "";

		std::cout << "\n\n ------------------ USER FOUND ------------------ \n\n";
		auto passwordResult = myPasswordService.verifyPassword(user, structLoginInfo.userPassword);
		if (passwordResult == Wt::Auth::PasswordResult::PasswordInvalid) {
			
			resultUser.userToken = Wt::WString(userToken).toUTF8();
			resultUser.loginResponse = LoginResponse::IncorectPassword;
		}
		else if (passwordResult == Wt::Auth::PasswordResult::LoginThrottling) {
			resultUser.loginResponse = LoginResponse::ThrottlingActivated;
		}
		else if (passwordResult == Wt::Auth::PasswordResult::PasswordValid) {
			auto userToken = myAuthService.createAuthToken(user);
			resultUser.userToken = Wt::WString(userToken).toUTF8();
			resultUser.loginResponse = LoginResponse::LoggedIn;
		}
	}
	else {
		std::cout << "\n\n ------------------ USER NOT FOUND ------------------ \n\n";
	}
	transaction.commit();


	std::cout << "\n\n Try To Login ENDED  \n\n";
	return resultUser;
}

Wt::WString AuthSession::processUserTokenForName(Wt::WString userToken)
{
	std::string userName;

	dbo::Transaction transaction(session_);

	auto userResultToken = myAuthService.processAuthToken(userToken.toUTF8(), users());

	auto user = users_->find(userResultToken.user());
	userName = user->user()->name;

	transaction.commit();

	return userName;
}

Auth::AbstractUserDatabase& AuthSession::users()
{
	return *users_;
}

const Auth::AuthService& AuthSession::auth()
{
	return myAuthService;
}

const Auth::AbstractPasswordService& AuthSession::passwordAuth()
{
	return myPasswordService;
}

int AuthSession::getUserIdFromToken(Wt::WString userToken)
{
	int userId;

	dbo::Transaction transaction(session_);

	auto userResultToken = myAuthService.processAuthToken(userToken.toUTF8(), users());

	auto user = users_->find(userResultToken.user());
	userId = user->user()->id();

	transaction.commit();

	return userId;
}
