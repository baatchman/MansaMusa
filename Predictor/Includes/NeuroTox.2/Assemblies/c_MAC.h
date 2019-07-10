//This is the Basic_Chrono_Predictor assembly.
class c_MAC_Assembly: public c_Base_Assembly
{
protected:
     
public:
     
     c_MAC_Assembly(c_Table_Handler * p_Tables, string p_Assembly_Name = "MAC Assembly", int p_AID = 0) : c_Base_Assembly(p_Tables, p_Assembly_Name, p_AID)
     {
          Constructs = NULL;
          Construct_Count = 0;
          
          //Set the construct name.
          Assembly_Name = p_Assembly_Name;
          
          //Set the type of assembly.
          Assembly_Type = "MAC";
          
          //Set the table handler.
          Tables = p_Tables;
          
          //Set the ID of the Assembly.
          AID = p_AID;
          
          //The input index to draw data from the gathering wall with.
          Gathering_Input_Table_Index = p_AID * 10;
          
          //Create the MSC that will be common to this assembly.
          create_Construct("MSC", 1, 1);
          Constructs[0]->set_Input_Type(1);
          
          Construct_Index_Table->push_string(0, "MSC");
          
          //Updates the settings table.
          update_Settings_Table();
          update_Internal_Table();
     }
     
     //Gathers the inputs for the table based on the hotlinked tables.
     //The hotlinking table holds the table ID and the Row.
     void gather_Inputs()
     {
          //Input_Table_Hotlinks->output_Table();
          
          for (int cou_Index=0;cou_Index<Input_Table_Hotlinks->get_Row_Count();cou_Index++)
          {
               Tables->copy_Row(Input_Table_Hotlinks->get_int(cou_Index, 0), Input_Table_Hotlinks->get_int(cou_Index, 1), Input_Table->get_Table_ID(), cou_Index);
               //Tables->output_Table(Input_Table_Hotlinks->get_int(cou_Index, 0));
          }
          //Input_Table->output_Table();
     }
     
     /*
     //Gather the inputs for Eval.
     void gather_Inputs_Eval(int p_Set)
     {
          for (int cou_Index=0;cou_Index<Input_Table_Hotlinks->get_Row_Count();cou_Index++)
          {
               //Shift the input tables column.
               Tables->shift_Column(Input_Table, 0);
               
               //Gather the new nodes for the chrono.
               Tables->translate_Row(Input_Table_Hotlinks->get_int(cou_Index, 0), Input_Table_Hotlinks->get_int(cou_Index, 1), get_Construct_Input_Table_ID(tmp_CName + "_Chrono"), 0);
               
               Tables->copy_Row(Input_Table_Hotlinks->get_int(cou_Index, 0), Input_Table_Hotlinks->get_int(cou_Index, 1), Input_Table->get_Table_ID(), cou_Index);
               //Tables->output_Table(Input_Table_Hotlinks->get_int(cou_Index, 0));
          }
               
     }     */    
     
     //Creates a hotlink to a table with the given TID and Index.
     void set_Hotlink(int p_Table, int p_Index, int p_Pos = -1)
     {
          if (p_Pos == -1)
          {
               Input_Table_Hotlinks->pop_push_int(p_Table);
               Input_Table_Hotlinks->pushc_int(p_Index);
          }
          else
          {
               Input_Table_Hotlinks->set_int(p_Pos, 0, p_Table);
               Input_Table_Hotlinks->set_int(p_Pos, 1, p_Index);
          }
     }
     
     //==--     INTERFACE COMMANDS
     
     
     //Build
     virtual string Build()
     {
          //Gathers the input for the table.
          gather_Inputs();
          
          //The name of the current construct being worked on.
          int tmp_CID = 0;
          
          int tmp_MSC_Table = get_Construct_Input_Table_ID("MSC");
          
          Tables->reset_Table(tmp_MSC_Table);
          
          //Loop through all of the raw construct in the priority table.
          for (int cou_Index=0;cou_Index<Input_Table->get_Row_Count();cou_Index++)
          {
               //Add the input to the MSC from the raw treetop.
               //Translate the row from the current contructs treetops into the MSC, this will allow for row by row building of what is actually the columns.
               Tables->translate_Row(Input_Table->get_Table_ID(), cou_Index, tmp_MSC_Table, cou_Index);
               
               //Constructs[tmp_CID]->output_Tables();
          }
          
          tmp_CID = str2int(get_Construct_ID("MSC"));
          
          //Now that all the raw treetop are gathered we will build the MSC column by column.
          build_Construct(tmp_CID, "full_RC_row");
          
          //Constructs[tmp_CID]->output_Tables();
          
          return "Build";
     }
     
     //Build
     virtual string Build_BM()
     {
          //Gathers the input for the table.
          gather_Inputs();
          
          //The name of the current construct being worked on.
          int tmp_CID = 0;
          
          int tmp_MSC_Table = get_Construct_Input_Table_ID("MSC");
          
          if (Input_Table->table_Is_Empty()){ cout << "\n\t  NO INPUT!"; return "No_Input!"; }
          
          Tables->reset_Table(tmp_MSC_Table);
          
          //Loop through all of the raw construct in the priority table.
          for (int cou_Index=0;cou_Index<Input_Table->get_Row_Count();cou_Index++)
          {
               //Add the input to the MSC from the raw treetop.
               //Translate the row from the current contructs treetops into the MSC, this will allow for row by row building of what is actually the columns.
               Tables->translate_Row(Input_Table->get_Table_ID(), cou_Index, tmp_MSC_Table, cou_Index);
               
               //Constructs[tmp_CID]->output_Tables();
          }
          
          tmp_CID = str2int(get_Construct_ID("MSC"));
          
          //Now that all the raw treetop are gathered we will build the MSC column by column.
          build_Construct_BM(tmp_CID, "full_RC_row");
          
          //Constructs[tmp_CID]->output_Tables();
          
          return "Build";
     }
     
     
     //Eval
     virtual string Eval()
     {
          cout << "\n\t Eval() --MAC";
          
          gather_Inputs();
          
          Input_Table->output_Table();
          
          //The name of the current construct being worked on.
          string tmp_CName = "";
          int tmp_CID = 0;
          string tmp_Output_Node_Table = "";
          string tmp_Output_Charge_Table = "";
          
          //Loop through all of the raw construct in the priority table.
          for (int cou_Index=0;cou_Index<Construct_Index_Table->get_Row_Count();cou_Index++)
          {
               
               tmp_CName = Construct_Index_Table->get_string(cou_Index, 0);
               
               cout << "\n\t Eval(" << cou_Index << ")->" << tmp_CName;
               
               tmp_CID = str2int(get_Construct_ID(tmp_CName));
               
               if (tmp_CID == -1){ continue; }
               
               //Gather the input for the construct.
               Tables->translate_Row(Input_Table->get_Table_ID(), cou_Index, get_Construct_Input_Table_ID(tmp_CID), 0);
               
               cout << "\n\t eval_Construct(tmp_CName, full)";
               
               //Eval.
               eval_Construct(tmp_CName, "full");
               
               //Shift the Chrono tables column.
               Tables->shift_Column(get_Construct_Input_Table_ID(tmp_CName + "_Chrono"), 0);
               
               //Gather the new nodes for the chrono.
               Tables->translate_Row(get_Construct_Treetop_Table_ID(tmp_CID), 0, get_Construct_Input_Table_ID(tmp_CName + "_Chrono"), 0);
               
               cout << "\n\t build_Construct(" << tmp_CName + "_Chrono" << ", full)";
               
               //Build the Chrono.
               build_Construct((tmp_CName + "_Chrono"), "full_RC");
               
               //Add to the MSC.
               
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
          
          return "Add_Raw_Construct->" + p_CName;
     }
          
     //Adds a raw number construct chain to the assembly.
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
          
          return "Add_Raw_Construct->" + p_CName;
     }
     
     //Adds a raw number construct chain to the assembly.
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
          
          return "Add_Raw_Construct->" + p_CName;
     }
     
     

};