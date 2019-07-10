//This is the linked list for the buffer.
class c_NT3_Linked_List_NonStrict
{
public:
     
     //The next node in the chain.
     c_NT3_Linked_List_NonStrict * Next;
     c_NT3_Linked_List_NonStrict * Previous;
     u_Data_3 Charge;
     c_NT3_Base_Node_NonStrict * NID;
     
     c_NT3_Linked_List_NonStrict()
     {
          Next = NULL;
          Previous = NULL;
          Charge.I = 0;
          NID = NULL;
     }
     
     ~c_NT3_Linked_List_NonStrict()
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
class c_NT3_Linked_List_NonStrict_Handler
{
public:
     
     //The nodes in the linked list.
     c_NT3_Linked_List_NonStrict * Root;
     c_NT3_Linked_List_NonStrict ** Current_LL;
     long long int Depth;
     
     c_NT3_Linked_List_NonStrict_Handler()
     {
          Root = NULL;
          Current_LL = &Root;
          Depth = 0;
     }
     
     ~c_NT3_Linked_List_NonStrict_Handler()
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
     c_NT3_Linked_List_NonStrict * new_LL(c_NT3_Base_Node_NonStrict * p_NID, long long int p_Charge)
     {
          c_NT3_Linked_List_NonStrict * tmp_LL = NULL;
          
          *Current_LL = new c_NT3_Linked_List_NonStrict;
          
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
          c_NT3_Linked_List_NonStrict * tmp_LL = Root;
          Root = NULL;
          c_NT3_Linked_List_NonStrict * tmp_LL_Next = NULL;
          
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
          c_NT3_Linked_List_NonStrict * tmp_Prv = NULL;
          c_NT3_Linked_List_NonStrict * tmp_Next = NULL;
          c_NT3_Linked_List_NonStrict * tmp_Current = NULL;
          bool flg_Done = false;
          
          while(!flg_Done)
          {
               flg_Done = true;
               
               
          }
     }
      * */
};


class c_NT3_Buffer_NonStrict_Node_NonStrict
{
     friend class c_NT3_Buffer_NonStrict_Tree_NonStrict; 
     private:
          c_NT3_Buffer_NonStrict_Node_NonStrict * Right;
          c_NT3_Buffer_NonStrict_Node_NonStrict * Center;
          c_NT3_Buffer_NonStrict_Node_NonStrict * Left;
          
     public:
          u_Data_3 Data;
          
          c_NT3_Linked_List_NonStrict * LL;
          
          c_NT3_Buffer_NonStrict_Node_NonStrict()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
               LL = NULL;
               Data.I = 0;
          }
          
          ~c_NT3_Buffer_NonStrict_Node_NonStrict()
          {
               LL = NULL;
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_NT3_Buffer_NonStrict_Tree_NonStrict
{
private:
             
             
     public:
          c_NT3_Buffer_NonStrict_Node_NonStrict * Root;
          c_NT3_Buffer_NonStrict_Node_NonStrict ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          //For handling the current search.
          u_Data_3 tmp_Data;
          u_Data_3 tmp_SData;
          
          c_NT3_Buffer_NonStrict_Tree_NonStrict()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_NT3_Buffer_NonStrict_Tree_NonStrict()
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
          c_NT3_Buffer_NonStrict_Node_NonStrict * search(c_NT3_Base_Node_NonStrict * p_Data)
          {
               flg_Foundit = false;
               
               if (p_Data == NULL){ Current = NULL; return NULL; }
               
               u_Data_3 tmp_SSData;
               tmp_SSData.NR = p_Data;
               
               
               
               //--cout << "\n\n SEARCHING ->" << tmp_SData.U;
               //system("PAUSE > NULL");
               return query_Node(Root, tmp_SSData, 31);
          }
          
          c_NT3_Buffer_NonStrict_Node_NonStrict * query_Node(c_NT3_Buffer_NonStrict_Node_NonStrict *&p_Node, u_Data_3 p_Data, int p_Shift)
          {
               //These are used because we have to be carefull not to leave Mutex locked with a return.
               c_NT3_Buffer_NonStrict_Node_NonStrict * tmp_Return = NULL;
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
                    p_Node = new c_NT3_Buffer_NonStrict_Node_NonStrict;
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
          
          void output_Node(c_NT3_Buffer_NonStrict_Node_NonStrict *&p_Node, int p_Tab)
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
          
          c_NT3_Linked_List_NonStrict * get_Node_LL(c_NT3_Buffer_NonStrict_Node_NonStrict * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->LL;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_LL(c_NT3_Buffer_NonStrict_Node_NonStrict * &p_Node, c_NT3_Linked_List_NonStrict * p_LL)
          {
               if (p_Node != NULL)
               {
                    p_Node->LL = p_LL;
               }
          }
          
          c_NT3_Linked_List_NonStrict * set_Current_Node_LL(c_NT3_Linked_List_NonStrict * p_LL)
          {
               set_Node_LL(*Current, p_LL);
               return p_LL;
          }
          
          c_NT3_Linked_List_NonStrict * get_Current_Node_LL()
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
          
          //Resets the tree destroying the nodes as it does so. Iterates rather than recursive.
          //The tree searches recursively so hopefully a recursive destruction will be OK.
          //void reset()
          //{
               //Tracks the previous node.
               
               //Current node.
               
               //Finished tracker.
               
               //If the node has a left leg follow it.
               
               //If the node has a middle leg follow it.
               
               //If the node has a right leg then follow it.
               
               //If the node has no legs then go back to the previous node and delete the current one.
               
          //}
};



//The class that houses the charging buffer tree and the LL.
class c_NT3_Buffer_NonStrict
{
private:
     
public:
     
     //The search tree.
     c_NT3_Buffer_NonStrict_Tree_NonStrict Tree;
     
     //The linked lists.
     c_NT3_Linked_List_NonStrict_Handler Current_Charge;
     
     c_NT3_Linked_List_NonStrict_Handler Output;
     
     c_NT3_Linked_List_NonStrict_Handler Treetops;
     
     //Flags
     bool flg_Not_Done;
     
     int Input_Position;
     
     //Settings & trackers for the settings.
     long long int Base_Charge;
     long long int Current_Highest_Charge;
     long long int Previous_Highest_Charge;
     
     double Modifier_Charge;
     double Action_Potential_Threshold;
          
     c_NT3_Buffer_NonStrict()
     {
          flg_Not_Done = 0;
          Input_Position = 0;
          
          Current_Highest_Charge = 0;
          Previous_Highest_Charge = 0;
          Base_Charge = 0;
          Modifier_Charge = 0.0f;
          Action_Potential_Threshold = 0.0f;
     }
     
     ~c_NT3_Buffer_NonStrict()
     {
          Tree.reset();
          Current_Charge.reset();
          Output.reset();
          Treetops.reset();
     }
     
     //=====--            --=====//
     //==--     CHARGING     --==//
     //=====--            --=====//
     
     
     //--  CHARGING FOR ORIGINAL ALGO, USED FOR MSC WHERE ALL PATTERNS ARE TO BE FOUND
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs(int p_Use_Spike)
     {
          c_NT3_Linked_List_NonStrict * tmp_LL = Output.Root;
          
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
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count;cou_A++)
               {
                    submit(tmp_LL->NID->Axons[cou_A], tmp_Charge);
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
          c_NT3_Linked_List_NonStrict * tmp_LL = Output.Root;
          
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
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons[cou_A], tmp_Charge);
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
     
     
     //=====--              --=====//
     //==--     SUBMISSION     --==//
     //=====--              --=====//
     
     //Accepts a submission of nodes to charge, used for CAN.
     void submit(c_NT3_Base_Node_NonStrict * p_Node, long long int p_Charge = 10000)
     {
          //Search for the submitted node, if found add the charge.
          if ((c_NT3_Linked_List_NonStrict*) p_Node->Ref != NULL)
          {
               ((c_NT3_Linked_List_NonStrict*) p_Node->Ref)->Charge.I += p_Charge;
               if (((c_NT3_Linked_List_NonStrict*) p_Node->Ref)->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = ((c_NT3_Linked_List_NonStrict*) p_Node->Ref)->Charge.I; }
          }
          
          if ((c_NT3_Linked_List_NonStrict*) p_Node->Ref == NULL && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               p_Node->Ref = Current_Charge.new_LL(p_Node, p_Charge);
          }
     }
          
     //Accepts a submission of nodes to charge, used for CAN.
     void submit_RC(c_NT3_Base_Node_NonStrict * p_Node, long long int p_Charge = 10000)
     {
          
          //Search for the submitted node, if found add the charge.
          if ((c_NT3_Linked_List_NonStrict*) p_Node->Ref != NULL)
          {
               ((c_NT3_Linked_List_NonStrict*) p_Node->Ref)->Charge.I += (p_Charge * ((c_NT3_Linked_List_NonStrict*) p_Node->Ref)->NID->get_RC_Score());
               
               if (((c_NT3_Linked_List_NonStrict*) p_Node->Ref)->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = ((c_NT3_Linked_List_NonStrict*) p_Node->Ref)->Charge.I; }
          }
          
          if (((c_NT3_Linked_List_NonStrict*) p_Node->Ref) == NULL && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               p_Node->Ref = Current_Charge.new_LL(p_Node, (p_Node->get_RC_Score() * p_Charge));
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
          
     }
     
     //Charges the outputs back into the charging buffer.
     void gather_Treetops()
     {
          c_NT3_Linked_List_NonStrict * tmp_LL = Treetops.Root;
          c_NT3_Linked_List_NonStrict * tmp_LL_Root = Treetops.Root;
          
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
     
     //=====--                       --=====//
     //==--     CLEANUP & ALLOCATION    --==//
     //=====--                       --=====//
     
     
     //Destroys the output buffers.
     void destroy_Output_Buffers()
     {
          //Delete the old Output.
          Output.reset();    
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
          output_Treetops();
     }
     
     //Outputs the buffer.
     void output_Buffer()
     {
          Tree.output_Tree();
          output_Charge_LL();
          output_Output_LL();
          output_Treetops();
     }
     
     //Outputs the buffer.
     void output_Buffers()
     {
          cout << " Charge->";
          output_Charge_LL();
          cout << " Output->";
          output_Output_LL();
          cout << " TTops->";
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