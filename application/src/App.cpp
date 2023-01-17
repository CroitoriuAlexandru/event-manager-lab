#include "include/App.h"
#include "include/AddEventView.h"
#include "include/test.h"
#include <Wt/WTemplate.h>
#include <Wt/WRadioButton.h>
#include <Wt/WPushButton.h>
#include <Wt/WImage.h>
#include <Wt/WAnimation.h>

// Application is a containerWidget
EventManagerLab::EventManagerLab()
	: WContainerWidget(),
	  login_(std::make_shared<Login>())
{

	addStyleClass("animate");

	authWidget_ = addWidget(std::make_unique<AuthWidget>(login_));

	createApp();

	login_->changed().connect(this, &EventManagerLab::handleUserAuth);
	login_->changed().emit();

	// authWidget_->loginFormView_->process();
}

void EventManagerLab::createContentHeader()
{
	auto contentHeader = content_->addWidget(std::make_unique<Wt::WContainerWidget>());
	contentHeader->addStyleClass("content-header");

	auto toggleSidebarBtn = contentHeader->addWidget(std::make_unique<Wt::WTemplate>(tr("hamburger-svg")));
	toggleSidebarBtn->setStyleClass("content-menu");
	toggleSidebarBtn->clicked().connect(this, [=]()
										{
		if(sidebar_->hasStyleClass("minimized")){
			sidebar_->removeStyleClass("minimized");
		} else {
			sidebar_->addStyleClass("minimized");
		} });
}

void EventManagerLab::createApp(std::string iconPath, std::string userName)
{

	// Create App Sidebar and Content
	sidebar_ = addWidget(std::make_unique<Wt::WContainerWidget>());
	content_ = addWidget(std::make_unique<Wt::WContainerWidget>());

	// Add Class Style to sidebar and content
	sidebar_->setStyleClass("sidebar");
	content_->addStyleClass("content");

	createContentHeader();

	// Create App Content Widget
	appContents_ = content_->addWidget(std::make_unique<Wt::WStackedWidget>());
	appContents_->setTransitionAnimation(Wt::WAnimation(Wt::AnimationEffect::Fade), true);
	appContents_->setStyleClass("w-100 h-100");

	// Create Sidebar Header
	auto sidebarHeader = sidebar_->addWidget(std::make_unique<Wt::WTemplate>(tr("sidebar-header")));
	sidebarHeader->setStyleClass("sidebar-header");
	sidebar_->addWidget(std::make_unique<Wt::WText>("<hr />", Wt::TextFormat::XHTML));

	// Create Sidebar Menu
	menuSidebar_ = sidebar_->addWidget(std::make_unique<Wt::WMenu>(appContents_));
	sidebar_->addWidget(std::make_unique<Wt::WText>("<hr />", Wt::TextFormat::XHTML));

	menuSidebar_->setStyleClass("nav-sidebar style-three");
	menuSidebar_->setInternalPathEnabled();

	// Add menu items to the sidebar
	test_menuItem_ = menuSidebar_->addItem("Test", std::make_unique<Test>());
	addEvent_menuItem_ = menuSidebar_->addItem("Add Event", std::make_unique<AddEventView>(login_));
	calendar_menuItem_ = menuSidebar_->addItem("Calendar", std::make_unique<Wt::WText>("Calendar Content Here"));
	workload_menuItem_ = menuSidebar_->addItem("Workload", std::make_unique<Wt::WText>("Workload Content Here"));
	profile_menuItem_ = menuSidebar_->addItem("Profile", std::make_unique<Wt::WText>("Profile Content Here"));

	auto avatarWidget = sidebar_->addWidget(std::make_unique<Wt::WContainerWidget>());

	avatarWidget->addStyleClass("sidebar-footer bg-gradient mt-auto");

	auto avatar = avatarWidget->addWidget(std::make_unique<Wt::WContainerWidget>());
	avatar->setStyleClass("d-flex justify-content-center align-items-center w-100 h-100");
	avatar->setAttributeValue("data-bs-toggle", "dropdown");
	avatar->setAttributeValue("aria-expanded", "false");

	auto avatarIcon = avatar->addWidget(std::make_unique<Wt::WImage>(Wt::WLink(iconPath)));
	avatarIcon->setStyleClass("ms-2 avatar online");

	auto avatarbody = avatar->addWidget(std::make_unique<Wt::WContainerWidget>());
	avatarbody->setStyleClass("avatar-body d-flex justify-content-between align-items-center");
	auto avatarText = avatarbody->addWidget(std::make_unique<Wt::WText>("<h6>" + userName + "</h6>", Wt::TextFormat::XHTML));
	avatarText->setStyleClass("text-wrap");

	auto svgSettingsIcon = avatarbody->addWidget(std::make_unique<Wt::WTemplate>(tr("settings-svg")));
	svgSettingsIcon->setStyleClass("me-2");

	auto avatarMenu = avatarWidget->addWidget(std::make_unique<Wt::WContainerWidget>());
	avatarMenu->setStyleClass("dropdown-menu dropdown-menu-dark shadow");
	avatarMenu->setAttributeValue("aria-labelledby", "dropdownUser");
	avatarMenu->setAttributeValue("data-popper-placement", "top-start");

	auto logoutBtn = avatarMenu->addWidget(std::make_unique<Wt::WTemplate>(tr("logout-btn")));
	logoutBtn->clicked().connect(this, [=]()
								 { login_->logout(); });
	avatarMenu->addWidget(std::make_unique<Wt::WText>("<hr class='mx-0' />"));
	createThemeToggler(avatarMenu);
}

void EventManagerLab::createThemeToggler(Wt::WContainerWidget *avatarMenu)
{
	auto themeToggler = avatarMenu->addWidget(std::make_unique<Wt::WContainerWidget>());
	themeToggler->setStyleClass("d-flex justify-content-between align-items-center ms-4 pt-2");
	auto themeTogglerGroup_ = std::make_shared<Wt::WButtonGroup>();

	auto button = themeToggler->addWidget(std::make_unique<Wt::WRadioButton>(""));
	button->setStyleClass("p-1");
	themeTogglerGroup_->addButton(button);

	button = themeToggler->addWidget(std::make_unique<Wt::WRadioButton>(""));
	button->setStyleClass("p-1");
	themeTogglerGroup_->addButton(button);

	button = themeToggler->addWidget(std::make_unique<Wt::WRadioButton>(""));
	button->setStyleClass("p-1");
	themeTogglerGroup_->addButton(button);

	button = themeToggler->addWidget(std::make_unique<Wt::WRadioButton>(""));
	button->setStyleClass("p-1");
	themeTogglerGroup_->addButton(button);

	button = themeToggler->addWidget(std::make_unique<Wt::WRadioButton>(""));
	button->setStyleClass("p-1");
	button->setId("skin-indigo");
	themeTogglerGroup_->addButton(button);

	button = themeToggler->addWidget(std::make_unique<Wt::WRadioButton>(""));
	button->setStyleClass("p-1");
	button->setId("skin-gradient");
	themeTogglerGroup_->addButton(button);

	themeTogglerGroup_->checkedChanged().connect(this, [=]()
												 {
													 std::cout << "\n\n"
															   << themeTogglerGroup_->selectedButtonIndex() << "\n\n";
													removeStyleClass(styleClass());
													setStyleClass("animate");
													 switch (themeTogglerGroup_->selectedButtonIndex())
													 {
													 case 0:
													 	addStyleClass("skin-base");
														break;
														case 1:
													 	addStyleClass("skin-blue");
														break;
														case 2:
													 	addStyleClass("skin-dark");
														break;
														case 3:
													 	addStyleClass("skin-red");
														break;
														case 4:
													 	addStyleClass("skin-indigo");
														break;
														case 5:
													 	addStyleClass("skin-gradient");
														break;
													 } });

	// set initial theme checked
	themeTogglerGroup_->setSelectedButtonIndex(5);
	themeTogglerGroup_->checkedChanged().emit(themeTogglerGroup_->checkedButton());
}

void EventManagerLab::handleUserAuth()
{
	if (!login_->isLoggedIn())
	{
		sidebar_->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromLeft));
		content_->animateHide(Wt::WAnimation(Wt::AnimationEffect::Fade));
		authWidget_->animateShow(Wt::WAnimation(Wt::AnimationEffect::SlideInFromTop));
	}
	else
	{
		sidebar_->animateShow(Wt::WAnimation(Wt::AnimationEffect::Fade));
		content_->animateShow(Wt::WAnimation(Wt::AnimationEffect::Fade));
		authWidget_->animateHide(Wt::WAnimation(Wt::AnimationEffect::SlideInFromTop));
	}
}