#include "include/test.h"
#include <Wt/WText.h>
#include <Wt/WTemplate.h>
#include <Wt/WPushButton.h>

Test::Test()
	: WContainerWidget()
{
	addStyleClass("");
	setPositionScheme(Wt::PositionScheme::Absolute);
	auto header = addWidget(std::make_unique<Wt::WContainerWidget>());
	header->addStyleClass("");

	// Close Btn
	auto closeBtn = header->addWidget(std::make_unique<Wt::WPushButton>("X"));
	closeBtn->clicked().connect(this, &Test::closeWindow);
}

void Test::closeWindow()
{
}