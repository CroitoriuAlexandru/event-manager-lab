#include <Ice/Ice.h>
#include <stdexcept>
#include "include/AuthModule.h"

#include "include/AuthentificationViews.h"
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>

LoginFormView::LoginFormView(std::shared_ptr<Login> login)
	: login_(login)
{
	// add model to view
	model_ = std::make_shared<LoginFormModel>();

	// link xml template <message id='template-name'> to the FormView
	setTemplateText(Wt::WString::tr("login-form-template"));

	auto facebookBtn = bindWidget("social-facebook-login", std::make_unique<Wt::WTemplate>(tr("facebook-svg")));
	auto googleBtn = bindWidget("social-google-login", std::make_unique<Wt::WTemplate>(tr("google-svg")));

	facebookBtn->clicked().connect(this, [=]()
								   { bindWidget("submit-info", std::make_unique<Wt::WText>("<p class='text-danger'>facebook login not implemented yet</p>", Wt::TextFormat::XHTML)); });
	googleBtn->clicked().connect(this, [=]()
								 { bindWidget("submit-info", std::make_unique<Wt::WText>("<p class='text-danger'>google login not implemented yet</p>", Wt::TextFormat::XHTML)); });

	// link Form Model Fields to FormWidget and set up the view
	auto userEmailLineEdit = std::make_unique<Wt::WLineEdit>();
	userEmailLineEdit->setPlaceholderText("Email");
	setFormWidget(LoginFormModel::UserEmail, std::move(userEmailLineEdit));

	auto userPasswordLineEdit = std::make_unique<Wt::WLineEdit>();
	userPasswordLineEdit->setEchoMode(Wt::EchoMode::Password);
	userPasswordLineEdit->setPlaceholderText("Password");
	setFormWidget(LoginFormModel::UserPassword, std::move(userPasswordLineEdit));

	// Login Btn
	auto loginSubmitBtn = std::make_unique<Wt::WPushButton>("Login");
	loginSubmitBtn->clicked().connect(this, &LoginFormView::process);
	bindWidget("submit-button", std::move(loginSubmitBtn));

	// submit info
	bindEmpty("submit-info");

	updateView(model_.get());
}

void LoginFormView::process()
{
	// Update Model, for next step
	updateModel(model_.get());

	// Validate Model to send data to Authentification Server
	if (model_->validate())
	{

		// Ice communication for loging in a user starts here
		try
		{
			Ice::CommunicatorHolder ich = Ice::initialize();
			auto base = ich->stringToProxy("Authentification:default -p 10000");
			auto authInterface = Ice::checkedCast<AuthModule::AuthInterfacePrx>(base);
			if (!authInterface)
			{
				throw std::runtime_error("Invalid proxy");
			}

			// Try to login in server
			AuthModule::StructLoginReturn userDataResult = authInterface->tryLogin(model_->userData());
			switch (userDataResult.loginResponse)
			{
			case AuthModule::LoginResponse::NotIdentified:
				bindWidget("submit-info", std::make_unique<Wt::WText>("No user with that Email"));
				break;
			case AuthModule::LoginResponse::LoggedIn:
				bindWidget("submit-info", std::make_unique<Wt::WText>("User Succesfuly Logged In"));
				login_->login(userDataResult);
				break;
			case AuthModule::LoginResponse::Identified:
				bindWidget("submit-info", std::make_unique<Wt::WText>("User Succesfuly Identified"));
			case AuthModule::LoginResponse::IncorectPassword:
				bindWidget("submit-info", std::make_unique<Wt::WText>("Incorect Password, reset Password ?"));
				break;
			case AuthModule::LoginResponse::ThrottlingActivated:
				bindWidget("submit-info", std::make_unique<Wt::WText>("Password Trottling is activated, you can try again in 5 minutes, this is for your accout protection"));
				break;
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}

RegistrationFormView::RegistrationFormView(std::shared_ptr<Login> login)
	: login_(login)
{
	// add model to view
	model_ = std::make_shared<RegistrationFormModel>();

	// link xml template <message id='template-name'> to the FormView
	setTemplateText(Wt::WString::tr("registration-form-template"));

	auto facebookBtn = bindWidget("social-facebook-registration", std::make_unique<Wt::WTemplate>(tr("facebook-svg")));
	auto googleBtn = bindWidget("social-google-registration", std::make_unique<Wt::WTemplate>(tr("google-svg")));

	facebookBtn->clicked().connect(this, [=]()
								   { bindWidget("submit-info", std::make_unique<Wt::WText>("facebook registration not implemented yet", Wt::TextFormat::XHTML)); });
	googleBtn->clicked().connect(this, [=]()
								 { bindWidget("submit-info", std::make_unique<Wt::WText>("google registration not implemented yet", Wt::TextFormat::XHTML)); });

	// link Form Model Fields to FormWidget and set up the view
	auto userNameLineEdit = std::make_unique<Wt::WLineEdit>();
	userNameLineEdit->setPlaceholderText("User Name");

	setFormWidget(RegistrationFormModel::UserName, std::move(userNameLineEdit));

	auto userEmailLineEdit = std::make_unique<Wt::WLineEdit>();
	userEmailLineEdit->setPlaceholderText("Email");

	setFormWidget(RegistrationFormModel::UserEmail, std::move(userEmailLineEdit));

	auto userPasswordLineEdit = std::make_unique<Wt::WLineEdit>();
	userPasswordLineEdit->setEchoMode(Wt::EchoMode::Password);
	userPasswordLineEdit->setPlaceholderText("Password");

	setFormWidget(RegistrationFormModel::UserPassword, std::move(userPasswordLineEdit));

	auto userPasswordRepeatLineEdit = std::make_unique<Wt::WLineEdit>();
	userPasswordRepeatLineEdit->setEchoMode(Wt::EchoMode::Password);
	userPasswordRepeatLineEdit->setPlaceholderText("Repeat Password");

	// auto v = std::make_shared<Wt::WValidator>(true);
	// userPasswordRepeatLineEdit->setValidator(std::move(v));
	setFormWidget(RegistrationFormModel::UserPasswordRepeat, std::move(userPasswordRepeatLineEdit));

	// Login Btn
	auto submitBtn = std::make_unique<Wt::WPushButton>("Register");
	submitBtn->clicked().connect(this, &RegistrationFormView::process);
	bindWidget("submit-button", std::move(submitBtn));

	bindEmpty("submit-info");

	updateView(model_.get());
}

void RegistrationFormView::process()
{
	// Update Model, for next step
	updateModel(model_.get());

	// Validate Model to send data to Authentification Server
	if (model_->validate())
	{
		auto password = model_->valueText(model_->UserPassword);
		auto repeatPassword = model_->valueText(model_->UserPasswordRepeat);
		if (password != repeatPassword)
		{
			bindWidget("submit-info", std::make_unique<Wt::WText>("Passwords are not matching, try again"));
			return;
		}
		// Ice communication for loging in a user starts here
		try
		{
			Ice::CommunicatorHolder ich = Ice::initialize();
			auto base = ich->stringToProxy("Authentification:default -p 10000");
			auto authInterface = Ice::checkedCast<AuthModule::AuthInterfacePrx>(base);
			if (!authInterface)
			{
				throw std::runtime_error("Invalid proxy");
			}

			// Try to login in server
			AuthModule::StructRegistrationReturn structRegistrationReturn = authInterface->tryRegister(model_->userData());
			switch (structRegistrationReturn.registrationResponse)
			{
			case AuthModule::RegistrationResponse::UserRegistrationSuccessful:
				bindWidget("submit-info", std::make_unique<Wt::WText>("Registration Successful"));
				login_->login(structRegistrationReturn.structLoginReturn);
				break;
			case AuthModule::RegistrationResponse::UserEmailAlreadyExists:
				bindWidget("submit-info", std::make_unique<Wt::WText>("User with same Email identified, forgot Password ?"));
				break;
			}
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << std::endl;
		}
	}
}
