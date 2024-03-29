//This is the linked list for the buffer.
class c_NT3_Vision_Linked_List
{
public:
     
     //The next node in the chain.
     c_NT3_Vision_Linked_List * Next;
     u_Data_3 Charge;
     u_Data_3 State;
     
     c_NT3_Vision_Linked_List()
     {
          Next = NULL;
          Charge.I = 0;
          State.I = 0;
     }
     
     ~c_NT3_Vision_Linked_List()
     {
          Charge.I = 0;
          State.I = 0;
     }
     
     //Outputs the linked list.
     void output_LL()
     {
          cout << " [";
          cout << State.C;
          cout << " $" << Charge.I << "]  ";
          if (Next != NULL){ Next->output_LL(); }
     }
};

//This handles the linked list creation and deletion so that recursive destruction does not crash the stack.
class c_NT3_Vision_Linked_List_Handler
{
public:
     
     //The nodes in the linked list.
     c_NT3_Vision_Linked_List * Root;
     c_NT3_Vision_Linked_List ** Current_LL;
     
     c_NT3_Vision_Linked_List_Handler()
     {
          Root = NULL;
          Current_LL = &Root;
     }
     
     ~c_NT3_Vision_Linked_List_Handler()
     {
          reset();
     }
     
     //Adds a node.
     c_NT3_Vision_Linked_List * new_LL(u_Data_3 p_State, long long int p_Charge)
     {
          c_NT3_Vision_Linked_List * tmp_LL = NULL;
          
          *Current_LL = new c_NT3_Vision_Linked_List;
          
          (*Current_LL)->Charge.I = p_Charge;
          (*Current_LL)->State = p_State;
          
          tmp_LL = *Current_LL;
          
          Current_LL = &(*Current_LL)->Next;
          
          return tmp_LL;
     }
     
     //Resets the linked list.
     void reset()
     {
          c_NT3_Vision_Linked_List * tmp_LL = Root;
          Root = NULL;
          c_NT3_Vision_Linked_List * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               tmp_LL->Next = NULL;
               delete tmp_LL;
               tmp_LL = NULL;
               tmp_LL = tmp_LL_Next;
          }
          
          Root = NULL;
          Current_LL = &Root;
     }
     
};



class c_NT3_Vision_Buffer_Node
{
     friend class c_NT3_Vision_Buffer_Tree; 
     private:
          c_NT3_Vision_Buffer_Node * Right;
          c_NT3_Vision_Buffer_Node * Center;
          c_NT3_Vision_Buffer_Node * Left;
          
     public:
          u_Data_3 Data;
          
          c_NT3_Vision_Linked_List * LL;
          
          c_NT3_Vision_Buffer_Node()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
               LL = NULL;
               Data.I = 0;
          }
          
          ~c_NT3_Vision_Buffer_Node()
          {
               LL = NULL;
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_NT3_Vision_Buffer_Tree
{
private:
             
             
     public:
          c_NT3_Vision_Buffer_Node * Root;
          c_NT3_Vision_Buffer_Node ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          //For handling the current search.
          u_Data_3 tmp_Data;
          u_Data_3 tmp_SData;
          
          c_NT3_Vision_Buffer_Tree()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_NT3_Vision_Buffer_Tree()
          {
               delete Root;
          }
          
          void search(u_Data_3 p_Data)
          {
               flg_Foundit = false;
               
               tmp_Data.U = 0;
               
               query_Node(Root, p_Data, 63);
          }
          
          int query_Node(c_NT3_Vision_Buffer_Node *&p_Node, u_Data_3 p_Data, int p_Shift)
          {
               tmp_Data.U = p_Data.U >> p_Shift;
               //cout << "\n\n\n tmp_Data.U = " << tmp_Data.U << " p_Shift = " << p_Shift;
               
               
               //cout << "\n\n\t __QN__ tmp_Data.U->" << tmp_Data.U << " p_Data.U->" << p_Data.U;
               
               if (p_Node == NULL)
               {      
                    //cout << "\n\t   Node is NULL";
                    p_Node = new c_NT3_Vision_Buffer_Node;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    if (p_Shift > 1)
                    {
                         //cout << "\n\t   tmp_Data.U > 1";
                         p_Node->Data = tmp_Data;
                    }
                    else 
                    {
                         //cout << "\n\t   tmp_Data.U !> 1";
                         p_Node->Data = p_Data;
                         Current = &p_Node; 
                         flg_Foundit = false;
                         return 0;
                    }
               }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.U == tmp_Data.U && p_Shift > 0)
               {
                    //cout << "\n\t   p_Node->Data.U (" << p_Node->Data.U << ") == tmp_Data.U (" << tmp_Data.U << ")";
                    
                    //cout << "\t tmp_Data.U = " << tmp_Data.U;
                    
                    query_Node(p_Node->Center, p_Data, (p_Shift - 4));
                    return 0;  
               }
               
               //If the node data matches the given data exactly the node has been found.
               if (p_Node->Data.U == p_Data.U)
               {
                    //cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
                    Current = &p_Node;  
                    flg_Foundit = true;
                    return 1;
               }
               
               
               if (tmp_Data.U < p_Node->Data.U)
               {
                    //cout << "\n\t    tmp_Data.U (" << tmp_Data.U << ") < p_Node->Data.U(" << p_Node->Data.U << ")";
                    query_Node(p_Node->Left, p_Data, p_Shift);
                    return 0;
               }
               if (tmp_Data.U > p_Node->Data.U)
               {
                    //cout << "\n\t    tmp_Data.U (" << tmp_Data.U << ") > p_Node->Data.U(" << p_Node->Data.U << ")";
                    query_Node(p_Node->Right, p_Data, p_Shift);
                    return 0;
               }
               return 0;
          }
          
          void output_Tree()
          {
               output_Node(Root, 0);
          }
          
          void output_Node(c_NT3_Vision_Buffer_Node *&p_Node, int p_Tab)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left, (p_Tab));
               cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    cout << "  ";
               }
               cout << (p_Node->Data).I;
               if (p_Node->LL != NULL){ cout << "->" << (p_Node->LL->State.C); }
               output_Node(p_Node->Center, (p_Tab + 1));
               output_Node(p_Node->Right, (p_Tab));
          }
          
          c_NT3_Vision_Linked_List * get_Node_LL(c_NT3_Vision_Buffer_Node * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->LL;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_LL(c_NT3_Vision_Buffer_Node * &p_Node, c_NT3_Vision_Linked_List * p_LL)
          {
               if (p_Node != NULL)
               {
                    p_Node->LL = p_LL;
               }
          }
          
          c_NT3_Vision_Linked_List * set_Current_Node_LL(c_NT3_Vision_Linked_List * p_LL)
          {
               set_Node_LL(*Current, p_LL);
               return p_LL;
          }
          
          c_NT3_Vision_Linked_List * get_Current_Node_LL()
          {
               return get_Node_LL(*Current);
          }
          
          
          //Resets the tree.
          void reset()
          {
               delete Root;
               Root = NULL;
               
               Current = NULL;
               flg_Foundit = false;
          }
                    
};



//The class that houses the charging buffer tree and the LL.
class c_NT3_Vision_Buffer
{
private:
     
public:
     
     //The search tree.
     c_NT3_Vision_Buffer_Tree Tree;
     
     //The linked lists.
     c_NT3_Vision_Linked_List_Handler Current_Charge;
     
     c_NT3_Vision_Linked_List_Handler Output;
     
     c_NT3_Vision_Linked_List_Handler Treetops;
     
     //Flags
     bool flg_Not_Done;
     
     int Input_Position;
          
     //Used for the gathering of the strongest state.
     u_Data_3 SState;
     u_Data_3 SCharge;
     
     c_NT3_Vision_Buffer()
     {
          Current_Charge.reset();
          
          Treetops.reset();
          
          Output.reset();
     }
     
     ~c_NT3_Vision_Buffer()
     {
          Current_Charge.reset();
          
          Treetops.reset();
          
          Output.reset();
     }
     
     //Accepts a submission of nodes to charge, used for CAN.
     void submit(u_Data_3 p_Node, long long int p_Charge = 10000)
     {
          //Search for the submitted node, if found add the charge.
          Tree.search(p_Node);
          
          if (Tree.flg_Foundit)
          {
               (Tree.get_Current_Node_LL())->Charge.I += p_Charge;
          }
          
          if (!Tree.flg_Foundit)
          {
               Tree.set_Current_Node_LL(Current_Charge.new_LL(p_Node, p_Charge));
          }
     }
     
     //Gathers the output nodes and resets the tree for the next iteration.
     void gather()
     {
          //Delete the old Output.
          Output.reset();
          
          //Reassign the linked lists.
          Output.Root = Current_Charge.Root;
          Current_Charge.Root = NULL;
          Current_Charge.reset();
          
          //Delete the tree.
          Tree.reset();
          
          //Finds the strongest state and charge.
          find_SState();
     }
     
     //Finds the strongest state in the output buffer.
     void find_SState()
     {
          c_NT3_Vision_Linked_List * tmp_LL = Output.Root;
          
          SState.I = 0;
          SCharge.I = 0;
          
          while(tmp_LL != NULL)
          {
               if (tmp_LL->Charge.I > SCharge.I)
               {
                    SState = tmp_LL->State;
                    SCharge = tmp_LL->Charge;
               }
               
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Outputs the buffer.
     void output_Buffer()
     {
          Tree.output_Tree();
          output_Charge_LL();
          output_Output_LL();
     }
     
     //Outputs the Current_Charge_LL.
     void output_Charge_LL()
     {
          cout << "\n\t Current_Charge_LL->";
          if (Current_Charge.Root == NULL){ cout << "empty..."; return; }
          Current_Charge.Root->output_LL();
     }
     
     //Outputs the Output_LL.
     void output_Output_LL()
     {
          cout << "\n\t Output_LL->";
          if (Output.Root == NULL){ cout << "empty..."; return; }
          Output.Root->output_LL();
     }
     
     //Outputs the Treetops.
     void output_Treetops()
     {
          cout << "\n\t Treetops_LL->";
          if (Treetops.Root == NULL){ cout << "empty..."; return; }
          Treetops.Root->output_LL();
     }
};