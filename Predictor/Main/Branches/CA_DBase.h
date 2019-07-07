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

	 //Middle, H + L / 2
	 double Middle;
     
     // Close
     double Close;
     
     // Volume
     double Volume;
     
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
     }

	 void load_Record(ifstream* p_File)
	 {
		 *p_File >> Open_Time;
		 *p_File >> Open;
		 *p_File >> High;
		 *p_File >> Low;
		 Middle = (High + Low) / 2;
		 *p_File >> Close;
		 *p_File >> Volume;
		 *p_File >> Close_Time;
		 *p_File >> Quote_Asset_Volume;
		 *p_File >> Number_Of_Trader;
		 *p_File >> Taker_Buy_Base_Asset_Volume;
		 *p_File >> Taker_Buy_Quote_Asset_Volume;
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
     
	c_CA_DBase_Chunk Chunks[100];
	int Current_DBase_Chunk;

	c_CA_DBase()
	{
		Current_DBase_Chunk = 0;
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


		 Chunks[Current_DBase_Chunk].init_Records(Line_Count);

		 DBase.close();
		 DBase.open(p_DBase_Name);

		 //Send the file to the current record.
		 while (!(DBase.eof()))
		 {
			 Chunks[Current_DBase_Chunk].load_Record(&DBase);
		 }
		 cout << "\n Finished Loading DBase " << p_DBase_Name;
     }
     
     //Gets a record from the DBase
     c_CA_Record get_Record(int p_Record_ID)
     {
          
     }

	 //Get chunk of middle
	 void get_Middle_Chunk(int p_Chunk, int p_Record_ID, int p_Length, double * p_Data)
	 {
		 for (int cou_Index = 0; cou_Index < p_Length; cou_Index++)
		 {
			 p_Data[cou_Index] = Chunks[p_Chunk].Records[p_Record_ID + cou_Index].Middle;
		 }
	 }

	 //Gets number of records in a database chunk.
	 long int get_DBase_Chunk_Depth(int p_Chunk)
	 {
		 return Chunks[p_Chunk].Depth;
	 }

	 //Gets the stats from a given record. Matches string for selection to allow for easily adding functionality.

};

void DBase_Testermon()
{
	c_CA_DBase Testermon;
	Testermon.load_DBase("Debug/BTCUSDT.Master.dat");
}