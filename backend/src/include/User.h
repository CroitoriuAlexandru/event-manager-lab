#pragma once
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Dbo.h>


namespace dbo = Wt::Dbo;

class User;


namespace Wt {
	namespace Dbo {

		template<>
		struct dbo_traits<User> : public dbo_default_traits {
			static const char* versionField() {
				return 0;
			}
		};
	}
}

using AuthInfo = Wt::Auth::Dbo::AuthInfo<User>;


class User : public Wt::Dbo::Dbo<User> {
public:
	std::string name;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, name, "name");
	}
};

