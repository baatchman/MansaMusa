//This class is the base node network used for all of the networks. It is dimensionally dependent, this is for efficiency.

class c_MT_Base_Node_Network
{
protected:
     
     
public:
     
     //This is the array that holds all of the nodes for the network.
     //The first index is the tier the nodes are on, the next index is the 
     //nodes position in the tier.
     c_MT_Base_Node_1D *** Nodes;
     
     //The node network uses counters to track the number of nodes in the 
     //network as there will be up to millions of nodes in some tiers and
     //doing a search for NULL terminator would not be efficient.
     int ** Node_Count;
     
     //The current number of tiers in the network.
     int Number_Of_Tiers;
     
     //The maximun index count for the tier index of the Nodes array.
     int Number_Of_Tiers_MAX;
     
     //The maximum index in every tier.
     int ** Node_Count_MAX;
     
     //Each network has its own charging legs to allow for right leg charging, or chrono charging.
     bool Charging_Legs[2];
     
     //The binary tree that holds the states and the nodes relating to them.
     //c_State_Node_Bin_Tree_1D State_Tree[1000];
     //c_Fractal_State_Bin_Tree ** State_Tree;
     c_MT_Fractal_State_Bin_Tree State_Tree;
     
     //int State_Tree_Count;
     
     //The treetop tree to use for dereferencing foreign nodes.
     c_MT_Fractal_State_Bin_Tree ** Treetop_Bin_Tree;
     
     //The current Treetop NID.
     u_Data Current_TtID;
     
     //The CID the node network belongs to. This is used to add an identifier to the Treetop_ID.
     int CID;
     
     //This is the name for the node network.
     string Name;
     
     //This array holds the coodinates for the state nodes until they can be set after all the constructs are built.
     int * tmp_State_Coordinates[3];
     
     //The current depth of the state coordinates array.
     int tmp_State_Coordinates_Depth;
     
     //The type of state tree this network has.
     int State_Type;
     
     //--   TABLES
     
     
     //The pattern output table.
     c_Table_1D * Output_Table_Pattern;
     
     //The charge output table.
     c_Table_1D * Output_Table_Charge;
     
     
     //Initializes the network with one NULL node on the bottom tier.
     c_MT_Base_Node_Network()
     {
          //Create the array to hold the nodes.
          Nodes = new c_MT_Base_Node_1D**[1];
          Nodes[0] = new c_MT_Base_Node_1D*[1];
          Nodes[0] [0] = NULL;
          
          //Currently no nodes.
          Node_Count = new int*[1];
          Node_Count[0] = new int;
          *Node_Count[0] = 0;
          
          Node_Count_MAX = new int*[1];
          Node_Count_MAX[0] = new int;
          *Node_Count_MAX[0] = 0;
          
          Number_Of_Tiers = 0;
          
          Current_TtID.I = 0;
          Treetop_Bin_Tree = new c_MT_Fractal_State_Bin_Tree*[1];
          Treetop_Bin_Tree[0] = new c_MT_Fractal_State_Bin_Tree;
          
          /*
          State_Tree = new c_State_Node_Bin_Tree_1D*[1];
          State_Tree[0] = new c_State_Node_Bin_Tree_1D;
          State_Tree_Count = 1;
          */
          
          //Charging legs default to on.
          Charging_Legs[0] = 1;
          Charging_Legs[1] = 1;
          
          //Init State node references.
          tmp_State_Coordinates[0] = NULL;
          tmp_State_Coordinates[1] = NULL;
          tmp_State_Coordinates[2] = NULL;
          
          tmp_State_Coordinates_Depth = 0;
          
          //Set the state type to the default of 0, or a basic state.
          State_Type = 0;
     }
     
     ~c_MT_Base_Node_Network()
     {
          for (int cou_Index=0;cou_Index<Number_Of_Tiers;cou_Index++)
          {
               delete [] Nodes[cou_Index];
               Nodes[cou_Index] = NULL;
               
               delete Node_Count[cou_Index];
               Node_Count[cou_Index] = NULL;
               
               delete Node_Count_MAX[cou_Index];
               Node_Count_MAX[cou_Index] = NULL;
          }
          
          delete [] Nodes;
          Nodes = NULL;
          
          delete [] Node_Count;
          Node_Count = NULL;
          
          delete [] Node_Count_MAX;
          Node_Count_MAX = NULL;
          
          //cout << "\n\n\n\t int(Current_TtID.I % 1000)->" << int (Current_TtID.I / 1000);
          for (int cou_Index=0;cou_Index<(int (Current_TtID.I / 1000));cou_Index++)
          {
               delete Treetop_Bin_Tree[cou_Index];
               Treetop_Bin_Tree[cou_Index] = NULL;
          }
          delete [] Treetop_Bin_Tree;
          Treetop_Bin_Tree = NULL;
          
          /*
          for (int cou_Index=0;cou_Index<State_Tree_Count;cou_Index++)
          {
               delete State_Tree[cou_Index];
               State_Tree[cou_Index] = NULL;
          }
          delete [] State_Tree;
          State_Tree = NULL;
          */
          
          delete [] tmp_State_Coordinates[0];
          tmp_State_Coordinates[0] = NULL;
          delete [] tmp_State_Coordinates[1];
          tmp_State_Coordinates[1] = NULL;
          delete [] tmp_State_Coordinates[2];
          tmp_State_Coordinates[2] = NULL;
          
          Name = "No Name Network";
     }
     
     //Sets the output tables.
     void set_Output_Tables(c_Table_1D * p_Output_Table_Pattern, c_Table_1D * p_Output_Table_Charge)
     {
          Output_Table_Pattern = p_Output_Table_Pattern;
          Output_Table_Charge = p_Output_Table_Charge;
     }
     
     //Sets the name of the node network.
     void set_Name(string p_Name)
     {
          Name = p_Name;
     }
     
     //Sets the CID of the node network.
     void set_CID(int p_CID)
     {
          CID = p_CID;
          Current_TtID.I = CID;
     }
          
     //Sets the state type.
     void set_State_Type(int p_State_Type)
     {
          State_Type = p_State_Type;
     }
     
     //--     NODE CREATION FOR LOADING, IJECTS INTO GIVEN INDEX
     //We are running with no bounds checking as only the loading functions should be using.
     
     //Creates a new node on the given tier.
     virtual c_MT_Base_Node_1D * new_Node(int p_Tier, int p_Index)
     {
          
          Nodes[p_Tier] [p_Index] = new c_MT_Simple_Node;
          
          Nodes[p_Tier] [p_Index]->set_Index(p_Index);
          
          return Nodes[p_Tier] [p_Index];
     }
     
     //Creates a new node on the given tier.
     virtual c_MT_Base_Node_1D * new_RC_Node(int p_Tier, int p_Index)
     {
          Nodes[p_Tier] [p_Index] = new c_MT_RC_Node_1D;
          
          Nodes[p_Tier] [p_Index]->set_Index(p_Index);
          
          return Nodes[p_Tier] [p_Index];
     }
     
     //Creates a new node on the given tier.
     virtual c_MT_Base_Node_1D * new_Treetop_Node(int p_Tier, int p_Index)
     {
          Nodes[p_Tier] [p_Index] = new c_MT_Treetop_Simple_Node_1D;
          
          Nodes[p_Tier] [p_Index]->set_Index(p_Index);
          
          //Add the node to the treetop network.
          //The index is broken into 1000 node chunks.
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->search(Current_TtID);
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->set_Current_Node_NID(Nodes[p_Tier] [*Node_Count[p_Tier] - 1]);
          
          //Sets the state for the node to the identifier.
          Nodes[p_Tier] [p_Index]->set_Treetop(Current_TtID);
          
          //Sets the U_CID, Tier, and Index;
          Nodes[p_Tier] [p_Index]->set_CID(CID);
          Nodes[p_Tier] [p_Index]->set_Tier(p_Tier);
          Nodes[p_Tier] [p_Index]->set_Index(p_Index);
          
          //Increment the treetop node counter.
          Current_TtID.I += 100;
          
          //If a new chunk needs allocated then allocate it for the treetops.
          if (((Current_TtID.I - CID) % 1000) == 0)
          {
               increase_Treetop_Bin_Tree(int ((Current_TtID.I - CID) / 1000) + 1);
          }
          
          return Nodes[p_Tier] [p_Index];
     }
     
     
     //Creates a new node on the given tier.
     virtual c_MT_Base_Node_1D * new_Treetop_RC_Node(int p_Tier, int p_Index)
     {
          Nodes[p_Tier] [p_Index] = new c_MT_Treetop_RC_Node_1D;
          
          
          //Add the node to the treetop network.
          //The index is broken into 1000 node chunks.
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->search(Current_TtID);
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->set_Current_Node_NID(Nodes[p_Tier] [p_Index]);
          
          //Sets the ID of the treetop node.
          Nodes[p_Tier] [p_Index]->set_Treetop(Current_TtID);
          
          //Sets the U_CID, Tier, and Index;
          Nodes[p_Tier] [p_Index]->set_CID(CID);
          Nodes[p_Tier] [p_Index]->set_Tier(p_Tier);
          Nodes[p_Tier] [p_Index]->set_Index(p_Index);
          
          //Increment the treetop node counter.
          Current_TtID.I += 100;
          
          //If a new chunk needs allocated then allocate it for the treetops.
          if (((Current_TtID.I - CID) % 1000) == 0)
          {
               increase_Treetop_Bin_Tree(int ((Current_TtID.I - CID) / 1000) + 1);
          }
          
          return Nodes[p_Tier] [p_Index];
     }
     
     
     //Creates a state node with the given state.
     virtual c_MT_Base_Node_1D * new_State_Node(int p_Index, u_Data p_State)
     {
          Nodes[0] [*Node_Count[0] - 1] = new c_MT_State_Node_1D;
          
          Nodes[0] [*Node_Count[0] - 1]->set_Index(*Node_Count[0] - 1);
          
          Nodes[0] [*Node_Count[0] - 1]->set_State(p_State);
          
          return Nodes[0] [p_Index];
     }
     
     //Creates a state node with the given state.
     virtual c_MT_Base_Node_1D * new_RC_State_Node(int p_Index, u_Data p_State)
     {
          Nodes[0] [p_Index] = new c_MT_RC_State_Node_1D;
          
          Nodes[0] [p_Index]->set_Index(p_Index);
          
          Nodes[0] [p_Index]->set_State(p_State);
          
          return Nodes[0] [p_Index];
     }
          
     //Creates a state node with the given state.
     virtual c_MT_Base_Node_1D * new_Treetop_State_Node(int p_Tier, int p_Index, u_Data p_State)
     {
          Nodes[p_Tier] [p_Index] = new c_MT_Treetop_State_Node_1D;
          
          Nodes[p_Tier] [p_Index]->set_State(p_State);
                    
          //Add the node to the treetop network.
          //The index is broken into 1000 node chunks.
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->search(Current_TtID);
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->set_Current_Node_NID(Nodes[p_Tier] [p_Index]);
          
          //Sets the ID of the treetop node.
          Nodes[p_Tier] [p_Index]->set_Treetop(Current_TtID);
          
          //Sets the U_CID, Tier, and Index;
          Nodes[p_Tier] [p_Index]->set_CID(CID);
          Nodes[p_Tier] [p_Index]->set_Tier(p_Tier);
          Nodes[p_Tier] [p_Index]->set_Index(p_Index);
          
          //Increment the treetop node counter.
          Current_TtID.I += 100;
          
          //If a new chunk needs allocated then allocate it for the treetops.
          if (((Current_TtID.I - CID) % 1000) == 0)
          {
               increase_Treetop_Bin_Tree(int ((Current_TtID.I - CID) / 1000) + 1);
          }
          return Nodes[p_Tier] [p_Index];
     }
     
     //Creates a state node with the given state.
     virtual c_MT_Base_Node_1D * new_Treetop_RC_State_Node(int p_Tier, int p_Index, u_Data p_State)
     {
          Nodes[p_Tier] [p_Index] = new c_MT_Treetop_RC_State_Node_1D;
          
          Nodes[p_Tier] [p_Index]->set_State(p_State);
                    
          //Add the node to the treetop network.
          //The index is broken into 1000 node chunks.
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->search(Current_TtID);
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->set_Current_Node_NID(Nodes[p_Tier] [p_Index]);
          
          //Sets the ID of the treetop node.
          Nodes[p_Tier] [p_Index]->set_Treetop(Current_TtID);
          
          //Sets the U_CID, Tier, and Index;
          Nodes[p_Tier] [p_Index]->set_CID(CID);
          Nodes[p_Tier] [p_Index]->set_Tier(p_Tier);
          Nodes[p_Tier] [p_Index]->set_Index(p_Index);
          
          //Increment the treetop node counter.
          Current_TtID.I += 100;
          
          //If a new chunk needs allocated then allocate it for the treetops.
          if (((Current_TtID.I - CID) % 1000) == 0)
          {
               increase_Treetop_Bin_Tree(int ((Current_TtID.I - CID) / 1000) + 1);
          }
          
          return Nodes[p_Tier] [p_Index];
     }
     
     
     //--     NODE CREATION
     
     //Creates a new node on the given tier.
     virtual c_MT_Base_Node_1D * new_Node(int p_Tier)
     {
          //If the tier does not yet exist then create it by expanding the arrays as necessary.
          if (p_Tier >= Number_Of_Tiers)
          {
               increase_Number_Of_Tiers(p_Tier + 1);
          }
          
          //Increase the number of nodes to accommodate the new node.
          increment_Number_Of_Nodes_On_Tier(p_Tier);
          
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1] = new c_MT_Simple_Node;
          
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Index(*Node_Count[p_Tier] - 1);
          
          return Nodes[p_Tier] [*Node_Count[p_Tier] - 1];
     }
     
     //Creates a new node on the given tier.
     virtual c_MT_Base_Node_1D * new_RC_Node(int p_Tier)
     {
          //If the tier does not yet exist then create it by expanding the arrays as necessary.
          if (p_Tier >= Number_Of_Tiers)
          {
               increase_Number_Of_Tiers(p_Tier + 1);
          }
          
          //Increase the number of nodes to accommodate the new node.
          increment_Number_Of_Nodes_On_Tier(p_Tier);
          
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1] = new c_MT_RC_Node_1D;
          
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Index(*Node_Count[p_Tier] - 1);
          
          return Nodes[p_Tier] [*Node_Count[p_Tier] - 1];
     }
     
     //Creates a new node on the given tier.
     virtual c_MT_Base_Node_1D * new_Treetop_Node(int p_Tier)
     {
          //If the tier does not yet exist then create it by expanding the arrays as necessary.
          if (p_Tier >= Number_Of_Tiers)
          {
               increase_Number_Of_Tiers(p_Tier + 1);
          }
          
          //Increase the number of nodes to accommodate the new node.
          increment_Number_Of_Nodes_On_Tier(p_Tier);
          
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1] = new c_MT_Treetop_Simple_Node_1D;
          
          //Add the node to the treetop network.
          //The index is broken into 1000 node chunks.
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->search(Current_TtID);
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->set_Current_Node_NID(Nodes[p_Tier] [*Node_Count[p_Tier] - 1]);
          
          //Sets the state for the node to the identifier.
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Treetop(Current_TtID);
                    
          //Sets the U_CID, Tier, and Index;
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_CID(CID);
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Tier(p_Tier);
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Index(*Node_Count[p_Tier] - 1);
          
          //Increment the treetop node counter.
          Current_TtID.I += 100;
          
          //If a new chunk needs allocated then allocate it for the treetops.
          if (((Current_TtID.I - CID) % 1000) == 0)
          {
               increase_Treetop_Bin_Tree(int ((Current_TtID.I - CID) / 1000) + 1);
          }
          
          return Nodes[p_Tier] [*Node_Count[p_Tier] - 1];
     }
     
     
     //Creates a new node on the given tier.
     virtual c_MT_Base_Node_1D * new_Treetop_RC_Node(int p_Tier)
     {
          //If the tier does not yet exist then create it by expanding the arrays as necessary.
          if (p_Tier >= Number_Of_Tiers)
          {
               increase_Number_Of_Tiers(p_Tier + 1);
          }
          
          //Increase the number of nodes to accommodate the new node.
          increment_Number_Of_Nodes_On_Tier(p_Tier);
          
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1] = new c_MT_Treetop_RC_Node_1D;
          
          
          //Add the node to the treetop network.
          //The index is broken into 1000 node chunks.
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->search(Current_TtID);
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->set_Current_Node_NID(Nodes[p_Tier] [*Node_Count[p_Tier] - 1]);
          
          //Sets the ID of the treetop node.
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Treetop(Current_TtID);
          
          //Sets the U_CID, Tier, and Index;
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_CID(CID);
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Tier(p_Tier);
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Index(*Node_Count[p_Tier] - 1);
          
          //Increment the treetop node counter.
          Current_TtID.I += 100;
          
          //If a new chunk needs allocated then allocate it for the treetops.
          if (((Current_TtID.I - CID) % 1000) == 0)
          {
               increase_Treetop_Bin_Tree(int ((Current_TtID.I - CID) / 1000) + 1);
          }
          
          return Nodes[p_Tier] [*Node_Count[p_Tier] - 1];
     }
     
     
     //Creates a state node with the given state.
     virtual c_MT_Base_Node_1D * new_State_Node(u_Data p_State)
     {
          //If the tier does not yet exist then create it by expanding the arrays as necessary.
          if (Number_Of_Tiers < 1)
          {
               increase_Number_Of_Tiers(1);
          }
          
          //Increase the number of nodes to accommodate the new node.
          increment_Number_Of_Nodes_On_Tier(0);
          
          Nodes[0] [*Node_Count[0] - 1] = new c_MT_State_Node_1D;
          
          Nodes[0] [*Node_Count[0] - 1]->set_Index(*Node_Count[0] - 1);
          
          Nodes[0] [*Node_Count[0] - 1]->set_State(p_State);
          
          return Nodes[0] [*Node_Count[0] - 1];
     }
     
     //Creates a state node with the given state.
     virtual c_MT_Base_Node_1D * new_RC_State_Node(u_Data p_State)
     {
          //If the tier does not yet exist then create it by expanding the arrays as necessary.
          if (Number_Of_Tiers < 1)
          {
               increase_Number_Of_Tiers(1);
          }
          
          //Increase the number of nodes to accommodate the new node.
          increment_Number_Of_Nodes_On_Tier(0);
          
          Nodes[0] [*Node_Count[0] - 1] = new c_MT_RC_State_Node_1D;
          
          Nodes[0] [*Node_Count[0] - 1]->set_Index(*Node_Count[0] - 1);
          
          Nodes[0] [*Node_Count[0] - 1]->set_State(p_State);
          
          return Nodes[0] [*Node_Count[0] - 1];
     }
          
     //Creates a state node with the given state.
     virtual c_MT_Base_Node_1D * new_Treetop_State_Node(int p_Tier, u_Data p_State)
     {
          //If the tier does not yet exist then create it by expanding the arrays as necessary.
          if (p_Tier >= Number_Of_Tiers)
          {
               increase_Number_Of_Tiers(p_Tier + 1);
          }
          
          //Increase the number of nodes to accommodate the new node.
          increment_Number_Of_Nodes_On_Tier(p_Tier);
          
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1] = new c_MT_Treetop_State_Node_1D;
          
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_State(p_State);
                    
          //Add the node to the treetop network.
          //The index is broken into 1000 node chunks.
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->search(Current_TtID);
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->set_Current_Node_NID(Nodes[p_Tier] [*Node_Count[p_Tier] - 1]);
          
          //Sets the ID of the treetop node.
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Treetop(Current_TtID);
          
          //Sets the U_CID, Tier, and Index;
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_CID(CID);
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Tier(p_Tier);
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Index(*Node_Count[p_Tier] - 1);
          
          //Increment the treetop node counter.
          Current_TtID.I += 100;
          
          //If a new chunk needs allocated then allocate it for the treetops.
          if (((Current_TtID.I - CID) % 1000) == 0)
          {
               increase_Treetop_Bin_Tree(int ((Current_TtID.I - CID) / 1000) + 1);
          }
          return Nodes[p_Tier] [*Node_Count[p_Tier] - 1];
     }
     
     //Creates a state node with the given state.
     virtual c_MT_Base_Node_1D * new_Treetop_RC_State_Node(int p_Tier, u_Data p_State)
     {
          //If the tier does not yet exist then create it by expanding the arrays as necessary.
          if (p_Tier >= Number_Of_Tiers)
          {
               increase_Number_Of_Tiers(p_Tier + 1);
          }
          
          //Increase the number of nodes to accommodate the new node.
          increment_Number_Of_Nodes_On_Tier(p_Tier);
          
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1] = new c_MT_Treetop_RC_State_Node_1D;
          
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_State(p_State);
                    
          //Add the node to the treetop network.
          //The index is broken into 1000 node chunks.
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->search(Current_TtID);
          Treetop_Bin_Tree[int (Current_TtID.I / 1000)]->set_Current_Node_NID(Nodes[p_Tier] [*Node_Count[p_Tier] - 1]);
          
          //Sets the ID of the treetop node.
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Treetop(Current_TtID);
                    
          //Sets the U_CID, Tier, and Index;
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_CID(CID);
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Tier(p_Tier);
          Nodes[p_Tier] [*Node_Count[p_Tier] - 1]->set_Index(*Node_Count[p_Tier] - 1);
          
          //Increment the treetop node counter.
          Current_TtID.I += 100;
          
          //If a new chunk needs allocated then allocate it for the treetops.
          if (((Current_TtID.I - CID) % 1000) == 0)
          {
               increase_Treetop_Bin_Tree(int ((Current_TtID.I - CID) / 1000) + 1);
          }
          
          return Nodes[p_Tier] [*Node_Count[p_Tier] - 1];
     }
     
     
     //==--     CHECKS FOR EXISTING NODES AND CREATES THEM IF THEY DO NOT EXIST
     
     //Gets a state node.
     virtual c_MT_Base_Node_1D * get_State_Node(u_Data p_State, int p_Tree = 0)
     {
          //increase_State_Bin_Tree(p_Tree);
          //[p_Tree]
          
          //Searches the tree for the given state.
          State_Tree.search(p_State);
          
          //Control statement for the flg_Foundit.
          if (State_Tree.flg_Foundit)
          {
               //If the state was found return the node.
               return State_Tree.get_Current_Node_NID();
          }
          else
          {
               //If the state was not found then return the newly created node.
               return State_Tree.set_Current_Node_NID(new_State_Node(p_State));
          }
     }
     
     //Gets a state node.
     virtual c_MT_Base_Node_1D * get_RC_State_Node(u_Data p_State, int p_Tree = 0)
     {
          //increase_State_Bin_Tree(p_Tree);
          //if (Name == "P_Nodes"){ cout << "\n\n\t get_RC_State_Node->" << p_State.I; }
          
          //Searches the tree for the given state.
          State_Tree.search(p_State);
          
          //Control statement for the flg_Foundit.
          if (State_Tree.flg_Foundit)
          {
               //if (Name == "P_Nodes"){ cout << "\n\t  found->" << p_State.I << " Node->" << State_Tree.get_Current_Node_NID(); }
               
               //If the state was found return the node.
               return State_Tree.get_Current_Node_NID();
          }
          else
          {
               //if (Name == "P_Nodes"){ cout << "\n\t  NOT found->" << p_State.I << " creating_Node"; State_Tree.search_Verbose(p_State); system("PAUSE > NULL"); }
               
               //If the state was not found then return the newly created node.
               return State_Tree.set_Current_Node_NID(new_RC_State_Node(p_State));
          }
     }
     
     
     //Gets a state node.
     virtual c_MT_Base_Node_1D * get_Treetop_State_Node(u_Data p_State, int p_Tree = 0)
     {
          //increase_State_Bin_Tree(p_Tree);
          
          //Searches the tree for the given state.
          State_Tree.search(p_State);
          
          //Control statement for the flg_Foundit.
          if (State_Tree.flg_Foundit)
          {
               //If the state was found return the node.
               return State_Tree.get_Current_Node_NID();
          }
          else
          {
               //If the state was not found then return the newly created node.
               return State_Tree.set_Current_Node_NID(new_Treetop_State_Node(0, p_State));
          }
     }
     
     //Gets a state node.
     virtual c_MT_Base_Node_1D * get_Treetop_RC_State_Node(u_Data p_State, int p_Tree = 0)
     {
          //increase_State_Bin_Tree(p_Tree);
          
          //Searches the tree for the given state.
          State_Tree.search(p_State);
          
          //Control statement for the flg_Foundit.
          if (State_Tree.flg_Foundit)
          {
               //If the state was found return the node.
               return State_Tree.get_Current_Node_NID();
          }
          else
          {
               //If the state was not found then return the newly created node.
               return State_Tree.set_Current_Node_NID(new_Treetop_RC_State_Node(0, p_State));
          }
     }
          
     //Gets a treetop node associated with the given state.
     virtual c_MT_Base_Node_1D * get_Treetop_Node(u_Data p_State)
     {
          if (p_State.I >= Current_TtID.I){ return NULL; }
          
          //Searches the tree for the given state.
          Treetop_Bin_Tree[int (p_State.I / 1000)]->search(p_State);
          
          //Control statement for the flg_Foundit.
          if (Treetop_Bin_Tree[int (p_State.I / 1000)]->flg_Foundit)
          {
               //If the state was found return the node.
               return Treetop_Bin_Tree[int (p_State.I / 1000)]->get_Current_Node_NID();
          }
          else
          {
               //If the state was not found then return a NULL.
               return NULL;
          }
     }
          
     //Sets a treetop node associated with the given state.
     virtual void set_Treetop_Node(u_Data p_State, c_MT_Base_Node_1D * p_Node)
     {    
          if (p_State.I >= Current_TtID.I){ return; }
          
          //Searches the tree for the given state.
          Treetop_Bin_Tree[int (p_State.I / 1000)]->search(p_State);
          
          //Control statement for the flg_Foundit.
          if (Treetop_Bin_Tree[int (p_State.I / 1000)]->flg_Foundit)
          {
               //If the state was found return the node.
               return;
          }
          else
          {
               //If the state was not found then return the newly created node.
               Treetop_Bin_Tree[int (p_State.I / 1000)]->set_Current_Node_NID(p_Node);
          }
     }
     
     //Gets a reference to a treetop node.
     virtual c_MT_Base_Node_1D * get_Treetop_Node_Reference(u_Data p_Handle)
     {
          return Treetop_Bin_Tree[int (p_Handle.I / 1000)]->get_NID(p_Handle);
     }
     

     
     //==--     CREATES A TREETOP NODE IF NONE EXISTS
     
     //Gets the upper tier connectino between the given nodes, if none found one is created and returned, this is for treetops.
     c_MT_Base_Node_1D * get_Treetop_Connection(int p_Upper_Tier, c_MT_Base_Node_1D * p_Left, c_MT_Base_Node_1D * p_Right)
     {
          //Temporary node to hold the connection when found.
          c_MT_Base_Node_1D * tmp_Upper_Tier_Connection;
          
          //Check if one already exists.
          tmp_Upper_Tier_Connection = does_Upper_Tier_Connection_Exist(p_Left, p_Right);
          
          //If not then create one.
          if (!tmp_Upper_Tier_Connection)
          {
               tmp_Upper_Tier_Connection = create_Treetop_Connection(p_Upper_Tier, p_Left, p_Right);
          }
          
          //Return the Upper_Tier_Connection.
          return tmp_Upper_Tier_Connection;
          
     }
     
     //Gets the upper tier connectino between the given nodes, if none found one is created and returned, this is for treetops.
     c_MT_Base_Node_1D * get_Treetop_RC_Connection(int p_Upper_Tier, c_MT_Base_Node_1D * p_Left, c_MT_Base_Node_1D * p_Right)
     {
          //Temporary node to hold the connection when found.
          c_MT_Base_Node_1D * tmp_Upper_Tier_Connection;
          
          //Check if one already exists.
          tmp_Upper_Tier_Connection = does_Upper_Tier_Connection_Exist(p_Left, p_Right);
          
          //If not then create one.
          if (!tmp_Upper_Tier_Connection)
          {
               tmp_Upper_Tier_Connection = create_Treetop_RC_Connection(p_Upper_Tier, p_Left, p_Right);
          }
          
          //Return the Upper_Tier_Connection.
          return tmp_Upper_Tier_Connection;
          
     }
     
     //Creates an upper tier connection.
     c_MT_Base_Node_1D * create_Treetop_Connection(int p_Upper_Tier, c_MT_Base_Node_1D * p_Left, c_MT_Base_Node_1D * p_Right)
     {
          //Create the upper tier node to connect to.
          c_MT_Base_Node_1D * Upper_Tier_Node = new_Treetop_Node(p_Upper_Tier);
          
          //Create the connections.
          create_Connection(0, p_Left, Upper_Tier_Node);
          create_Connection(1, p_Right, Upper_Tier_Node);
          
          //Return the newly created upper tier connection.
          return Upper_Tier_Node;
     }
     
     //Creates treetop RC connection.
     c_MT_Base_Node_1D * create_Treetop_RC_Connection(int p_Upper_Tier, c_MT_Base_Node_1D * p_Left, c_MT_Base_Node_1D * p_Right)
     {
          //Create the upper tier node to connect to.
          c_MT_Base_Node_1D * Upper_Tier_Node = new_Treetop_RC_Node(p_Upper_Tier);
          
          //Create the connections.
          create_Connection(0, p_Left, Upper_Tier_Node);
          create_Connection(1, p_Right, Upper_Tier_Node);
          
          //Return the newly created upper tier connection.
          return Upper_Tier_Node;
     }
     
     
     //==--     QUERYING FOR UPPER TIER NODES
     
     //--   PURE
     
     //Creates an upper tier connection.
     c_MT_Base_Node_1D * create_Upper_Tier_Connection(int p_Upper_Tier, c_MT_Base_Node_1D * p_Left, c_MT_Base_Node_1D * p_Right)
     {
          //Create the upper tier node to connect to.
          c_MT_Base_Node_1D * Upper_Tier_Node = new_Node(p_Upper_Tier);
          
          //Create the connections.
          create_Connection(0, p_Left, Upper_Tier_Node);
          create_Connection(1, p_Right, Upper_Tier_Node);
          
          //Return the newly created upper tier connection.
          return Upper_Tier_Node;
     }
     
     //Returns true of false depending on if an upper tier connection already exists.
     c_MT_Base_Node_1D * does_Upper_Tier_Connection_Exist(c_MT_Base_Node_1D * p_Left, c_MT_Base_Node_1D * p_Right)
     {
          //If either are NULL then no.
          if (p_Left == NULL || p_Right == NULL){ return NULL; }
          
          //If either of them have no axons then the answer is no.
          if (!p_Left->get_Axon_Count(0) || !p_Right->get_Axon_Count(1))
          {
               return 0;
          }
          
          //Check the axons on the left node, if it does not connect the asnwer is no.
          return p_Left->does_Upper_Tier_Connection_Exist(p_Right);
     }
     
     //Gets the upper tier connectino between the given nodes, if none found one is created and returned.
     c_MT_Base_Node_1D * get_Upper_Tier_Connection(int p_Upper_Tier, c_MT_Base_Node_1D * p_Left, c_MT_Base_Node_1D * p_Right)
     {
          //Temporary node to hold the connection when found.
          c_MT_Base_Node_1D * tmp_Upper_Tier_Connection;
          
          //Check if one already exists.
          tmp_Upper_Tier_Connection = does_Upper_Tier_Connection_Exist(p_Left, p_Right);
          
          //If not then create one.
          if (!tmp_Upper_Tier_Connection)
          {
               tmp_Upper_Tier_Connection = create_Upper_Tier_Connection(p_Upper_Tier, p_Left, p_Right);
          }
          
          //Return the Upper_Tier_Connection.
          return tmp_Upper_Tier_Connection;
          
     }
     
     //--   RC
     
     //Creates an upper tier connection.
     c_MT_Base_Node_1D * create_Upper_Tier_Connection_RC(int p_Upper_Tier, c_MT_Base_Node_1D * p_Left, c_MT_Base_Node_1D * p_Right)
     {
          //Create the upper tier node to connect to.
          c_MT_Base_Node_1D * Upper_Tier_Node = new_RC_Node(p_Upper_Tier);
          
          //Create the connections.
          create_Connection(0, p_Left, Upper_Tier_Node);
          create_Connection(1, p_Right, Upper_Tier_Node);
          
          //Return the newly created upper tier connection.
          return Upper_Tier_Node;
     }
     
     //Returns true of false depending on if an upper tier connection already exists.
     c_MT_Base_Node_1D * does_Upper_Tier_Connection_Exist_RC(c_MT_Base_Node_1D * p_Left, c_MT_Base_Node_1D * p_Right)
     {
          //If either of them have no axons then the answer is no.
          if (!p_Left->get_Axon_Count(0) || !p_Right->get_Axon_Count(1))
          {
               return 0;
          }
          
          //Check the axons on the left node, if it does not connect the asnwer is no.
          return p_Left->does_Upper_Tier_Connection_Exist(p_Right);
     }
     
     //Gets the upper tier connectino between the given nodes, if none found one is created and returned.
     c_MT_Base_Node_1D * get_Upper_Tier_Connection_RC(int p_Upper_Tier, c_MT_Base_Node_1D * p_Left, c_MT_Base_Node_1D * p_Right)
     {
          //Temporary node to hold the connection when found.
          c_MT_Base_Node_1D * tmp_Upper_Tier_Connection;
          
          //Check if one already exists.
          tmp_Upper_Tier_Connection = does_Upper_Tier_Connection_Exist_RC(p_Left, p_Right);
          
          //If not then create one.
          if (!tmp_Upper_Tier_Connection)
          {
               tmp_Upper_Tier_Connection = create_Upper_Tier_Connection_RC(p_Upper_Tier, p_Left, p_Right);
          }
          
          //Return the Upper_Tier_Connection.
          return tmp_Upper_Tier_Connection;
          
     }
     
     //--   CHRONO PURE
     
     //Creates a chrono connection.
     c_MT_Base_Node_1D * create_Chrono_Connection(c_MT_Base_Node_1D * p_Previous, c_MT_Base_Node_1D * p_Lower, int p_Tier)
     {          
          //Create the chrono tier node to connect to.
          c_MT_Base_Node_1D * Chrono_Node = new_Treetop_Node(p_Tier);
          
          //Create the connections.
          create_Connection(0, p_Previous, Chrono_Node);
          create_Connection(1, p_Lower, Chrono_Node);
          
          //Return the newly created upper tier connection.
          return Chrono_Node;
     }
     
     //Returns true or false depending upon whether or not the current chrono node exists.
     c_MT_Base_Node_1D * does_Chrono_Connection_Exist(c_MT_Base_Node_1D * p_Previous_Node, c_MT_Base_Node_1D * p_Lower)
     {
          //If either of them have no axons then the answer is no.
          if (!p_Previous_Node->get_Axon_Count(0) || !p_Lower->get_Axon_Count(1))
          {
               return 0;
          }
          
          //Check the axons on the left node, if it does not connect the asnwer is no.
          return p_Previous_Node->does_Upper_Tier_Connection_Exist(p_Lower);
     }
     
     //Gets the chrono node linking the queried nodes, if none are found it creates one and returns it instead.
     c_MT_Base_Node_1D * get_Chrono_Connection(c_MT_Base_Node_1D * p_Previous, c_MT_Base_Node_1D * p_Lower, int p_Tier = 1)
     {
          //Temporary node to hold the connection when found.
          c_MT_Base_Node_1D * tmp_Chrono_Connection;
          
          //Check if one already exists.
          tmp_Chrono_Connection = does_Chrono_Connection_Exist(p_Previous, p_Lower);
          
          //If not then create one.
          if (!tmp_Chrono_Connection)
          {
               tmp_Chrono_Connection = create_Chrono_Connection(p_Previous, p_Lower, p_Tier);
          }
          
          //Return the tmp_Chrono_Connection.
          return tmp_Chrono_Connection;
          
     }
     
     //--   CHRONO RC
     
     //Creates a chrono connection.
     c_MT_Base_Node_1D * create_Chrono_Connection_RC(c_MT_Base_Node_1D * p_Previous, c_MT_Base_Node_1D * p_Lower, int p_Tier)
     {          
          //Create the chrono tier node to connect to.
          c_MT_Base_Node_1D * Chrono_Node = new_Treetop_RC_Node(p_Tier);
          
          //Create the connections.
          create_Connection(0, p_Previous, Chrono_Node);
          create_Connection(1, p_Lower, Chrono_Node);
          
          //Return the newly created upper tier connection.
          return Chrono_Node;
     }
     
     //Returns true or false depending upon whether or not the current chrono node exists.
     c_MT_Base_Node_1D * does_Chrono_Connection_Exist_RC(c_MT_Base_Node_1D * p_Previous_Node, c_MT_Base_Node_1D * p_Lower)
     {
          //If either of them have no axons then the answer is no.
          if (!p_Previous_Node->get_Axon_Count(0) || !p_Lower->get_Axon_Count(1))
          {
               return 0;
          }
          
          //Check the axons on the left node, if it does not connect the asnwer is no.
          return p_Previous_Node->does_Upper_Tier_Connection_Exist(p_Lower);
     }
     
     //Gets the chrono node linking the queried nodes, if none are found it creates one and returns it instead.
     c_MT_Base_Node_1D * get_Chrono_Connection_RC(c_MT_Base_Node_1D * p_Previous, c_MT_Base_Node_1D * p_Lower, int p_Tier = 1)
     {
          //Temporary node to hold the connection when found.
          c_MT_Base_Node_1D * tmp_Chrono_Connection;
          
          //Check if one already exists.
          tmp_Chrono_Connection = does_Chrono_Connection_Exist_RC(p_Previous, p_Lower);
          
          //If not then create one.
          if (!tmp_Chrono_Connection)
          {
               tmp_Chrono_Connection = create_Chrono_Connection_RC(p_Previous, p_Lower, p_Tier);
          }
          
          //Return the tmp_Chrono_Connection.
          return tmp_Chrono_Connection;
          
     }
     
     //==--      CONNECTION AND CHARGING MANIPULATIONS
          
     //Creates a connection between two nodes.
     void create_Connection(int p_Leg, c_MT_Base_Node_1D * p_From, c_MT_Base_Node_1D * p_To)
     {
          p_From->add_Axon(p_Leg, p_To);
          p_To->set_Dendrite(p_Leg, p_From);
     }
     
     //Sets the state of a charging leg.
     void set_Charging_Leg(int p_Leg, int p_State)
     {
          Charging_Legs[p_Leg] = p_State;
     }
     
     //--     ARRAY EXPANSION FUNCTIONS
     
     //Increases the treetop bin tree array.
     void increase_Treetop_Bin_Tree(int p_Chunk)
     {
          //cout << "\n\t\t increase_Treetop_Bin_Tree(" << p_Chunk << ")";
          
          int tmp_Size = Current_TtID.I / 1000;
          
          //Allocate the tmp array.
          c_MT_Fractal_State_Bin_Tree ** tmp_Treetop_Bin_Tree;
          tmp_Treetop_Bin_Tree = new c_MT_Fractal_State_Bin_Tree*[tmp_Size];
          
          //Read the old ones into the newly allocated temporary array.
          for (int cou_Index=0;cou_Index<tmp_Size;cou_Index++)
          {
               tmp_Treetop_Bin_Tree[cou_Index] = Treetop_Bin_Tree[cou_Index];
               Treetop_Bin_Tree[cou_Index] = NULL;
          }
          
          //Delete the old array.
          delete [] Treetop_Bin_Tree;
          
          //Reallocate and copy the old array back into it.
          Treetop_Bin_Tree = new c_MT_Fractal_State_Bin_Tree*[p_Chunk];
          
          //Allocate the new tree.
          Treetop_Bin_Tree[p_Chunk -1] = new c_MT_Fractal_State_Bin_Tree;
          
          for (int cou_Index=0;cou_Index<tmp_Size;cou_Index++)
          {
               Treetop_Bin_Tree[cou_Index] = tmp_Treetop_Bin_Tree[cou_Index];
               tmp_Treetop_Bin_Tree[cou_Index] = NULL;
          }
          
          //Cleanup.
          delete [] tmp_Treetop_Bin_Tree;
     }
        
     /*
     //Increases the treetop bin tree array.
     void increase_State_Bin_Tree(int p_Chunk)
     {
          if (p_Chunk <= State_Tree_Count){ return; }
          
          //Allocate the tmp array.
          c_State_Node_Bin_Tree_1D ** tmp_State_Bin_Tree;
          tmp_State_Bin_Tree = new c_State_Node_Bin_Tree_1D*[State_Tree_Count];
          
          //Read the old ones into the newly allocated temporary array.
          for (int cou_Index=0;cou_Index<State_Tree_Count;cou_Index++)
          {
               tmp_State_Bin_Tree[cou_Index] = State_Tree[cou_Index];
               State_Tree[cou_Index] = NULL;
          }
          
          //Delete the old array.
          delete [] State_Tree;
          State_Tree = NULL;
          
          //Reallocate and copy the old array back into it.
          State_Tree = new c_State_Node_Bin_Tree_1D*[p_Chunk];
          
          //Allocate the new trees.
          for (int cou_Index=State_Tree_Count;cou_Index<p_Chunk;cou_Index++)
          {
               State_Tree[cou_Index] = new c_State_Node_Bin_Tree_1D;
          }
          
          for (int cou_Index=0;cou_Index<State_Tree_Count;cou_Index++)
          {
               State_Tree[cou_Index] = tmp_State_Bin_Tree[cou_Index];
               tmp_State_Bin_Tree[cou_Index] = NULL;
          }
          
          //Cleanup.
          delete [] tmp_State_Bin_Tree;
          tmp_State_Bin_Tree = NULL;
     }
     */
     
     //Increases the number of tiers to a given level.
     void increase_Number_Of_Tiers(int p_Tier)
     {
          //Setup the temporary array that will hold the tiers.
          c_MT_Base_Node_1D *** tmp_Nodes = new c_MT_Base_Node_1D ** [Number_Of_Tiers];
          
          //Setup the temporary array that will hold the node count.
          int ** tmp_Node_Count = new int * [Number_Of_Tiers];
          
          //Setup the temporary array that will hold the node count MAX.
          int ** tmp_Node_Count_MAX = new int * [Number_Of_Tiers];
          
          //Loop through each tier allocating the appropriate number of nodes and copying the old ones into the new array.
          //The node count will be copied as well.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               //Allocate the number of nodes necessary.
               tmp_Nodes[cou_T] = new c_MT_Base_Node_1D * [*Node_Count[cou_T]];
               
               //Copy the old nodes into the temporary tier.
               for (int cou_NID=0;cou_NID<*Node_Count[cou_T];cou_NID++)
               {
                    tmp_Nodes[cou_T] [cou_NID] = Nodes[cou_T] [cou_NID];
               }
               
               //Delete the current tier in the original nodes array.
               delete [] Nodes[cou_T];
               Nodes[cou_T] = NULL;
               
               //Copy the node count.
               tmp_Node_Count[cou_T] = Node_Count[cou_T];
               tmp_Node_Count_MAX[cou_T] = Node_Count_MAX[cou_T];
          }
          
          //Delete the Node_Count array and reallocate it to the appropriate height, then copy the old values into it.
          delete [] Node_Count;
          Node_Count = NULL;
          
          delete [] Node_Count_MAX;
          Node_Count_MAX = NULL;
          
          Node_Count = new int * [p_Tier];
          Node_Count_MAX = new int * [p_Tier];
          
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               Node_Count[cou_T] = tmp_Node_Count[cou_T];
               Node_Count_MAX[cou_T] = tmp_Node_Count_MAX[cou_T];
          }
          
          //Create the count for the new tiers.
          for (int cou_T=Number_Of_Tiers;cou_T<p_Tier;cou_T++)
          {
               Node_Count[cou_T] = new int;
               *Node_Count[cou_T] = 0;
               Node_Count_MAX[cou_T] = new int;
               *Node_Count_MAX[cou_T] = 0;
          }
          
          //Delete the temporary array.
          delete [] tmp_Node_Count;
          tmp_Node_Count = NULL;
          
          delete [] tmp_Node_Count_MAX;
          tmp_Node_Count_MAX = NULL;
          
          //Delete the node array and reallocate it to the appropriate height.
          delete [] Nodes;
          Nodes = NULL;
          Nodes = new c_MT_Base_Node_1D ** [p_Tier];
          
          //Allocate the nodes for the tiers and copy them into the node array from the temporary array.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               Nodes[cou_T] = new c_MT_Base_Node_1D * [*Node_Count[cou_T]];
               
               for (int cou_NID=0;cou_NID<*Node_Count[cou_T];cou_NID++)
               {
                    Nodes[cou_T] [cou_NID] = tmp_Nodes[cou_T] [cou_NID];
               }
               
               //Cleanup the tiers on the temporary after it is copied.
               delete [] tmp_Nodes[cou_T];
               tmp_Nodes[cou_T] = NULL;
               
          }
          
          //Delete the tmp_Nodes array as it is no longer needed.
          delete [] tmp_Nodes;
          tmp_Nodes = NULL;
          
          //NULL the newly created tiers.
          for (int cou_T=Number_Of_Tiers;cou_T<p_Tier;cou_T++)
          {
               Nodes[cou_T] = new c_MT_Base_Node_1D * [1];
               Nodes[cou_T] [0] = NULL;
          }
          
          //Set the number of tiers to the correct amount.
          Number_Of_Tiers = p_Tier;
          
     }
     
     
     //Increases the number of nodes on a given tier.
     void increase_Number_Of_Nodes_On_Tier(int p_Tier, int p_Index)
     {
          //If the number of nodes has reached the MAX_Node_Count for a tier then
          //the tier is expanded, if not this will just return.
          if (*Node_Count[p_Tier] < *Node_Count_MAX[p_Tier]){ return; }
          if (*Node_Count[p_Tier] >= p_Index){ return; }
          
          //Increase the maximun number of indexes.
          *Node_Count_MAX[p_Tier] += (int (*Node_Count_MAX[p_Tier] * .75) + 1);
          
          /* With the installation of the MAX counters this step will not be neccesary as the tier has nodes.
          //If the tier has no nodes then reallocate the tier withouth the tedious copying.
          if (*Node_Count[p_Tier] == 0)
          {
               //Delete the current tier and allocate it.
               delete [] Nodes[p_Tier];
               Nodes[p_Tier] = NULL;
               
               Nodes[p_Tier] = new c_MT_Base_Node_1D * [p_Index];
               for (int cou_Index=0;cou_Index<p_Index;cou_Index++)
               {
                    Nodes[p_Tier] [cou_Index] = NULL;
               }
               
               *Node_Count[p_Tier] = p_Index;
               return;
          }*/
          
          //Setup the temporay array to hold the nodes while it is reallocated.
          c_MT_Base_Node_1D ** tmp_Tier;
          tmp_Tier = new c_MT_Base_Node_1D * [*Node_Count[p_Tier]];
          
          //Copy the nodes into the temporary array.
          for (int cou_NID=0;cou_NID<*Node_Count[p_Tier];cou_NID++)
          {
               tmp_Tier[cou_NID] = Nodes[p_Tier] [cou_NID];
          }
          
          //Delete the original and reallocate it to the current size plus one.
          delete [] Nodes[p_Tier];
          Nodes[p_Tier] = NULL;
          
          Nodes[p_Tier] = new c_MT_Base_Node_1D * [*Node_Count_MAX[p_Tier]];
          
          //Copy the old nodes back into the tier.
          for (int cou_NID=0;cou_NID<*Node_Count[p_Tier];cou_NID++)
          {
               Nodes[p_Tier] [cou_NID] = tmp_Tier[cou_NID];
          }
          
          //Cleanup the temporary array.
          delete [] tmp_Tier;
          tmp_Tier = NULL;
          
          //NULL the newest nodes.
          for (int cou_Index=*Node_Count[p_Tier];cou_Index<*Node_Count_MAX[p_Tier];cou_Index++)
          {
               Nodes[p_Tier] [cou_Index] = NULL;
          }
          
          //Increment the counter.
          *Node_Count[p_Tier] = p_Index;
          
     }
     
     //Increases the number of nodes on a given tier.
     void increase_Number_Of_Nodes_On_Tier_Loading(int p_Tier, int p_Index)
     {
          //If the number of nodes has reached the MAX_Node_Count for a tier then
          //the tier is expanded, if not this will just return.
          //if (*Node_Count[p_Tier] < *Node_Count_MAX[p_Tier]){ return; }
          if (*Node_Count[p_Tier] >= p_Index){ return; }
          
          //Increase the maximun number of indexes.
          *Node_Count_MAX[p_Tier] += (int (*Node_Count_MAX[p_Tier] * .75) + p_Index);
          
          /* With the installation of the MAX counters this step will not be neccesary as the tier has nodes.
          //If the tier has no nodes then reallocate the tier withouth the tedious copying.
          if (*Node_Count[p_Tier] == 0)
          {
               //Delete the current tier and allocate it.
               delete [] Nodes[p_Tier];
               Nodes[p_Tier] = NULL;
               
               Nodes[p_Tier] = new c_MT_Base_Node_1D * [p_Index];
               for (int cou_Index=0;cou_Index<p_Index;cou_Index++)
               {
                    Nodes[p_Tier] [cou_Index] = NULL;
               }
               
               *Node_Count[p_Tier] = p_Index;
               return;
          }*/
          
          //Setup the temporay array to hold the nodes while it is reallocated.
          c_MT_Base_Node_1D ** tmp_Tier;
          tmp_Tier = new c_MT_Base_Node_1D * [*Node_Count[p_Tier]];
          
          //Copy the nodes into the temporary array.
          for (int cou_NID=0;cou_NID<*Node_Count[p_Tier];cou_NID++)
          {
               tmp_Tier[cou_NID] = Nodes[p_Tier] [cou_NID];
          }
          
          //Delete the original and reallocate it to the current size plus one.
          delete [] Nodes[p_Tier];
          Nodes[p_Tier] = NULL;
          
          Nodes[p_Tier] = new c_MT_Base_Node_1D * [*Node_Count_MAX[p_Tier]];
          
          //Copy the old nodes back into the tier.
          for (int cou_NID=0;cou_NID<*Node_Count[p_Tier];cou_NID++)
          {
               Nodes[p_Tier] [cou_NID] = tmp_Tier[cou_NID];
          }
          
          //Cleanup the temporary array.
          delete [] tmp_Tier;
          tmp_Tier = NULL;
          
          //NULL the newest nodes.
          for (int cou_Index=*Node_Count[p_Tier];cou_Index<*Node_Count_MAX[p_Tier];cou_Index++)
          {
               Nodes[p_Tier] [cou_Index] = NULL;
          }
          
          //Increment the counter.
          *Node_Count[p_Tier] = p_Index;
          
     }
     
     //Increases the number of nodes on a given tier.
     void increment_Number_Of_Nodes_On_Tier(int p_Tier)
     {
          //If the tier has no nodes then reallocate the tier withouth the tedious copying.
          if (*Node_Count[p_Tier] == 0)
          {
               //Delete the current tier and allocate it.
               delete [] Nodes[p_Tier];
               Nodes[p_Tier] = NULL;
               
               Nodes[p_Tier] = new c_MT_Base_Node_1D * [1];
               Nodes[p_Tier] [0] = NULL;
               
               *Node_Count[p_Tier] += 1;
               return;
          }
          
          //Setup the temporay array to hold the nodes whilest it is reallocated.
          c_MT_Base_Node_1D ** tmp_Tier;
          tmp_Tier = new c_MT_Base_Node_1D * [*Node_Count[p_Tier]];
          
          //Copy the nodes into the temporary array.
          for (int cou_NID=0;cou_NID<*Node_Count[p_Tier];cou_NID++)
          {
               tmp_Tier[cou_NID] = Nodes[p_Tier] [cou_NID];
          }
          
          //Delete the original and reallocate it to the current size plus one.
          delete [] Nodes[p_Tier];
          Nodes[p_Tier] = NULL;
          
          Nodes[p_Tier] = new c_MT_Base_Node_1D * [*Node_Count[p_Tier] + 1];
          
          //Copy the old nodes back into the tier.
          for (int cou_NID=0;cou_NID<*Node_Count[p_Tier];cou_NID++)
          {
               Nodes[p_Tier] [cou_NID] = tmp_Tier[cou_NID];
          }
          
          //Cleanup the temporary array.
          delete [] tmp_Tier;
          tmp_Tier = NULL;
          
          //NULL the newest node.
          Nodes[p_Tier] [*Node_Count[p_Tier]] = NULL;
          
          //Increment the counter.
          *Node_Count[p_Tier] += 1;
          
     }
     
     //Increases the number of nodes in the tmp_State_Node_Coordinates array.
     void set_State_Coordinates_Depth(int p_Depth)
     {
          delete tmp_State_Coordinates[0];
          tmp_State_Coordinates[0] = NULL;
          
          delete tmp_State_Coordinates[1];
          tmp_State_Coordinates[1] = NULL;
          
          delete tmp_State_Coordinates[2];
          tmp_State_Coordinates[2] = NULL;
          
          tmp_State_Coordinates[0] = new int[p_Depth];
          tmp_State_Coordinates[1] = new int[p_Depth];
          tmp_State_Coordinates[2] = new int[p_Depth];
          
          tmp_State_Coordinates_Depth = p_Depth;
     }
     
     //Sets the size of the tmp_State_Coordinates array to the size of the 0 tier which holds the state nodes.
     void inflate_State_Coordinates()
     {
          if (Node_Count[0] == NULL){ return; }
          
          set_State_Coordinates_Depth(*Node_Count[0]);
     }
     
     //--     BACKPROPAGATION FUNCTIONS
     // "cout << " will be replaced with an push to the output table when completed.
     
     //Initiates a backpropagation in the node network for debug output.
     void initiate_Backpropagation_Output_Only(c_MT_Base_Node_1D * &p_Node)
     {
          //For when directly output the dendrites of a nodes on the base tier.
          if (p_Node == NULL){ return; }
          cout << "~<";
          initiate_Backpropagation_LEFT_Output_Only(p_Node);
          cout << ">~";
     }
     
     //Initiates a backpropagation in the node network.
     void initiate_Backpropagation(c_MT_Base_Node_1D * &p_Node)
     {
          //For when directly output the dendrites of a nodes on the base tier.
          if (p_Node == NULL){ return; }
          
          //cout << "\n Charge->" << p_Charge << " State->";
          initiate_Backpropagation_LEFT(p_Node);
     }
     
     //Initiates a backpropagation LEFT.
     void initiate_Backpropagation_LEFT(c_MT_Base_Node_1D * &p_Node)
     {
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (p_Node->Dendrites[0] != NULL)
          {
               initiate_Backpropagation_LEFT(p_Node->Dendrites[0]);
               initiate_Backpropagation_RIGHT(p_Node->Dendrites[1]);
          }
          else
          {
               Output_Table_Pattern->push_Data_Bit_Into_Current_Cell_On_Current_Row(p_Node->get_State());
               //cout << char ((p_Node->get_State()).I);
          }
          
     }
     
     //Initiates a backpropagation RIGHT
     void initiate_Backpropagation_RIGHT(c_MT_Base_Node_1D * &p_Node)
     {
          //If a right leg exists then initiate a backpropagation.
          if (p_Node->Dendrites[1] != NULL)
          {
               initiate_Backpropagation_RIGHT(p_Node->Dendrites[1]);
          }
          else
          {
               Output_Table_Pattern->push_Data_Bit_Into_Current_Cell_On_Current_Row(p_Node->get_State());
               //cout << char ((p_Node->get_State()).I);
          }
     }
     
     //Initiates a backpropagation LEFT.
     void initiate_Backpropagation_LEFT_Output_Only(c_MT_Base_Node_1D * &p_Node)
     {
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (p_Node->Dendrites[0] != NULL)
          {
               initiate_Backpropagation_LEFT_Output_Only(p_Node->Dendrites[0]);
               initiate_Backpropagation_RIGHT_Output_Only(p_Node->Dendrites[1]);
          }
          else
          {
               //cout << char ((p_Node->get_State()).I);
               ochr(0, (p_Node->get_Reinforcement_Counter() + p_Node->get_Reinforcement_Counter_Level()), char ((p_Node->get_State()).I));
               //cout << p_Node->get_State().NR << " ";
          }
          
     }
     
     //Initiates a backpropagation RIGHT
     void initiate_Backpropagation_RIGHT_Output_Only(c_MT_Base_Node_1D * &p_Node)
     {
          //If a right leg exists then initiate a backpropagation.
          if (p_Node->Dendrites[1] != NULL)
          {
               initiate_Backpropagation_RIGHT_Output_Only(p_Node->Dendrites[1]);
          }
          else
          {
               //cout << char ((p_Node->get_State()).I);
               ochr(0, (p_Node->get_Reinforcement_Counter() + p_Node->get_Reinforcement_Counter_Level()), char ((p_Node->get_State()).I));
               //cout << p_Node->get_State().NR << " ";
          }
     }
     
     
     
     //Initiates a backpropagation in the node network.
     void initiate_Backpropagation_Gather_Data(c_MT_Base_Node_1D * &p_Node, c_Cell_1D * p_Cell)
     {
          //Reset the given cell.
          p_Cell->reset_Cell();
          
          //For when directly output the dendrites of a nodes on the base tier.
          if (p_Node == NULL){ return; }
          
          initiate_Backpropagation_LEFT_Gather_Data(p_Node, p_Cell);
     }
     
     //Initiates a backpropagation LEFT.
     void initiate_Backpropagation_LEFT_Gather_Data(c_MT_Base_Node_1D * &p_Node, c_Cell_1D * p_Cell)
     {
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (p_Node->Dendrites[0] != NULL)
          {
               initiate_Backpropagation_LEFT_Gather_Data(p_Node->Dendrites[0], p_Cell);
               initiate_Backpropagation_RIGHT_Gather_Data(p_Node->Dendrites[1], p_Cell);
          }
          else
          {
               p_Cell->push(p_Node->get_State(), 0);
          }
          
     }
     
     //Initiates a backpropagation RIGHT
     void initiate_Backpropagation_RIGHT_Gather_Data(c_MT_Base_Node_1D * &p_Node, c_Cell_1D * p_Cell)
     {
          //If a right leg exists then initiate a backpropagation.
          if (p_Node->Dendrites[1] != NULL)
          {
               initiate_Backpropagation_RIGHT_Gather_Data(p_Node->Dendrites[1], p_Cell);
          }
          else
          {
               p_Cell->push(p_Node->get_State(), 0);
          }
     }
     
     
     //Initiates a backpropagation in the node network.
     void initiate_Backpropagation_From_Given_u_Data_For_Given_Row(u_Data p_Data_Node, int p_Row)
     {
          //Gather the address of the current treetop node.
          c_MT_Base_Node_1D * tmp_Node = get_Treetop_Node(p_Data_Node);
          
          
          //For when directly output the dendrites of a nodes on the base tier.
          if (tmp_Node == NULL){ return; }
          
          initiate_Backpropagation_For_Given_Row(tmp_Node, p_Row);
     }
          
     //Initiates a backpropagation in the node network for the given output row.
     void initiate_Backpropagation_For_Given_Row(c_MT_Base_Node_1D * &p_Node, int p_Row)
     {
          //For when directly output the dendrites of a nodes on the base tier.
          if (p_Node == NULL){ return; }
          
          cout << "\n  State->";
          initiate_Backpropagation_LEFT_For_Given_Row(p_Node, p_Row);
          cout << "  " << Output_Table_Pattern->get_string(p_Row, Output_Table_Pattern->get_Current_Cell_In_Given_Row(p_Row));
     }
     
     //Initiates a backpropagation LEFT.
     void initiate_Backpropagation_LEFT_For_Given_Row(c_MT_Base_Node_1D * &p_Node, int p_Row)
     {
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (p_Node->Dendrites[0] != NULL)
          {
               initiate_Backpropagation_LEFT_For_Given_Row(p_Node->Dendrites[0], p_Row);
               initiate_Backpropagation_RIGHT_For_Given_Row(p_Node->Dendrites[1], p_Row);
          }
          else
          {
               Output_Table_Pattern->push_Data_Bit_Into_Current_Cell_On_Given_Row(p_Row, p_Node->get_State());
               cout << char ((p_Node->get_State()).I);
          }
          
     }
     
     //Initiates a backpropagation RIGHT
     void initiate_Backpropagation_RIGHT_For_Given_Row(c_MT_Base_Node_1D * &p_Node, int p_Row)
     {
          //If a right leg exists then initiate a backpropagation.
          if (p_Node->Dendrites[1] != NULL)
          {
               initiate_Backpropagation_RIGHT_For_Given_Row(p_Node->Dendrites[1], p_Row);
          }
          else
          {
               Output_Table_Pattern->push_Data_Bit_Into_Current_Cell_On_Given_Row(p_Row, p_Node->get_State());
               cout << char ((p_Node->get_State()).I);
          }
     }
     
     //Gets the RCA from a given tier.
     float get_RCA(int p_Tier = -1)
     {
          //cout << "\n\n\t get_RCA->" << p_Tier;
          
          if (p_Tier == -1){ p_Tier = Number_Of_Tiers - 1; }
          if (p_Tier >= Number_Of_Tiers || p_Tier < 0){ return 0.0; }
          if (*Node_Count[p_Tier] == 0){ return 0.0; }
          
          //The float to use.
          float tmp_RCA = 0.0;
          
          //Loop through all the nodes collecting their RC and then averaging it.
          for (int cou_Index=0;cou_Index<*Node_Count[p_Tier];cou_Index++)
          {
               //tmp_RCA += RC_LVL + Percentage towards next level.
               //cout << "\n\t Nodes[" << p_Tier << "] [" << cou_Index << "]->get_RC_Score() = " << Nodes[p_Tier] [cou_Index]->get_RC_Score();
               tmp_RCA += Nodes[p_Tier] [cou_Index]->get_RC_Score();
          }
          
          //cout << "\n\n\t\t tmp_RCA->" << tmp_RCA;
          
          tmp_RCA = tmp_RCA / *Node_Count[p_Tier];
          
          //cout << "\n\t\t tmp_RCA->" << tmp_RCA;
          
          return tmp_RCA;
     }
     
     //Get the node RC for a given node.
     int get_RC_Score(c_MT_Base_Node_1D * &p_Node)
     {
          if (p_Node == NULL){ return 0.0; }
          
          return p_Node->get_RC_Score();
     }     
     //Get the node RC for a u_Data treetop.
     int get_RC_Score(u_Data p_Data)
     {
          //Gather the address of the current treetop node.
          c_MT_Base_Node_1D * tmp_Node = get_Treetop_Node(p_Data);
          
          return get_RC_Score(tmp_Node);
     }
     
     //Get the node RC for a given node.
     int get_RC(c_MT_Base_Node_1D * &p_Node)
     {
          if (p_Node == NULL){ return 0.0; }
          
          return p_Node->get_Reinforcement_Counter();
     }     
     //Get the node RC for a u_Data treetop.
     int get_RC(u_Data p_Data)
     {
          //Gather the address of the current treetop node.
          c_MT_Base_Node_1D * tmp_Node = get_Treetop_Node(p_Data);
          
          return get_RC(tmp_Node);
     }
     
     //Get the node RC for a given node.
     int get_RC_LVL(c_MT_Base_Node_1D * &p_Node)
     {
          if (p_Node == NULL){ return 0.0; }
          
          return p_Node->get_Reinforcement_Counter_Level();
     }     
     //Get the node RC for a u_Data treetop.
     int get_RC_LVL(u_Data p_Data)
     {
          //Gather the address of the current treetop node.
          c_MT_Base_Node_1D * tmp_Node = get_Treetop_Node(p_Data);
          
          return get_RC_LVL(tmp_Node);
     }
     
     //Gets the number of nodes in the highest tier.
     int get_Node_Count()
     {
          if (Number_Of_Tiers < 1){ return *Node_Count[0]; }
          return *Node_Count[Number_Of_Tiers - 1];
     }
          
     //Gets the number of nodes in the highest tier.
     int get_Node_Count_For_Tier(int p_Tier)
     {
          return *Node_Count[p_Tier];
     }
     
     //Gets the total number of nodes in the network.
     int get_Node_Count_Total()
     {
          int tmp_Node_Count = 0;
          for (int cou_Index=0;cou_Index<Number_Of_Tiers;cou_Index++)
          {
               tmp_Node_Count += *Node_Count[cou_Index];
          }
          
          return tmp_Node_Count;
     }
     
     //Gets the nodes Index with the given tier. If the node is not found -1 is returned.
     int get_Node_Index(c_MT_Base_Node_1D * p_Node, int p_Tier)
     {
          if (p_Tier >= Number_Of_Tiers || p_Tier < 0){ return -1; }
          
          //cout << "\n\t p_Node->" << p_Node << " p_Tier->" << p_Tier;
          
          for (int cou_Index=0;cou_Index<*Node_Count[p_Tier];cou_Index++)
          {
               //cout << "..." << cou_Index;
               if (Nodes[p_Tier] [cou_Index] == p_Node){ return cou_Index; }
          }
          
          return -1;
     }
     
     //Sets the construct input type.
     int set_Input_Type(int p_Type)
     {
          State_Type = p_Type;
          return State_Type;
     }
     
     //Gets the input type of the construct.
     int get_Input_Type()
     {
          return State_Type;
     }
     
     //--     OUTPUT FUNCTIONS
     
     
     //Output a given node.
     void output_Node(int p_Tier, int p_NID)
     {
          if (Nodes[p_Tier] [p_NID] == NULL){ cout << " NULL_NODE"; return; }
          
          //initiate_Backpropagation_Output_Only(Nodes[p_Tier] [p_NID]);
          //cout << " -" << Nodes[p_Tier] [p_NID];
          Nodes[p_Tier] [p_NID]->output_Node_Header();
          
          cout << " S->";
          initiate_Backpropagation_Output_Only(Nodes[p_Tier] [p_NID]);
          
          cout << " ~D";
          cout << " &";
          initiate_Backpropagation_Output_Only(Nodes[p_Tier] [p_NID]->Dendrites[0]);
          
          cout << " &";
          initiate_Backpropagation_Output_Only(Nodes[p_Tier] [p_NID]->Dendrites[1]);
          
          cout << " ~A[0]";
               
          for (int cou_A=0;cou_A<Nodes[p_Tier] [p_NID]->get_Axon_Count(0);cou_A++)
          {
               cout << " &";
               initiate_Backpropagation_Output_Only(Nodes[p_Tier] [p_NID]->Axons[0] [cou_A]);
          }
               
          cout << " ~A[1]";
          
          for (int cou_A=0;cou_A<Nodes[p_Tier] [p_NID]->get_Axon_Count(1);cou_A++)
          {
               cout << " &";
               initiate_Backpropagation_Output_Only(Nodes[p_Tier] [p_NID]->Axons[1] [cou_A]);
          }
     }
     
     //Output the top tier of the network.
     void output_Top_Tier()
     {
          for (int cou_Index=0;cou_Index<*Node_Count[Number_Of_Tiers - 1];cou_Index++)
          {
               cout << "\n\t   [" << cou_Index << "]";
               output_Node((Number_Of_Tiers - 1), cou_Index);
          }
     }
     
     //Output a tier of the network.
     void output_Tier(int p_Tier)
     {
          for (int cou_Index=0;cou_Index<*Node_Count[p_Tier];cou_Index++)
          {
               cout << "\n\t   [" << cou_Index << "]";
               output_Node(p_Tier, cou_Index);
          }
     }
     
     //Outputs all of the tiers in the network.
     void output_All_Tiers()
     {
          for (int cou_Tier=0;cou_Tier<Number_Of_Tiers;cou_Tier++)
          {
               cout << "\n\n\t -Tier:";
               oint(0, 13, cou_Tier);
               output_Tier(cou_Tier);
          }
     }
          
     //Outputs the number of nodes in each tier in the network.
     void output_Number_Of_Nodes()
     {
          for (int cou_Tier=0;cou_Tier<Number_Of_Tiers;cou_Tier++)
          {
               cout << "\n\n\t -Tier:" << cou_Tier << "->" << *Node_Count[cou_Tier];
          }
     }
     
     //Outputs the information pertaining to the network.
     void output_Node_Network()
     {
          cout << "\n\n\t----------";
          cout << "\n\t Node_Network->" << this << " ";
          ostr(0, 15, Name);
          cout << "\n";
          
          output_State_Tree();
          
          cout << "\n\t State_Type->";
          oint(0, 14, State_Type);
          
          output_State_Coordinates();
          cout << "\n\n\t Number_Of_Tiers->";
          oint(0, 14, Number_Of_Tiers);
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n\t\t ~" << cou_T << ":" << *Node_Count[cou_T] << " ~~";
               for (int cou_Index=0;cou_Index<*Node_Count[cou_T];cou_Index++)
               {
                    //ochr(0, (cou_Index + 1), 'X');
                    ochr(0, (Nodes[cou_T] [cou_Index]->get_Reinforcement_Counter() + Nodes[cou_T] [cou_Index]->get_Reinforcement_Counter_Level()), 'X');
               }
          }
          cout << "\n";
          output_All_Tiers();
     }
     
     //Outputs the information pertaining to the network.
     void output_Node_Network_Stats()
     {
          if (Name != "P_Nodes"){ return; }
          
          cout << "\n\n\t----------";
          cout << "\n\t Node_Network->" << this << " ";
          ostr(0, 15, Name);
          cout << "\n";
          
          output_State_Tree();
          
          cout << "\n\t State_Type->";
          oint(0, 14, State_Type);
          
          //output_State_Coordinates();
          cout << "\n\n\t Number_Of_Tiers->";
          oint(0, 14, Number_Of_Tiers);
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n\t\t ~" << cou_T << ":" << *Node_Count[cou_T] << " ~~";
          }
          cout << "\n";
          //output_All_Tiers();
     }
     
     //Outputs the state coordinates array.
     void output_State_Coordinates()
     {
          cout << "\n\t tmp_State_Coordinates Current_Depth->";
          oint(0, 15, tmp_State_Coordinates_Depth);
          for (int cou_Index=0;cou_Index<tmp_State_Coordinates_Depth;cou_Index++)
          {
               cout << "\n [" << cou_Index << "]->(";
               oint(0, 11, tmp_State_Coordinates[0] [cou_Index]);
               cout << ", ";
               oint(0, 12, tmp_State_Coordinates[1] [cou_Index]);
               cout << ", ";
               oint(0, 13, tmp_State_Coordinates[2] [cou_Index]);
               cout << ")";
          }
     }
     
     //Outputs the state tree.
     void output_State_Tree()
     {
          cout << "\n\t   ->State Tree:";
          State_Tree.output_Tree();
     }
     
     //==--     SAVING AND LOADING FUNCTIONS.
     

     
     //Saves the node network.
     void Save(string p_Directory = "Default", string p_Assembly_Name = "")
     {
          //Gathers the directory and creates it if need be
          //string tmp_MD = "md " + p_Directory;
          
          //cout << "\n\n\t->" << tmp_MD;
          //system(tmp_MD.c_str());
          
          //Gathers the file to save the node network to.
          ofstream SF;
          string tmp_FN = p_Directory + "\\" + p_Assembly_Name + "__" + Name + ".nnt";
          SF.open(tmp_FN.c_str());
          
          //Save the settings to the file.
          SF << Name << "\n";
          cout << " Name->" << Name;
          SF << Number_Of_Tiers << "\n";
          cout << " Number_Of_Tiers->" << Number_Of_Tiers;
          SF << CID << "\n";
          cout << " CID->" << CID << "\n";
          SF << State_Type << "\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               SF << "\n" << *Node_Count[cou_T];
          }
          
          //For every node the stats are saved, and the axons and dendrites are stored as coordinates to the linked node.
          
          cout << "\n\n\t";
          for (int cou_Index=0;cou_Index<Number_Of_Tiers;cou_Index++)
          {
               cout << "+";
          }
          cout << "\n\t";
          
          int tmp_Output_Tier = 0;
          
          ochr(0, 0, '^');
          
          //cout << "\n\n\t [" << 0 << "]";
          for (int cou_Index=0;cou_Index<*Node_Count[0];cou_Index++)
          {
               //cout << "\n\t  [" << 0 << "] [" << cou_Index << "]";
               //cout << "  Type->" << Nodes[0] [cou_Index]->get_Type();
               //if (State_Type == 0){ cout << "  T->SN"; }
               //if (State_Type == 1){ cout << "  T->SNR"; }
               //cout << "  ST->" << (Nodes[0] [cou_Index]->get_State()).I;
               //cout << "  RC->" << get_RC(Nodes[0] [cou_Index]);
               //cout << "  RC_LVL->" << get_RC_LVL(Nodes[0] [cou_Index]);
               //cout << "  Tt->" << (Nodes[0] [cou_Index]->get_Treetop()).I;
               
               SF << "\n" << 0 << " " << cou_Index << " ";
               SF << " " << Nodes[0] [cou_Index]->get_Type();
               if (State_Type == 0)
               { 
                    SF << " SN"; 
                    SF << " " << (Nodes[0] [cou_Index]->get_State()).I;
               }
               
               if (State_Type == 1)
               { 
                    SF << " SNR"; 
                    SF << " " << tmp_State_Coordinates[0] [cou_Index];
                    SF << " " << tmp_State_Coordinates[1] [cou_Index];
                    SF << " " << tmp_State_Coordinates[2] [cou_Index];
               }
               SF << " " << get_RC(Nodes[0] [cou_Index]);
               SF << " " << get_RC_LVL(Nodes[0] [cou_Index]);
               SF << " " << (Nodes[0] [cou_Index]->get_Treetop()).I;
               
               /*
               //cout << "  AC[0]->" << Nodes[0] [cou_Index]->Axon_Count[0];
               SF << " " << Nodes[0] [cou_Index]->Axon_Count[0];
               for (int cou_A=0;cou_A<Nodes[0] [cou_Index]->Axon_Count[0];cou_A++)
               {
                    //cout << "  A[" << cou_A << "]->" << get_Node_Index(Nodes[0] [cou_Index]->Axons[0] [cou_A], 1);
                    SF << " " << get_Node_Index(Nodes[0] [cou_Index]->Axons[0] [cou_A], 1);
               }
               
               //cout << "  AC[1]->" << Nodes[0] [cou_Index]->Axon_Count[1];
               SF << " " << Nodes[0] [cou_Index]->Axon_Count[1];
               for (int cou_A=0;cou_A<Nodes[0] [cou_Index]->Axon_Count[1];cou_A++)
               {
                    //cout << "  A[" << cou_A << "]->" << get_Node_Index(Nodes[0] [cou_Index]->Axons[1] [cou_A], 1);
                    SF << " " << get_Node_Index(Nodes[0] [cou_Index]->Axons[1] [cou_A], 1);
               }
               */
          }
               
          for (int cou_Tier=1;cou_Tier<Number_Of_Tiers;cou_Tier++)
          {
               //cout << "\n\t [" << cou_Tier << "]";
               
               if (tmp_Output_Tier < cou_Tier){ tmp_Output_Tier = cou_Tier; ochr(0, cou_Tier, '^'); }
               
               for (int cou_Index=0;cou_Index<*Node_Count[cou_Tier];cou_Index++)
               {
                    //cout << "\n\t  [" << cou_Tier << "] [" << cou_Index << "]";
                    //cout << "  Type->" << Nodes[cou_Tier] [cou_Index]->get_Type();
                    //cout << "  T->M";
                    //cout << "  RC->" << get_RC(Nodes[cou_Tier] [cou_Index]);
                    //cout << "  RC_LVL->" << get_RC_LVL(Nodes[cou_Tier] [cou_Index]);
                    //cout << "  Tt->" << (Nodes[cou_Tier] [cou_Index]->get_Treetop()).I;
                    
                    //cout << "  D[0]->" << get_Node_Index(Nodes[cou_Tier] [cou_Index]->Dendrites[0], (cou_Tier - 1));
                    //cout << "  D[1]->" << get_Node_Index(Nodes[cou_Tier] [cou_Index]->Dendrites[1], (cou_Tier - 1));
                    
                    SF << "\n" << cou_Tier << " " << cou_Index << " ";
                    SF << " " << Nodes[cou_Tier] [cou_Index]->get_Type();
                    SF << " M";
                    SF << " " << Nodes[cou_Tier] [cou_Index]->get_Reinforcement_Counter();
                    SF << " " << Nodes[cou_Tier] [cou_Index]->get_Reinforcement_Counter_Level();
                    
                    SF << " " << (Nodes[cou_Tier] [cou_Index]->get_Treetop()).I;
                    
                    SF << " " << get_Node_Index(Nodes[cou_Tier] [cou_Index]->Dendrites[0], (cou_Tier - 1));
                    SF << " " << get_Node_Index(Nodes[cou_Tier] [cou_Index]->Dendrites[1], (cou_Tier - 1));
                    /*
                    SF << " " << Nodes[cou_Tier] [cou_Index]->Axon_Count[0];
                    for (int cou_A=0;cou_A<Nodes[cou_Tier] [cou_Index]->Axon_Count[0];cou_A++)
                    {
                         //cout << "  A[" << cou_A << "]->" << get_Node_Index(Nodes[cou_Tier] [cou_Index]->Axons[0] [cou_A], (cou_Tier + 1));
                         //SF << " " << get_Node_Index(Nodes[cou_Tier] [cou_Index]->Axons[0] [cou_A], (cou_Tier + 1));
                         SF << " " << (Nodes[cou_Tier] [cou_Index]->Axons[0] [cou_A])->get_Index();
                    }
                    
                    SF << " " << Nodes[cou_Tier] [cou_Index]->Axon_Count[1];
                    for (int cou_A=0;cou_A<Nodes[cou_Tier] [cou_Index]->Axon_Count[1];cou_A++)
                    {
                         //cout << "  A[" << cou_A << "]->" << get_Node_Index(Nodes[cou_Tier] [cou_Index]->Axons[1] [cou_A], (cou_Tier + 1));
                         //SF << " " << get_Node_Index(Nodes[cou_Tier] [cou_Index]->Axons[1] [cou_A], (cou_Tier + 1));
                         SF << " " << (Nodes[cou_Tier] [cou_Index]->Axons[1] [cou_A])->get_Index();
                    }
                    */
               }
          }
          
          SF.close();
     }
     
     //Loads the node network.
     void load(string p_Directory, string p_File)
     {    
          //The temp vars.
          string tmp_String;
          int tmp_Int;
          c_Scaffold tmp_Scaffold;
          int tmp_Tier;
          int tmp_Index;
          //int tmp_Axon_Count[2];
          //int tmp_Axon;
          string tmp_Type;
          string tmp_T;
          u_Data tmp_State;
          int tmp_State_CID;
          int tmp_State_Tier;
          int tmp_State_Index;
          int tmp_RC;
          int tmp_RC_Lvl;
          u_Data tmp_Treetop;
          
          tmp_Scaffold.reset_Scaffold();
          
          //Gathers the file to save the node network to.
          ifstream SF;
          string tmp_FN = p_Directory + "\\" + p_File + ".nnt";
          cout << "\n\t Opening File->" << tmp_FN;
          SF.open(tmp_FN.c_str());
          
          if (!SF.is_open()){ cout << "\n\n\t FILE " << tmp_FN << " NOT OPENED!>!>!>!"; return; }
          
          SF >> tmp_String;
          
          cout << "\n\t Setting_Name(" << tmp_String << ")";
          set_Name(tmp_String);
          
          //Load the settings from the file.
          SF >> tmp_Int;
          cout << "\n\t Increasing Number Of Tiers To->" << tmp_Int;
          increase_Number_Of_Tiers(tmp_Int);
          tmp_Scaffold.increase_Number_Of_Tiers(tmp_Int);
          
          SF >> CID;
          SF >> State_Type;
          cout << "\n\t CID->" << CID << " State_Type->" << State_Type;
          
          cout << "\n\t Gathering Node Counts...";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               SF >> tmp_Int;
               cout << " [" << cou_T << "].." << tmp_Int;
               increase_Number_Of_Nodes_On_Tier_Loading(cou_T, tmp_Int);
               tmp_Scaffold.increase_Number_Of_Nodes_On_Tier(cou_T, tmp_Int);
          }
          
          //Expand the State node references to hold the state nodes if the state node type is 1.
          if (State_Type == 1)
          {
               cout << "\n\t Expanding State Coordinates Depth To ->";
               oint(0, 9, *Node_Count[0]);
               set_State_Coordinates_Depth(*Node_Count[0]);
          }
          
          
          
          //Now to gather all of the remaining nodes.
          cout << "\n\t Gathering_Main_Memory_Nodes...";
          
          cout << "\n\n\t\t   Tiers:";
          
		int tmp_tier_X = get_Console_Cursor_X();
		int tmp_tier_Y = get_Console_Cursor_Y();
		
          cout << "\n\n\t\t   Current Tier:";
          
		int tmp_node_X = get_Console_Cursor_X();
		int tmp_node_Y = get_Console_Cursor_Y();
		
          
          
          
          int tmp_Output_Tier = 0;
          
          int tmp_Output_Node = 1;
          
          while (!SF.eof())
          {
               //Get the coordinates.
               SF >> tmp_Tier;
               SF >> tmp_Index;
               
               if (tmp_Output_Tier < tmp_Tier)
			{ 
				tmp_Output_Tier = tmp_Tier;
				console_Move_Cursor_To_Given_X_And_Y(tmp_tier_X, tmp_tier_Y);
				cout << "(" << tmp_Tier << " / " << (Number_Of_Tiers - 1) << ") ";
				cout << (float (tmp_Tier) / float (Number_Of_Tiers)) * 100 << "%\t";
				tmp_Output_Node = (int (*Node_Count[tmp_Tier] / 100)) + 1;
			}
			
               
               if (!(tmp_Index % tmp_Output_Node))
			{ 
				console_Move_Cursor_To_Given_X_And_Y(tmp_node_X, tmp_node_Y);
				cout << (float (tmp_Index) / float (*Node_Count[tmp_Tier])) * 100 << "%\t\t";
			}
               
               //cout << "\n\n\t tmp_Tier->" << tmp_Tier << " tmp_Index->" << tmp_Index; //cout.flush();
               
               //Get the type of node.
               SF >> tmp_Type;
               
               //cout << "\n\t tmp_Type->" << tmp_Type;
               
               //cout << "\n\t tmp_Type->" << tmp_Type;
               if (tmp_Type == "State_Node"){ Nodes[tmp_Tier] [tmp_Index] = new_State_Node(tmp_Index, tmp_State); }
               if (tmp_Type == "RC_State_Node"){ Nodes[tmp_Tier] [tmp_Index] = new_RC_State_Node(tmp_Index, tmp_State); }
               if (tmp_Type == "Treetop_State_Node"){ Nodes[tmp_Tier] [tmp_Index] = new_Treetop_State_Node(tmp_Tier, tmp_Index, tmp_State); }
               if (tmp_Type == "Treetop_RC_State_Node"){ Nodes[tmp_Tier] [tmp_Index] = new_Treetop_RC_State_Node(tmp_Tier, tmp_Index, tmp_State); }
               
               if (tmp_Type == "Simple_Node"){ Nodes[tmp_Tier] [tmp_Index] = new_Node(tmp_Tier, tmp_Index); }
               if (tmp_Type == "RC_Node"){ Nodes[tmp_Tier] [tmp_Index] = new_RC_Node(tmp_Tier, tmp_Index); }
               if (tmp_Type == "Treetop_Simple_Node"){ Nodes[tmp_Tier] [tmp_Index] = new_Treetop_Node(tmp_Tier, tmp_Index); }
               if (tmp_Type == "Treetop_RC_Node"){ Nodes[tmp_Tier] [tmp_Index] = new_Treetop_RC_Node(tmp_Tier, tmp_Index); }
               
               //cout << "\n\n\t Nodes[";
               //oint(0, (tmp_Tier + 1), tmp_Tier);
               //cout << "] [";
               //oint(0, (tmp_Index + 1), tmp_Index);
               //cout << "]->" << Nodes[tmp_Tier] [tmp_Index];
               
               SF >> tmp_T;
               
               //cout << "\n\t tmp_T->" << tmp_T; //cout.flush();
               
               //If it is a state node check for state or coordinates.
               if (tmp_T == "SN")
               {
                    SF >> tmp_State.I;
                    //cout << "\n\t" << tmp_String; //cout.flush();
                    
                    State_Tree.search(tmp_State); State_Tree.set_Current_Node_NID(Nodes[tmp_Tier] [tmp_Index]);
                    
                    //cout << " tmp_State->" << tmp_State.I; //cout.flush();
                    Nodes[tmp_Tier] [tmp_Index]->set_State(tmp_State);
               }
               if (tmp_T == "SNR")
               {
                    SF >> tmp_State_CID;
                    SF >> tmp_State_Tier;
                    SF >> tmp_State_Index;
                    tmp_State_Coordinates[0] [tmp_Index] = tmp_State_CID;
                    tmp_State_Coordinates[1] [tmp_Index] = tmp_State_Tier;
                    tmp_State_Coordinates[2] [tmp_Index] = tmp_State_Index;
                    //cout << " tmp_State_Tier->" << tmp_State_Tier << " tmp_State_Index->" << tmp_State_Index; //cout.flush();
                    //Nodes[tmp_Tier] [tmp_Index]->set_State(tmp_State);
               }
               
               //Gather the RC & RC_Lvl
               SF >> tmp_RC;
               SF >> tmp_RC_Lvl;
               
               //Set the RC and RC_Lvl
               Nodes[tmp_Tier] [tmp_Index]->set_Reinforcement_Counter(tmp_RC);
               Nodes[tmp_Tier] [tmp_Index]->set_Reinforcement_Counter_Level(tmp_RC_Lvl);
               
               //cout << "\n\t tmp_RC->"; //cout.flush();
               //oint(0, 9, tmp_RC);
               
               //cout << "\n\t tmp_RC_LVL->"; //cout.flush();
               //oint(0, 9, tmp_RC_Lvl);
               
               SF >> tmp_Treetop.I;
               Nodes[tmp_Tier] [tmp_Index]->set_Treetop(tmp_Treetop);
               
               //cout << "\n\t tmp_RC->" << tmp_RC << " tmp_RC_Lvl->" << tmp_RC_Lvl; //cout.flush();
               
               //Gather the dendrites. Two procedures as state nodes will be different.
               if (tmp_Tier > 0)
               {
                    //cout << "\n\t Gathering Dendrites..."; //cout.flush();
                    
                    tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->Dendrites[0] [0] = tmp_Tier - 1;
                    SF >> tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->Dendrites[0] [1];
                    
                    //cout << " [0]->" << tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->Dendrites[0] [0]; //cout.flush();
                    //cout << ", " << tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->Dendrites[0] [1]; //cout.flush();
                    
                    tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->Dendrites[1] [0] = tmp_Tier - 1;
                    SF >> tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->Dendrites[1] [1];
                    
                    //cout << " [1]->" << tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->Dendrites[1] [0]; //cout.flush();
                    //cout << ", " << tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->Dendrites[1] [1]; //cout.flush();
               }
               else
               {
                    tmp_Scaffold.Nodes[0] [tmp_Index]->Dendrites[0] [0] = 0;
                    tmp_Scaffold.Nodes[0] [tmp_Index]->Dendrites[0] [1] = 0;
                    
                    tmp_Scaffold.Nodes[0] [tmp_Index]->Dendrites[1] [0] = 0;
                    tmp_Scaffold.Nodes[0] [tmp_Index]->Dendrites[1] [1] = 0;
               }
               
               /*
               //Gather Axons by first getting the count and then moving to the individual axons themselves.
               //The scaffold will be used here to store the axons until the nodes themselves are all created.
               SF >> tmp_Axon_Count[0];
               
               //cout << "\n\t tmp_Axon_Count[0]->" << tmp_Axon_Count[0]; //cout.flush();
               
               tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->increase_Axons(0, tmp_Axon_Count[0]);
               
               for (int cou_Index=0;cou_Index<tmp_Axon_Count[0];cou_Index++)
               {
                    SF >> tmp_Axon;
                    
                    //cout << "\n\t  [" << cou_Index << "] " << tmp_Axon; //cout.flush();
                    
                    tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->increase_Axons(0, tmp_Axon_Count[0]);
                    
                    tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->Axons[0] [0] [cou_Index] = (tmp_Tier + 1);
                    tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->Axons[0] [1] [cou_Index] = tmp_Axon;
               }
               
               SF >> tmp_Axon_Count[1];
               
               //cout << "\n\t tmp_Axon_Count[1]->" << tmp_Axon_Count[1]; //cout.flush();
               
               tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->increase_Axons(1, tmp_Axon_Count[1]);
               
               for (int cou_Index=0;cou_Index<tmp_Axon_Count[1];cou_Index++)
               {
                    SF >> tmp_Axon;
                    
                    //cout << "\n\t  [" << cou_Index << "] " << tmp_Axon; //cout.flush();
                    
                    tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->increase_Axons(1, tmp_Axon_Count[1]);
                    
                    tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->Axons[1] [0] [cou_Index] = (tmp_Tier + 1);
                    tmp_Scaffold.Nodes[tmp_Tier] [tmp_Index]->Axons[1] [1] [cou_Index] = tmp_Axon;
               }
               */
          }
          //tmp_Scaffold.output_Scaffold();
          
          int tmp_From[2];
          int tmp_To[2];
          
          cout << "\n\t Building the Scaffold..."; //cout.flush();
          cout << "\n\t  Building the State layer.."; //cout.flush();
          
          ///////////////////////////////////////////////////////
          //==--   SWITCHING TO DENDRITE ONLY BUILDING AND SAVING
          
          //Build the state nodes using the scaffolding as a guide.
          for (int cou_Index=0;cou_Index<*Node_Count[0];cou_Index++)
          {
               ////cout << "\n\t [" << cou_Index << "] "; //cout.flush();
               ////cout.flush();
               
               //Build the axonic connections.
               /*
               //cout << "\n\t  L->0";
               for (int cou_A=0;cou_A<tmp_Scaffold.Nodes[0] [cou_Index]->Axons_Count[0];cou_A++)
               {
                    //cout << "\n\t   <" << cou_A << ">"; //cout.flush();
                    //Get the coordinates.
                    tmp_From[0] = 0;
                    tmp_From[1] = cou_Index;
                    
                    tmp_To[0] = 1;
                    tmp_To[1] = tmp_Scaffold.Nodes[0] [cou_Index]->Axons[0] [1] [cou_A];
                    
                    //cout << "\n\t      create_Connection(0, " << Nodes[tmp_From[0]] [tmp_From[1]] << ", " << Nodes[tmp_To[0]] [tmp_To[1]] << ") F->" << tmp_From[0] << ", " << tmp_From[1] << " T->" << tmp_To[0] << ", " << tmp_To[1]; //cout.flush();
                    create_Connection(0, Nodes[tmp_From[0]] [tmp_From[1]], Nodes[tmp_To[0]] [tmp_To[1]]);
                    //create_Connection(int, c_MT_Base_Node_1D*, c_MT_Base_Node_1D*)
               }
               
               //cout << "\n\t  L->1"; //cout.flush();
               for (int cou_A=0;cou_A<tmp_Scaffold.Nodes[0] [cou_Index]->Axons_Count[1];cou_A++)
               {
                    //cout << "\n\t   <" << cou_A << ">"; //cout.flush();
                    //cout << "\n\t     gathered"; //cout.flush();
                    
                    //Get the coordinates.
                    tmp_From[0] = 0;
                    //cout << " tmp_From[0]->" << tmp_From[0]; //cout.flush();
                    
                    tmp_From[1] = cou_Index;
                    //cout << " tmp_From[1]->" << tmp_From[1]; //cout.flush();
                    
                    tmp_To[0] = 1;
                    //cout << " tmp_To[0]->" << tmp_To[0]; //cout.flush();
                    
                    tmp_To[1] = tmp_Scaffold.Nodes[0] [cou_Index]->Axons[1] [1] [cou_A];
                    //cout << " tmp_To[1]->" << tmp_To[1]; //cout.flush();
                    
                    //cout << " F(" << Nodes[tmp_From[0]] [tmp_From[1]] << "->" << tmp_From[0] << ", " << tmp_From[1] << ") T(" << Nodes[tmp_To[0]] [tmp_To[1]] << "->" << tmp_To[0] << ", " << tmp_To[1] <<")";
                    //cout << "\n\t      create_Connection(1, " << Nodes[tmp_From[0]] [tmp_From[1]] << ", " << Nodes[tmp_To[0]] [tmp_To[1]] << ") F->" << tmp_From[0] << ", " << tmp_From[1] << " T->" << tmp_To[0] << ", " << tmp_To[1];
                    create_Connection(1, Nodes[tmp_From[0]] [tmp_From[1]], Nodes[tmp_To[0]] [tmp_To[1]]);
                    //create_Connection(int, c_MT_Base_Node_1D*, c_MT_Base_Node_1D*)
               }
               
               */
          }
          cout << "\n\t   Building The (" << Number_Of_Tiers << ") Memory Tiers...";
          
          cout << "\n\n\t";
          for (int cou_Index=0;cou_Index<Number_Of_Tiers;cou_Index++)
          {
               cout << "+";
          }
          cout << "\n\t";
          
          //Build the connections between the nodes using the scaffolding.
          for (int cou_T=1;cou_T<Number_Of_Tiers;cou_T++)
          {
               ochr(0, cou_T, '^');
               //cout << "\n\t   [" << cou_T << "] ";
               
               for (int cou_Index=0;cou_Index<*Node_Count[cou_T];cou_Index++)
               {
                    //cout << "\n\t    [" << cou_Index << "]";
                                             
                    //Get the coordinates.
                    tmp_From[0] = cou_T - 1;
                    tmp_From[1] = tmp_Scaffold.Nodes[cou_T] [cou_Index]->Dendrites[0] [1];
                    
                    tmp_To[0] = cou_T;
                    tmp_To[1] = cou_Index;
                    
                    //cout << "\n\t      create_Connection(0, " << Nodes[tmp_From[0]] [tmp_From[1]] << ", " << Nodes[tmp_To[0]] [tmp_To[1]] << ") F->" << tmp_From[0] << ", " << tmp_From[1] << " T->" << tmp_To[0] << ", " << tmp_To[1];
                    create_Connection(0, Nodes[tmp_From[0]] [tmp_From[1]], Nodes[tmp_To[0]] [tmp_To[1]]);
                    
                                             
                    //Get the coordinates.
                    tmp_From[0] = cou_T - 1;
                    tmp_From[1] = tmp_Scaffold.Nodes[cou_T] [cou_Index]->Dendrites[1] [1];
                    
                    tmp_To[0] = cou_T;
                    tmp_To[1] = cou_Index;
                    
                    //cout << "\n\t      create_Connection(0, " << Nodes[tmp_From[0]] [tmp_From[1]] << ", " << Nodes[tmp_To[0]] [tmp_To[1]] << ") F->" << tmp_From[0] << ", " << tmp_From[1] << " T->" << tmp_To[0] << ", " << tmp_To[1];
                    create_Connection(1, Nodes[tmp_From[0]] [tmp_From[1]], Nodes[tmp_To[0]] [tmp_To[1]]);
                    //create_Connection(int, c_MT_Base_Node_1D*, c_MT_Base_Node_1D*)
                    
                    /*
                    //Build the connections.
                    for (int cou_A=0;cou_A<tmp_Scaffold.Nodes[cou_T] [cou_Index]->Axons_Count[0];cou_A++)
                    {
                         //Get the coordinates.
                         tmp_From[0] = cou_T;
                         tmp_From[1] = cou_Index;
                         
                         tmp_To[0] = (cou_T + 1);
                         tmp_To[1] = tmp_Scaffold.Nodes[cou_T] [cou_Index]->Axons[0] [1] [cou_A];
                         
                         //cout << "\n\t      create_Connection(0, " << Nodes[tmp_From[0]] [tmp_From[1]] << ", " << Nodes[tmp_To[0]] [tmp_To[1]] << ") F->" << tmp_From[0] << ", " << tmp_From[1] << " T->" << tmp_To[0] << ", " << tmp_To[1];
                         create_Connection(0, Nodes[tmp_From[0]] [tmp_From[1]], Nodes[tmp_To[0]] [tmp_To[1]]);
                         //create_Connection(int, c_MT_Base_Node_1D*, c_MT_Base_Node_1D*)
                    }
                    
                    for (int cou_A=0;cou_A<tmp_Scaffold.Nodes[cou_T] [cou_Index]->Axons_Count[1];cou_A++)
                    {
                         //Get the coordinates.
                         tmp_From[0] = cou_T;
                         tmp_From[1] = cou_Index;
                         
                         tmp_To[0] = (cou_T + 1);
                         tmp_To[1] = tmp_Scaffold.Nodes[cou_T] [cou_Index]->Axons[1] [1] [cou_A];
                         
                         //cout << "\n\t      create_Connection(0, " << Nodes[tmp_From[0]] [tmp_From[1]] << ", " << Nodes[tmp_To[0]] [tmp_To[1]] << ") F->" << tmp_From[0] << ", " << tmp_From[1] << " T->" << tmp_To[0] << ", " << tmp_To[1];
                         create_Connection(1, Nodes[tmp_From[0]] [tmp_From[1]], Nodes[tmp_To[0]] [tmp_To[1]]);
                         //create_Connection(int, c_MT_Base_Node_1D*, c_MT_Base_Node_1D*)
                    }*/
               }
          }
          
          /*
           * This is done after the constructs are loaded into memory at the gathering level.
          //Dangling pointers are very dangerous.
          delete tmp_State_Coordinates[0];
          tmp_State_Coordinates[0] = NULL;
          
          delete tmp_State_Coordinates[1];
          tmp_State_Coordinates[1] = NULL;
          
          delete tmp_State_Coordinates[2];
          tmp_State_Coordinates[2] = NULL;
          
          tmp_State_Coordinates_Depth = 0;
          */
          
          SF.close();
          //output_State_Coordinates();
     }
     
};
