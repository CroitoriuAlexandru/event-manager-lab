#pragma once
#include "AuthModule.h"

#include <Wt/Auth/Dbo/UserDatabase.h>

#include <Wt/Dbo/SqlConnection.h>

#include <Wt/WString.h>

#include <Wt/Dbo/Session.h>
#include <Wt/Dbo/ptr.h>
#include "User.h"

namespace dbo = Wt::Dbo;
using namespace AuthModule;

// using AuthInfo = Wt::Auth::Dbo::AuthInfo<User>; defined in User.h
using UserDatabase = Wt::Auth::Dbo::UserDatabase<AuthInfo>;

class AuthSession
{
public:
    AuthSession(std::unique_ptr<dbo::SqlConnection> conn);
    ~AuthSession();

    static void configureAuth();
    void configureSession();

    // functions used by auth interface
    StructLoginReturn tryLoginUser(StructLoginInfo structLoginInfo);
    StructRegistrationReturn tryRegisterNewUser(StructRegistrationInfo structRegistrationInfo);
    Wt::WString processUserTokenForName(Wt::WString userToken);

    // functions used by other interfaces
    int getUserIdFromToken(Wt::WString userToken);

    Wt::Auth::AbstractUserDatabase& users();
    static const Wt::Auth::AuthService& auth();
    static const Wt::Auth::AbstractPasswordService& passwordAuth();

  
private:

    mutable Wt::Dbo::Session session_;
    std::unique_ptr<UserDatabase> users_;
    //std::shared_ptr<dbo::FixedSqlConnectionPool> connPool_;
};
