//This is the linked list for the buffer.
class c_NT3_Linked_List
{
public:
     
     //The next node in the chain.
     c_NT3_Linked_List * Next;
     c_NT3_Linked_List * Previous;
     u_Data_3 Charge;
     c_NT3_Base_Node_1D * NID;
     
     c_NT3_Linked_List()
     {
          Next = NULL;
          Previous = NULL;
          Charge.I = 0;
          NID = NULL;
     }
     
     ~c_NT3_Linked_List()
     {
          Charge.I = 0;
          if (NID != NULL){ NID->Ref = NULL; }
          NID = NULL;
          //delete Next; Handled by the handler.
     }
     
     //Outputs the linked list.
     //-- !WARNING WILL CAUSE STACK CORRUPTION ERROR WITH ENOUGH NODES!
     void output_LL()
     {
          //*cout << " [";
          if (NID != NULL){ NID->bp_O(); }
          //*cout << " $" << Charge.I << "]  ";
          if (Next != NULL){ Next->output_LL(); }
     }
};

//This handles the linked list creation and deletion so that recursive destruction does not crash the stack.
class c_NT3_Linked_List_Handler
{
public:
     
     //The nodes in the linked list.
     c_NT3_Linked_List * Root;
     c_NT3_Linked_List ** Current_LL;
     long long int Depth;
     
     c_NT3_Linked_List_Handler()
     {
          Root = NULL;
          Current_LL = &Root;
          Depth = 0;
     }
     
     ~c_NT3_Linked_List_Handler()
     {
          reset();
     }
     
     //Init
     void init()
     {
          Root = NULL;
          Current_LL = &Root;
          Depth = 0;
     }
     
     //Adds a node.
     c_NT3_Linked_List * new_LL(c_NT3_Base_Node_1D * p_NID, long long int p_Charge)
     {
          c_NT3_Linked_List * tmp_LL = NULL;
          
          *Current_LL = new c_NT3_Linked_List;
          
          (*Current_LL)->Charge.I = p_Charge;
          (*Current_LL)->NID = p_NID;
          
          tmp_LL = *Current_LL;
          
          (*Current_LL)->Previous = tmp_LL;
          
          Current_LL = &(*Current_LL)->Next;
          
          Depth++;
          
          return tmp_LL;
     }
     
     //Resets the linked list.
     void reset()
     {
          c_NT3_Linked_List * tmp_LL = Root;
          Root = NULL;
          c_NT3_Linked_List * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               tmp_LL->Next = NULL;
               tmp_LL->Previous = NULL;
               delete tmp_LL;
               tmp_LL = NULL;
               tmp_LL = tmp_LL_Next;
          }
          
          delete Root;
          Root = NULL;
          Current_LL = &Root;
          
          Depth = 0;
     }
     
     /*
     //Bubble sorts the linked list.
     void bubble_Sort()
     {
          //Tmp vars
          c_NT3_Linked_List * tmp_Prv = NULL;
          c_NT3_Linked_List * tmp_Next = NULL;
          c_NT3_Linked_List * tmp_Current = NULL;
          bool flg_Done = false;
          
          while(!flg_Done)
          {
               flg_Done = true;
               
               
          }
     }
      * */
};


class c_NT3_Buffer_Node
{
     friend class c_NT3_Buffer_Tree; 
     private:
          c_NT3_Buffer_Node * Right;
          c_NT3_Buffer_Node * Center;
          c_NT3_Buffer_Node * Left;
          
     public:
          u_Data_3 Data;
          
          c_NT3_Linked_List * LL;
          
          c_NT3_Buffer_Node()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
               LL = NULL;
               Data.I = 0;
          }
          
          ~c_NT3_Buffer_Node()
          {
               LL = NULL;
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_NT3_Buffer_Tree
{
private:
             
             
     public:
          c_NT3_Buffer_Node * Root;
          c_NT3_Buffer_Node ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          //For handling the current search.
          u_Data_3 tmp_Data;
          u_Data_3 tmp_SData;
          
          c_NT3_Buffer_Tree()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_NT3_Buffer_Tree()
          {
               delete Root;
          }
          
          void init()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          //Now returning whether or not the node was found as the foundit flag does not work with multiple threads.
          //This returns the node at the end of the chain, whether it was found or not does not matter the 
          //buffer node is returned and has to be checked for values.
          c_NT3_Buffer_Node * search(c_NT3_Base_Node_1D * p_Data)
          {
               flg_Foundit = false;
               
               if (p_Data == NULL){ Current = NULL; return NULL; }
               
               u_Data_3 tmp_SSData;
               tmp_SSData.NR = p_Data;
               
               
               
               //--cout << "\n\n SEARCHING ->" << tmp_SData.U;
               //system("PAUSE > NULL");
               return query_Node(Root, tmp_SSData, 31);
          }
          
          c_NT3_Buffer_Node * query_Node(c_NT3_Buffer_Node *&p_Node, u_Data_3 p_Data, int p_Shift)
          {
               //These are used because we have to be carefull not to leave Mutex locked with a return.
               c_NT3_Buffer_Node * tmp_Return = NULL;
               bool flg_Done = false;
               
               u_Data_3 tmp_DData;
               tmp_DData.U = p_Data.U;
               tmp_DData.U = tmp_DData.U >> p_Shift;
               //--cout << "\n\n\n tmp_DData.U = " << tmp_DData.U << " p_Shift = " << p_Shift;
               
               
               //--cout << "\n\n\t __QN__ tmp_DData.U->" << tmp_DData.U << " p_Data.U->" << p_Data.U;
               
               Mutex.lock();
               if (p_Node == NULL)
               {      
                    //--cout << "\n\t   Node is NULL";
                    p_Node = new c_NT3_Buffer_Node;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    if (p_Shift > 1)
                    {
                         //--cout << "\n\t   tmp_DData.U > 1";
                         p_Node->Data = tmp_DData;
                    }
                    else 
                    {
                         //--cout << "\n\t   tmp_DData.U !> 1";
                         p_Node->Data = p_Data;
                         Current = &p_Node; 
                         flg_Foundit = false;
                         tmp_Return = p_Node;
                         flg_Done = true;
                         //return 0;
                    }
               }
               Mutex.unlock();
               //If the node was not found then return NULL;
               if (flg_Done){ return tmp_Return; }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.U == tmp_DData.U && p_Shift > 0)
               {
                    //--cout << "\n\t   p_Node->Data.U (" << p_Node->Data.U << ") == tmp_DData.U (" << tmp_DData.U << ")";
                    
                    //--cout << "\t tmp_DData.U = " << tmp_DData.U;
                    
                    return query_Node(p_Node->Center, p_Data, (p_Shift - 1));
               }
               
               //If the node data matches the given data exactly the node has been found.
               //This is not locked because even if the Current & flg_Foundit are overwritten they will not be
               //used by the threads, but will work with the standard single thread.
               if (p_Node->Data.U == p_Data.U)
               {
                    //--cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
                    Current = &p_Node;  
                    flg_Foundit = true;
                    tmp_Return = p_Node;
                    flg_Done = true;
                    //return 1;
               }
               
               //If the node was found then return tmp_Return;
               if (flg_Done){ return tmp_Return; }
               
               if (tmp_DData.U < p_Node->Data.U)
               {
                    //--cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") < p_Node->Data.U(" << p_Node->Data.U << ")";
                    return query_Node(p_Node->Left, p_Data, p_Shift);
               }
               if (tmp_DData.U > p_Node->Data.U)
               {
                    //--cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") > p_Node->Data.U(" << p_Node->Data.U << ")";
                    return query_Node(p_Node->Right, p_Data, p_Shift);
               }
               return tmp_Return;
          }
          
          void output_Tree()
          {
               output_Node(Root, 0);
          }
          
          void output_Node(c_NT3_Buffer_Node *&p_Node, int p_Tab)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left, (p_Tab));
               //*cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    //*cout << "  ";
               }
               //*cout << (p_Node->Data).I;
               if (p_Node->LL != NULL){ cout << "->" << (p_Node->LL->NID); }
               output_Node(p_Node->Center, (p_Tab + 1));
               output_Node(p_Node->Right, (p_Tab));
          }
          
          c_NT3_Linked_List * get_Node_LL(c_NT3_Buffer_Node * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->LL;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_LL(c_NT3_Buffer_Node * &p_Node, c_NT3_Linked_List * p_LL)
          {
               if (p_Node != NULL)
               {
                    p_Node->LL = p_LL;
               }
          }
          
          c_NT3_Linked_List * set_Current_Node_LL(c_NT3_Linked_List * p_LL)
          {
               set_Node_LL(*Current, p_LL);
               return p_LL;
          }
          
          c_NT3_Linked_List * get_Current_Node_LL()
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
class c_NT3_Buffer
{
private:
     
public:
     
     //The search tree.
     c_NT3_Buffer_Tree Tree;
     
     //The linked lists.
     c_NT3_Linked_List_Handler Current_Charge;
     
     c_NT3_Linked_List_Handler Output;
     
     c_NT3_Linked_List_Handler Treetops;
     
     //For the OP tree.
     c_NT3_Linked_List_Handler * Current_Charge_Opp;
     
     c_NT3_Linked_List_Handler * Output_Opp;
     
     c_NT3_Buffer_Tree * Tree_Opp;
     
     long long int * Current_Highest_Charge_Opp;
     long long int * Previous_Highest_Charge_Opp;
     
     bool * Opp_Set;
     
     int Opp_Count;
     
     
     //Flags
     bool flg_Not_Done;
     
     int Input_Position;
     
     //Settings & trackers for the settings.
     long long int Base_Charge;
     long long int Current_Highest_Charge;
     long long int Previous_Highest_Charge;
     
     double Modifier_Charge;
     double Action_Potential_Threshold;
          
     //The threads for the construct.
     thread ** Threeads;
     
     c_NT3_Buffer()
     {
          flg_Not_Done = 0;
          Input_Position = 0;
          
          Current_Highest_Charge = 0;
          Previous_Highest_Charge = 0;
          Base_Charge = 0;
          Modifier_Charge = 0.0f;
          Action_Potential_Threshold = 0.0f;
          
          //For the OP tree.
          Current_Charge_Opp = NULL;
          Output_Opp = NULL;
          Tree_Opp = NULL;
          Opp_Set = NULL;
          Previous_Highest_Charge_Opp = NULL;
          Current_Highest_Charge_Opp = NULL;
          
          Opp_Count = 0;
          
          Threeads = NULL;
          init_Threeads();
     }
     
     ~c_NT3_Buffer()
     {
          for (int cou_Index=0;cou_Index<Opp_Count;cou_Index++)
          {
               
               //For the OP tree.
               Current_Charge_Opp[cou_Index].reset();
               Output_Opp[cou_Index].reset();
               Tree_Opp[cou_Index].reset();
               
               Opp_Set[cou_Index] = 0;
          }
          
          //For the OP tree.
          Current_Charge_Opp = NULL;
          Output_Opp = NULL;
          Tree_Opp = NULL;
          Opp_Set = NULL;
          
          Opp_Count = 0;
          
          reset_Threeads();
     }
     
     
     //=====--                    --=====//
     //==--     THREEAD HANDLING     --==//
     //=====--                    --=====//
     
     void init_Threeads()
     {
          Threeads = new thread*[Central_Core_Count];
          for (int cou_T=0;cou_T<Central_Core_Count;cou_T++)
          {
               Threeads[cou_T] = NULL;
          }
     }    
     
     void reset_Threeads()
     {
          if (Threeads == NULL){ return; }
          for (int cou_T=0;cou_T<Central_Core_Count;cou_T++)
          {
               delete Threeads[cou_T];
               Threeads[cou_T] = NULL;
          }
          delete [] Threeads;
          Threeads = NULL;
     }
     
     //=====--            --=====//
     //==--     CHARGING     --==//
     //=====--            --=====//
     
     
     //--  CHARGING FOR ORIGINAL ALGO, USED FOR MSC WHERE ALL PATTERNS ARE TO BE FOUND
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs(int p_Use_Spike)
     {
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                    //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                    //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_RC(int p_Use_Spike)
     {
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                    //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                    //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_L(int p_Use_Spike)
     {
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                    //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
          
          //Charge the opposing side.
          charge_All_Outputs_R_Opp();
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_R(int p_Use_Spike)
     {
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aR->" << tmp_LL->NID->Axon_Count_R;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_R[cou_A];
                    //*tmp_LL->NID->Axons_R[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               
               tmp_LL = tmp_LL->Next;
          }
          
          //Charge the opposing side.
          charge_All_Outputs_L_Opp();
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_RC_L(int p_Use_Spike)
     {
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                    //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
          
          //Charge the opposing side.
          charge_All_Outputs_RC_R_Opp();
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_RC_R(int p_Use_Spike)
     {
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aR->" << tmp_LL->NID->Axon_Count_R;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_R[cou_A];
                    //*tmp_LL->NID->Axons_R[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
          
          //Charge the opposing side.
          charge_All_Outputs_RC_L_Opp();
     }
     
     //--   CHARGING FOR THE OPP
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_L_Opp(int p_Opp)
     {
          c_NT3_Linked_List * tmp_LL = Output_Opp[p_Opp].Root;
          
          Previous_Highest_Charge_Opp[p_Opp] = Current_Highest_Charge; //Not needed as this will be called from the regular which will already have reset the charges
          Current_Highest_Charge_Opp[p_Opp] = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge_Opp[p_Opp]);
               tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge);
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge_Opp[p_Opp]) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit_Opp(p_Opp, tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                    //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges all of the outputs in Opp.
     void charge_All_Outputs_L_Opp()
     {
          for (int cou_Index=0;cou_Index<Opp_Count;cou_Index++)
          {
               charge_Outputs_L_Opp(cou_Index);
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_R_Opp(int p_Opp)
     {
          c_NT3_Linked_List * tmp_LL = Output_Opp[p_Opp].Root;
          
          Previous_Highest_Charge_Opp[p_Opp] = Current_Highest_Charge; //Not needed as this will be called from the regular which will already have reset the charges
          Current_Highest_Charge_Opp[p_Opp] = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge_Opp[p_Opp]);
               tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aR->" << tmp_LL->NID->Axon_Count_R;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit_Opp(p_Opp, tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_R[cou_A];
                    //*tmp_LL->NID->Axons_R[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges all of the outputs in Opp.
     void charge_All_Outputs_R_Opp()
     {
          for (int cou_Index=0;cou_Index<Opp_Count;cou_Index++)
          {
               charge_Outputs_R_Opp(cou_Index);
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_RC_L_Opp(int p_Opp)
     {
          c_NT3_Linked_List * tmp_LL = Output_Opp[p_Opp].Root;
          
          Previous_Highest_Charge_Opp[p_Opp] = Current_Highest_Charge; //Not needed as this will be called from the regular which will already have reset the charges
          Current_Highest_Charge_Opp[p_Opp] = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge_Opp[p_Opp]) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge_Opp[p_Opp]);
               tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge);
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge_Opp[p_Opp]) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit_RC_Opp(p_Opp, tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                    //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges all of the outputs in Opp.
     void charge_All_Outputs_RC_L_Opp()
     {
          for (int cou_Index=0;cou_Index<Opp_Count;cou_Index++)
          {
               charge_Outputs_RC_L_Opp(cou_Index);
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_RC_R_Opp(int p_Opp)
     {
          c_NT3_Linked_List * tmp_LL = Output_Opp[p_Opp].Root;
          
          Previous_Highest_Charge_Opp[p_Opp] = Current_Highest_Charge; //Not needed as this will be called from the regular which will already have reset the charges
          Current_Highest_Charge_Opp[p_Opp] = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge_Opp[p_Opp]) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge_Opp[p_Opp]);
               tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge);
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge_Opp[p_Opp]) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aR->" << tmp_LL->NID->Axon_Count_R;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit_RC_Opp(p_Opp, tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_R[cou_A];
                    //*tmp_LL->NID->Axons_R[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges all of the outputs in Opp.
     void charge_All_Outputs_RC_R_Opp()
     {
          for (int cou_Index=0;cou_Index<Opp_Count;cou_Index++)
          {
               charge_Outputs_RC_R_Opp(cou_Index);
          }
     }
     
     
     
     
     
     
     
     
     
     int run_With_It(int p_Core, long int p_Start, long int p_End)
     {
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          //Shift the current output to the proper start point.
          for (int cou_Index=0;cou_Index<p_Start;cou_Index++)
          {
               tmp_LL = tmp_LL->Next;
          }
          
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          for (int cou_LL=p_Start;cou_LL<p_End;cou_LL++)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge);
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit_MT(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit_MT(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Mutex.lock();
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
                    Mutex.unlock();
               }
               tmp_LL = tmp_LL->Next;
          }
          return 1;
     }
     
     //These are separated because they use the multithreaded approach
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_MT()
     {
          //--  GATHERING AND ASSIGNING SHIFT POSITIONS
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          //Assign the cores their running spaces.
          long int tmp_Shift_Count = Output.Depth / Central_Core_Count;
          if (tmp_Shift_Count == 0){ tmp_Shift_Count = 1; }
          long int tmp_Start = 0;
          long int tmp_End = 0;
          
          //--  THREEAD HANDLING
          
          Mutex.lock();
          //*cout << "\n\t Running threeads()";
          Mutex.unlock();
          
          reset_Threeads();
          init_Threeads();
          
          
          for (int cou_T=0;cou_T<Central_Core_Count;cou_T++)
          {
               if (Output.Depth >= cou_T){ continue; }
               
               //*Mutex.lock();
               //*cout << "\n\t cou_T->" << cou_T; cout.flush();
               
               //*cout << "\n\t B Threeads[" << cou_T << "]->" << Threeads[cou_T] << " ~>"; cout.flush();
               //*cout << "<~"; cout.flush();
                    
               //*Mutex.unlock();
               
               //Gather the core shift for the current core.
               tmp_Start = (cou_T * tmp_Shift_Count);
               tmp_End = ((cou_T + 1) * tmp_Shift_Count);
               if (cou_T == (Central_Core_Count - 1)){ tmp_End = Output.Depth; }
               
               
               //Insert function here.
               //run_With_It(cou_T, tmp_Start, tmp_End);
               Threeads[cou_T] = new std::thread(&c_NT3_Buffer::run_With_It, this, cou_T, tmp_Start, tmp_End);
               
               
               //*Mutex.lock();
               //*cout << "\n\t A Threeads[" << cou_T << "]->" << Threeads[cou_T] << " ~>"; cout.flush();
               //*cout << "<~"; cout.flush();
               //*Mutex.unlock();
               //charge_Charging_Buffer_Stack(cou_T);
          }

          bool flg_Done_Threads = false;
          
          while (!flg_Done_Threads)
          {
               flg_Done_Threads = true;
               for (int cou_T=0;cou_T<Central_Core_Count;cou_T++)
               {
                    if (Threeads[cou_T] == NULL){ continue; }
                    //*cout << "\n\t Threeads[" << cou_T << "]->" << Threeads[cou_T] << " ~>"; cout.flush();
                    //*cout << Threeads[cou_T]->joinable();
                    //*cout << "<~"; cout.flush();
                    if (Threeads[cou_T]->joinable())
                    {
                         //*cout << Threeads[cou_T]->joinable(); cout.flush();
                         
                         if (std::this_thread::get_id() == Threeads[cou_T]->get_id())
                         {
                              //*cout << "\n\t DEADLOCK DETECTED!";
                              break;
                         }
                         
                         //*cout << "\n\t Trying to join()"; cout.flush();
                              Threeads[cou_T]->join();
                        
                         //*cout << Threeads[cou_T]->joinable(); cout.flush();
                         
                         flg_Done_Threads = false;
                    }
                    //*cout << "\n\t Join Complete..."; cout.flush();
               }
          }
          
     }
     
     int run_With_It_RC(int p_Core, long int p_Start, long int p_End)
     {
          
          c_NT3_Linked_List * tmp_LL = Output.Root;
          //*c_NT3_Linked_List * tmp_sLL;
          
          //Shift the current output to the proper start point.
          for (int cou_Index=0;cou_Index<p_Start;cou_Index++)
          {
               tmp_LL = tmp_LL->Next;
          }
          //*tmp_sLL = tmp_LL;
          
          
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          for (int cou_LL=p_Start;cou_LL<p_End;cou_LL++)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge);
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit_RC_MT(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit_RC_MT(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Mutex.lock();
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
                    Mutex.unlock();
               }
               tmp_LL = tmp_LL->Next;
          }
          /*
          Mutex.lock();
          cout << "\n run_With_It_RC(" << p_Core << ", " << p_Start << ", " << p_End << ") sLL->" << tmp_sLL << " LL->" << tmp_LL;
          if (tmp_LL != NULL){ cout << ".Next->" << tmp_LL->Next << "                                            "; }
          Mutex.unlock();
          */
          return 1;
     }
     
     //These are separated because they use the multithreaded approach
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_RC_MT()
     {
          //system("CLS");
          
          //--  GATHERING AND ASSIGNING SHIFT POSITIONS
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          //Assign the cores their running spaces.
          long int tmp_Shift_Count = Output.Depth / Central_Core_Count;
          long int tmp_Start = 0;
          long int tmp_End = 0;
          
          //--  THREEAD HANDLING
          
          Mutex.lock();
          cout << "\n\t Running threeads()";
          cout << "\n\n Output.Depth = " << Output.Depth << " Core_Count = " << Central_Core_Count;
          Mutex.unlock();
          
          reset_Threeads();
          init_Threeads();
          
          
          for (int cou_T=0;cou_T<Central_Core_Count;cou_T++)
          {
               /*--*/Mutex.lock();
               /*--*/cout << "\n\t cou_T->" << cou_T; cout.flush();
               
               /*--*/cout << "\n\t B Threeads[" << cou_T << "]->" << Threeads[cou_T] << " ~>"; cout.flush();
               /*--*/cout << "<~"; cout.flush();
                    
               /*--*/Mutex.unlock();
               
               //Gather the core shift for the current core.
               tmp_Start = (cou_T * tmp_Shift_Count);
               tmp_End = ((cou_T + 1) * tmp_Shift_Count);
               if (cou_T == (Central_Core_Count - 1)){ tmp_End = Output.Depth; }
               
               
               //Insert function here.
               //run_With_It(cou_T, tmp_Start, tmp_End);
               Threeads[cou_T] = new std::thread(&c_NT3_Buffer::run_With_It_RC, this, cou_T, tmp_Start, tmp_End);
               
               
               /*--*/Mutex.lock();
               /*--*/cout << "\n\t A Threeads[" << cou_T << "]->" << Threeads[cou_T] << " ~>"; cout.flush();
               /*--*/cout << "<~"; cout.flush();
               /*--*/Mutex.unlock();
               //charge_Charging_Buffer_Stack(cou_T);
          }

          bool flg_Done_Threads = false;
          
          
          while (!flg_Done_Threads)
          {
               flg_Done_Threads = true;
               for (int cou_T=0;cou_T<Central_Core_Count;cou_T++)
               {
                    if (Threeads[cou_T] == NULL){ continue; }
                    /*--*/cout << "\n\t Threeads[" << cou_T << "]->" << Threeads[cou_T] << " ~>"; cout.flush();
                    /*--*/cout << Threeads[cou_T]->joinable();
                    /*--*/cout << "<~"; cout.flush();
                    
                    if (Threeads[cou_T]->joinable())
                    {
                         /*--*/cout << Threeads[cou_T]->joinable(); cout.flush();
                         
                         if (std::this_thread::get_id() == Threeads[cou_T]->get_id())
                         {
                              /*--*/cout << "\n\t DEADLOCK DETECTED!";
                              break;
                         }
                         
                         /*--*/cout << "\n\t Trying to join()"; cout.flush();
                              Threeads[cou_T]->join();
                        
                         /*--*/cout << Threeads[cou_T]->joinable(); cout.flush();
                         
                         flg_Done_Threads = false;
                    }
                    /*--*/cout << "\n\t Join Complete..."; cout.flush();
               }
          }
     }
     
     
     //=====--              --=====//
     //==--     SUBMISSION     --==//
     //=====--              --=====//
     
     //Accepts a submission of nodes to charge, used for CAN.
     void submit(c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          //Search for the submitted node, if found add the charge.
          if ((c_NT3_Linked_List*) p_Node->Ref != NULL)
          {
               ((c_NT3_Linked_List*) p_Node->Ref)->Charge.I += p_Charge;
               if (((c_NT3_Linked_List*) p_Node->Ref)->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = ((c_NT3_Linked_List*) p_Node->Ref)->Charge.I; }
          }
          
          if ((c_NT3_Linked_List*) p_Node->Ref == NULL && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               p_Node->Ref = Current_Charge.new_LL(p_Node, p_Charge);
          }
     }
          
     //Accepts a submission of nodes to charge, used for CAN.
     void submit_RC(c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          
          //Search for the submitted node, if found add the charge.
          if ((c_NT3_Linked_List*) p_Node->Ref != NULL)
          {
               ((c_NT3_Linked_List*) p_Node->Ref)->Charge.I += (p_Charge * ((c_NT3_Linked_List*) p_Node->Ref)->NID->get_RC_Score());
               if (((c_NT3_Linked_List*) p_Node->Ref)->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = ((c_NT3_Linked_List*) p_Node->Ref)->Charge.I; }
          }
          
          if (((c_NT3_Linked_List*) p_Node->Ref) == NULL && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               p_Node->Ref = Current_Charge.new_LL(p_Node, (p_Node->get_RC_Score() * p_Charge));
          }
     }
     
     //Accepts a submission of nodes to charge, used for CAN.
     void Old_submit(c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          //Search for the submitted node, if found add the charge.
          Tree.search(p_Node);
          
          if (Tree.flg_Foundit)
          {
               (Tree.get_Current_Node_LL())->Charge.I += p_Charge;
               if ((Tree.get_Current_Node_LL())->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = (Tree.get_Current_Node_LL())->Charge.I; }
          }
          
          if (!Tree.flg_Foundit && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               Tree.set_Current_Node_LL(Current_Charge.new_LL(p_Node, p_Charge));
          }
     }
          
     //Accepts a submission of nodes to charge, used for CAN.
     void Old_submit_RC(c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          
          //Search for the submitted node, if found add the charge.
          Tree.search(p_Node);
          
          if (Tree.flg_Foundit)
          {
               (Tree.get_Current_Node_LL())->Charge.I += (p_Charge * (Tree.get_Current_Node_LL())->NID->get_RC_Score());
               if ((Tree.get_Current_Node_LL())->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = (Tree.get_Current_Node_LL())->Charge.I; }
          }
          
          if (!Tree.flg_Foundit && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               Tree.set_Current_Node_LL(Current_Charge.new_LL(p_Node, p_Charge));
          }
     }
     
     
     //--  SUBMISSION FOR MT
     
     //Accepts a submission of nodes to charge, used for CAN.
     void submit_MT(c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          //Search for the submitted node, if found add the charge.
          c_NT3_Buffer_Node * tmp_Current_Node = Tree.search(p_Node);
          
          Mutex.lock();
          if (tmp_Current_Node->LL != NULL)
          {
               (tmp_Current_Node->LL)->Charge.I += p_Charge;
               if ((tmp_Current_Node->LL)->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = (tmp_Current_Node->LL)->Charge.I; }
          }
          
          if ((tmp_Current_Node->LL == NULL) && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               tmp_Current_Node->LL = (Current_Charge.new_LL(p_Node, p_Charge));
          }
          Mutex.unlock();
     }
          
     //Accepts a submission of nodes to charge, used for CAN.
     void submit_RC_MT(c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          
          //Search for the submitted node, if found add the charge.
          c_NT3_Buffer_Node * tmp_Current_Node = Tree.search(p_Node);
          
          Mutex.lock();
          if (tmp_Current_Node->LL != NULL)
          {
               (tmp_Current_Node->LL)->Charge.I += (p_Charge * (tmp_Current_Node->LL)->NID->get_RC_Score());
               if ((tmp_Current_Node->LL)->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = (tmp_Current_Node->LL)->Charge.I; }
          }
          
          if ((tmp_Current_Node->LL == NULL) && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               tmp_Current_Node->LL = (Current_Charge.new_LL(p_Node, p_Charge));
          }
          Mutex.unlock();
     }
     
     
     //--  SUBMISSION FOR OPP
     
     //Accepts a submission of nodes to charge, used for CAN.
     void submit_Opp(int p_Opp, c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          Opp_Set[p_Opp] = 1;
          
          //Search for the submitted node, if found add the charge.
          Tree_Opp[p_Opp].search(p_Node);
          
          if (Tree_Opp[p_Opp].flg_Foundit)
          {
               (Tree_Opp[p_Opp].get_Current_Node_LL())->Charge.I += p_Charge;
               if ((Tree_Opp[p_Opp].get_Current_Node_LL())->Charge.I > Current_Highest_Charge_Opp[p_Opp]){ Current_Highest_Charge_Opp[p_Opp] = (Tree_Opp[p_Opp].get_Current_Node_LL())->Charge.I; }
          }
          
          if (!Tree_Opp[p_Opp].flg_Foundit && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge_Opp[p_Opp]){ Current_Highest_Charge_Opp[p_Opp] = p_Charge; }
               
               Tree_Opp[p_Opp].set_Current_Node_LL(Current_Charge.new_LL(p_Node, p_Charge));
          }
     }
          
     //Accepts a submission of nodes to charge, used for CAN.
     void submit_RC_Opp(int p_Opp, c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          
          //Search for the submitted node, if found add the charge.
          Tree_Opp[p_Opp].search(p_Node);
          
          if (Tree_Opp[p_Opp].flg_Foundit)
          {
               (Tree_Opp[p_Opp].get_Current_Node_LL())->Charge.I += (p_Charge * (Tree_Opp[p_Opp].get_Current_Node_LL())->NID->get_RC_Score());
               if ((Tree_Opp[p_Opp].get_Current_Node_LL())->Charge.I > Current_Highest_Charge_Opp[p_Opp]){ Current_Highest_Charge_Opp[p_Opp] = (Tree_Opp[p_Opp].get_Current_Node_LL())->Charge.I; }
          }
          
          if (!Tree_Opp[p_Opp].flg_Foundit && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge_Opp[p_Opp]){ Current_Highest_Charge_Opp[p_Opp] = p_Charge; }
               
               Tree_Opp[p_Opp].set_Current_Node_LL(Current_Charge.new_LL(p_Node, p_Charge));
          }
     }
     
     
     //=====--             --=====//
     //==--     GATHERING     --==//
     //=====--             --=====//
     
     //For gathering the Opp
     //>Target Tier - 1
     //>charge
     //>gather
     //>flush(Target_Tier)
     
     //Gathers the output nodes and resets the tree for the next iteration.
     void gather()
     {
          //Delete the old Output.
          Output.reset();
          
          //Reassign the linked lists.
          Output.Root = Current_Charge.Root;
          Output.Depth = Current_Charge.Depth;
          
          Current_Charge.Root = NULL;
          Current_Charge.reset();
          
          //Delete the tree.
          Tree.reset();
          
          //Sets the done flag if Output == NULL.
          flg_Not_Done = 1;
          if ((Output.Root == NULL)){ flg_Not_Done = 0; }
          
          //Gathers all of the Opp
          for (int cou_Index=0;cou_Index<Opp_Count;cou_Index++)
          {
               Output_Opp[cou_Index].reset();
               
               Output_Opp[cou_Index].Root = Current_Charge_Opp[cou_Index].Root;
               Current_Charge_Opp[cou_Index].Root = NULL;
               Current_Charge_Opp[cou_Index].reset();
               
               Tree_Opp[cou_Index].reset();
               
               if ((Output_Opp[cou_Index].Root != NULL)){ flg_Not_Done = 1; }
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void gather_Treetops()
     {
          c_NT3_Linked_List * tmp_LL = Treetops.Root;
          c_NT3_Linked_List * tmp_LL_Root = Treetops.Root;
          
          long long int tmp_Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          
          //Find the current highest charge.
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               if (tmp_Current_Highest_Charge < tmp_LL->Charge.I){ tmp_Current_Highest_Charge = tmp_LL->Charge.I; }
               
               tmp_LL = tmp_LL->Next;    
          }
          
          tmp_LL = Treetops.Root;
          Treetops.Root = NULL;
          Treetops.reset();
          
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / tmp_Current_Highest_Charge) * Base_Charge);
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               Treetops.new_LL(tmp_LL->NID, (tmp_Charge));
               
               tmp_LL = tmp_LL->Next;
          }
          
          delete tmp_LL_Root;
     }
     
     //Transfers all nodes from the Opp tree to the Vanilla Tree.
     void flush(int p_Opp)
     {
          if (p_Opp >= Opp_Count){ return; }
          
          c_NT3_Linked_List * tmp_LL = Output_Opp[p_Opp].Root;
          
          //Previous_Highest_Charge = Current_Highest_Charge; //Not needed as this will be called from the regular which will already have reset the charges
          //Current_Highest_Charge = 0;
          
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //Append the current node onto the end of the Current_Charge Linked list, may cause duplicates but would be more expensive to search for them.
               Current_Charge.new_LL(tmp_LL->NID, tmp_LL->Charge.I);
               
               tmp_LL = tmp_LL->Next;
          }
          
          Output_Opp[p_Opp].reset();
          Opp_Set[p_Opp] = 0;
     }
     
     //=====--                       --=====//
     //==--     CLEANUP & ALLOCATION    --==//
     //=====--                       --=====//
     
     
     //Destroys the output buffers.
     void destroy_Output_Buffers()
     {
          //Delete the old Output.
          Output.reset();    
          
          for (int cou_Index=0;cou_Index<Opp_Count;cou_Index++)
          {
               if (Opp_Set[cou_Index])
               {
                    //For the OP tree.
                    Output_Opp[cou_Index].reset();
               }
          }
     }
     
     //Destroys the _Opp.
     void destroy_Opp()
     {
          for (int cou_Index=0;cou_Index<Opp_Count;cou_Index++)
          {
               
               //For the OP tree.
               Current_Charge_Opp[cou_Index].reset();
               Output_Opp[cou_Index].reset();
               Tree_Opp[cou_Index].reset();
                    
               Opp_Set[cou_Index] = 0;
               Current_Highest_Charge_Opp[cou_Index] = 0;
               Previous_Highest_Charge_Opp[cou_Index] = 0;
          }
          
          //For the OP tree.
          delete [] Current_Charge_Opp; Current_Charge_Opp = NULL;
          delete [] Output_Opp; Output_Opp = NULL;
          delete [] Tree_Opp; Tree_Opp = NULL;
          delete [] Opp_Set; Opp_Set = NULL;
          delete [] Current_Highest_Charge_Opp; Current_Highest_Charge_Opp = NULL;
          delete [] Previous_Highest_Charge_Opp; Previous_Highest_Charge_Opp = NULL;
          
          Opp_Count = 0;
     }
     
     //Expands the Opp.
     void resize_Opp(int p_Size)
     {
          if (p_Size <= Opp_Count){ return; }
          
          //No keeping the old ones around.
          destroy_Opp();
          
          //For the OP tree.
          Current_Charge_Opp = new c_NT3_Linked_List_Handler[p_Size];
          Output_Opp = new c_NT3_Linked_List_Handler[p_Size];
          Tree_Opp = new c_NT3_Buffer_Tree[p_Size];
          Opp_Set = new bool[p_Size];
          Current_Highest_Charge_Opp = new long long int[p_Size];
          Previous_Highest_Charge_Opp = new long long int[p_Size];
          
          Opp_Count = p_Size;
          
          for (int cou_Index=0;cou_Index<Opp_Count;cou_Index++)
          {
               //For the OP tree.
               Current_Charge_Opp[cou_Index].init();
               Output_Opp[cou_Index].init();
               Tree_Opp[cou_Index].init();
               
               Current_Highest_Charge_Opp[cou_Index] = 0;
               Previous_Highest_Charge_Opp[cou_Index] = 0;
               Opp_Set[cou_Index] = 0;
          }
     }
     
     //=====--          --=====//
     //==--     OUTPUT     --==//
     //=====--          --=====//
     
     //Outputs all of the buffers.
     void output_All_Buffers()
     {
          //system("CLS");
          cout << "\n Charge_LL->" << Current_Charge.Depth << " ->";
          output_Charge_LL();
          cout << "\n Output_LL->" << Output.Depth << " ->";
          output_Output_LL();
          for (int cou_Index=0;cou_Index<Opp_Count;cou_Index++)
          {
               cout << "\n  Opp[" << cou_Index << "]";
               if (!Opp_Set[cou_Index]){ continue; }
               output_Opp_Charge_LL(cou_Index);
               output_Opp_Output_LL(cou_Index);
          }
          output_Treetops();
     }
          
     //Outputs the Current_Charge_LL.
     void output_Opp_Charge_LL(int p_Opp)
     {
          if (Current_Charge_Opp[p_Opp].Root == NULL){ return; }
          //*cout << "\n\t Current_Opp_Charge_LL[" << p_Opp << "]->";
          Current_Charge_Opp[p_Opp].Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
     }
     
     //Outputs the Output_LL.
     void output_Opp_Output_LL(int p_Opp)
     {
          if (Output_Opp[p_Opp].Root == NULL){ return; }
          //*cout << "\n\t Output_Opp_LL[" << p_Opp << "]->";
          Output_Opp[p_Opp].Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
     }
     
     //Outputs the buffer.
     void output_Buffer()
     {
          Tree.output_Tree();
          output_Charge_LL();
          output_Output_LL();
          output_Treetops();
     }
     
     //Outputs the Current_Charge_LL.
     void output_Charge_LL()
     {
          //*cout << "\n\t Current_Charge_LL->";
          if (Current_Charge.Root == NULL){ cout << "empty..."; return; }
          Current_Charge.Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
     }
     
     //Outputs the Output_LL.
     void output_Output_LL()
     {
          //*cout << "\n\t Output_LL->";
          if (Output.Root == NULL){ cout << "empty..."; return; }
          Output.Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
     }
     
     //Outputs the Treetops.
     void output_Treetops()
     {
          //*cout << "\n\t Treetops_LL->";
          if (Treetops.Root == NULL){ cout << "empty..."; return; }
          Treetops.Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
     }
};