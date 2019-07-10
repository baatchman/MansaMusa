

//   The CAN has several Nodes, one for each leg, that are
// set each tick of the CAN. These can be retrieved by reference, the
// purpose of these nodes is that they are the ones being evaluated for 
// upper tier connections. When no nodes remain to be built the CAN returns
// 0. 
class c_Poly_CAN
{
public:
     
     //The c_Poly_Node_Network that this CAN will operate upon.
     c_Poly_Network Node_Network;
     
     //The name of the construct.
     string Name;
     
     //The ID of the construct.
     int CID;
     
     //The coordinates of the construct.
     COORD Position;
     
     //The output for the current CAN.
     c_1D_Output Output_Address;
     
     //The dimensions of the CAN.
     int Dimensions;
     
     //The current input depth, which is used to determine the number of tiers.
     int Input_Depth;
     
     //The current set of input states.
     u_IO_Data * Current_Input_State;
     
     //The current number of inputs the CAN is build upon.
     int Current_Input_Depth;
     
     //The CAN nodes
     c_Poly_CAN_Node *** CAN_Nodes;
     
     //The current treetop node reference.
     void* Treetop;
     
     //The current number of CAN_Nodes.
     int CAN_Nodes_Current_Count;
     
     //The current number of indexes allocated for the nodes array.
     int CAN_Nodes_Max_Count;
     
     //The construct type, determines the output functions. 0 is the default construct type with state nodes, 1 is a construct with reference nodes.
     int Construct_Type;
     
     //The input type for the construct, 0: Normal, 1: Normal_Chrono, 2: Reference, 3: Reference_Chrono
     int Input_Type;
     
     
     //---    Tables and related members.
     c_Table_Handler * Tables;
     
     //Input Table.
     int Input_Table;
     
     //Ouput Table;
     int Output_Table;
     
     //Properties Table, holds the properties for the construct.
     int Properties_Table;
     
     //Settings Table, holds the settings for the construct.
     int Settings_Table;
     
     //Treetop Table;
     int Treetop_Table;
     
     //Torch node Table.
     int Torch_Table;
     
     //The constructor.
     c_Poly_CAN(int p_CID, c_Table_Handler * p_Tables, string p_Construct_Name = "Generic_Construct", int p_Construct_Type = 0)
     {
          //Initialize the member variables.
          Name = p_Construct_Name;
          CID = p_CID;
          Tables = p_Tables;  
          Dimensions = 0;
          Input_Depth = 0;
          Current_Input_State = NULL;
          Current_Input_Depth = 0;
          CAN_Nodes = NULL;
          CAN_Nodes_Current_Count = 0;
          CAN_Nodes_Max_Count = 0;
          Treetop = NULL;  
          Input_Type = 0;
          Construct_Type = p_Construct_Type;
          
          Node_Network.Output_Address = &Output_Address;
          Node_Network.set_Table_Handler(Tables);
          Node_Network.CID = CID;
          
          Position.X = 0;
          Position.Y = 0;
          
          if (p_Tables == NULL){ p_Tables = new c_Table_Handler; }
          
          string tmp_Table_Name;
     
          Tables = p_Tables;
          Input_Table = Tables->new_Table();
          tmp_Table_Name = Name + ": Input";
          Tables->set_Table_Name(tmp_Table_Name, Input_Table);
          
          Output_Table = Tables->new_Table();
          tmp_Table_Name = Name + ": Output";
          Tables->set_Table_Name(tmp_Table_Name, Output_Table);
          Node_Network.set_Output_Table(Output_Table);
          
          Properties_Table = Tables->new_Table();
          tmp_Table_Name = Name + ": Properties";
          Tables->set_Table_Name(tmp_Table_Name, Properties_Table);
          
          Settings_Table = Tables->new_Table();
          tmp_Table_Name = Name + ": Settings";
          Tables->set_Table_Name(tmp_Table_Name, Settings_Table);
          
          create_Settings_Table();
          
          Treetop_Table = Tables->new_Table();
          tmp_Table_Name = Name + ": Treetops";
          Tables->set_Table_Name(tmp_Table_Name, Treetop_Table);
          
          Torch_Table = Tables->new_Table();
          tmp_Table_Name = Name + ": Torches";
          Tables->set_Table_Name(tmp_Table_Name, Torch_Table);
          Node_Network.set_Torch_Table(Torch_Table);
          
          update_All_Tables_From_Internal();
          
     }
     
     //The destructor.
     ~c_Poly_CAN()
     {
          delete [] Current_Input_State;
          delete [] CAN_Nodes;
     }
     
     //Sets the constructs name.
     void set_Construct_Name(string p_Construct_Name = "Generic_Construct")
     {
          Name = p_Construct_Name;
     }
     
     //Sets the output address.
     void set_Output_Adress(void * p_Output_Address = NULL)
     {
          //Node_Network.Output_Address = p_Output_Address;
          //Output_Address = p_Output_Address;
     }
     
     //Resets the CAN array.
     void reset_CAN()
     {
//          cout << "\n\t\t reset_CAN()"; cout.flush();
          //NULL all the CAN references and delete the CAN array.
          
          //Sets the Input_Length to 0.
     }
     
     //Rebuilds the CAN to match the given input depth, used for building when the depth is known.
     //The input depth needs to be set independantly of this function.
     void rebuild_CAN_Array()
     {
//          cout << "\n\t\t rebuild_CAN_Array()"; cout.flush();
          
          //   As a result of the pyramid structure of the CAN, the number of nodes in each
          // layer equals ( (Input_Depth ^ Dimension) - (Tier ^ Dimension) ), assuming square layers.
          // The number of tiers in the network is equal to the largest input depth.
          
          //If the CAN_Nodes are already allocated then delete them.
          if (CAN_Nodes != NULL)
          {
               //Loop through all tiers deleting them.
               for (int cou_T=0;cou_T<Current_Input_Depth;cou_T++){
                    
                    //NULL the nodes in each tier.
                    for (int cou_Index=0;cou_Index<(Current_Input_Depth - cou_T);cou_Index++){
                         //CAN_Nodes[cou_T] [cou_Index] = NULL;
                         delete CAN_Nodes[cou_T] [cou_Index];
                    }
               
                    //Delete the current tier.
                    delete [] CAN_Nodes[cou_T];
                    CAN_Nodes[cou_T] = NULL;
               }
               
               //Delete the CAN_Nodes so that they can be rebuilt.
               delete CAN_Nodes;
               CAN_Nodes = NULL;
          }
          
          //   Create the CAN tiers, then create the nodes in each layer.
          // In this case the Input_Depth is the number of tiers.
          CAN_Nodes = new c_Poly_CAN_Node**[Input_Depth];
          
          //   Get the current number of inputs the CAN is built on.
          Current_Input_Depth = Input_Depth;
          
          //   Create the nodes in each tier.
          for (int cou_T=0;cou_T<Input_Depth;cou_T++){
               CAN_Nodes[cou_T] = new c_Poly_CAN_Node*[(Input_Depth - cou_T)];
               
               //NULL the nodes in each tier.
               for (int cou_Index=0;cou_Index<(Input_Depth - cou_T);cou_Index++){
                    //CAN_Nodes[cou_T] [cou_Index] = NULL;
                    CAN_Nodes[cou_T] [cou_Index] = new c_Poly_CAN_Node;
               }
          }
     }
     
     //Rebuilds the CAN to match the given input depth and copies the information from the old one, used for resizing the CAN.
     //The input depth needs to be set independantly of this function.
     void rebuild_CAN_Array_Copy_Data()
     {
//          cout << "\n\t\t rebuild_CAN_Array_Copy_Data()"; cout.flush();
          
          //   As a result of the pyramid structure of the CAN, the number of nodes in each
          // layer equals ( (Input_Depth ^ Dimension) - (Tier ^ Dimension) ), assuming square layers.
          // The number of tiers in the network is equal to the largest input depth.
          
          //The tmp_CAN_Nodes will be the new CAN once the data is copied.
          c_Poly_CAN_Node *** tmp_CAN_Nodes;
          
          //   Create the tmp_CAN tiers, then create the nodes in each layer.
          // In this case the Input_Depth is the number of tiers.
          tmp_CAN_Nodes = new c_Poly_CAN_Node**[Input_Depth];
          
          //   Create the nodes in each tier.
          for (int cou_T=0;cou_T<Input_Depth;cou_T++){
               tmp_CAN_Nodes[cou_T] = new c_Poly_CAN_Node*[(Input_Depth - cou_T)];
               
               //NULL the temporary nodes.
               for (int cou_Index=0;cou_Index<(Input_Depth - cou_T);cou_Index++){
                    
                    //NULL the newly created nodes.
                    tmp_CAN_Nodes[cou_T] [cou_Index] = NULL;
               }
          }
          
          //   Loop through the original CAN to copy the data to the new one.
          for (int cou_T=0;cou_T<Current_Input_Depth;cou_T++){
               
               //Copy the nodes in each tier into the newly allocated tmp_CAN_Nodes, then NULL the original.
               for (int cou_Index=0;cou_Index<(Current_Input_Depth - cou_T);cou_Index++){
                    
                    //Copy the nodes into the temporary nodes.
                    tmp_CAN_Nodes[cou_T] [cou_Index] = CAN_Nodes[cou_T] [cou_Index];
                    
                    //NULL the original so that the CAN node itself is not deleted.
                    CAN_Nodes[cou_T] [cou_Index] = NULL;
               }
          }
          
          //Delete the original array
          //Loop through all tiers deleting them.
          for (int cou_T=0;cou_T<Current_Input_Depth;cou_T++){
               
               //Delete the current tier.
               delete [] CAN_Nodes[cou_T];
          }
               
          //Delete the CAN_Nodes so that they can be rebuilt.
          delete CAN_Nodes;
          
          //   Get the current number of inputs the CAN is built on.
          Current_Input_Depth = Input_Depth;
          
          // Set the original array to the newly created one.
          CAN_Nodes = tmp_CAN_Nodes;
     }
     
     //Takes an array of node references for the raw tier.
     void submit_CAN_Raw_Node_Array(u_IO_Data p_Data[], int p_Input_Depth)
     {
          //cout << "\n\t\t c_Poly_CAN::submit_CAN_Raw_Node_Array(u_IO_Data p_Data[], int p_Input_Depth:" << p_Input_Depth << ")"; cout.flush();
          /*
          cout << "\n\t\t\t p_Data[]:";
          for (int cou_Index=0;cou_Index<p_Input_Depth;cou_Index++)
          {
               cout << "[" << cou_Index << "]" << p_Data[cou_Index].I;
          }
          */
          
          //Set the input depth for the CAN rebuild.
          Input_Depth = p_Input_Depth;
          
          //Rebuild the CAN to the appropriate dimensions.
          rebuild_CAN_Array();
          
          //Deletes the input state array to prep for reallocation.
          delete [] Current_Input_State;
          Current_Input_State = NULL;
          
          //Reallocates the input state array.
          Current_Input_State = new u_IO_Data[Input_Depth];
          
          //Reads the p_Data into the raw tier.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++){
               //Queries the network binary tree with the p_Data to get the appropriate node references.
               CAN_Nodes[0] [cou_Index]->Current_Target_Node = Node_Network.get_State_Node(p_Data[cou_Index]);
               //cout << "\n\t\t\t -CAN_Nodes[0] [" << cou_Index << "](<" << CAN_Nodes[0] [cou_Index]->Current_Target_Node << ">)"; cout.flush();
               
               //Gathers the current state. 
               Current_Input_State[cou_Index].NR = p_Data[cou_Index].NR;
          }
          //cout << "\n\t\t\t ~Data read into raw tier."; cout.flush();
          
          
          //Builds the CAN.
          build_CAN();
     } 
     
     //Takes an array of node references for the raw tier.
     void submit_CAN_Raw_Node_Array_Query(u_IO_Data p_Data[], int p_Input_Depth)
     {
          //cout << "\n\t\t submit_CAN_Raw_Node_Array_Query(" << p_Input_Depth << ")"; cout.flush();
          
          //Set the input depth for the CAN rebuild.
          Input_Depth = p_Input_Depth;
          
          //Rebuild the CAN to the appropriate dimensions.
          rebuild_CAN_Array();
          
          //Deletes the input state array to prep for reallocation.
          delete [] Current_Input_State;
          
          //Reallocates the input state array.
          Current_Input_State = new u_IO_Data[Input_Depth];
          
          //Reads the p_Data into the raw tier.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++){
               //Queries the network binary tree with the p_Data to get the appropriate node references.
               CAN_Nodes[0] [cou_Index]->Current_Target_Node = Node_Network.get_State_Node(p_Data[cou_Index], false, true);
               //cout << "\n\t\t CAN_Nodes[0] [" << cou_Index << "](<" << CAN_Nodes[0] [cou_Index] << ">)"; cout.flush();
               //cout << "  CAN_Nodes[0] [" << cou_Index << "]->Current_Target_Node = " << CAN_Nodes[0] [cou_Index]->Current_Target_Node; cout.flush();
               
               //Gathers the current state.
               Current_Input_State[cou_Index].NR = p_Data[cou_Index].NR;
               //cout << "  c (" << char (Current_Input_State[cou_Index].I) << ") i (" << Current_Input_State[cou_Index].I << ")\n";
          }
          
          //Builds the CAN.
          //cout << "\n\t\t ~~build_CAN_For_Query(0)"; cout.flush();
          build_CAN_For_Query(0);
          
          //cout << "\n\t\t ~~Finished..."; cout.flush();
          
          //output_CAN();
          
          //((c_Poly_Node*) Treetop)->flg_Treetop = false;
          //Node_Network.Nodes_Treetop_Count--;
     } 
     
     //Takes an array of node references for the raw tier.
     void submit_CAN_Node_Reference_Array(void * p_Data[], int p_Input_Depth)
     {
          //cout << "\n\tc_Poly_CAN::submit_CAN_Node_Reference_Array(void * p_Data[], int p_Input_Depth:: " << p_Input_Depth << ") CID::" << CID;
          
          //Set the input depth for the CAN rebuild.
          Input_Depth = p_Input_Depth;
          
          //Rebuild the CAN to the appropriate dimensions.
          rebuild_CAN_Array();
          
          //Deletes the input state array to prep for reallocation.
          delete [] Current_Input_State;
          
          //Reallocates the input state array.
          Current_Input_State = new u_IO_Data[Input_Depth];
          
          //Reads the p_Data into the raw tier.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++){
               
               //Gathers the current state.
               Current_Input_State[cou_Index].NR = p_Data[cou_Index];
               
               //Queries the network binary tree with the p_Data to get the appropriate node references.
               //cout << "\n\t\t CAN_Nodes[0] [cou_Index](<" << CAN_Nodes[0] [cou_Index] << ">)"; cout.flush();
               CAN_Nodes[0] [cou_Index]->Current_Target_Node = Node_Network.get_State_Node(Current_Input_State[cou_Index], 1);
               
          }
          
          //Builds the CAN.
          build_CAN(0);
          
          //output_CAN();
          
          //((c_Poly_Node*) Treetop)->flg_Treetop = false;
          //Node_Network.Nodes_Treetop_Count--;
     } 
     
     //Takes an array of node references for the raw tier.
     void submit_CAN_Node_Reference_Array_Query(void * p_Data[], int p_Input_Depth)
     {
          
          //Set the input depth for the CAN rebuild.
          Input_Depth = p_Input_Depth;
          
          //Rebuild the CAN to the appropriate dimensions.
          rebuild_CAN_Array();
          
          //Deletes the input state array to prep for reallocation.
          delete [] Current_Input_State;
          
          //Reallocates the input state array.
          Current_Input_State = new u_IO_Data[Input_Depth];
          
          //Reads the p_Data into the raw tier.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++){
               
               //Gathers the current state.
               Current_Input_State[cou_Index].NR = p_Data[cou_Index];
               
               //Queries the network binary tree with the p_Data to get the appropriate node references.
               //cout << "\n\t\t CAN_Nodes[0] [cou_Index](<" << CAN_Nodes[0] [cou_Index] << ">)"; cout.flush();
               CAN_Nodes[0] [cou_Index]->Current_Target_Node = p_Data[cou_Index];
               
          }
          
          //Builds the CAN.
          build_CAN_For_Query(0);
          
          //output_CAN();
          
          //((c_Poly_Node*) Treetop)->flg_Treetop = false;
          //Node_Network.Nodes_Treetop_Count--;
     } 
     
     //This function builds the CAN starting at the raw tier.
     void build_CAN_For_Query(bool p_flg_Treetop = true)
     {
//          cout << "\n\t\t build_CAN()"; cout.flush();
          
          //The temporary nodes to query the network with.
          void * tmp_Nodes[2];
          
          //Prevent bounding errors with inputs less than 2.
          if (Input_Depth == 0){ return; }
          if (Input_Depth == 1)
          {
               Treetop = CAN_Nodes[0] [0];
               return;
          }
          
          //Loop through all of the tiers starting at the raw tier.
          //Input depth is equivelent to the number of tiers.
          //   The top tier is not evaluated because it does not have
          // nodes to connect to each other, it holds the "treetop" node.
          for (int cou_T=0;cou_T<(Input_Depth - 1);cou_T++){
               
               //   Loop through every node in the current tier sans the last node.
               // This is because the current node and the next one in the tier are checked
               // for upper level connections by exploring their axons. If the loop were to
               // go all the way to the last node then the check would be for a node that does not exist.
               for (int cou_Node=0;cou_Node<((Input_Depth - cou_T) - 1);cou_Node++){
                    //Gathers the nodes to query into one array.
                    tmp_Nodes[0] = CAN_Nodes[cou_T] [cou_Node]->Current_Target_Node;
                    tmp_Nodes[1] = CAN_Nodes[cou_T] [cou_Node + 1]->Current_Target_Node;
                    
                    //Checks to see if either of the nodes is NULL, if so it sets the upper tier connection to NULL and continues.
                    if (tmp_Nodes[0] == NULL || tmp_Nodes[1] == NULL){ CAN_Nodes[cou_T + 1] [cou_Node]->Current_Target_Node = NULL; continue; }
                    
                    //Gets an address to an upper level node and sets the upper CAN to that node.
                    CAN_Nodes[cou_T + 1] [cou_Node]->Current_Target_Node = Node_Network.does_Upper_Tier_Connection_Exist(tmp_Nodes, 2);
               }
          }
          
          
          //output_CAN();
          
          //Sets the treetop to the top CAN node.
          Treetop = CAN_Nodes[Input_Depth - 1] [0]->Current_Target_Node;
          //Tables->push_NR(Treetop,  Treetop_Table);
     }
     
     //This function builds the CAN starting at the raw tier.
     void build_CAN(bool p_flg_Treetop = true)
     {
          //cout << "\n\t\t c_Poly_CAN::build_CAN()"; cout.flush();
          //Prevent bounding errors from 0 length input.
          if (Input_Depth < 1){ Treetop = NULL; return; }
          

          //The temporary nodes to query the network with.
          void * tmp_Nodes[2];
          
          //Loop through all of the tiers starting at the raw tier.
          //Input depth is equivelent to the number of tiers.
          //   The top tier is not evaluated because it does not have
          // nodes to connect to each other, it holds the "treetop" node.
          for (int cou_T=0;cou_T<(Input_Depth - 1);cou_T++){
               
               //   Loop through every node in the current tier sans the last node.
               // This is because the current node and the next one in the tier are checked
               // for upper level connections by exploring their axons. If the loop were to
               // go all the way to the last node then the check would be for a node that does not exist.
               for (int cou_Node=0;cou_Node<((Input_Depth - cou_T) - 1);cou_Node++){
                    //Gathers the nodes to query into one array.
                    tmp_Nodes[0] = CAN_Nodes[cou_T] [cou_Node]->Current_Target_Node;
                    tmp_Nodes[1] = CAN_Nodes[cou_T] [cou_Node + 1]->Current_Target_Node;
                    
                    //Gets an address to an upper level node and sets the upper CAN to that node.
                    CAN_Nodes[cou_T + 1] [cou_Node]->Current_Target_Node = Node_Network.get_Upper_Tier_Node(tmp_Nodes, 2);
                    
                    //Sets the tier for the current CAN node.
                    ((c_Poly_Node*) CAN_Nodes[cou_T] [cou_Node]->Current_Target_Node)->Tier = cou_T;
               }
               //Sets the tier for the node left out in the CAN building run.
               ((c_Poly_Node*) CAN_Nodes[cou_T] [(Input_Depth - cou_T) - 1]->Current_Target_Node)->Tier = cou_T;
               
          }
          
          //Sets the tier for the node left out in the CAN building run.
          //cout << "\n\t CID::" << CID << " ((c_Poly_Node*) CAN_Nodes[(Input_Depth::" << Input_Depth << " - 1)] [0]->Current_Target_Node)->Tier = (Input_Depth - 1);";
          ((c_Poly_Node*) CAN_Nodes[(Input_Depth - 1)] [0]->Current_Target_Node)->Tier = (Input_Depth - 1);
          
          //Sets the treetop flag if necessary.
          if (!((c_Poly_Node*) CAN_Nodes[(Input_Depth - 1)] [0]->Current_Target_Node)->flg_Treetop && p_flg_Treetop)
          {
               //((c_Poly_Node*) CAN_Nodes[(Input_Depth - 1)] [0]->Current_Target_Node)->flg_Treetop = true;
               Node_Network.new_Treetop_Node(CAN_Nodes[(Input_Depth - 1)] [0]->Current_Target_Node);
          }
          
          
          
          //Loops through all of the tiers to set the states in them as well as reinforce the nodes.
          for (int cou_T=0;cou_T<Input_Depth;cou_T++){
               
               //Sets the states for all the nodes by looping through all of the nodes.
               for (int cou_Node=0;cou_Node<(Input_Depth - cou_T);cou_Node++){
                    
                    //Reinforces the current CAN_Node;
                    ((c_Poly_Node*) CAN_Nodes[cou_T] [cou_Node]->Current_Target_Node)->reinforce();
                    
                    //For readability.
//                    int tmp_RC = ((c_Poly_Node*) CAN_Nodes[cou_T] [cou_Node]->Current_Target_Node)->get_Reinforcement_Counter();
                    /*
                    //For the properties table. [10] is the highest Reinforcement_Counter and [11] is the lowerst.
                    if ( tmp_RC > Tables->get_Data_Item_Int(Properties_Table, 10, 0))
                    {
                         Tables->set_Data_Int(&tmp_RC, Properties_Table, 10, 0);
                    }
                    if ( tmp_RC < Tables->get_Data_Item_Int(Properties_Table, 11, 0))
                    {
                         Tables->set_Data_Int(&tmp_RC, Properties_Table, 11, 0);
                    }
                    */
                    
                    
                    //If the current node does not have a state set then set it to the appropriate state.
                    if (!((c_Poly_Node*) CAN_Nodes[cou_T] [cou_Node]->Current_Target_Node)->State_Length)
                    {
                         //Temporary state array to hold the submission.
                         u_IO_Data * tmp_State_For_Node;
                         
                         //The length of the state is equal to the current tier + 1;
                         tmp_State_For_Node = new u_IO_Data[(cou_T + 1)];
                         
                         //Collect the states into the temporary array.
                         for (int cou_State=0;cou_State<(cou_T + 1);cou_State++){
                              tmp_State_For_Node[cou_State].NR = Current_Input_State[cou_State + cou_Node].NR;
                         }
                         
                         //Sets the current nodes State to the collected state.
                         ((c_Poly_Node*) CAN_Nodes[cou_T] [cou_Node]->Current_Target_Node)->set_State(tmp_State_For_Node, (cou_T + 1));
                         
                         //Cleanup
                         delete [] tmp_State_For_Node;
                         tmp_State_For_Node = NULL;
                    }
                    
               }
          }
           
          //Sets the treetop to the top CAN node.
          Treetop = CAN_Nodes[Input_Depth - 1] [0]->Current_Target_Node;
          u_IO_Data * tmp_Data = new u_IO_Data;
          tmp_Data->NR = Treetop;
          //Tables->push(tmp_Data, 1, Treetop_Table);
          
          //Cleanup
          delete tmp_Data;
          tmp_Data = NULL;
          
          //output_CAN();
          
          //cout << "\n\t\t\t ~Finished c_Poly_CAN::build_CAN()";
          
     }
     
     //Charges the CAN for a given number of times.
     void charge_CAN_Given_Times(int p_Charging_Tier, int p_Eval, int p_Recharge = 0, int p_Output_Info = 0, int p_Output_Graphical = 0)
     {
          //Have to neutralize the nodes first.
          Node_Network.neutralize_All_Nodes();
          
          cout << "\n";
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++){
               cout << "_";
          }
          cout << "\n";
          
          for (int cou_Index=0;cou_Index<p_Recharge;cou_Index++){
               cout << "\n";
               charge_CAN_Given_Tier(p_Charging_Tier, p_Output_Info, p_Output_Graphical, p_Recharge);
          }
          cout << "\n";
     }
     
     //Charges the CAN for a given number of times without neutralizing the network. This will allow for multiple queries.
     void charge_CAN_Given_Times_No_Network_Neutralize(int p_Charging_Tier, int p_Recharge, int p_Output_Info, int p_Output_Graphical, float p_Charge)
     {
          
          for (int cou_Index=0;cou_Index<p_Recharge;cou_Index++){
               charge_CAN_Given_Tier(p_Charging_Tier, p_Output_Info, p_Output_Graphical, p_Charge);
          }
     }
     
     //Charges the CAN.
     void charge_CAN(int p_Charging_Tier, int p_Output_Info, int p_Output_Graphical, float p_Charge)
     {
          //Loops through all of the tiers starting at the given charging tier.
          for (int cou_T=p_Charging_Tier;cou_T<Input_Depth;cou_T++){
               
               //Charges the curren tier.
               charge_CAN_Given_Tier(cou_T, p_Output_Info, p_Output_Graphical, p_Charge);
          }
     }
     
     //Charges only the given layer of the CAN, does not force the current pattern.
     void charge_CAN_Given_Tier(int p_Charging_Tier, int p_Output_Info, int p_Output_Graphical, float p_Charge)
     {
          //Loops through all the CAN nodes in the base layer.
          for (int cou_Node=0;cou_Node<(Input_Depth - p_Charging_Tier);cou_Node++){
               
               //Initiate an action potential in the current CAN node.
               if (CAN_Nodes[p_Charging_Tier] [cou_Node]->Current_Target_Node != NULL)
               {
                    ((c_Poly_Node*) CAN_Nodes[p_Charging_Tier] [cou_Node]->Current_Target_Node)->initiate_CAN_Action_Potential(p_Output_Info, p_Output_Graphical, p_Charging_Tier, p_Charge);
               }
          }
          
     }
     
     //Discharge the given node using backpropagation.
     void discharge_Given_Node(void * p_NID)
     {
          //Fire the backpropagation potential for the given node.
          ((c_Poly_Node*)p_NID)->initiate_Backpropagation_Both(1);
     }
     
     //Discharges the torch nodes in the Node_Network.
     void discharge_Torch_Nodes()
     {
          Node_Network.activate_Torch_Nodes();
     }
     
     //-- COMMAND FUNCTIONS
     
     //Builds the given Input_Table index.
     void build_Input_Index(int p_Index)
     {
          //cout << "\n\t\t c_Poly_CAN::build_Input_Index(" << p_Index << ")"; cout.flush();
          if (Construct_Type == 0)
          { 
               //cout << "\n\t\t\t -Construct_Type == 0 ~Data_Length:" << Tables->get_Data_Length(Input_Table, p_Index); cout.flush(); 
               //Tables->output_Specific_Cell(Input_Table, p_Index);
               if (!Tables->get_Data_Length(Input_Table, p_Index)){ return; }
               submit_CAN_Raw_Node_Array(Tables->get_Data(Input_Table, p_Index), Tables->get_Data_Length(Input_Table, p_Index));
               //cout << "\n\t\t\t -Finished";
          }
          
          if (Construct_Type == 1)
          {
               //cout << "\n\t\t\t -Construct_Type == 1"; cout.flush(); 
               if (!Tables->get_Data_Length(Input_Table, p_Index)){ return; }
               
               //Setup the temporary variables.
               int tmp_Data_Length = Tables->get_Data_Length(Input_Table, p_Index);
               u_IO_Data * tmp_Data_IO = Tables->get_Data(Input_Table, p_Index);
               void ** tmp_Data;
               tmp_Data = new void*[tmp_Data_Length];
               
               //Gather the node references from the table.
               for (int cou_Index=0;cou_Index<tmp_Data_Length;cou_Index++)
               {
                    tmp_Data[cou_Index] = tmp_Data_IO[cou_Index].NR;
               }
               submit_CAN_Node_Reference_Array(tmp_Data, tmp_Data_Length); 
               
               //Cleanup.
               delete [] tmp_Data;
          }
          
     }
     
     //Builds all of the inputs.
     void build_Inputs(int p_Method)
     {
          //cout << "\n\t\t c_Poly_CAN::build_Inputs()"; cout.flush();
          
          if (p_Method == 0)
          {    
               for (int cou_Index=0;cou_Index<Tables->get_Table_Item_Count(Input_Table);cou_Index++)
               {
                    build_Input_Index(cou_Index);
               }
          }
          
          if (p_Method == 1)
          {
               int tmp_Current_Index = 0;
               u_IO_Data * tmp_Current_Input_Set;
               int tmp_Input_Length = 0;
               u_IO_Data * tmp_Input_Data;
               
               //Gather the size of the tmp_Input_Array needed.
               for (int cou_Index=0;cou_Index<Tables->get_Table_Item_Count(Input_Table);cou_Index++)
               {
                    tmp_Input_Length += Tables->get_Data_Length(Input_Table, cou_Index);
               }
               
               //Setup the temporary array.
               tmp_Input_Data = new u_IO_Data[tmp_Input_Length];
               
               //Gather the inputs.
               for (int cou_Input=0;cou_Input<Tables->get_Table_Item_Count(Input_Table);cou_Input++)
               {
                    //Get the current input set.
                    tmp_Current_Input_Set = Tables->get_Data(Input_Table, cou_Input);
                    
                    //For each index in the current input set add it to the current input set being built.
                    for (int cou_Index=0;cou_Index<Tables->get_Data_Length(Input_Table, cou_Input);cou_Index++)
                    {
                         tmp_Input_Data[tmp_Current_Index].NR = tmp_Current_Input_Set[cou_Index].NR;
                         tmp_Current_Index++;
                    }
               }
               
               //Build the temporary inputs as one.
               if (Construct_Type == 0)
               {
                    submit_CAN_Raw_Node_Array(tmp_Input_Data, tmp_Input_Length);
               }
               
               void ** tmp_Input_Data_Reference;
               tmp_Input_Data_Reference = new void*[tmp_Current_Index];
               
               for (int cou_Index=0;cou_Index<tmp_Current_Index;cou_Index++)
               {
                    tmp_Input_Data_Reference[cou_Index] = tmp_Input_Data[cou_Index].NR;
               }
               
               if (Construct_Type == 1)
               {
                    submit_CAN_Node_Reference_Array(tmp_Input_Data_Reference, tmp_Input_Length); 
               }
               
               delete [] tmp_Input_Data;
               delete [] tmp_Input_Data_Reference;
               
               //cout << " ~Fin";
          }
          
          //Node_Network.index_Nodes_Condensed();
     }
     
     //Queries the given Input_Table index.
     void query_Input_Index(int p_Index)
     {
          //cout << "\n\t\t\t query_Input_Index(" << p_Index << ")"; cout.flush();
          
          if (Construct_Type == 0)
          { 
               //cout << "\n\t\t\t\t Construct_Type = 0"; cout.flush();
               submit_CAN_Raw_Node_Array_Query(Tables->get_Data(Input_Table, p_Index), Tables->get_Data_Length(Input_Table, p_Index)); 
          }
          if (Construct_Type == 1)
          {
               //Setup the temporary variables.
               int tmp_Data_Length = Tables->get_Data_Length(Input_Table, p_Index);
               u_IO_Data * tmp_Data_IO = Tables->get_Data(Input_Table, p_Index);
               void ** tmp_Data;
               tmp_Data = new void*[tmp_Data_Length];
               
               //Gather the node references from the table.
               for (int cou_Index=0;cou_Index<tmp_Data_Length;cou_Index++)
               {
                    tmp_Data[cou_Index] = tmp_Data_IO[cou_Index].NR;
               }
               submit_CAN_Node_Reference_Array_Query(tmp_Data, tmp_Data_Length); 
               
               //Cleanup.
               delete [] tmp_Data;
          }
          
     }
     
     //Queries all of the inputs.
     void query_Inputs(int p_Method)
     {
          if (p_Method == 0)
          {
               for (int cou_Index=0;cou_Index<Tables->get_Table_Item_Count(Input_Table);cou_Index++)
               {
                    query_Input_Index(cou_Index);
               }
          }
          if (p_Method == 1)
          {
               int tmp_Current_Index = 0;
               u_IO_Data * tmp_Current_Input_Set;
               int tmp_Input_Length = 0;
               u_IO_Data * tmp_Input_Data;
               
               //Gather the size of the tmp_Input_Array needed.
               for (int cou_Index=0;cou_Index<Tables->get_Table_Item_Count(Input_Table);cou_Index++)
               {
                    tmp_Input_Length += Tables->get_Data_Length(Input_Table, cou_Index);
               }
               
               //Setup the temporary array.
               tmp_Input_Data = new u_IO_Data[tmp_Input_Length];
               
               //Gather the inputs.
               for (int cou_Input=0;cou_Input<Tables->get_Table_Item_Count(Input_Table);cou_Input++)
               {
                    //Get the current input set.
                    tmp_Current_Input_Set = Tables->get_Data(Input_Table, cou_Input);
                    
                    //For each index in the current input set add it to the current input set being built.
                    for (int cou_Index=0;cou_Index<Tables->get_Data_Length(Input_Table, cou_Input);cou_Index++)
                    {
                         tmp_Input_Data[tmp_Current_Index].NR = tmp_Current_Input_Set[cou_Index].NR;
                         tmp_Current_Index++;
                    }
               }
               
               //Build the temporary inputs as one.
               if (Construct_Type == 0)
               {
                    submit_CAN_Raw_Node_Array_Query(tmp_Input_Data, tmp_Current_Index);
               }
               
               void ** tmp_Input_Data_Reference;
               tmp_Input_Data_Reference = new void*[tmp_Current_Index];
               
               for (int cou_Index=0;cou_Index<tmp_Current_Index;cou_Index++)
               {
                    tmp_Input_Data_Reference[cou_Index] = tmp_Input_Data[cou_Index].NR;
               }
               
               if (Construct_Type == 1)
               {
                    submit_CAN_Node_Reference_Array_Query(tmp_Input_Data_Reference, tmp_Current_Index); 
               }
               
               delete [] tmp_Input_Data;
               delete [] tmp_Input_Data_Reference;
          }
     }
     
     //Queries the given Input_Table index.
     void query_Input_Index_With_Build(int p_Index)
     {
          //cout << "\n\t\t\t query_Input_Index_With_Build(" << p_Index << ")"; cout.flush();
          
          if (Construct_Type == 0)
          { 
               //cout << "\n\t\t\t\t Construct_Type = 0"; cout.flush();
               submit_CAN_Raw_Node_Array(Tables->get_Data(Input_Table, p_Index), Tables->get_Data_Length(Input_Table, p_Index)); 
          }
          if (Construct_Type == 1)
          {
               //Setup the temporary variables.
               int tmp_Data_Length = Tables->get_Data_Length(Input_Table, p_Index);
               u_IO_Data * tmp_Data_IO = Tables->get_Data(Input_Table, p_Index);
               void ** tmp_Data;
               tmp_Data = new void*[tmp_Data_Length];
               
               //Gather the node references from the table.
               for (int cou_Index=0;cou_Index<tmp_Data_Length;cou_Index++)
               {
                    tmp_Data[cou_Index] = tmp_Data_IO[cou_Index].NR;
               }
               submit_CAN_Node_Reference_Array(tmp_Data, tmp_Data_Length); 
               
               //Cleanup.
               delete [] tmp_Data;
          }
          
     }
     
     //Queries all of the inputs.
     void query_Inputs_With_Build()
     {
          for (int cou_Index=0;cou_Index<Tables->get_Table_Item_Count(Input_Table);cou_Index++)
          {
               query_Input_Index_With_Build(cou_Index);
          }
     }
     
     //Reset the IO tables.
     void reset_IO_Tables()
     {
          Tables->reset_Table(Input_Table);
          
          //Deletes the input state array to prep for reallocation.
          delete [] Current_Input_State;
          Current_Input_State = NULL;
          Current_Input_Depth = 0;
          update_Settings_Table();
          
          Tables->reset_Table(Output_Table);
     }
          
     //Reset the I tables.
     void reset_I_Tables()
     {
          Tables->reset_Table(Input_Table); 
          
          //Deletes the input state array to prep for reallocation.
          delete [] Current_Input_State;
          Current_Input_State = NULL;
          Current_Input_Depth = 0;
          update_Settings_Table();
     }
    
     //Reset the O tables.
     void reset_O_Tables()
     {
          Tables->reset_Table(Output_Table);
     }
     
     //Fully charges the CAN starting at the given tier (precision).
     void scan_Force_Pattern(int p_Precision, float p_Charge)
     {
          //Have to neutralize the nodes first.
          //Node_Network.neutralize_All_Nodes();
          
          charge_CAN(p_Precision, 0, 0, p_Charge);
          //discharge_Torch_Nodes();
     }
     
     //Precision is the tier, cannot let that be known though as it offers insight into the inner workings.
     //This only does the given tier.
     void scan(int p_Precision, float p_Charge)
     {
          //Have to neutralize the nodes first.
          //Node_Network.neutralize_All_Nodes();
          
          charge_CAN_Given_Tier(p_Precision, 0, 0, p_Charge);
          //discharge_Torch_Nodes();
     }
     
     //Queries the Construct with the currrent input Table, charges all tiers starting at the given tier.
     void query_Construct_Force_Pattern(int p_Precision, int p_Method, float p_Charge)
     {
          
          query_Inputs(p_Method);
          //query_Inputs_With_Build();
          //build_Inputs(p_Method);
          scan_Force_Pattern(p_Precision, p_Charge);
     }
     
     //Queries the construct with the current input table, but only charges the given tier.
     void query_Construct(int p_Precision, int p_Method, float p_Charge)
     {
          //cout << "\n\t\t c_Poly_CAN::query_Construct()"; cout.flush();
          
          //Builds the query for the inputs.
          query_Inputs(p_Method);
          
          //index_Nodes();
          
          //Scans the network with the query CAN.
          scan(p_Precision, p_Charge);
     }
     
     //Builds the current input table.
     void build_Construct(int p_Method)
     {
          //cout << "\n\t c_CAN::bld"; cout.flush();
          build_Inputs(p_Method);
          update_All_Tables_From_Internal();
          //cout << "\n\t c_CAN::bld -Fin"; cout.flush();
          
     }
     
     
     void update_All_Tables_From_Internal()
     {
          //cout << "\n\t\t c_Poly_CAN::update_All_Tables_From_Internal()"; cout.flush();
          //update_Torch_Table();
          //update_Properties_Table();
          update_Settings_Table();
     }
     
     //Updates the torch node table.
     void update_Torch_Table()
     {
          Node_Network.update_Torch_Table();
     }
     
     //Updates the properties table.
     void update_Properties_Table()
     {
          cout << "\n\t\t c_Poly_CAN::update_Properties_Table()"; cout.flush();
          
     }
     
     //Updates the settings table.
     void create_Settings_Table()
     {
          
          cout << "\n\t\t c_Poly_CAN::update_Settings_Table()"; cout.flush();
          
          //cout << "updateing the dammn table"; system("pause > null");
          Tables->set_Data_Int(CID, Settings_Table, 0);
          Tables->set_Cell_Name(" CID:", Settings_Table, 0);
          cout << "\n\t\t\t -Tables->set_Cell_Name( CID:, Settings_Table, 0);";
          
          Tables->set_Data_Int(Construct_Type, Settings_Table, 1);
          Tables->set_Cell_Name(" Construct_Type:", Settings_Table, 1);
          cout << "\n\t\t\t -Tables->set_Cell_Name( Construct_Type:, Settings_Table, 1);";
          
          Tables->set_Data_Int(Dimensions, Settings_Table, 5);
          Tables->set_Cell_Name(" Dimensions:", Settings_Table, 5);
          cout << "\n\t\t\t -Tables->set_Cell_Name( Dimensions:, Settings_Table, 5);";
          
          Tables->set_Data_Int(Input_Depth, Settings_Table, 6);
          Tables->set_Cell_Name(" Input Depth:", Settings_Table, 6);
          cout << "\n\t\t\t -Tables->set_Cell_Name( Input Depth:, Settings_Table, 6);";
          
          Tables->set_Data(Current_Input_State, Current_Input_Depth, Settings_Table, 7);
          Tables->set_Cell_Name(" Current Input State:", Settings_Table, 7);
          cout << "\n\t\t\t -Tables->set_Cell_Name( Current Input State:, Settings_Table, 7);";
          
          Tables->set_Data_Int(Current_Input_Depth, Settings_Table, 8);
          Tables->set_Cell_Name(" Current_Input_Depth:", Settings_Table, 8);
          cout << "\n\t\t\t -Tables->set_Cell_Name( Current_Input_Depth:, Settings_Table, 8);";
          
          u_IO_Data tmp_Data;
          tmp_Data.NR = Treetop;
          Tables->set_Data(&tmp_Data, 1, Settings_Table, 9);
          Tables->set_Cell_Name(" Treetop:", Settings_Table, 9);
          cout << "\n\t\t\t -Tables->set_Cell_Name( Treetop:, Settings_Table, 9);";
          
     }
     
     //Updates the settings table.
     void update_Settings_Table()
     {
          
          //cout << "\n\t\t c_Poly_CAN::update_Settings_Table()"; cout.flush();
          
          //cout << "updateing the dammn table"; system("pause > null");
          Tables->set_Data_Int(&CID, 1, Settings_Table, 0);
          
          Tables->set_Data_Int(&Construct_Type, 1, Settings_Table, 1);
          
          Tables->set_Data_Int(&Dimensions, 1, Settings_Table, 5);
          
          Tables->set_Data_Int(&Input_Depth, 1, Settings_Table, 6);
          
          Tables->set_Data(Current_Input_State, Current_Input_Depth, Settings_Table, 7);
          
          Tables->set_Data_Int(&Current_Input_Depth, 1, Settings_Table, 8);
          
          u_IO_Data tmp_Data;
          tmp_Data.NR = Treetop;
          Tables->set_Data(&tmp_Data, 1, Settings_Table, 9);
          
          //cout << " ~Finished.";
          
     }
     /*
          Name = p_Construct_Name;
          Tables = p_Tables;  
          Dimensions = 0;
          Input_Depth = 0;
          Current_Input_State = NULL;
          Current_Input_Depth = 0;
          CAN_Nodes = NULL;
          CAN_Nodes_Current_Count = 0;
          CAN_Nodes_Max_Count = 0;
          Treetop = NULL;  
          Input_Type = 0;
          
          Node_Network.Output_Address = p_Output_Address;
          Output_Address = p_Output_Address; 
          
          Position.X = 0;
          Position.Y = 0;
          */
     
     //Outputs the current CAN if there is one.
     void output_CAN()
     {
          cout << "\n\n\n CAN ~Input_Depth:" << Input_Depth << " Treetop:" << Treetop << "\n\n";
          //Loops through all of the tiers.
          for (int cou_T=0;cou_T<Input_Depth;cou_T++){
               
               cout << "\n";
               
               //Loops through all the CAN nodes in the current layer.
               for (int cou_Node=0;cou_Node<(Input_Depth - cou_T);cou_Node++){
                    
                    //If the current CAN node target is NULL then continue.
                    if (CAN_Nodes[cou_T] [cou_Node]->Current_Target_Node == NULL)
                    {
                         cout << "(NULL)";
                         continue;
                    }
                    
                    //Outputs the current CAN node target.
                    cout << " (" << CAN_Nodes[cou_T] [cou_Node]->Current_Target_Node;
                    ((c_Poly_Node*) CAN_Nodes[cou_T] [cou_Node]->Current_Target_Node)->output_State();
                    cout << ")";
               }
          }
     }
};

