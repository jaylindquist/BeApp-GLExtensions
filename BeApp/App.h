#pragma once

#include "BeApp.h"

#include <memory>

namespace BitwiseEngine
{

class BE_APPLICATION_ITEM App
{
protected:
	App(void);

public:
	virtual ~App(void);

	/********************************************
	*
	* Create a main function like the following
	*	to run your code
	*
	*	int main(int argc, char** argv)
	*	{
	*		App::TheApplication = new YourAppClass();
	*		return App::TheApplication->Run(argc, argv);
	*	}
	*
	********************************************/
	static App* TheApplication;
	int Run(int argc, char** argv);

	/*********************************************
	*
	* User implemented entry point
	* This will be called by Run
	*
	**********************************************/
	virtual int Main(int argc, char** argv) =  0;

	/*********************************************
	*
	* User implemented initializer and destructor
	* Called before and after Main()
	* If either returns false, the application
	*	will exit with an error code
	*
	*********************************************/
	virtual bool OnPreMain() { return true; }
	virtual bool OnPostMain() { return true; }
};

}