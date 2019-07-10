//This is the Basic_Chrono_Predictor assembly.
class c_BCP_Assembly: public c_Base_Assembly
{
protected:
     
public:
     
     c_BCP_Assembly(c_Table_Handler * p_Tables, string p_Assembly_Name = "Default_BCP_Assembly", int p_AID = 0) : c_Base_Assembly(p_Tables, p_Assembly_Name, p_AID)
     {
          Constructs = NULL;
          Construct_Count = 0;
          
          //Set the construct name.
          Assembly_Name = p_Assembly_Name;
          
          //Set the type of assembly.
          Assembly_Type = "Basic_Chrono_Predictor";
          
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
               
               //output_Construct_Internals(tmp_Chrono_CID);
          }
          
          
          return "Build";
     }
     
     
     //Eval
     virtual string Eval()
     {
          
          //The name of the current construct being worked on.
          string tmp_CName = "";
          int tmp_CID = 0;
          int tmp_Chrono_CID = 0;
          string tmp_Output_Node_Table = "";
          string tmp_Output_Charge_Table = "";
          string tmp_Output_Pattern_Table = "";
          c_Table_1D * tmp_Output_Charge_Table_Ref = NULL;
          c_Table_1D * tmp_Output_Pattern_Table_Ref = NULL;
          u_Data tmp_Node;
          int tmp_Charge = 0;
          
          //Loop through all of the raw construct in the priority table.
          for (int cou_Index=0;cou_Index<Construct_Index_Table->get_Row_Count();cou_Index++)
          {
               
               tmp_CName = Construct_Index_Table->get_string(cou_Index, 0);
               
               cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t Eval(" << cou_Index << ")->" << tmp_CName;
               
               tmp_CID = str2int(get_Construct_ID(tmp_CName));
               tmp_Chrono_CID = str2int(get_Construct_ID(tmp_CName + "_Chrono"));
               
               if (tmp_CID == -1){ continue; }
               
               //Gather the input for the construct.
               Tables->translate_Row(Input_Table->get_Table_ID(), cou_Index, get_Construct_Input_Table_ID(tmp_CID), 0);
               Tables->translate_Row(Input_Table_Truth->get_Table_ID(), cou_Index, get_Construct_Input_Table_Truth_ID(tmp_CID), 0);
               
               cout << "\n\t Evaluating Input Table:";
               Tables->output_Table(get_Construct_Input_Table_ID(tmp_CID));
               
               cout << "\n\t eval_Construct(tmp_CName, full)";
               
               //Eval.
               eval_Construct(tmp_CName, "full");
               
               cout << "\n\t Results of Raw Eval:";
               Tables->output_Table(get_Construct_Output_Table_Pattern_ID(tmp_CID));
               Tables->output_Table(get_Construct_Output_Table_Charge_ID(tmp_CID));
               //Tables->output_Table(get_Construct_Treetop_Table_ID(tmp_CID));
               
               //Shift the Chrono tables column.
               Tables->shift_Column(get_Construct_Input_Table_ID(tmp_CName + "_Chrono"), 0);
               
               //Gather the new nodes for the chrono.
               Tables->translate_Row(get_Construct_Treetop_Table_ID(tmp_CID), 0, get_Construct_Input_Table_ID(tmp_CName + "_Chrono"), 0);
               
               cout << "\n\t build_Construct(" << tmp_CName + "_Chrono" << ", full)";
               
               //Build the Chrono.
               build_Construct((tmp_CName + "_Chrono"), "full_RC_row");
               
               output_Construct_Full(tmp_Chrono_CID);
               
               //Gather the tables to use.
               tmp_Output_Node_Table = Tables->get_Table_Name(get_Construct_Output_Table_Nodes_ID(tmp_CName));
               tmp_Output_Charge_Table = Tables->get_Table_Name(get_Construct_Output_Table_Charge_ID(tmp_CName));
               
               cout << "\n\t foreign output tables:";
               
               //Tables->output_Table(tmp_Output_Node_Table);
               //Tables->output_Table(tmp_Output_Charge_Table);
               
               cout << "\n\t eval_Foreign_Output_Table(" << tmp_CName + "_Chrono" << ", full)";
               
               //Eval the Chrono using the output tables from the raw construct.
               eval_Foreign_Output_Table((tmp_CName + "_Chrono"), tmp_Output_Node_Table, tmp_Output_Charge_Table);
               
               cout << "\n\t eval_Foreign_Output_Table...Done...";
               
               cout << "\n\t Gathering backpropagated patterns...";
               
               //output_Construct_Full(tmp_CID);
               output_Construct_Full(tmp_Chrono_CID);
               
               //Clear the tables of the raw construct.
               reset_Construct_Output_Tables(tmp_CID);
               
               //Repurpose the tmp_Tables.
               tmp_Output_Pattern_Table_Ref = Constructs[tmp_Chrono_CID]->get_Output_Table_Pattern_Reference();
               tmp_Output_Charge_Table_Ref = Constructs[tmp_Chrono_CID]->get_Output_Table_Charge_Reference();
               
               
               cout << "\n\t Chrono tables to backpropagate:";
               
               //tmp_Output_Pattern_Table_Ref->output_Table();
               //tmp_Output_Charge_Table_Ref->output_Table();
               
               //For each row in the Chrono output the second bit will be taken from output_Pattern. This will be used to backpropagate the current pattern.
               //Along with the backpropagation the charge will be added to the output tables as well.
               for (int cou_Row=0;cou_Row<tmp_Output_Pattern_Table_Ref->get_Row_Count();cou_Row++)
               {
                    //Now for each cell.
                    for (int cou_Cell=0;cou_Cell<tmp_Output_Pattern_Table_Ref->get_Row_Cell_Count(cou_Row);cou_Cell++)
                    {
                         //Gather the node and charge to use.
                         tmp_Node = (tmp_Output_Pattern_Table_Ref->get_data(cou_Row, cou_Cell, 1));
                         tmp_Charge = tmp_Output_Charge_Table_Ref->get_int(cou_Row, cou_Cell);
                         
                         //Submit the charge and node to the construct for backpropagation.
                         Constructs[tmp_CID]->initiate_Backpropagation_Foreign_Node(tmp_Node, cou_Row, tmp_Charge);
                    }
               }
               
               Constructs[tmp_CID]->gather_Output_Tables();
               
               //output_Construct_Full(tmp_CID);
               output_Construct_Full(tmp_Chrono_CID);
               
               cout << "\n\t Results of Chrono Eval:";
               Tables->output_Table(get_Construct_Output_Table_Pattern_ID(tmp_CID));              
               Tables->output_Table(get_Construct_Output_Table_Charge_ID(tmp_CID));
               
          }
          
          return "Eval";
     }
     
     //Train
     virtual string Train()
     {
          Build();
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