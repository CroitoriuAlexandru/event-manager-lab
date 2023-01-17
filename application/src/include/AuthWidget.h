#pragma once
#include "AuthentificationViews.h"
#include "LoginObj.h"

#include <Wt/WContainerWidget.h>
#include <Wt/WTemplate.h>

class AuthWidget : public Wt::WContainerWidget
{
public:
	AuthWidget(std::shared_ptr<Login> login);

	std::shared_ptr<Login> login_;

	// loginFormView_ is private, but for faster dev, its make public
	LoginFormView *loginFormView_;

private:
	Wt::WTemplate *authTemp_;
	RegistrationFormView *registrationFormView_;
};