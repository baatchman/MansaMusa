//Holds a record.
class c_CA_Record
{
public:
     
     // Open time
     unsigned long long int Open_Time;
     
     // Open
     double Open;
     
     // High
     double High;
     
     // Low
     double Low;

	 //Middle, (O + C + H + L) / 2
	 //Used for calculating target as well as the difference between two candles.
	 double Middle;
     
     // Close
     double Close;
     
     // Volume
     double Volume;

	 //For the candlestick.
	 double Candle_Low;
	 double Candle_Open;
	 double Candle_Close;
	 double Candle_High;
	 double Candle_BullBear;
     
     // Close time
     unsigned long long int Close_Time;
     
     // Quote asset volume
     double Quote_Asset_Volume;
     
     // Number of trades
     int Number_Of_Trader;
     
     // Taker buy base asset volume
     double Taker_Buy_Base_Asset_Volume;
     
     // Taker buy quote asset volume
     double Taker_Buy_Quote_Asset_Volume;
     
     c_CA_Record()
     {
          Open_Time = 0;
          Open = 0.0;
          High = 0.0;
          Low = 0.0;
          Close = 0.0;
          Volume = 0.0;
          Close_Time = 0;
          Quote_Asset_Volume = 0.0;
          Number_Of_Trader = 0;
          Taker_Buy_Base_Asset_Volume = 0.0;
          Taker_Buy_Quote_Asset_Volume = 0.0;
		  Middle = 0.0;
		  Candle_Low = 0;
		  Candle_Open = 0;// -Low;
		  Candle_Close = 0;// -Low;
		  Candle_High = 0;// -Low;
		  Candle_BullBear = 0;
     }

	 void load_Record(ifstream* p_File)
	 {
		 *p_File >> Open_Time;
		 *p_File >> Open;
		 *p_File >> High;
		 *p_File >> Low;
		 *p_File >> Close;
		 *p_File >> Volume;
		 *p_File >> Close_Time;
		 *p_File >> Quote_Asset_Volume;
		 *p_File >> Number_Of_Trader;
		 *p_File >> Taker_Buy_Base_Asset_Volume;
		 *p_File >> Taker_Buy_Quote_Asset_Volume;
		 Middle = (Open + High + Low + Close) / 4;
		 Candle_Low = 0;
		 Candle_Open = int(Open - Low);
		 Candle_Close = int(Close - Low);
		 Candle_High = int(High - Low);
		 Candle_BullBear = Candle_Close - Candle_Open;
		 if (Candle_BullBear > 0) { Candle_BullBear = 1; }
		 if (Candle_BullBear < 0) { Candle_BullBear = -1; }
	 }

	 void output_Record()
	 {
		 cout << "\n";
		 cout << "\n Open_Time-" << Open_Time;
		 cout << "\n Open-" << Open;
		 cout << "\n High-" << High;
		 cout << "\n Low-" << Low;
		 cout << "\n Middle-" << Middle;
		 cout << "\n Close-" << Close;
		 cout << "\n Volume-" << Volume;
		 cout << "\n Close_Time-" << Close_Time;
		 cout << "\n Quote_Asset_Volume-" << Quote_Asset_Volume;
		 cout << "\n Number_Of_Trader-" << Number_Of_Trader;
		 cout << "\n Taker_Buy_Base_Asset_Volume-" << Taker_Buy_Base_Asset_Volume;
		 cout << "\n Taker_Buy_Quote_Asset_Volume-" << Taker_Buy_Quote_Asset_Volume;
	 }

	 void output_Record_Compact()
	 {
		 cout << "\n";
		 double tmp_Delta = Close - Open;
		 //tmp_Delta = tmp_Delta / Open;
		 //cout << tmp_Delta;
		 if (tmp_Delta > 0)
		 {
			 for (int cou_Index = 0; cou_Index<int(tmp_Delta); cou_Index++)
			 {
				 cout << "+";
			 }
		 }
		 if (tmp_Delta < 0)
		 {
			 tmp_Delta *= -1;
			 for (int cou_Index = 0; cou_Index<int(tmp_Delta); cou_Index++)
			 {
				 cout << "O";
			 }
		 }
		 //cout << "\n Open_Time-" << Open_Time;
		 //cout << "\n Open-" << Open;
		 //cout << "\n High-" << High;
		 //cout << "\n Low-" << Low;
		 //cout << "\n Close-" << Close;
		 //cout << "\n Volume-" << Volume;
		 //cout << "\n Close_Time-" << Close_Time;
		 //cout << "\n Quote_Asset_Volume-" << Quote_Asset_Volume;
		 //cout << "\n Number_Of_Trader-" << Number_Of_Trader;
		 //cout << "\n Taker_Buy_Base_Asset_Volume-" << Taker_Buy_Base_Asset_Volume;
		 //cout << "\n Taker_Buy_Quote_Asset_Volume-" << Taker_Buy_Quote_Asset_Volume;
	 }
};

//This holds a linked list of records.
class c_CA_DBase_Chunk
{
public:

	c_CA_Record * Records;

	long int Depth;

	//The current record ID ready to be assigned.
	__int64 Record_Current_ID;

	c_CA_DBase_Chunk()
	{
		//Records = NULL;
		Record_Current_ID = 0;
		Records = NULL;
		Depth = 0;
	}

	~c_CA_DBase_Chunk()
	{
		if (Records != NULL) { delete[] Records; Records = NULL; }
	}

	//Setup the records database.
	void init_Records(int p_Count)
	{
		Records = new c_CA_Record[p_Count];
		Depth = p_Count;
	}

	//Accepts a file to read a record from.
	void load_Record(ifstream* p_File)
	{
		//c_CA_Record* tmp_Record = new c_CA_Record;
		//Records.new_LL(Record_Current_ID, tmp_Record);
		Records[Record_Current_ID].load_Record(p_File);
		Record_Current_ID++;
		//tmp_Record->output_Record_Compact();

	}
};

//The database has chunks of records so that non sequential records can be read in and handled by the same database.
//For example when working with incomplete datasets this will allow for loading them all at once instead of having separate databases for each even though the content belongs to the same dataset.
class c_CA_DBase
{
public:
     
	c_CA_DBase_Chunk DBase_Chunk;
	//int Current_DBase_Chunk;

	c_CA_DBase()
	{
		//Current_DBase_Chunk = 0;
	}

     //Loads a DBase
     void load_DBase(string p_DBase_Name)
     {
          //Open the database and verify it.
		 ifstream DBase;
		 cout << "\n FILE:" << p_DBase_Name;
		 DBase.open(p_DBase_Name);
		 if (!(DBase.is_open())) { cout << "\n FAILED TO OPEN FILE"; return; }

		 c_CA_Record tmp_Record;
		 long int Line_Count = 0;


		 //Send the file to the current record.
		 while (!(DBase.eof()))
		 {
			 tmp_Record.load_Record(&DBase);
			 Line_Count++;
		 }

		 cout << "\n --Line_Count:" << Line_Count;


		 DBase_Chunk.init_Records(Line_Count);

		 DBase.close();
		 DBase.open(p_DBase_Name);

		 //Send the file to the current record.
		 while (!(DBase.eof()))
		 {
			 DBase_Chunk.load_Record(&DBase);
		 }
		 cout << "\n Finished Loading DBase " << p_DBase_Name;
     }
     
     //Gets a record from the DBase
     c_CA_Record get_Record(int p_Record_ID)
     {
          
     }

	 //Get chunk of middle ajusted for the highest and lowest changes.
	 //Is flattened to base 5 as well.
	 void get_Middle_Chunk_PC(int p_Record_ID, int p_Length, double* p_Data)
	 {
		 double tmp_Start = DBase_Chunk.Records[p_Record_ID].Middle;
		 cout << "\nPC\n";
		 for (int cou_Index = 0; cou_Index < p_Length; cou_Index++)
		 {
			 p_Data[cou_Index] = DBase_Chunk.Records[p_Record_ID + cou_Index].Middle;
			 cout << "\n[" << cou_Index << "] " << p_Data[cou_Index];
			 p_Data[cou_Index] = p_Data[cou_Index] / tmp_Start;
			 p_Data[cou_Index] = int(((1 - p_Data[cou_Index]) * -1) * 10000);
			 p_Data[cou_Index] = (int(p_Data[cou_Index] / 5)) * 5.0;
			 
			 for (int cou_X = 0; cou_X < p_Data[cou_Index]; cou_X++)
			 {
				 cout << "~";
			 }
			 cout << p_Data[cou_Index];
			 
		 }
	 }

	 //Gets an almost calculated averaged value from a chunk.
	 double get_Averaged_Middle_Chunk(int p_Record_ID, int p_Length)
	 {
		 double tmp_Total = 0.0f;
		 //cout << "\n\n";
		 for (int cou_Index = 0; cou_Index < p_Length; cou_Index++)
		 {
			 tmp_Total += DBase_Chunk.Records[p_Record_ID + cou_Index].Middle;
			 /*
			 for (int cou_X = 0; cou_X < p_Data[cou_Index]; cou_X++)
			 {
				 cout << "~";
			 }
			 cout << p_Data[cou_Index];
			 */
		 }
		 return tmp_Total / p_Length;
	 }



	 //Get chunk of middle ajusted for the highest and lowest changes.
	 //Is flattened to base 5 as well.
	 //p_Length is the length of the input data and the number of averaged chunks to gather.
	 //p_Sub_Length is the length of the chunks to average.
	 void get_Averaged_Middle_Chunk_PC(int p_Record_ID, int p_Length, int p_Sub_Length, double* p_Data)
	 {
		 double tmp_Previous = get_Averaged_Middle_Chunk(p_Record_ID, p_Sub_Length);
		 double tmp_Current = 0.0f;
		 //cout << "\nAVG\n";
		 for (int cou_Index = 0; cou_Index < p_Length; cou_Index++)
		 {
			 p_Data[cou_Index] = get_Averaged_Middle_Chunk((p_Record_ID + (cou_Index * p_Sub_Length)), p_Sub_Length);
			 tmp_Current = p_Data[cou_Index];
			 //cout << "\n[" << cou_Index << "] " << p_Data[cou_Index] << " ";
			 p_Data[cou_Index] = p_Data[cou_Index] / tmp_Previous;
			 p_Data[cou_Index] = int(((1 - p_Data[cou_Index]) * -1) * 10000);

			 p_Data[cou_Index] = (int(p_Data[cou_Index] / 5)) * 5.0;
			 tmp_Previous = tmp_Current;
			 /*
			 for (int cou_X = 0; cou_X < p_Data[cou_Index]; cou_X++)
			 {
				 cout << "~";
			 }
			 cout << p_Data[cou_Index] << "       ";
			 */
			 
		 }
	 }



	 //Get chunk of middle
	 void get_Middle_Chunk(int p_Record_ID, int p_Length, double* p_Data)
	 {
		 for (int cou_Index = 0; cou_Index < p_Length; cou_Index++)
		 {
			 p_Data[cou_Index] = DBase_Chunk.Records[p_Record_ID + cou_Index].Middle;
		 }
	 }



	 //Gets number of records in a database chunk.
	 long int get_DBase_Depth()
	 {
		 return DBase_Chunk.Depth;
	 }

	 //Gets the stats from a given record. Matches string for selection to allow for easily adding functionality.

	 //Get candlestick.
	 void get_Candlestick(int p_Record, double p_Data[4])
	 {
		 p_Data[0] = DBase_Chunk.Records[p_Record].Candle_Low;
		 p_Data[1] = DBase_Chunk.Records[p_Record].Candle_Open;
		 p_Data[2] = DBase_Chunk.Records[p_Record].Candle_Close;
		 p_Data[3] = DBase_Chunk.Records[p_Record].Candle_High;
		 p_Data[4] = DBase_Chunk.Records[p_Record].Candle_BullBear;
	 }

	 //Get candlestick difference
	 double get_Candlestick_Difference(int p_Record_0, int p_Record_1)
	 {
		 return DBase_Chunk.Records[p_Record_1].Middle - DBase_Chunk.Records[p_Record_0].Middle;
	 }

	 //Get the support and resistence bands

	 //Get the fibonacci bands

	 //Get the moving average
	 double get_SMA(int p_Record_ID, int p_Length)
	 {
		 double tmp_SMA = 0.0f;
		 double tmp_Total = 0.0f;

		 for (int cou_Index = p_Record_ID; cou_Index < (p_Record_ID + p_Length); cou_Index++)
		 {
			 tmp_Total += DBase_Chunk.Records[cou_Index].Middle;
		 }
		 tmp_SMA = tmp_Total / p_Length;
		 tmp_SMA = int(DBase_Chunk.Records[p_Record_ID + p_Length - 1].Middle - tmp_SMA);
		 return tmp_SMA;
	 }

	 //Get the trend
	 //Works by splitting the input length in 2 and getting the averages of them. The difference is the trend.
	 //p_Record_ID is the starting point, p_Lenght is the length to get and is divided in half to get the two sections.
	 double get_Trend(int p_Record_ID, int p_Length)
	 {
		 double tmp_Data[2]; tmp_Data[0] = 0.0f; tmp_Data[1] = 0.0f;
		 double tmp_Trend = 0.0;
		 int tmp_Length = p_Length / 2;

		 //Get the two halves.
		 tmp_Data[0] = get_Averaged_Middle_Chunk(p_Record_ID, tmp_Length);
		 tmp_Data[1] = get_Averaged_Middle_Chunk((p_Record_ID + tmp_Length), tmp_Length);

		 tmp_Trend = int(tmp_Data[1] - tmp_Data[0]);

		 return tmp_Trend;
	 }

	 //Get the target.	 
	 //Get chunk of middle ajusted for the highest and lowest changes.
	 //Is flattened to base 5 as well.
	 //p_Length is the length of the input data and the number of averaged chunks to gather.
	 //p_Sub_Length is the length of the chunks to average.
	 double get_Averaged_Middle_Target_PC(int p_Record_ID, int p_Length, int p_Sub_Length, int p_Target_Offset, int p_Target_Length)
	 {
		 double tmp_Start = get_Averaged_Middle_Chunk((p_Record_ID + ((p_Length - 1) * p_Sub_Length)), p_Sub_Length);
		 //cout << "\nAVG_TARGET\n";

		 double tmp_Target = get_Averaged_Middle_Chunk((p_Record_ID + (p_Length * p_Sub_Length) + (p_Target_Offset * p_Sub_Length)), p_Target_Length);

		 //cout << "\n\n[TAR]:" << tmp_Target << " ";

		 tmp_Target = tmp_Target / tmp_Start;
		 tmp_Target = int(((1 - tmp_Target) * -1) * 10000);
		 tmp_Target = (int(tmp_Target / 5)) * 5.0;
		 /*
		 for (int cou_X = 0; cou_X < tmp_Target; cou_X++)
		 {
			cout << "~";
		 }
		 cout << tmp_Target;
		 */
		 return tmp_Target;
	 }




};


void DBase_Testermon()
{
	c_CA_DBase Testermon;
	Testermon.load_DBase("Debug/BTCUSDT.Master.dat");
}