//What
c_CA_DBase SOURCE;


c_NT3_Command_Parser ENGINE;
int ENGINE_TDEE;

//Init prediction engine
void init_Engine()
{
	//ENGINE.Save("Engine_Core\\", "CA_Engine");
	ENGINE.eval_Command_String("lrealm Engine_Core");
	ENGINE_TDEE = 20;
}

//Train the engine on the database file.
void train_Engine()
{
	double Data_Chunk[50];
	int Target_Offset = 0;
	int Target_Length = 5;

	//Construct IDS
	int Candlestick = 0;
	int Mov_Avg = 2;
	int Trend = 3;
	int Target = 4;
	int ENGINE_ID = 6;

	//The input data arrays
	double tmp_Mov_Avg[5];
	double tmp_Candle[5];
	double tmp_Candle_Diff = 0.0f;
	double tmp_Trend[3];
	double tmp_Target = 0.0f;

	//u_Data_3 tmp_Candles
	u_Data_3 tmp_Data; tmp_Data.U = 0;

	//Perhaps establish the average and set that as the base 0 and work with deviations, or use the old %change from base

	long int Food_Calories = SOURCE.get_DBase_Depth();
	cout << "\n Database is worth:" << Food_Calories;


	long int tmp_End = (Food_Calories - ENGINE_TDEE - (Target_Length + Target_Offset));
	system("CLS");

	for (long int cou_Record = 1000; cou_Record < tmp_End; cou_Record++)
	{
		xy(0, 0);
		cout << " " << cou_Record << " \\ " << tmp_End;

		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[ENGINE_ID])->reset_Inputs();
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[5])->reset_Inputs();
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Mov_Avg])->reset_Inputs();
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Trend])->reset_Inputs();

		for (int cou_Candle = 0; cou_Candle < (ENGINE_TDEE - 1); cou_Candle++)
		{
			SOURCE.get_Candlestick((cou_Record + cou_Candle), tmp_Candle);
			//xy((cou_Candle * 7), 1);
			//cout << " Candle[" << cou_Candle << "] Bull_Or_Bear:" << tmp_Candle[4] << "  L:" << tmp_Candle[0] << " O:" << tmp_Candle[1] << " C:" << tmp_Candle[2] << " H:" << tmp_Candle[3];
			//cout << " Candle_Diff:" << SOURCE.get_Candlestick_Difference((cou_Record + cou_Candle), (cou_Record + cou_Candle + 1)) << "          ";
			//xy((cou_Candle * 5), 3);
			((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[0])->reset_Inputs();
			((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[0])->ina_A(0, 0, tmp_Candle, 5);
			((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[0])->Build();

			tmp_Candle_Diff = SOURCE.get_Candlestick_Difference((cou_Record + cou_Candle - 1), (cou_Record + cou_Candle));

			((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[1])->reset_Inputs();
			((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[1])->in(0, 0, tmp_Candle_Diff);
			((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[1])->Build();

			ENGINE.eval_Command_String("pa 5");
			ENGINE.eval_Command_String("pa 6");
			

			//system("PAUSE > NUL");
			//tmp_Data = ((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[0])->get_Treetop_Data_3();
			//cout << tmp_Data.U;
			//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[0])->output_CAN((cou_Candle * 7), 3);

			//Shift treetop over to the MSC
			//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[5])->ina(0, 0, tmp_Data);

			//Add the difference to the input on the MSC.

		}
		ENGINE.eval_Command_String("build 5");
		
		//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[5])->output_CAN(50, 25);

		cout << "\n\n Mov_Avg 60:  " << int(SOURCE.get_SMA((cou_Record - 60), 60)) << "        ";
		cout << "\n Mov_Avg 30:  " << int(SOURCE.get_SMA((cou_Record - 30), 30)) << "        ";
		cout << "\n Mov_Avg 15:  " << int(SOURCE.get_SMA((cou_Record - 15), 15)) << "        ";
		cout << "\n Mov_Avg 5:  " << int(SOURCE.get_SMA((cou_Record - 5), 5)) << "        ";
		tmp_Mov_Avg[0] = int(SOURCE.get_SMA((cou_Record - 60), 60));
		tmp_Mov_Avg[1] = int(SOURCE.get_SMA((cou_Record - 30), 30));
		tmp_Mov_Avg[2] = int(SOURCE.get_SMA((cou_Record - 15), 15));
		tmp_Mov_Avg[3] = int(SOURCE.get_SMA((cou_Record - 5), 5));

		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Mov_Avg])->ina_A(0, 0, tmp_Mov_Avg, 4);
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Mov_Avg])->Build();
		//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Mov_Avg])->output_CAN(20, 9);

		cout << "\n\n Trend:" << int(SOURCE.get_Trend(cou_Record, 15)) << "        ";
		cout << "\n Trend:" << int(SOURCE.get_Trend(cou_Record, 30)) << "        ";
		cout << "\n Trend:" << int(SOURCE.get_Trend(cou_Record, 60)) << "        ";
		tmp_Trend[0] = int(SOURCE.get_Trend(cou_Record, 15));
		tmp_Trend[1] = int(SOURCE.get_Trend(cou_Record, 30));
		tmp_Trend[2] = int(SOURCE.get_Trend(cou_Record, 60));

		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Trend])->ina_A(0, 0, tmp_Trend, 3);
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Trend])->Build();
		//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Trend])->output_CAN(26, 9);

		tmp_Target = SOURCE.get_Averaged_Middle_Target_PC(cou_Record, ENGINE_TDEE, 1, Target_Offset, Target_Length);

		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Target])->reset_Inputs();
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Target])->in(0, 0, tmp_Target);
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Target])->Build();
		//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Target])->output_CAN(35, 9);

		ENGINE.eval_Command_String("po 7");
		ENGINE.eval_Command_String("pa 8");
		ENGINE.eval_Command_String("pa 9");
		ENGINE.eval_Command_String("pa 10");
		ENGINE.eval_Command_String("build 6");
		//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[6])->output_Input_Table();
		//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[6])->output_Output_Tables("U");
		//system("PAUSE > NUL");
	}
	



	//ENGINE.in(0, 0, "AMERICA #1");
	//ENGINE.ina_A(0, 0, Data_Chunk, 10);
	//ENGINE.Build();
	//ENGINE.output_CAN();
	//ENGINE.output_Input_Table_F();
	//ENGINE.output_Output_Tables();
}

//Feeds the engine input and charges it.
void charge_Engine()
{
	double Data_Chunk[50];
	int Target_Offset = 0;
	int Target_Length = 5;

	//Construct IDS
	int Candlestick = 0;
	int Mov_Avg = 2;
	int Trend = 3;
	int Target = 4;
	int ENGINE_ID = 6;

	//The input data arrays
	double tmp_Mov_Avg[5];
	double tmp_Candle[5];
	double tmp_Trend[3];
	double tmp_Target = 0.0f;

	//u_Data_3 tmp_Candles
	u_Data_3 tmp_Data; tmp_Data.U = 0;

	//Perhaps establish the average and set that as the base 0 and work with deviations, or use the old %change from base

	long int Food_Calories = SOURCE.get_DBase_Depth();
	cout << "\n Database is worth:" << Food_Calories;
	double tmp_Candle_Diff = 0.0f;

	long int tmp_End = (Food_Calories - ENGINE_TDEE - (Target_Length + Target_Offset));
	//system("CLS");

	for (long int cou_Record = 1000; cou_Record < tmp_End; cou_Record++)
	{
		//xy(0, 0);
		cout << " " << cou_Record << " \\ " << tmp_End;

		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[ENGINE_ID])->reset_Inputs();
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[5])->reset_Inputs();
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Mov_Avg])->reset_Inputs();
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Trend])->reset_Inputs();

		for (int cou_Candle = 0; cou_Candle < (ENGINE_TDEE - 1); cou_Candle++)
		{
			SOURCE.get_Candlestick((cou_Record + cou_Candle), tmp_Candle);
			//xy((cou_Candle * 7), 1);
			//cout << " Candle[" << cou_Candle << "] Bull_Or_Bear:" << tmp_Candle[4] << "  L:" << tmp_Candle[0] << " O:" << tmp_Candle[1] << " C:" << tmp_Candle[2] << " H:" << tmp_Candle[3];
			//cout << " Candle_Diff:" << SOURCE.get_Candlestick_Difference((cou_Record + cou_Candle), (cou_Record + cou_Candle + 1)) << "          ";
			//xy((cou_Candle * 5), 3);
			
			((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[0])->reset_Inputs();
			((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[0])->ina_A(0, 0, tmp_Candle, 5);
			((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[0])->Build();

			tmp_Candle_Diff = SOURCE.get_Candlestick_Difference((cou_Record + cou_Candle - 1), (cou_Record + cou_Candle));

			((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[1])->reset_Inputs();
			((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[1])->in(0, 0, tmp_Candle_Diff);
			((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[1])->Build();

			ENGINE.eval_Command_String("pa 5");
			ENGINE.eval_Command_String("pa 6");

		}

		ENGINE.eval_Command_String("eval 5");
		//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[0])->output_Output_Tables("F");
		//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[1])->output_Output_Tables("F");
		//cout << "\n\n Candle_Pat";
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[5])->output_Input_Table_I();
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[5])->output_Output_Tables("U");

		system("PAUSE");

		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[5])->Build();
		//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[5])->output_CAN(50, 25);

		cout << "\n\n Mov_Avg 60:  " << int(SOURCE.get_SMA((cou_Record - 60), 60)) << "        ";
		cout << "\n Mov_Avg 30:  " << int(SOURCE.get_SMA((cou_Record - 30), 30)) << "        ";
		cout << "\n Mov_Avg 15:  " << int(SOURCE.get_SMA((cou_Record - 15), 15)) << "        ";
		cout << "\n Mov_Avg 5:  " << int(SOURCE.get_SMA((cou_Record - 5), 5)) << "        ";
		tmp_Mov_Avg[0] = int(SOURCE.get_SMA((cou_Record - 60), 60));
		tmp_Mov_Avg[1] = int(SOURCE.get_SMA((cou_Record - 30), 30));
		tmp_Mov_Avg[2] = int(SOURCE.get_SMA((cou_Record - 15), 15));
		tmp_Mov_Avg[3] = int(SOURCE.get_SMA((cou_Record - 5), 5));

		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Mov_Avg])->ina_A(0, 0, tmp_Mov_Avg, 4);
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Mov_Avg])->Eval(1);
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Mov_Avg])->output_Output_Tables("D");

		//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Mov_Avg])->output_CAN(20, 9);

		cout << ((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Mov_Avg])->get_Treetop();


		cout << "\n\n Trend:" << int(SOURCE.get_Trend(cou_Record, 15)) << "        ";
		cout << "\n Trend:" << int(SOURCE.get_Trend(cou_Record, 30)) << "        ";
		cout << "\n Trend:" << int(SOURCE.get_Trend(cou_Record, 60)) << "        ";
		tmp_Trend[0] = int(SOURCE.get_Trend(cou_Record, 15));
		tmp_Trend[1] = int(SOURCE.get_Trend(cou_Record, 30));
		tmp_Trend[2] = int(SOURCE.get_Trend(cou_Record, 60));

		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Trend])->ina_A(0, 0, tmp_Trend, 3);
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Trend])->Eval(1);
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Trend])->output_Output_Tables("D");
		//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Trend])->output_CAN(26, 9);

		cout << "\n Target:" << SOURCE.get_Averaged_Middle_Target_PC(cou_Record, ENGINE_TDEE, 1, Target_Offset, Target_Length) << "        ";
		tmp_Target = SOURCE.get_Averaged_Middle_Target_PC(cou_Record, ENGINE_TDEE, 1, Target_Offset, Target_Length);

		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Target])->reset_Inputs();
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Target])->Eval(1);
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Target])->output_Output_Tables("D");
		//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[Target])->output_CAN(35, 9);

		//ENGINE.eval_Command_String("po 7");
		ENGINE.eval_Command_String("po 8");
		ENGINE.eval_Command_String("pa 9");
		ENGINE.eval_Command_String("pa 10");

		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[ENGINE_ID])->output_Input_Tables();
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[ENGINE_ID])->Eval(1);
		((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[ENGINE_ID])->output_Output_Tables("C");
		//((c_NT3_Construct_MMap_1D*)ENGINE.Sandbox.Constructs[ENGINE_ID])->output_CAN(40, 25);
		system("PAUSE");
		//ENGINE.reset_Inputs();
		//ENGINE.ina_A(0, 0, Data_Chunk, (ENGINE_TDEE + 1));
		//ENGINE.Build();
		//ENGINE.output_CAN();
	}
}

//Gather engine output
void gather_Engine_Output()
{
	return;
}

void Engine_Workshop()
{
	ENGINE.rcon();
}

//Testermon
void Testermon_Predictor()
{
	cout << "\n Starting Testermon_Predictor()"; cout.flush();
	//ENGINE.Save("Engine_Core\\", "CA_Engine");
	//return;
	SOURCE.load_DBase("Debug/BTCUSDT.Master.dat");

	//Engine_Workshop();

	init_Engine();
	//Engine_Workshop();

	//train_Engine();
	charge_Engine();

	gather_Engine_Output();

	system("PAUSE > NULL");
	system("PAUSE > NULL");
	system("PAUSE > NULL");
}




















