#pragma once
#include <Wt/WFormModel.h> // used to create the Model of the form
#include <Wt/WValidator.h> // used to create a Validator

#include "AuthModule.h"

// Login Form Model Definition
class LoginFormModel : public Wt::WFormModel
{
public:
	static const Field UserEmail;
	static const Field UserPassword;

	LoginFormModel();
	AuthModule::StructLoginInfo userData();

private:
};

// Registration Form Model Definition
class RegistrationFormModel : public Wt::WFormModel
{
public:
	static const Field UserName;
	static const Field UserEmail;
	static const Field UserPassword;
	static const Field UserPasswordRepeat;

	RegistrationFormModel();

	AuthModule::StructRegistrationInfo userData();

private:
};
