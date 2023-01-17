module AuthModule
{

	enum LoginResponse { NotIdentified, Identified, LoggedIn, IncorectPassword, ThrottlingActivated };
	enum RegistrationResponse { UserRegistrationSuccessful ,UserEmailAlreadyExists };

	struct StructLoginInfo
	{
		string userEmail;
		string userPassword;
	}

	struct StructRegistrationInfo
	{
		string userName;
		StructLoginInfo structLoginInfo;
	}

	struct StructLoginReturn
	{
		string userName;
		string userToken;
		LoginResponse loginResponse;
	}


	struct StructRegistrationReturn
	{
		StructLoginReturn structLoginReturn;
		RegistrationResponse registrationResponse;
	}
		
    interface AuthInterface
    {
        StructLoginReturn tryLogin(StructLoginInfo structLoginInfo);
		
		StructRegistrationReturn tryRegister(StructRegistrationInfo structRegistrationInfo);
		
		string getUserName(string userToken);
    }
}