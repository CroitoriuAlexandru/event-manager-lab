#include "include/AddEventView.h"
#include "include/EventDataModule.h"
#include <Wt/WPushButton.h>
#include <Wt/WMenu.h>
#include <Wt/WStackedWidget.h>
#include <Ice/Ice.h>
#include <stdexcept>
#include <Wt/WText.h>

AddEventView::AddEventView(std::shared_ptr<Login> login)
	: WTemplate(tr("add-event-temp")),
	  login_(login)
{
	setMaximumSize(Wt::WLength(100, Wt::LengthUnit::Percentage), Wt::WLength(569, Wt::LengthUnit::Pixel));
	eventForm_ = bindWidget("event-form", std::make_unique<EventFormView>());

	auto submitBtn = bindWidget("submit-btn", std::make_unique<Wt::WPushButton>("Submit"));
	submitBtn->clicked().connect(this, &AddEventView::processFormsData);

	auto servicesStack = bindWidget("services-stack", std::make_unique<Wt::WStackedWidget>());
	servicesStack->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::Fade), true);

	auto servicesMenu = bindWidget("services-menu", std::make_unique<Wt::WMenu>(servicesStack));
	servicesMenu->addStyleClass("overflow-auto flex-nowrap");
	// servicesMenu->setMinimumSize(Wt::WLength(10, Wt::LengthUnit::Pica), Wt::WLength(4, Wt::LengthUnit::Pica));
	auto serviceOne = servicesMenu->addItem("Service 1", std::make_unique<ServiceFormView>());
	serviceOne->setIcon("/resources/svgs/user-rounded.svg");
	serviceOne->setStyleClass("m-0");
	servicesMenu->addItem("Service 2", std::make_unique<ServiceFormView>());
	servicesMenu->addItem("Service 3", std::make_unique<ServiceFormView>());
	servicesMenu->addItem("Service 1", std::make_unique<ServiceFormView>());
	servicesMenu->addItem("Service 2", std::make_unique<ServiceFormView>());
	servicesMenu->addItem("Service 3", std::make_unique<ServiceFormView>());
	servicesMenu->addItem("Service 1", std::make_unique<ServiceFormView>());
	servicesMenu->addItem("Service 2", std::make_unique<ServiceFormView>());
}

void AddEventView::processFormsData()
{
	std::cout << "\n\n ----------------- process add event forms data started \n\n";
}

void AddEventView::addService()
{
}
