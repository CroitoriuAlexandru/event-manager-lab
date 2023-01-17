#pragma once
#include <Wt/WContainerWidget.h>
#include "LoginObj.h"
#include <Wt/WStackedWidget.h>
#include <Wt/WMenu.h>
#include "AuthWidget.h"
#include <Wt/WButtonGroup.h>

class EventManagerLab : public Wt::WContainerWidget
{
public:
    EventManagerLab();

    std::shared_ptr<Login> login_;

    Wt::WContainerWidget *sidebar_;
    Wt::WContainerWidget *content_;

    AuthWidget *authWidget_;

    Wt::WStackedWidget *appContents_;
    Wt::WMenu *menuSidebar_;

    Wt::WMenuItem *addEvent_menuItem_;
    Wt::WMenuItem *calendar_menuItem_;
    Wt::WMenuItem *workload_menuItem_;
    Wt::WMenuItem *test_menuItem_;
    Wt::WMenuItem *settings_menuItem_;
    Wt::WMenuItem *profile_menuItem_;

    Wt::WButtonGroup *themeTogglerGroup_;

private:
    void handleUserAuth();
    // content header is just the toggle button of the sidebar, it acts as a spacer at the same time
    void createContentHeader();
    void createApp(std::string iconPath = "resources/images/blank-profile-picture.png", std::string userName = "Test User Name Here");
    void createThemeToggler(Wt::WContainerWidget *avatarMenu);
};