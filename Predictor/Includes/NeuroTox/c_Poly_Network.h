
//   This class holds all of the nodes in the network,
// when a new node is requested the network creates one and 
// returns a handle to it. Through this handle manipulators can
// be modified to create desired behavior.
class c_Poly_Network
{
public:
     
     friend class c_Poly_Node;
     
     //All of the nodes in this particular network.
     c_Poly_Node ** Nodes;
     
     //The adrress of the output this construct is hooked to.
     void * Output_Address;
     
     //The CID of the construct that the network belongs to.
     int CID;
     
     //The current number of nodes in the network.
     int Nodes_Current_Count;
     
     //The maximun indexes currently allocated for the array.  
     int Nodes_Max_Count;
     
     //The number of tiers in the network.
     int Number_Of_Tiers;
     
     //The highest layer index in the tree.
     int Highest_Layer_Count;
     
     //The Highest index in each layer.
     int * Highest_Layer_Count_Individual;
     
     //The treetop nodes in this network.
     c_Poly_Node ** Treetop_Nodes;
     
     //The current number of treetop nodes in the network.
     int Nodes_Treetop_Count;
     
     //The maximun number of indexes in the treetop array.
     int Nodes_Treetop_Max_Count;
     
     //The treetop nodes in this network.
     c_Poly_Node ** Charged_Treetop_Nodes;
     
     //The current number of Charged_treetop nodes in the network.
     int Nodes_Charged_Treetop_Count;
     
     //The maximun number of indexes in the Charged_treetop array.
     int Nodes_Charged_Treetop_Max_Count;
     
     //The depth of the charges in the network.
     int Charge_Depth;
     
     //The value that the membrane potential is multiplied by when a node fires an action potential
     float Membrane_Potential_Multiplier;
     
     //The integer value for the separator when displaying full networks.
     int Separator;
     
     //Y between nodes = (Separator * Aspect_Ration)
     int Aspect_Ratio;
     
     //The state binary tree for this networks input nodes.
     c_State_Bin_Tree State;
     
     
     //---    Tables and related members.
     c_Table_Handler * Tables;
     
     //The handle for the torch node table.
     int Torch_Table;
     
     //The handle for the output table.
     int Output_Table;
     
     //--     NODE INFO
     
     c_Poly_Node_Settings Node_Settings;
     
     //The constructor for this network.
     c_Poly_Network(int p_CID = 0, float p_Action_Potential_Threshold = 0.01, float p_Neuroplasticity_Modifier = 0.001, int p_Reinforcement_Counter_Rate = 2, int p_Dimension = 1)
     {
          //Initialize the member variables.
          Nodes = NULL;
          Nodes_Current_Count = 0;
          Nodes_Max_Count = 0;
          Highest_Layer_Count = 0;
          Highest_Layer_Count_Individual = NULL;
          CID = p_CID;
          
          Number_Of_Tiers = 0;
          Separator = 0;
          Aspect_Ratio = 0;
          
          Treetop_Nodes = NULL;
          Nodes_Treetop_Count = 0;
          Nodes_Treetop_Max_Count = 0;
          
          Charged_Treetop_Nodes = NULL;
          Nodes_Charged_Treetop_Count = 0;
          Nodes_Charged_Treetop_Max_Count = 0;
          
          Charge_Depth = 1;
          Membrane_Potential_Multiplier = .1;
          Output_Address = NULL;
          Tables = NULL;
          Torch_Table = -1;
          
          //Init the node vars.
          Node_Settings.Action_Potential_Threshold = p_Action_Potential_Threshold;
          Node_Settings.Reinforcement_Counter_Rate = p_Reinforcement_Counter_Rate;
          Node_Settings.Neuroplasticity_Modifier = p_Neuroplasticity_Modifier; 
          Node_Settings.set_Dimension(p_Dimension);
          Node_Settings.CID = CID;
     }
     
     //The destructor.
     ~c_Poly_Network()
     {
          delete [] Nodes;
          delete [] Treetop_Nodes;
          delete [] Charged_Treetop_Nodes;
          delete [] Highest_Layer_Count_Individual;
     }
     
     //Increases the size of the node array.
     void increase_Node_Array_Depth()
     {
          //Allocate a temporary array and copy the original into it, then delete the original and set it to the temporary array.
          
          //Increase the number of dendrites.
          Nodes_Max_Count += 1000;
          
          //Creates the array to copy the information into with the new number of dendrites.
          c_Poly_Node ** tmp_Nodes;
          tmp_Nodes = new c_Poly_Node*[Nodes_Max_Count];
          
          //Copy the addresses from the original array into the newly allocated one, then NULL the dendrite.
          for (int cou_NID=0;cou_NID<Nodes_Max_Count;cou_NID++){
               
               //NULL the current dendrite.
               tmp_Nodes[cou_NID] = NULL;
          }
          
          //Copy the addresses from the original array into the newly allocated one, then NULL the dendrite.
          for (int cou_NID=0;cou_NID<(Nodes_Max_Count - 1000);cou_NID++){
               
               //Copy the information from the Nodes into the new array at the current index.
               tmp_Nodes[cou_NID] = Nodes[cou_NID];
               
               //NULL the current dendrite.
               Nodes[cou_NID] = NULL;
          }
               
          //Delete the allocated memory for the original Nodes array.
          delete [] Nodes;
          Nodes = NULL;
          
          //Reallocate and NULL the node array.
          Nodes = new c_Poly_Node*[Nodes_Max_Count];
          
          for (int cou_NID=0;cou_NID<Nodes_Max_Count;cou_NID++){
               
               //NULL the current dendrite.
               Nodes[cou_NID] = NULL;
          }
          
          //Copy the addresses from the original array into the newly allocated one, then NULL the dendrite.
          for (int cou_NID=0;cou_NID<(Nodes_Max_Count - 1000);cou_NID++){
               
               //Copy the information from the Nodes into the new array at the current index.
               Nodes[cou_NID] = tmp_Nodes[cou_NID];
               
               //NULL the current dendrite.
               tmp_Nodes[cou_NID] = NULL;
          }
          
          delete [] tmp_Nodes;
          tmp_Nodes = NULL;
          
          cout << "\n\n\t " << this <<" has " << Nodes_Current_Count << " nodes.";
     }
     
     //Increases the size of the node array.
     void increase_Treetop_Node_Array_Depth()
     {
          //Allocate a temporary array and copy the original into it, then delete the original and set it to the temporary array.
          
          //Increase the number of dendrites.
          Nodes_Treetop_Max_Count += 10;
          
          //Creates the array to   the information into with the new number of dendrites.
          c_Poly_Node ** tmp_Nodes;
          tmp_Nodes = new c_Poly_Node*[Nodes_Treetop_Max_Count];
          
          //Copy the addresses from the original array into the newly allocated one, then NULL the dendrite.
          for (int cou_NID=0;cou_NID<(Nodes_Treetop_Max_Count - 10);cou_NID++){
               
               //Copy the information from the Treetop_Nodes into the new array at the current index.
               tmp_Nodes[cou_NID] = Treetop_Nodes[cou_NID];
               
               //NULL the current dendrite.
               Treetop_Nodes[cou_NID] = NULL;
          }
               
          //Delete the allocated memory for the original Treetop_Nodes array.
          delete [] Treetop_Nodes;
          
          //Sets the Treetop_Nodes array to the tmp_Nodes.
          Treetop_Nodes = tmp_Nodes;
     }
     
     //Increases the size of the node array.
     void increase_Charged_Treetop_Node_Array_Depth()
     {
          //Allocate a temporary array and copy the original into it, then delete the original and set it to the temporary array.
          
          //Increase the number of dendrites.
          Nodes_Charged_Treetop_Max_Count += 10;
          
          //Creates the array to   the information into with the new number of dendrites.
          c_Poly_Node ** tmp_Nodes;
          tmp_Nodes = new c_Poly_Node*[Nodes_Charged_Treetop_Max_Count];
          
          //Copy the addresses from the original array into the newly allocated one, then NULL the dendrite.
          for (int cou_NID=0;cou_NID<(Nodes_Charged_Treetop_Max_Count - 10);cou_NID++){
               
               //Copy the information from the Treetop_Nodes into the new array at the current index.
               tmp_Nodes[cou_NID] = Charged_Treetop_Nodes[cou_NID];
               
               //NULL the current dendrite.
               Charged_Treetop_Nodes[cou_NID] = NULL;
          }
               
          //Delete the allocated memory for the original Treetop_Nodes array.
          delete [] Charged_Treetop_Nodes;
          
          //Sets the Treetop_Nodes array to the tmp_Nodes.
          Charged_Treetop_Nodes = tmp_Nodes;
     }
     
     //Decreases the size of the node array.
     //The index of the last node is inclusive as to be saved.
     void decrease_Charged_Treetop_Node_Array_Depth(int p_Number_Of_Nodes_To_Save = 0)
     {
          //Allocate a temporary array and copy the original into it, then delete the original and set it to the temporary array.
          
          //Increase the number of dendrites.
          Nodes_Charged_Treetop_Max_Count = p_Number_Of_Nodes_To_Save;
          
          //Creates the array to   the information into with the new number of dendrites.
          c_Poly_Node ** tmp_Nodes;
          tmp_Nodes = new c_Poly_Node*[p_Number_Of_Nodes_To_Save];
          
          //Copy the addresses from the original array into the newly allocated one, then NULL the dendrite.
          for (int cou_NID=0;cou_NID<p_Number_Of_Nodes_To_Save;cou_NID++){
               
               //Copy the information from the Treetop_Nodes into the new array at the current index.
               tmp_Nodes[cou_NID] = Charged_Treetop_Nodes[cou_NID];
               
               //NULL the current dendrite.
               Charged_Treetop_Nodes[cou_NID] = NULL;
          }
               
          //Delete the allocated memory for the original Treetop_Nodes array.
          delete [] Charged_Treetop_Nodes;
          
          //Sets the Treetop_Nodes array to the tmp_Nodes.
          Charged_Treetop_Nodes = tmp_Nodes;
     }
     
     //Adds a node to the network.
     void* new_Node()
     {
          //Checks for array bounding.
          if (Nodes_Current_Count == Nodes_Max_Count)
          {
               //Increase the size of the node array so that we do not throw bounding errors.
               increase_Node_Array_Depth();
          }
          
          //Creates the new node, passes the charge depth and a reference to this network.
          //Nodes[Nodes_Current_Count] = new c_Poly_Node(&Node_Settings, CID, Nodes_Current_Count, Output_Address);
          Nodes[Nodes_Current_Count] = new c_Poly_Node(this, CID, Nodes_Current_Count, Output_Address);
                    
          //Increases the node count.
          Nodes_Current_Count++;
          
          //Returns a reference to the newly created node.
          return Nodes[Nodes_Current_Count - 1];
     }
     
     //Creates a node with the given properties, used for loading files.
     void* load_Node(int p_UNID)
     {
          //Checks for array bounding.
          if (Nodes_Current_Count == Nodes_Max_Count)
          {
               //Increase the size of the node array so that we do not throw bounding errors.
               increase_Node_Array_Depth();
          }
          
          //Creates the new node, passes the charge depth and a reference to this network.
          //Nodes[Nodes_Current_Count] = new c_Poly_Node(&Node_Settings, CID, p_UNID, Output_Address);
          Nodes[Nodes_Current_Count] = new c_Poly_Node(this, CID, p_UNID, Output_Address); //old nodes
               
          //Increases the node count.
          Nodes_Current_Count++;
          
          //Returns a reference to the newly created node.
          return Nodes[Nodes_Current_Count - 1];
     }
     
     //Fills out the node references on the dendrites and the axons.
     void fill_Nodes()
     {
          
     }
     
     //Prepares the network for saving.
     void prep_For_Save()
     {
          gather_Connection_References();
     }
     
     //Gathers the node references for the axons and dendrites of the networks nodes.
     void gather_Connection_References()
     {
          for (int cou_Index=0;cou_Index<Nodes_Current_Count;cou_Index++)
          {
               Nodes[cou_Index]->gather_Connection_References();
          }
     }
     
     //Sets the CID of the network and also sets the settings at the same time.
     void set_CID(int p_CID)
     {
          CID = p_CID;
          Node_Settings.CID = CID;
     }
     
     //Adds a node to the treetop array.
     void* new_Treetop_Node(void * p_Node)
     {
          //Checks for array bounding.
          if (Nodes_Treetop_Count == Nodes_Treetop_Max_Count)
          { 
               //Increase the size of the node array so that we do not throw bounding errors.
               increase_Treetop_Node_Array_Depth();
          }
          
          //Creates the new node, passes the charge depth and a reference to this network.
          Treetop_Nodes[Nodes_Treetop_Count] = ((c_Poly_Node*) p_Node);
          
          //Sets the treetop flag on the node.
          Treetop_Nodes[Nodes_Treetop_Count]->flg_Treetop = true;
          
          //Increases the node count.
          Nodes_Treetop_Count++;
          
          //Returns a reference to the newly created node.
          return Treetop_Nodes[Nodes_Treetop_Count - 1];
     }
     
     //Creates a connection between two nodes.
     void* create_Upper_Tier_Node(void * p_Nodes[], int p_Number_Of_Nodes)
     {
          //Create a new node.
          new_Node();
          
          //Create the connections to the new node for each of the given nodes.
          for (int cou_NID=0;cou_NID<p_Number_Of_Nodes;cou_NID++){
               //Creates a connection from the current p_Node to the newly created node at the index )Nodes_Current_Count - 1).
               ((c_Poly_Node*)p_Nodes[cou_NID])->create_Connection_To_Given_Node(Nodes[Nodes_Current_Count - 1]);
          }
          
          //Return a reference to the newly created node.
          return Nodes[Nodes_Current_Count - 1];
     }
     
     //Gets the reference to an upper tier node, if none exists it creates one.
     void* get_Upper_Tier_Node(void * p_Nodes[], int p_Number_Of_Nodes = 0)
     {
          //The tmp variable to hold the adress of the upper tier connection.
          void* tmp_Connection_Node = NULL;
          
          //Checks to see if an upper tier connection exists.
          tmp_Connection_Node = ((c_Poly_Node*) p_Nodes[0])->does_Upper_Tier_Connection_Exist(p_Nodes);
          
          if (tmp_Connection_Node == NULL)
          {
               //If no upper tier connection yet exists then create one.
               tmp_Connection_Node = create_Upper_Tier_Node(p_Nodes, p_Number_Of_Nodes);
               
               //Return the reference to the newly created upper tier node.
               return tmp_Connection_Node;
          } 
          else 
          {
               //If a connection was found then return the reference to it.
               return tmp_Connection_Node;
          }
          
          return NULL;
     }
     
     
     //Gets the reference to an upper tier node, if none exists it creates one.
     void* does_Upper_Tier_Connection_Exist(void * p_Nodes[], int p_Number_Of_Nodes = 0)
     {
          //The tmp variable to hold the adress of the upper tier connection.
          void* tmp_Connection_Node = NULL;
          
          //Checks to see if an upper tier connection exists.
          tmp_Connection_Node = ((c_Poly_Node*) p_Nodes[0])->does_Upper_Tier_Connection_Exist(p_Nodes);
          
          return tmp_Connection_Node;
     }
     
     
     //Sets all of the charging legs on the nodes.
     void set_Charging_Legs_All_Nodes(bool p_Legs[])
     {
          for (int cou_Index=0;cou_Index<Nodes_Current_Count;cou_Index++)
          {
               set_Charging_Legs(cou_Index, p_Legs);
          }
     }
     
     //Sets the charging legs for a given node.
     void set_Charging_Legs(int p_Node, bool p_Legs[])
     {
          Nodes[p_Node]->set_Charging_Legs(p_Legs);
     }
     
     //Sets a charging leg on all nodes.
     void set_Charging_Leg_All_Nodes(int p_Node, int p_Leg, bool p_Charge)
     {
          Nodes[p_Node]->set_Charging_Leg(p_Leg, p_Charge);
     }
     
     //Neutralizes all the nodes in the network for the given eval.
     void neutralize_All_Nodes()
     {
          //Loop through all of the nodes.
          for (int cou_Index=0;cou_Index<Nodes_Current_Count;cou_Index++){
               
               //Neutralize the current node for the given eval.
               Nodes[cou_Index]->neutralize_Node();
          }
          
          //Resets the charged treetop array.
          decrease_Charged_Treetop_Node_Array_Depth();
     }
     
     //Initiates a backpropgation for any nodes carrying a torch.
     void activate_Torch_Nodes()
     {
          cout << "\n\t\t\t ~activate_Torch_Nodes()";
          
          //Reset the table for new output.
          Tables->reset_Table(Output_Table);
          
          //Loop through all of the nodes.
          for (int cou_Index=0;cou_Index<Nodes_Current_Count;cou_Index++){
               
               //If this node carries a torch then activate the backpropagation.
               if (Nodes[cou_Index]->flg_Torch)
               {
                    //cout << "\n Tables->push_Table_Index(Output_Table);";
                    //Move ot the next index for the new output set.
                    Tables->push_Table_Index(Output_Table);
                    
                    //Get the current node charge.
                    u_IO_Data tmp_Charge;
                    //cout << "\n tmp_Charge.F = Nodes[cou_Index]->Charge[p_Eval];";
                    tmp_Charge.F = Nodes[cou_Index]->Charge;
                    
                    //Set teh table flag to the current output charge.
                    //cout << "\n Tables->set_Flag(Output_Table, Tables->get_Current_Push_Index(Output_Table), 0, tmp_Charge);";
                    Tables->set_Flag(Output_Table, Tables->get_Current_Push_Index(Output_Table), 0, tmp_Charge);
                    
                    //Initiate the backpropagation.
                    //cout << "\n Nodes[cou_Index]->initiate_Backpropagation_Both(1, Nodes[cou_Index]->Charge[p_Eval]);";
                    Nodes[cou_Index]->initiate_Backpropagation_Both(Nodes[cou_Index]->Charge);
                    
                    //Gather the output into the output table
                    //cout << "\n Tables->push(((c_1D_Output*) Output_Address)->get_Current_Output(), ((c_1D_Output*) Output_Address)->get_Data_Length(), Output_Table);";
                    Tables->set_Data(((c_1D_Output*) Output_Address)->get_Current_Output(), ((c_1D_Output*) Output_Address)->get_Data_Length(), Output_Table, Tables->get_Current_Push_Index(Output_Table));
                    
                    //Reset the output for the next set.
                    //cout << "\n ((c_1D_Output*) Output_Address)->reset_IO_Nodes();";
                    ((c_1D_Output*) Output_Address)->reset_IO_Nodes();
               }
          }
     }
     
     
     
     //Updates the torch node table.
     void update_Torch_Table(int p_Eval = 0)
     {
          cout << "\n\t\t c_Poly_Network::update_Torch_Table() -Torch_Table:" << Torch_Table;
          //If the table is not set then return.
          if (Torch_Table == -1){ return; }
          
                    cout << "\n\t\t\t -";
                    
          //Loop through all of the nodes.
          for (int cou_Index=0;cou_Index<Nodes_Current_Count;cou_Index++){
               
               //If this node carries a torch then activate the backpropagation.
               if (Nodes[cou_Index]->flg_Torch)
               {
                    //Nodes[cou_Index]->initiate_Backpropagation_Both(1, Nodes[cou_Index]->Charge[p_Eval]);
                    //If the torch table is set, push the current nodes onto it.
                    //Tables->push_NR(Nodes[cou_Index], Torch_Table);
                    //cout << " ~" << Nodes[cou_Index];
               }
          }
     }
     
     //Gets the node reference associated with the given state.
     void* get_State_Node(u_IO_Data &p_Data, bool p_State_Reference = 0, bool p_Query = 0)
     {
//          cout << "\n\t\t c_Poly_Network::get_State_Node()"; cout.flush();
          
          //Queries the binary tree with the given p_Data.C.
//          cout << "\n\t\t c_Poly_Network::get_State_Node()::State.search()"; cout.flush();
          State.search(p_Data.C, 4);
          
          //Checks to see if the given state was found.
//          cout << "\n\t\t c_Poly_Network::get_State_Node()::if (State.flg_Foundit)"; cout.flush();
          if (State.flg_Foundit){
               //If a node was found that matches then that nodes reference is returned.
//               cout << "\n\t\t return State.get_Current_Node_NID(<" << State.get_Current_Node_NID() << ">)"; cout.flush();
               return State.get_Current_Node_NID();
          }
          else 
          { 
               //If this call is for a query then no node is created.
               if (p_Query)
               {
                    return NULL;
               }
                    
               //If a node matching the state was not found the set the node reference to a new node.
//               cout << "\n\t\t State.set_Current_Node_NID(new_Node());"; cout.flush();
               State.set_Current_Node_NID(new_Node()); 
               
               //Sets the flag on the newly created node to signify that it is a state node.
               ((c_Poly_Node*) State.get_Current_Node_NID())->flg_State = true;
               
               //Sets the flag on the newly created node to signify that it is a state node.
               ((c_Poly_Node*) State.get_Current_Node_NID())->flg_State_Reference = p_State_Reference;
               
               //Creates a connection between the newly created state node and the given treetop node.
               if (p_State_Reference && p_Data.NR)
               {
                    ((c_Poly_Node*) p_Data.NR)->create_Connection_To_Given_Node(State.get_Current_Node_NID());
               }
               
               //Returns the reference to the newly created node.
//               cout << "\n\t\t return State.get_Current_Node_NID(<" << State.get_Current_Node_NID() << ">)"; cout.flush();
               return State.get_Current_Node_NID();
               }
          //So that the compiler does not bitch, return null if this function reaches this point.
          return NULL;  
     }
     
     //Sets the table handler.
     void set_Table_Handler(c_Table_Handler * p_Tables)
     {
          Tables = p_Tables;
     }
     
     //Sets the table handle for the torch nodes.
     void set_Torch_Table(int p_TID)
     {
          Torch_Table = p_TID;
     }
     
     //Sets the table handle for the output table.
     void set_Output_Table(int p_TID)
     {
          Output_Table = p_TID;
     }
     
     //Outputs all the nodes in the network.
     void output_All_Poly_Nodes()
     {
          cout << "\n\n\n";
          cout << "#Nodes " << Nodes_Current_Count;
          for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
               Nodes[cou_NID]->output_Poly_Node();
          }
     }
     
     //Outputs all the nodes in the network.
     void output_All_Poly_Nodes_Graphical_Absolute_Full()
     {
          for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
               cout << " Separator:" << Separator;
               Nodes[cou_NID]->output_Poly_Node_Graphical_Absolute_Full(Separator, Aspect_Ratio);
          }
     }
     
     
     //Outputs all the nodes in the network.
     void output_All_Poly_Nodes_Graphical_Absolute_Full_By_Tier()
     {
          
          //cout << "\n\n\n";
          //cout << "# Nodes:" << Nodes_Current_Count << " # Tiers:" << Number_Of_Tiers;
          
          
          for (int cou_Tier=0;cou_Tier<(Number_Of_Tiers + 1);cou_Tier++){
               //cout << "\n Tier(" << cou_Tier << ")\t";
               
               for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
                    
                    if (Nodes[cou_NID]->Tier == cou_Tier)
                    {
                         Nodes[cou_NID]->output_Poly_Node_Graphical_Absolute_Full(Separator, Aspect_Ratio);
                    }
               }
          } 
     }
     
     
     //Outputs all the nodes in the network.
     void scribe_All_Poly_Nodes_Graphical_Absolute_Full_By_Tier(ofstream * p_File)
     {
          
          //cout << "\n\n\n";
          //cout << "# Nodes:" << Nodes_Current_Count << " # Tiers:" << Number_Of_Tiers;
          
          
          for (int cou_Tier=0;cou_Tier<(Number_Of_Tiers + 1);cou_Tier++){
               //cout << "\n Tier(" << cou_Tier << ")\t";
               
               for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
                    
                    if (Nodes[cou_NID]->Tier == cou_Tier)
                    {
                         Nodes[cou_NID]->scribe_Poly_Node_Graphical_Absolute_Full(p_File, Separator, Aspect_Ratio);
                    }
               }
          } 
     }
     
     //Outputs all the nodes in the network graphically.
     void output_All_Poly_Nodes_Graphical(int p_Width = 250)
     {
          cout << "\n\n\n";
          cout << "#Nodes " << Nodes_Current_Count;
          for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
               
               if (!(cou_NID % p_Width)){ cout << "\n"; }
               
               Nodes[cou_NID]->output_Poly_Node_Graphical();
          }
     }
     
     //Indexes the nodes based on their position in the tiers and layers.
     int index_Nodes(int p_X, int p_Y, int p_Separator = 5, int p_Aspect_Ratio = 1)
     {
          //The temporary tracker for the current index on the current tier.
          int tmp_NID = 0;
          
          //Reset the highest node count.
          Highest_Layer_Count = 0;
          
          //Loops through all of the nodes to find the max number of tiers.
          for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
               
               if (Nodes[cou_NID]->Tier > Number_Of_Tiers){ Number_Of_Tiers = Nodes[cou_NID]->Tier; }
          }
          
          //This is a nice feature, however, needs work as the MSC connections are off on their targets.
          //Separator = Number_Of_Tiers+10;
          Separator = p_Separator;
          Aspect_Ratio = p_Aspect_Ratio;
          
          //cout << "\n\t\t Separator ~~" << Separator << " Number_Of_Tiers:" << Number_Of_Tiers;
          //system("PAUSE > NULL");
          
          //Cleanup old Layer counts.
          delete [] Highest_Layer_Count_Individual;
          Highest_Layer_Count_Individual = NULL;
          
          //Reallocate the array with the appropriate depth;
          Highest_Layer_Count_Individual = new int[Number_Of_Tiers + 1];
          
          //Initialize the newly created array.
          for (int cou_Index=0;cou_Index<(Number_Of_Tiers + 1);cou_Index++){
               Highest_Layer_Count_Individual[cou_Index] = 0;
          }
          
          //Loops through each tier setting the NID for each node as it goes.
          for (int cou_Tier=0;cou_Tier<(Number_Of_Tiers + 1);cou_Tier++){
               
               //Sets the temporary NID.
               tmp_NID = 0;
               
               for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
                    
                    //If the node is on the current tier then index it accordingly.
                    if (Nodes[cou_NID]->Tier == cou_Tier)
                    {
                         Nodes[cou_NID]->NID = tmp_NID;
                         tmp_NID++;
                         if (tmp_NID > Highest_Layer_Count){ Highest_Layer_Count = tmp_NID; }
                         if (tmp_NID > Highest_Layer_Count_Individual[cou_Tier]){ Highest_Layer_Count_Individual[cou_Tier] = tmp_NID; }
                    }
               }
          } 
          
          //Loops through each tier setting the X and Y for each one.
          for (int cou_Tier=0;cou_Tier<(Number_Of_Tiers + 1);cou_Tier++){
               
               for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
                    
                    Nodes[cou_NID]->X = ((int (Highest_Layer_Count * .5) - int (Highest_Layer_Count_Individual[Nodes[cou_NID]->Tier] * .5)) + p_X) + 1;
                    
                    Nodes[cou_NID]->Y = p_Y;
               }
          }
          
          return Highest_Layer_Count;
     }
     
     //Indexes the nodes based on their position in the tiers and layers.
     int index_Nodes_Condensed(int p_X = 0, int p_Y = 0)
     {
          
          
          //The temporary tracker for the current index on the current tier.
          int tmp_NID = 0;
          
          //Reset the highest node count.
          Highest_Layer_Count = 0;
          
          //Loops through all of the nodes to find the max number of tiers.
          for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
               
               if (Nodes[cou_NID]->Tier > Number_Of_Tiers){ Number_Of_Tiers = Nodes[cou_NID]->Tier; }
          }
          
          //Cleanup old Layer counts.
          delete [] Highest_Layer_Count_Individual;
          
          //Reallocate the array with the appropriate depth;
          Highest_Layer_Count_Individual = new int[Number_Of_Tiers];
          
          //Initialize the newly created array.
          for (int cou_Index=0;cou_Index<Number_Of_Tiers;cou_Index++){
               Highest_Layer_Count_Individual[cou_Index] = 0;
          }
          
          //Loops through each tier setting the NID for each node as it goes.
          for (int cou_Tier=0;cou_Tier<(Number_Of_Tiers + 1);cou_Tier++){
               
               //Sets the temporary NID.
               tmp_NID = 0;
               
               for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
                    
                    //If the node is on the current tier then index it accordingly.
                    if (Nodes[cou_NID]->Tier == cou_Tier)
                    {
                         Nodes[cou_NID]->NID = tmp_NID;
                         tmp_NID++;
                         if (tmp_NID > Highest_Layer_Count){ Highest_Layer_Count = tmp_NID; }
                         if (tmp_NID > Highest_Layer_Count_Individual[cou_Tier]){ Highest_Layer_Count_Individual[cou_Tier] = tmp_NID; }
                    }
               }
          } 
          
          //Loops through each tier setting the X and Y for each one.
          for (int cou_Tier=0;cou_Tier<(Number_Of_Tiers + 1);cou_Tier++){
               
               for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
                    
                    Nodes[cou_NID]->X = (int (Highest_Layer_Count * .5) - int (Highest_Layer_Count_Individual[Nodes[cou_NID]->Tier] * .5)) + p_X;
                    Nodes[cou_NID]->Y = p_Y;
               }
          }
          return Highest_Layer_Count;
     }
     
     //Outputs all the nodes in the network graphically.
     void output_All_Poly_Nodes_Graphical_By_Tier(int p_Tier_Low = 0)
     {
          //Index all the nodes to prep them for output.
          //index_Nodes();
          
          cout << "\n\n\n";
          cout << "# Nodes:" << Nodes_Current_Count << " # Tiers:" << Number_Of_Tiers;
          
          
          for (int cou_Tier=p_Tier_Low;cou_Tier<(Number_Of_Tiers + 1);cou_Tier++){
               cout << "\n Tier(" << cou_Tier << ")\t";
               
               for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
                    
                    if (Nodes[cou_NID]->Tier == cou_Tier)
                    {
                         Nodes[cou_NID]->output_Poly_Node_Graphical();
                    }
               }
          } 
     }
     
     //Outputs all the nodes in the network graphically.
     void output_All_Poly_Nodes_Graphical_By_Tier_Absolute_Pos(int p_Tier_Low = 0, int p_X = 0, int p_Y = 0)
     {
          //system("CLS");
          
          //Index all the nodes to prep them for output.
          //index_Nodes();
          
          console_Move_Cursor_To_Given_X_And_Y(0, 0);
          
          
          for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
                    
               Nodes[cou_NID]->output_Poly_Node_Graphical_Absolute();
          }
          
          console_Move_Cursor_To_Given_X_And_Y(0, (Number_Of_Tiers + 2));
          
          for (int cou_Index_Y=0;cou_Index_Y<100;cou_Index_Y++){
               cout << "\n";
               for (int cou_Index_X=0;cou_Index_X<100;cou_Index_X++){
                    cout << ' ';
               }
          }
          
          console_Move_Cursor_To_Given_X_And_Y(0, (Number_Of_Tiers + 2));
          
          cout << "\n # Nodes:" << Nodes_Current_Count << " # Tiers:" << Number_Of_Tiers;
          
          
     }
     
     //Outputs all the nodes in the network graphically.
     void output_All_Poly_Nodes_Graphical_By_Tier_Absolute_Pos_RC(int p_Tier_Low = 0, int p_X = 0, int p_Y = 0)
     {
          //system("CLS");
          
          //Index all the nodes to prep them for output.
          //index_Nodes();
          
          console_Move_Cursor_To_Given_X_And_Y(0, 0);
          
          
          for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
                    
               Nodes[cou_NID]->output_Poly_Node_Graphical_Absolute_RC();
          }
          
          console_Move_Cursor_To_Given_X_And_Y(0, (Number_Of_Tiers + 2));
          
          for (int cou_Index_Y=0;cou_Index_Y<100;cou_Index_Y++){
               cout << "\n";
               for (int cou_Index_X=0;cou_Index_X<100;cou_Index_X++){
                    cout << ' ';
               }
          }
          
          console_Move_Cursor_To_Given_X_And_Y(0, (Number_Of_Tiers + 2));
          
          cout << "\n # Nodes:" << Nodes_Current_Count << " # Tiers:" << Number_Of_Tiers;
          
          
     }
     
     //Outputs the graphical nodes that are currently charged to visibility.
     void output_All_Poly_Node_Graphical_Absolute_Charged()
     {
          for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
               Nodes[cou_NID]->output_Poly_Node_Graphical_Absolute_Charged();
          }
     }
     
     //Outputs all the nodes in the network.
     void output_All_Charged_Poly_Nodes()
     {
          int tmp_Charged_Nodes = 0;//Counter for the number of charged nodes.
          cout << "\n\n\n";
          cout << "#Nodes " << Nodes_Current_Count;
          for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
               if (Nodes[cou_NID]->Charge)
               {
                    tmp_Charged_Nodes++;
               }
          }
          
          cout << " #Charged:" << tmp_Charged_Nodes;
          
          for (int cou_NID=0;cou_NID<Nodes_Current_Count;cou_NID++){
               if (Nodes[cou_NID]->Charge)
               {
                    Nodes[cou_NID]->output_Poly_Node();
               }
          }
     }
     
     //Outputs all the nodes in the network.
     void output_All_Treetop_Nodes()
     {
          cout << "\n\n\n";
          cout << "#Treetop_Nodes " << Nodes_Treetop_Count;
          for (int cou_NID=0;cou_NID<Nodes_Treetop_Count;cou_NID++){
               Treetop_Nodes[cou_NID]->output_Poly_Node();
          }
     }
     
     //Outputs all the nodes in the network.
     void output_All_Treetop_Nodes_Graphical()
     {
          cout << "\n\n\n";
          cout << "#Treetop_Nodes " << Nodes_Treetop_Count;
          for (int cou_NID=0;cou_NID<Nodes_Treetop_Count;cou_NID++){
               Treetop_Nodes[cou_NID]->output_Poly_Node_Graphical();
          }
     }
     
     //Outputs all the nodes in the network.
     void output_All_Charged_Treetop_Nodes(int p_Eval = 0)
     {
          cout << "\n\n\n";
          cout << "#Treetop_Nodes " << Nodes_Treetop_Count;
          for (int cou_NID=0;cou_NID<Nodes_Treetop_Count;cou_NID++){
               if (Treetop_Nodes[cou_NID]->Charge)
               {
                    Treetop_Nodes[cou_NID]->output_Poly_Node();
               }
          }
     }
     
     //Outputs the top x poly_Nodes based on their current charge.
     void output_Top_X_Poly_Nodes_Sorted_By_Charge(int p_X = 10, int p_Eval = 0)
     {
          //--  Temporary variables for the function.
          
          //Holds the node addresses so that they can be bubble sorted.
          c_Poly_Node ** tmp_Nodes;
          
          //The temporary placeholder.
          c_Poly_Node * tmp_Node_Placeholder = 0;
          
          //The flag for the bubble sort.
          bool flg_Done;
          
          //Allocate the memory for the temporary node array.
          tmp_Nodes = new c_Poly_Node*[Nodes_Current_Count];
          
          //Copies the current node array into the temporary array.
          for (int cou_Index=0;cou_Index<Nodes_Current_Count;cou_Index++){
               tmp_Nodes[cou_Index] = Nodes[cou_Index];
          }
          
          //Sets the flag for loop entry.
          flg_Done = false;
          
          while (!flg_Done)
          {
               //Sets the state flag.
               flg_Done = true;
               
               //Loop through each node in the array starting at index 0 and ending at index (Nodes_Current_Count - 1).
               for (int cou_Index=0;cou_Index<(Nodes_Current_Count - 1);cou_Index++){
                    
                    //If the current index has less of a charge then the next one then initiate the swapping procedure.
                    if ((tmp_Nodes[cou_Index]->Charge) < (tmp_Nodes[cou_Index + 1]->Charge))
                    {
                         //Get the current node and put it into the placeholder.
                         tmp_Node_Placeholder = tmp_Nodes[cou_Index];
                         
                         //Overwrite the current index with the node stored in the next one.
                         tmp_Nodes[cou_Index] = tmp_Nodes[cou_Index + 1];
                         
                         //Set the next index to the one stored in the placeholder variable.
                         tmp_Nodes[cou_Index + 1] = tmp_Node_Placeholder;
                         
                         flg_Done = false;
                    }
               }
          }
          
          //Checks to make sure the p_X is less than the current number of nodes to prevent overflow errors.
          if (p_X > Nodes_Current_Count){ p_X = Nodes_Current_Count; }
          
          //Loops through X nodes in the sorted array and outputs them.
          for (int cou_Index=0;cou_Index<p_X;cou_Index++){
               tmp_Nodes[cou_Index]->output_Poly_Node();
          }
          
          //--  Cleanup the allocated memory.
          delete [] tmp_Nodes;
     }
     
     //Outputs the top x poly_Nodes based on their current charge.
     void output_Top_X_Poly_Nodes_Sorted_By_Charge_Only_Treetop(int p_X = 10, int p_Eval = 0)
     {
          //--  Temporary variables for the function.
          
          //Holds the node addresses so that they can be bubble sorted.
          c_Poly_Node ** tmp_Nodes;
          
          //The current index of the tmp_Nodes.
          int tmp_Nodes_Current_Index = 0;
          
          //The temporary placeholder.
          c_Poly_Node * tmp_Node_Placeholder = 0;
          
          //The flag for the bubble sort.
          bool flg_Done;
          
          //Allocate the memory for the temporary node array.
          tmp_Nodes = new c_Poly_Node*[Nodes_Treetop_Count];
          
          //Copies the current node array into the temporary array.
          for (int cou_Index=0;cou_Index<Nodes_Current_Count;cou_Index++){
               
               //If the current node is a treetop node, copy it into the array.
               if (Nodes[cou_Index]->flg_Treetop)
               {
                    tmp_Nodes[tmp_Nodes_Current_Index] = Nodes[cou_Index];
                    
                    //Increment the tmp_Nodes_Current_Index.
                    tmp_Nodes_Current_Index++;
               }
               
          }
          
          //Sets the flag for loop entry.
          flg_Done = false;
          
          while (!flg_Done)
          {
               //Sets the state flag.
               flg_Done = true;
               
               //Loop through each node in the array starting at index 0 and ending at index (Nodes_Treetop_Count - 1).
               for (int cou_Index=0;cou_Index<(Nodes_Treetop_Count - 1);cou_Index++){
                    
                    //If the current index has less of a charge then the next one then initiate the swapping procedure.
                    if ((tmp_Nodes[cou_Index]->Charge) < (tmp_Nodes[cou_Index + 1]->Charge))
                    {
                         //Get the current node and put it into the placeholder.
                         tmp_Node_Placeholder = tmp_Nodes[cou_Index];
                         
                         //Overwrite the current index with the node stored in the next one.
                         tmp_Nodes[cou_Index] = tmp_Nodes[cou_Index + 1];
                         
                         //Set the next index to the one stored in the placeholder variable.
                         tmp_Nodes[cou_Index + 1] = tmp_Node_Placeholder;
                         
                         flg_Done = false;
                    }
               }
          }
          
          //Checks to make sure the p_X is less than the current number of nodes to prevent overflow errors.
          if (p_X > Nodes_Treetop_Count){ p_X = Nodes_Treetop_Count; }
          
          //Loops through X nodes in the sorted array and outputs them.
          for (int cou_Index=0;cou_Index<p_X;cou_Index++){
               
               //If the node has a charge then output it.
               if (tmp_Nodes[cou_Index]->Charge){ tmp_Nodes[cou_Index]->output_Poly_Node(); }
          }
          
          //--  Cleanup the allocated memory.
          delete [] tmp_Nodes;
     }
};   










































//This class contains the handler for the node network, used for node network referenceing.
class c_Poly_Network_Handler
{
private:
     
     //The c_Network array.
     c_Poly_Network ** Networks;
     
     //The current number of Networks.
     int Network_Count;
     
     //Increases the Networks[] index count.
     void increase_Network_Array()
     {
          //Create temporary array.
          c_Poly_Network ** tmp_Networks;
          
          //Increment the Network_Count.
          Network_Count++;
          
          //Allocate the new c_Poly_Network array.
          tmp_Networks = new c_Poly_Network*[Network_Count];
          
          //Copy current data.
          //  (Network_Count - 1) --Because the Network_Count has been incremented.
          for (int cou_Network=0;cou_Network<(Network_Count - 1);cou_Network++)
          {
               tmp_Networks[cou_Network] = Networks[cou_Network];
          }
          
          //Destroy the old array.
          delete Networks;
          Networks = NULL;
          
          //Reallocate the old aray.
          Networks = new c_Poly_Network*[Network_Count];
          
          //Null the newly created array.
          for (int cou_Index=0;cou_Index<Network_Count;cou_Index++)
          {
               Networks[cou_Index] = NULL;
          }
          
          //Copy current data.
          //  (Network_Count - 1) --Because the Network_Count has been incremented.
          for (int cou_Network=0;cou_Network<(Network_Count - 1);cou_Network++)
          {
               Networks[cou_Network] = tmp_Networks[cou_Network];
               tmp_Networks[cou_Network] = NULL;
          }
          
          delete [] tmp_Networks;
     }
     
     bool b_Networks(int p_Network)
     {
          if (p_Network >= Network_Count){ return 0; }
          if (p_Network < 0){ return 0; }
          return 1;
     }
     
public:
     
     c_Poly_Network_Handler()
     {
          //Initialize the counters.
          Network_Count = 0;
          Networks = NULL;
          
     }
     
     //new_Network() This function creates a new table and returns a reference to the newly created table.
     void * r_new_Network(int p_CID)
     {
          //Adds a new table to the array.
          increase_Network_Array();
          
          //Initializes the current index.
          Networks[Network_Count - 1] = new c_Poly_Network(p_CID);
          
          //Returns the Index value of the new table.
          return Networks[Network_Count - 1];
     }
     
     //new_Network() This function creates a new table and returns a reference to the newly created table.
     int i_new_Network(int p_CID)
     {
          //Adds a new table to the array.
          increase_Network_Array();
          
          //Initializes the current index.
          Networks[Network_Count - 1] = new c_Poly_Network(p_CID);
          
          //Returns the Index value of the new table.
          return (Network_Count - 1);
     }
     
     
     //This function gets the table count.
     int get_Network_Count()
     {
          return Network_Count;
     }
     
     
};