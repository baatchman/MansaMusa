








class c_Float_Construct: public c_Base_Construct
{
protected:

     //The ID of the construct.
     int CID;

     //The Universal CID for the construct.
     int U_CID;

     //The core processing unit.
     c_Bin_Table_Float Node_Tree;

     //The treetop dereferencing controller for foreign node backpropagation.
     c_Float_Node_Bin_Tree_1D Treetop_Bin_Tree;

     //The file to save the Benchmarking information to.
     ofstream BenchMark;

     //==--     Internals

     //The current number of CANs
     int CAN_Count;

     //The highest tier in the CAN network.
     int Current_Highest_Tier;

     //The RC_Average.
     float RC_Average;

     //==--     SETTINGS

     //The name of the construct.
     string Construct_Name;

     //The name of the assebly this construct belongs to.
     string Assembly_Name;

     //The Filter charge to use.
     float Filter_Charge;

     //The base charge to add.
     int Base_Charge;

     //The charge precision used by the Charging buffer.
     int Charge_Precision;

     //The number of results from a query to keep.
     int Query_Cap;

     //The flg for getting the RCA
     int flg_RC_Average;

     //Whether to search for higher, lower, or split between both.
     int flg_Charging_Leg;

     //The tolerance of the truth table.
     float Tolerance;


public:

     //==--   TABLES

     //The table handler.
     c_Table_Handler * Tables;

     //Input table to build the inputs from.
     c_Table_1D * Input_Table;

     //Input table to gather the truths from.
     c_Table_1D * Input_Table_Truth;

     //The integer array for the status of the output tables.
     int flg_Table_Output[6];

     //The pattern output table.
     c_Table_1D * Output_Table_Pattern;

     //The charge output table.
     c_Table_1D * Output_Table_Charge;

     //The nodes output table.
     c_Table_1D * Output_Table_Nodes;

     //The RC output table.
     c_Table_1D * Output_Table_RC;

     //The truth output table.
     c_Table_1D * Output_Table_Truth;

     //The Vision output.
     c_Table_1D * Output_Table_Vision;

     //The setting table for the construct.
     c_Table_1D * Settings_Table;

     //The treetop table.
     c_Table_1D * Treetop_Table;

     //The internal states.
     c_Table_1D * Internal_Table;


     c_Float_Construct(c_Table_Handler * p_Tables, string p_Assembly_Name, string p_Construct_Name, int p_CID, int p_Type = 0)
     {
          cout << "\n\t c_Float_Construct()";

          //Sets the ID of the construct.
          CID = p_CID;

          //Set the universal CID of the construct and iterates the UCID for the base constructs.
          U_CID = UCID;
          UCID++;

          //Sets the name of the bin table.
          Node_Tree.set_Table_Name(p_Construct_Name + "_Node_Tree");

          //Set all the counters.
          Current_Highest_Tier = 0;
          flg_RC_Average = 0;
          RC_Average = 0.0;

          //Set the settings.
          //Generic

          Filter_Charge = .5;
          Base_Charge = 1000;
          Charge_Precision = 10000;
          Query_Cap = 10;
          Tolerance = 5;

          flg_Table_Output[0] = 3; //Pattern
          flg_Table_Output[1] = 3; //Charge
          flg_Table_Output[2] = 3; //Nodes
          flg_Table_Output[3] = 3; //Truth
          flg_Table_Output[4] = 3; //RC
          flg_Table_Output[5] = 3; //Vision

          flg_Charging_Leg = 2;

          //Set the construct name.
          Construct_Name = p_Construct_Name;
          Assembly_Name = p_Assembly_Name;

          //Set the table handler.
          Tables = p_Tables;

          //Setup the tables.
          Input_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_c_" + Construct_Name + "_Input");
          Input_Table_Truth = Tables->register_New_Table_1D("a_" + Assembly_Name + "_c_" + Construct_Name + "_Input_Truth");

          Output_Table_Pattern = Tables->register_New_Table_1D("a_" + Assembly_Name + "_c_" + Construct_Name + "_Output_Pattern");
          Output_Table_Charge = Tables->register_New_Table_1D("a_" + Assembly_Name + "_c_" + Construct_Name + "_Output_Charge");
          Output_Table_Nodes = Tables->register_New_Table_1D("a_" + Assembly_Name + "_c_" + Construct_Name + "_Output_Nodes");
          Output_Table_Truth = Tables->register_New_Table_1D("a_" + Assembly_Name + "_c_" + Construct_Name + "_Output_Truth");
          Output_Table_RC = Tables->register_New_Table_1D("a_" + Assembly_Name + "_c_" + Construct_Name + "_Output_RC");
          Output_Table_Vision = Tables->register_New_Table_1D("a_" + Assembly_Name + "_c_" + Construct_Name + "_Output_Vision");

          //With the settings table the first index is the name of the setting.
          Settings_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_c_" + Construct_Name + "_Settings");

          Internal_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_c_" + Construct_Name + "_Internal");
          Treetop_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_c_" + Construct_Name + "_Treetop");

          update_Internal_Table();
          update_Settings_Table();

          //Open the benchmarking file.
          string tmp_File = "BenchMarking\\" + Construct_Name + "_BenchMark.dat";
          BenchMark.open(tmp_File.c_str());
          BenchMark << "\n\n Begin Test";
     }

     ~c_Float_Construct()
     {
          BenchMark.close();

          //Update the registry.
          Tables->delete_Table(Input_Table->get_Table_ID());
          Tables->delete_Table(Input_Table_Truth->get_Table_ID());

          Tables->delete_Table(Output_Table_Pattern->get_Table_ID());
          Tables->delete_Table(Output_Table_Charge->get_Table_ID());
          Tables->delete_Table(Output_Table_Nodes->get_Table_ID());
          Tables->delete_Table(Output_Table_Truth->get_Table_ID());
          Tables->delete_Table(Output_Table_RC->get_Table_ID());
          Tables->delete_Table(Output_Table_Vision->get_Table_ID());

          Tables->delete_Table(Settings_Table->get_Table_ID());
          Tables->delete_Table(Internal_Table->get_Table_ID());
          Tables->delete_Table(Treetop_Table->get_Table_ID());
     }

     //--     TABLE HANDLING

     //Gets the ID of the input table.
     int get_Input_Table_ID(){ return Input_Table->get_Table_ID(); }
     int get_Input_Table_Truth_ID(){ return Input_Table_Truth->get_Table_ID(); }

     int get_Output_Table_Charge_ID(){ return Output_Table_Charge->get_Table_ID(); }
     int get_Output_Table_Pattern_ID(){ return Output_Table_Pattern->get_Table_ID(); }
     int get_Output_Table_Nodes_ID(){ return Output_Table_Nodes->get_Table_ID(); }
     int get_Output_Table_Truth_ID(){ return Output_Table_Truth->get_Table_ID(); }
     int get_Output_Table_RC_ID(){ return Output_Table_RC->get_Table_ID(); }
     int get_Output_Table_Vision_ID(){ return Output_Table_Vision->get_Table_ID(); }

     int get_Settings_Table_ID(){ return Settings_Table->get_Table_ID(); }
     int get_Internal_Table_ID(){ return Internal_Table->get_Table_ID(); }
     int get_Treetop_Table_ID(){ return Treetop_Table->get_Table_ID(); }
     //Gets the Name of the table.
     string get_Input_Table_Name(){ return Input_Table->get_Table_Name(); }
     string get_Input_Table_Truth_Name(){ return Input_Table_Truth->get_Table_Name(); }

     string get_Output_Table_Charge_Name(){ return Output_Table_Charge->get_Table_Name(); }
     string get_Output_Table_Pattern_Name(){ return Output_Table_Pattern->get_Table_Name(); }
     string get_Output_Table_Nodes_Name(){ return Output_Table_Nodes->get_Table_Name(); }
     string get_Output_Table_Truth_Name(){ return Output_Table_Truth->get_Table_Name(); }
     string get_Output_Table_RC_Name(){ return Output_Table_RC->get_Table_Name(); }
     string get_Output_Table_Vision_Name(){ return Output_Table_Vision->get_Table_Name(); }

     string get_Settings_Table_Name(){ return Settings_Table->get_Table_Name(); }
     string get_Internal_Table_Name(){ return Internal_Table->get_Table_Name(); }
     string get_Treetop_Table_Name(){ return Treetop_Table->get_Table_Name(); }

     //Gets a reference to a table.
     c_Table_1D * get_Input_Table_Reference(){ return Input_Table; }
     c_Table_1D * get_Input_Table_Truth_Reference(){ return Input_Table_Truth; }

     c_Table_1D * get_Output_Table_Charge_Reference(){ return Output_Table_Charge; }
     c_Table_1D * get_Output_Table_Pattern_Reference(){ return Output_Table_Pattern; }
     c_Table_1D * get_Output_Table_Nodes_Reference(){ return Output_Table_Nodes; }
     c_Table_1D * get_Output_Table_Truth_Reference(){ return Output_Table_Truth; }
     c_Table_1D * get_Output_Table_RC_Reference(){ return Output_Table_RC; }
     c_Table_1D * get_Output_Table_Vision_Reference(){ return Output_Table_Vision; }

     c_Table_1D * get_Settings_Table_Reference(){ return Settings_Table; }
     c_Table_1D * get_Internal_Table_Reference(){ return Internal_Table; }
     c_Table_1D * get_Treetop_Table_Reference(){ return Treetop_Table; }


     //Resets the tables.
     void reset_Tables()
     {
          Input_Table->reset_Table();
          Input_Table_Truth->reset_Table();

          Output_Table_Charge->reset_Table();
          Output_Table_Pattern->reset_Table();
          Output_Table_Nodes->reset_Table();
          Output_Table_Truth->reset_Table();
          Output_Table_RC->reset_Table();
          Output_Table_Vision->reset_Table();

          Treetop_Table->reset_Table();
     }

     //Resets the output tables only.
     void reset_Output_Tables()
     {
          Output_Table_Charge->reset_Table();
          Output_Table_Pattern->reset_Table();
          Output_Table_Nodes->reset_Table();
          Output_Table_Truth->reset_Table();
          Output_Table_RC->reset_Table();
          Output_Table_Vision->reset_Table();
     }

     //Updates the settings table from the internal values.
     void update_Settings_Table()
     {
          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(0, 0, "Construct Name");
          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(0, 1, Construct_Name);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(1, 0, "Construct ID");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(1, 1, CID);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(2, 0, "Filter_Charge");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_F(2, 1, Filter_Charge);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(3, 0, "Base_Charge");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_F(3, 1, Base_Charge);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(4, 0, "Charge_Precision");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_F(4, 1, Charge_Precision);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(5, 0, "Charging 0:Lower, 1:Higher, 2:Split");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_F(5, 1, flg_Charging_Leg);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(6, 0, "Query_Cap");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(6, 1, Query_Cap);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(7, 0, "flg_RC_Average");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(7, 1, flg_RC_Average);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(8, 0, "Tolerance");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(8, 1, Tolerance);
          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(8, 2, "~Variance allowed for being considered true.");


          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(10, 0, "Output_Table_Charge");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(10, 1, flg_Table_Output[0]);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(11, 0, "Output_Table_Pattern");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(11, 1, flg_Table_Output[1]);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(12, 0, "Output_Table_Nodes");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(12, 1, flg_Table_Output[2]);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(13, 0, "Output_Table_Truth");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(13, 1, flg_Table_Output[3]);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(14, 0, "Output_Table_RC");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(14, 1, flg_Table_Output[4]);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(15, 0, "Output_Table_Vision");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(15, 1, flg_Table_Output[5]);

     }

     //Updates the internal table from the internal values.
     void update_Internal_Table()
     {
          if (Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(7, 1, 0))
          {
               //RC_Average = Node_Network.get_RCA(1);
          }

          Internal_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(2, 0, "RC_Average_Top_Tier");
          Internal_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(2, 1, RC_Average);
     }

     //Updates the values of the variables based on the settings table.
     void update_Settings()
     {
          Settings_Table->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(0, 0, &Construct_Name);
          CID = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(1, 1, 0);
          Filter_Charge = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(2, 1, 0);
          Base_Charge = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(3, 1, 0);
          Charge_Precision = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(4, 1, 0);
          flg_Charging_Leg = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(5, 1, 0);
          Query_Cap = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(6, 1, 0);
          flg_RC_Average = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(7, 1, 0);
          Tolerance = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(8, 1, 0);


          flg_Table_Output[0] = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(10, 1, 0);
          flg_Table_Output[1] = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(11, 1, 0);
          flg_Table_Output[2] = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(12, 1, 0);
          flg_Table_Output[3] = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(13, 1, 0);
          flg_Table_Output[4] = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(14, 1, 0);
     }

     //--      CONSTRUCT PROPERTIES MANIPULATION

     //Changes all settings in one go.
     void change_Settings(string p_Construct_Name, string p_Assembly_Name, int p_Construct_Input_Type, int p_Base_Charge, float p_Filter_Charge, int p_Charge_Precision, int p_Query_Cap, int p_Charging_Tier, int p_Tolerance)
     {

     }

     //Changes the name of the assembly and updates the tables in the registry.
     void set_Construct_Name(string p_Construct_Name)
     {
          //Set the name.
          Construct_Name = p_Construct_Name;

          //Updates the tables to reflect the new name.
          update_Table_Names();

     }

     //Gets the name of the construct.
     string get_Construct_Name()
     {
          return Construct_Name;
     }

     //Updates the assembly name for the construct.
     void set_Assembly_Name(string p_Assembly)
     {
          Assembly_Name = p_Assembly;

          //Update the tables.
          update_Table_Names();
     }

     //Gets the name of the assembly.
     string get_Assembly_Name()
     {
          return Assembly_Name;
     }

     //Gets the ID of the construct.
     int get_Construct_ID()
     {
          return CID;
     }

     int set_Input_Type(int p_Type){ return 0; }
     int get_Input_Type(){ return 0; }

     //Updates the table names in the table handler.
     void update_Table_Names()
     {
          //Update the registry.
          Tables->set_Table_Name(Input_Table->get_Table_ID(), "a_" + Assembly_Name + "_c_" + Construct_Name + "_Input");
          Tables->set_Table_Name(Input_Table_Truth->get_Table_ID(), "a_" + Assembly_Name + "_c_" + Construct_Name + "_Input_Truth");

          Tables->set_Table_Name(Output_Table_Pattern->get_Table_ID(), "a_" + Assembly_Name + "_c_" + Construct_Name + "_Output_Pattern");
          Tables->set_Table_Name(Output_Table_Charge->get_Table_ID(), "a_" + Assembly_Name + "_c_" + Construct_Name + "_Output_Charge");
          Tables->set_Table_Name(Output_Table_Nodes->get_Table_ID(), "a_" + Assembly_Name + "_c_" + Construct_Name + "_Output_Nodes");
          Tables->set_Table_Name(Output_Table_Truth->get_Table_ID(), "a_" + Assembly_Name + "_c_" + Construct_Name + "_Output_Truth");
          Tables->set_Table_Name(Output_Table_RC->get_Table_ID(), "a_" + Assembly_Name + "_c_" + Construct_Name + "_Output_RC");

          Tables->set_Table_Name(Internal_Table->get_Table_ID(), "a_" + Assembly_Name + "_c_" + Construct_Name + "_Internal");
          Tables->set_Table_Name(Treetop_Table->get_Table_ID(), "a_" + Assembly_Name + "_c_" + Construct_Name + "_Treetop");
          Tables->set_Table_Name(Settings_Table->get_Table_ID(), "a_" + Assembly_Name + "_c_" + Construct_Name + "_Settings");


          update_Internal_Table();
          update_Settings_Table();
     }





     void Eval()
     {

     }

     void Build()
     {

     }

     //Submit an input to the input table.
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -"INPUT"
     //   0            1          2          3       4      5
     void push_Input(int p_Row, string p_Input)
     {
          Input_Table->push_string(p_Row, p_Input);
     }

     void push_Input_Truth(int p_Row, string p_Input)
     {
          Input_Table_Truth->push_string(p_Row, p_Input);
     }

     //Submit an input to the input table.
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -CELL -"INPUT"
     //   0            1          2          3       4    5       6
     void set_Input(int p_Row, int p_Cell, string p_Input)
     {
          Input_Table->set_string(p_Row, p_Cell, p_Input);
     }

     void eval_Input_Table(int p_Building_Method=0, int p_Charging_Method=0)
     {
          //The number of cells in the current row.
          int tmp_Row_Count = Input_Table->get_Row_Count();

          //Neutralizes all the output tables.
          Output_Table_Charge->reset_Table();
          Output_Table_Pattern->reset_Table();
          Output_Table_Nodes->reset_Table();
          Output_Table_Truth->reset_Table();
          Output_Table_RC->reset_Table();
          Output_Table_Vision->reset_Table();
          Treetop_Table->reset_Table();

          //Build all the input rows.
          for (int cou_Index=0;cou_Index<tmp_Row_Count;cou_Index++)
          {
               build_Input_Row(cou_Index, p_Building_Method);
          }

          //Sort the table for the upcoming election.
          Node_Tree.sort_Nodes_Float();

          for (int cou_Index=0;cou_Index<tmp_Row_Count;cou_Index++)
          {
               eval_Input_Row(cou_Index);
          }

          update_Internal_Table();
     }

     void eval_Input_Row(int p_Row)
     {
          //string copy_Row_Set(void * p_Table_To_Copy_From, int p_Start, int p_End, void * p_Table_To_Copy_To, int p_Offset)
          int tmp_TRID = 0;

          //Eval all the cells in the given row..
          for (int cou_Index=0;cou_Index<Input_Table->get_Row_Cell_Count(p_Row);cou_Index++)
          {
               tmp_TRID = Node_Tree.get_TRID(str2float(Input_Table->get_string(p_Row, cou_Index)));
               discharge_Treetop(tmp_TRID, p_Row);
          }
     }



     void build_Input_Table(int p_Building_Method=0, int p_Charging_Method=0)
     {
          //The number of cells in the current row.
          int tmp_Row_Count = Input_Table->get_Row_Count();

          //Neutralizes all the output tables.
          Treetop_Table->reset_Table();

          //Eval all the input rows.
          for (int cou_Index=0;cou_Index<tmp_Row_Count;cou_Index++)
          {
               build_Input_Row(cou_Index, p_Building_Method);
          }

          //Sort the tree after.
          //Node_Tree.sort_Nodes_Float();

          //Treetop_Table->output_Table();

          update_Internal_Table();
     }

     void build_Input_Row(int p_Row, int p_Building_Method)
     {
          c_Num_Bin_Node_Float * tmp_Node = NULL;
          u_Data tmp_Data;

          //Eval all the cells in the given row..
          for (int cou_Index=0;cou_Index<Input_Table->get_Row_Cell_Count(p_Row);cou_Index++)
          {
               float tmp_Input = str2float(Input_Table->get_string(p_Row, cou_Index));
               if (p_Building_Method == 0){ tmp_Node = Node_Tree.submit_Input(tmp_Input); }
               if (p_Building_Method == 1){ tmp_Node = Node_Tree.submit_Input_RC(tmp_Input); }

               Treetop_Table->push_reference(0, tmp_Node);
               tmp_Data.NR = tmp_Node;

               Treetop_Bin_Tree.search(tmp_Data);
               Treetop_Bin_Tree.set_Current_Node_NID(tmp_Node);
          }
     }

     void build_Input_Table_BM(int p_Building_Method=0, int p_Charging_Method=0)
     {
          //The number of cells in the current row.
          int tmp_Row_Count = Input_Table->get_Row_Count();

          //Neutralizes all the output tables.
          Treetop_Table->reset_Table();

          //Eval all the input rows.
          for (int cou_Index=0;cou_Index<tmp_Row_Count;cou_Index++)
          {
               build_Input_Row(cou_Index, p_Building_Method);
          }

          //Sort the tree after.
          //Node_Tree.sort_Nodes_Float();

          //Treetop_Table->output_Table();

          update_Internal_Table();
     }

     void build_Input_Row_BM(int p_Row, int p_Building_Method)
     {
          c_Num_Bin_Node_Float * tmp_Node = NULL;
          u_Data tmp_Data;

          //Eval all the cells in the given row..
          for (int cou_Index=0;cou_Index<Input_Table->get_Row_Cell_Count(p_Row);cou_Index++)
          {
               float tmp_Input = str2float(Input_Table->get_string(p_Row, cou_Index));
               if (p_Building_Method == 0){ tmp_Node = Node_Tree.submit_Input(tmp_Input); }
               if (p_Building_Method == 1){ tmp_Node = Node_Tree.submit_Input_RC(tmp_Input); }

               Treetop_Table->push_reference(0, tmp_Node);
               tmp_Data.NR = tmp_Node;

               Treetop_Bin_Tree.search(tmp_Data);
               Treetop_Bin_Tree.set_Current_Node_NID(tmp_Node);
          }
     }

     void discharge_Treetop(int p_TRID, int p_Row, int p_Oflg = 2)
     {
          //Pop the ouput tables if past the first row.
          if (flg_Table_Output[0] == p_Oflg || flg_Table_Output[0] == 3){ if (!Output_Table_Pattern->table_Is_Empty()){ Output_Table_Pattern->pop_Row(); } }
          if (flg_Table_Output[1] == p_Oflg || flg_Table_Output[1] == 3){ if (!Output_Table_Charge->table_Is_Empty()){ Output_Table_Charge->pop_Row(); } }
          if (flg_Table_Output[2] == p_Oflg || flg_Table_Output[2] == 3){ if (!Output_Table_Nodes->table_Is_Empty()){ Output_Table_Nodes->pop_Row(); } }
          if (flg_Table_Output[3] == p_Oflg || flg_Table_Output[3] == 3){ if (!Output_Table_Truth->table_Is_Empty()){ Output_Table_Truth->pop_Row(); } }
          if (flg_Table_Output[4] == p_Oflg || flg_Table_Output[4] == 3)
          {
               //Avoid /0 errors.
               RC_Average = Node_Tree.get_RC_Average();

               if (RC_Average == 0.0){ RC_Average = 1; }

               if (!Output_Table_RC->table_Is_Empty())
               {
                    Output_Table_RC->pop_Row();
               }
          }
          if (flg_Table_Output[5] == p_Oflg || flg_Table_Output[5] == 3){ if (!Output_Table_Vision->table_Is_Empty()){ Output_Table_Vision->pop_Row(); } }


          //==--Gather the tables.

          //Get the low and high indexes to use.
          int tmp_Low = 0;
          int tmp_High = 0;
          int tmp_Query_Range = Query_Cap / 2;

          //Search < TRID
          if (flg_Charging_Leg == 0)
          {
               tmp_Low = p_TRID - Query_Cap;

               //No bounding errors to see here.
               if (tmp_Low < 0){ tmp_Low = 0; }

               tmp_High = p_TRID;
          }
          //Search > TRID
          if (flg_Charging_Leg == 1)
          {
               tmp_Low = p_TRID;
               tmp_High = p_TRID + Query_Cap;

               //No bounding errors allowed.
               if (tmp_High >= Node_Tree.get_Row_Count()){ tmp_High = Node_Tree.get_Row_Count() - 1; }
          }
          //Search TRID + (< Query_Cap / 2) + (> Query_Cap / 2)
          if (flg_Charging_Leg == 2)
          {
               tmp_Low = p_TRID - tmp_Query_Range;
               tmp_High = p_TRID + tmp_Query_Range + 1;

               //No bounding errors allowed.
               if (tmp_Low < 0){ tmp_Low = 0; }
               if (tmp_High >= Node_Tree.get_Row_Count()){ tmp_High = Node_Tree.get_Row_Count() - 1; }
          }

          //These hold the temporary charges as they are read in.
          int tmp_Charge = 0;
          float tmp_Difference = 0.0;
          float tmp_Charge_Modifier = 0.0;
          float tmp_TRID_Value = Node_Tree.get_Pattern(p_TRID);
          float tmp_TRID_Related_Value = 0.0;
          c_Num_Bin_Node_Float * tmp_Node = NULL;
          u_Data tmp_Data;

          //Loop through all of the indexes allocated by high and low.
          //This is where the tables are gathered.
          for (int cou_Index=tmp_Low;cou_Index<=tmp_High;cou_Index++)
          {

               //flg_Table_Output[0] //Pattern
               //flg_Table_Output[1] //Charge
               //flg_Table_Output[2] //Nodes
               //flg_Table_Output[3] //Truth
               //flg_Table_Output[4] //RC
               //flg_Table_Output[5] //Vision

               //Get the current TRID Value.
               tmp_TRID_Related_Value = Node_Tree.get_Pattern(cou_Index);

               if (tmp_TRID_Related_Value == 0.0){ Output_Table_Charge->push_float(cou_Index, 0.0); continue; }

               //Get the difference.
               tmp_Difference = absolutef(tmp_TRID_Related_Value - tmp_TRID_Value);

               //Get the charge modifier.
               tmp_Charge_Modifier = tmp_Difference / tmp_TRID_Related_Value;

               //Get the temporary charge.
               tmp_Charge = Charge_Precision - (Charge_Precision * tmp_Charge_Modifier);

               if (tmp_Charge < (Base_Charge * Filter_Charge))
               {
                    continue;
               }

               if (flg_Table_Output[0] == p_Oflg || flg_Table_Output[0] == 3){ Output_Table_Pattern->pushc_string(float2str(tmp_TRID_Related_Value)); }
               if (flg_Table_Output[1] == p_Oflg || flg_Table_Output[1] == 3)
               {
                    Output_Table_Charge->pushc_int(tmp_Charge);
               }
               if (flg_Table_Output[2] == p_Oflg || flg_Table_Output[2] == 3)
               {
                    tmp_Node = Node_Tree.submit_Input(tmp_TRID_Related_Value);
                    Output_Table_Nodes->pushc_reference(tmp_Node);
                    tmp_Data.NR = tmp_Node;
                    Treetop_Bin_Tree.search(tmp_Data);
                    Treetop_Bin_Tree.set_Current_Node_NID(tmp_Node);
               }
               if (flg_Table_Output[3] == p_Oflg || flg_Table_Output[3] == 3)
               {
                    float tmp_Low_Tolerance = tmp_TRID_Related_Value - Tolerance;
                    float tmp_High_Tolerance = tmp_TRID_Related_Value + Tolerance;
                    float tmp_Truth = str2float(Input_Table_Truth->get_string(p_Row, 0));

                    if (tmp_Truth >= tmp_Low_Tolerance && tmp_Truth <= tmp_High_Tolerance)
                    {
                         Output_Table_Truth->pushc_bool(1);
                    }
                    else
                    {
                         Output_Table_Truth->pushc_bool(0);
                    }
               }
               if (flg_Table_Output[4] == p_Oflg || flg_Table_Output[4] == 3)
               {
                    //cout << "\n\t\t cou_Index->" << cou_Index;
                    //cout << " score = " << Node_Tree.get_RC_Score(cou_Index) << " / " << RC_Average << " = " << (Node_Tree.get_RC_Score(cou_Index) / RC_Average);
                    Output_Table_RC->pushc_float((Node_Tree.get_RC_Score(cou_Index) / RC_Average));
                    //cout << "\n\t\t Score gathered...";
               }
               if (flg_Table_Output[5] == p_Oflg || flg_Table_Output[5] == 3){  }
          }
     }

     void gather_Output_Tables()
     {

          //Get the RCA is need be.
          if (flg_Table_Output[4] == 3)
          {
               //Avoid /0 errors.
               RC_Average = Node_Tree.get_RC_Average();

               if (RC_Average == 0.0){ RC_Average = 1; }
          }

          //Reset the output tables other than the pattern and charge.
          Output_Table_RC->reset_Table();
          Output_Table_Truth->reset_Table();
          Output_Table_Vision->reset_Table();

          //flg_Table_Output[0] //Pattern
          //flg_Table_Output[1] //Charge
          //flg_Table_Output[2] //Nodes
          //flg_Table_Output[3] //Truth
          //flg_Table_Output[4] //RC
          //flg_Table_Output[5] //Vision

          c_Cell_1D tmp_Cell;

          //The RC is pushed as a string. It will be read as a pattern.
          float tmp_RC_Score;

          //This will hold the node related to the current
          int tmp_TRID = 0;

          //Holds the current charge for the current pattern.
          int tmp_Pattern_Charge = 0;

          //Holds the current pattern.
          u_Data tmp_Pattern;

          //Holds the information for the current vision.
          float tmp_Vision = 0;
          int tmp_Vision_Charge = 0;

          cout << "\n\n\t Gathering output tables for " << Construct_Name;

          //Loop through all of the rows in the pattern table evaluating each cell in the rows.
          for (int cou_Row=0;cou_Row<Output_Table_Pattern->get_Row_Count();cou_Row++)
          {
               cout << "\n\t\t Row->" << cou_Row;

               for (int cou_Cell=0;cou_Cell<Output_Table_Pattern->get_Row_Cell_Count(cou_Row);cou_Cell++)
               {
                    cout << "\n\t\t\t Cell->" << cou_Cell;

                    //Get the pattern and then the trid associated with it along with the charge.
                    tmp_Pattern.F = str2float(Output_Table_Pattern->get_string(cou_Row, cou_Cell));
                    tmp_TRID = Node_Tree.get_TRID(tmp_Pattern.F);
                    tmp_Pattern_Charge = Output_Table_Charge->get_int(cou_Row, cou_Cell);

                    //Get the RC score.
                    tmp_RC_Score = Node_Tree.get_RC_Score(tmp_TRID) / RC_Average;

                    //Redpill the node.
                    if (flg_Table_Output[3] > 0)
                    {
                         cout << "\t Truth";
                         float tmp_Low_Tolerance = tmp_Pattern.F - Tolerance;
                         float tmp_High_Tolerance = tmp_Pattern.F + Tolerance;

                         float tmp_Truth = str2float(Input_Table_Truth->get_string(cou_Row, 0));

                         if (tmp_Truth >= tmp_Low_Tolerance && tmp_Truth <= tmp_High_Tolerance)
                         {
                              Output_Table_Truth->push_bool(cou_Row, 1);
                         }
                         else
                         {
                              Output_Table_Truth->push_bool(cou_Row, 0);
                         }
                    }
                    //Get the score.
                    if (flg_Table_Output[4] > 0)
                    {
                         cout << "\t Score";
                         Output_Table_RC->set_float(cou_Row, cou_Cell, tmp_RC_Score);
                    }

                    //Gather the vision of the future.
                    //In the future it may be an option to average the visions.
                    if (flg_Table_Output[5] > 0)
                    {
                         cout << "\t Vision->" << tmp_Pattern.F;

                         //If the current node has a higher charge then set it to be the vision.
                         if ((tmp_Pattern_Charge * tmp_Pattern_Charge) >= tmp_Vision_Charge)
                         {
                              tmp_Vision = tmp_Pattern.F;
                              tmp_Vision_Charge = (tmp_Pattern_Charge * tmp_RC_Score);
                         }

                    }
               }
               if (flg_Table_Output[5] > 0)
               {
                    cout << "\t Whole_Vision->" << tmp_Vision << " C->" << tmp_Vision_Charge;
                    Output_Table_Vision->set_float(cou_Row, 0, tmp_Vision);
               }
          }

          output_Tables();
     }

     //Gets the universal CID of the construct.
     int get_Construct_U_CID()
     {
          return U_CID;
     }

     //Initiates a backpropagation in the given node, submits the given cell to gather the pattern in. Used mostly for Chrono gathering. Only charges right leg.
     virtual void initiate_Backpropagation_Foreign_Node(u_Data p_Data, int p_Row, int p_Charge)
     {
          //Pop the cell in the given row if it is not empty.
          if (!Output_Table_Pattern->row_Is_Empty(p_Row)){ Output_Table_Pattern->pop_Cell(p_Row); }

          //Push the charge.
          Output_Table_Charge->push_int(p_Row, p_Charge);

          c_Num_Bin_Node_Float * tmp_Node = Treetop_Bin_Tree.exists(p_Data);

          if (tmp_Node == NULL)
          {
               Output_Table_Pattern->push_string(p_Row, "0.0f");
               return;
          }

          //Gather the backpropogated pattern using the current cell in the given row.
          Output_Table_Pattern->push_string(p_Row, float2str(tmp_Node->Pattern));
     }

     //==--     OLD MEMBERS THAT DO NOT APPLY TO THIS CONSTRUCT


     void eval_Foreign_Output_Table(c_Table_1D * p_Output_Table_Nodes, c_Table_1D * p_Output_Table_Charges){}
     void output_given_CAN(int p_CAN){}
     void output_Every_CAN(){}
     void output_Node_Network(){}
     void output_Node_Network_Tier(int p_Tier){}
     void output_Node_Network_Stats(){}
     void inflate_State_Coordinates(){}
     c_Base_Node_Network_1D * get_Node_Network_Reference(){ return NULL; }

     void output_Construct_Internals()
     {
          Node_Tree.output_Bin_Tree();
     }


     //Outputs the tables containted within.
     void output_Tables()
     {
          Input_Table->output_Table();
          Input_Table_Truth->output_Table();
          Treetop_Table->output_Table();
          Output_Table_Charge->output_Table();
          Output_Table_Pattern->output_Table();
          Output_Table_Nodes->output_Table();
          Output_Table_Truth->output_Table();
          Output_Table_RC->output_Table();
     }

     //Outputs the settings table after updating it from internal values.
     void output_Settings_Table()
     {
          update_Settings_Table();
          Settings_Table->output_Table();
     }

     //Outputs the constructs tables.
     void output_Construct_Tables()
     {
          Tables->output_Table(Settings_Table->get_Table_ID());
          Tables->output_Table(Internal_Table->get_Table_ID());
          Tables->output_Table(Input_Table->get_Table_ID());
          Tables->output_Table(Treetop_Table->get_Table_ID());
          Tables->output_Table(Output_Table_Charge->get_Table_ID());
          Tables->output_Table(Output_Table_Nodes->get_Table_ID());
          Tables->output_Table(Output_Table_RC->get_Table_ID());
          Tables->output_Table(Output_Table_Pattern->get_Table_ID());
     }



     //Outputs the construct.
     void output_Construct_Full()
     {
          cout << "\n\n\t----------";
          cout << "\n\t Construct->" << Construct_Name << " " << this;
          output_Tables();
          output_Construct_Internals();
     }

     //Outputs the construct.
     void output_Construct()
     {
          cout << "\n\n\t----------";
          cout << "\n\t Construct->" << Construct_Name << " " << this;
          output_Settings_Table();
     }

     //Gets the stats from the construct in a string that can be echoed to a file.
     string get_Stats()
     {
          return " C.f:" + Construct_Name + " #N:" + int2str(Node_Tree.get_Row_Count());
     }

     //==--     SAVING AND LOADING

     void Save(string p_Directory)
     {

     }

     void load(string p_Directory, string p_File)
     {

     }
};




