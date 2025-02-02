#include "ApplicationHarness.h"

//Include the necessary header file here for your game type
#include "PhysicsApp.h"

int main()
{
	ApplicationHarness program(new PhysicsApp());	//Create a new instance of your application type to pass into the harness here.

	program.Run();

	return 0;
}