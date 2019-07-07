//What
c_CA_DBase SOURCE;


c_NT3_Construct_MMap_1D ENGINE;
int ENGINE_TDEE;

//Init prediction engine
void init_Engine()
{
	//ENGINE.Save("Engine_Core\\", "CA_Engine");
	ENGINE.Load("Engine_Core\\", "CA_Engine");
	ENGINE_TDEE = 50;
}

//Feed the engine some input.
void feed_Engine()
{
	double Data_Chunk[25];
	
	//Perhaps establish the average and set that as the base 0 and work with deviations, or use the old %change from base

	long int Food_Calories = SOURCE.get_DBase_Chunk_Depth(0);
	cout << "\n Database is worth:" << Food_Calories;

	for (long int cou_Record = 0; cou_Record < (Food_Calories - ENGINE_TDEE); cou_Record++)
	{
		SOURCE.get_Middle_Chunk(0, cou_Record, ENGINE_TDEE, Data_Chunk);

		cout << "\n";
		for (int cou_Index = 0; cou_Index < ENGINE_TDEE; cou_Index++)
		{
			cout << "\n " << cou_Index << ": " << Data_Chunk[cou_Index];
		}

		ENGINE.reset_Inputs();
		ENGINE.ina_A(0, 0, Data_Chunk, ENGINE_TDEE);
		ENGINE.Build();
		ENGINE.output_CAN();
	}
	



	//ENGINE.in(0, 0, "AMERICA #1");
	ENGINE.ina_A(0, 0, Data_Chunk, 10);
	ENGINE.Build();
	ENGINE.output_CAN();
	ENGINE.output_Input_Table_F();
	ENGINE.output_Output_Tables();
}

//Gather engine output
void gather_Engine_Output()
{

}


//Testermon
void Testermon_Predictor()
{
	
	SOURCE.load_DBase("Debug/BTCUSDT.Master.dat");

	init_Engine();

	feed_Engine();

	gather_Engine_Output();
}




















