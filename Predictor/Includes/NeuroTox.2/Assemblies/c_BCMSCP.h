//This is the Basic_Chrono_Predictor assembly.
class c_BCMSCP_Assembly: public c_Base_Assembly
{
protected:
     
public:
     
     c_BCMSCP_Assembly(c_Table_Handler * p_Tables, string p_Assembly_Name = "Default_BCMSCP_Assembly", int p_AID = 0) : c_Base_Assembly(p_Tables, p_Assembly_Name, p_AID)
     {
          Constructs = NULL;
          Construct_Count = 0;
          
          //Set the construct name.
          Assembly_Name = p_Assembly_Name;
          
          //Set the type of assembly.
          Assembly_Type = "Basic_Chrono_Multi-Sensory_Construct";
          
          //Set the table handler.
          Tables = p_Tables;
          
          //Set the ID of the Assembly.
          AID = p_AID;
          
          //The input index to draw data from the gathering wall with.
          Gathering_Input_Table_Index = p_AID * 10;
          
          //Setup the tables.
          Input_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Input");
          Input_Table_Truth = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Input_Truth");
          Input_Table_Hotlinks = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Input_Hotlinks");
          
          Output_Table_Pattern = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Output_Pattern");
          Output_Table_Charge = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Output_Charge");
          Output_Table_Nodes = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Output_Nodes");
          Output_Table_Truth = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Output_Truth");
          Output_Table_RC = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Output_RC");
          
          Internal_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Internal");
          
          //With the settings table the first index is the name of the setting.
          Settings_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Settings");
          
          Construct_Index_Table = Tables->register_New_Table_1D("a_" + Assembly_Name + "_Construct_Index");
          
          //Create the MSC that will be common to this assembly.
          create_Construct("MSC", 1, 1);
          
          //Updates the settings table.
          update_Settings_Table();
          update_Internal_Table();
     }
     
     
     
     //==--     INTERFACE COMMANDS
     
     
     //Build
     virtual string Build()
     {
          //The name of the current construct being worked on.
          string tmp_CName = "";
          int tmp_CID = 0;
          int tmp_Chrono_CID = 0;
          
          //Loop through all of the raw construct in the priority table.
          for (int cou_Index=0;cou_Index<Construct_Index_Table->get_Row_Count();cou_Index++)
          {
               tmp_CName = Construct_Index_Table->get_string(cou_Index, 0);
               tmp_CID = str2int(get_Construct_ID(tmp_CName));
               tmp_Chrono_CID = str2int(get_Construct_ID(tmp_CName + "_Chrono"));
               
               if (tmp_CID == -1){ continue; }
               
               //Gather the input for the construct.
               Tables->translate_Row(Input_Table->get_Table_ID(), cou_Index, get_Construct_Input_Table_ID(tmp_CID), 0);
               
               //Build it.
               build_Construct(tmp_CName, "full");
               
               //Shift the Chrono tables column.
               Tables->shift_Column(get_Construct_Input_Table_ID(tmp_Chrono_CID), 0);
               
               //Gather the new nodes for the chrono.
               Tables->translate_Row(get_Construct_Treetop_Table_ID(tmp_CID), 0, get_Construct_Input_Table_ID(tmp_Chrono_CID), 0);
               
               //And the Chrono.
               build_Construct(tmp_Chrono_CID, "full_RC_row");
               
               //Add the input to the MSC from the chrono treetop.
               //Translate the row from the current contructs treetops into the MSC, this will allow for row by row building of what is actually the columns.
               Tables->translate_Row(get_Construct_Treetop_Table_ID(tmp_CName + "_Chrono"), 0, get_Construct_Input_Table_ID("MSC"), cou_Index);
          }
          
          tmp_CID = str2int(get_Construct_ID("MSC"));
          //int tmp_MSC_Input = get_Construct_Input_Table_ID("MSC");
          //Tables->output_All_Tables();
          
          //Now that all the raw treetop are gathered we will build the MSC column by column.
          
          //Build each set.
          build_Construct(tmp_CID, "full_RC_row");
          
          
          return "Build";
     }
     
     
     //Eval
     virtual string Eval()
     {
          
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
          
          //Create the raw construct.
          create_Construct((p_CName + "_Chrono"), 2, p_Dimension);
          Constructs[Construct_Count - 1]->set_Input_Type(1);
          
          //Create the chrono construct.
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
          
          //Create the raw construct.
          create_Construct((p_CName + "_Chrono"), 2, p_Dimension);
          Constructs[Construct_Count - 1]->set_Input_Type(1);
          
          //Create the chrono construct.
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
          
          //Create the raw construct.
          create_Construct((p_CName + "_Chrono"), 2, p_Dimension);
          Constructs[Construct_Count - 1]->set_Input_Type(1);
          
          //Create the chrono construct.
          return "Add_Raw_Construct->" + p_CName;
     }
     

};