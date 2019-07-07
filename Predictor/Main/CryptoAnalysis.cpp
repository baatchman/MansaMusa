// CryptoAnalysis.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "..\..\..\Master.h"

#include <iostream>
#include "Branches/CA_Branches.h"



 //For openGL try int main(int argc, char * args[]), apparently it is defined in a header file that way.
int main(int argc, char** argv)
{


	cout << "\n argc->" << argc;
	for (int cou_Index = 0; cou_Index < argc; cou_Index++)
	{
		cout << "\n [" << cou_Index << "]" << argv[cou_Index];
	}
	cout << " ";

	srand(time(NULL));

	init_LoTd(20);

	set_Console_Color(0, 10);

	cout << "\n\t Entering Sub Program..."; cout.flush();
	//ca_main(argc, argv);
	//DBase_Testermon();

	Testermon_Predictor();

	system("PAUSE > NULL");

	return 1;
}
