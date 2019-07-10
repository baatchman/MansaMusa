//This file holds the CAN class. The CAN is the structure that temporarily holds
// the current input nodes and allows for building a construct, as well as 
// charging.

//1 dimensional CAN.
class c_MT_Base_CAN_1D
{
protected:
     
     
public:

     //This is an array that temporarily holds the nodes we are working with.
     c_Base_Node_1D *** CAN_Nodes;
     
     //This is the node network that the current CAN is connected to.
     //To communicate with other constructs the buffers will be pushed onto output tables.
     //The node network is passed to the CAN to allow for multiple CANs to share a network.
     c_Base_Node_Network_1D * Node_Network;
     
     //The charging buffer to use, is passed from the construct.
     c_Charging_Buffer_1D * Charging_Buffer;
     
     //The output file for BenchMarking.
     ofstream * BenchMark;
     
     //The current number of tiers.
     int Number_Of_Tiers;
     
     //The current number of nodes in the tiers is proportionate to the input length
     //and the current tier.
     
     //This is the length of the current input.
     int Input_Length;
     
     //The current input set.
     u_Data * Input_Set;
     
     //The current input set CIDs
     u_Data * Input_Set_CID;
     
     //The current input set charges
     u_Data * Input_Set_Charges;
     
     //--     TABLES
     
     //The Treetop output table
     c_Table_1D * Treetop_Table;
     
     
     c_MT_Base_CAN_1D(c_Base_Node_Network_1D * p_Node_Network, c_Charging_Buffer_1D * p_Charging_Buffer, c_Table_1D * p_Treetop_Table, ofstream * p_BenchMark)
     {
          //The node network.
          Node_Network = p_Node_Network;
          
          //The charging buffer to use.
          Charging_Buffer = p_Charging_Buffer;
          
          //Create the array to hold the nodes.
          CAN_Nodes = new c_Base_Node_1D**[1];
          CAN_Nodes[0] = new c_Base_Node_1D*[1];
          CAN_Nodes[0] [0] = NULL;
          
          Number_Of_Tiers = 0;
          
          //Currently no input set.
          Input_Length = 0;
          Input_Set = NULL;
          Input_Set_Charges = NULL;
          Input_Set_CID = NULL;
          
          //Treetop table.
          Treetop_Table = p_Treetop_Table;
          
          //The Benchmark File.
          BenchMark = p_BenchMark;
     }
     
     ~c_MT_Base_CAN_1D()
     {
          //Deallocate the CAN.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               delete [] CAN_Nodes[cou_T];
               CAN_Nodes[cou_T] = NULL;
          }
          delete [] CAN_Nodes;
          CAN_Nodes = NULL;
          
          delete [] Input_Set;
          Input_Set = NULL;
          
          delete [] Input_Set_Charges;
          Input_Set_Charges = NULL;
     }
     
     //Accepts an array of u_Data to read into the input set.
     void submit_Input_Set(u_Data p_Input_Set[], int p_Input_Length)
     {
          
          //Expands the input the appropriate length.
          expand_Input_Set(p_Input_Length);
          
          //Read the input set into the current input set.
          for (int cou_Index=0;cou_Index<p_Input_Length;cou_Index++)
          {
               Input_Set[cou_Index].NR = p_Input_Set[cou_Index].NR;
          }
          
          //Now expand the CAN to accomodate the input set.
          expand_CAN_Array(p_Input_Length);
     }
     
     //Accepts an array of u_Data to read into the input set for chrono building.
     void submit_Chrono_Input_Set(u_Data p_Input_Set[], int p_Input_Length)
     {
          
          //Expands the input the appropriate length.
          expand_Input_Set(p_Input_Length);
          
          //Read the input set into the current input set.
          for (int cou_Index=0;cou_Index<p_Input_Length;cou_Index++)
          {
               Input_Set[cou_Index].NR = p_Input_Set[cou_Index].NR;
          }
          
          //Now expand the CAN to accomodate the input set.
          expand_CAN_Array_For_Chrono(p_Input_Length);
     }
     
     //Accepts an array of u_Data to read into the input charges
     void submit_Input_Set_CID(u_Data p_Input_Set_CID[], int p_Input_Length)
     {
          //Read the input set into the current input set.
          for (int cou_Index=0;cou_Index<p_Input_Length;cou_Index++)
          {
               Input_Set_CID[cou_Index].NR = p_Input_Set_CID[cou_Index].NR;
          }
     }
     
     //Accepts an array of u_Data to read into the input charges
     void submit_Input_Set_Charges(u_Data p_Input_Set_Charges[], int p_Input_Length)
     {
          //Read the input set into the current input set.
          for (int cou_Index=0;cou_Index<p_Input_Length;cou_Index++)
          {
               Input_Set_Charges[cou_Index].NR = p_Input_Set_Charges[cou_Index].NR;
          }
     }
     
     //Builds the CAN in a chronological manner.
     //The p_Base_Chrono is the first link in the chrono chain.
     void build_Chrono(c_Base_Node_1D * p_Base_Chrono_Link)
     {

          //Node_Network->output_Number_Of_Nodes();
          //output_Input_Set();
          //cout << "\n\t" << Input_Length;
          
          if (Input_Length == 0){ return; }
          
          //Builds the state nodes.
          for (int cou_Index=0;cou_Index<Input_Length;cou_Index++)
          {
               CAN_Nodes[0] [cou_Index] = Node_Network->get_State_Node(Input_Set[cou_Index]);
          }
          
          //Build the first chrono node.
          CAN_Nodes[1] [0] = Node_Network->get_Chrono_Connection(p_Base_Chrono_Link, CAN_Nodes[0] [0]);
          
          //Build the chrono tier.
          for (int cou_NID=1;cou_NID<Input_Length;cou_NID++)
          {
               CAN_Nodes[1] [cou_NID] = Node_Network->get_Chrono_Connection(CAN_Nodes[1] [cou_NID - 1], CAN_Nodes[0] [cou_NID]);
               
               //Submit the treetop node to the treetop table.
               Treetop_Table->push_Data_Bit_Onto_Current_Row_I((CAN_Nodes[1] [cou_NID]->get_Treetop()).I);
          }
          
          
          
     }
     
     //Builds the CAN, full.
     void build_Full()
     {
          //Node_Network->output_Number_Of_Nodes();
          //output_Input_Set();
          //cout << "\n\t" << Input_Length;
          
          if (Input_Length == 0){ return; }
          
          //If there is only one tier then the state nodes are the Treetop nodes.
          if (Input_Length == 1)
          {
               //Builds the state node
               CAN_Nodes[0] [0] = Node_Network->get_Treetop_State_Node(Input_Set[0]);
               
               //Submit the treetop node to the treetop table.
               Treetop_Table->push_Data_Bit_Onto_Current_Row_I((CAN_Nodes[0] [0]->get_Treetop()).I);
               
               return;
          }
          
          //Builds the state nodes.
          for (int cou_Index=0;cou_Index<Input_Length;cou_Index++)
          {
               CAN_Nodes[0] [cou_Index] = Node_Network->get_State_Node(Input_Set[cou_Index]);
          }
          
          //Build the upper tiers. (Number_Of_Tiers - 1) because we are doing .get_Upper_Tier_Connection().
          for (int cou_T=0;cou_T<Number_Of_Tiers - 2;cou_T++)
          {
               for (int cou_NID=0;cou_NID<(Number_Of_Tiers - cou_T - 1);cou_NID++)
               {
                    CAN_Nodes[cou_T + 1] [cou_NID] = Node_Network->get_Upper_Tier_Connection(cou_T + 1, CAN_Nodes[cou_T] [cou_NID], CAN_Nodes[cou_T] [cou_NID + 1]);
               }
          }
          
          //Get the treetop connection.
          CAN_Nodes[Number_Of_Tiers - 1] [0] = Node_Network->get_Treetop_Connection(Number_Of_Tiers - 1, CAN_Nodes[Number_Of_Tiers - 2] [0], CAN_Nodes[Number_Of_Tiers - 2] [1]);
          
          //Submit the treetop node to the treetop table.
          Treetop_Table->push_Data_Bit_Onto_Current_Row_I((CAN_Nodes[Number_Of_Tiers - 1] [0]->get_Treetop()).I);
          
     }
     
     //Builds the CAN, full RC.
     void build_Full_RC()
     {
          if (Input_Length == 0){ return; }
          
          //If there is only one tier then the state nodes are the Treetop nodes.
          if (Input_Length == 1)
          {
               //Builds the state node
               CAN_Nodes[0] [0] = Node_Network->get_Treetop_RC_State_Node(Input_Set[0]);
               
               //Submit the treetop node to the treetop table.
               Treetop_Table->push_Data_Bit_Onto_Current_Row_I((CAN_Nodes[0] [0]->get_Treetop()).I);
               
               return;
          }
          
          //Builds the state nodes.
          for (int cou_Index=0;cou_Index<Input_Length;cou_Index++)
          {
               CAN_Nodes[0] [cou_Index] = Node_Network->get_RC_State_Node(Input_Set[cou_Index]);
          }
          
          //Build the upper tiers. (Number_Of_Tiers - 1) because we are doing .get_Upper_Tier_Connection().
          for (int cou_T=0;cou_T<Number_Of_Tiers - 2;cou_T++)
          {
               for (int cou_NID=0;cou_NID<(Number_Of_Tiers - cou_T - 1);cou_NID++)
               {
                    CAN_Nodes[cou_T + 1] [cou_NID] = Node_Network->get_Upper_Tier_Connection_RC(cou_T + 1, CAN_Nodes[cou_T] [cou_NID], CAN_Nodes[cou_T] [cou_NID + 1]);
               }
          }
          
          //Get the treetop connection.
          CAN_Nodes[Number_Of_Tiers - 1] [0] = Node_Network->get_Treetop_RC_Connection(Number_Of_Tiers - 1, CAN_Nodes[Number_Of_Tiers - 2] [0], CAN_Nodes[Number_Of_Tiers - 2] [1]);
          
          
          //Reinforce the nodes.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               for (int cou_NID=0;cou_NID<(Number_Of_Tiers - cou_T);cou_NID++)
               {
                    CAN_Nodes[cou_T] [cou_NID]->reinforce();
               }
          }
          //Submit the treetop node to the treetop table.
          Treetop_Table->push_Data_Bit_Onto_Current_Row_I((CAN_Nodes[Number_Of_Tiers - 1] [0]->get_Treetop()).I);
     }
     
     //Builds the CAN, full.
     void build_Full_BM()
     {
          //Node_Network->output_Number_Of_Nodes();
          //output_Input_Set();
          //cout << "\n\t" << Input_Length;
          
          //The timer for benchmarking.
          clock_t tmp_Timer = clock();
          clock_t tmp_Timer_Total = tmp_Timer;
          
          if (Input_Length == 0){ return; }
          
          //If there is only one tier then the state nodes are the Treetop nodes.
          if (Input_Length == 1)
          {
               //Builds the state node
               CAN_Nodes[0] [0] = Node_Network->get_Treetop_State_Node(Input_Set[0]);
               
               //Submit the treetop node to the treetop table.
               Treetop_Table->push_Data_Bit_Onto_Current_Row_I((CAN_Nodes[0] [0]->get_Treetop()).I);
               
               return;
          }
          
          //*BenchMark << "\n  Gathering State Nodes...";
          tmp_Timer = clock();
          
          //Builds the state nodes.
          for (int cou_Index=0;cou_Index<Input_Length;cou_Index++)
          {
               CAN_Nodes[0] [cou_Index] = Node_Network->get_State_Node(Input_Set[cou_Index]);
          }
          
          tmp_Timer = clock() - tmp_Timer;
          
          *BenchMark << "\n  State Nodes Took " << tmp_Timer << " Cycles";
          
          tmp_Timer = clock();
          
          //Build the upper tiers. (Number_Of_Tiers - 1) because we are doing .get_Upper_Tier_Connection().
          for (int cou_T=0;cou_T<Number_Of_Tiers - 2;cou_T++)
          {
               
               for (int cou_NID=0;cou_NID<(Number_Of_Tiers - cou_T - 1);cou_NID++)
               {
                    CAN_Nodes[cou_T + 1] [cou_NID] = Node_Network->get_Upper_Tier_Connection(cou_T + 1, CAN_Nodes[cou_T] [cou_NID], CAN_Nodes[cou_T] [cou_NID + 1]);
               }
          }
          
          tmp_Timer = clock() - tmp_Timer;
          
          *BenchMark << "  Building " << Number_Of_Tiers - 2 << " Took " << tmp_Timer << " Cycles.";
          
          tmp_Timer = clock();
          
          //Get the treetop connection.
          CAN_Nodes[Number_Of_Tiers - 1] [0] = Node_Network->get_Treetop_Connection(Number_Of_Tiers - 1, CAN_Nodes[Number_Of_Tiers - 2] [0], CAN_Nodes[Number_Of_Tiers - 2] [1]);
          
          tmp_Timer = clock() - tmp_Timer;
          
          *BenchMark << "\n  Gathering The Treetop Took " << tmp_Timer << " Cycles.";
          
          tmp_Timer_Total = tmp_Timer_Total - clock();
          
          *BenchMark << "\n  Total Time->" << tmp_Timer_Total;
          
          //Submit the treetop node to the treetop table.
          Treetop_Table->push_Data_Bit_Onto_Current_Row_I((CAN_Nodes[Number_Of_Tiers - 1] [0]->get_Treetop()).I);
          
     }
     
     //Builds the CAN, full RC.
     void build_Full_RC_BM()
     {
          //The timer for benchmarking.
          clock_t tmp_Timer = clock();
          
          clock_t tmp_Timer_Total = tmp_Timer;
          
          if (Input_Length == 0){ return; }
          
          //If there is only one tier then the state nodes are the Treetop nodes.
          if (Input_Length == 1)
          {
               //Builds the state node
               CAN_Nodes[0] [0] = Node_Network->get_Treetop_RC_State_Node(Input_Set[0]);
               
               //Submit the treetop node to the treetop table.
               Treetop_Table->push_Data_Bit_Onto_Current_Row_I((CAN_Nodes[0] [0]->get_Treetop()).I);
               
               return;
          }
          
          //*BenchMark << "\n  Gathering State Nodes...";
          tmp_Timer = clock();
          
          
          //Builds the state nodes.
          for (int cou_Index=0;cou_Index<Input_Length;cou_Index++)
          {
               CAN_Nodes[0] [cou_Index] = Node_Network->get_RC_State_Node(Input_Set[cou_Index]);
          }
          
          tmp_Timer = clock() - tmp_Timer;
          
          *BenchMark << "\n  State Nodes Took " << tmp_Timer << " Cycles";
          
          
          
          //Build the upper tiers. (Number_Of_Tiers - 1) because we are doing .get_Upper_Tier_Connection().
          for (int cou_T=0;cou_T<Number_Of_Tiers - 2;cou_T++)
          {
               tmp_Timer = clock();
               
               for (int cou_NID=0;cou_NID<(Number_Of_Tiers - cou_T - 1);cou_NID++)
               {
                    CAN_Nodes[cou_T + 1] [cou_NID] = Node_Network->get_Upper_Tier_Connection_RC(cou_T + 1, CAN_Nodes[cou_T] [cou_NID], CAN_Nodes[cou_T] [cou_NID + 1]);
               }
               
               tmp_Timer = clock() - tmp_Timer;
               
               *BenchMark << "  Tier[" << cou_T << "] Took " << tmp_Timer << " Cycles.";
          }
          
          tmp_Timer = clock();
          
          //Get the treetop connection.
          CAN_Nodes[Number_Of_Tiers - 1] [0] = Node_Network->get_Treetop_RC_Connection(Number_Of_Tiers - 1, CAN_Nodes[Number_Of_Tiers - 2] [0], CAN_Nodes[Number_Of_Tiers - 2] [1]);
          
          tmp_Timer = clock() - tmp_Timer;
          
          *BenchMark << "\n  Gathering The Treetop Took " << tmp_Timer << " Cycles.";
          
          tmp_Timer = clock();
          
          //Reinforce the nodes.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               for (int cou_NID=0;cou_NID<(Number_Of_Tiers - cou_T);cou_NID++)
               {
                    CAN_Nodes[cou_T] [cou_NID]->reinforce();
               }
          }
          
          tmp_Timer = clock() - tmp_Timer;
          
          *BenchMark << "\n  Reinforcement Took " << tmp_Timer << " Cycles.";
          
          tmp_Timer_Total = tmp_Timer_Total - clock();
          
          *BenchMark << "\n  Total Time->" << tmp_Timer_Total;
          
          //Submit the treetop node to the treetop table.
          Treetop_Table->push_Data_Bit_Onto_Current_Row_I((CAN_Nodes[Number_Of_Tiers - 1] [0]->get_Treetop()).I);
     }
          
     //Builds the CAN, full.
     void build_Query()
     {
          //Builds the state nodes.
          for (int cou_Index=0;cou_Index<Input_Length;cou_Index++)
          {
               CAN_Nodes[0] [cou_Index] = Node_Network->get_State_Node(Input_Set[cou_Index]);
          }
          
          //Build the upper tiers. (Number_Of_Tiers - 1) because we are doing .get_Upper_Tier_Connection().
          for (int cou_T=0;cou_T<Number_Of_Tiers - 1;cou_T++)
          {
               for (int cou_NID=0;cou_NID<(Number_Of_Tiers - cou_T - 1);cou_NID++)
               {
                    CAN_Nodes[cou_T + 1] [cou_NID] = Node_Network->does_Upper_Tier_Connection_Exist(CAN_Nodes[cou_T] [cou_NID], CAN_Nodes[cou_T] [cou_NID + 1]);
               }
          }
          
          //output_CAN();
     }
     
     
     //Charges the input tier with the charges from the passed array.
     void charge_Tier_With_Charge_Set(int p_Tier)
     {
          //If the given tier does not exist then return.
          if (p_Tier >= Number_Of_Tiers){ return; }
          
          //Read the nodes in the given Tier into the charging buffer with the CANs charge.
          for (int cou_NID=0;cou_NID<(Number_Of_Tiers - p_Tier);cou_NID++)
          {
               Charging_Buffer->submit(CAN_Nodes[p_Tier] [cou_NID], Input_Set_Charges[cou_NID].I);
          }
     }
     
     //Charges the given tier into the given charging_Buffer.
     void charge_Tier(int p_Tier, unsigned int p_Charge = 1000)
     {
          //If the given tier does not exist then return.
          if (p_Tier >= Number_Of_Tiers){ return; }
          
          //Read the nodes in the given Tier into the charging buffer with the CANs charge.
          for (int cou_NID=0;cou_NID<(Number_Of_Tiers - p_Tier);cou_NID++)
          {
               Charging_Buffer->submit(CAN_Nodes[p_Tier] [cou_NID], p_Charge);
          }
     }
     
     
     //--     ARRAY EXPANSION FUNCTIONS
     
     //Increases the number of indexes in the input array to accept the current input.
     void expand_Input_Set(int p_Input_Length)
     {
          //Just to be safe we will delete what is there if anything.
          delete [] Input_Set;
          Input_Set = NULL;
          
          delete [] Input_Set_Charges;
          Input_Set_Charges = NULL;
          
          delete [] Input_Set_CID;
          Input_Set_CID = NULL;
          
          //Set the input length to the correct amount.
          Input_Length = p_Input_Length;
          
          //Allocate the input array and zero the array.
          Input_Set = new u_Data [Input_Length];
          Input_Set_CID = new u_Data [Input_Length];
          Input_Set_Charges = new u_Data [Input_Length];
          
          
          for (int cou_Index=0;cou_Index<Input_Length;cou_Index++)
          {
               Input_Set[cou_Index].I = 0;
               Input_Set_CID[cou_Index].I = 0;
               Input_Set_Charges[cou_Index].I = 0;
          }
     }
     
     
     //Increases the number of tiers in the CAN_Node array.
     void expand_CAN_Array(int p_Tier)
     {
          //If the CAN is not empty then reset it to prep for expansion.
          if (CAN_Nodes != NULL){ reset_CAN(); }
      
          //Set the number of tiers to the given tier count.
          Number_Of_Tiers = p_Tier;
    
          //Allocate the appropriate number of tiers.
          CAN_Nodes = new c_Base_Node_1D ** [Number_Of_Tiers];
          
          //Allocate the nodes for the tiers, the number of nodes in a tier is (Number_Of_Tiers - Current_Tier).
          //This works because for each tier up, one less node is needed.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               CAN_Nodes[cou_T] = new c_Base_Node_1D * [Number_Of_Tiers - cou_T];
               
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    CAN_Nodes[cou_T] [cou_Index] = NULL;
               }
          }
          
     }
          
     //Increases the number of tiers in the CAN_Node array for chronological building.
     void expand_CAN_Array_For_Chrono(int p_Input_Length)
     {
          //If the CAN is not empty then reset it to prep for expansion.
          if (CAN_Nodes != NULL){ reset_CAN(); }
      
          //Set the number of tiers to the given tier count.
          Number_Of_Tiers = 2;
    
          //Allocate the appropriate number of tiers.
          CAN_Nodes = new c_Base_Node_1D ** [Number_Of_Tiers];
          
          //Allocate the nodes for the tiers, the number of nodes in a tier is (Number_Of_Tiers - Current_Tier).
          //This works because for each tier up, one less node is needed.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               CAN_Nodes[cou_T] = new c_Base_Node_1D * [p_Input_Length];
               
               for (int cou_Index=0;cou_Index<p_Input_Length;cou_Index++)
               {
                    CAN_Nodes[cou_T] [cou_Index] = NULL;
               }
          }
          
     }
     
     //--     ARRAY RESET FUNCTIONS
     
     //Resets the CAN nodes.
     void reset_CAN()
     {
          //Remove the nodes in the tiers.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               delete [] CAN_Nodes[cou_T];
               CAN_Nodes[cou_T] = NULL;
          }
          
          //Delete the CAN_Nodes array.
          delete [] CAN_Nodes;
          CAN_Nodes = NULL;
          
          //Reallocate the array back to a single NULL index.
          CAN_Nodes = NULL;
          
          Number_Of_Tiers = 0;
          
          
     }
     
     //Resets the input set.
     void reset_Input_Set()
     {
          //Delete the input set.
          delete [] Input_Set;
          Input_Set = NULL;
          
          //Delete the charges array.
          delete [] Input_Set_CID;
          Input_Set_CID = NULL;
          
          //Delete the charges array.
          delete [] Input_Set_Charges;
          Input_Set_Charges = NULL;
          
          //No Inputs.
          Input_Length = 0;
     }
     
     
     //--     OUTPUT FUNCTIONS
     
     
     //Outputs the CAN.
     void output_CAN()
     {
          cout << "\n\n\t----------";
          cout << "\n\t CAN->" << this;
          cout << "\n\t  #Tiers->" << Number_Of_Tiers;
          output_Input_Set();
          output_CAN_Nodes();
     }
     
     //Output the input set.
     void output_Input_Set()
     {
          cout << "\n\t Input_Length->" << Input_Length << "\n\n";
          cout << "\n\t Input_Set->";
          for (int cou_Index=0;cou_Index<Input_Length;cou_Index++)
          {
               cout << " [" << cou_Index << "]" << char (Input_Set[cou_Index].I);
          }
          
          if (Input_Set_Charges != NULL)
          {
               cout << "\n\t Input_Set_Charges->";
               for (int cou_Index=0;cou_Index<Input_Length;cou_Index++)
               {
                    cout << " [" << cou_Index << "]" << Input_Set_Charges[cou_Index].I;
               }
          }
               
     }
     
     //Outputs the CAN nodes.
     void output_CAN_Nodes()
     {
          cout << "\n\n";
          //Loop through the tiers and output all the nodes in each tier.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n";
               for (int cou_NID=0;cou_NID<(Number_Of_Tiers - cou_T);cou_NID++)
               {
                    cout << "<" << CAN_Nodes[cou_T] [cou_NID] << "> ";
               }
          }
     }
     

};
