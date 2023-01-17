module EventDataModule
{
	struct StructClient
	{
		int		eventId;
		string	name;
		string	phone;
	}
	struct StructService 
	{
		int		eventId;
		string	serviceTitle;
		string	providerIdentity;
		string	startHour;
		double	duration;
		int		cost;
		string	description;
		string	observations;
	}

	sequence<StructService>	SeqServices;
	
	struct StructRegisterEvent
	{
		string			userToken;
		int				userId;
		StructClient	client;
		string			date;
		string			startHour;
		double			duration;
		string			location;
		string			observations;
		SeqServices		services;
	}

	interface EventsDataInterface
    {
        void registerEvent(StructRegisterEvent structRegisterEvent);
		

    }
}