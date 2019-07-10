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

	/*
	c_NT3_Fractal_State_Tree testermon;
	u_Data_3 tmp_Data; tmp_Data.U = 1000;
	string tmp_String = "Cthulhu Ftagn";
	system("CLS");
	c_NT3_Base_Node_1D * tmp_Ar[100];
	cout << "\n\n\n";
	for (int cou_Index = 0; cou_Index < tmp_String.length(); cou_Index++)
	{
		tmp_Data.U = unsigned int(tmp_String.c_str()[cou_Index]);
		testermon.search(tmp_Data);
		tmp_Ar[cou_Index] = new c_NT3_Node_1D;
		testermon.set_Current_Node_NID(tmp_Ar[cou_Index]);
	}
	xy(0, 0);
	testermon.output_Tree();

	xy(0, 0);
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	for (int cou_Index = 0; cou_Index < tmp_String.length(); cou_Index++)
	{
		tmp_Data.U = unsigned int(tmp_String.c_str()[cou_Index]);
		cout << "\n"; oindex(cou_Index); cout << " " << tmp_String.c_str()[cou_Index] << ": "; oint(0, 15, tmp_Data.U); cout << " "; opoi(0, 13, tmp_Ar[cou_Index]);
	}
	system("PAUSE > NULL");*/


	cout << "\n\t Entering Sub Program..."; cout.flush();
	//ca_main(argc, argv);
	//DBase_Testermon();

	Testermon_Predictor();

	system("PAUSE > NULL");

	return 1;
}

//Modify the pipes to create direct bonds between treetops and other constructs.
//Allow for the direct input into higher constructs charging buffers.
