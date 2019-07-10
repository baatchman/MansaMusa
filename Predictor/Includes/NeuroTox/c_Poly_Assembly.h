
//   The assembly holds all of the internal constructs. Each construct is held in an 
// array of references to the constructs. Each construct is accessed by the cosntruct ID
// which corresponds to the particular constructs posistion in the construct array.
// Constructs[1] would be Construct_ID = 1. The other parts of the constructs that are
// neccessary for the constructs to operate together such as the input classes are held in
// arrays.
class c_Poly_Assembly
{
public:
     
     //All of the Constructs in this particular assembly.
     c_Poly_CAN ** Constructs;
     
     //The current number of Constructs in the assembly.
     int Constructs_Current_Count;
     
     //The maximun indexes currently allocated for the assembly.  
     int Constructs_Max_Count;
     
     //The last created construct.
     int Last_Created_Construct;
     
     //All of the Outputs in this particular assembly, the outputs arrays uses the construct counters.
     c_1D_Output ** Outputs;
     
     //The variables for the file loading.
     c_Poly_Node * Last_Created_Node;
     int tmp_int_File[10];
     float tmp_float_File[10];
     u_IO_Data tmp_data_File[10];
     
     //The table handler for all internal tables.
     c_Table_Handler Tables;
     
     //The input output tables as reference IDs.
     int Input_Table;
     int Output_Table;
     int Properties_Table;
     int Settings_Table;
     int Command_Table;
     int Submission_Cmd_Table;
     int Query_Cmd_Table;
     int Bld_Cmd_Table;
     int Qry_Cmd_Table;
     int Torch_Cmd_Table;
     int Neutralize_Cmd_Table;
     
     //The autobuild members.
     int flg_Autobuild;
     bool flg_Autobuild_Input_Has_Construct[1000];
     
     u_IO_Data tmp_Command_Text[25];
     char tmp_Command_Char[25];
     string tmp_Command_String;
     
     c_Poly_Assembly(int p_Autobuild = 1)
     { 
          
          //When a construct is created it is passed a reference to the table handler, this way the construct can internally handle table manipulations.
          
          //Setup the construct variables.
          Constructs_Current_Count = 0;
          Constructs_Max_Count = 100;
          Constructs = new c_Poly_CAN*[Constructs_Max_Count];
          Last_Created_Construct = 0;
          
          //Setup the output variables.
          Outputs = new c_1D_Output*[Constructs_Max_Count];
          
          //Setup the tables. This is done after all of the member variables are initialized.
          Input_Table = Tables.new_Table();
          Tables.set_Table_Name("Assembly: Input", Input_Table);
          
          Output_Table = Tables.new_Table();
          Tables.set_Table_Name("Assembly: Output", Output_Table);
          
          Properties_Table = Tables.new_Table();
          Tables.set_Table_Name("Assembly: Properties", Properties_Table);
          
          Settings_Table = Tables.new_Table();
          Tables.set_Table_Name("Assembly: Settings", Settings_Table);
          
          Command_Table = Tables.new_Table();
          Tables.set_Table_Name("Assembly: Command", Command_Table);
          
          Submission_Cmd_Table = Tables.new_Table();
          Tables.set_Table_Name("Assembly: Submission_Cmd_Table", Submission_Cmd_Table);
          
          Query_Cmd_Table = Tables.new_Table();
          Tables.set_Table_Name("Assembly: Query_Cmd_Table", Query_Cmd_Table);
          
          Bld_Cmd_Table = Tables.new_Table();
          Tables.set_Table_Name("Assembly: Bld_Cmd_Table", Bld_Cmd_Table);
          
          Qry_Cmd_Table = Tables.new_Table();
          Tables.set_Table_Name("Assembly: Qry_Cmd_Table", Qry_Cmd_Table);
          
          Torch_Cmd_Table = Tables.new_Table();
          Tables.set_Table_Name("Assembly: Torch_Cmd_Table", Torch_Cmd_Table);
          
          Neutralize_Cmd_Table = Tables.new_Table();
          Tables.set_Table_Name("Assembly: Neutralize_Cmd_Table", Neutralize_Cmd_Table);
          
          
     
          update_Assembly_Tables();
          
          flg_Autobuild = p_Autobuild;
          if (flg_Autobuild)
          {
               create_New_Construct("Auto_MSC", 1);
               //generate command text.
               
               //--Submission Table
               
               //clear the tmp string.
               tmp_Command_String.clear();
               clear_Char(tmp_Command_Char, 25);
               
               //Build the commands and submit them.
               
               int2char(Bld_Cmd_Table, tmp_Command_Char);
               tmp_Command_String = "eval ";
               tmp_Command_String += tmp_Command_Char;
               Tables.push(tmp_Command_String, Submission_Cmd_Table);
               Tables.push("reset I", Submission_Cmd_Table);
               
               //--Query Table
               
               
               //Build and submit the commands.
               
               //Tables.push("neutralize all", Query_Cmd_Table);//Neutralize all charges to prep for query.
               Tables.push("reset O", Query_Cmd_Table);//Reset all outputs to prep for query.
               
               //clear the tmp string and gather the table ID for the Qry table.
               tmp_Command_String.clear();
               clear_Char(tmp_Command_Char, 25);
               int2char(Qry_Cmd_Table, tmp_Command_Char); //Get the Table ID as a char array.
               tmp_Command_String = "eval ";
               tmp_Command_String += tmp_Command_Char;
               Tables.push(tmp_Command_String, Query_Cmd_Table);
               
               //clear the tmp string and gather the table ID for the torch table.
               tmp_Command_String.clear();
               clear_Char(tmp_Command_Char, 25);
               int2char(Torch_Cmd_Table, tmp_Command_Char); //Get the Table ID as a char array.
               tmp_Command_String = "eval ";
               tmp_Command_String += tmp_Command_Char;
               Tables.push(tmp_Command_String, Query_Cmd_Table);
               
               Tables.push("srt all 0000", Query_Cmd_Table);//Default sorting flag is 0.
               
               
               //--Bld Table
               Tables.shove("bld 0000 0001", Bld_Cmd_Table);
               
               //--Torch table
               Tables.push("torch 0000", Torch_Cmd_Table);
               
               
               for (int cou_Index=0;cou_Index<1000;cou_Index++)
               {
                    flg_Autobuild_Input_Has_Construct[cou_Index] = false;
               }
          }
          
          cout << " c_Poly_Assembly(" << this << ")"; cout.flush();
     }
     
     ~c_Poly_Assembly()
     {
          delete [] Constructs;
          delete [] Outputs;
     }
     
     
     //Creates a new construct in the current assembly.
     void* create_New_Construct(string p_Construct_Name = "Generic_Construct", int p_Construct_Type = 0)
     {
          cout << "\n\t->create_New_Construct()";
          
          //Checks for array bounding.
          if (Constructs_Current_Count == Constructs_Max_Count)
          {
               //Increase the size of the node array so that we do not throw bounding errors.
               increase_Construct_Array_Depth();
          }
          
          //Creates the output for the construct that is to be created.
          Outputs[Constructs_Current_Count] = new c_1D_Output();
          
          //Creates the new construct, passes the adress of the output associated with this construct.
          Constructs[Constructs_Current_Count] = new c_Poly_CAN(Constructs_Current_Count, &Tables, p_Construct_Name, p_Construct_Type);
          
          cout << "\n\t\t~~Construct Created:" << Constructs[Constructs_Current_Count] << " Constructs_Current_Count:" << Constructs_Current_Count;
          
          //Increases the node count.
          Constructs_Current_Count++;
          
          //Updates the assembly tables to reflect the new addtions.
          update_Assembly_Settings_Table();
          
          //Gets the CID of the last created construct.
          Last_Created_Construct = Constructs_Current_Count - 1;
          
          //Returns a reference to the newly created node.
          return Constructs[Constructs_Current_Count - 1];
     }
     
     /*
     //The reference inputs for this assemble, links to treetops. [Construct_ID] [Number_Of_Treetops] **Pointer to the treetop pointer
     void **** Reference_Inputs;
     
     //The number of references for a particular construct.
     int * Number_Of_Reference_Nodes;
      * 
      * 
      * 
     void * te;
     void ** tes;
     te = &testera;
     cout << te;
     tes = &te;
     cout << " " << *tes << " " << tes;
     */
     //Increases the number of constructs in the assembly.
     void increase_Construct_Array_Depth()
     {
          cout << "\n   increase_Construct_Array_Depth()"; cout.flush();
          //Allocate a temporary array and copy the original into it, then delete the original and set it to the temporary array.
          
          //Increase the number of dendrites.
          Constructs_Max_Count += 1;
          
          //Creates the array to copy the information into with the new number of dendrites.
          c_Poly_CAN ** tmp_Constructs;
          c_1D_Output ** tmp_Outputs;
          
          tmp_Constructs = new c_Poly_CAN*[Constructs_Max_Count];
          tmp_Outputs = new c_1D_Output*[Constructs_Max_Count];
          
          //Copy the addresses from the original array into the newly allocated one, then NULL the dendrite.
          for (int cou_NID=0;cou_NID<(Constructs_Max_Count - 1);cou_NID++){
               
               //Copy the information from the Constructs into the new array at the current index.
               delete tmp_Constructs[cou_NID];
               tmp_Constructs[cou_NID] = Constructs[cou_NID];
               
               tmp_Outputs[cou_NID] = NULL;
               tmp_Outputs[cou_NID] = Outputs[cou_NID];
               
               //NULL the current dendrite.
               Constructs[cou_NID] = NULL;
               Outputs[cou_NID] = NULL;
          }
               
          //Delete the allocated memory for the original Constructs array.
          delete [] Constructs;
          delete [] Outputs;
          
          //Sets the Constructs array to the tmp_Constructs.
          Constructs = tmp_Constructs;
          Outputs = tmp_Outputs;
          
          cout << "   ~Finished: increase_Construct_Array_Depth()"; cout.flush();
     }
     
     //Submit an input set to the given construct.
     void submit_Entry(int p_Construct_ID, u_IO_Data p_Input[], int p_Input_Length)//, int p_Focus[])
     {
          //If there is no input then return.
          if (p_Input_Length <= 0){ return; }
          
          cout << "\n\t submit_Entry(" << p_Construct_ID << ", " << p_Input_Length << ")"; cout.flush();
          //Submits the entry to the appropriate construct.
          Constructs[p_Construct_ID]->submit_CAN_Raw_Node_Array(p_Input, p_Input_Length);
          
          cout << "\n\t ~~Finished submit_Entry()"; cout.flush();
     }
     
     //Submit an input set to the given construct as a query.
     void submit_Query(int p_Construct_ID, u_IO_Data * p_Input, int p_Input_Length)
     {
          //Submits the entry to the appropriate construct.
          Constructs[p_Construct_ID]->submit_CAN_Raw_Node_Array_Query(p_Input, p_Input_Length);
     }
     
     //Charges the given construct x times.
     void charge_Construct(int p_Construct_ID, int p_Charging_Tier = 0, int p_Recharge = 0, int p_Output_Info = 0, int p_Output_Graphical = 0)
     {
          //Submits the entry to the appropriate construct.
          Constructs[p_Construct_ID]->charge_CAN_Given_Times_No_Network_Neutralize(p_Charging_Tier, 0, p_Recharge, p_Output_Info, p_Output_Graphical);
     }
     
     //Resets the IO of the given construct.
     void reset_Construct_Output(int p_CID)
     {
          Outputs[p_CID]->reset_IO_Nodes();
     }
     
     //Activates torch nodes for the given construct.
     void activate_Torch_Nodes(int p_CID)
     {
          Constructs[p_CID]->Node_Network.activate_Torch_Nodes();
     }
     
     //Neutralizes all of the constructs.
     void neutralize_Assembly()
     {
          //Loop through all of the constructs and neutralize them.
          for (int cou_CID=0;cou_CID<Constructs_Current_Count;cou_CID++){
               Constructs[cou_CID]->Node_Network.neutralize_All_Nodes();
          }
     }
     
     //Indexes all of the constructs for output.
     void index_Constructs(int p_X = 0, int p_Y = 0, int p_Separator = 1, int p_Aspect_Ratio = 1)
     {
          
          //tmp_HNID == temporary variable to hold the highest found NID.
          int tmp_HNID = 0;
          int tmp_HT = 0;
          int tmp_X = 0;
          int tmp_Y = 0;
          //Loop through all of the constructs and index their constituate nodes.
          for (int cou_CID=0;cou_CID<Constructs_Current_Count;cou_CID++){
               if (Constructs[cou_CID]->Construct_Type != 0){ continue; }
               tmp_HNID = Constructs[cou_CID]->Node_Network.index_Nodes((tmp_X + p_X), p_Y, p_Separator, p_Aspect_Ratio);
               tmp_HT = Constructs[cou_CID]->Node_Network.Number_Of_Tiers;
               if (tmp_HT > tmp_Y){ tmp_Y = tmp_HT; }
               tmp_X += tmp_HNID;
               tmp_X += 2;
          }
          
          tmp_Y *= (p_Aspect_Ratio * p_Separator);
          tmp_Y += (p_Aspect_Ratio * p_Separator) * 2;
          
          tmp_X = 0;
          for (int cou_CID=0;cou_CID<Constructs_Current_Count;cou_CID++){
               if (Constructs[cou_CID]->Construct_Type != 1){ continue; }
               tmp_X += Constructs[cou_CID]->Node_Network.index_Nodes((tmp_X + p_X), (p_Y + tmp_Y), p_Separator, p_Aspect_Ratio);
               tmp_X += 2;
          }
     }
     
     //Fires a given constructs CID
     
     //Fires all the constructs by CID
     
     //Fires all the constructs by their firing order. 
     
     
     //--     COMMANDS FOR INTERNAL CONTROL     --//
     
     
     
     //--     COMMANDS FOR USER CONTROL     --//

     //--     BUILDING COMMANDS
     

     
     //Sets the index of a given table.
     void set_Table_Index(u_IO_Data * p_Data, int p_Data_Length, int p_Target_Table, int p_Target_Index)
     {
          Tables.set_Data(p_Data, p_Data_Length, p_Target_Table, p_Target_Index);
     }
     
     //Gets the data from a given index.
     u_IO_Data * get_Table_Index(int p_Target_Table, int p_Target_Index)
     {
          return Tables.get_Data(p_Target_Table, p_Target_Index);
     }
     
     //Sets a tables index as a reference.
     void set_Table_Index_As_Reference(int p_Data_Table, int p_Data_Index, int p_Target_Table, int p_Target_Index)
     {
          Tables.set_Reference(p_Data_Table, p_Data_Index, p_Target_Table, p_Target_Index);
     }
     
          //Sets the table indexes starting at the given index to the given tables data.
     // For example, [0]Input_Table has 10 indexes. [1]Construct_Example_Table has data up to 
     // [12]. using include_Table_Data(1, 0, 12) would set indexes [12] through [22] to the 
     // Input_Tables data.
     void include_Table_Data(int p_Target_Table, int p_Data_Table, int p_Index)
     {
          
     }
     
     
     //--     EVAL COMMANDS
     
     //Updates all tables in all constructs and the assembly.
     void update_All_Tables()
     {
          update_All_Construct_Tables_From_Internal();
          update_Assembly_Tables();
     }
     
     //Updates the variables that a table is linked to inside of a construct.
     //Say you set the indexes in a constructs Settings_Table and want the constructs
     //settings to reflect the values you set, this function would do that.
     void update_Construct_Internal_From_Table(int p_CID = -1)
     {
          
     }
     
     //Updates the tables with the current data in the construct.
     //If you want a table, such as the settings_Table, to reflect current
     //values you would call this function. The table indexes will be updated
     //to reflect the current values.
     void update_Construct_Tables_From_Internal(int p_CID = -1)
     {
          if (p_CID == -1){ return; }
          if (p_CID < 0 || p_CID >= Constructs_Current_Count){ return; }
          Constructs[p_CID]->update_All_Tables_From_Internal();
     }
     
     //Updates all of the constructs tables.
     void update_All_Construct_Tables_From_Internal()
     {
          for (int cou_Index=0;cou_Index<Constructs_Current_Count;cou_Index++)
          {
               //If the current index does not actually have a construct, such as after a deletion, skip it.
               if (Constructs[cou_Index] == NULL){ continue; }
               
               update_Construct_Tables_From_Internal(cou_Index);
          }
     }
     
     //Updates the variables for the assembly using the settings_Table.
     void update_Assembly_Settings()
     {
          
     }
     
     //Updates teh assembly tables with the interal data.
     void update_Assembly_Tables()
     {
          update_Assembly_Settings_Table();
     }
     
     //Updates the assembly settings_Table
     void update_Assembly_Settings_Table()
     { 
          Tables.set_Data_Int(Constructs_Current_Count, Settings_Table, 0);
          Tables.set_Cell_Name("Construct_Current_Count", Settings_Table, 0);
     }
     
     //Resets all the IO tables.
     void reset_All_IO_Tables()
     {
          reset_Assembly_IO_Tables();
          reset_All_Constructs_IO_Tables();
     }
     
     //Resets the input tables for the assembly.
     void reset_Assembly_IO_Tables()
     {
          Tables.reset_Table(Input_Table);
          Tables.reset_Table(Output_Table);
     }
     
     //Resets all of the constructs IO tables.
     void reset_All_Constructs_IO_Tables()
     {
          for (int cou_Index=0;cou_Index<Constructs_Current_Count;cou_Index++)
          {
               reset_Construct_IO_Tables(cou_Index);
          }
     }
     
     //Reset a given constructs IO tables.
     void reset_Construct_IO_Tables(int p_Construct)
     {
          Constructs[p_Construct]->reset_IO_Tables();
     }
     
     //Resets all Input tables.
     void reset_All_I_Tables()
     {
          Tables.reset_Table(Input_Table);
          reset_All_Construct_I_Tables();
     }
     
     //Resets all construct input tables.
     void reset_All_Construct_I_Tables()
     {
          for (int cou_Index=0;cou_Index<Constructs_Current_Count;cou_Index++)
          {
               reset_Construct_I_Table(cou_Index);
          }
     }
     
     //Resets a given constructs Input table.
     void reset_Construct_I_Table(int p_Construct)
     {
          Constructs[p_Construct]->reset_I_Tables();
     }
     
     //Resets all Output tables.
     void reset_All_O_Tables()
     {
          Tables.reset_Table(Output_Table);
          reset_All_Construct_O_Tables();
     }
     
     //Resets all construct output tables.
     void reset_All_Construct_O_Tables()
     {
          for (int cou_Index=0;cou_Index<Constructs_Current_Count;cou_Index++)
          {
               reset_Construct_O_Table(cou_Index);
          }
     }
     
     //Resets a given constructs Input table.
     void reset_Construct_O_Table(int p_Construct)
     {
          Constructs[p_Construct]->reset_O_Tables();
     }
     
     //Reset a specified table.
     void reset_Table(int p_Table)
     {
          Tables.reset_Table(p_Table);
     }
     
     //Resets a specific table index.
     void reset_Table_Index(int p_Table, int p_Index)
     {
          Tables.reset_Table_Index(p_Table, p_Index);
     }
     
     //======================================
     //==     USER AVAILABLE FUNCTIONS     ==
     //======================================
     
     
     //Function list.
     //create_Construct()
     //delete_Construct()    
     
     
     //Adds a new construct to the assembly.
     void * new_Construct(string p_Construct_Name)
     {
          //Loops through all of the constructs, if it reaches an index set to NULL it creates the new construct in that position.
          for (int cou_Index=0;cou_Index<Constructs_Current_Count;cou_Index++)
          {
               if (Constructs[cou_Index] == NULL)
               {
                    //Creates the output for the construct that is to be created.
                    Outputs[cou_Index] = new c_1D_Output();
                    
                    //Creates the new construct, passes the adress of the output associated with this construct.
                    Constructs[cou_Index] = new c_Poly_CAN(cou_Index, &Tables, p_Construct_Name);
                    
                    return Constructs[cou_Index];
               }
          }
     }
     
     //Deletes a construct.
     void del_Construct(int p_CID = -1)
     {
          if (p_CID == -1){ return; }
          delete Constructs[p_CID];
          delete Outputs[p_CID];
     }
     
     void change_Construct_Setting(int p_CID, int p_Setting, u_IO_Data p_Setting_State)
     {
          
     }
     
     //build_Input(int p_CID)
     //query_Input(int p_CID)
     //query_Input_With_Build(int p_CID)
     
     //Evaluates a given construct.
     void build_Construct(int p_CID, int p_Method)
     {
          if (!bounds_CID(p_CID)){ return; }
          
          
          //cout << "\n\t\t c_Assembly::build_Construct(" << p_CID << ")"; 
          //cout << "\n bld_C[" << p_CID << "]";
          Constructs[p_CID]->build_Construct(p_Method);
          //cout << "\n bld_C[" << p_CID << "] ~Fin";
     }
     
     //Queries a construct, builds the current pattern as well.
     void query_Construct(int p_CID, bool p_flg_Force_Pattern, int p_Precision, int p_Method, float p_Depth)
     {
          if (!bounds_CID(p_CID)){ return; }
          
          if (!p_flg_Force_Pattern)
          {
               cout << " --Query_Construct_No_Build."; cout.flush();
               Constructs[p_CID]->query_Construct(p_Precision, p_Method, p_Depth);
          }
          else
          {
               cout << " --Query_Construct_With_Build."; cout.flush();
               Constructs[p_CID]->query_Construct_Force_Pattern(p_Precision, p_Method, p_Depth);
          }
          
     }
     
     //Discharges the torch nodes of a given construct.
     void discharge_Torch_Nodes(int p_CID)
     {
          Constructs[p_CID]->discharge_Torch_Nodes();
     }
     
     //Submit input set.
     void set_Input(u_IO_Data * p_Data, int p_Data_Length, int p_Table, int p_Index)
     {
          Tables.set_Data(p_Data, p_Data_Length, p_Table, p_Index);
     }
     
     //Submit input set as string.
     void set_Input(string p_Input, int p_Table, int p_Index)
     {
          Tables.set_Data(p_Input, p_Table, p_Index);
     }
     
     //    Using the new u_IO_Data flags in the table data the reference data can be stored in the flag table, 
     // leaving the data free to be set to the reference target data.
     void create_Connection(int p_CID_Input, int p_CID_Input_Index, int p_CID_Source, int p_CID_Source_Index)
     {
          
     }
     
     void delete_Connection(int p_CID_Input)
     {
          
     }
     
     
     //--     TABLE FUNCTIONS
     
     //Sorts a given table.
     void sort_Table_By_Flag(int p_Table, int p_Flag = 0)
     {
          cout << " ~Condition:" << Tables.sort_Table_By_Flag(p_Table, p_Flag);
     }
     
     void sort_All_Tables_By_Flag(int p_Flag = 0)
     {
          Tables.sort_All_Tables_By_Flag(p_Flag);
     }
     
     //--     OUTPUT FUNCTIONS
     
     //Output all of the construct networks.
     void output_All_Constructs()
     {
          //Indexes the constructs to prep them for output
          index_Constructs();
          
          //Loop through all of the constructs outputting their nodes.
          for (int cou_CID=0;cou_CID<Constructs_Current_Count;cou_CID++){
               Constructs[cou_CID]->Node_Network.output_All_Poly_Nodes();
          }
     }
     
     //Output all of the construct networks.
     void output_All_Constructs_Charged_Poly_Nodes()
     {
          //Indexes the constructs to prep them for output
          index_Constructs();
          
          //Loop through all of the constructs outputting their nodes.
          for (int cou_CID=0;cou_CID<Constructs_Current_Count;cou_CID++){
               Constructs[cou_CID]->Node_Network.output_All_Charged_Poly_Nodes();
          }
     }
     
     
     //Output all of the construct networks.
     void output_All_Construct_CANs()
     {    
          //Loop through all of the constructs outputting their nodes.
          for (int cou_CID=0;cou_CID<Constructs_Current_Count;cou_CID++){
               Constructs[cou_CID]->output_CAN();
          }
     }
     
     //Output all of the construct networks as full graphical.
     void output_All_Constructs_Full(int p_X, int p_Y, int p_Separator, int p_Aspect_Ratio)
     {
          system("CLS");
          //Indexes the constructs to prep them for output
          index_Constructs(p_X, p_Y, p_Separator, p_Aspect_Ratio );
          
          //Loop through all of the constructs outputting their nodes.
          for (int cou_CID=0;cou_CID<Constructs_Current_Count;cou_CID++){
               Constructs[cou_CID]->Node_Network.output_All_Poly_Nodes_Graphical_Absolute_Full_By_Tier();
          }
     }
     
     //Scribes all of the construct networks as full graphical.
     void scribe_All_Constructs_Full(ofstream * p_File, int p_X, int p_Y, int p_Separator, int p_Aspect_Ratio)
     {
          system("CLS");
          //Indexes the constructs to prep them for output
          index_Constructs(p_X, p_Y, p_Separator, p_Aspect_Ratio );
          
          //Loop through all of the constructs outputting their nodes.
          for (int cou_CID=0;cou_CID<Constructs_Current_Count;cou_CID++){
               Constructs[cou_CID]->Node_Network.scribe_All_Poly_Nodes_Graphical_Absolute_Full_By_Tier(p_File);
          }
     }
     
     //Output given construct network as full graphical.
     void output_Construct_Full(int p_CID, int p_X, int p_Y, int p_Separator, int p_Aspect_Ratio)
     {
          //Indexes the constructs to prep them for output
          index_Constructs();
          
          Constructs[p_CID]->Node_Network.output_All_Poly_Nodes_Graphical_Absolute_Full();
     }
     
     //Outputs the 1d_Ouputs for a given construct.
     void output_1D_Output(int p_CID)
     {
          Constructs[p_CID]->Output_Address.output_C();
     }
     
     //Outputs the 1d_Ouputs for a given construct.
     void output_All_1D_Output()
     {
          for (int cou_Index=0;cou_Index<Constructs_Current_Count;cou_Index++)
          {
               output_1D_Output(cou_Index);
          }
     }

     //Outputs the nodes for a given construct.
     void output_Construct_Nodes(int p_CID)
     {
          Constructs[p_CID]->Node_Network.output_All_Poly_Nodes();
     }
     
     //Outputs the input and output tables of the assembly.
     void output_IO()
     {
          cout << "\n\n\t\t~";
          Tables.output_Table(0, 1);
          cout << "\n\n\t\t~";
          Tables.output_Table(1, 1);
     }
     
     
     
     
     
     
     
     
     
     //--     FILE FUNCTIONS
     
     void load_Script(char p_Script[])
     {
          ifstream f_Script;
          f_Script.open(p_Script);
          cout << "\n\n\t Opening File: \"" << p_Script << "\" ~" << f_Script.is_open();
          char tmp_Char[100];
          u_IO_Data tmp_Data[100];
          int tmp_Int[10];
          
          if (f_Script.is_open())
          {
               //Start at the beggining.
               f_Script.seekg(0);
               tmp_Int[0] = 0;
               
               cout << "\n\n\t File:" << p_Script << " has been opened...";
               while (!f_Script.eof())
               {
                    for (int cou_Index=0;cou_Index<100;cou_Index++)
                    {
                         tmp_Char[cou_Index] = char (0);
                    }
                    
                    f_Script.get(tmp_Char[tmp_Int[0]]);
                    tmp_Data[tmp_Int[0]].I = int (tmp_Char[tmp_Int[0]]);
                    
                    //Gatheres the query.
                    while (tmp_Char[tmp_Int[0]] != '\n')
                    {
                         tmp_Int[0]++;
                         f_Script.get(tmp_Char[tmp_Int[0]]);
                         tmp_Data [tmp_Int[0]].I = int (tmp_Char[tmp_Int[0]]);
                         if (tmp_Char[tmp_Int[0]] == '\n'){ break; }
                         if (f_Script.eof()){ break; }
                    }
                    
                    push_Command(tmp_Data, tmp_Int[0]);
                    
                    for (int cou_Index=0;cou_Index<(tmp_Int[0]+1);cou_Index++)
                    {
                         cout << tmp_Char[cou_Index];
                         tmp_Char[cou_Index] = char (0);
                         tmp_Data[cou_Index].I = 0;
                    }
                    
                    tmp_Int[0] = 0;
                         
               }
          }
          else
          {
               cout << "\n\n\t File:" << p_Script << " has NOT been opened...";
          }
          
          f_Script.close();
     }
     
     void load_Assembly(char p_Assembly[])
     {
          ifstream f_Script;
          f_Script.open(p_Assembly);
          cout << "\n\n\t Opening File: \"" << p_Assembly << "\" ~" << f_Script.is_open();
          char tmp_Char[1000];
          u_IO_Data tmp_Data[1000];
          int tmp_Int[10];
          
          if (f_Script.is_open())
          {
               //Start at the beggining.
               f_Script.seekg(0);
               tmp_Int[0] = 0;
               
               cout << "\n\n\t File:" << p_Assembly << " has been opened...";
               while (!f_Script.eof())
               {
                    for (int cou_Index=0;cou_Index<100;cou_Index++)
                    {
                         tmp_Char[cou_Index] = char (0);
                    }
                    
                    f_Script.get(tmp_Char[tmp_Int[0]]);
                    tmp_Data[tmp_Int[0]].I = int (tmp_Char[tmp_Int[0]]);
                    
                    //Gatheres the query.
                    while (tmp_Char[tmp_Int[0]] != '\n')
                    {
                         tmp_Int[0]++;
                         f_Script.get(tmp_Char[tmp_Int[0]]);
                         tmp_Data [tmp_Int[0]].I = int (tmp_Char[tmp_Int[0]]);
                         if (tmp_Char[tmp_Int[0]] == '\n'){ break; }
                         if (f_Script.eof()){ break; }
                    }
                    
                    evaluate_Command_Text(tmp_Data);
                    
                    
                    for (int cou_Index=0;cou_Index<(tmp_Int[0]+1);cou_Index++)
                    {
                         //cout << tmp_Char[cou_Index];
                         tmp_Char[cou_Index] = char (0);
                         tmp_Data[cou_Index].I = 0;
                    }
                    
                    tmp_Int[0] = 0;
                         
               }
               
               //set_Node_References();
          }
          else
          {
               cout << "\n\n\t File:" << p_Assembly << " has NOT been opened...";
          }
          
          f_Script.close();
     }
     
     
     //Sets all of the node references.
     void set_Node_References()
     {
          cout << "\n\n Setting raisin node references..."; cout.flush();
          
          cout << "\n\t Looping through watery constructs..."; cout.flush();
          
          //Loop through all of the constructs setting the node references.
          for (int cou_Construct=0;cou_Construct<Constructs_Current_Count;cou_Construct++)
          {
               cout << "\n\t\t Looping through tiny Nodes in Construct[" << cou_Construct << "]..."; cout.flush();
               
               //Loop through all of the nodes in the current construct.
               for (int cou_NID=0;cou_NID<Constructs[cou_Construct]->Node_Network.Nodes_Current_Count;cou_NID++)
               {
                    //cout << "\n\t\t Looping through old Dendrites in Node[" << cou_NID << "]"; cout.flush();
                    
                    //Loop through all of the dendrites gathering the references to the nodes.
                    for (int cou_D=0;cou_D<Constructs[cou_Construct]->Node_Network.Nodes[cou_NID]->Dendrite_Count;cou_D++)
                    {
                         //cout << " (" << cou_D << ")"; cout.flush();
                         
                         //Get the reference CID.
                         tmp_int_File[0] = Constructs[cou_Construct]->Node_Network.Nodes[cou_NID]->Dendrite_Target_Reference[cou_D].Construct;
                         //cout << tmp_int_File[0] << "-";
                         
                         //Get the reference UNID.
                         tmp_int_File[1] = Constructs[cou_Construct]->Node_Network.Nodes[cou_NID]->Dendrite_Target_Reference[cou_D].Node;
                         //cout << tmp_int_File[1]; cout.flush();
                         
                         //Gets the reference to the node.
                         tmp_data_File[0].NR = Constructs[tmp_int_File[0]]->Node_Network.Nodes[tmp_int_File[1]];
                         
                         //Sets the current dendrites reference.
                         Constructs[cou_Construct]->Node_Network.Nodes[cou_NID]->Dendrites[cou_D] = tmp_data_File[0].NR;
                    }
                    
                    //cout << "\n\t\t Looping through Axons in Node[" << cou_NID << "]"; cout.flush();
                    
                    //Loop through all of the axons gathering the references to the nodes as well as the synaptic indexes.
                    for (int cou_A=0;cou_A<Constructs[cou_Construct]->Node_Network.Nodes[cou_NID]->Axon_Count;cou_A++)
                    {
                         //cout << " (" << cou_A << ")"; cout.flush();
                         
                         //Get the reference CID.
                         tmp_int_File[0] = Constructs[cou_Construct]->Node_Network.Nodes[cou_NID]->Axon_Target_Reference[cou_A].Construct;
                         //cout << tmp_int_File[0] << "-"; cout.flush();
                         
                         //Get the reference UNID.
                         tmp_int_File[1] = Constructs[cou_Construct]->Node_Network.Nodes[cou_NID]->Axon_Target_Reference[cou_A].Node;
                         //cout << tmp_int_File[1] << "&"; cout.flush();
                         
                         //Get the synaptic index.
                         tmp_int_File[2] = Constructs[cou_Construct]->Node_Network.Nodes[cou_NID]->Axon_Target_Reference[cou_A].Target_Synaptic_Index;
                         //cout << tmp_int_File[2]; cout.flush();
                         
                         //Gets the reference to the node.
                         tmp_data_File[0].NR = Constructs[tmp_int_File[0]]->Node_Network.Nodes[tmp_int_File[1]];
                         
                         //Sets the current axon reference.
                         Constructs[cou_Construct]->Node_Network.Nodes[cou_NID]->Axons[cou_A] = tmp_data_File[0].NR;
                         Constructs[cou_Construct]->Node_Network.Nodes[cou_NID]->Axon_Target_Synaptic_Index[cou_A] = tmp_int_File[2];
                    }
                    
               }
               
          }
          
     }
     
     //Saves the assembly to a file.
     void save_Assembly(char p_Filename[])
     {
          /*
          int tmp_Construct_Input_Table = 0;
          int tmp_Target_Table = 0;
          int tmp_Target_Index = 0;
          */
          
          //Index the construct before saving.
          index_Constructs();
          
          ofstream Save_File;
          Save_File.open(p_Filename);
          
          //Save the constructs.
          for (int cou_Index=0;cou_Index<Constructs_Current_Count;cou_Index++)
          {
               Save_File << "crt " << Constructs[cou_Index]->Name << " " << Constructs[cou_Index]->Construct_Type << "\n";
               Save_File.flush();
               cout << Constructs[cou_Index]->Name;
               
               /*
               tmp_Construct_Input_Table = Constructs[cou_Index]->Input_Table;
               
               for (int cou_I=0;cou_I<Tables.get_Table_Item_Count(tmp_Construct_Input_Table);cou_I++)
               {
                    if (Tables.get_isReference(tmp_Construct_Input_Table, cou_I))
                    {
                         Save_File << "con ";
                         Save_File.flush();
                         
                         tmp_Target_Table = (Tables.get_Flag(tmp_Construct_Input_Table, cou_I, 7));
                         tmp_Target_Index = (Tables.get_Flag(tmp_Construct_Input_Table, cou_I, 8));
                         for (int cou_Place=0;cou_Place<(4 - get_int_Place(tmp_Construct_Input_Table));cou_Place++)
                         {
                              Save_File << '0';
                              Save_File.flush();
                         }
                         Save_File << tmp_Construct_Input_Table << ' ';
                         Save_File.flush();
                         
                         for (int cou_Place=0;cou_Place<(4 - get_int_Place(cou_I));cou_Place++)
                         {
                              Save_File << '0';
                              Save_File.flush();
                         }
                         Save_File << cou_I << ' ';
                         Save_File.flush();
                         
                         for (int cou_Place=0;cou_Place<(4 - get_int_Place(tmp_Target_Table));cou_Place++)
                         {
                              Save_File << '0';
                              Save_File.flush();
                         }
                         Save_File << tmp_Target_Table << ' ';
                         Save_File.flush();
                         
                         for (int cou_Place=0;cou_Place<(4 - get_int_Place(tmp_Target_Index));cou_Place++)
                         {
                              Save_File << '0';
                              Save_File.flush();
                         }
                         Save_File << tmp_Target_Index << '\n';
                         Save_File.flush();
                         
                         
                              //set_Table_Index_As_Reference(int p_Data_Table, int p_Data_Index, int p_Target_Table, int p_Target_Index)
                    }
                     
               }
                * */
               
          }
          
          //Save the tables.
          //Save_File << Tables.get_Table_Name()
          
          //Save the nodes.
          for (int cou_Index=0;cou_Index<Constructs_Current_Count;cou_Index++)
          {
               //Prep the network nodes to gather the index references.
               Constructs[cou_Index]->Node_Network.prep_For_Save();
               
               for (int cou_NID=0;cou_NID<Constructs[cou_Index]->Node_Network.Nodes_Current_Count;cou_NID++)
               {
                    Save_File << "\nnode";
                    
                    //The current construct that houses the node.
                    Save_File << ' ';
                    for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->CID));cou_Place++)
                    {
                         Save_File << '0';
                         Save_File.flush();
                    }
                    Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->CID;
                    
                    
                    //The universal node NID.
                    Save_File << ' ';
                    for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->UNID));cou_Place++)
                    {
                         Save_File << '0';
                         Save_File.flush();
                    }
                    Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->UNID;
                    
                    
                    //Reinforcement counter.
                    Save_File << ' ';
                    for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Reinforcement_Counter));cou_Place++)
                    {
                         Save_File << '0';
                         Save_File.flush();
                    }
                    Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Reinforcement_Counter;
                    
                    
                    //flg_Treetop;
                    Save_File << ' ';
                    for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->flg_Treetop));cou_Place++)
                    {
                         Save_File << '0';
                         Save_File.flush();
                    }
                    Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->flg_Treetop;
                    
                    
                    //Tier.
                    Save_File << ' ';
                    for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Tier));cou_Place++)
                    {
                         Save_File << '0';
                         Save_File.flush();
                    }
                    Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Tier;
                    
                    
                    //flg_Torch.
                    Save_File << ' ';
                    for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->flg_Torch));cou_Place++)
                    {
                         Save_File << '0';
                         Save_File.flush();
                    }
                    Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->flg_Torch;
                    
                    
                    //X.
                    Save_File << ' ';
                    for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->X));cou_Place++)
                    {
                         Save_File << '0';
                         Save_File.flush();
                    }
                    Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->X;
                    cout << "\n\t [" << cou_Index << "]-[" << cou_NID << "]" << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->X;
                    
                    
                    //Y.
                    Save_File << ' ';
                    for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Y));cou_Place++)
                    {
                         Save_File << '0';
                         Save_File.flush();
                    }
                    Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Y;
                    
                    
                    //State nodes.
                    //if (Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->flg_State)
                    //{
                         Save_File << "\nsta";
                         
                         Save_File << ' ';
                         for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->State_Length));cou_Place++)
                         {
                              Save_File << '0';
                              Save_File.flush();
                         }
                         Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->State_Length;
                         Save_File.flush();
                         
                         
                         Save_File << ' ';
                         Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->flg_State_Reference;
                         
                         Save_File << ' ';
                         Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->flg_State;
                         
                         
                         for (int cou_State=0;cou_State<Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->State_Length;cou_State++)
                         {
                              
                              Save_File << ' ';
                              
                              for (int cou_I=0;cou_I<4;cou_I++)
                              {
                                   Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->State[cou_State].C[cou_I];
                              }
                              Save_File.flush();
                         
                         }
                         
                    //}
                    
                    //Dendrites
                    if (Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Dendrite_Count)
                    {
                         
                         for (int cou_D=0;cou_D<Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Dendrite_Count;cou_D++)
                         {
                              Save_File << "\nden";
                              cout << "\n " << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Dendrite_Target_Reference[cou_D].Construct << "-" << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Dendrite_Target_Reference[cou_D].Node;
                              
                              // Save_File << " " << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Dendrite_Target_Reference[cou_D].Construct << "-" << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Dendrite_Target_Reference[cou_D].Node;
                         
                              Save_File << ' ';
                              for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Dendrite_Target_Reference[cou_D].Construct));cou_Place++)
                              {
                                   Save_File << '0';
                                   Save_File.flush();
                              }
                              Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Dendrite_Target_Reference[cou_D].Construct;
                              Save_File.flush();
                         
                              Save_File << ' ';
                              for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Dendrite_Target_Reference[cou_D].Node));cou_Place++)
                              {
                                   Save_File << '0';
                                   Save_File.flush();
                              }
                              Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Dendrite_Target_Reference[cou_D].Node;
                              Save_File.flush();
                         }
                    }
                         
                    //Axons
                    if (Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Axon_Count)
                    {
                              
                         
                         for (int cou_D=0;cou_D<Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Axon_Count;cou_D++)
                         {
                              Save_File << "\naxo";
                              cout << "\n " << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Axon_Target_Reference[cou_D].Construct << "-" << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Axon_Target_Reference[cou_D].Node;
                         
                              // Save_File << " " << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Axon_Target_Reference[cou_D].Construct << "-" << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Axon_Target_Reference[cou_D].Node;
                         
                              Save_File << ' ';
                              for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Axon_Target_Reference[cou_D].Construct));cou_Place++)
                              {
                                   Save_File << '0';
                                   Save_File.flush();
                              }
                              Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Axon_Target_Reference[cou_D].Construct;
                              Save_File.flush();
                              
                              Save_File << ' ';
                              for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Axon_Target_Reference[cou_D].Node));cou_Place++)
                              {
                                   Save_File << '0';
                                   Save_File.flush();
                              }
                              Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Axon_Target_Reference[cou_D].Node;
                              Save_File.flush();
                              
                              Save_File << ' ';
                              for (int cou_Place=0;cou_Place<(4 - get_int_Place(Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Axon_Target_Reference[cou_D].Target_Synaptic_Index));cou_Place++)
                              {
                                   Save_File << '0';
                                   Save_File.flush();
                              }
                              Save_File << Constructs[cou_Index]->Node_Network.Nodes[cou_NID]->Axon_Target_Reference[cou_D].Target_Synaptic_Index;
                              Save_File.flush();
                         }
                    }
               }
          }
          
          //Set Node References.
          Save_File << "\nnrset";
          
          //Close the file.
          Save_File.close();
     }
     
     
     
     int get_Construct_Input_Table(int p_Construct)
     {
          return Constructs[p_Construct]->Input_Table;
     }
     
     int get_Construct_Output_Table(int p_Construct)
     {
          return Constructs[p_Construct]->Output_Table;
     }
     
     int get_Construct_Setting_Table(int p_Construct)
     {
          return Constructs[p_Construct]->Settings_Table;
     }
     
     
     
     //--      COMMAND PROCESSOR
     
     
     //Evaluates the commands located in the command table.
     void run()
     {
          //Runs the command table script.
          evaluate_Command_Table();
     }
     
     void evaluate_Command_Table(int p_EIP = 0)
     {
          cout << "\n\t Evaluating the command table...";
          eval(Command_Table);
     }
     
     void eval(int p_Table, int p_EIP = 0)
     {
          
          //cout << "\n\t\t Command_Count:" << Tables.get_Table_Item_Count(p_Table);
          if (p_EIP <= 0){ p_EIP = 0; }
          //Loop through the table and evaluate each index.
          for(int cou_EIP=p_EIP;cou_EIP<Tables.get_Table_Item_Count(p_Table);cou_EIP++)
          {
               //The 0 index is the one holding the abstract function header.
               //evaluate_Command(Tables.get_Data_Item_Int(p_Table, 0), cou_EIP);
               cou_EIP += evaluate_Command_Text(Tables.get_Data(p_Table, cou_EIP));
          }
          //system("pause > NULL");
     }
     
     
     
     void evaluate_Command(int p_Command, int p_Command_Address)
     {
          switch (p_Command)
          {
               case 20:
                    //Temporary string for the name.
                    string tmp_Name;
                    
                    //Create the temporary array to gather the characters into for the name.
                    char * tmp_Char_Name;
                    tmp_Char_Name = new char[Tables.get_Data_Item_Int(Command_Table, p_Command_Address, 1)];
                    
                    //Gets all of the characters in the name, the first two characters in the index are skipped as they are
                    //control characters and not part of the name. The [1] index is the length of the name.
                    for (int cou_Index=2;cou_Index<(Tables.get_Data_Item_Int(Command_Table, p_Command_Address, 1) + 2);cou_Index++)
                    {
                         //Gets the current character from the table index.
                         tmp_Char_Name[cou_Index - 2] = char (Tables.get_Data_Item_Int(Command_Table, p_Command_Address, cou_Index));
                    }
                    
                    //Sets the string to the gathered tmp_Char_Name.
                    tmp_Name = tmp_Char_Name;
                    
                    //Cleanup pointers.
                    delete [] tmp_Char_Name;
                    
                    //Create the new construct with the gathered name.
                    create_New_Construct(tmp_Name);
                    break;
          }
     }
     
     /*
     int evaluate_Command_Text(char p_Command_Text[])
     {
          u_IO_Data * tmp_Command_Text;
          int tmp_int = 0;
          
          //Get the length of the command text string.
          while(p_Command_Text[tmp_int] != ';')
          {
               tmp_int++;
          }
          
          tmp_Command_Text = new u_IO_Data[tmp_int];
          
          //Gather the string.
          for (int cou_Index=0;cou_Index<tmp_int;cou_Index++)
          {
               tmp_Command_Text[cou_Index].I = int (p_Command_Text[cou_Index]);
          }
          
          evaluate_Command_Text(tmp_Command_Text);
          
          return 0;
          
     }
     */
     
     //Runs a loop allowing manipulation of the assembly through direct command text input.
     //Mainly used for debugging purposes.
     void CMD()
     {
          char tmp_Command_Text_Input[1000];
          u_IO_Data tmp_Command_Text_Input_Data[1000];
          
          for (int cou_Index=0;cou_Index<1000;cou_Index++)
          {
               tmp_Command_Text_Input[cou_Index] = char (0);
          }
          
          
          //Gather the command text and submit it to the assembly.
          do
          {
               
               //Clear the input.
               for (int cou_Index=0;cou_Index<1000;cou_Index++)
               {
                    tmp_Command_Text_Input[cou_Index] = '\n';
               }
               
               
               //Gather the submission.
               cout << "\n\n\t Enter Command Text:";
               cin.getline(tmp_Command_Text_Input, sizeof(tmp_Command_Text_Input));
               
               //Reset the data array.
               for (int cou_Index=0;cou_Index<1000;cou_Index++)
               {
                    tmp_Command_Text_Input_Data[cou_Index].I = 0;
               }
               
               //Read the array into the data array.
               for (int cou_Index=0;cou_Index<1000;cou_Index++)
               {
                    tmp_Command_Text_Input_Data[cou_Index].I = int (tmp_Command_Text_Input[cou_Index]);
               }
               
               //Submit the gathered command text.
               evaluate_Command_Text(tmp_Command_Text_Input_Data);
               
          }
          while (tmp_Command_Text_Input[0] != '~');
     }
     
     //The p_Command holds the command and the arguments.
     void set_Command(int p_Command_Index, u_IO_Data p_Command[], int p_Command_Count)
     {
            Tables.set_Data(p_Command, p_Command_Count, Command_Table, p_Command_Index);
     }
     
     //Pushes a command into the command table.
     void push_Command(u_IO_Data p_Command[], int p_Command_Length)
     {
          Tables.push(p_Command, p_Command_Length, Command_Table);
     }
     
     void set_CMD(int p_Command_Table, int p_Command_Index, u_IO_Data p_Command[], int p_Command_Count)
     {
          Tables.set_Data(p_Command, p_Command_Count, p_Command_Table, p_Command_Index);
     }
     
     //Pushy, pushy.
     void push_CMD(int p_Command_Table, u_IO_Data p_Command[], int p_Command_Count)
     {
          Tables.push(p_Command, p_Command_Count, p_Command_Table);
     }
     
     //Sets a string input.
     void set(int p_Input, string p_String)
     {
          set_Input(p_String, 0, p_Input);
     }
     
     //Sets a character input.
     void set_Char(int p_Input, char p_Data[])
     {
          //cout << "set_Char:\"" << p_Data << "\"";
          int tmp_Int = 0;
          
          //Gather the string length.
          while (p_Data[tmp_Int] != char (0))
          {
               tmp_Int++;
          }
          
          //cout << "\n\t tmp_Int:" << tmp_Int;
          
          //Create the tmp_Data
          u_IO_Data * tmp_Data;
          tmp_Data = new u_IO_Data[tmp_Int];
          
          //Gather the data.
          for (int cou_Index=0;cou_Index<tmp_Int;cou_Index++)
          {
               tmp_Data[cou_Index].I = int (p_Data[cou_Index]);
               //set_Input(u_IO_Data * p_Data, int p_Data_Length, int p_Table, int p_Index)
          }
          
          //Set the input.
          set_Input(tmp_Data, tmp_Int, 0, p_Input);
           //set_Input(u_IO_Data * p_Data, int p_Data_Length, int p_Table, int p_Index)
          
          //Cleanup.
          delete [] tmp_Data;
     }
     
     //Will put in a push_Float & push_Etc.
     
     //Sets an int input.
     void set_Int(int p_Input, int p_Data)
     {
          
          //Create the tmp_Data
          u_IO_Data tmp_Data[25];
          int tmp_Int;
          
          //Get the int.
          //tmp_Data->I = p_Data;
          
          //Read the int into the data array.
          tmp_Int = int2data(p_Data, tmp_Data);
          
          //Set the input.
          set_Input(tmp_Data, tmp_Int, 0, p_Input);
           //set_Input(u_IO_Data * p_Data, int p_Data_Length, int p_Table, int p_Index)
     }
     
     //Sets a float input.
     void set_Float(int p_Input, float p_Data)
     {
          
          //Create the tmp_Data
          u_IO_Data tmp_Data[25];
          
          //Create teh temporary variable to hold the length of the input.
          int tmp_Int;
          
          tmp_Int = float2data(p_Data, tmp_Data);
          
          //Get the float.
          //tmp_Data->F = p_Data;
          
          //Set the input.
          set_Input(tmp_Data, tmp_Int, 0, p_Input);
          
     }
     
     
     //Runs the CMD_Submit table.
     void submit()
     {
          eval(Submission_Cmd_Table);
     }
     
     //Runs the CMD_Query table.
     void query()
     {
          eval(Query_Cmd_Table);
     }
     
     
     //The autobuild function.
     
     
     
     
     //BOUNDS CHECKING
     
     bool bounds_CID(int p_CID)
     {
          if (p_CID == -1){ cout << "\n\t ERROR!: Assembly::build_Construct -p_CID == -1"; system("PAUSE > NULL"); return 0; }
          if (p_CID < 0 ){ cout << "\n\t ERROR!: Assembly::build_Construct -p_CID < 0"; system("PAUSE > NULL"); return 0; }
          if (p_CID >= Constructs_Current_Count){ cout << "\n\t ERROR!: Assembly::build_Construct -p_CID >= Constructs_Current_Count"; system("PAUSE > NULL"); return 0; }
          return 1;
     }
     
     //This allows for string literals to be used as command text input instead of the u_IO_Data.
     int evaluate_Command_Text(string p_Command_Text)
     {
          //If the string is of 0 length return to avoid bounding errors.
          cout << "\n\n\n\t p_Command_Text.size()" << p_Command_Text.size(); 
          if (p_Command_Text.size() == 0){ return 0; }
          
          //Setup the temporary command text.
          int tmp_Int = 0; //for returning.
          char * tmp_Char;
          u_IO_Data * tmp_Data;
          tmp_Data = new u_IO_Data[p_Command_Text.size() + 1];
          tmp_Char = new char[p_Command_Text.size() + 1];
          
          //Gather the string into the character array.
          strcpy(tmp_Char, p_Command_Text.c_str());
          
          //Gather the text into the tmp_Data.
          for (int cou_Index=0;cou_Index<int (p_Command_Text.size());cou_Index++)
          {
               tmp_Data[cou_Index].I = int (tmp_Char[cou_Index]);
          }
          
          //Submit the tmp_Data.
          tmp_Int = evaluate_Command_Text(tmp_Data);
          
          //Cleanup.
          delete [] tmp_Char;
          delete [] tmp_Data;
          
          return tmp_Int;
     }
     
     /*
     //This allows for character strings to be used for command text input instead of the typical u_IO_Data.
     int evaluate_Command_Text(char * p_Command_Text)
     {
          int tmp_Int = 0;
          u_IO_Data * tmp_Data;
          
          //Get the length of the character string.
          while (p_Command_Text[tmp_Int] != '\n')
          {
               tmp_Int++;
          }
          
          //Initialize the tmp_Data.
          tmp_Data = new u_IO_Data[tmp_Int];
          
          //Gather the character string into the temporary array.
          for (int cou_Index=0;cou_Index<tmp_Int;cou_Index++)
          {
               tmp_Data[cou_Index].I = int (p_Command_Text[cou_Index]);
          }
          
          //Submit the command text.
          tmp_Int = evaluate_Command_Text(tmp_Data);
          
          //Cleanup pointers.
          delete [] tmp_Data;
          
          //Done
          return tmp_Int;
     }
     */
     
     //The reason this is at the end is because it is edited on a regular basis and this helps in locating the function.
     int evaluate_Command_Text(u_IO_Data * p_Command_Text)
     {
           
          
          //cout << "\n\t evaluate_Command_Text()";
          
          if (p_Command_Text == NULL){ cout << "\n\t\t\t NO COMMAND GIVEN!"; return 0; }
          //cout << "~" << char (p_Command_Text[0].I) << char (p_Command_Text[1].I) << char (p_Command_Text[2].I) << char (p_Command_Text[3].I) << char (p_Command_Text[4].I);
          
          char tmp_Name[100];
          
          //Attempt to fix the name fuckup bug with tables.
          for (int cou_Index=0;cou_Index<100;cou_Index++)
          {
               tmp_Name[cou_Index] = char (0);
          }
          string tmp_Name_String;
          
          int tmp_Int[10];
          //float tmp_Float[10];
          //u_IO_Data tmp_Data[100];
          
          //Create new construct.
          if (comp_CMD("crt", p_Command_Text))
          {
               
                    //Gather the name of the construct to create.
                    tmp_Int[0] = 4;
                    cout << "\n\n   ~Gathering construct name: {";
                    while(char (p_Command_Text[tmp_Int[0]].I) != '\n')
                    {
                         tmp_Name[tmp_Int[0] - 4] = char (p_Command_Text[tmp_Int[0]].I);
                         cout << tmp_Name[tmp_Int[0] - 4];
                         tmp_Int[0]++;
                    }
                    cout << "}";
                    
                    //Set the string name.
                    tmp_Name_String = tmp_Name;
                    
                    tmp_Int[1] = (p_Command_Text[tmp_Int[0] +1].I - 48);
                    cout << "\n    create_New_Construct()"; cout.flush();
                    create_New_Construct(tmp_Name_String, tmp_Int[1]);
                    
                    return 0;
          }
         
          //raw construct creation.
          if (comp_CMD("raw", p_Command_Text))
          {
               
                    //Gather the name of the construct to create.
                    tmp_Int[0] = 4;
                    cout << "\n\n   ~Gathering construct name: {";
                    while(char (p_Command_Text[tmp_Int[0]].I) != '\n')
                    {
                         tmp_Name[tmp_Int[0] - 4] = char (p_Command_Text[tmp_Int[0]].I);
                         cout << tmp_Name[tmp_Int[0] - 4];
                         tmp_Int[0]++;
                    }
                    cout << "}";
                    
                    //Set the string name.
                    tmp_Name_String = tmp_Name;
                    
                    cout << "\n    create_New_Raw_Construct()"; cout.flush();
                    create_New_Construct(tmp_Name_String, 0);
                    
                    return 0;
          }
          
          //msc creation.
          
          //raw construct creation.
          if (comp_CMD("msc", p_Command_Text))
          {
               
                    //Gather the name of the construct to create.
                    tmp_Int[0] = 4; 
                    cout << "\n\n   ~Gathering construct name: {";
                    while(char (p_Command_Text[tmp_Int[0]].I) != '\n')
                    {
                         tmp_Name[tmp_Int[0] - 4] = char (p_Command_Text[tmp_Int[0]].I);
                         cout << tmp_Name[tmp_Int[0] - 4];
                         tmp_Int[0]++;
                    }
                    cout << "}";
                    
                    //Set the string name.
                    tmp_Name_String = tmp_Name;
                    
                    cout << "\n    create_New_Raw_Construct()"; cout.flush();
                    create_New_Construct(tmp_Name_String, 1);
                    
                    return 0;
          }
          
          
          if (comp_CMD("load", p_Command_Text))
          {
               
                    //Gather the name of the construct to create.
                    tmp_Int[0] = 5;
                    cout << "\n\n   ~Gathering filename: {";
                    while(char (p_Command_Text[tmp_Int[0]].I) != '\n')
                    {
                         tmp_Name[tmp_Int[0] - 5] = char (p_Command_Text[tmp_Int[0]].I);
                         cout << tmp_Name[tmp_Int[0] - 5];
                         tmp_Int[0]++;
                    }
                    cout << "}";
                    
                    tmp_Int[1] = p_Command_Text[tmp_Int[0] +1].I;
                    //create_New_Construct(tmp_Name_String, tmp_Int[1]);
                    load_Assembly(tmp_Name);
                    
                    return 0;
          }
          
          
          if (comp_CMD("save", p_Command_Text))
          {
               
                    //Gather the name of the construct to create.
                    tmp_Int[0] = 5;
                    cout << "\n\n   ~Gathering filename: {";
                    while(char (p_Command_Text[tmp_Int[0]].I) != '\n')
                    {
                         tmp_Name[tmp_Int[0] - 5] = char (p_Command_Text[tmp_Int[0]].I);
                         cout << tmp_Name[tmp_Int[0] - 5];
                         tmp_Int[0]++;
                    }
                    cout << "}";
                    
                    tmp_Int[1] = p_Command_Text[tmp_Int[0] +1].I;
                    //create_New_Construct(tmp_Name_String, tmp_Int[1]);
                    save_Assembly(tmp_Name);
                    
                    return 0;
          }
          
          
          
          
          if (comp_CMD("set", p_Command_Text))
          {
                    
                    //The input to use.
                    //ASCII to int.
                    tmp_Int[0] = char2int((p_Command_Text + 4), 4);
                    
                    //Gather the submission.
                    tmp_Int[1] = 9;
                    
                    cout << "\n\n   ~Gathering submission: {";
                    while(char (p_Command_Text[tmp_Int[1]].I) != '\n')
                    {
                         //tmp_Name[tmp_Int[1] - 4] = char (p_Command_Text[tmp_Int[1]].I);
                         cout << char (p_Command_Text[tmp_Int[1]].I);
                         tmp_Int[1]++;
                    }
                    
                    //Do not pass the '\n'.
                    tmp_Int[1]--;
                    cout << "}";
                    
                    if (flg_Autobuild == 1 && !flg_Autobuild_Input_Has_Construct[tmp_Int[0]])
                    {
                         
                         //Assemble the name.
                         char tmpc_Input_Name[4];
                         string tmp_Input_Name;
                         
                         tmpc_Input_Name[0] = char (p_Command_Text[4].I);
                         tmpc_Input_Name[1] = char (p_Command_Text[5].I); 
                         tmpc_Input_Name[2] = char (p_Command_Text[6].I);
                         tmpc_Input_Name[3] = char (p_Command_Text[7].I);
                         
                         //The concatination station.
                         tmp_Input_Name = "Raw_";
                         tmp_Input_Name += tmpc_Input_Name;
                          
                         create_New_Construct(tmp_Input_Name, 0);
                         flg_Autobuild_Input_Has_Construct[tmp_Int[0]] = true;
                         
                         //set_Table_Index_As_Reference(int p_Data_Table, int p_Data_Index, int p_Target_Table, int p_Target_Index)
                         
                         //Set the input for the construct to the corresponding Assembly input.
                         set_Table_Index_As_Reference(get_Construct_Input_Table(Last_Created_Construct), 0, 0, tmp_Int[0]);
                         
                         //Setup the input for the Assembly output.
                         set_Table_Index_As_Reference(1, tmp_Int[0], get_Construct_Output_Table(Last_Created_Construct), 0);
                         
                         //Setup the MSC connections.
                         set_Table_Index_As_Reference(get_Construct_Input_Table(0), tmp_Int[0], get_Construct_Setting_Table(Last_Created_Construct), 9);
                         
                         //Generate the command script for the newly created construct.
                         
                         //--Bld Table
                         
                         //Clear the strings.
                         tmp_Command_String.clear();
                         clear_Char(tmp_Command_Char, 25);
                         clear_u_IO_Data(tmp_Command_Text, 25);
                         
                         //Build the entry.
                         tmp_Command_String = "bld ";
                         int2char(Last_Created_Construct, tmp_Command_Char);
                         tmp_Command_String += tmp_Command_Char;
                         tmp_Command_String += " 0000";
                         Tables.shove(tmp_Command_String, Bld_Cmd_Table);
                         
                         //--Qry Table
                         
                         //Clear the strings.
                         tmp_Command_String.clear();
                         clear_Char(tmp_Command_Char, 25);
                         clear_u_IO_Data(tmp_Command_Text, 25);
                         
                         //Build the entry.
                         tmp_Command_String = "qry ";
                         int2char(Last_Created_Construct, tmp_Command_Char);
                         tmp_Command_String += tmp_Command_Char;
                         tmp_Command_String += " 0000 0000 0001 0001";
                         Tables.shove(tmp_Command_String, Qry_Cmd_Table);
                         
                         //These tables are not needed anymore, however, they will be left incase they are needed in the future.
                         //The torches will automatically initiate a backpropagation in lower constructs, no need to activate the low level ones.
                         /*
                         //--Torch Table
                         
                         //Clear the strings.
                         tmp_Command_String.clear();
                         clear_Char(tmp_Command_Char, 25);
                         clear_u_IO_Data(tmp_Command_Text, 25);
                         
                         //Build the entry.
                         tmp_Command_String = "torch ";
                         int2char(Last_Created_Construct, tmp_Command_Char);
                         tmp_Command_String += tmp_Command_Char;
                         strcpy(tmp_Command_Char, tmp_Command_String.c_str());
                         char2u_IO_Data(tmp_Command_Char, tmp_Command_Text, tmp_Command_String.size());
                         
                         //Set the command.
                         Tables.push(tmp_Command_Text, tmp_Command_String.size(), Torch_Cmd_Table);
                         
                         
                         //--Neutralize Table
                         
                         //Clear the strings.
                         tmp_Command_String.clear();
                         clear_Char(tmp_Command_Char, 25);
                         clear_u_IO_Data(tmp_Command_Text, 25);
                         
                         //Build the entry.
                         tmp_Command_String = "neutralize ";
                         int2char(Last_Created_Construct, tmp_Command_Char);
                         tmp_Command_String += tmp_Command_Char;
                         strcpy(tmp_Command_Char, tmp_Command_String.c_str());
                         char2u_IO_Data(tmp_Command_Char, tmp_Command_Text, tmp_Command_String.size());
                         
                         //Set the command.
                         Tables.push(tmp_Command_Text, tmp_Command_String.size(), Neutralize_Cmd_Table);
                         */
                    }
                    
                    set_Input((p_Command_Text + 9), (tmp_Int[1] - 8), Input_Table, tmp_Int[0]);
                    //set_Input(u_IO_Data * p_Data, int p_Data_Length, int p_Table, int p_Index)
                    
                    return 0;
          }
          
          
          
          if (comp_CMD("submit", p_Command_Text))
          {
                    
                    eval(Submission_Cmd_Table);
                    return 0;
          }
          
          
          if (comp_CMD("query", p_Command_Text))
          {
                    
                    eval(Query_Cmd_Table);
                    return 0;
          }
          
          
          if (comp_CMD("new query", p_Command_Text))
          {
               
               evaluate_Command_Text("neutralize all");//Neutralize all assembly nodes.
               evaluate_Command_Text("reset a");//Reset all the IO tables for the assembly.
               evaluate_Command_Text("query");
          }
          
          if (comp_CMD("query for results", p_Command_Text))
          {
                    //Get the number of queries.
                    tmp_Int[0] = char2int((p_Command_Text + 17), 4);
                    
                    for (int cou_Index=0;cou_Index<tmp_Int[0];cou_Index++)
                    {
                         evaluate_Command_Text("query");
                    }
          }
          
          if (comp_CMD("new query for results", p_Command_Text))
          {
                    //Get the number of queries.
                    tmp_Int[0] = char2int((p_Command_Text + 17), 4);
                    
                    //Do the new query to initialize the query process.
                    evaluate_Command_Text("new query");
                    
                    //Submit the query for all but one to account for the "new query".
                    for (int cou_Index=0;cou_Index<tmp_Int[0] - 1;cou_Index++)
                    {
                         evaluate_Command_Text("query");
                    }
          }
          
          if (comp_CMD("bld", p_Command_Text))
          {
                    //Get the construct to build.
                    tmp_Int[0] = char2int((p_Command_Text + 4), 4);
                    
                    
                    //Get the method for building.
                    tmp_Int[1] = char2int((p_Command_Text + 9), 4);
                    
                    build_Construct(tmp_Int[0], tmp_Int[1]);
                    
                    return 0;
          }
          
          
          if (comp_CMD("qry", p_Command_Text))
          {
                    //Get the construct to query.
                    tmp_Int[0] = char2int((p_Command_Text + 4), 4);
                    
                    //Get the charging tier for charging.
                    tmp_Int[1] = char2int((p_Command_Text + 9), 4);
                    
                    //Get the method for building.
                    tmp_Int[2] = char2int((p_Command_Text + 14), 4);
                    
                    //Get the depth for building.
                    tmp_Int[3] = char2int((p_Command_Text + 19), 4); 
                    
                    //Charging pattern.
                    tmp_Int[4] = char2int((p_Command_Text + 24), 4);
                    
                    if (tmp_Int[4] == 0){ Constructs[tmp_Int[0]]->query_Construct(tmp_Int[1], tmp_Int[2], tmp_Int[3]); }
                    if (tmp_Int[4] == 1){ Constructs[tmp_Int[0]]->query_Construct_Force_Pattern(tmp_Int[1], tmp_Int[2], tmp_Int[3]); }
                    if (tmp_Int[4] == 2){ Constructs[tmp_Int[0]]->query_Construct(tmp_Int[1], tmp_Int[2], tmp_Int[3]); }
                    if (tmp_Int[4] == 3){ Constructs[tmp_Int[0]]->query_Construct(tmp_Int[1], tmp_Int[2], tmp_Int[3]); }
                    
                    return 0;
                    
                    //qry [Construct_ID] [Precision] [Method] [Depth] [Charging_Method]
          }
          
          
          //The command index for the tables will always be the same.
          //This is used after submission and queries to evaluate the network correctly.
          if (comp_CMD("eval", p_Command_Text))
          {
                    //Get the table to eval.
                    tmp_Int[0] = char2int((p_Command_Text + 5), 4);
                    
                    eval(tmp_Int[0]);
                    
                    return 0;
          }
          
          
          if (comp_CMD("scmd", p_Command_Text))
          {
                    //Get the command table.
                    tmp_Int[0] = char2int((p_Command_Text + 5), 4);
                    
                    //Get the command index.
                    tmp_Int[1] = char2int((p_Command_Text + 10), 4);
                    
                    //Get the command itself.
                    tmp_Int[5] = 15; 
                    cout << "\n\n   ~Gathering command: {";
                    while(char (p_Command_Text[tmp_Int[5]].I) != '\n')
                    {
                         tmp_Name[tmp_Int[5] - 15] = char (p_Command_Text[tmp_Int[5]].I);
                         cout << tmp_Name[tmp_Int[5] - 15];
                         tmp_Int[5]++;
                    }
                    cout << "}";
                    
                    set_CMD(tmp_Int[0], tmp_Int[1], (p_Command_Text + 15), (tmp_Int[5] - 15));
                    
                    
                    //set_CMD(tmp_Int[0], tmp_Int[1], u_IO_Data p_Command[], tmp_Int[2])
                    return 0;
          }
          
          
          if (comp_CMD("pcmd", p_Command_Text))
          {
                    //Get the command table.
                    tmp_Int[1] = char2int((p_Command_Text + 5), 4);
                    
                    //Get the command itself.
                    tmp_Int[0] = 10; 
                    cout << "\n\n   ~Gathering command: {";
                    while(char (p_Command_Text[tmp_Int[0]].I) != '\n')
                    {
                         tmp_Name[tmp_Int[0] - 10] = char (p_Command_Text[tmp_Int[0]].I);
                         cout << tmp_Name[tmp_Int[0] - 10];
                         tmp_Int[0]++;
                    }
                    cout << "}";
                    
                    push_CMD(tmp_Int[1], (p_Command_Text + 10), (tmp_Int[0] - 10));
                    
                    //push_CMD(int p_Command_Table, int p_Command_Index, u_IO_Data p_Command[], int p_Command_Count)
                    return 0;
          }
          
          
          if (comp_CMD("srt", p_Command_Text))
          {
                    //For sorting all constructs.
                    if (char (p_Command_Text[4].I) == 'a' && char (p_Command_Text[5].I) == 'l' && char (p_Command_Text[6].I) == 'l')
                    {
                         tmp_Int[0] = char2int((p_Command_Text + 8), 4);//Get the flag to sort by.
                         
                         sort_All_Tables_By_Flag(tmp_Int[0]);
                         
                         return 0; //Do not fall through.
                    }
                    
                    tmp_Int[0] = char2int((p_Command_Text + 4), 4);
                    tmp_Int[1] = char2int((p_Command_Text + 9), 4);
                    
                    cout << "\n\t srt " << tmp_Int[0] << " " << tmp_Int[1];
                    sort_Table_By_Flag(tmp_Int[0], tmp_Int[1]);
                    return 0;
          }
          
          if (comp_CMD("reset", p_Command_Text))
          {
                    if (p_Command_Text[5].I == int ('\n'))
                    { 
                         reset_Assembly_IO_Tables(); 
                         return 0;
                    }
                    
                    
                    switch (p_Command_Text[6].I)
                    {
                         //Resets the tables for an assembly.
                         case 'a':
                              reset_All_IO_Tables();
                              break;
                              
                         //Resets all input tables.
                         case 'I':
                              reset_All_I_Tables();
                              break;
                         
                         //Resets all output tables.
                         case 'O':
                              reset_All_O_Tables();
                              break;
                              
                         //Reset a constructs Table.
                         case 'c':
                              tmp_Int[0] = char2int((p_Command_Text + 8), 4);
                              reset_Construct_IO_Tables(tmp_Int[0]);
                              break;
                              
                         //Reset a specific table.
                         case 't':
                              tmp_Int[0] = char2int((p_Command_Text + 8), 4);
                              Tables.reset_Table(tmp_Int[0]);
                              break;
                              
                         //Reset a specific index in a given table.
                         case 'i':
                              tmp_Int[0] = char2int((p_Command_Text + 8), 4);
                              tmp_Int[1] = char2int((p_Command_Text + 13), 4);
                              Tables.reset_Table_Index(tmp_Int[0], tmp_Int[1]);
                              break;
                    }
                    return 0;
          }
          

          if (comp_CMD("torch", p_Command_Text))
          {
                    
                    tmp_Int[0] = char2int((p_Command_Text + 6), 4);
                    
                    discharge_Torch_Nodes(tmp_Int[0]);
                    
                    return 0;
          }
          
          
          
          if (comp_CMD("con", p_Command_Text))
          {
               
                    //Gather the connections.
                    tmp_Int[0] = char2int((p_Command_Text + 4), 4);
                    tmp_Int[1] = char2int((p_Command_Text + 9), 4);
                    tmp_Int[2] = char2int((p_Command_Text + 14), 4);
                    tmp_Int[3] = char2int((p_Command_Text + 19), 4);
                    
                    
                    //cout << "\n\n\t char2int(" << eip_Char << "):" << char2int(tmp_Char, eip_Char);
                    //system("pause > null");
     
                    set_Table_Index_As_Reference(tmp_Int[0], tmp_Int[1], tmp_Int[2], tmp_Int[3]);
                    
                    return 0;
          }
          
          //Sets node references.
          if (comp_CMD("nrset", p_Command_Text))
          {
                    set_Node_References();
                    return 0;
          }
          
               
          if (comp_CMD("node", p_Command_Text))
          {
               
                    /*
                    * 
                    [0]CID
                    [1]UNID
                    [2]Reinforcement_Counter
                    [3]flg_Treetop
                    [4]Tier
                    [5]flg_Torch
                    [6]X
                    [7]Y
                    */
                    
                    //Gather the connections. 
                    tmp_Int[0] = char2int((p_Command_Text + 5), 4);
                    tmp_Int[1] = char2int((p_Command_Text + 10), 4);
                    tmp_Int[2] = char2int((p_Command_Text + 15), 4);
                    tmp_Int[3] = char2int((p_Command_Text + 20), 4);
                    tmp_Int[4] = char2int((p_Command_Text + 25), 4);
                    tmp_Int[5] = char2int((p_Command_Text + 30), 4);
                    tmp_Int[6] = char2int((p_Command_Text + 35), 4);
                    tmp_Int[7] = char2int((p_Command_Text + 40), 4);
                    
                    Last_Created_Node = ((c_Poly_Node*) Constructs[tmp_Int[0]]->Node_Network.load_Node(tmp_Int[1]));
                    Last_Created_Node->set_Reinforcement_Counter(tmp_Int[2]);
                    Last_Created_Node->flg_Treetop = bool (tmp_Int[3]);
                    Last_Created_Node->Tier = tmp_Int[4];
                    Last_Created_Node->flg_Torch = bool (tmp_Int[5]);
                    Last_Created_Node->X = tmp_Int[6];
                    Last_Created_Node->Y = tmp_Int[7];
                    
                    //Last_Created_Node->output_Poly_Node();
                    
                    return 0;
          }

                   
          if (comp_CMD("sta", p_Command_Text))
          {
               
                    //Gather the connections.
                    tmp_Int[0] = char2int((p_Command_Text + 4), 4);
                    tmp_Int[1] = char2int((p_Command_Text + 9), 1);
                    tmp_Int[2] = char2int((p_Command_Text + 11), 1);
                    
                    Last_Created_Node->State_Length = tmp_Int[0];
                    Last_Created_Node->flg_State_Reference = bool (tmp_Int[1]);
                    Last_Created_Node->flg_State = bool (tmp_Int[2]);
                    
                    //cout << "\n State Length = " << tmp_Int[0];
                    
                    for (int cou_State=0;cou_State<Last_Created_Node->State_Length;cou_State++)
                    {
                         //cout << "\n\t Current_State:" << cou_State << "\t";
                         
                         
                         for (int cou_I=0;cou_I<4;cou_I++)
                         {
                              tmp_data_File[cou_State].C[cou_I] = char ((p_Command_Text[(13 + (cou_State * 5)) + cou_I].I));
                         }
                         
                    }
                    
                    Last_Created_Node->set_State(tmp_data_File, tmp_Int[0]);
                    
                    
                    return 0;
          }
          
          if (comp_CMD("axo", p_Command_Text))
          { 
                    
                    //Gather the connections.
                    tmp_Int[0] = char2int((p_Command_Text + 4), 4);
                    tmp_Int[1] = char2int((p_Command_Text + 9), 4);
                    tmp_Int[2] = char2int((p_Command_Text + 14), 4);
                    
                    
                    Last_Created_Node->add_Axon_Connection_Reference(tmp_Int[0], tmp_Int[1], tmp_Int[2]);
                    
                    return 0;
          }

               
          if (comp_CMD("den", p_Command_Text))
          {
                    
                    //Gather the connections.
                    tmp_Int[0] = char2int((p_Command_Text + 4), 4);
                    tmp_Int[1] = char2int((p_Command_Text + 9), 4);
                    
                    Last_Created_Node->add_Dendrite_Connection_Reference(tmp_Int[0], tmp_Int[1]);
                    
                    return 0;
          }
             
             
          if (comp_CMD("output", p_Command_Text))
          {
                    
                    if (comp_CMD("full", p_Command_Text, 7))
                    {
                         
                         //Gather the parameters.
                         tmp_Int[0] = char2int((p_Command_Text + 12), 4);
                         tmp_Int[1] = char2int((p_Command_Text + 17), 4);
                         
                         //Outputs the assembly.
                         output_All_Constructs_Full(0, 0, tmp_Int[0], tmp_Int[1]);
                    
                    }
                    
                    if (comp_CMD("scribe full", p_Command_Text, 7))
                    {
                         
                         //Gather the parameters.
                         tmp_Int[0] = char2int((p_Command_Text + 19), 4);
                         tmp_Int[1] = char2int((p_Command_Text + 24), 4);
                         
                         //Gathers the filename to use and opens the file for scribing.
                         clear_Char(tmp_Name, 100);
                         cin >> tmp_Name;
                         
                         //Open the file.
                         ofstream scribe_File;
                         scribe_File.open(tmp_Name);
                         
                         if (!scribe_File.is_open())
                         {
                              cout << "\t CANNOT OPEN FILE...";
                              return 0;
                         }
                         
                         scribe_File << "<canvas id=\"can_Memory_Plot\" width=\"5000\" height=\"5000\" style=\"border:1px solid #000000;\">\n";
                         scribe_File << "</canvas>\n";
                         
                         scribe_File << "<script>\n";
                         scribe_File << "var c=document.getElementByID(\"can_Memory_Plot\");\n";
                         scribe_File << "var ctx=c.getContext(\"2d\");\n";
                         scribe_File << "ctx.font=\"15px Arial\";\n";
                         
                         //Outputs the assembly.
                         scribe_All_Constructs_Full(&scribe_File, 0, 0, tmp_Int[0], tmp_Int[1]);
                         
                         scribe_File << "</script>\n";
                         
                         //Close the file.
                         scribe_File.close();
                    }
                    
                    if (comp_CMD("table", p_Command_Text, 7))
                    {
                         
                         //Gather the parameters.
                         tmp_Int[0] = char2int((p_Command_Text + 13), 4);
                         
                         //Gather the parameters.
                         tmp_Int[1] = char2int((p_Command_Text + 18), 4);
                         
                         //Outputs the table.
                         Tables.output_Table(tmp_Int[0], tmp_Int[1]);
                    }
                    
                    if (comp_CMD("all tables", p_Command_Text, 7))
                    {
                         
                         //Gather the parameters.
                         tmp_Int[0] = char2int((p_Command_Text + 18), 4);
                         
                         //Outputs the table.
                         Tables.output_All_Tables(tmp_Int[0]);
                    }
                    
                    if (comp_CMD("nodes", p_Command_Text, 7))
                    {
                         
                         //Gather the parameters.
                         tmp_Int[0] = char2int((p_Command_Text + 13), 4);
                         
                         //Outputs the construct nodes.
                         Constructs[tmp_Int[0]]->Node_Network.output_All_Poly_Nodes();
                    }
                    
                    if (comp_CMD("all nodes", p_Command_Text, 7))
                    {
                         //Outputs all of the nodes in the assembly.
                         output_All_Constructs();
                    }
                    
                    if (comp_CMD("all charged nodes", p_Command_Text, 7))
                    {
                         output_All_Constructs_Charged_Poly_Nodes();
                    }
                    
                    //output_All_Treetop_Nodes();
                    
                    //output_All_Charged_Treetop_Nodes();
                    
                    return 0;
          }
          
                    
          if (comp_CMD("neutralize all", p_Command_Text))
          {
                    
                    neutralize_Assembly();
                    
                    return 0;
          }
          
          
          if (comp_CMD("neutralize", p_Command_Text))
          {
                    
                    tmp_Int[0] = char2int((p_Command_Text + 11), 4);
                    cout << "\n\n\t neutralize_Construct(" << tmp_Int[0] << ")";
                    Constructs[tmp_Int[0]]->Node_Network.neutralize_All_Nodes();
                    
                    return 0;
          }
          
          if (comp_CMD("auto", p_Command_Text))
          {
                    
                    tmp_Int[0] = char2int((p_Command_Text + 5), 4);
                    
                    flg_Autobuild = tmp_Int[0];
                    
                    return 0;
          }
          
          
          
          //cout << "\n\n\t\t ~NO COMMAND RECOGNIZED!";
          return 0;
     }
     
     //Compares command text to character string.
     bool comp_CMD(string p_String, u_IO_Data * p_Data, int p_Offset = 0)
     {
          //Gather the string into a temporary character array.
          char * tmp_Char;
          tmp_Char = new char[p_String.size() + 1];
          strcpy(tmp_Char, p_String.c_str());
          
          //Loop through each character checking for errors.
          for (int cou_Index=0;cou_Index<(int (p_String.size()));cou_Index++)
          {
               if (tmp_Char[cou_Index] != char (p_Data[cou_Index + p_Offset].I)){ return 0; }
          }
          
          delete [] tmp_Char;
          
          //Retuns one for a match
          return 1;
     }
};

/*
 * //There are no tiers, only the current depth of the current node.
 * The CAN builds as it needs to, expanding if the need arises to find the topmost node.
 * Once the topmost node is found then the topmost node is set in the place of the treetop output.
 * After a charge of the network then the subnetwork is discharged.
 * The discharging will pass the charges with no dampening to the output, or the modifier charges along the way may take effect.
 * 
 * 
 * 
 * 
 */
/*
 *

 *   //All nodes in a construct are stored in tiers, they can then all be fired starting at the base tier. 
 * The constructs containing the nodes are organized in much the same fashion. A simple table holds the 
 * order that the constructs are to be evaluated. Different tables may be used for different things such 
 * as e, q, o, t, etc.  
 
 * 
*/ 
