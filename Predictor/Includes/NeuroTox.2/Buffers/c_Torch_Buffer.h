
class c_Torch_Buffer_Node_1D
{
     friend class c_Torch_Buffer_Tree_1D; 
     private:
          
     public:
          unsigned int Charge;
          c_Base_Node_1D * NID;
          c_Torch_Buffer_Node_1D * Right;
          c_Torch_Buffer_Node_1D * Left;
                
          ~c_Torch_Buffer_Node_1D()
          {
               delete Left;
               Left = NULL;
               delete Right;
               Right = NULL;
          }
};

class c_Torch_Buffer_Tree_1D
{
     private:
          
     public:
          c_Torch_Buffer_Node_1D * Root;
          c_Torch_Buffer_Node_1D ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          unsigned int Top_Charge;
          int Number_Of_Nodes;
     
          //The current Charge_Filter.
          int Charge_Filter;
     
          //The lowest charge to make it onto the output.
          unsigned int Low_Charge;
          
          c_Torch_Buffer_Tree_1D()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
               Top_Charge = 0;
               Number_Of_Nodes = 0;
               Charge_Filter = 0;
               Low_Charge = 0;
          }
          
          ~c_Torch_Buffer_Tree_1D()
          {
               delete Root;
          }
          

          //Avoid the ini in the function.
          c_Torch_Buffer_Node_1D ** tmp_Search_Node;
          
          //Searches the tree using an iterative method rather than a recursive method.
          void search(c_Base_Node_1D * p_NID, unsigned int p_Charge)
          {
               flg_Foundit = false;
               if (p_Charge > Top_Charge){ Top_Charge = p_Charge; }
               
               //Get the root node.
               tmp_Search_Node = &Root;
               
               //cout << "\n\n\t Searching NR   p_Data->" << p_Data.I; 
               
               while ((*tmp_Search_Node) != NULL)
               {
                    //cout << "\n\t  tmp_SN->" << tmp_Search_Node;
                    
                    //If the data matches a match has been found.
                    if ((*tmp_Search_Node)->NID == p_NID)
                    {
                         //If this is the current node then add the charges to it as long as it is below the maximum.
                         if (((*tmp_Search_Node)->Charge + p_Charge) < INT_MAX)
                         {
                              (*tmp_Search_Node)->Charge += p_Charge;
                         }
                         else
                         {
                              (*tmp_Search_Node)->Charge = UINT_MAX;
                         }
                         
                         
                         //If the charge is over the current top charge then set the current top charge to this charge.
                         if (Top_Charge < (*tmp_Search_Node)->Charge){ Top_Charge = (*tmp_Search_Node)->Charge; }
                    
                         if ((*tmp_Search_Node)->NID != NULL){ flg_Foundit = true; }
                         
                         Current = tmp_Search_Node;
                         return;
                    }
                    
                    //If the data is less than the current data search the left leg.
                    if ((*tmp_Search_Node)->NID > p_NID)
                    {
                         //cout << "\n\t   tmp_Node->Data->" << (*tmp_Search_Node)->Data.I << " > p_Data.I->" << p_Data.I;
                         
                         tmp_Search_Node = &((*tmp_Search_Node)->Left);
                         continue;
                    }
                    
                    //If the data is greater than the current data search the right leg.
                    if ((*tmp_Search_Node)->NID < p_NID)
                    {
                         //cout << "\n\t   tmp_Node->Data->" << (*tmp_Search_Node)->Data.I << " < p_Data.I->" << p_Data.I;
                         
                         tmp_Search_Node = &((*tmp_Search_Node)->Right);
                         continue;
                    }
                    
               }
               
               //If the node equals NULL then create a new node.
               if ((*tmp_Search_Node) == NULL)
               {
                    (*tmp_Search_Node) = new c_Torch_Buffer_Node_1D;
                    (*tmp_Search_Node)->NID = p_NID;
                    (*tmp_Search_Node)->Right = NULL;
                    (*tmp_Search_Node)->Left = NULL;
                    (*tmp_Search_Node)->Charge = p_Charge;
                    Current = tmp_Search_Node;
                    Number_Of_Nodes++;
                    return;
               }
               
               //Return.
               return;
          }
          
          void search_R(c_Base_Node_1D * p_NID, unsigned int p_Charge)
          {
               flg_Foundit = false;
               if (p_Charge > Top_Charge){ Top_Charge = p_Charge; }
               query_Node(Root, p_NID, p_Charge);
          }
          
          int query_Node(c_Torch_Buffer_Node_1D *&p_Node, c_Base_Node_1D * p_NID, int p_Charge)
          {
               if (p_Node == NULL){      
                    p_Node = new c_Torch_Buffer_Node_1D;
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
                    if (((p_Node->Charge / UINT_MAX) + (p_Charge / UINT_MAX)) < 1)
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
               cout << "\n\n\t Torch_Buffer_Tree(" << this << ")";
               cout << "\n\t\t   Node_Count->" << Number_Of_Nodes;
               cout << "\n\t\t   Top_Charge->" << Top_Charge;
               cout << "\n\t\t   Charge_Filter->" << Charge_Filter;
               cout << "\n\t\t   Low_Charge->" << Low_Charge;
               cout << endl;
               output_Node(Root);
          }
          
          void output_Node(c_Torch_Buffer_Node_1D *&p_Node)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left);
               cout << "\n\t &" << p_Node->NID << " >" << p_Node->Charge;
               output_Node(p_Node->Right);
          }
          
          c_Base_Node_1D * get_Node_NID(c_Torch_Buffer_Node_1D * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->NID;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_NID(c_Torch_Buffer_Node_1D * &p_Node, c_Base_Node_1D * p_NID)
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
               
               //Starting at the root node delete all the nodes.
               reset_Node(Root);
               Root = NULL;
          }
          
          //Resets the node.
          void reset_Node(c_Torch_Buffer_Node_1D *&p_Node)
          {
               if (p_Node == NULL){ return; }
               reset_Node(p_Node->Left);
               reset_Node(p_Node->Right);
               delete p_Node;
               p_Node = NULL;
          }
          
                    
};

//This class is the buffer to be used by the node network.
class c_Torch_Buffer_1D
{
protected:
     
public:
     
     //The charging tree.
     c_Torch_Buffer_Tree_1D Tree;
     
     //The buffer output nodes.
     c_Base_Node_1D ** NID_Output;
     
     //The charges attached to the nodes.
     int * Charge_Output;
     
     //The current length of the buffer output.
     int Output_Length;
     
     
     c_Torch_Buffer_1D()
     {
          NID_Output = NULL;
          Charge_Output = NULL;
          Output_Length = 0;
     }
     
     ~c_Torch_Buffer_1D()
     {
          reset_Tree();
          reset_Output();
     }
     
     //Submits a node and charge to the tree.
     void submit(c_Base_Node_1D * p_NID, int p_Charge)
     {
          //Submits the charge to the tree with the node it is attached to.
          Tree.search(p_NID, p_Charge);
     }
     
     //Gathers the charges and nodes into the output buffer. The charge filter is applied as the nodes are read out.
     void gather(int p_Charge_Filter)
     {
          //Compute low charge for threshold testing.
          Tree.Charge_Filter = p_Charge_Filter;
          
          if ((Tree.Top_Charge - Tree.Charge_Filter) >= 0)
          {
               Tree.Low_Charge = (Tree.Top_Charge - Tree.Charge_Filter);
          }
          
          //A -1 on the Charge_Filter allows all to pass.
          if (Tree.Charge_Filter == -1)
          {
               Tree.Low_Charge = 0;
          }
          
          //Resets the output before beginning.
          reset_Output();
          
          //If the tree has no nodes then do not gather anything.
          if (Tree.Number_Of_Nodes < 1){ return; }
          
          //Create the array to hold all nodes in the tree regardless of the fact they will not probably all be stored.
          NID_Output = new c_Base_Node_1D * [Tree.Number_Of_Nodes];
          Charge_Output = new int [Tree.Number_Of_Nodes];
          
          //The output length will be lengthened for every node read into the buffer output.
          Output_Length = 0;
          
          //The Low_Charge is used to determine baseline charge necessary.
          //gather_Node(Tree.Root);
          //console_Move_Cursor_To_Given_X_And_Y(0, 5);
          
          //set_Console_Color(0, ((rand() % 7) + 1));
          
          gather_Nodes();
          
          //set_Console_Color(0, 10);
          
          sort_Output();
     }
     
     //Gathers the nodes non recursively.
     void gather_Nodes()
     {
          //cout << "\n  gather_Nodes() Number_Of_Nodes->" << Number_Of_Nodes;
          
          if (Tree.Number_Of_Nodes == 0){ return; }
          
          //This integer is used to track the current node in the pseudostack.
          int Node_EIP = 0;
          
          //This is used to determine whether or not a node has been gathered when moving back up the array.
          int * Node_State;
          Node_State = new int[Tree.Number_Of_Nodes];
          
          //This acts as a temporary stack.
          c_Torch_Buffer_Node_1D ** Node_Stack;
          Node_Stack = new c_Torch_Buffer_Node_1D*[Tree.Number_Of_Nodes];
          
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
               Charge_Output[Output_Length] = (Node_Stack[Node_EIP]->Charge - Tree.Low_Charge);
               Output_Length++;
          }
                         
          //cout << "\n\t     Node_EIP->" << Node_EIP << "srt_P->" << srt_Position << " srt_Data->" << srt_Data[srt_Position].I << " srt_NID->" << srt_NID[srt_Position];
          
          
          while (1)
          {
               if (Node_EIP < 0){ break; }
               
               //cout << "\n\t EIP->" << Node_EIP << " srt_P->" << srt_Position;
               //cout << "\n\t\t\t\t\t\t " << Node_EIP << " " << Output_Length << " NID:" << NID_Output[Output_Length - 1] << " C:" << Charge_Output[Output_Length - 1];
               
               
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
                              Charge_Output[Output_Length] = (Node_Stack[Node_EIP]->Charge - Tree.Low_Charge);
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
                              Charge_Output[Output_Length] = (Node_Stack[Node_EIP]->Charge - Tree.Low_Charge);
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
     
     //Gathers nodes recursively.
     void gather_Node(c_Torch_Buffer_Node_1D *&p_Node)
     {
          if (p_Node == NULL){ return; }
          
          //Get the lower node first.
          gather_Node(p_Node->Left);
          
          //If the node has sufficient charge then add it to the output.
          if (p_Node->Charge >= Tree.Low_Charge)
          {
               NID_Output[Output_Length] = p_Node->NID;
               Charge_Output[Output_Length] = (p_Node->Charge - Tree.Low_Charge);
               Output_Length++;
          }
          
          //Now for the higher node.
          gather_Node(p_Node->Right);
          
     }
     
     //Bubble sorts the output based on charge.
     void sort_Output()
     {
          c_Base_Node_1D * tmp_NID = NULL;
          int tmp_Charge = 0;;
          bool flg_Not_Done = 1;
          
          while (flg_Not_Done)
          {
               flg_Not_Done = 0;
               
               for (int cou_Index=0;cou_Index<(Output_Length - 1);cou_Index++)
               {
                    if (Charge_Output[cou_Index] < Charge_Output[cou_Index + 1])
                    {
                         tmp_NID = NID_Output[cou_Index];
                         tmp_Charge = Charge_Output[cou_Index];
                         
                         NID_Output[cou_Index] = NID_Output[cou_Index + 1];
                         Charge_Output[cou_Index] = Charge_Output[cou_Index + 1];
                         
                         NID_Output[cou_Index + 1] = tmp_NID;
                         Charge_Output[cou_Index + 1] = tmp_Charge;
                         
                         flg_Not_Done = 1;
                    }
               }
          }
          
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
          
          Output_Length = 0;
     }
     
     //Resets the buffer for new evaluation.
     void reset_Buffer()
     {
          reset_Tree();
          reset_Output();
     }
     
     //Output the output array as well as the tree.
     void output_Torch_Buffer()
     {
          cout << "\n\n\t----------";
          cout << "\n\t Torch_Buffer->" << this;
          Tree.output_Tree();
          output_Output();
     }
     
     //Output the output array.
     void output_Output()
     {
          cout << "\n\t output_Length->" << Output_Length;
          
          cout << "\n\t    [NID | Charge]-> ";
          for (int cou_Index=0;cou_Index<Output_Length;cou_Index++)
          {
               cout << "<" << NID_Output[cou_Index] << " | " << Charge_Output[cou_Index] << ">  ";
          }
          
     }

};
