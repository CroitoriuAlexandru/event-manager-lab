#include "include/AuthentificationModels.h"
#include <Wt/WLengthValidator.h> // used to add type and functionality to the validator
#include <Wt/WRegExpValidator.h>

// login form fields
const Wt::WFormModel::Field LoginFormModel::UserEmail = "user-email";
const Wt::WFormModel::Field LoginFormModel::UserPassword = "user-password";

// registration form fields
const Wt::WFormModel::Field RegistrationFormModel::UserName = "user-name";
const Wt::WFormModel::Field RegistrationFormModel::UserEmail = "user-email";
const Wt::WFormModel::Field RegistrationFormModel::UserPassword = "user-password";
const Wt::WFormModel::Field RegistrationFormModel::UserPasswordRepeat = "user-password-repeat";

std::shared_ptr<Wt::WValidator> createUserEmailValidator()
{
	auto v = std::make_shared<Wt::WRegExpValidator>("([a-zA-Z0-9\\d\\.-]+)@([a-zA-Z0-9\\d-]+)\\.([a-zA-Z0-9]{2,8})(\\.[a-zA-Z0-9]{2,8})?");
	v->setMandatory(true);
	return v;
}

std::shared_ptr<Wt::WValidator> createUserPasswordValidator()
{
	auto v = std::make_shared<Wt::WRegExpValidator>("[a-zA-Z0-9!@#$&()\\-`.+,/\"]{5,20}");
	v->setMandatory(true);
	return v;
}

std::shared_ptr<Wt::WValidator> createUserNameValidator()
{
	auto v = std::make_shared<Wt::WRegExpValidator>("[a-zA-Z ]{3,30}");
	v->setMandatory(true);
	return v;
}

// Login Form Model Implementation
LoginFormModel::LoginFormModel()
	: WFormModel()
{
	// add Line Edit field, Validator, and some value to the input if you want
	addField(UserEmail);
	addField(UserPassword);

	setValidator(UserEmail, createUserEmailValidator());
	setValidator(UserPassword, createUserPasswordValidator());

	setValue(UserEmail, "croitoriu.93@gmail.com");
	setValue(UserPassword, "asdfghj1");
}

AuthModule::StructLoginInfo LoginFormModel::userData()
{
	std::cout << "\n\n AuthentificationWidget::loginUser() Started \n";

	AuthModule::StructLoginInfo loginUserInfo;
	loginUserInfo.userEmail = valueText(UserEmail).toUTF8();
	loginUserInfo.userPassword = valueText(UserPassword).toUTF8();
	return loginUserInfo;
}

// Registration Form Model Implementation
RegistrationFormModel::RegistrationFormModel()
	: WFormModel()
{
	// add Line Edit field, Validator, and some value to the input if you want
	addField(UserName);
	addField(UserEmail);
	addField(UserPassword);
	addField(UserPasswordRepeat);

	setValidator(UserName, createUserNameValidator());
	setValidator(UserEmail, createUserEmailValidator());
	setValidator(UserPassword, createUserPasswordValidator());
	// setValidator(UserPasswordRepeat, createUserPasswordRepeatValidator());

	setValue(UserName, "Croitoriu Alexandru Dan");
	setValue(UserEmail, "croitoriu.93@gmail.com");
	setValue(UserPassword, "asdfghj1");
	setValue(UserPasswordRepeat, "asdfghj1");
}

AuthModule::StructRegistrationInfo RegistrationFormModel::userData()
{
	AuthModule::StructRegistrationInfo structRegistrationInfo;
	structRegistrationInfo.userName = valueText(UserName).toUTF8();
	structRegistrationInfo.structLoginInfo.userEmail = valueText(UserEmail).toUTF8();
	structRegistrationInfo.structLoginInfo.userPassword = valueText(UserPassword).toUTF8();

	return structRegistrationInfo;
}
