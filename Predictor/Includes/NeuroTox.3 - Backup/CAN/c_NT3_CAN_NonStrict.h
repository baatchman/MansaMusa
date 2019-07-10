

//The class for the 1D CAN.
class c_NT3_CAN_NonStrict
{
private:
     
     
public:
     
     //Reference to the input table to use for building.
     c_Raw_Table_Cell_1D * Input;
     
     //Reference to the node network to use for the node handling.
     c_NT3_Node_Network_NonStrict * Nodes;
     
     //The nodes that make up the CAN.
     c_NT3_Base_Node_NonStrict *** CAN;
     
     //The current Length of the input string.
     int Length_Of_Input;
     
     //The treetop node.
     c_NT3_Base_Node_NonStrict * Treetop;
     
     c_NT3_CAN_NonStrict()
     {
          Input = NULL;
          Nodes = NULL;
          CAN = NULL;
          Treetop = NULL;
          Length_Of_Input = 0;
     }
     
     ~c_NT3_CAN_NonStrict()
     {
          reset();
     }
     
     //Initializes the CAN with an input cell and the node netowrk.
     void init(c_NT3_Node_Network_NonStrict * p_Nodes)
     {
          Nodes = p_Nodes;
     }
     
     //Sets the current input cell.
     void set_Input(c_Raw_Table_Cell_1D * p_Input)
     {
          Input = p_Input;
     }
     
     //Builds full.
     void Full()
     {
          resize();
          fill_State();
          build_Tiers_Full();
     }
     
     //Builds full with RC reinforcing.
     void RC()
     {
          resize();
          fill_State();
          build_Tiers_Full();
          reinforce();
     }
     
     //Builds for Query.
     void Query()
     {
          resize();
          query_State();
          build_Tiers_Query();
     }
     
     //Fills out the State tier.
     void fill_State()
     {
          for (int cou_Index=0;cou_Index<Length_Of_Input;cou_Index++)
          {
               CAN[0] [cou_Index] = Nodes->get_State_Node(Input->Data[cou_Index]);
          }
     }
          
     //Fills out the State tier.
     void query_State()
     {
          for (int cou_Index=0;cou_Index<Length_Of_Input;cou_Index++)
          {
               CAN[0] [cou_Index] = Nodes->get_State_Node_Query(Input->Data[cou_Index]);
          }
     }
     
     //Builds the Tiers full.
     void build_Tiers_Full()
     {
          //If there are not enough tiers to create a treetop connection then return NULL.
          if (Length_Of_Input < 1){ Treetop = NULL; return; }
          
          //Gets the treetop node.
          //Passes the bottom row to the get_Connection.
          CAN[1] [0] = Nodes->get_Treetop_Connection(CAN[0], Length_Of_Input);
          
          //Gather treetop node.
          Treetop = CAN[1] [0];
     }
     
     //Builds the Tiers full.
     void build_Tiers_Query()
     {
          if (Length_Of_Input < 1){ return; }
          CAN[1] [0] = Nodes->does_Upper_Tier_Connection_Exist(CAN[0], Length_Of_Input);
     }
     
     //Reinforces the nodes in the CAN.
     void reinforce()
     {
          for (int cou_Index=0;cou_Index<(Length_Of_Input);cou_Index++)
          {
               if (CAN[0] [cou_Index] == NULL){ continue; }
               CAN[0] [cou_Index]->reinforce();
          }
          
          if (CAN[1] [0] != NULL){ CAN[1] [0]->reinforce(); }
     }
     
     //Resets the CAN.
     void reset()
     {
          if (Length_Of_Input != 0)
          {
               for (int cou_Index=0;cou_Index<(Length_Of_Input);cou_Index++)
               {
                    CAN[0] [cou_Index] = NULL;
               }
               CAN[1] [0] = NULL;
               
               delete [] CAN[0];
               CAN[0] = NULL;
               delete [] CAN[1];
               CAN[1] = NULL;
                    
               delete [] CAN;
               CAN = NULL;
          }
          Length_Of_Input = 0;
          
     }
     
     //Resizes the CAN to build the current input set.
     void resize()
     {
          //Reset the CAN before we begin.
          reset();
          
          //Gather the number of tiers to build.
          Length_Of_Input = Input->Length;
          
          
          //Setup the tiers.
          CAN = new c_NT3_Base_Node_NonStrict**[2];
          
          //Init the tiers and setup the indexes NULLing them along the way.

          CAN[0] = new c_NT3_Base_Node_NonStrict*[(Input->Length)];
          
          for (int cou_Index=0;cou_Index<(Length_Of_Input);cou_Index++)
          {
               CAN[0] [cou_Index] = NULL;
          }
          
          CAN[1] = new c_NT3_Base_Node_NonStrict*[1];
          CAN[1] [0] = NULL;
     }
     
     //Outputs the CAN.
     void output_CAN()
     {
          cout << "\n\n";
          cout << "\n" << 0 << "->";
          for (int cou_Index=0;cou_Index<(Length_Of_Input);cou_Index++)
          {
               cout << " {" << CAN[0] [cou_Index] << "}";
          }
          cout << "\n" << 1 << "->";
          cout << " {" << CAN[1] [0] << "}";
     }
          
     //Outputs the CAN.
     void output_CAN_RC()
     {
          cout << "\n\n";
          cout << "\n" << 0 << "->";
          for (int cou_Index=0;cou_Index<(Length_Of_Input);cou_Index++)
          {
               cout << " {" << CAN[0] [cou_Index]->get_RC_Score() << "}";
          }
          cout << "\n" << 1 << "->";
          cout << " {" << CAN[1] [0]->get_RC_Score() << "}";
     }
          
     //Outputs the CAN.
     void output_CAN_State()
     {
          cout << "\n\n";
          cout << "\n" << 0 << "->";
          for (int cou_Index=0;cou_Index<(Length_Of_Input);cou_Index++)
          {
               cout << " {";
               Nodes->bp_Output_Only(CAN[0] [cou_Index]);
               cout << "}";
          }
          cout << "\n" << 1 << "->";
          cout << " {";
          Nodes->bp_Output_Only(CAN[1] [0]);
          cout << "}";
     }
          
     //Outputs the CAN.
     void output_CAN_Type()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Length_Of_Input;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_Index=0;cou_Index<(Length_Of_Input - cou_T);cou_Index++)
               {
                    cout << " {" << (CAN[cou_T] [cou_Index]->get_Type()) << "}";
               }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_Axons()
     {
          cout << "\n\n";
          cout << "\n" << 0 << "->";
          for (int cou_Index=0;cou_Index<(Length_Of_Input);cou_Index++)
          {
               cout << " {" << CAN[0] [cou_Index] << " (" << (CAN[0] [cou_Index]->Axon_Count) << ")}";
          }
          cout << "\n" << 1 << "->";
          cout << " {" << CAN[1] [0] << " (" << (CAN[1] [0]->Axon_Count) << ")}";
     }

};