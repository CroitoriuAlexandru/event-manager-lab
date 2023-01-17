#pragma once
#include <Wt/WTemplateFormView.h>
#include "AuthentificationModels.h"
#include "LoginObj.h"

// Login View
class LoginFormView : public Wt::WTemplateFormView
{
public:
	LoginFormView(std::shared_ptr<Login> login);

	std::shared_ptr<LoginFormModel> model_;
	std::shared_ptr<Login> login_;
	// process is private, but for faster dev, its make public
	void process();

private:
};

// Registration View
class RegistrationFormView : public Wt::WTemplateFormView
{
public:
	RegistrationFormView(std::shared_ptr<Login> login);

	std::shared_ptr<RegistrationFormModel> model_;

	std::shared_ptr<Login> login_;

private:
	void process();
};