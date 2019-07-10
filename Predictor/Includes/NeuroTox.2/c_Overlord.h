//This class functions as the controller for the gathering as well as the interface to exterior manipulation.
//   Contained is the gathering which allows the base functionality of submitting data to, building, charging, and extracting
// information from constructs. There is also the Command_Parser which gives the ability to script behaviors for the
// gathering.

class c_Overlord
{
protected:

     //The command parser for this gathering.
     c_Command_Parser * CMD;

     //The table handler associated with this gathering.
     c_Table_Handler Tables;

     //The gathering that the overlord presides over.
     c_Base_Gathering * Gathering;

public:




     c_Overlord(string p_Name = "The_Gathering")
     {


          //Sets the console color.
          set_Console_Color(0, 10);

          //Setup the Gathering.
          Gathering = new c_Base_Gathering(&Tables, p_Name);

          //The command parser for the gathering.
          CMD = new c_Command_Parser(&Tables, Gathering);
     }

     ~c_Overlord()
     {
          delete Gathering;
          delete CMD;
     }

     c_Table_Handler * get_Table_Handler()
     {
          return &Tables;
     }


     //Allows live manipulation through the Command_Parser.
     void rcon()
     {
          CMD->rcon();
     }

     //Updates the settings in the gathering and all owned members.
     void update_All_Settings()
     {
          //cout << "  Updating_All_Settings"; cout.flush();
          //cout << "\n\t Gathering->" << Gathering;
          Gathering->update_All_Settings();
     }

     //Saves the Overlord.
     //
     //The overlord and assemblies will have their settings and componets saved into a linker file.
     //The constructs will have their own files linking to the files saving their internals.
     //Node networks will have their own files that save the nodes as tiers and indexes, or in the case of the number constructs the trees and tables.
     //For the state nodes the upper tier constructs will have the inputs saved as coordinates to a node whose address will be retrieved upon loading.
     void Save(int p_Ticket_Number = -1)
     {
          //Save the Overlord.

          //Save the Gathering.
          Gathering->Save(p_Ticket_Number);

          //Save The Assemblies.

          //Save the Constructs.
     }

     //Loads an Overlord.
     void Load(string p_Dir, string p_Gathering)
     {
          Gathering->Load(p_Dir, p_Gathering);
     }

     //Creates an assembly.
     int create_Assembly(string p_Name, string p_Type, int p_Tier = 0)
     {
          return Gathering->create_Assembly(p_Name, p_Type, p_Tier);
     }

     //Creates a hotlink based on assembly type.
     void create_Hotlink(string p_Assembly_From, string p_Construct_From, int p_Row_From, string p_Assembly_To)
     {
          Gathering->create_Hotlink(p_Assembly_From, p_Construct_From, p_Row_From, p_Assembly_To);
     }

     //Adds a raw construct to the given assembly.
     void add_Raw_Construct(int p_AID, string p_Name, int p_Dimension = 1){ Gathering->Add_Raw_Construct(p_AID, p_Name, p_Dimension); }
     void add_Raw_Int_Construct(int p_AID, string p_Name, int p_Dimension = 1){ Gathering->Add_Raw_Int_Construct(p_AID, p_Name, p_Dimension); }
     void add_Raw_Float_Construct(int p_AID, string p_Name, int p_Dimension = 1){ Gathering->Add_Raw_Float_Construct(p_AID, p_Name, p_Dimension); }

     void add_Raw_Construct(string p_AName, string p_Name, int p_Dimension = 1){ Gathering->Add_Raw_Construct(p_AName, p_Name, p_Dimension); }
     void add_Raw_Int_Construct(string p_AName, string p_Name, int p_Dimension = 1){ Gathering->Add_Raw_Int_Construct(p_AName, p_Name, p_Dimension); }
     void add_Raw_Float_Construct(string p_AName, string p_Name, int p_Dimension = 1){ Gathering->Add_Raw_Float_Construct(p_AName, p_Name, p_Dimension); }

     //Allows for the pushing of data into the gatherings input tables.
     void push_Input(int p_Row, string p_Input){ Gathering->push_Input(p_Row, p_Input); }
     void push_Input(int p_Row, int p_Input){ Gathering->push_Input(p_Row, int2str(p_Input)); }
     void push_Input(int p_Row, float p_Input){ Gathering->push_Input(p_Row, float2str(p_Input)); }

     //Allows for the pushing of data into the gatherings input tables.
     void set_Input(int p_Row, int p_Cell, string p_Input){ Gathering->set_Input(p_Row, p_Cell, p_Input); }
     void set_Input(int p_Row, int p_Cell, float p_Input){ Gathering->set_Input(p_Row, p_Cell, float2str(p_Input)); }
     void set_Input(int p_Row, int p_Cell, int p_Input){ Gathering->set_Input(p_Row, p_Cell, int2str(p_Input)); }

     //Resets the input table.
     void reset_Input(){ Gathering->reset_Input(); }

     //Resets all of the input tables.
     void reset_All_Inputs(){ Gathering->reset_All_Inputs(); }

     //Building, Evaling, and Training.
     void Gather_Inputs(){ Gathering->Gather_Inputs(); }
     void Build(){ Gathering->Build(); }
     void Build_BM(){ Gathering->Build_BM(); }
     void Eval(){ Gathering->Eval(); }
     void Train(){ Gathering->Train(); }

     //Clears the tables.
     void reset_Tables()
     {
          Gathering->reset_Tables();
     }

     //Evaluates command text string.
     string eval_Command_Text(string p_Command_Text)
     {
          return CMD->eval_Command_String(p_Command_Text);
     }

     //Outputs the gathering.
     void output_Overlord()
     {
          cout << "\n\n\t----------";
          cout << "\n\t The Overlord";
          Gathering->output_Gathering();
     }


     //Outputs all of the tables in the Table Handler.
     void output_All_Tables()
     {
          Tables.output_All_Tables();
     }

     //Outputs the debug information currently in use.
     void output_Debug()
     {
          cout << "\n\n\t Outputting Overlord...";

          Gathering->output_Debug();
     }

     //Outputs the stats on the node networks.
     void output_Node_Network_Stats()
     {
          Gathering->output_Node_Network_Stats();
     }

     string get_Stats()
     {
          return Gathering->get_Stats();
     }

     //Saves the input table.
     void save_Input_Table()
     {
          Gathering->save_Input_Table();
     }
};
