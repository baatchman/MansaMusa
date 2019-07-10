
class c_Charging_Buffer_Linked_Node_1D
{
     private:
          
     public:
          unsigned int Charge;
          c_Base_Node_1D * NID;
          c_Charging_Buffer_Linked_Node_1D * Next;
          
          c_Charging_Buffer_Linked_Node_1D()
          {
               Charge = 0;
               NID = NULL;
               Next = NULL;
          }
          
          c_Charging_Buffer_Linked_Node_1D(c_Base_Node_1D * p_NID, unsigned int p_Charge)
          {
               Charge = p_Charge;
               NID = p_NID;
               Next = NULL;
          }
          
          ~c_Charging_Buffer_Linked_Node_1D()
          {
               delete Next;
               Next = NULL;
          }
          
          void output_Nodes()
          {
               cout << "\n\t " << this << " NID->" << NID << " Charge->" << Charge;
               if (Next != NULL){ Next->output_Nodes(); }
          }
                
};

class c_Charging_Buffer_Node_1D
{
     friend class c_Charging_Buffer_Tree_1D; 
     private:
          
     public:
          unsigned int Charge;
          c_Base_Node_1D * NID;
          c_Charging_Buffer_Node_1D * Right;
          c_Charging_Buffer_Node_1D * Left;
          
          c_Charging_Buffer_Node_1D()
          {
               Charge = 0;
               NID = NULL;
               Right = NULL;
               Left = NULL;
          }
          
          ~c_Charging_Buffer_Node_1D()
          {
               delete Left;
               Left = NULL;
               delete Right;
               Right = NULL;
          }
                
};

class c_Charging_Buffer_Tree_1D
{
     private:
          
     public:
          c_Charging_Buffer_Node_1D * Root;
          c_Charging_Buffer_Node_1D ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          unsigned int Top_Charge;
          int Number_Of_Nodes;
     
          //The current Charge_Filter.
          float Charge_Filter;
          float Previous_Charge_Filter;
     
          //The lowest charge to make it onto the output.
          unsigned int Low_Charge;
          unsigned int Previous_Low_Charge;
          
          //Avoid the ini in the function.
          c_Charging_Buffer_Node_1D *** tmp_Search_Node;
          
          c_Charging_Buffer_Tree_1D()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
               Top_Charge = 0;
               Number_Of_Nodes = 0;
               Charge_Filter = 0;
               Low_Charge = 0;
               Previous_Charge_Filter = 0;
               Previous_Low_Charge = 0;
               tmp_Search_Node = new c_Charging_Buffer_Node_1D**[Central_Core_Count];
               
               for (int cou_T=0;cou_T<Central_Core_Count;cou_T++)
               {
                    tmp_Search_Node[cou_T] = NULL;
               }
          }
          
          ~c_Charging_Buffer_Tree_1D()
          {
               delete Root;
               delete [] tmp_Search_Node;
          }
          
          
          
          //Searches the tree using an iterative method rather than a recursive method.
          void search(c_Base_Node_1D * p_NID, unsigned int p_Charge, int p_Core = 0)
          {
               cout << "\n\t\t\t search(c_Base_Node_1D * p_NID, unsigned int p_Charge, int p_Core = 0)"; cout.flush();
               //Get the root node.
               tmp_Search_Node[p_Core] = &Root;
               
               //cout << "\n\n\t Searching NR   p_Data->" << p_Data.I; 
               
               //while ((*tmp_Search_Node[p_Core]) != NULL)
               while(1)
               {
                    flg_Foundit = false;
                    if (p_Charge > Top_Charge){ Top_Charge = p_Charge; }
                    //cout << "\n\t  tmp_SN->" << tmp_Search_Node[p_Core];
                    
                            
                    //If the node equals NULL then create a new node.
                    if ((*tmp_Search_Node[p_Core]) == NULL)
                    {
                         cout << "\n\t   tmp_Search_Node[p_Core]->" << tmp_Search_Node[p_Core] << "   *tmp_Search_Node[p_Core]->" << *tmp_Search_Node[p_Core];
                         
                         Mutex.lock();
                         //If the node has been set in the short time between the check and the mutex then continue.
                         if ((*tmp_Search_Node[p_Core]) == NULL)
                         {
                              try
                              {
                                   cout << "\n\t\t   Allocating New Node..."; cout.flush();
                                   (*tmp_Search_Node[p_Core]) = new c_Charging_Buffer_Node_1D;
                                   cout << "\n\t\t   New Node Allocated..."; cout.flush();
                                   (*tmp_Search_Node[p_Core])->NID = p_NID;
                                   (*tmp_Search_Node[p_Core])->Right = NULL;
                                   (*tmp_Search_Node[p_Core])->Left = NULL;
                                   (*tmp_Search_Node[p_Core])->Charge = p_Charge; cout.flush();
                                   cout << "\n\t\t   Members Are Set...";
                                   Current = tmp_Search_Node[p_Core];
                                   Number_Of_Nodes++;
                                   cout << "\n\t\t   Finished Allocation Block, New Node->" << (*tmp_Search_Node[p_Core]); cout.flush();
                              }
                              catch (...)
                              {
                                   cout << "\n\t    EXCEPTION!!!";
                              }
                              Mutex.unlock();
                              cout << "\n\t\t     return;"; cout.flush();
                              return;
                         }
                         Mutex.unlock();
                         
                    }
                    
                    //If the data matches a match has been found.
                    if ((*tmp_Search_Node[p_Core])->NID == p_NID)
                    {
                         cout << "\n\t   tmp_Node->Data->" << (*tmp_Search_Node[p_Core])->NID << " == p_NID->" << p_NID;
                         
                         
                         //If this is the current node then add the charges to it as long as it is below the maximum.
                         if (((*tmp_Search_Node[p_Core])->Charge + p_Charge) < INT_MAX)
                         {
                              (*tmp_Search_Node[p_Core])->Charge += p_Charge;
                         }
                         else
                         {
                              (*tmp_Search_Node[p_Core])->Charge = UINT_MAX;
                         }
                         
                         
                         //If the charge is over the current top charge then set the current top charge to this charge.
                         if (Top_Charge < (*tmp_Search_Node[p_Core])->Charge){ Top_Charge = (*tmp_Search_Node[p_Core])->Charge; }
                    
                         if ((*tmp_Search_Node[p_Core])->NID != NULL){ flg_Foundit = true; }
                         
                         Current = tmp_Search_Node[p_Core];
                         return;
                    }
                    
                    //If the data is less than the current data search the left leg.
                    if ((*tmp_Search_Node[p_Core])->NID > p_NID)
                    {
                         cout << "\n\t   tmp_Node->Data->" << (*tmp_Search_Node[p_Core])->NID << " > p_NID->" << p_NID;
                         
                         *tmp_Search_Node[p_Core] = ((*tmp_Search_Node[p_Core])->Left);
                         continue;
                    }
                    
                    //If the data is greater than the current data search the right leg.
                    if ((*tmp_Search_Node[p_Core])->NID < p_NID)
                    {
                         cout << "\n\t   tmp_Node->Data->" << (*tmp_Search_Node[p_Core])->NID << " < p_NID->" << p_NID;
                         
                         *tmp_Search_Node[p_Core] = ((*tmp_Search_Node[p_Core])->Right);
                         continue;
                    }
                    
               }
               
               
               //Return.
               return;
          }
          
          
          int query_Node(c_Charging_Buffer_Node_1D *&p_Node, c_Base_Node_1D * p_NID, unsigned int p_Charge)
          {
               if (p_Node == NULL){      
                    p_Node = new c_Charging_Buffer_Node_1D;
                    p_Node->Right = NULL;
                    p_Node->Left = NULL;
                    p_Node->Charge = p_Charge;
                    p_Node->NID = p_NID;
                    
                    //Increase the number of nodes.
                    Number_Of_Nodes++;
                    }
                    
               if (p_Node->NID == p_NID)
               {
                    //If this is the current node then add the charges to it as long as it is below the maximum.
                    if ((p_Node->Charge + p_Charge) < INT_MAX)
                    {
                         p_Node->Charge += p_Charge;
                    }
                    else
                    {
                         p_Node->Charge = UINT_MAX;
                    }
                    
                    
                    //If the charge is over the current top charge then set the current top charge to this charge.
                    if (Top_Charge < p_Node->Charge){ Top_Charge = p_Node->Charge; }
                    Current = &p_Node;  
                    if (p_Node->NID != NULL){ flg_Foundit = true; }
                    return 0;
               }
                    
               if (p_NID < p_Node->NID)
               {
                    query_Node(p_Node->Left, p_NID, p_Charge);
                    return 0;
               }
                    
               if (p_NID > p_Node->NID)
               {
                    query_Node(p_Node->Right, p_NID, p_Charge);
                    return 0;
               }
                    
               return 0;
          }
          
          void output_Tree()
          {
               cout << "\n\n\t Charging_Buffer_Tree(" << this << ")";
               cout << "\n\n\t\t   Node_Count->" << Number_Of_Nodes;
               cout << "\n\t\t   Top_Charge->" << Top_Charge;
               cout << "\n\t\t   Charge_Filter->" << Charge_Filter;
               cout << "\n\t\t   Low_Charge->" << Low_Charge;
               cout << "\n\t\t   Previous_Charge_Filter->" << Previous_Charge_Filter;
               cout << "\n\t\t   Previous_Low_Charge->" << Previous_Low_Charge;
               cout << endl;
               output_Node(Root);
          }
          
          void output_Node(c_Charging_Buffer_Node_1D *&p_Node)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left);
               cout << "\n\t &" << p_Node->NID << " >" << p_Node->Charge;
               output_Node(p_Node->Right);
          }
          
          c_Base_Node_1D * get_Node_NID(c_Charging_Buffer_Node_1D * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->NID;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_NID(c_Charging_Buffer_Node_1D * &p_Node, c_Base_Node_1D * p_NID)
          {
               if (p_Node != NULL){
                    p_Node->NID = p_NID;
                    }
          }
          
          c_Base_Node_1D * set_Current_Node_NID(c_Base_Node_1D * p_NID)
          {
               set_Node_NID(*Current, p_NID);
               return p_NID;
          }
          
          c_Base_Node_1D * get_Current_Node_NID()
          {
               return get_Node_NID(*Current);
          }
          
          //Resets the tree back to NULL.
          void reset_Tree()
          {
               //The top charge is reset along with the node count.
               Number_Of_Nodes = 0;
               Top_Charge = 0;
               
               Previous_Charge_Filter = Charge_Filter;
               Previous_Low_Charge = Low_Charge;
               
               Charge_Filter = 0;
               Low_Charge = 0;
               
               //Starting at the root node delete all the nodes.
               //reset_Node(Root);
               delete Root;
               Root = NULL;
          }
          
          //Resets the node.
          void reset_Node(c_Charging_Buffer_Node_1D *&p_Node)
          {
               if (p_Node == NULL){ return; }
               reset_Node(p_Node->Left);
               reset_Node(p_Node->Right);
               
               p_Node->NID = NULL;
               delete p_Node;
               p_Node = NULL;
          }
          
                    
};

//This class is the buffer to be used by the node network.
class c_Charging_Buffer_1D
{
protected:
     
public:
     
     //The charging tree.
     c_Charging_Buffer_Tree_1D Tree;
     
     //The buffer output nodes.
     c_Base_Node_1D ** NID_Output;
     
     //The charges attached to the nodes.
     unsigned int * Charge_Output;
     
     //The buffer output in linked list format.
     c_Charging_Buffer_Linked_Node_1D * Linked_Output;
     
     //The length of each buffer.
     int * Linked_Output_Length;
     
     //The current length of the buffer output.
     int Output_Length;
     
     
     c_Charging_Buffer_1D()
     {
          NID_Output = NULL;
          Charge_Output = NULL;
          Linked_Output = NULL;
          Linked_Output_Length = NULL;
          Output_Length = 0;
     }
     
     ~c_Charging_Buffer_1D()
     {
          reset_Tree();
          reset_Output();
     }
     
     
     
     //Submits a node and charge to the tree.
     void submit(c_Base_Node_1D * p_NID, unsigned int p_Charge, int p_Core = 0)
     {
          cout << "\n\t\t submit(c_Base_Node_1D * p_NID, unsigned int p_Charge, int p_Core = 0)"; cout.flush();
          //Submits the charge to the tree with the node it is attached to.
          Tree.search(p_NID, p_Charge, p_Core);
     }
     
     //Gathers the charges and nodes into the output buffer. The charge filter is applied as the nodes are read out.
     void gather_LL(float p_Charge_Filter, int p_Charge_Precision)
     {
          //cout << "\n\t gather()";
          //Tree.output_Tree();
          //cout << "\n Tree Output_Done...";
          
          //Resets the output before beginning.
          reset_Output();
          
          //Compute low charge for threshold testing.
          Tree.Charge_Filter = p_Charge_Filter;
          
          //New charging algorithm
          Tree.Low_Charge = (unsigned int) (float (Tree.Top_Charge) * Tree.Charge_Filter);
          
          //cout << "\n\n\n\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
          //cout << "\n\t\t Tree.Top_Charge->" << Tree.Top_Charge;
          //cout << "\n\t\t Tree.Charge_Filter->" << Tree.Charge_Filter;
          //cout << "\n\t\t Tree.Low_Charge->" << Tree.Low_Charge;
          //cout << "\n\n\n\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
          
          //Old Charge_Filter algorithm
          /*
          if ((Tree.Top_Charge - Tree.Charge_Filter) >= 1)
          {
               Tree.Low_Charge = (Tree.Top_Charge - Tree.Charge_Filter);
          }
          else
          {
               Tree.Low_Charge = 1;
          }
          
          //A -1 on the Charge_Filter allows all to pass.
          if (Tree.Charge_Filter == -1)
          {
               Tree.Low_Charge = 0;
          }*/
          
          
          delete [] Linked_Output;
          Linked_Output = NULL;
          
          Linked_Output = new c_Charging_Buffer_Linked_Node_1D[Central_Core_Count];
          
          delete [] Linked_Output_Length;
          Linked_Output_Length = NULL;
          
          Linked_Output_Length = new int[Central_Core_Count];
          
          for (int cou_Index=0;cou_Index<Central_Core_Count;cou_Index++)
          {
               Linked_Output_Length[cou_Index] = 0;
          }
          
          Output_Length = 0;
          
          //If the tree has no nodes then do not gather anything, wipe the output first.
          if (Tree.Number_Of_Nodes < 1){ return; }
          
          
          //Create the array to hold all nodes in the tree regardless of the fact they will not probably all be stored.
          //NID_Output = new c_Base_Node_1D * [Tree.Number_Of_Nodes];
          //Charge_Output = new unsigned int [Tree.Number_Of_Nodes];
          
          //The Low_Charge is used to determine baseline charge necessary.
          //gather_Node(Tree.Root, p_Charge_Precision);
          
          //console_Move_Cursor_To_Given_X_And_Y(0, 5);
          
          //set_Console_Color(0, ((rand() % 7) + 8));
          
          gather_Nodes_LL(p_Charge_Precision);
          
          //set_Console_Color(0, 10);
          
     }
     
     //Gathers the nodes non recursively.
     void gather_Nodes_LL(int p_Charge_Precision)
     {
          //cout << "\n  gather_Nodes()";// Number_Of_Nodes->" << Number_Of_Nodes;
          
          Output_Length = 0;
          int tmp_Current_Node_Stack = 0;
          
          if (Tree.Number_Of_Nodes == 0){ return; }
          
          //Used to track which node list to append the currently gathered node onto the end of.
          
          
          //This tracks which LL_Node is currently at the end of the stacks.
          c_Charging_Buffer_Linked_Node_1D ** tmp_Current_Node_Stack_Node;
          tmp_Current_Node_Stack_Node = new c_Charging_Buffer_Linked_Node_1D*[Central_Core_Count];
          
          for (int cou_Index=0;cou_Index<Central_Core_Count;cou_Index++)
          {
               tmp_Current_Node_Stack_Node[cou_Index] = &Linked_Output[cou_Index];
          }
          
          //This integer is used to track the current node in the pseudostack.
          int Node_EIP = 0;
          
          //This is used to determine whether or not a node has been gathered when moving back up the array.
          int * Node_State;
          Node_State = new int[Tree.Number_Of_Nodes];
          
          //This acts as a temporary stack.
          c_Charging_Buffer_Node_1D ** Node_Stack;
          Node_Stack = new c_Charging_Buffer_Node_1D*[Tree.Number_Of_Nodes];
          
          for (int cou_Index=0;cou_Index<Tree.Number_Of_Nodes;cou_Index++)
          {
               Node_State[cou_Index] = false;
               Node_Stack[cou_Index] = NULL;
          }
          
          //State here with the gathering of the root node.
          Node_Stack[0] = Tree.Root;
          
          //If the first node is null then break.
          if (Node_Stack[0] == NULL){ return; }
          
          //Gather the Root node.
          //If the node has sufficient charge then add it to the output.
          //cout << "\n   " << Node_Stack[Node_EIP] << " " << Node_Stack[Node_EIP]->Charge << " (" << (Node_Stack[Node_EIP]->Charge >= Tree.Low_Charge) << ")";
          if (Node_Stack[Node_EIP]->Charge >= Tree.Low_Charge)
          {
                    /*
                    tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->Next = new c_Charging_Buffer_Linked_Node_1D(Node_Stack[Node_EIP]->NID, ((float (Node_Stack[Node_EIP]->Charge) / float (Tree.Top_Charge)) * p_Charge_Precision));
                    tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack] = tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->Next;
                    */
                    tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->NID = Node_Stack[Node_EIP]->NID;
                    tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->Charge = ((float (Node_Stack[Node_EIP]->Charge) / float (Tree.Top_Charge)) * p_Charge_Precision);
                    tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->Next = new c_Charging_Buffer_Linked_Node_1D();;
                    tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack] = tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->Next;
                    Linked_Output_Length[tmp_Current_Node_Stack]++;
                    
                    tmp_Current_Node_Stack++;
                    if (tmp_Current_Node_Stack >= Central_Core_Count){ tmp_Current_Node_Stack = 0; }
                    
          }
                         
          //cout << "\n\t     Node_EIP->" << Node_EIP;// << "srt_P->" << srt_Position << " srt_Data->" << srt_Data[srt_Position].I << " srt_NID->" << srt_NID[srt_Position];
                         
          
          while (1)
          {
               if (Node_EIP < 0){ break; }
               
               //cout << "\n   " << Node_EIP << " " << Output_Length << " NID:" << NID_Output[Output_Length - 1] << " C:" << Charge_Output[Output_Length - 1];
               
               //Check State of the current node.
               //cout << "\n\t   St";
               //If 0 then check the left leg and increment the state.
               
               if (Node_State[Node_EIP] == 0)
               {
                    //Increment the counter for the current node.
                    Node_State[Node_EIP]++;
               
                    //If there is a left leg then add it to the pseudostack and set the EIP. Then continue with the investigation.
                    if (Node_Stack[Node_EIP]->Left != NULL)
                    {
                         //cout << "\n\t   L";
                         
                         Node_Stack[Node_EIP + 1] = Node_Stack[Node_EIP]->Left;
                         Node_EIP++;
                         
                         //Gather the left node.
                         
                         //If the node has sufficient charge then add it to the output.
                         //cout << "\n   " << Node_Stack[Node_EIP] << " " << Node_Stack[Node_EIP]->Charge << " (" << (Node_Stack[Node_EIP]->Charge >= Tree.Low_Charge) << ")";
                         if (Node_Stack[Node_EIP]->Charge >= Tree.Low_Charge)
                         {
                              tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->NID = Node_Stack[Node_EIP]->NID;
                              tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->Charge = ((float (Node_Stack[Node_EIP]->Charge) / float (Tree.Top_Charge)) * p_Charge_Precision);
                              tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->Next = new c_Charging_Buffer_Linked_Node_1D();;
                              tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack] = tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->Next;
                              Linked_Output_Length[tmp_Current_Node_Stack]++;
                              tmp_Current_Node_Stack++;
                              if (tmp_Current_Node_Stack >= Central_Core_Count){ tmp_Current_Node_Stack = 0; }
                         }
                         //cout << "\n\t     Node_EIP->" << Node_EIP;// << "srt_P->" << srt_Position << " srt_Data->" << srt_Data[srt_Position].I << " srt_NID->" << srt_NID[srt_Position];
                         
                         
                         continue;
                    }
               }
                    
               //If it is 1 then check the right leg and increment it.
               if (Node_State[Node_EIP] == 1)
               {     
                    //Increment the counter for the current node.
                    Node_State[Node_EIP]++;
                    
                    //If there is a right leg then add it to the pseudostack and set the EIP accordingly.
                    if (Node_Stack[Node_EIP]->Right != NULL)
                    {
                         //cout << "\n\t   R";
                         
                         Node_Stack[Node_EIP + 1] = Node_Stack[Node_EIP]->Right;
                         Node_EIP++;
                         
                         //If the node has sufficient charge then add it to the output.
                         //cout << "\n   " << Node_Stack[Node_EIP] << " " << Node_Stack[Node_EIP]->Charge << " (" << (Node_Stack[Node_EIP]->Charge >= Tree.Low_Charge) << ")";
                         if (Node_Stack[Node_EIP]->Charge >= Tree.Low_Charge)
                         {
                              tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->NID = Node_Stack[Node_EIP]->NID;
                              tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->Charge = ((float (Node_Stack[Node_EIP]->Charge) / float (Tree.Top_Charge)) * p_Charge_Precision);
                              tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->Next = new c_Charging_Buffer_Linked_Node_1D();;
                              tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack] = tmp_Current_Node_Stack_Node[tmp_Current_Node_Stack]->Next;
                              Linked_Output_Length[tmp_Current_Node_Stack]++;
                              tmp_Current_Node_Stack++;
                              if (tmp_Current_Node_Stack >= Central_Core_Count){ tmp_Current_Node_Stack = 0; }   
                         }
                         
                         //cout << "\n\t     Node_EIP->" << Node_EIP;// << "srt_P->" << srt_Position << " srt_Data->" << srt_Data[srt_Position].I << " srt_NID->" << srt_NID[srt_Position];
                         
                         
                         continue;
                    }
                         
                    //If the right leg does not exist then fall through to the undocking procedures.
               }
               
               //cout << "\n\t   U";
               
               //If the routine has fallen through to this point then do the backpedal pseudostack.
               Node_Stack[Node_EIP] = NULL;
               Node_State[Node_EIP] = 0;
               Node_EIP--;
               //console_Move_Cursor_To_Given_X_And_Y(0, 5);
                    
          }
               
          delete [] Node_Stack;
          Node_Stack = NULL;
          
          delete [] Node_State;
          Node_State = NULL;
          
          Output_Length = 0;
          for (int cou_Index=0;cou_Index<Central_Core_Count;cou_Index++)
          {
               Output_Length += Linked_Output_Length[cou_Index];
          }
          
     }
     
     
     
     //Gathers the charges and nodes into the output buffer. The charge filter is applied as the nodes are read out.
     void gather___OLD(float p_Charge_Filter, int p_Charge_Precision)
     {
          //cout << "\n\t gather()";
          //Tree.output_Tree();
          
          //Resets the output before beginning.
          reset_Output();
          
          //Compute low charge for threshold testing.
          Tree.Charge_Filter = p_Charge_Filter;
          
          //New charging algorithm
          Tree.Low_Charge = (unsigned int) (float (Tree.Top_Charge) * Tree.Charge_Filter);
          
          //cout << "\n\n\n\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
          //cout << "\n\t\t Tree.Top_Charge->" << Tree.Top_Charge;
          //cout << "\n\t\t Tree.Charge_Filter->" << Tree.Charge_Filter;
          //cout << "\n\t\t Tree.Low_Charge->" << Tree.Low_Charge;
          //cout << "\n\n\n\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~";
          
          //Old Charge_Filter algorithm
          /*
          if ((Tree.Top_Charge - Tree.Charge_Filter) >= 1)
          {
               Tree.Low_Charge = (Tree.Top_Charge - Tree.Charge_Filter);
          }
          else
          {
               Tree.Low_Charge = 1;
          }
          
          //A -1 on the Charge_Filter allows all to pass.
          if (Tree.Charge_Filter == -1)
          {
               Tree.Low_Charge = 0;
          }*/
          
          //If the tree has no nodes then do not gather anything, wipe the output first.
          if (Tree.Number_Of_Nodes < 1){ return; }
          
          
          //Create the array to hold all nodes in the tree regardless of the fact they will not probably all be stored.
          NID_Output = new c_Base_Node_1D * [Tree.Number_Of_Nodes];
          Charge_Output = new unsigned int [Tree.Number_Of_Nodes];
          
          for (int cou_Index=0;cou_Index<Tree.Number_Of_Nodes;cou_Index++)
          {
               NID_Output[cou_Index] = NULL;
               Charge_Output[cou_Index] = 0;
          }
          
          //The output length will be lengthened for every node read into the buffer output.
          Output_Length = 0;
          
          //The Low_Charge is used to determine baseline charge necessary.
          //gather_Node(Tree.Root, p_Charge_Precision);
          
          //console_Move_Cursor_To_Given_X_And_Y(0, 5);
          
          //set_Console_Color(0, ((rand() % 7) + 8));
          
          gather_Nodes___OLD(p_Charge_Precision);
          
          //set_Console_Color(0, 10);
          
     }
     
     
     //Gathers the nodes non recursively.
     void gather_Nodes___OLD(int p_Charge_Precision)
     {
          //cout << "\n  gather_Nodes() Number_Of_Nodes->" << Number_Of_Nodes;
          
          if (Tree.Number_Of_Nodes == 0){ return; }
          
          //This integer is used to track the current node in the pseudostack.
          int Node_EIP = 0;
          
          //This is used to determine whether or not a node has been gathered when moving back up the array.
          int * Node_State;
          Node_State = new int[Tree.Number_Of_Nodes];
          
          //This acts as a temporary stack.
          c_Charging_Buffer_Node_1D ** Node_Stack;
          Node_Stack = new c_Charging_Buffer_Node_1D*[Tree.Number_Of_Nodes];
          
          for (int cou_Index=0;cou_Index<Tree.Number_Of_Nodes;cou_Index++)
          {
               Node_State[cou_Index] = false;
               Node_Stack[cou_Index] = NULL;
          }
          
          //State here with the gathering of the root node.
          Node_Stack[0] = Tree.Root;
          
          //If the first node is null then break.
          if (Node_Stack[0] == NULL){ return; }
          
          //Gather the Root node.
          //If the node has sufficient charge then add it to the output.
          if (Node_Stack[Node_EIP]->Charge >= Tree.Low_Charge)
          {
                    NID_Output[Output_Length] = Node_Stack[Node_EIP]->NID;
                    Charge_Output[Output_Length] = ((float (Node_Stack[Node_EIP]->Charge) / float (Tree.Top_Charge)) * p_Charge_Precision);
                    Output_Length++;  
          }
                         
          //cout << "\n\t     Node_EIP->" << Node_EIP << "srt_P->" << srt_Position << " srt_Data->" << srt_Data[srt_Position].I << " srt_NID->" << srt_NID[srt_Position];
                         
          
          while (1)
          {
               if (Node_EIP < 0){ break; }
               
               //cout << "\n   " << Node_EIP << " " << Output_Length << " NID:" << NID_Output[Output_Length - 1] << " C:" << Charge_Output[Output_Length - 1];
               
               //Check State of the current node.
               //cout << "\n\t   St";
               //If 0 then check the left leg and increment the state.
               
               if (Node_State[Node_EIP] == 0)
               {
                    //Increment the counter for the current node.
                    Node_State[Node_EIP]++;
               
                    //If there is a left leg then add it to the pseudostack and set the EIP. Then continue with the investigation.
                    if (Node_Stack[Node_EIP]->Left != NULL)
                    {
                         //cout << "\n\t   L";
                         
                         Node_Stack[Node_EIP + 1] = Node_Stack[Node_EIP]->Left;
                         Node_EIP++;
                         
                         //Gather the left node.
                         
                         //If the node has sufficient charge then add it to the output.
                         if (Node_Stack[Node_EIP]->Charge >= Tree.Low_Charge)
                         {
                              NID_Output[Output_Length] = Node_Stack[Node_EIP]->NID;
                              Charge_Output[Output_Length] = ((float (Node_Stack[Node_EIP]->Charge) / float (Tree.Top_Charge)) * p_Charge_Precision);
                              Output_Length++;     
                         }
                         //cout << "\n\t     Node_EIP->" << Node_EIP << "srt_P->" << srt_Position << " srt_Data->" << srt_Data[srt_Position].I << " srt_NID->" << srt_NID[srt_Position];
                         
                         
                         continue;
                    }
               }
                    
               //If it is 1 then check the right leg and increment it.
               if (Node_State[Node_EIP] == 1)
               {     
                    //Increment the counter for the current node.
                    Node_State[Node_EIP]++;
                    
                    //If there is a right leg then add it to the pseudostack and set the EIP accordingly.
                    if (Node_Stack[Node_EIP]->Right != NULL)
                    {
                         //cout << "\n\t   R";
                         
                         Node_Stack[Node_EIP + 1] = Node_Stack[Node_EIP]->Right;
                         Node_EIP++;
                         
                         //If the node has sufficient charge then add it to the output.
                         if (Node_Stack[Node_EIP]->Charge >= Tree.Low_Charge)
                         {
                              NID_Output[Output_Length] = Node_Stack[Node_EIP]->NID;
                              Charge_Output[Output_Length] = ((float (Node_Stack[Node_EIP]->Charge) / float (Tree.Top_Charge)) * p_Charge_Precision);
                              Output_Length++;       
                         }
                         
                         //cout << "\n\t     Node_EIP->" << Node_EIP << "srt_P->" << srt_Position << " srt_Data->" << srt_Data[srt_Position].I << " srt_NID->" << srt_NID[srt_Position];
                         
                         
                         continue;
                    }
                         
                    //If the right leg does not exist then fall through to the undocking procedures.
               }
               
               //cout << "\n\t   U";
               
               //If the routine has fallen through to this point then do the backpedal pseudostack.
               Node_Stack[Node_EIP] = NULL;
               Node_State[Node_EIP] = 0;
               Node_EIP--;
               //console_Move_Cursor_To_Given_X_And_Y(0, 5);
                    
          }
               
          delete [] Node_Stack;
          Node_Stack = NULL;
          
          delete [] Node_State;
          Node_State = NULL;
          
     }
     
     

     
     //Resets the tree.
     void reset_Tree()
     {
          Tree.reset_Tree();
     }
     
     //Resets the output.
     void reset_Output()
     {
          delete [] NID_Output;
          NID_Output = NULL;
          
          delete [] Charge_Output;
          Charge_Output = NULL;
          
          if (Linked_Output != NULL)
          {
               delete [] Linked_Output;
          }
          Linked_Output = NULL;
          
          if (Linked_Output_Length != NULL)
          {
               delete [] Linked_Output_Length;
          }
          Linked_Output_Length = NULL;
          
          Output_Length = 0;
     }
     
     //Resets the buffer for new evaluation.
     void reset_Buffer()
     {
          reset_Tree();
          reset_Output();
     }
     
     //Output the output array as well as the tree.
     void output_Charging_Buffer()
     {
          cout << "\n\n\t----------";
          cout << "\n\t Charging_Buffer->" << this;
          Tree.output_Tree();
          output_Output();
     }
     
     //Output the output array.
     void output_Output()
     {
          
          cout << "\n\t  Total_Output_Length->" << Output_Length;
          for (int cou_C=0;cou_C<Central_Core_Count;cou_C++)
          {
               cout << "\n\t Core<[" << Central_Core_Count << "]>";
               cout << "\n\t  output_Length->" << Linked_Output_Length[cou_C];
               
               cout << "\n\t    [NID | Charge]-> ";
               
               if (Linked_Output == NULL){ continue; }
               Linked_Output[cou_C].output_Nodes();
          }
          
     }

};
