#include "App.h"
#include <iostream>

using namespace BitwiseEngine;

App* App::TheApplication = 0;

App::App(void)
{
}

App::~App(void)
{
}

int
App::Run(int argc, char **argv)
{
	using namespace std;

	int result = 0;

	if(!OnPreMain())
	{
		cerr << "Error: App::OnPreMain() returned false" << endl;
		cerr << "       Program Exiting" << endl;

		return -1;
	}

	result = Main(argc, argv);
	
	if(!OnPostMain())
	{
		cerr << "Error: App::OnPostMain() returned false" << endl;
		cerr << "       Program Exiting" << endl;

		return -1;
	}

	delete App::TheApplication;

	return result;
}