//This is the Basic_Chrono_Predictor assembly.
class c_MSC_Assembly: public c_Base_Assembly
{
protected:

public:

     c_MSC_Assembly(c_Table_Handler * p_Tables, string p_Assembly_Name = "MSC Assembly", int p_AID = 0) : c_Base_Assembly(p_Tables, p_Assembly_Name, p_AID)
     {
          Constructs = NULL;
          Construct_Count = 0;

          //Set the construct name.
          Assembly_Name = p_Assembly_Name;

          //Set the type of assembly.
          Assembly_Type = "MSC";

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



     //==--     INTERFACE COMMANDS


     //Build
     virtual string Build()
     {
          //The name of the current construct being worked on.
          string tmp_CName = "";
          int tmp_CID = 0;
          int tmp_MSC_Table = get_Construct_Input_Table_ID("MSC");

          Tables->reset_Table(tmp_MSC_Table);

          //Loop through all of the raw construct in the priority table.
          for (int cou_Index=0;cou_Index<Construct_Index_Table->get_Row_Count();cou_Index++)
          {
               tmp_CName = Construct_Index_Table->get_string(cou_Index, 0);
               tmp_CID = str2int(get_Construct_ID(tmp_CName));

               if (tmp_CID == -1){ continue; }

               //Gather the input for the construct.
               Tables->translate_Row(Input_Table->get_Table_ID(), cou_Index, get_Construct_Input_Table_ID(tmp_CID), 0);

               //Build it.
               build_Construct(tmp_CName, "full");

               //Add the input to the MSC from the raw treetop.
               //Translate the row from the current contructs treetops into the MSC, this will allow for row by row building of what is actually the columns.
               Tables->translate_Row(get_Construct_Treetop_Table_ID(tmp_CName), 0, tmp_MSC_Table, cou_Index);

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
          //The name of the current construct being worked on.
          string tmp_CName = "";
          int tmp_CID = 0;
          int tmp_MSC_Table = get_Construct_Input_Table_ID("MSC");

          Tables->reset_Table(tmp_MSC_Table);

          //Loop through all of the raw construct in the priority table.
          for (int cou_Index=0;cou_Index<Construct_Index_Table->get_Row_Count();cou_Index++)
          {
               tmp_CName = Construct_Index_Table->get_string(cou_Index, 0);
               tmp_CID = str2int(get_Construct_ID(tmp_CName));

               if (tmp_CID == -1){ continue; }

               //Gather the input for the construct.
               Tables->translate_Row(Input_Table->get_Table_ID(), cou_Index, get_Construct_Input_Table_ID(tmp_CID), 0);

               //Build it.
               build_Construct_BM(tmp_CName, "full");

               //Add the input to the MSC from the raw treetop.
               //Translate the row from the current contructs treetops into the MSC, this will allow for row by row building of what is actually the columns.
               Tables->translate_Row(get_Construct_Treetop_Table_ID(tmp_CName), 0, tmp_MSC_Table, cou_Index);

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

          cout << "\n\t Eval() --MSC";

          if (Input_Table->table_Is_Empty()){ cout << "\n\t  NO INPUT!"; return "No_Input!"; }

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

               cout << "\n\t  IR[" << cou_Index << "]->" << Input_Table->row_Is_Empty(cou_Index);

               if (Input_Table->row_Is_Empty(cou_Index)){ cout << "\n\t  CONSTRUCT " << tmp_CName << " (" << cou_Index << ") NO INPUT!"; continue; }

               //Gather the input for the construct.
               Tables->translate_Row(Input_Table->get_Table_ID(), cou_Index, get_Construct_Input_Table_ID(tmp_CID), 0);

               cout << "\n\t eval_Construct(tmp_CName, full)";

               //Eval.
               eval_Construct(tmp_CName, "full");

          }

          //void eval_Foreign_Output_Table(string p_Construct, string p_Output_Table_Nodes, string p_Output_Table_Charges)

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

          Construct_Index_Table->output_Table();
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

          Construct_Index_Table->output_Table();
          return "Add_Raw_Construct->" + p_CName;
     }



};
