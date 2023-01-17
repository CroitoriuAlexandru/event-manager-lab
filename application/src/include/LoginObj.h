#pragma once
#include "AuthModule.h"
#include <Wt/WSignal.h>

class Login
{
public:
	Login();

	void login(AuthModule::StructLoginReturn structLoginReturn);
	void logout();
	bool isLoggedIn();
	AuthModule::StructLoginReturn &user() { return user_; };
	Wt::Signal<> &changed() { return changed_; };

private:
	AuthModule::StructLoginReturn user_;
	Wt::Signal<> changed_;
};