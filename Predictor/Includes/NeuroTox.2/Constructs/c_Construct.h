
class c_Base_Construct_1D: public c_Base_Construct
{
protected:

     //The ID of the construct.
     int CID;

     //The Universal CID for the construct.
     int U_CID;

     //The CANs contained in the construct.
     c_Base_CAN_1D ** CAN;

     //The file to save the Benchmarking information to.
     ofstream BenchMark;

     //==--     Internals

     //The current number of CANs
     int CAN_Count;

     //The highest tier in the CAN network.
     int Current_Highest_Tier;

     //The RC_Average.
     float RC_Average;

     //The Node_Network used by the construct.
     c_Base_Node_Network_1D Node_Network;

     //The charging buffer used to evaluate the node network.
     c_Charging_Buffer_1D Charging_Buffer;

     //The torch buffer for discharging.
     c_Torch_Buffer_1D Torch_Buffer;

     //The type of construct.
     int Construct_Type;
     
     //The threads for the construct.
     thread ** Threads;
     
     //For the charging of the stack.
     c_Charging_Buffer_Linked_Node_1D ** tmp_Node;

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

     //The tier to start charging on.
     int Charging_Tier;

     //The limit on truth depth, used mainly for predictors where the only truth wanted is the most recent.
     int Tolerance;

     //The type of construct input, raw = 0, MSC = 1.
     int Construct_Input_Type;

     //The building method to use for the construct.
     string Building_Method;

     //The evaluation method to use.
     string Eval_Method;

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

     //The subsystem the output_Table_Vision relies on.
     c_Vision_1D Output_Vision_Tree;

     //The setting table for the construct.
     c_Table_1D * Settings_Table;

     //The treetop table.
     c_Table_1D * Treetop_Table;

     //The internal states.
     c_Table_1D * Internal_Table;

     c_Base_Construct_1D(c_Table_Handler * p_Tables, string p_Assembly_Name, string p_Construct_Name, int p_CID, int p_Type = 0)
     {
          cout << "\n\t c_Base_Construct_1D()";

          //No CANs yet.
          CAN = NULL;
          CAN_Count = 0;

          //Sets the ID of the construct.
          CID = p_CID;

          //Set the universal CID of the construct and iterates the UCID for the base constructs.
          U_CID = UCID;
          UCID++;

          //Set the type of construct.
          Construct_Type = p_Type;

          //Set all the counters.
          Current_Highest_Tier = 0;
          CAN_Count = 0;
          flg_RC_Average = 0;
          RC_Average = 0.0;

          //Set the settings.
          Charging_Tier = 0;
          Filter_Charge = .1;
          Base_Charge = 10000;
          Charge_Precision = 10000;
          Query_Cap = 100;
          Tolerance = 1;
          Construct_Input_Type = 0;
          Building_Method = "full_RC";
          Eval_Method = "query";
          /*
          if (p_Building_Method
          0) build_Every_CAN_Full
          1) build_Every_CAN_Full_RC
          2) build_Every_CAN_Query
          3) build_Every_CAN_Full__Build_Row_As_One
          4) build_Every_CAN_Full_RC__Build_Row_As_One
          */

          //Generic
          if (p_Type == 0)
          {
               flg_Table_Output[0] = 3; //Pattern
               flg_Table_Output[1] = 3; //Charge
               flg_Table_Output[2] = 3; //Nodes
               flg_Table_Output[3] = 3; //Truth
               flg_Table_Output[4] = 3; //RC
               flg_Table_Output[5] = 3; //Vision
          }
          //Raw
          if (p_Type == 1)
          {
               flg_Table_Output[0] = 3; //Pattern
               flg_Table_Output[1] = 3; //Charge
               flg_Table_Output[2] = 3; //Nodes
               flg_Table_Output[3] = 3; //Truth
               flg_Table_Output[4] = 3; //RC
               flg_Table_Output[5] = 3; //Vision

               Node_Network.set_Charging_Leg(0, 1);
               Node_Network.set_Charging_Leg(1, 0);
          }
          //Chrono
          if (p_Type == 2)
          {
               flg_Table_Output[0] = 2; //Pattern
               flg_Table_Output[1] = 2; //Charge
               flg_Table_Output[2] = 2; //Nodes
               flg_Table_Output[3] = 2; //Truth
               flg_Table_Output[4] = 2; //RC
               flg_Table_Output[5] = 2; //Vision

               Node_Network.set_Charging_Leg(0, 1);
               Node_Network.set_Charging_Leg(1, 0);
          }

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

          Node_Network.set_Output_Tables(Output_Table_Pattern, Output_Table_Charge);
          Node_Network.set_Name(Construct_Name + "_Nodes");
          Node_Network.set_CID(U_CID);
          Node_Network.set_Input_Type(Construct_Input_Type);
          
          Threads = new thread*[Central_Core_Count];
          for (int cou_T=0;cou_T<Central_Core_Count;cou_T++)
          {
               Threads[cou_T] = NULL;
          }
          
          tmp_Node = new c_Charging_Buffer_Linked_Node_1D*[Central_Core_Count];
          
          for (int cou_T=0;cou_T<Central_Core_Count;cou_T++)
          {
               tmp_Node[cou_T] = NULL;
          }

          //Open the benchmarking file.
          string tmp_File = "BenchMarking\\" + Construct_Name + "_BenchMark.dat";
          BenchMark.open(tmp_File.c_str());
          BenchMark << "\n\n Begin Test";
     }

     ~c_Base_Construct_1D()
     {
          BenchMark.close();

          for (int cou_Index=0;cou_Index<CAN_Count;cou_Index++)
          {
               delete CAN[cou_Index];
               CAN[cou_Index] = NULL;
          }

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

     //Returns a reference to the node network.
     c_Base_Node_Network_1D * get_Node_Network_Reference()
     {
          return &Node_Network;
     }

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

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(5, 0, "Charging_Legs[2]");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_B(5, 1, Node_Network.Charging_Legs[0]);
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_B(5, 2, Node_Network.Charging_Legs[1]);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(6, 0, "Query_Cap");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(6, 1, Query_Cap);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(7, 0, "flg_RC_Average");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(7, 1, flg_RC_Average);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(8, 0, "Charging Tier");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(8, 1, Charging_Tier);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(9, 0, "Tolerance");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(9, 1, Tolerance);


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



          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(16, 0, "Building_Method");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_S(16, 1, Building_Method);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(17, 0, "Eval_Method");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_S(17, 1, Eval_Method);

     }

     //Updates the internal table from the internal values.
     void update_Internal_Table()
     {
          Internal_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(0, 0, "CAN Count");
          Internal_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(0, 1, CAN_Count);

          Internal_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(1, 0, "Current_Highest_Tier");
          Internal_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(1, 1, Current_Highest_Tier);

          if (Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(7, 1, 0))
          {
               RC_Average = Node_Network.get_RCA();
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
          Node_Network.Charging_Legs[0] = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_B(5, 1, 0);
          Node_Network.Charging_Legs[1] = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_B(5, 2, 0);
          Query_Cap = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(6, 1, 0);
          flg_RC_Average = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(7, 1, 0);
          Charging_Tier = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(8, 1, 0);
          Tolerance = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(9, 1, 0);

          /*
          cout << "\n\t\t CID->" << CID;
          cout << "\n\t\t Filter_Charge->" << Filter_Charge;
          cout << "\n\t\t Base_Charge->" << Base_Charge;
          cout << "\n\t\t Charge_Precision->" << Charge_Precision;
          cout << "\n\t\t Node_Network.Charging_Legs[0]->" << Node_Network.Charging_Legs[0];
          cout << "\n\t\t Node_Network.Charging_Legs[1]->" << Node_Network.Charging_Legs[1];
          cout << "\n\t\t Query_Cap->" << Query_Cap;
          cout << "\n\t\t flg_RC_Average->" << flg_RC_Average;
          cout << "\n\t\t Charging_Tier->" << Charging_Tier;
          cout << "\n\t\t Tolerance->" << Tolerance;
          */
          
          flg_Table_Output[0] = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(10, 1, 0);
          flg_Table_Output[1] = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(11, 1, 0);
          flg_Table_Output[2] = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(12, 1, 0);
          flg_Table_Output[3] = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(13, 1, 0);
          flg_Table_Output[4] = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(14, 1, 0);
          flg_Table_Output[5] = Settings_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(15, 1, 0);

           Settings_Table->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(16, 1, &Building_Method);
           Settings_Table->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(17, 1, &Eval_Method);

     }

     //--      CONSTRUCT PROPERTIES MANIPULATION

     //Changes all settings in one go.
     void change_Settings(string p_Construct_Name, string p_Assembly_Name, int p_Construct_Input_Type, int p_Base_Charge, float p_Filter_Charge, int p_Charge_Precision, int p_Query_Cap, int p_Charging_Tier, int p_Tolerance)
     {
          ostr(0, 5, "\n\t     change_Settings() in ->");
          cout << this;

          set_Construct_Name(p_Construct_Name);
          set_Assembly_Name(p_Assembly_Name);
          set_Input_Type(p_Construct_Input_Type);

          //Set the settings.
          Base_Charge = p_Base_Charge;
          Filter_Charge = p_Filter_Charge;
          Charge_Precision = p_Charge_Precision;
          Query_Cap = p_Query_Cap;
          Charging_Tier = p_Charging_Tier;
          Tolerance = p_Tolerance;

          //Generic
          if (Construct_Input_Type == 0)
          {
               flg_Table_Output[0] = 3; //Pattern
               flg_Table_Output[1] = 3; //Charge
               flg_Table_Output[2] = 3; //Nodes
               flg_Table_Output[3] = 3; //Truth
               flg_Table_Output[4] = 3; //RC
               flg_Table_Output[5] = 3; //Vision
          }
          //Raw
          if (Construct_Input_Type == 1)
          {
               flg_Table_Output[0] = 3; //Pattern
               flg_Table_Output[1] = 3; //Charge
               flg_Table_Output[2] = 3; //Nodes
               flg_Table_Output[3] = 3; //Truth
               flg_Table_Output[4] = 3; //RC
               flg_Table_Output[5] = 3; //Vision

               Node_Network.set_Charging_Leg(0, 1);
               Node_Network.set_Charging_Leg(0, 0);
          }
          //Chrono
          if (Construct_Input_Type == 2)
          {
               flg_Table_Output[0] = 2; //Pattern
               flg_Table_Output[1] = 2; //Charge
               flg_Table_Output[2] = 2; //Nodes
               flg_Table_Output[3] = 2; //Truth
               flg_Table_Output[4] = 2; //RC
               flg_Table_Output[5] = 2; //Vision

               Node_Network.set_Charging_Leg(0, 1);
               Node_Network.set_Charging_Leg(0, 0);
          }

          update_Internal_Table();
          update_Settings_Table();
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

     //Gets the universal CID of the construct.
     int get_Construct_U_CID()
     {
          return U_CID;
     }

     //Gets the ID of the construct.
     int get_Construct_ID()
     {
          return CID;
     }

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

     //Sets the input type of the construct.
     int set_Input_Type(int p_Type)
     {
          Construct_Input_Type = p_Type;
          Node_Network.set_Input_Type(Construct_Input_Type);
          return Construct_Input_Type;
     }

     //Gets the construct input type.
     int get_Input_Type()
     {
          return Construct_Input_Type;
     }

     //--      CONSTRUCT INPUT EVALUATING AND BUILDING


     void Eval()
     {
          if (Eval_Method == "full")       { eval_Input_Table(0); return; }
          if (Eval_Method == "full_RC")    { eval_Input_Table(1); return; }
          if (Eval_Method == "query")      { eval_Input_Table(2); return; }
          if (Eval_Method == "full_row")   { eval_Input_Table(3); return; }
          if (Eval_Method == "full_RC_row"){ eval_Input_Table(4); return; }
     }

     void Build()
     {

          if (Building_Method == "full")       { build_Input_Table(0); return; }
          if (Building_Method == "full_RC")    { build_Input_Table(1); return; }
          if (Building_Method == "query")      { build_Input_Table(2); return; }
          if (Building_Method == "full_row")   { build_Input_Table(3); return; }
          if (Building_Method == "full_RC_row"){ build_Input_Table(4); return; }


     }

     //Builds and evaluates the given input index.
     void build_Given_Input_Row(int p_Row, int p_Building_Method)
     {
          //cout << "\n\t void build_Given_Input_Row(int " << p_Row << ", int p_Building_Method = 0)";
          if (p_Row >= Input_Table->get_Row_Count()){ return; }

          //If the building method is for reading the row as one input then jump to that function.
          if (p_Building_Method == 3 || p_Building_Method == 4){ build_Given_Input_Row_As_One(p_Row, p_Building_Method); return; }

          //The number of cells in the current row.
          int tmp_Cell_Count = Input_Table->get_Row_Cell_Count(p_Row);

          //Setup a CAN for each cell in the current row.
          expand_CAN(tmp_Cell_Count);

          //For every cell in the row take its data and submit it to the correct CAN for eval.
          for (int cou_Cell=0;cou_Cell<tmp_Cell_Count;cou_Cell++)
          {
               submit_Input_Set_To_CAN(cou_Cell, Input_Table->get_Data_Chunk_From_Given_Cell_In_Given_Row(p_Row, cou_Cell), Input_Table->get_Data_Depth_For_Given_Cell_In_Given_Row(p_Row, cou_Cell));
          }

          //Build all the CANs with the given method.
          build_Every_CAN(p_Building_Method);
     }

     //Builds the given row reading in the whole rows data as one entry.
     //This is used mostrly for building treetops for chrono.
     //Is important to note it only reads the first bit of the cell.
     void build_Given_Input_Row_As_One(int p_Row, int p_Building_Method)
     {
          //cout << "\n\n\t build_Given_Input_Row_As_One(p_Row " << p_Row << " , p_Building_Method " << p_Building_Method << ")";

          //The number of cells to read in.
          int tmp_Data_Depth = Input_Table->get_Row_Cell_Count(p_Row);

          //cout << "\n\t\t tmp_Data_Depth->" << tmp_Data_Depth;

          //The temporary data set to use.
          u_Data * tmp_Data = new u_Data[tmp_Data_Depth];
          u_Data * tmp_CID_Data = new u_Data[tmp_Data_Depth];

          //Collect the cells into a data chunk.
          for (int cou_Index=0;cou_Index<tmp_Data_Depth;cou_Index++)
          {
               tmp_Data[cou_Index] = Input_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row(p_Row, cou_Index, 0);
               tmp_CID_Data[cou_Index] = Input_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row(p_Row, cou_Index, 1);
               //cout << "tmp_Data[" << cou_Index << "]->" << tmp_Data[cou_Index].NR;
          }

          //Expand the CAN man.
          expand_CAN(1);

          //Submit it to the CAN.
          submit_Input_Set_To_CAN(0, tmp_Data, tmp_Data_Depth);
          submit_Input_Set_CID_To_CAN(0, tmp_Data, tmp_Data_Depth);

          if (p_Building_Method == 3){ build_Every_CAN(0); }
          if (p_Building_Method == 4){ build_Every_CAN(1); }

          //No memory leaks wanted.
          delete [] tmp_Data;


     }


          //Builds and evaluates the given input index.
     void build_Given_Input_Row_BM(int p_Row, int p_Building_Method)
     {
          //cout << "\n\t void build_Given_Input_Row(int " << p_Row << ", int p_Building_Method = 0)";
          if (p_Row >= Input_Table->get_Row_Count()){ return; }

          //If the building method is for reading the row as one input then jump to that function.
          if (p_Building_Method == 3 || p_Building_Method == 4){ build_Given_Input_Row_As_One_BM(p_Row, p_Building_Method); return; }

          //The number of cells in the current row.
          int tmp_Cell_Count = Input_Table->get_Row_Cell_Count(p_Row);

          //Setup a CAN for each cell in the current row.
          expand_CAN(tmp_Cell_Count);

          //For every cell in the row take its data and submit it to the correct CAN for eval.
          for (int cou_Cell=0;cou_Cell<tmp_Cell_Count;cou_Cell++)
          {
               submit_Input_Set_To_CAN(cou_Cell, Input_Table->get_Data_Chunk_From_Given_Cell_In_Given_Row(p_Row, cou_Cell), Input_Table->get_Data_Depth_For_Given_Cell_In_Given_Row(p_Row, cou_Cell));
          }

          //Build all the CANs with the given method.
          build_Every_CAN_BM(p_Building_Method);
     }

     //Builds the given row reading in the whole rows data as one entry.
     //This is used mostrly for building treetops for chrono.
     //Is important to note it only reads the first bit of the cell.
     void build_Given_Input_Row_As_One_BM(int p_Row, int p_Building_Method)
     {
          //cout << "\n\n\t build_Given_Input_Row_As_One(p_Row " << p_Row << " , p_Building_Method " << p_Building_Method << ")";

          //The number of cells to read in.
          int tmp_Data_Depth = Input_Table->get_Row_Cell_Count(p_Row);

          //cout << "\n\t\t tmp_Data_Depth->" << tmp_Data_Depth;

          //The temporary data set to use.
          u_Data * tmp_Data = new u_Data[tmp_Data_Depth];
          u_Data * tmp_CID_Data = new u_Data[tmp_Data_Depth];

          //Collect the cells into a data chunk.
          for (int cou_Index=0;cou_Index<tmp_Data_Depth;cou_Index++)
          {
               tmp_Data[cou_Index] = Input_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row(p_Row, cou_Index, 0);
               tmp_CID_Data[cou_Index] = Input_Table->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row(p_Row, cou_Index, 1);
               //cout << "tmp_Data[" << cou_Index << "]->" << tmp_Data[cou_Index].NR;
          }

          //Expand the CAN man.
          expand_CAN(1);

          //Submit it to the CAN.
          submit_Input_Set_To_CAN(0, tmp_Data, tmp_Data_Depth);
          submit_Input_Set_CID_To_CAN(0, tmp_Data, tmp_Data_Depth);

          if (p_Building_Method == 3){ build_Every_CAN_BM(0); }
          if (p_Building_Method == 4){ build_Every_CAN_BM(1); }

          //No memory leaks wanted.
          delete [] tmp_Data;


     }


     //Charges given input row.
     void eval_Given_Input_Row(int p_Row, int p_Building_Method, int p_Charging_Method = 0)
     {
          if (p_Row >= Input_Table->get_Row_Count()){ return; }

          //Charge the CAN.
          charge(p_Charging_Method, Charging_Tier);

          //Discharges the Buffers.
          discharge_Torch_Nodes(2, p_Row);
     }


     //Evals all the inputs.
     void eval_Input_Table(int p_Building_Method, int p_Charging_Method = 0)
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

          //Eval all the input rows.
          for (int cou_Index=0;cou_Index<tmp_Row_Count;cou_Index++)
          {
               build_Given_Input_Row(cou_Index, p_Building_Method);
               eval_Given_Input_Row(cou_Index, p_Building_Method, p_Charging_Method);
          }

          update_Internal_Table();
     }

     //Builds an input table with no eval.
     void build_Input_Table(int p_Building_Method, int p_Charging_Method = 0)
     {

          //The number of cells in the current row.
          int tmp_Row_Count = Input_Table->get_Row_Count();

          //Neutralizes all the output tables.
          Treetop_Table->reset_Table();

          //Eval all the input rows.
          for (int cou_Index=0;cou_Index<tmp_Row_Count;cou_Index++)
          {
               build_Given_Input_Row(cou_Index, p_Building_Method);
          }

          update_Internal_Table();

          //Node_Network.output_State_Tree();
     }

     //Builds an input table with no eval.
     void build_Input_Table_BM(int p_Building_Method, int p_Charging_Method = 0)
     {
          BenchMark.flush();

          clock_t tmp_Timer = clock();
          //The number of cells in the current row.
          int tmp_Row_Count = Input_Table->get_Row_Count();

          //Neutralizes all the output tables.
          Treetop_Table->reset_Table();

          tmp_Timer = clock() - tmp_Timer;

          //cout << "\n Prepping To Build Rows->" << tmp_Timer;
          BenchMark << "\n\n\n Prepping To Build Rows->" << tmp_Timer;

          BenchMark <<  "\n  #T->" << Node_Network.Number_Of_Tiers << " #N->" << Node_Network.get_Node_Count_Total();

          //Eval all the input rows.
          for (int cou_Index=0;cou_Index<tmp_Row_Count;cou_Index++)
          {
               BenchMark << "\n";

               tmp_Timer = clock();

               build_Given_Input_Row_BM(cou_Index, p_Building_Method);

               tmp_Timer = clock() - tmp_Timer;
               //cout << "\n Row [" << cou_Index << "] Built In " << tmp_Timer << " Cycles";
               BenchMark << "\n Row [" << cou_Index << "] Built In " << tmp_Timer << " Cycles";
          }

          update_Internal_Table();

          //Node_Network.output_State_Tree();
     }


     //==--     FOREIGN OUTPUT TABLE EVALUATING

     //THe foreign evaluation is used for evauating the results of evaluations of lower construct. For example, charging a chrono construct
     //using the output tables of the raw construct. When it comes to actually building the construct in the upper tier construct the Treetop_Table
     //is read into the output allowing for column shifting. That being said, I will probably allow for the regular building methods here eventually
     //as they could be usefull in unforeseen circumstances.


     //Evals the nodes in a given output table, one output table for the NIDs, the other for their charges.
     void eval_Foreign_Output_Table(c_Table_1D * p_Output_Table_Nodes, c_Table_1D * p_Output_Table_Charges)
     {


          //The number of cells in the current row.
          int tmp_Row_Count = p_Output_Table_Nodes->get_Row_Count();

          //cout << "\n\t eval_Foreign_Output_Table Row_Count->" << tmp_Row_Count;

          //Neutralizes all the output tables.
          Output_Table_Charge->reset_Table();
          Output_Table_Pattern->reset_Table();
          Output_Table_Nodes->reset_Table();
          Output_Table_Truth->reset_Table();
          Output_Table_RC->reset_Table();
          Output_Table_Vision->reset_Table();

          //Eval all the input rows.
          for (int cou_Index=0;cou_Index<tmp_Row_Count;cou_Index++)
          {
               build_Given_Foreign_Output_Row_As_One(cou_Index, p_Output_Table_Nodes, p_Output_Table_Charges);
               eval_Given_Foreign_Output_Row(cou_Index);
          }

          update_Internal_Table();


          //output_Construct_Internals();

          //output_Every_CAN();
          //system("pause > null");
     }


     //Builds the given row reading in the whole rows data as one entry.
     //This is used mostrly for building treetops for chrono.
     //Is important to note it only reads the first bit of the cell.
     void build_Given_Foreign_Output_Row_As_One(int p_Row, c_Table_1D * p_Output_Table_Nodes, c_Table_1D * p_Output_Table_Charges)
     {
          //cout << "\n\n\t build_Given_Input_Row_As_One(p_Row " << p_Row << " , p_Building_Method " << p_Building_Method << ")";

          //The number of cells to read in.
          int tmp_Data_Depth = p_Output_Table_Nodes->get_Row_Cell_Count(p_Row);

          //cout << "\n\t\t tmp_Data_Depth->" << tmp_Data_Depth;

          //The temporary data set to use.
          u_Data * tmp_Data = new u_Data[tmp_Data_Depth];

          //The temporary charging set to use.
          u_Data * tmp_Data_Charge = new u_Data[tmp_Data_Depth];

          //Collect the cells into a data chunk.
          for (int cou_Index=0;cou_Index<tmp_Data_Depth;cou_Index++)
          {
               tmp_Data[cou_Index] = p_Output_Table_Nodes->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row(p_Row, cou_Index, 0);
               tmp_Data_Charge[cou_Index] = p_Output_Table_Charges->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row(p_Row, cou_Index, 0);
               //cout << "tmp_Data[" << cou_Index << "]->" << tmp_Data[cou_Index].NR;
          }

          //Expand the CAN man.
          expand_CAN(1);

          //Submit it to the CAN.
          submit_Input_Set_To_CAN(0, tmp_Data, tmp_Data_Depth);
          submit_Input_Set_Charges_To_CAN(0, tmp_Data_Charge, tmp_Data_Depth);


          //Build the query.
          build_Every_CAN(2);

          //CAN[0]->output_CAN();

          //No memory leaks wanted.
          delete [] tmp_Data;
          delete [] tmp_Data_Charge;

     }


     //Charges given input row.
     void eval_Given_Foreign_Output_Row(int p_Row)
     {
          //Neutralize the buffers.
          neutralize();

          //Charge the CAN.
          charge_Foreign_Output_Every_CAN_Given_Tier(0);

          //Charge the rest of the construct normally.
          charge_No_Reset(0, 1);

          //Discharges the Buffers.
          discharge_Torch_Nodes(2, p_Row);


     }


     //Charges a given CAN at a given tier.
     void charge_Foreign_Output_Given_CAN_Given_Tier(int p_CAN, int p_Tier)
     {
          CAN[p_CAN]->charge_Tier_With_Charge_Set(p_Tier);
     }

     //Charges every CAN at a given tier.
     void charge_Foreign_Output_Every_CAN_Given_Tier(int p_Tier)
     {
          for (int cou_CAN=0;cou_CAN<CAN_Count;cou_CAN++)
          {
               //cout << "\n\t\t charging CAN[" << cou_CAN << "]";
               charge_Foreign_Output_Given_CAN_Given_Tier(cou_CAN, 0);
          }
     }

     //==--    CAN HANDLING

     //Reads a submission into a given CAN.
     void submit_Input_Set_To_CAN(int p_CAN, u_Data p_Input_Set[], int p_Input_Length)
     {
          CAN[p_CAN]->submit_Input_Set(p_Input_Set, p_Input_Length);
     }

     //Reads a submission into a given CAN.
     void submit_Input_Set_CID_To_CAN(int p_CAN, u_Data p_Input_Set_CID[], int p_Input_Length)
     {
          CAN[p_CAN]->submit_Input_Set_CID(p_Input_Set_CID, p_Input_Length);
     }

     //Reads a submissions charges into a given CAN.
     void submit_Input_Set_Charges_To_CAN(int p_CAN, u_Data p_Input_Set[], int p_Input_Length)
     {
          CAN[p_CAN]->submit_Input_Set_Charges(p_Input_Set, p_Input_Length);
     }


     //Submit an input to the input table.
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -"INPUT"
     //   0            1          2          3       4      5
     void push_Input(int p_Row, string p_Input)
     {
          Input_Table->push_string(p_Row, p_Input);
     }

     //Submit an input to the input table.
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -CELL -"INPUT"
     //   0            1          2          3       4    5       6
     void set_Input(int p_Row, int p_Cell, string p_Input)
     {
          Input_Table->set_string(p_Row, p_Cell, p_Input);
     }

     //--     BUILDING CAN FUNCTIONS

     //Builds a given CAN.
     void build_CAN_Full(int p_CAN)
     {
          CAN[p_CAN]->build_Full();

          //Gets the current highest tier to use in the charge_Every_Tier(int p_Tier) Function.
          if (CAN[p_CAN]->Number_Of_Tiers > Current_Highest_Tier){ Current_Highest_Tier = CAN[p_CAN]->Number_Of_Tiers; }
     }

     //Builds all current CANs
     void build_Every_CAN_Full()
     {
          for (int cou_CAN=0;cou_CAN<CAN_Count;cou_CAN++)
          {
               build_CAN_Full(cou_CAN);
          }
     }

     //Builds a given CAN.
     void build_CAN_Full_BM(int p_CAN)
     {
          //cout << "\n  build_CAN_Full_BM(" << p_CAN << ")";
          BenchMark << "\n  build_CAN_Full_BM(" << p_CAN << ")";

          CAN[p_CAN]->build_Full_BM();

          //Gets the current highest tier to use in the charge_Every_Tier(int p_Tier) Function.
          if (CAN[p_CAN]->Number_Of_Tiers > Current_Highest_Tier){ Current_Highest_Tier = CAN[p_CAN]->Number_Of_Tiers; }
     }

     //Builds all current CANs
     void build_Every_CAN_Full_BM()
     {
          for (int cou_CAN=0;cou_CAN<CAN_Count;cou_CAN++)
          {
               build_CAN_Full_BM(cou_CAN);
          }
     }

     //Builds a given CAN.
     void build_CAN_BM(int p_CAN)
     {
          //cout << "\n  build_CAN_BM(" << p_CAN << ")";
          BenchMark << "\n  build_CAN_BM(" << p_CAN << ")";

          CAN[p_CAN]->build_Full_BM();

          //Gets the current highest tier to use in the charge_Every_Tier(int p_Tier) Function.
          if (CAN[p_CAN]->Number_Of_Tiers > Current_Highest_Tier){ Current_Highest_Tier = CAN[p_CAN]->Number_Of_Tiers; }
     }

     //Builds all current CANs
     void build_Every_CAN_BM()
     {
          for (int cou_CAN=0;cou_CAN<CAN_Count;cou_CAN++)
          {
               build_CAN_BM(cou_CAN);
          }
     }


     //==--     BUILD WITH RC
     //Builds a given CAN FUlly with RC.
     void build_CAN_Full_RC(int p_CAN)
     {
          CAN[p_CAN]->build_Full_RC();

          //Gets the current highest tier to use in the charge_Every_Tier(int p_Tier) Function.
          if (CAN[p_CAN]->Number_Of_Tiers > Current_Highest_Tier){ Current_Highest_Tier = CAN[p_CAN]->Number_Of_Tiers; }
     }

     //Builds all current CANs
     void build_Every_CAN_Full_RC()
     {
          for (int cou_CAN=0;cou_CAN<CAN_Count;cou_CAN++)
          {
               build_CAN_Full_RC(cou_CAN);
          }
     }

     //==--     BUILD WITH RC
     //Builds a given CAN FUlly with RC.
     void build_CAN_Full_RC_BM(int p_CAN)
     {
          CAN[p_CAN]->build_Full_RC_BM();

          //Gets the current highest tier to use in the charge_Every_Tier(int p_Tier) Function.
          if (CAN[p_CAN]->Number_Of_Tiers > Current_Highest_Tier){ Current_Highest_Tier = CAN[p_CAN]->Number_Of_Tiers; }
     }

     //Builds all current CANs
     void build_Every_CAN_Full_RC_BM()
     {
          for (int cou_CAN=0;cou_CAN<CAN_Count;cou_CAN++)
          {
               build_CAN_Full_RC_BM(cou_CAN);
          }
     }


     //==--     BUILD FOR QUERY
     //Builds a given CAN FUlly for query?
     void build_CAN_Query(int p_CAN)
     {
          //Treetop_Table->output_Table();
          CAN[p_CAN]->build_Query();

          //Treetop_Table->output_Table();
          //system("pause > NULL");
          //Gets the current highest tier to use in the charge_Every_Tier(int p_Tier) Function.
          if (CAN[p_CAN]->Number_Of_Tiers > Current_Highest_Tier){ Current_Highest_Tier = CAN[p_CAN]->Number_Of_Tiers; }
     }

     //Builds all current CANs for query.
     void build_Every_CAN_Query()
     {
          for (int cou_CAN=0;cou_CAN<CAN_Count;cou_CAN++)
          {
               build_CAN_Query(cou_CAN);
          }
     }


     //Builds all current CANs
     void build_Every_CAN(int p_Building_Method = 0)
     {
          if (p_Building_Method == 0){ build_Every_CAN_Full(); return; }
          if (p_Building_Method == 1){ build_Every_CAN_Full_RC(); return; }
          if (p_Building_Method == 2){ build_Every_CAN_Query(); return; }
     }


     //Builds all current CANs
     void build_Every_CAN_BM(int p_Building_Method = 0)
     {
          if (p_Building_Method == 0){ build_Every_CAN_Full_BM(); return; }
          if (p_Building_Method == 1){ build_Every_CAN_Full_RC_BM(); return; }
          if (p_Building_Method == 2){ build_Every_CAN_Query(); return; }
     }

     //--     CHARGING FUNCTIONS

     //Charges a given CAN at a given tier.
     void charge_Given_CAN_Given_Tier(int p_CAN, int p_Tier, int Base_Charge)
     {
          //cout << "\n  charge_Given_CAN_Given_Tier(" << p_CAN << ", " << p_Tier << ", " << Base_Charge << ")";
          CAN[p_CAN]->charge_Tier(p_Tier, Base_Charge);
     }

     //Charges every CAN at a given tier.
     void charge_Every_CAN_Given_Tier(int p_Tier, int Base_Charge)
     {
          //cout << "\n  charge_Every_CAN_Given_Tier(" << p_Tier << ", " << Base_Charge << ")";
          for (int cou_CAN=0;cou_CAN<CAN_Count;cou_CAN++)
          {
               //cout << "\n\t\t charging CAN[" << cou_CAN << "]";
               charge_Given_CAN_Given_Tier(cou_CAN, p_Tier, Base_Charge);
          }
     }

     //Charges the nodes in the Charging_Buffer output.
     void charge_Charging_Buffer_Output__OLD()
     {
          //output_Charging_Buffer();

          //cout << "\n\t charging_Charging_Buffer_Output()";

          for (int cou_NID=0;cou_NID<Charging_Buffer.Output_Length;cou_NID++)
          {
               //cout << " [" << Charging_Buffer.NID_Output[cou_NID];
               //ochr(0, 13, ']');
               //Node_Network.initiate_Backpropagation(Charging_Buffer.NID_Output[cou_NID]);

               //If the current node is NULL then continue.
               if (Charging_Buffer.NID_Output[cou_NID] == NULL){ continue; }

               //cout << " AX(1)";
               //oint(0, 8, Charging_Buffer.NID_Output[cou_NID]->get_Axon_Count(1));
               //cout << " AX(0)";
               //oint(0, 7, Charging_Buffer.NID_Output[cou_NID]->get_Axon_Count(0));

               //Charge the axons for the current node (LEFT LEG).
               if (Node_Network.Charging_Legs[0])
               {
                    for (int cou_A=0;cou_A<Charging_Buffer.NID_Output[cou_NID]->get_Axon_Count(0);cou_A++)
                    {
                         Charging_Buffer.submit(Charging_Buffer.NID_Output[cou_NID]->Axons[0] [cou_A], Charging_Buffer.Charge_Output[cou_NID]);
                         //Node_Network.initiate_Backpropagation_Output_Only(Charging_Buffer.NID_Output[cou_NID]->Axons[0] [cou_A]);
                    }
               }

               //Charge the axons for the current node (RIGHT LEG).
               if (Node_Network.Charging_Legs[1])
               {
                    for (int cou_A=0;cou_A<Charging_Buffer.NID_Output[cou_NID]->get_Axon_Count(1);cou_A++)
                    {
                         Charging_Buffer.submit(Charging_Buffer.NID_Output[cou_NID]->Axons[1] [cou_A], Charging_Buffer.Charge_Output[cou_NID]);
                         //Node_Network.initiate_Backpropagation_Output_Only(Charging_Buffer.NID_Output[cou_NID]->Axons[1] [cou_A]);
                    }
               }

               if (!Charging_Buffer.NID_Output[cou_NID]->get_Axon_Count(0) && !Charging_Buffer.NID_Output[cou_NID]->get_Axon_Count(1))
               {
                    //Node_Network.initiate_Backpropagation_Output_Only(Charging_Buffer.NID_Output[cou_NID]);
                    Torch_Buffer.submit(Charging_Buffer.NID_Output[cou_NID], Charging_Buffer.Charge_Output[cou_NID]);
               }
          }
     }
     
     
     //Charge Core Buffer dummy for thread testing.
     void charge_Charging_Buffer_Stack_D(int p_Stack)
     {
          Sleep(10);
     }
          
     //Charge Core Buffer.
     void charge_Charging_Buffer_Stack(int p_Stack)
     {
          
          
          //Gather the root node, it actually skips the first node.
          //cout << "\n Charging_Buffer.Linked_Output[" << p_Stack << "] Len->" << Charging_Buffer.Linked_Output_Length[p_Stack];
          /*
          Mutex.lock();
          ostr(0, (p_Stack + 4), "\n Charging_Buffer.Linked_Output[");
          oint(0, (p_Stack + 4), p_Stack);
          ostr(0, (p_Stack + 4), "] Len->");
          oint(0, (p_Stack + 4), Charging_Buffer.Linked_Output_Length[p_Stack]);
          Mutex.unlock();
          */
          
          if (Charging_Buffer.Linked_Output_Length[p_Stack] == 0)
          { 
               /*
               Mutex.lock();
               ostr(0, (p_Stack + 4), " Stack Length == 0... Returning...");
               Mutex.unlock();
               */
               return; 
          }
          else
          {
               tmp_Node[p_Stack] = &Charging_Buffer.Linked_Output[p_Stack];
               
               //Until the linked node has a pointer to NULL for the next in line loop through charging the NID.
               while (tmp_Node[p_Stack]->NID != NULL)
               {
                    /*
                    Mutex.lock();
                    ostr(0, (p_Stack + 4), "{{ ");
                    Node_Network.initiate_Backpropagation_Output_Only(tmp_Node[p_Stack]->NID);
                    cout << " ";
                    oint(0, (p_Stack + 4), tmp_Node[p_Stack]->Charge);
                    cout << " ";
                    Mutex.unlock();
                    */
                    
                    //Charge the axons for the current node (LEFT LEG).
                    if (Node_Network.Charging_Legs[0])
                    {
                         for (int cou_A=0;cou_A<tmp_Node[p_Stack]->NID->get_Axon_Count(0);cou_A++)
                         {
                              Charging_Buffer.submit(tmp_Node[p_Stack]->NID->Axons[0] [cou_A], tmp_Node[p_Stack]->Charge, p_Stack);
                              Node_Network.initiate_Backpropagation_Output_Only(tmp_Node[p_Stack]->NID->Axons[0] [cou_A]);
                         }
                    }

                    //Charge the axons for the current node (RIGHT LEG).
                    if (Node_Network.Charging_Legs[1])
                    {
                         for (int cou_A=0;cou_A<tmp_Node[p_Stack]->NID->get_Axon_Count(1);cou_A++)
                         {
                              Charging_Buffer.submit(tmp_Node[p_Stack]->NID->Axons[1] [cou_A], tmp_Node[p_Stack]->Charge, p_Stack);
                              Node_Network.initiate_Backpropagation_Output_Only(tmp_Node[p_Stack]->NID->Axons[1] [cou_A]);
                         }
                    }
                    /*
                    Mutex.lock();
                    ostr(0, (p_Stack + 4), "if (!");
                    oint(0, (p_Stack + 4), tmp_Node[p_Stack]->NID->get_Axon_Count(0));
                    ostr(0, (p_Stack + 4), " && !");
                    oint(0, (p_Stack + 4), tmp_Node[p_Stack]->NID->get_Axon_Count(1));
                    ostr(0, (p_Stack + 4), ") ->");
                    oint(0, (p_Stack + 4), (!tmp_Node[p_Stack]->NID->get_Axon_Count(0) && !tmp_Node[p_Stack]->NID->get_Axon_Count(1)));
                    Mutex.unlock();
                    */
                    if (!tmp_Node[p_Stack]->NID->get_Axon_Count(0) && !tmp_Node[p_Stack]->NID->get_Axon_Count(1))
                    {
                         Node_Network.initiate_Backpropagation_Output_Only(tmp_Node[p_Stack]->NID);
                         Torch_Buffer.submit(tmp_Node[p_Stack]->NID, tmp_Node[p_Stack]->Charge);
                    }
                    /*
                    Mutex.lock();
                    ostr(0, (p_Stack + 4), "}}");
                    Mutex.unlock();
                    */
                    tmp_Node[p_Stack] = tmp_Node[p_Stack]->Next;
               }
               
          }
     }
     
     
     //Charges the nodes in the Charging_Buffer output.
     void charge_Charging_Buffer_Output_LL()
     {
          //output_Charging_Buffer();

          Mutex.lock();
          cout << "\n\t charging_Charging_Buffer_Output()";
          Mutex.unlock();
          
          cout << "\n Threads->" << Threads; cout.flush();
          for (int cou_Index=0;cou_Index<Central_Core_Count;cou_Index++)
          {
               cout << "\n\t Threads[" << cou_Index << "]->"; cout.flush();
               cout << Threads[cou_Index]; cout.flush();
               delete Threads[cou_Index];
               Threads[cou_Index] = NULL;
          }
          
          cout << "\n\n";
          cout << "\n\t THIS->" << this;
          cout << "\n\n";
          
          for (int cou_T=0;cou_T<Central_Core_Count;cou_T++)
          {
               Mutex.lock();
               cout << "\n\t cou_T->" << cou_T; cout.flush();
               
               cout << "\n\t B Threads[" << cou_T << "]->" << Threads[cou_T] << " ~>"; cout.flush();
               cout << "<~"; cout.flush();
                    
               Mutex.unlock();
               Threads[cou_T] = new std::thread(&c_Base_Construct_1D::charge_Charging_Buffer_Stack, this, cou_T);
               
               Mutex.lock();
               cout << "\n\t A Threads[" << cou_T << "]->" << Threads[cou_T] << " ~>"; cout.flush();
               cout << "<~"; cout.flush();
               Mutex.unlock();
               //charge_Charging_Buffer_Stack(cou_T);
          }
          
          
          
          bool flg_Done_Threads = false;
          
          while (!flg_Done_Threads)
          {
               flg_Done_Threads = true;
               for (int cou_T=0;cou_T<Central_Core_Count;cou_T++)
               {
                    if (Threads[cou_T] == NULL){ continue; }
                    cout << "\n\t Threads[" << cou_T << "]->" << Threads[cou_T] << " ~>"; cout.flush();
                    cout << Threads[cou_T]->joinable();
                    cout << "<~"; cout.flush();
                    if (Threads[cou_T]->joinable())
                    {
                         cout << Threads[cou_T]->joinable(); cout.flush();
                         
                         if (std::this_thread::get_id() == Threads[cou_T]->get_id())
                         {
                              cout << "\n\t DEADLOCK DETECTED!";
                              break;
                         }
                         
                              cout << "\n\t Trying to join()"; cout.flush();
                              Threads[cou_T]->join();
                              cout << "\n\t Join Complete..."; cout.flush();
                              
                         cout << Threads[cou_T]->joinable(); cout.flush();
                         
                         flg_Done_Threads = false;
                    }
               }
          }
          for (int cou_T=0;cou_T<Central_Core_Count;cou_T++)
          {
               cout << "\n\n\n\t All Threads Returned To Main"; cout.flush();
               
               delete Threads[cou_T];
               Threads[cou_T] = NULL;
          }
          
     }

     //Collects the charge buffer nodes.
     void gather_Charging_Buffer()
     {
          //cout << "\n    gather_Charging_Buffer()";
          
          //gather(Filter_Charge) resets the Output.
          Charging_Buffer.gather_LL(Filter_Charge, Charge_Precision);
          Charging_Buffer.reset_Tree();
     }

     //Charges a tier.
     void charge_Tier(int p_Tier)
     {
          //cout << "\n   charge_Tier(" <<  p_Tier << ")";
          //Gather whatever nodes happen to be in the charging buffer.
          //This is done first as the tree is deconstructed when called.
          gather_Charging_Buffer();
          
          cout << "\n\t Charging charge_Charging_Buffer_Output_LL()"; cout.flush();
          
          //Charge the Charging_Buffer.Output.
          charge_Charging_Buffer_Output_LL();
          
          //Charge every CAN.
          charge_Every_CAN_Given_Tier(p_Tier, Base_Charge);


          //For debug.
          //output_Charging_Buffer();
     }

     //Charges all tiers starting at 0.
     void charge_All_Tiers(int p_Tier)
     {
          //cout << "\n  charge_All_Tiers(" << p_Tier << ")";
          /*
          cout << "\n\t   ";
          for (int cou_T=p_Tier;cou_T<Current_Highest_Tier;cou_T++)
          {
               cout << ".";
          }*/

          //cout << "\n\t   ";
          for (int cou_T=p_Tier;cou_T<Current_Highest_Tier;cou_T++)
          {
               
               //ochr(0, cou_T, '^');
               charge_Tier(cou_T);
               //Torch_Buffer.output_Torch_Buffer();
          }
     }

     //Charges all the tiers, when those run out it keeps charging the buffer until no more axons can be followed.
     void charge(int p_Charging_Method = 0, int p_Tier = 0)
     {
          //Neutralize Current charges.
          neutralize();

          cout << "\n Charging_All_Tiers->" << Construct_Name << " #Tiers->" << Current_Highest_Tier << " p_Tier->" << p_Tier;

          //Charge all the tiers starting at the given Tier.
          charge_All_Tiers(p_Tier);

          //cout << "\n\t Discharging Buffer->" << Charging_Buffer.Output_Length;


          //Charge the charging buffer until nothing remains.
          while (Charging_Buffer.Output_Length > 0)
          {
               //cout << "\n\t   " << Charging_Buffer.Output_Length;

               charge_Charging_Buffer_Output_LL();
               //cout << " charge_Charging_Buffer_Output()" << Charging_Buffer.Output_Length;

               //output_Charging_Buffer();
               gather_Charging_Buffer();
               //cout << " gather_Charging_Buffer()" << Charging_Buffer.Output_Length;

          }
          //cout << "   end()" << Charging_Buffer.Output_Length;
          //cout << "\n\n\n";
     }

     //Charges with no reset, used when coupled with the foreign charging.
     void charge_No_Reset(int p_Charging_Method = 0, int p_Tier = 0)
     {
          cout << "\n  charge_No_Reset(" <<  p_Charging_Method << " = 0, " << p_Tier << "= 0)";
          
          //Charge all the tiers starting at the given Tier.
          charge_All_Tiers(p_Tier);

          //Charge the charging buffer until nothing remains.
          while (Charging_Buffer.Output_Length)
          {
               //output_Charging_Buffer();
               gather_Charging_Buffer();
               charge_Charging_Buffer_Output_LL();
          }
     }


     //Initiates a backpropagation in the given node, submits the given cell to gather the pattern in. Used mostly for Chrono gathering.
     virtual void initiate_Backpropagation_Foreign_Node(u_Data p_Data, int p_Row, int p_Charge)
     {
          //Pop the cell in the given row if it is not empty.
          if (!Output_Table_Pattern->row_Is_Empty(p_Row)){ Output_Table_Pattern->pop_Cell(p_Row); }

          //Push the charge.
          Output_Table_Charge->push_int(p_Row, p_Charge);

          //Push the given node onto the node table.
          Output_Table_Nodes->push_reference(p_Row, p_Data.NR);

          //Gather the backpropogated pattern using the current cell in the given row.
          Node_Network.initiate_Backpropagation_From_Given_u_Data_For_Given_Row(p_Data, p_Row);

     }

     //Discharges the torch buffer nodes.
     void discharge_Torch_Nodes(int p_Oflg, int p_Row)
     {
          cout << "\n  discharge_Torch_Nodes(" << p_Oflg<< ", " << p_Row<< ")";
          
          //Gather the nodes in the torch buffer.
          Torch_Buffer.gather(-1);


          //Pop the ouput tables if past the first row.
          if (flg_Table_Output[0] == p_Oflg || flg_Table_Output[0] == 3)
          {
               if (!Output_Table_Pattern->table_Is_Empty())
               {
                    Output_Table_Pattern->pop_Row();
               }
          }
          if (flg_Table_Output[1] == p_Oflg || flg_Table_Output[1] == 3)
          {
               if (!Output_Table_Charge->table_Is_Empty())
               {
                    Output_Table_Charge->pop_Row();
               }
          }
          if (flg_Table_Output[2] == p_Oflg || flg_Table_Output[2] == 3)
          {
               if (!Output_Table_Nodes->table_Is_Empty())
               {
                    Output_Table_Nodes->pop_Row();
               }
          }
          if (flg_Table_Output[3] == p_Oflg || flg_Table_Output[3] == 3)
          {
               if (!Output_Table_Truth->table_Is_Empty())
               {
                    Output_Table_Truth->pop_Row();
               }
          }
          if (flg_Table_Output[4] == p_Oflg || flg_Table_Output[4] == 3)
          {
               //Avoid /0 errors.
               RC_Average = Node_Network.get_RCA();

               if (RC_Average == 0.0){ RC_Average = 1; }

               if (!Output_Table_RC->table_Is_Empty())
               {
                    Output_Table_RC->pop_Row();
               }
          }
          if (flg_Table_Output[5] == p_Oflg || flg_Table_Output[5] == 3)
          {
               if (!Output_Table_Vision->table_Is_Empty())
               {
                    Output_Table_Vision->pop_Row();
               }
          }

          //flg_Table_Output[0] //Pattern
          //flg_Table_Output[1] //Charge
          //flg_Table_Output[2] //Nodes
          //flg_Table_Output[3] //Truth
          //flg_Table_Output[4] //RC
          //flg_Table_Output[5] //Vision

          int tmp_Limit = 0;
          c_Cell_1D tmp_Cell;

          //The RC is pushed as a string. It will be read as a pattern.
          float tmp_RC_Score;

          if (Torch_Buffer.Output_Length > Query_Cap){ tmp_Limit = Query_Cap; } else { tmp_Limit = Torch_Buffer.Output_Length; }

          //Loops through the output initiating a backpropagation as it goes.
          for (int cou_Index=0;cou_Index<tmp_Limit;cou_Index++)
          {
               //Pop the ouput tables if past the first row.
               if (flg_Table_Output[0] == p_Oflg || flg_Table_Output[0] == 3)
               {
                    if (cou_Index)
                    {
                         Output_Table_Pattern->pop_Cell_In_Current_Row();
                    }
                    Node_Network.initiate_Backpropagation(Torch_Buffer.NID_Output[cou_Index]);
               }
               if (flg_Table_Output[1] == p_Oflg || flg_Table_Output[1] == 3){ Output_Table_Charge->push_Data_Bit_Onto_Current_Row_I(Torch_Buffer.Charge_Output[cou_Index]); }
               if (flg_Table_Output[2] == p_Oflg || flg_Table_Output[2] == 3){ Output_Table_Nodes->push_Data_Bit_Onto_Current_Row_NR(Torch_Buffer.NID_Output[cou_Index]); }
               if (flg_Table_Output[3] == p_Oflg || flg_Table_Output[3] == 3)
               {
                    Node_Network.initiate_Backpropagation_Gather_Data(Torch_Buffer.NID_Output[cou_Index], &tmp_Cell);
                    Output_Table_Truth->push_Data_Bit_Onto_Current_Row_F(get_Truth(&tmp_Cell, Input_Table_Truth->get_Cell_Reference(p_Row, 0)));
               }
               if (flg_Table_Output[4] == p_Oflg || flg_Table_Output[4] == 3)
               {
                    tmp_RC_Score = Node_Network.get_RC_Score(Torch_Buffer.NID_Output[cou_Index]) / RC_Average;
                    Output_Table_RC->push_Data_Bit_Onto_Current_Row_F(tmp_RC_Score);
               }
               //Gather the patterns into the vision.
               if (flg_Table_Output[5] == p_Oflg || flg_Table_Output[5] == 3)
               {
                    tmp_RC_Score = Node_Network.get_RC_Score(Torch_Buffer.NID_Output[cou_Index]) / RC_Average;
                    Output_Vision_Tree.submit_Charged_Pattern(Output_Table_Pattern->getc_string(), (Output_Table_Charge->getc_int() * tmp_RC_Score));
               }

          }

          if (flg_Table_Output[5] == p_Oflg || flg_Table_Output[5] == 3)
          {
               Output_Table_Vision->push_Data_Bit_Onto_Current_Row_S(Output_Vision_Tree.get_Strongest_Pattern_S());
          }

          //output_Torch_Buffer();
     }



     //Gathers the other output tables based on the pattern and charge. Used after foreign output nodes have been backpropagated.
     void gather_Output_Tables()
     {

          //Get the RCA is need be.
          if (flg_Table_Output[4] == 3)
          {
               //Avoid /0 errors.
               RC_Average = Node_Network.get_RCA();

               if (RC_Average == 0.0){ RC_Average = 1; }
          }

          //Reset the output tables other than the pattern and charge.
          Output_Table_RC->reset_Table();
          Output_Table_Truth->reset_Table();
          Output_Table_Vision->reset_Table();
          Output_Vision_Tree.reset_Vision();

          //flg_Table_Output[0] //Pattern
          //flg_Table_Output[1] //Charge
          //flg_Table_Output[2] //Nodes
          //flg_Table_Output[3] //Truth
          //flg_Table_Output[4] //RC
          //flg_Table_Output[5] //Vision

          c_Cell_1D tmp_Cell;

          //The RC is pushed as a string. It will be read as a pattern.
          float tmp_RC_Score;

          cout << "\n\n\t Gathering output tables for " << Construct_Name;

          //Loop through all of the rows in the pattern table evaluating each cell in the rows.
          for (int cou_Row=0;cou_Row<Output_Table_Pattern->get_Row_Count();cou_Row++)
          {
               cout << "\n\t\t Row->" << cou_Row;

               for (int cou_Cell=0;cou_Cell<Output_Table_Pattern->get_Row_Cell_Count(cou_Row);cou_Cell++)
               {
                    cout << "\n\t\t\t Cell->" << cou_Cell;

                    //Redpill the node.
                    if (flg_Table_Output[3] > 0)
                    {
                         cout << "\t Truth";
                         Output_Table_Truth->set_float(cou_Row, cou_Cell, get_Truth(Output_Table_Pattern->get_Cell_Reference(cou_Row, cou_Cell), Input_Table_Truth->get_Cell_Reference(cou_Row, 0)));
                    }
                    //Get the score.
                    if (flg_Table_Output[4] > 0)
                    {
                         cout << "\t Score";
                         tmp_RC_Score = Node_Network.get_RC_Score(Output_Table_Nodes->get_data(cou_Row, cou_Cell)) / RC_Average;
                         Output_Table_RC->set_float(cou_Row, cou_Cell, tmp_RC_Score);
                    }
                    //Gather the vision of the future.
                    if (flg_Table_Output[5] > 0)
                    {
                         cout << "\t Vision->" << Output_Table_Pattern->get_string(cou_Row, cou_Cell);
                         tmp_RC_Score = Node_Network.get_RC_Score(Output_Table_Nodes->get_data(cou_Row, cou_Cell)) / RC_Average;
                         Output_Vision_Tree.submit_Charged_Pattern(Output_Table_Pattern->get_string(cou_Row, cou_Cell), (Output_Table_Charge->get_int(cou_Row, cou_Cell) * tmp_RC_Score));
                    }
               }
               if (flg_Table_Output[5] > 0)
               {
                    cout << "\t Whole_Vision->";
                    cout << Output_Vision_Tree.get_Strongest_Pattern_S();
                    Output_Table_Vision->set_string(cou_Row, 0, Output_Vision_Tree.get_Strongest_Pattern_S());
               }
          }

          //output_Tables();
     }

     //Compares the data in two cells and returns a float of the match percentage.
     float get_Truth(void * p_Current_Pattern, void * p_Truth_Pattern)
     {
          int p_Limit = ((c_Cell_1D*) p_Current_Pattern)->get_Data_Depth();
          float p_Score = 0.0;

          //Avoiding bounding errors.
          if (((c_Cell_1D*) p_Truth_Pattern)->get_Data_Depth() < p_Limit){ p_Limit = ((c_Cell_1D*) p_Truth_Pattern)->get_Data_Depth(); }
          if (Tolerance < p_Limit){ p_Limit = Tolerance; }

          for (int cou_Index=0;cou_Index<p_Limit;cou_Index++)
          {
               if (((c_Cell_1D*) p_Current_Pattern)->get_Data_Bit_From_Given_Index_NR(cou_Index) == ((c_Cell_1D*) p_Truth_Pattern)->get_Data_Bit_From_Given_Index_NR(cou_Index))
               {
                    p_Score += 1;
               }
          }
          p_Score = p_Score / float (p_Limit);

          return p_Score;
     }

     //Gathers the RCA from the node network on the given tier.
     float get_RC_Average(int p_Tier)
     {
          return Node_Network.get_RCA(p_Tier);
     }

     //Neutralizes both the charging_Buffer & the torch_Buffer.
     void neutralize()
     {
          Charging_Buffer.reset_Buffer();
          Torch_Buffer.reset_Buffer();
          Output_Vision_Tree.reset_Vision();
     }

     //Sets the state of a charging leg.
     void set_Charging_Leg(int p_Leg, int p_State)
     {
          Node_Network.set_Charging_Leg(p_Leg, p_State);
     }

     //--     ARRAY MANAGMENT FUNCTIONS

     //Expands the CAN array to the given depth destroying any existing CANs.
     void expand_CAN(int p_CAN_Count)
     {
          //Delete the old ones.
          reset_CANS();

          //Set the CAN count.
          CAN_Count = p_CAN_Count;

          //Setup the array.
          CAN = new c_Base_CAN_1D * [p_CAN_Count];

          //Create the individual CANs and pass the node network to them.
          for (int cou_CAN=0;cou_CAN<CAN_Count;cou_CAN++)
          {
               CAN[cou_CAN] = new c_Base_CAN_1D(&Node_Network, &Charging_Buffer, Treetop_Table, &BenchMark);
          }

     }

     //Resets the CAN array.
     void reset_CANS()
     {
          for (int cou_CAN=0;cou_CAN<CAN_Count;cou_CAN++)
          {
               delete CAN[cou_CAN];
               CAN[cou_CAN] = NULL;
          }
          delete [] CAN;
          CAN = NULL;
          CAN_Count = 0;
     }

     //Inflates the State node coordinates array in the node network to the size of the state nodes tier.
     void inflate_State_Coordinates()
     {
          Node_Network.inflate_State_Coordinates();
     }

     //--     OUTPUT FUNCTIONS

     //Outputs a given CAN.
     void output_given_CAN(int p_CAN)
     {
          CAN[p_CAN]->output_CAN();
     }

     //Outputs all the CANs
     void output_Every_CAN()
     {
          for (int cou_CAN=0;cou_CAN<CAN_Count;cou_CAN++)
          {
               cout << "\n";
               output_given_CAN(cou_CAN);
          }
     }

     //Output Node Network.
     void output_Node_Network()
     {
          Node_Network.output_Node_Network();
     }

     //Output Node Network.
     void output_Node_Network_Stats()
     {
          Node_Network.output_Node_Network_Stats();
     }

     //Outputs a Tier in the Node Network.
     void output_Node_Network_Tier(int p_Tier)
     {
          if (p_Tier == -1){ Node_Network.output_Top_Tier(); return; }
          Node_Network.output_Tier(p_Tier);
     }

     //Outputs the charging buffer.
     void output_Charging_Buffer()
     {
          Charging_Buffer.output_Charging_Buffer();
     }

     //Outputs the torch buffer.
     void output_Torch_Buffer()
     {
          Torch_Buffer.gather(-1);
          Torch_Buffer.output_Torch_Buffer();
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
          Tables->output_Table(Output_Table_Vision->get_Table_ID());
          Tables->output_Table(Output_Table_Pattern->get_Table_ID());
     }

     //Outputs the construct internals.
     void output_Construct_Internals()
     {
          output_Charging_Buffer();
          output_Torch_Buffer();
          output_Every_CAN();
          output_Node_Network();
     }

     //Outputs the construct.
     void output_Construct_Full()
     {
          cout << "\n\n\t----------";
          cout << "\n\t Construct->" << Construct_Name << " " << this;
          cout << "\n\t   CAN_Count->" << CAN_Count;
          output_Construct_Tables();
          output_Construct_Internals();
     }

     //Outputs the construct.
     void output_Construct()
     {
          cout << "\n\n\t----------";
          cout << "\n\t Construct->" << Construct_Name << " " << this;
          cout << "\n\t   CAN_Count->" << CAN_Count;
          output_Settings_Table();
     }

     //Gets the stats from the construct in a string that can be echoed to a file.
     string get_Stats()
     {
          return " C.n:" + Construct_Name + " #N:" + int2str(Node_Network.get_Node_Count()) + " #T:" + int2str(Node_Network.Number_Of_Tiers);
     }

     //==--     SAVING AND LOADING

     void Save(string p_Directory)
     {
          //Gathers the directory and creates it if need be.
          //string tmp_MD = "md " + p_Directory;

          //cout << "\n\n\t->" << tmp_MD;
          //system(tmp_MD.c_str());

          //Gathers the file to save the assebmly to.
          ofstream SF;
          string tmp_FN = p_Directory + "\\" + Assembly_Name + "__" + Construct_Name + ".con";
          cout << "\n\t tmp_FN->" << tmp_FN;
          SF.open(tmp_FN.c_str());

          //Save the Construct Information.
          SF << Construct_Name;
          SF << "\n" << Assembly_Name;
          SF << "\n" << Construct_Input_Type;
          SF << "\n" << Construct_Type;

          SF << "\n" << Base_Charge;
          SF << "\n" << Filter_Charge;
          SF << "\n" << Charge_Precision;
          SF << "\n" << Query_Cap;
          SF << "\n" << Charging_Tier;
          SF << "\n" << Tolerance;

          Node_Network.Save(p_Directory, Assembly_Name);

          //Close the save file.
          SF.close();
     }

     void load_Construct(string p_Directory, string p_File)
     {

     }

     void load(string p_Directory, string p_File)
     {
          Node_Network.load(p_Directory, p_File);
     }
};



