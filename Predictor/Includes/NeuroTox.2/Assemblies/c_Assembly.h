//This class allows for the manipulation of an assembly.
//Assemblies are not dimension dependent.

class c_Base_Assembly
{
protected:


public:

     //The name of the assembly.
     string Assembly_Name;

     //The constructs contained in the assembly.
     c_Base_Construct ** Constructs;

     //The current number of constructs.
     int Construct_Count;

     //The assembly ID.
     int AID;

     //The lookup tree for the constructs.
     c_Lookup_Tree Construct_Registry;

     //The index to gather inputs from the gathering wall.
     int Gathering_Input_Table_Index;

     //The assembly type.
     string Assembly_Type;

     //--   TABLES

     //The table handler. Used when generating new tables and allows for the command script to manipulate them.
     c_Table_Handler * Tables;

     //Input table to build the inputs from.
     c_Table_1D * Input_Table;

     //Input table to build the inputs from.
     c_Table_1D * Input_Nodes_Table;

     //Input table to build the inputs from.
     c_Table_1D * Input_Charges_Table;

     //Input table to build the inputs from.
     c_Table_1D * Input_Table_Truth;

     //Holds the tables and rows to draw inputs from.
     c_Table_1D * Input_Table_Hotlinks;

     //Holds the tables and rows to draw inputs from.
     c_Table_1D * Input_Table_Nodes_Hotlinks;

     //Holds the tables and rows to draw inputs from.
     c_Table_1D * Input_Table_Charges_Hotlinks;

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

     //The internal table.
     c_Table_1D * Internal_Table;

     //The setting table for the assembly.
     c_Table_1D * Settings_Table;

     //The table to hold the evaluation order of the constructs, only the raw constructs.
     //Each construct is read in as %Current_Construct% to be used by scripts, therefore, only the Raw constructs need read in.
     c_Table_1D * Construct_Index_Table;

     c_Base_Assembly(c_Table_Handler * p_Tables, string p_Assembly_Name = "Default_Assembly", int p_AID = 0)
     {
          cout << "\n\t  c_Base_Assembly() ~~Constructor";

          Constructs = NULL;
          Construct_Count = 0;

          //Set the construct name.
          Assembly_Name = p_Assembly_Name;

          //Set the type of assembly.
          Assembly_Type = "Basic";

          //Set the table handler.
          Tables = p_Tables;

          //Set the ID of the Assembly.
          AID = p_AID;

          //The input index to draw data from the gathering wall with.
          Gathering_Input_Table_Index = p_AID * 10;

          //Setup the tables.
          Input_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Input");
          Input_Nodes_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Input_Nodes");
          Input_Charges_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Input_Charges");
          Input_Table_Truth = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Input_Truth");
          Input_Table_Hotlinks = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Input_Hotlinks");
          Input_Table_Nodes_Hotlinks = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Input_Nodes_Hotlinks");
          Input_Table_Charges_Hotlinks = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Input_Charges_Hotlinks");

          Output_Table_Pattern = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Output_Pattern");
          Output_Table_Charge = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Output_Charge");
          Output_Table_Nodes = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Output_Nodes");
          Output_Table_Truth = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Output_Truth");
          Output_Table_RC = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Output_RC");

          Internal_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Internal");

          //With the settings table the first index is the name of the setting.
          Settings_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Settings");

          Construct_Index_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Construct_Index");

          //Updates the settings table.
          update_Settings_Table();

          Settings_Table->output_Table();
          Settings_Table->save_Table();

          update_Internal_Table();


     }

     virtual ~c_Base_Assembly()
     {

          //Update the registry.
          Tables->delete_Table(Input_Table->get_Table_ID());
          Tables->delete_Table(Input_Table_Truth->get_Table_ID());
          Tables->delete_Table(Input_Table_Hotlinks->get_Table_ID());

          Tables->delete_Table(Output_Table_Pattern->get_Table_ID());
          Tables->delete_Table(Output_Table_Charge->get_Table_ID());
          Tables->delete_Table(Output_Table_Nodes->get_Table_ID());
          Tables->delete_Table(Output_Table_Truth->get_Table_ID());
          Tables->delete_Table(Output_Table_RC->get_Table_ID());

          Tables->delete_Table(Settings_Table->get_Table_ID());
          Tables->delete_Table(Construct_Index_Table->get_Table_ID());
          Tables->delete_Table(Internal_Table->get_Table_ID());

          //Delete the constructs.
          for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
          {
               delete Constructs[cou_Index];
               Constructs[cou_Index] = NULL;
          }
          delete [] Constructs;
          Constructs = NULL;
     }

     //--     INPUT GATHERING

     //Gathers the input.
     virtual void gather_Inputs()
     {

     }

     //Creates a hotlink to a table with the given TID and Index.
     void set_Hotlink(int p_Table, int p_Index, int p_Assembly_From, int p_Construct_From, int p_Row_From)
     {
          if (!Input_Table_Hotlinks->table_Is_Empty())
          {
               Input_Table_Hotlinks->pop_push_int(p_Table);
               Input_Table_Hotlinks->pushc_int(p_Index);
               Input_Table_Hotlinks->pushc_int(p_Assembly_From);
               Input_Table_Hotlinks->pushc_int(p_Construct_From);
               Input_Table_Hotlinks->pushc_int(p_Row_From);
          }
          else
          {
               Input_Table_Hotlinks->pushc_int(p_Table);
               Input_Table_Hotlinks->pushc_int(p_Index);
               Input_Table_Hotlinks->pushc_int(p_Assembly_From);
               Input_Table_Hotlinks->pushc_int(p_Construct_From);
               Input_Table_Hotlinks->pushc_int(p_Row_From);
          }
     }

     //Creates a hotlink to a table with the given TID and Index.
     void set_Hotlink_Nodes(int p_Table, int p_Index, int p_Assembly_From, int p_Construct_From, int p_Row_From)
     {
          if (!Input_Table_Nodes_Hotlinks->table_Is_Empty())
          {
               Input_Table_Nodes_Hotlinks->pop_push_int(p_Table);
               Input_Table_Nodes_Hotlinks->pushc_int(p_Index);
               Input_Table_Nodes_Hotlinks->pushc_int(p_Assembly_From);
               Input_Table_Nodes_Hotlinks->pushc_int(p_Construct_From);
               Input_Table_Nodes_Hotlinks->pushc_int(p_Row_From);
          }
          else
          {
               Input_Table_Nodes_Hotlinks->pushc_int(p_Table);
               Input_Table_Nodes_Hotlinks->pushc_int(p_Index);
               Input_Table_Nodes_Hotlinks->pushc_int(p_Assembly_From);
               Input_Table_Nodes_Hotlinks->pushc_int(p_Construct_From);
               Input_Table_Nodes_Hotlinks->pushc_int(p_Row_From);
          }
     }

     //Creates a hotlink to a table with the given TID and Index.
     void set_Hotlink_Charges(int p_Table, int p_Index, int p_Assembly_From, int p_Construct_From, int p_Row_From)
     {
          if (!Input_Table_Charges_Hotlinks->table_Is_Empty())
          {
               Input_Table_Charges_Hotlinks->pop_push_int(p_Table);
               Input_Table_Charges_Hotlinks->pushc_int(p_Index);
               Input_Table_Charges_Hotlinks->pushc_int(p_Assembly_From);
               Input_Table_Charges_Hotlinks->pushc_int(p_Construct_From);
               Input_Table_Charges_Hotlinks->pushc_int(p_Row_From);
          }
          else
          {
               Input_Table_Charges_Hotlinks->pushc_int(p_Table);
               Input_Table_Charges_Hotlinks->pushc_int(p_Index);
               Input_Table_Charges_Hotlinks->pushc_int(p_Assembly_From);
               Input_Table_Charges_Hotlinks->pushc_int(p_Construct_From);
               Input_Table_Charges_Hotlinks->pushc_int(p_Row_From);
          }
     }

     //Resets the tables.
     void reset_Tables()
     {
          cout << "\n\t   reset_Tables()";

          Input_Table->reset_Table();
          Input_Table_Truth->reset_Table();

          Output_Table_Charge->reset_Table();
          Output_Table_Pattern->reset_Table();
          Output_Table_Nodes->reset_Table();
          Output_Table_Truth->reset_Table();
          Output_Table_RC->reset_Table();
     }

     //Resets the tables in the Assembly as well as the constructs.
     void reset_All_Tables()
     {
          reset_Tables();
          reset_All_Construct_Tables();
     }

     //--     TABLE HANDLING

     //Gets the ID of the input table.
     int get_Input_Table_ID(){ return Input_Table->get_Table_ID(); }
     int get_Input_Nodes_Table_ID(){ return Input_Nodes_Table->get_Table_ID(); }
     int get_Input_Charges_Table_ID(){ return Input_Charges_Table->get_Table_ID(); }
     int get_Input_Table_Truth_ID(){ return Input_Table_Truth->get_Table_ID(); }
     int get_Input_Table_Hotlinks_ID(){ return Input_Table_Hotlinks->get_Table_ID(); }
     int get_Input_Table_Nodes_Hotlinks_ID(){ return Input_Table_Nodes_Hotlinks->get_Table_ID(); }
     int get_Input_Table_Charges_Hotlinks_ID(){ return Input_Table_Charges_Hotlinks->get_Table_ID(); }

     int get_Output_Table_Charge_ID(){ return Output_Table_Charge->get_Table_ID(); }
     int get_Output_Table_Pattern_ID(){ return Output_Table_Pattern->get_Table_ID(); }
     int get_Output_Table_Nodes_ID(){ return Output_Table_Nodes->get_Table_ID(); }
     int get_Output_Table_Truth_ID(){ return Output_Table_Truth->get_Table_ID(); }
     int get_Output_Table_RC_ID(){ return Output_Table_RC->get_Table_ID(); }

     int get_Settings_Table_ID(){ return Settings_Table->get_Table_ID(); }
     int get_Internal_Table_ID(){ return Internal_Table->get_Table_ID(); }

     //Gets the Name of the table.
     string get_Input_Table_Name(){ return Input_Table->get_Table_Name(); }
     string get_Input_Nodes_Table_Name(){ return Input_Nodes_Table->get_Table_Name(); }
     string get_Input_Charges_Table_Name(){ return Input_Charges_Table->get_Table_Name(); }
     string get_Input_Table_Truth_Name(){ return Input_Table_Truth->get_Table_Name(); }
     string get_Input_Table_Hotlinks_Name(){ return Input_Table_Hotlinks->get_Table_Name(); }
     string get_Input_Table_Eval_Hotlinks_Name(){ return Input_Table_Hotlinks->get_Table_Name(); }
     string get_Input_Table_Nodes_Hotlinks_Name(){ return Input_Table_Nodes_Hotlinks->get_Table_Name(); }
     string get_Input_Table_Charges_Hotlinks_Name(){ return Input_Table_Charges_Hotlinks->get_Table_Name(); }

     string get_Output_Table_Charge_Name(){ return Output_Table_Charge->get_Table_Name(); }
     string get_Output_Table_Pattern_Name(){ return Output_Table_Pattern->get_Table_Name(); }
     string get_Output_Table_Nodes_Name(){ return Output_Table_Nodes->get_Table_Name(); }
     string get_Output_Table_Truth_Name(){ return Output_Table_Truth->get_Table_Name(); }
     string get_Output_Table_RC_Name(){ return Output_Table_RC->get_Table_Name(); }

     string get_Settings_Table_Name(){ return Settings_Table->get_Table_Name(); }
     string get_Internal_Table_Name(){ return Internal_Table->get_Table_Name(); }

     //Gets a reference to a table.
     c_Table_1D * get_Input_Table_Reference(){ return Input_Table; }
     c_Table_1D * get_Input_Nodes_Table_Reference(){ return Input_Nodes_Table; }
     c_Table_1D * get_Input_Charges_Table_Reference(){ return Input_Charges_Table; }
     c_Table_1D * get_Input_Table_Truth_Reference(){ return Input_Table_Truth; }
     c_Table_1D * get_Input_Table_Hotlinks_Reference(){ return Input_Table_Hotlinks; }
     c_Table_1D * get_Input_Table_Nodes_Hotlinks_Reference(){ return Input_Table_Nodes_Hotlinks; }
     c_Table_1D * get_Input_Table_Charges_Hotlinks_Reference(){ return Input_Table_Charges_Hotlinks; }

     c_Table_1D * get_Output_Table_Charge_Reference(){ return Output_Table_Charge; }
     c_Table_1D * get_Output_Table_Pattern_Reference(){ return Output_Table_Pattern; }
     c_Table_1D * get_Output_Table_Nodes_Reference(){ return Output_Table_Nodes; }
     c_Table_1D * get_Output_Table_Truth_Reference(){ return Output_Table_Truth; }
     c_Table_1D * get_Output_Table_RC_Reference(){ return Output_Table_RC; }

     c_Table_1D * get_Settings_Table_Reference(){ return Settings_Table; }
     c_Table_1D * get_Internal_Table_Reference(){ return Internal_Table; }

     //Updates the values of the variables based on the settings table.
     void update_Settings()
     {
          Settings_Table->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(0, 1, &Assembly_Name);
     }

     void update_Construct_Settings()
     {
          for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
          {
               if (Constructs[cou_Index] == NULL){ continue; }
               //cout << "\n\t\t Constructs[" << cou_Index << "]->" << Constructs[cou_Index]; cout.flush();
               Constructs[cou_Index]->update_Settings();
               Constructs[cou_Index]->update_Settings_Table();
          }
     }

     void update_All_Settings()
     {
          update_Settings();
          update_Construct_Settings();
     }


     //Updates the values of the settings table based on internal values.
     void update_Settings_Table()
     {
          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(0, 0, "Assembly Name");
          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(0, 1, Assembly_Name);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(1, 0, "Assembly ID");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(1, 1, AID);

          Settings_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(2, 0, "Gathering Input Table Index");
          Settings_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(2, 1, Gathering_Input_Table_Index);
     }

     //Sets the given setting to the given value.
     void change_Setting(string p_Setting, int p_Row, int p_Cell = 1){ Settings_Table->set_string(p_Row, p_Cell, p_Setting); }
     void change_Setting(int p_Setting,    int p_Row, int p_Cell = 1){ Settings_Table->set_int(p_Row, p_Cell, p_Setting); }
     void change_Setting(float p_Setting,  int p_Row, int p_Cell = 1){ Settings_Table->set_float(p_Row, p_Cell, p_Setting); }

     //Returns the state of a setting.
     string get_Setting_String(int p_Row, int p_Cell=1){ return Settings_Table->get_string(p_Row, p_Cell); }
     int    get_Setting_Int   (int p_Row, int p_Cell=1){ return Settings_Table->get_int(p_Row, p_Cell); }
     float  get_Setting_Float (int p_Row, int p_Cell=1){ return Settings_Table->get_float(p_Row, p_Cell); }

     //Returns the state of an internal setting.
     string get_Internal_String(int p_Row, int p_Cell=1){ return Internal_Table->get_string(p_Row, p_Cell); }
     int    get_Internal_Int   (int p_Row, int p_Cell=1){ return Internal_Table->get_int(p_Row, p_Cell); }
     float  get_Internal_Float (int p_Row, int p_Cell=1){ return Internal_Table->get_float(p_Row, p_Cell); }

     //Updates the values of the settings table based on internal values.
     void update_Internal_Table()
     {
          Internal_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(0, 0, "Number Of Constructs");
          Internal_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(0, 1, Construct_Count);

          Internal_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(1, 0, "Number Of Inputs");
          Internal_Table->set_Data_Bit_For_Given_Cell_In_Given_Row_I(1, 1, Construct_Index_Table->get_Row_Count());
     }



     //Sets the ID of the assembly.
     void set_Assembly_ID(int p_AID)
     {
          AID = p_AID;
     }

     //Gets the ID of the Assembly.
     int get_Assembly_ID()
     {
          return AID;
     }

     //Gets the name of the assembly.
     string get_Assembly_Name()
     {
          return Assembly_Name;
     }

     //Changes the name of the assembly and updates the tables in the registry.
     void set_Assembly_Name(string p_Assembly_Name)
     {
          //Set the name.
          Assembly_Name = p_Assembly_Name;

          //Update the registry.
          Tables->set_Table_Name(Input_Table->get_Table_ID(), "a_" + Assembly_Name + "_Input");
          Tables->set_Table_Name(Input_Table_Truth->get_Table_ID(), "a_" + Assembly_Name + "_Input_Truth");

          Tables->set_Table_Name(Output_Table_Pattern->get_Table_ID(), "a_" + Assembly_Name + "_Output_Pattern");
          Tables->set_Table_Name(Output_Table_Charge->get_Table_ID(), "a_" + Assembly_Name + "_Output_Charge");
          Tables->set_Table_Name(Output_Table_Nodes->get_Table_ID(), "a_" + Assembly_Name + "_Output_Nodes");
          Tables->set_Table_Name(Output_Table_Truth->get_Table_ID(), "a_" + Assembly_Name + "_Output_Truth");
          Tables->set_Table_Name(Output_Table_RC->get_Table_ID(), "a_" + Assembly_Name + "_Output_RC");

          Tables->set_Table_Name(Internal_Table->get_Table_ID(), "a_" + Assembly_Name + "_Internal");
          Tables->set_Table_Name(Settings_Table->get_Table_ID(), "a_" + Assembly_Name + "_Settings");
          Tables->set_Table_Name(Construct_Index_Table->get_Table_ID(), "a_" + Assembly_Name + "_Construct_Index");


          update_Settings_Table();

          //Updates the assembly name in the constructs.
          for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
          {
               Constructs[cou_Index]->set_Assembly_Name(Assembly_Name);
          }
     }

     //Gets the type of the assembly.
     string get_Assembly_Type()
     {
          return Assembly_Type;
     }

     //Checks to see if the assembly contains the given construct.
     bool does_Assembly_Contain_Construct(int p_CID)
     {
          for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
          {
               //cout << "\n\t  if (" << Constructs[cou_Index]->get_Construct_U_CID() << " == " << p_CID << ")->" << (Constructs[cou_Index]->get_Construct_U_CID() == p_CID);
               if (Constructs[cou_Index]->get_Construct_U_CID() == p_CID)
               {
                    return 1;
               }
          }
          return 0;
     }

     //Checks to see if the assembly contains the given construct.
     c_Base_Construct * does_Assembly_Contain_Construct(string p_Construct_Name)
     {
          for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
          {
               cout << "\n\t  if (" << Constructs[cou_Index]->get_Construct_Name() << " == " << p_Construct_Name << ")->" << (Constructs[cou_Index]->get_Construct_Name() == p_Construct_Name);
               if (Constructs[cou_Index]->get_Construct_Name() == p_Construct_Name)
               {
                    return Constructs[cou_Index];
               }
          }
          return NULL;
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

     //Pops the input.
     void pop_Input()
     {
          Input_Table->pop_Row();
     }

     //Submit an input to the input table.
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -"INPUT"
     //   0            1          2          3       4      5
     void push_Input_Truth(int p_Row, string p_Input)
     {
          Input_Table_Truth->push_string(p_Row, p_Input);
     }

     //Submit an input to the input table.
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -CELL -"INPUT"
     //   0            1          2          3       4    5       6
     void set_Input_Truth(int p_Row, int p_Cell, string p_Input)
     {
          Input_Table->set_string(p_Row, p_Cell, p_Input);
     }

     //Pops the input.
     void pop_Input_Truth()
     {
          Input_Table_Truth->pop_Row();
     }


     //Resets all of the inputs.
     void reset_All_Inputs()
     {
          reset_All_Tables();

          for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
          {
               Constructs[cou_Index]->reset_Tables();
          }
     }

     //--     CONSTRUCT INTERFACE

     //Gets the ID of the input table.
     int get_Construct_Input_Table_ID(int p_CID){ if (!bounds_CID(p_CID)){ return -1; } return Constructs[p_CID]->get_Input_Table_ID(); }
     int get_Construct_Input_Table_Truth_ID(int p_CID){ if (!bounds_CID(p_CID)){ return -1; } return Constructs[p_CID]->get_Input_Table_Truth_ID(); }

     int get_Construct_Output_Table_Charge_ID(int p_CID){ if (!bounds_CID(p_CID)){ return -1; } return Constructs[p_CID]->get_Output_Table_Charge_ID(); }
     int get_Construct_Output_Table_Pattern_ID(int p_CID){ if (!bounds_CID(p_CID)){ return -1; } return Constructs[p_CID]->get_Output_Table_Pattern_ID(); }
     int get_Construct_Output_Table_Nodes_ID(int p_CID){ if (!bounds_CID(p_CID)){ return -1; } return Constructs[p_CID]->get_Output_Table_Nodes_ID(); }
     int get_Construct_Output_Table_Truth_ID(int p_CID){ if (!bounds_CID(p_CID)){ return -1; } return Constructs[p_CID]->get_Output_Table_Truth_ID(); }
     int get_Construct_Output_Table_RC_ID(int p_CID){ if (!bounds_CID(p_CID)){ return -1; } return Constructs[p_CID]->get_Output_Table_RC_ID(); }

     int get_Construct_Settings_Table_ID(int p_CID){ if (!bounds_CID(p_CID)){ return -1; } return Constructs[p_CID]->get_Settings_Table_ID(); }
     int get_Construct_Internal_Table_ID(int p_CID){ if (!bounds_CID(p_CID)){ return -1; } return Constructs[p_CID]->get_Internal_Table_ID(); }
     int get_Construct_Treetop_Table_ID(int p_CID){ if (!bounds_CID(p_CID)){ return -1; } return Constructs[p_CID]->get_Treetop_Table_ID(); }

     //Gets the ID of the input table.
     int get_Construct_Input_Table_ID(string p_CName){ return get_Construct_Input_Table_ID(str2int(get_Construct_ID(p_CName))); }
     int get_Construct_Input_Table_Truth_ID(string p_CName){ return get_Construct_Input_Table_Truth_ID(str2int(get_Construct_ID(p_CName))); }

     int get_Construct_Output_Table_Charge_ID(string p_CName){ return get_Construct_Output_Table_Charge_ID(str2int(get_Construct_ID(p_CName))); }
     int get_Construct_Output_Table_Pattern_ID(string p_CName){ return get_Construct_Output_Table_Pattern_ID(str2int(get_Construct_ID(p_CName))); }
     int get_Construct_Output_Table_Nodes_ID(string p_CName){ return get_Construct_Output_Table_Nodes_ID(str2int(get_Construct_ID(p_CName))); }
     int get_Construct_Output_Table_Truth_ID(string p_CName){ return get_Construct_Output_Table_Truth_ID(str2int(get_Construct_ID(p_CName))); }
     int get_Construct_Output_Table_RC_ID(string p_CName){ return get_Construct_Output_Table_RC_ID(str2int(get_Construct_ID(p_CName))); }

     int get_Construct_Settings_Table_ID(string p_CName){ return get_Construct_Settings_Table_ID(str2int(get_Construct_ID(p_CName))); }
     int get_Construct_Internal_Table_ID(string p_CName){ return get_Construct_Internal_Table_ID(str2int(get_Construct_ID(p_CName))); }
     int get_Construct_Treetop_Table_ID(string p_CName){ return get_Construct_Treetop_Table_ID(str2int(get_Construct_ID(p_CName))); }

     //Resets the tables of a construct.
     void reset_Construct_Tables(int p_CID)
     {
          if (!bounds_CID(p_CID)){ return; }
          Constructs[p_CID]->reset_Tables();
     }
     void reset_Construct_Tables(string p_Construct)
     {
          reset_Construct_Tables(str2int(get_Construct_ID(p_Construct)));
     }

     //Resets the tables for all of the constructs.
     void reset_All_Construct_Tables()
     {
          for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
          {
               reset_Construct_Tables(cou_Index);
          }
     }

     //Resets the output tables of a construct.
     void reset_Construct_Output_Tables(int p_CID)
     {
          if (!bounds_CID(p_CID)){ return; }
          Constructs[p_CID]->reset_Output_Tables();
     }
     void reset_Construct_Output_Tables(string p_Construct)
     {
          reset_Construct_Tables(str2int(get_Construct_ID(p_Construct)));
     }

     //Creates a construct of the type specified by the parameter passed, returns a reference to the new construct.
     void * create_Construct(string p_Construct_Name = "Default_Construct", int p_Type = 0, int p_Dimension = 1)
     {
          switch(p_Dimension)
          {
               case 1:
                    //Raw Constructs, they are added to the Construct_Index.
                    //Check for NULL indexes to set.
                    for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
                    {
                         if (Constructs[cou_Index] == NULL)
                         {
                              if (p_Type == 0){ Constructs[cou_Index] = new c_Base_Construct_1D(Tables, Assembly_Name, (p_Construct_Name), cou_Index, p_Type); cout << "\n\t   new c_Construct()"; }
                              if (p_Type == 1){ Constructs[cou_Index] = new c_Base_Construct_1D(Tables, Assembly_Name, (p_Construct_Name), cou_Index, p_Type); cout << "\n\t   new c_Construct()"; }
                              if (p_Type == 2){ Constructs[cou_Index] = new c_Base_Construct_1D(Tables, Assembly_Name, (p_Construct_Name), cou_Index, p_Type); cout << "\n\t   new c_Construct()"; }
                              if (p_Type == 3){ Constructs[cou_Index] = new c_Int_Construct(Tables, Assembly_Name, (p_Construct_Name), cou_Index, p_Type); cout << "\n\t   new c_Int_Construct()"; }
                              if (p_Type == 4){ Constructs[cou_Index] = new c_Float_Construct(Tables, Assembly_Name, (p_Construct_Name), cou_Index, p_Type); cout << "\n\t   new c_Float_Construct()"; }
                              Construct_Registry.set_int(p_Construct_Name, cou_Index);
                              update_Internal_Table();
                              return Constructs[cou_Index];
                         }
                    }

                    expand_Constructs();
                    if (p_Type == 0){ Constructs[Construct_Count - 1] = new c_Base_Construct_1D(Tables, Assembly_Name, (p_Construct_Name), (Construct_Count - 1), p_Type); ostr(0, 8, "\n\t   new c_Construct()"); }
                    if (p_Type == 1){ Constructs[Construct_Count - 1] = new c_Base_Construct_1D(Tables, Assembly_Name, (p_Construct_Name), (Construct_Count - 1), p_Type); ostr(0, 8, "\n\t   new c_Construct()"); }
                    if (p_Type == 2){ Constructs[Construct_Count - 1] = new c_Base_Construct_1D(Tables, Assembly_Name, (p_Construct_Name), (Construct_Count - 1), p_Type); ostr(0, 8, "\n\t   new c_Construct()"); }
                    if (p_Type == 3){ Constructs[Construct_Count - 1] = new c_Int_Construct(Tables, Assembly_Name, (p_Construct_Name), (Construct_Count - 1), p_Type); ostr(0, 8, "\n\t   new c_Int_Construct()"); }
                    if (p_Type == 4){ Constructs[Construct_Count - 1] = new c_Float_Construct(Tables, Assembly_Name, (p_Construct_Name), (Construct_Count - 1), p_Type); ostr(0, 8, "\n\t   new c_Float_Construct()"); }
                    Construct_Registry.set_int(p_Construct_Name, (Construct_Count -1));
                    update_Internal_Table();
                    return Constructs[Construct_Count - 1];
                    break;

          }
          return NULL;
     }

     //Deletes a construct.
     void delete_Construct(int p_CID)
     {
          if (!bounds_CID(p_CID)){ return; }

          Construct_Registry.remove_Node(Constructs[p_CID]->get_Construct_Name());

          delete Constructs[p_CID];
          Constructs[p_CID] = NULL;
          return;
     }

     //Deletes a construct.
     void delete_Construct(string p_Construct)
     {
          int tmp_CID = str2int(get_Construct_ID(p_Construct));

          if (!bounds_CID(tmp_CID)){ return; }

          Construct_Registry.remove_Node(p_Construct);

          delete Constructs[tmp_CID];
          Constructs[tmp_CID] = NULL;
          return;
     }

     //Gets whether or not a construct exists.
     string exists_Construct(string p_Construct)
     {
          return int2str(Construct_Registry.exists(p_Construct));
     }

     //Sets the name of a construct.
     void set_Construct_Name(int p_CID, string p_Construct_Name)
     {
          if (!bounds_CID(p_CID)){ return; }

          Construct_Registry.change_name(Constructs[p_CID]->get_Construct_Name(), p_Construct_Name);
          Constructs[p_CID]->set_Construct_Name(p_Construct_Name);
     }

     //Sets the name of a construct.
     string get_Construct_Name(int p_CID)
     {
          if (!bounds_CID(p_CID)){ return "0"; }

          return Constructs[p_CID]->get_Construct_Name();
     }

     //Sets the name of a construct.
     string get_Construct_ID(string p_Construct)
     {
          if (Construct_Registry.exists(p_Construct)){ return int2str(Construct_Registry.get_int(p_Construct)); }
          return "-1";
     }


     //Gets the index of a construct from the given UCID.
     int get_Construct_Index(int p_UCID)
     {
          for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
          {
               //If the constructs u_CID matches return the index of the construct.
               if (p_UCID == Constructs[cou_Index]->get_Construct_U_CID())
               {
                    return cou_Index;
               }
          }

          //If the construct is not found that matches then return -1.
          return -1;
     }

     //Submit an input to the input table.
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -"INPUT"
     //   0            1          2          3       4      5
     void push_Input_To_Construct(string p_Construct, int p_Row, string p_Input)
     {
          int tmp_CID = str2int(get_Construct_ID(p_Construct));

          if (!bounds_CID(tmp_CID)){ return; }

          Constructs[tmp_CID]->push_Input(p_Row, p_Input);
     }

     //Submit an input to the input table.
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -CELL -"INPUT"
     //   0            1          2          3       4    5       6
     void set_Input_In_Construct(string p_Construct, int p_Row, int p_Cell, string p_Input)
     {
          int tmp_CID = str2int(get_Construct_ID(p_Construct));

          if (!bounds_CID(tmp_CID)){ return; }

          Constructs[tmp_CID]->set_Input(p_Row, p_Cell, p_Input);
     }


     //Evals a given construct
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -CELL -"INPUT"
     //   0            1          2          3       4    5       6
     void eval_Construct(string p_Construct, string p_Method)
     {
          int tmp_CID = str2int(get_Construct_ID(p_Construct));

          //cout << "\n\t eval_Construct(p_C->" << p_Construct << ", p_M->" << p_Method << ")";

          if (!bounds_CID(tmp_CID)){ return; }

          if (p_Method == "full")       { Constructs[tmp_CID]->eval_Input_Table(0); return; }
          if (p_Method == "full_RC")    { Constructs[tmp_CID]->eval_Input_Table(1); return; }
          if (p_Method == "query")      { Constructs[tmp_CID]->eval_Input_Table(2); return; }
          if (p_Method == "full_row")   { Constructs[tmp_CID]->eval_Input_Table(3); return; }
          if (p_Method == "full_RC_row"){ Constructs[tmp_CID]->eval_Input_Table(4); return; }

          //Default is full.
          Constructs[tmp_CID]->eval_Input_Table(0);

          Constructs[tmp_CID]->output_Tables();
     }


     //Evals a given construct
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -CELL -"INPUT"
     //   0            1          2          3       4    5       6
     void build_Construct(string p_Construct, string p_Method)
     {
          int tmp_CID = str2int(get_Construct_ID(p_Construct));

          if (!bounds_CID(tmp_CID)){ return; }


          if (p_Method == "full")       { Constructs[tmp_CID]->build_Input_Table(0); return; }
          if (p_Method == "full_RC")    { Constructs[tmp_CID]->build_Input_Table(1); return; }
          if (p_Method == "query")      { Constructs[tmp_CID]->build_Input_Table(2); return; }
          if (p_Method == "full_row")   { Constructs[tmp_CID]->build_Input_Table(3); return; }
          if (p_Method == "full_RC_row"){ Constructs[tmp_CID]->build_Input_Table(4); return; }

          //Default is full.
          Constructs[tmp_CID]->build_Input_Table(0);
     }

     //Evals a given construct
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -CELL -"INPUT"
     //   0            1          2          3       4    5       6
     void build_Construct(int p_CID, string p_Method)
     {

          if (!bounds_CID(p_CID)){ return; }

          if (p_Method == "full")       { Constructs[p_CID]->build_Input_Table(0); return; }
          if (p_Method == "full_RC")    { Constructs[p_CID]->build_Input_Table(1); return; }
          if (p_Method == "query")      { Constructs[p_CID]->build_Input_Table(2); return; }
          if (p_Method == "full_row")   { Constructs[p_CID]->build_Input_Table(3); return; }
          if (p_Method == "full_RC_row"){ Constructs[p_CID]->build_Input_Table(4); return; }

          //Default is full.
          Constructs[p_CID]->build_Input_Table(0);
     }

     //Builds the construct using the constructs build settings.
     void build_Construct(int p_CID)
     {
          if (!bounds_CID(p_CID)){ return; }

          Constructs[p_CID]->Build();

     }
     void build_Construct(string p_Construct)
     {
          int tmp_CID = str2int(get_Construct_ID(p_Construct));
          build_Construct(tmp_CID);
     }

     //Eval a construct using the constructs eval settings.they seem to hate her even more than the used to.but that would mean that all of this was for nothing.
     void eval_Construct(int p_CID)
     {
          if (!bounds_CID(p_CID)){ return; }

          Constructs[p_CID]->Eval();

     }
     void eval_Construct(string p_Construct)
     {
          int tmp_CID = str2int(get_Construct_ID(p_Construct));
          eval_Construct(tmp_CID);
     }

     //Evals a given construct
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -CELL -"INPUT"
     //   0            1          2          3       4    5       6
     void build_Construct_BM(string p_Construct, string p_Method)
     {
          int tmp_CID = str2int(get_Construct_ID(p_Construct));

          if (!bounds_CID(tmp_CID)){ return; }

          if (p_Method == "full")       { Constructs[tmp_CID]->build_Input_Table_BM(0); return; }
          if (p_Method == "full_RC")    { Constructs[tmp_CID]->build_Input_Table_BM(1); return; }
          if (p_Method == "query")      { Constructs[tmp_CID]->build_Input_Table_BM(2); return; }
          if (p_Method == "full_row")   { Constructs[tmp_CID]->build_Input_Table_BM(3); return; }
          if (p_Method == "full_RC_row"){ Constructs[tmp_CID]->build_Input_Table_BM(4); return; }

          //Default is full.
          Constructs[tmp_CID]->build_Input_Table_BM(0);
     }

     //Evals a given construct
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -CELL -"INPUT"
     //   0            1          2          3       4    5       6
     void build_Construct_BM(int p_CID, string p_Method)
     {
          if (!bounds_CID(p_CID)){ return; }

          if (p_Method == "full")       { Constructs[p_CID]->build_Input_Table_BM(0); return; }
          if (p_Method == "full_RC")    { Constructs[p_CID]->build_Input_Table_BM(1); return; }
          if (p_Method == "query")      { Constructs[p_CID]->build_Input_Table_BM(2); return; }
          if (p_Method == "full_row")   { Constructs[p_CID]->build_Input_Table_BM(3); return; }
          if (p_Method == "full_RC_row"){ Constructs[p_CID]->build_Input_Table_BM(4); return; }

          //Default is full.
          Constructs[p_CID]->build_Input_Table_BM(0);
     }

     //Evaluates the results of a query on a lower tier construct.
     void eval_Foreign_Output_Table(string p_Construct, string p_Output_Table_Nodes, string p_Output_Table_Charges)
     {
          cout << "\n\t\t eval_Foreign_Output_Table(" << p_Construct << ", " << p_Output_Table_Nodes << ", " << p_Output_Table_Charges << ")";

          int tmp_CID = str2int(get_Construct_ID(p_Construct));

          cout << "\n\t tmp_CID->" << tmp_CID;

          //The output tables.
          void * tmp_Nodes =  Tables->get_Table_Reference(p_Output_Table_Nodes);
          void * tmp_Charges = Tables->get_Table_Reference(p_Output_Table_Charges);

          if (!bounds_CID(tmp_CID)){ return; }

          cout << "\n\t Constructs[" << tmp_CID << "]->eval_Foreign_Output_Table()";
          //Constructs[tmp_CID]->output_Tables();
          Constructs[tmp_CID]->eval_Foreign_Output_Table((c_Table_1D*) tmp_Nodes, (c_Table_1D*) tmp_Charges);
     }
     void eval_Foreign_Output_Table(int p_CID, int p_Output_Table_Nodes_ID, int p_Output_Table_Charges_ID)
     {
          //The output tables.
          void * tmp_Nodes =  Tables->get_Table_Reference(p_Output_Table_Nodes_ID);
          void * tmp_Charges = Tables->get_Table_Reference(p_Output_Table_Charges_ID);

          if (!bounds_CID(p_CID)){ return; }

          Constructs[p_CID]->eval_Foreign_Output_Table((c_Table_1D*) tmp_Nodes, (c_Table_1D*) tmp_Charges);
     }

     //--     ARRAY MANAGMENT FUNCTIONS

     //Expands the construct array by one.
     void expand_Constructs()
     {
          expand_Constructs(Construct_Count + 1);
     }

     //Expands the construct array to the given dimensions keeping the current constructs intact. After this is called the new indexes will be
     //NULL and will have to be created separately.
     void expand_Constructs(int p_Construct_Count)
     {
          //Defeat the enemy known as unecessary array copying and reallocation followed by more copying and deletion.
          if (p_Construct_Count <= Construct_Count){ return; }

          //Setup the temporary array to hold the current constructs.
          c_Base_Construct ** tmp_Constructs = new c_Base_Construct*[Construct_Count];

          //Copy the old constructs into the temporary array nullifying the old array as it goes.
          for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
          {
               tmp_Constructs[cou_Index] = Constructs[cou_Index];
               Constructs[cou_Index] = NULL;
          }

          //Delete the Constructs array and reallocate it to the appropriate dimensions.
          delete [] Constructs;
          Constructs = NULL;

          Constructs = new c_Base_Construct*[p_Construct_Count];

          //Copy the old construct addresses into the reallocated constructs array.
          for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
          {
               Constructs[cou_Index] = tmp_Constructs[cou_Index];
               tmp_Constructs[cou_Index] = NULL;
          }

          //Cleanup the temporary array to avoid memory leaks.
          delete [] tmp_Constructs;
          tmp_Constructs = NULL;

          //Initialize the newly allocated portion of the constructs array to NULL.
          for (int cou_Index=Construct_Count;cou_Index<p_Construct_Count;cou_Index++)
          {
               Constructs[cou_Index] = NULL;
          }

          //Set the construct count to the appropriate amount.
          Construct_Count = p_Construct_Count;

          return;
     }

     //Resets the construct array.
     void reset_constructs()
     {
          for (int cou_Construct=0;cou_Construct<Construct_Count;cou_Construct++)
          {
               delete Constructs[cou_Construct];
               Constructs[cou_Construct] = NULL;
          }
          delete [] Constructs;
          Constructs = NULL;
          Construct_Count = 0;
     }

     //Bounds checking
     bool bounds_CID(int p_CID)
     {
          if (p_CID < 0 || p_CID >= Construct_Count || Constructs[p_CID] == NULL){ return 0; }
          return 1;
     }


     //==--     INTERFACE COMMANDS FOR THE META-ASSEMBLIES

     //Build
     virtual string Build()
     {
          //The name of the current construct being worked on.
          string tmp_CName = "";
          int tmp_CID = 0;

          //Loop through all of the raw construct in the priority table.
          for (int cou_Index=0;cou_Index<Construct_Index_Table->get_Row_Count();cou_Index++)
          {
               tmp_CName = Construct_Index_Table->get_string(cou_Index, 0);
               tmp_CID = str2int(get_Construct_ID(tmp_CName));

               if (tmp_CID == -1){ continue; }

               //Gather the input for the construct.
               Tables->translate_Row(Input_Table->get_Table_ID(), cou_Index, get_Construct_Input_Table_ID(tmp_CID), 0);

               //Build it.
               //build_Construct(tmp_CName, "full_RC");
               build_Construct(tmp_CName);

          }
          return "Build";
     }



     //Builds for the benchmarking.
     virtual string Build_BM()
     {
          //The name of the current construct being worked on.
          string tmp_CName = "";
          int tmp_CID = 0;

          //Loop through all of the raw construct in the priority table.
          for (int cou_Index=0;cou_Index<Construct_Index_Table->get_Row_Count();cou_Index++)
          {
               tmp_CName = Construct_Index_Table->get_string(cou_Index, 0);
               tmp_CID = str2int(get_Construct_ID(tmp_CName));

               if (tmp_CID == -1){ continue; }

               //Gather the input for the construct.
               Tables->translate_Row(Input_Table->get_Table_ID(), cou_Index, get_Construct_Input_Table_ID(tmp_CID), 0);

               //Build it.
               build_Construct_BM(tmp_CName, "full_RC");

          }
          return "Build";
     }


     //Eval
     virtual string Eval()
     {
          //cout << "\n\t Eval() --BASE";

          if (Input_Table->table_Is_Empty()){ cout << "\n\t  NO INPUT!"; return "No_Input!"; }

          //The name of the current construct being worked on.
          string tmp_CName = "";
          int tmp_CID = 0;

          //Input_Table->output_Table();

          //Construct_Index_Table->output_Table();

          //Loop through all of the raw construct in the priority table.
          for (int cou_Index=0;cou_Index<Construct_Index_Table->get_Row_Count();cou_Index++)
          {
               tmp_CName = Construct_Index_Table->get_string(cou_Index, 0);
               tmp_CID = str2int(get_Construct_ID(tmp_CName));

               if (tmp_CID == -1){ continue; }


               //cout << "\n\t  IR[" << cou_Index << "]->" << Input_Table->row_Is_Empty(cou_Index);

               if (Input_Table->row_Is_Empty(cou_Index)){ cout << "\n\t  CONSTRUCT " << tmp_CName << " (" << cou_Index << ") NO INPUT!"; continue; }

               //Gather the input for the construct.
               Tables->translate_Row(Input_Table->get_Table_ID(), cou_Index, get_Construct_Input_Table_ID(tmp_CID), 0);

               //Tables->output_Table(get_Construct_Input_Table_ID(tmp_CID));

               //Build it.
               //eval_Construct(tmp_CName, "query");
               eval_Construct(tmp_CName);

          }
          return "Eval";
     }

     //Train
     virtual string Train()
     {

          return "Train";
     }

     //Adds a construct chain to the assembly.
     virtual string Add_Raw_Construct(string p_CName, int p_Dimension = 1)
     {
          //Create the raw construct as a basic raw construct.
          create_Construct(p_CName, 1, p_Dimension);

          //Push the raw construct onto the Index_Table.
          if (Construct_Index_Table->table_Is_Empty())
          {
               Construct_Index_Table->push_string(0, p_CName);
          }
          else
          {
               Construct_Index_Table->pop_push_string(p_CName);
          }
          Construct_Index_Table->output_Table();
          //Create the chrono construct.
          return "Add_Raw_Construct->" + p_CName;
     }

     //Adds a construct chain to the assembly.
     virtual string Add_Raw_Int_Construct(string p_CName, int p_Dimension = 1)
     {
          //Create the raw construct as a basic raw construct.
          create_Construct(p_CName, 3, p_Dimension);

          //Push the raw construct onto the Index_Table.
          if (Construct_Index_Table->table_Is_Empty())
          {
               Construct_Index_Table->push_string(0, p_CName);
          }
          else
          {
               Construct_Index_Table->pop_push_string(p_CName);
          }

          Construct_Index_Table->output_Table();

          //Create the chrono construct.
          return "Add_Raw_Construct->" + p_CName;
     }

     //Adds a construct chain to the assembly.
     virtual string Add_Raw_Float_Construct(string p_CName, int p_Dimension = 1)
     {
          //Create the raw construct as a basic raw construct.
          create_Construct(p_CName, 4, p_Dimension);

          //Push the raw construct onto the Index_Table.
          if (Construct_Index_Table->table_Is_Empty())
          {
               Construct_Index_Table->push_string(0, p_CName);
          }
          else
          {
               Construct_Index_Table->pop_push_string(p_CName);
          }

          Construct_Index_Table->output_Table();

          //Create the chrono construct.
          return "Add_Raw_Construct->" + p_CName;
     }

     //--     OUTPUT FUNCTIONS

     //Outputs a given Construct.
     void output_given_Construct(int p_Construct)
     {
          Constructs[p_Construct]->output_Construct();
     }

     //Outputs all the Constructs
     void output_Every_Construct()
     {
          for (int cou_Construct=0;cou_Construct<Construct_Count;cou_Construct++)
          {
               cout << "\n";
               output_given_Construct(cou_Construct);
          }
     }

     //Outputs all the Constructs Full
     void output_Every_Construct_Full()
     {
          for (int cou_Construct=0;cou_Construct<Construct_Count;cou_Construct++)
          {
               cout << "\n";
               output_Construct_Full(cou_Construct);
          }
     }

     //Outputs the tables containted within.
     void output_Tables()
     {
          Input_Table->output_Table();
          Input_Table_Hotlinks->output_Table();
          Output_Table_Charge->output_Table();
          Output_Table_Pattern->output_Table();
     }

     //Outputs all of the tables and the constructs tables that are contained within.
     void output_All_Tables()
     {
          output_Tables();
          for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
          {
               output_Construct_Tables(cou_Index);
          }
     }

     //Outputs the construct.
     void output_Assembly()
     {
          cout << "\n\n\t----------";
          cout << "\n\t Assembly->" << Assembly_Name << this;
          cout << "\n\t   Construct_Count->" << Construct_Count;
          output_Tables();
          output_Every_Construct_Full();
     }



     //==--     OUTPUT FUNCTIONS FOR A CONSTRUCT

	//Outputs the given CAN.
     void output_given_CAN(int p_CID, int p_CAN)
     {
          if (!bounds_CID(p_CID)){ return; }

          Constructs[p_CID]->output_given_CAN(p_CAN);
     }

     //Outputs all the CANs
     void output_Every_CAN(int p_CID)
     {
          if (!bounds_CID(p_CID)){ return; }

          Constructs[p_CID]->output_Every_CAN();
     }

     //Output Node Network.
     void output_Node_Network(int p_CID)
     {
          if (!bounds_CID(p_CID)){ return; }

          Constructs[p_CID]->output_Node_Network();
     }

     //Outputs the tables containted within.
     void output_Construct_Tables(int p_CID)
     {
          if (!bounds_CID(p_CID)){ return; }

          Constructs[p_CID]->output_Construct_Tables();
     }

     //Outputs the construct internals.
     void output_Construct_Internals(int p_CID)
     {
          if (!bounds_CID(p_CID)){ return; }

          Constructs[p_CID]->output_Construct_Internals();
     }

     //Outputs the construct.
     void output_Construct_Full(int p_CID)
     {
          if (!bounds_CID(p_CID)){ return; }

          Constructs[p_CID]->output_Construct_Full();

          //output_Construct_Tables(p_CID);
          //output_Construct_Internals(p_CID);
     }

     //Gets the state from the construct into a string for output to a file.
     string get_Stats()
     {
          string tmp_Stats = "   " + Assembly_Name + " ";
          for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
          {
               tmp_Stats += Constructs[cou_Index]->get_Stats();
          }
          return tmp_Stats;
     }

     //Outputs the current debug information.
     void output_Debug()
     {
          //int tmp_Table = 0;

          for (int cou_Index=0;cou_Index<Construct_Count;cou_Index++)
          {
               Constructs[cou_Index]->output_Node_Network_Tier(-1);
          }
     }

     //Saves the assembly and its contents.
     void Save(string p_Directory)
     {
          cout << "\n\n\t Saving Assembly->" << Assembly_Name;
          //Gathers the directory and creates it if need be.
          //string tmp_MD = "md " + p_Directory;

          //cout << "\n\n\t->" << p_Directory;
          //system(tmp_MD.c_str());

          //Gathers the file to save the assebmly to.
          ofstream SF;
          string tmp_FN = p_Directory + "\\" + Assembly_Name + ".ass";
          cout << "\n\t tmp_FN->" << tmp_FN;
          SF.open(tmp_FN.c_str());


          //Save the constructs.
          for (int cou_Construct=0;cou_Construct<Construct_Count;cou_Construct++)
          {
               Constructs[cou_Construct]->Save(p_Directory);
          }

          //Save the construct index table.
          SF << "\n" << Construct_Index_Table->get_Row_Count();

          cout << "\n\n\n\n\n\n\n";
          Construct_Index_Table->output_Table();


          for (int cou_Construct=0;cou_Construct<Construct_Index_Table->get_Row_Count();cou_Construct++)
          {
               SF << "\n" << Construct_Index_Table->get_string(cou_Construct, 0);
          }

          //Close the file when done.
          SF.close();
     }

     //Loads an assembly.
     void Load_Assembly(string p_Directory)
     {
          //Setup the temporary vars.
          string tmp_Assembly_Name;
          string tmp_String;
          int tmp_Int = 0;

          //Gathers the file to save the node network to.
          ifstream SF;
          string tmp_FN = p_Directory + Assembly_Name + ".ass";
          cout << "\n\t Opening File->" << tmp_FN;
          SF.open(tmp_FN.c_str());

          if (!SF.is_open()){ cout << "\n\n\t FILE " << tmp_FN << " NOT OPENED!>!>!>!"; return; }

          SF >> tmp_Int;

          for (int cou_Index=0;cou_Index<tmp_Int;cou_Index++)
          {
               SF >> tmp_String;

               //Exceptions for inbuilt constructs.
               if (tmp_String == "MSC"){ continue; }
               Construct_Index_Table->push_string(cou_Index, tmp_String);
          }

          Construct_Index_Table->output_Table();

     }

     //Loads an assembly.
     void Load_Construct(string p_Directory, string p_Construct)
     {
          //Setup the temporary vars.
          string tmp_Construct_Name;
          string tmp_Assembly_Name;
          int tmp_Construct_Input_Type;
          int tmp_Construct_Type;

          int tmp_Base_Charge;
          float tmp_Filter_Charge;
          int tmp_Charge_Precision;
          int tmp_Query_Cap;
          int tmp_Charging_Tier;
          int tmp_Tolerance;

          void * tmp_Construct = NULL;

          //Gathers the file to save the node network to.
          ifstream SF;
          string tmp_FN = p_Directory + Assembly_Name + "__" + p_Construct + ".con";
          cout << "\n\t Opening File->" << tmp_FN;
          SF.open(tmp_FN.c_str());

          if (!SF.is_open()){ cout << "\n\n\t FILE " << tmp_FN << " NOT OPENED!>!>!>!"; return; }

          //Save the Construct Information.
          SF >> tmp_Construct_Name;
          SF >> tmp_Assembly_Name;
          SF >> tmp_Construct_Input_Type;
          SF >> tmp_Construct_Type;

          SF >> tmp_Base_Charge;
          SF >> tmp_Filter_Charge;
          SF >> tmp_Charge_Precision;
          SF >> tmp_Query_Cap;
          SF >> tmp_Charging_Tier;
          SF >> tmp_Tolerance;


          //Set the settings.
          cout << "\n\t";
          ostr(0, 10, tmp_Construct_Name);
          cout << "\n\t";
          ostr(0, 10, tmp_Assembly_Name);

          cout << "\n\t";
          oint(0, 11, tmp_Construct_Input_Type);

          cout << "\n\t";
          oint(0, 11, tmp_Base_Charge);
          cout << "\n\t";
          oflt(0, 12, tmp_Filter_Charge);
          cout << "\n\t";
          oint(0, 11, tmp_Charge_Precision);
          cout << "\n\t";
          oint(0, 11, tmp_Query_Cap);
          cout << "\n\t";
          oint(0, 11, tmp_Charging_Tier);
          cout << "\n\t";
          oint(0, 11, tmp_Tolerance);

          tmp_Construct = does_Assembly_Contain_Construct(tmp_Construct_Name);

          if (tmp_Construct == NULL)
          {
               tmp_Construct = create_Construct(tmp_Construct_Name, tmp_Construct_Type);
          }

          ((c_Base_Construct*) tmp_Construct)->change_Settings(tmp_Construct_Name, tmp_Assembly_Name, tmp_Construct_Input_Type, tmp_Base_Charge, tmp_Filter_Charge, tmp_Charge_Precision, tmp_Query_Cap, tmp_Charging_Tier, tmp_Tolerance);

          tmp_FN = Assembly_Name + "__" + tmp_Construct_Name + "_Nodes";
          ((c_Base_Construct*) tmp_Construct)->load(p_Directory, tmp_FN);


          //void change_Settings
          //(string p_Construct_Name, string p_Assembly_Name, int p_Construct_Input_Type, float p_Base_Charge, float p_Filter_Charge, float p_Charge_Precision,
          //int p_Query_Cap, int p_Charging_Tier, int p_Tolerance)=0;

          SF.close();

     }
};




