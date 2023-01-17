#pragma once
#include <Wt/Dbo/Types.h>
#include <Wt/WGlobal.h>
#include <Wt/WString.h>
#include <Wt/Dbo/Dbo.h>
#include <chrono>

class Event;
class Client;
class Service;

namespace Wt
{
	namespace Dbo
	{

		template <>
		struct dbo_traits<Event> : public dbo_default_traits
		{
			static const char *versionField()
			{
				return 0;
			}
		};

		template <>
		struct dbo_traits<Client> : public dbo_default_traits
		{
			static const char *versionField()
			{
				return 0;
			}
		};

		template <>
		struct dbo_traits<Service> : public dbo_default_traits
		{
			static const char *versionField()
			{
				return 0;
			}
		};

	}
}

namespace dbo = Wt::Dbo;

class Client : public Wt::Dbo::Dbo<Client>
{
public:
	dbo::ptr<Event> event;
	std::string name;
	std::string phone;
	std::string specialNote;

	template <class Action>
	void persist(Action &a)
	{
		dbo::belongsTo(a, event);
		dbo::field(a, name, "name");
		dbo::field(a, phone, "phone");
		dbo::field(a, specialNote, "specialNote");
	}
};

class Service : public Wt::Dbo::Dbo<Service>
{
public:
	dbo::ptr<Event> event;
	std::string providerIdentity;
	double duration;
	int cost;
	std::string description;
	std::string observations;

	template <class Action>
	void persist(Action &a)
	{
		dbo::belongsTo(a, event, "event");
		dbo::field(a, providerIdentity, "provider_identity");
		dbo::field(a, duration, "duration");
		dbo::field(a, cost, "cost");
		dbo::field(a, description, "description");
		dbo::field(a, observations, "observations");
	}
};

class Event : public Wt::Dbo::Dbo<Event>
{
public:
	int userId;
	dbo::weak_ptr<Client> client;
	dbo::collection<dbo::ptr<Service>> services;
	std::chrono::system_clock::time_point dateTime;
	double duration;
	std::string location;
	std::string observations;

	template <class Action>
	void persist(Action &a)
	{
		dbo::field(a, userId, "user_id");
		dbo::hasOne(a, client);
		dbo::hasMany(a, services, dbo::ManyToOne, "event");
		dbo::field(a, dateTime, "date_time");
		dbo::field(a, duration, "duration");
		dbo::field(a, location, "location");
		dbo::field(a, observations, "observations");
	}
};
