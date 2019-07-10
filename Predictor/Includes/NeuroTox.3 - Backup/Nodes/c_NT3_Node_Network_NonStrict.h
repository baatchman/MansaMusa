

//The node network that handles the nodes.
class c_NT3_Node_Network_NonStrict
{
private:
     
public:
     
     //The one that started it all.
     c_NT3_Base_Node_NonStrict * Root;
     
     //The current node to attach to.
     c_NT3_Base_Node_NonStrict ** Current_Node;
     
     //The Fractional tree for handling state queries.
     c_NT3_Fractal_State_Tree_NonStrict State_Tree;
     
     //The tree to hold the Treetops for backpropagation.
     c_NT3_Fractal_State_Tree_NonStrict Treetops_Tree;
     
     //This tracks the current Nodes ID to be entered into the node tracking tree for saving and loading.
     u_Data_3 CNID;
     
     //Tree to track the nodes and their IDs.
     //Only used during saving and loading, then it is destroyed to save memory.
     c_NT3_Fractal_State_Tree_NonStrict NID_Tree;
     
     c_NT3_Node_Network_NonStrict()
     {
          CNID.I = 0;
          Root = NULL;
          Current_Node = &Root;
     }
     
     ~c_NT3_Node_Network_NonStrict()
     {
          cout << "\n ~c_NT3_Node_Network_NonStrict " << CNID.I << " " << this << ".........."; cout.flush();
          //delete Root;
          
          c_NT3_Base_Node_NonStrict * tmp_LL = Root;
          Root = NULL;
          c_NT3_Base_Node_NonStrict * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               tmp_LL->Next = NULL;
               delete tmp_LL;
               tmp_LL = NULL;
               tmp_LL = tmp_LL_Next;
          }
          
          delete Root;
          Root = NULL;
          
          cout << "\n ~~~c_NT3_Node_Network_NonStrict " << CNID.I << " " << this << ".........."; cout.flush();
     }
     
     //Resets the tree.
     void reset()
     {
          delete Root;
     }
     
     
     ////==------------------+
     //==--   NODE CREATION
     ////==------------------+
     
     //Creates a new state node.
     c_NT3_Base_Node_NonStrict * new_State_Node(u_Data_3 p_State, int p_D = 0, int p_A = 0)
     {
          //Create the state node.
          *Current_Node = new c_NT3_State_Node_NonStrict;
          
          //Sets the new nodes ID.
          (*Current_Node)->NID.I = CNID.I;
          
          //Increment the node tracker.
          CNID.I++;
          
          //If axon couts are given expand the axon arrays.
          if (p_D){ (*Current_Node)->expand_Dendrite(p_D); }
          if (p_A){ (*Current_Node)->expand_Axon(p_A); }
          
          //Add it to the state tree, assuming that the current state has already been queried.
          State_Tree.set_Current_Node_NID(*Current_Node);
          
          //Set the current nodes state.
          (*Current_Node)->set_State(p_State);
          
          //Set the Current node to the next one in the chain.
          Current_Node = &(*Current_Node)->Next;
                    
          //Return the created state node using the state tree as Current_Node is changed.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Creates a new node.
     c_NT3_Base_Node_NonStrict * new_Node(int p_D = 0, int p_A = 0)
     {
          //A tmp Node because we iterate the Current_Node.
          c_NT3_Base_Node_NonStrict * tmp_Return_Node;
          
          //Create the state node.
          *Current_Node = new c_NT3_Node_NonStrict;
          tmp_Return_Node = *Current_Node;
          
          //Sets the new nodes ID.
          (*Current_Node)->NID.I = CNID.I;
          
          //Increment the node tracker.
          CNID.I++;
          
          //If axon couts are given expand the axon arrays.
          if (p_D){ (*Current_Node)->expand_Dendrite(p_D); }
          if (p_A){ (*Current_Node)->expand_Axon(p_A); }
          
          //Set the Current node to the next one in the chain.
          Current_Node = &(*Current_Node)->Next;
          
          //Return the node that was created.
          return tmp_Return_Node;
     }
     
     //Creates a treetop state node for cases where input has only one tier.
     void convert_To_Treetop_Node(c_NT3_Base_Node_NonStrict * p_Node)
     {
          
          if (p_Node->get_Type() == 0){ p_Node->set_Type(2); } //If the node is a state node then set it to a state treetop node.
          if (p_Node->get_Type() == 1){ p_Node->set_Type(3); } //IF the node is a normal node then set the type to treetop.
          
          //Search for the node in the treetop tree.
          Treetops_Tree.search(p_Node->NID);
          
          //If the node has not been found then add it to the tree.
          if (Treetops_Tree.get_Current_Node_NID() == NULL)
          {
               Treetops_Tree.set_Current_Node_NID(p_Node);
          }
     }
     
     
     ////==------------------------------------+
     //==--   CONNECTION TESTING AND CREATION
     ////==------------------------------------+
     
     //Creates a connection from p_From to p_To.
     void create_Connection(c_NT3_Base_Node_NonStrict * p_From, c_NT3_Base_Node_NonStrict * p_To)
     {
          p_From->add_Axon(p_To);
          p_To->add_Dendrite(p_From);
     }
     
     //Creates a connection from p_From to p_To.
     void create_Predefined_Connection(c_NT3_Base_Node_NonStrict * p_From, c_NT3_Base_Node_NonStrict * p_To)
     {
          p_From->add_Predefined_Axon(p_To);
          p_To->add_Predefined_Dendrite(p_From);
     }
     
     //Checks for an upper tier connection.
     c_NT3_Base_Node_NonStrict * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_NonStrict ** p_Nodes, int p_Node_Count)
     {
          if (p_Node_Count < 1){ return NULL; }
          return p_Nodes[0]->does_Upper_Tier_Connection_Exist(p_Nodes, p_Node_Count);
     }
     
     
     
     ////==---------------------+
     //==--   RETRIEVING NODES
     ////==---------------------+
     
     //Assigns a given node to a state, used for loading.
     c_NT3_Base_Node_NonStrict * assign_State_Node(u_Data_3 p_State, c_NT3_Base_Node_NonStrict * p_Node, int p_A_L = 0, int p_A_R = 0)
     {
          //Search for the node.
          State_Tree.search(p_State);
          
          //If the node has not been found then create it.
          if (State_Tree.get_Current_Node_NID() == NULL)
          {
               new_State_Node(p_State, p_A_L, p_A_R);
          }
          
          //Return the current node NID.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Get a state node.
     c_NT3_Base_Node_NonStrict * get_State_Node(u_Data_3 p_State, int p_A_L = 0, int p_A_R = 0)
     {
          //Search for the node.
          State_Tree.search(p_State);
          
          //If the node has not been found then create it.
          if (State_Tree.get_Current_Node_NID() == NULL)
          {
               new_State_Node(p_State, p_A_L, p_A_R);
          }
          
          //Return the current node NID.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Get a state node for a query, if none exist return NULL.
     c_NT3_Base_Node_NonStrict * get_State_Node_Query(u_Data_3 p_State)
     {
          //Search for the node.
          State_Tree.search(p_State);
          
          //Return the current node NID.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Gets an upper tier connection even if one has to be created.
     c_NT3_Base_Node_NonStrict * get_Upper_Tier_Connection(c_NT3_Base_Node_NonStrict ** p_Nodes, int p_Node_Count)
     {
          //A tmp var to hold the upper tier node for returning.
          c_NT3_Base_Node_NonStrict * tmp_Node = NULL;
          
          if (p_Node_Count < 1){ return NULL; }
          
          //Check for an upper tier connection already, if one exists then return it.
          tmp_Node = p_Nodes[0]->does_Upper_Tier_Connection_Exist(p_Nodes, p_Node_Count);
          if (tmp_Node != NULL)
          {
               return tmp_Node;
          }
          
          //If one does not exist then create it.
          tmp_Node = new_Node(p_Node_Count);
          
          //Adds all the given nodes to the dendrite process of the upper tier node.
          //Adds the upper node to the axonic procceses of the lower nodes.
          for (int cou_Index=0;cou_Index<p_Node_Count;cou_Index++)
          {
               tmp_Node->add_Predefined_Dendrite(p_Nodes[cou_Index]);
               p_Nodes[cou_Index]->add_Axon(tmp_Node);
          }
          
          return tmp_Node;
     }
     
     //Gets a treetop connection.
     c_NT3_Base_Node_NonStrict * get_Treetop_Connection(c_NT3_Base_Node_NonStrict ** p_Nodes, int p_Node_Count)
     {
          c_NT3_Base_Node_NonStrict * tmp_Node = get_Upper_Tier_Connection(p_Nodes, p_Node_Count);
          
          convert_To_Treetop_Node(tmp_Node);
          
          return tmp_Node;
     }
     
     
     ////==----------------------------+
     //==--   BACKPROPAGATION STATION
     ////==----------------------------+
     
     //Starts the Backprop procedures for output only.
     void bp_Output_Only(c_NT3_Base_Node_NonStrict * p_Node, int p_Type = 0)
     {
          //For when directly output the dendrites of a nodes on the base tier.
          if (p_Node == NULL){ return; }
          ochr(0, 12, '<');
          bp_Output_Only_Lower(p_Node, p_Type);
          ochr(0, 12, '>');
     }
     
     //bp_Output the left node.
     void bp_Output_Only_Lower(c_NT3_Base_Node_NonStrict * p_Node, int p_Type)
     {
          char tmp_State = ' ';
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (p_Node->Dendrite_Count > 0)
          {
               for (int cou_Index=0;cou_Index<p_Node->Dendrite_Count;cou_Index++)
               {
                    bp_Output_Only_Lower(p_Node->Dendrites[cou_Index], p_Type);
               }
          }
          else
          {
               if (p_Type == 0)
               {
                    tmp_State = char (p_Node->get_State());
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    cout << tmp_State;
               }
               if (p_Type == 1)
               {
                    cout << p_Node->get_State() << " ";
               }
          }
     }
     
     //Starts the Backprop procedures for output only.
     void bp(c_Raw_Table_1D * p_Pattern_Output, int p_Input, c_NT3_Base_Node_NonStrict * p_Node)
     {
          //For when directly output the dendrites of a nodes on the base tier.
          if (p_Node == NULL){ return; }
          
          p_Pattern_Output->new_Cell(p_Input);
          
          bp_Lower(p_Pattern_Output, p_Input, p_Node);
     }
     
     //bp_Output the left node.
     void bp_Lower(c_Raw_Table_1D * p_Pattern_Output, int p_Input, c_NT3_Base_Node_NonStrict * p_Node)
     {
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (p_Node->Dendrite_Count > 0)
          {
               for (int cou_Index=0;cou_Index<p_Node->Dendrite_Count;cou_Index++)
               {
                    bp_Lower(p_Pattern_Output, p_Input, p_Node->Dendrites[cou_Index]);
               }
          }
          else
          {
               p_Pattern_Output->add_Data_Int(p_Input, p_Node->get_State());
          }
     }
     
     //Backpropagates a given Treetop. Returns the treetops address.
     c_NT3_Base_Node_NonStrict * bp_Treetop(c_Raw_Table_1D * p_Pattern_Output, int p_Input, u_Data_3 p_NID)
     {
          c_NT3_Base_Node_NonStrict * tmp_Node = NULL;
          
          //Search for the node.
          Treetops_Tree.search(p_NID);
          
          //Return the current node NID.
          tmp_Node = Treetops_Tree.get_Current_Node_NID();
          
          //Backpropagate the treetop node. It is assumed this will always be a valid treetop, however, the bp() will catch a NULL with no error.
          bp(p_Pattern_Output, p_Input, tmp_Node);
          
          return tmp_Node;
     }
     
     ////==---------------------------------+
     //==--   SAVING AND LOADING FUNCTIONS
     ////==---------------------------------+
     
     //Saves the node network.
     void save(ofstream * p_SF)
     {
          //Create the file.
          cout << "\n\n Node Network Saving..........";
          if (p_SF->is_open()){ cout << "Authorized"; } else { cout << "Denied"; return; }
          
          //Save the nodes of the nodes.
          cout << "\n Saving Nodes.........." << CNID.U << ".Found.....";
          
          *p_SF << "\nNode_Count " << CNID.U;
          
          c_NT3_Base_Node_NonStrict * tmp_Node = Root;
          
          while(tmp_Node != NULL)
          {
               if (tmp_Node->Type == 0)
               {
                    *p_SF << "\nSN " << tmp_Node->get_State();
                    *p_SF << " " << tmp_Node->NID.I;
                    *p_SF << " " << tmp_Node->Axon_Count;
               }
               if (tmp_Node->Type == 1)
               {
                    *p_SF << "\nN ";
                    *p_SF << " " << tmp_Node->NID.I;
                    *p_SF << " " << tmp_Node->Axon_Count;
                    *p_SF << " " << tmp_Node->Dendrite_Count;
                    for (int cou_Index=0;cou_Index<tmp_Node->Dendrite_Count;cou_Index++)
                    {
                         *p_SF << " " << tmp_Node->Dendrites[cou_Index]->NID.I;
                    }
               }
               if (tmp_Node->Type == 2)
               {
                    *p_SF << "\nSTN " << tmp_Node->get_State();
                    *p_SF << " " << tmp_Node->NID.I;
                    *p_SF << " " << tmp_Node->Axon_Count;
               }
               if (tmp_Node->Type == 3)
               {
                    *p_SF << "\nTN ";
                    *p_SF << " " << tmp_Node->NID.I;
                    *p_SF << " " << tmp_Node->Axon_Count;
                    *p_SF << " " << tmp_Node->Dendrite_Count;
                    for (int cou_Index=0;cou_Index<tmp_Node->Dendrite_Count;cou_Index++)
                    {
                         *p_SF << " " << tmp_Node->Dendrites[cou_Index]->NID.I;
                    }
               }
               *p_SF << " " << tmp_Node->RC_Lvl;
               
               tmp_Node = tmp_Node->Next;
          }
          
          cout << "Complete";
     }
     
     //Loads the node network, new faster method, dendrites only.
     void load(ifstream * p_LF)
     {
          //Create the file.
          cout << "\n\n Node Network Loading..........";
          if (p_LF->is_open()){ cout << "Authorized"; } else { cout << "Denied"; return; }
          
          u_Data_3 Node_Count;
          Node_Count.U = 0;
          
          //The new scaffold.
          c_NT3_Base_Node_NonStrict ** tmp_NScaffold = NULL;
          
          c_NT3_Base_Node_NonStrict * tmp_Node = NULL;
          
          string tmp_Node_Type = "";
          
          u_Data_3 tmp_State;
          tmp_State.I = 0;
          
          u_Data_3 tmp_NID;
          tmp_NID.I = 0;
          
          u_Data_3 tmp_D;
          tmp_D.I = 0;
          
          u_Data_3 tmp_RC_Lvl;
          tmp_RC_Lvl.F = 0.0f;
          
          int tmp_Axon_Count = 0;
          int tmp_Dendrite_Count = 0;
          
          *p_LF >> tmp_Node_Type;
          tmp_Node_Type = "";
          *p_LF >> Node_Count.U;
          cout << "\n " << Node_Count.U << " Nodes Found..........";
          
          //Setup the NScaffold
          if (Node_Count.U == 0){ return; }
          tmp_NScaffold = new c_NT3_Base_Node_NonStrict*[Node_Count.U];
          for (unsigned long long int cou_Index=0;cou_Index<Node_Count.U;cou_Index++)
          {
               tmp_NScaffold[cou_Index] = NULL;
          }
          
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_Y();
          u_Data_3 Node_Counter;
          Node_Counter.U = 1;
          int tmp_Div = Node_Count.I / 1000;
          if (tmp_Div == 0){ tmp_Div = 1; }
          
          while(!(*p_LF).eof())
          {
               if (!(Node_Counter.U % tmp_Div))
               { 
                    xy(tmp_X, tmp_Y); 
                    cout << ((double (Node_Counter.U) / double (Node_Count.U)) * 100) << "%      "; 
               }
               Node_Counter.U++;
               
               *p_LF >> tmp_Node_Type;
               //*cout << "\n\n" << tmp_Node_Type;
               if (tmp_Node_Type == "SN")
               {
                    *p_LF >> tmp_State.I;
                    //*cout << " St " << tmp_State.I;
                    
                    *p_LF >> tmp_NID.I;
                    //*cout << " NID " << tmp_NID.I;
                    
                    //Get the axon counts.
                    *p_LF >> tmp_Axon_Count;
                    
                    tmp_Node = get_State_Node(tmp_State, 0, tmp_Axon_Count);
                    
                    //Add the tmp_Node to the tmp_NScaffold
                    tmp_NScaffold[tmp_NID.I] = tmp_Node;
                    
               }
               if (tmp_Node_Type == "N")
               {
                    *p_LF >> tmp_NID.I;
                    //*cout << " NID " << tmp_NID.I;
                    
                    //Get the axon counts.
                    *p_LF >> tmp_Axon_Count;
                    *p_LF >> tmp_Dendrite_Count;
                    
                    //Create the node and add it to the state tree with the NID as an identifier.
                    tmp_Node = new_Node(tmp_Dendrite_Count, tmp_Axon_Count);
                    
                    //Add the tmp_Node to the tmp_NScaffold
                    tmp_NScaffold[tmp_NID.I] = tmp_Node;
                    
                    for (int cou_Index=0;cou_Index<tmp_Dendrite_Count;cou_Index++)
                    {
                         *p_LF >> tmp_D.I;
                         //*cout << " L " << tmp_L.I;
                         
                         //Create the lower connections.
                         create_Predefined_Connection(tmp_NScaffold[tmp_D.I], tmp_Node);
                    }
               }
                              
               if (tmp_Node_Type == "STN")
               {
                    *p_LF >> tmp_State.I;
                    //*cout << " St " << tmp_State.I;
                    
                    *p_LF >> tmp_NID.I;
                    //*cout << " NID " << tmp_NID.I;
                    
                    //Get the axon counts.
                    *p_LF >> tmp_Axon_Count;
                    
                    tmp_Node = get_State_Node(tmp_State, 0, tmp_Axon_Count);
                    convert_To_Treetop_Node(tmp_Node);
                    
                    //Add the tmp_Node to the tmp_NScaffold
                    tmp_NScaffold[tmp_NID.I] = tmp_Node;
                    
               }
               if (tmp_Node_Type == "TN")
               {
                    *p_LF >> tmp_NID.I;
                    //*cout << " NID " << tmp_NID.I;
                    
                    //Get the axon counts.
                    *p_LF >> tmp_Axon_Count;
                    *p_LF >> tmp_Dendrite_Count;
                    
                    //Create the node and add it to the state tree with the NID as an identifier.
                    tmp_Node = new_Node(tmp_Dendrite_Count, tmp_Axon_Count);
                    convert_To_Treetop_Node(tmp_Node);
                    
                    //Add the tmp_Node to the tmp_NScaffold
                    tmp_NScaffold[tmp_NID.I] = tmp_Node;
                    
                    tmp_Node->set_Type(3);
                    
                    for (int cou_Index=0;cou_Index<tmp_Dendrite_Count;cou_Index++)
                    {
                         *p_LF >> tmp_D.I;
                         //*cout << " L " << tmp_L.I;
                         
                         //Create the lower connections.
                         create_Predefined_Connection(tmp_NScaffold[tmp_D.I], tmp_Node);
                    }
               }
               
               *p_LF >> tmp_RC_Lvl.F;
               //*cout << " tmp_RC_Lvl " << tmp_RC_Lvl.F;
               
               tmp_Node->RC_Lvl = tmp_RC_Lvl.F;
               
               //*cout << " " << tmp_Node << " ";
               //tmp_Node->bp_O();
          }
          xy(tmp_X, tmp_Y); cout << "100.00%      ";
          cout << "\n Resetting Scaffold.....";
          
          //Derete the tmp_NScaffold
          for (unsigned long long int cou_Index=0;cou_Index<Node_Count.U;cou_Index++)
          {
               /*
               cout << "\n ["; oint(0, 13, cou_Index); cout << "] ";
               if (tmp_NScaffold[cou_Index] != NULL)
               { 
                    oint(0, 8, tmp_NScaffold[cou_Index]->NID.U);
                    cout << " ";
                    tmp_NScaffold[cou_Index]->bp_O(); 
                    
                    if (tmp_NScaffold[cou_Index]->Dendrite_L != NULL)
                    {
                         cout << " ";
                         tmp_NScaffold[cou_Index]->Dendrite_L->bp_O();
                    }
                    if (tmp_NScaffold[cou_Index]->Dendrite_R != NULL)
                    {
                         cout << " ";
                         tmp_NScaffold[cou_Index]->Dendrite_R->bp_O();
                    }
               }
               */
               tmp_NScaffold[cou_Index] = NULL;
          }
          delete [] tmp_NScaffold;
          
          cout << "Done\n";
     }
     

     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     void output_Treetops()
     {
          Treetops_Tree.output_Tree();
     }
     
     //Outputs the treetops as backpropagated nodes.
     void output_Treetops_BP()
     {
          Treetops_Tree.output_Tree_BP();
     }
     
     
     //Oututs all of the nodes.
     void output_Nodes()
     {
          c_NT3_Base_Node_NonStrict * tmp_LL = Root;
          c_NT3_Base_Node_NonStrict * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               cout << "\n  ";
               tmp_LL->bp_O();
               tmp_LL = tmp_LL_Next;
          }
     }
};