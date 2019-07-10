
class c_NT3_Fractal_State_Node_2D
{
     friend class c_NT3_Fractal_State_Tree_2D; 
     private:
          c_NT3_Fractal_State_Node_2D * Right;
          c_NT3_Fractal_State_Node_2D * Center;
          c_NT3_Fractal_State_Node_2D * Left;
          
     public:
          u_Data_3 Data;
          c_NT3_Base_Node_2D * NID;
          
          c_NT3_Fractal_State_Node_2D()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
               Data.I = 0;
          }
          
          ~c_NT3_Fractal_State_Node_2D()
          {
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_NT3_Fractal_State_Tree_2D
{
private:
             
             
     public:
          c_NT3_Fractal_State_Node_2D * Root;
          c_NT3_Fractal_State_Node_2D ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          u_Data_3 tmp_Data;
          
          c_NT3_Fractal_State_Tree_2D()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
               
               tmp_Data.U = 0;
          }
          
          ~c_NT3_Fractal_State_Tree_2D()
          {
               delete Root;
          }
          
          void reset()
          {
               delete Root;
               Root = NULL;
          }
          
          void search(u_Data_3 p_Data)
          {
               flg_Foundit = false;
               
               tmp_Data.U = 0;
               
               query_Node(Root, p_Data, 63);
          }
          
          int query_Node(c_NT3_Fractal_State_Node_2D *&p_Node, u_Data_3 p_Data, int p_Shift)
          {
               tmp_Data.U = p_Data.U >> p_Shift;
               
               //*cout << "\n\n\t __QN__ tmp_Data.U->" << tmp_Data.U << " p_Data.U->" << p_Data.U << " p_Shift->" << p_Shift;
               
               if (p_Node == NULL)
               {      
                    //*cout << "\n\t   Node is NULL";
                    p_Node = new c_NT3_Fractal_State_Node_2D;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    p_Node->NID = NULL;
                    if (p_Shift >= 1)
                    {
                         //*cout << "\n\t   p_Shift > 1";
                         p_Node->Data = tmp_Data;
                    }
                    else 
                    {
                         //*cout << "\n\t   p_Shift !> 1";
                         p_Node->Data = p_Data;
                         Current = &p_Node; 
                         flg_Foundit = false;
                         return 0;
                    }
               }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.U == tmp_Data.U && p_Shift >= 1)
               {
                    //*cout << "\n\t   p_Node->Data.U (" << p_Node->Data.U << ") == tmp_Data.U (" << tmp_Data.U << ")";
                    if (p_Shift == 1){ query_Node(p_Node->Center, p_Data, 0); return 0; }
                    query_Node(p_Node->Center, p_Data, (p_Shift - 2));
                    return 0;  
               }
               
               //If the node data matches the given data exactly the node has been found.
               if (p_Node->Data.U == p_Data.U)
               {
                    //*cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
                    Current = &p_Node;  
                    if (p_Node->NID != NULL){ flg_Foundit = true; }
                    return 0;
               }
               
               
               if (tmp_Data.U < p_Node->Data.U)
               {
                    //*cout << "\n\t    tmp_Data.U (" << tmp_Data.U << ") < p_Node->Data.U(" << p_Node->Data.U << ")";
                    query_Node(p_Node->Left, p_Data, p_Shift);
                    return 0;
               }
               if (tmp_Data.U > p_Node->Data.U)
               {
                    //*cout << "\n\t    tmp_Data.U (" << tmp_Data.U << ") > p_Node->Data.U(" << p_Node->Data.U << ")";
                    query_Node(p_Node->Right, p_Data, p_Shift);
                    return 0;
               }
               return 0;
          }
          
          
          
          void output_Tree()
          {
               output_Node(Root, 0);
          }
          
          void output_Node(c_NT3_Fractal_State_Node_2D *&p_Node, int p_Tab)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left, (p_Tab));
               cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    cout << "  ";
               }
               cout << (p_Node->Data).I << "->" << (p_Node->NID);
               output_Node(p_Node->Center, (p_Tab + 1));
               output_Node(p_Node->Right, (p_Tab));
          }
          
          void output_Tree_BP()
          {
               cout << "\n\t Fractal Tree Backpropagation Station";
               output_Node_BP(Root);
          }
          
          void output_Node_BP(c_NT3_Fractal_State_Node_2D *&p_Node)
          {
               if (p_Node == NULL){ return; }
               output_Node_BP(p_Node->Left);
               if ((p_Node->NID) != NULL){ cout << "\n\t" << (p_Node->Data).I << "->" << (p_Node->NID); (p_Node->NID)->bp_O(); }
               output_Node_BP(p_Node->Center);
               output_Node_BP(p_Node->Right);
          }
          
          c_NT3_Base_Node_2D * get_Node_NID(c_NT3_Fractal_State_Node_2D * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->NID;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_NID(c_NT3_Fractal_State_Node_2D * &p_Node, c_NT3_Base_Node_2D * p_NID)
          {
               if (p_Node != NULL)
               {
                    p_Node->NID = p_NID;
               }
          }
          
          c_NT3_Base_Node_2D * set_Current_Node_NID(c_NT3_Base_Node_2D * p_NID)
          {
               set_Node_NID(*Current, p_NID);
               return p_NID;
          }
          
          c_NT3_Base_Node_2D * get_Current_Node_NID()
          {
               return get_Node_NID(*Current);
          }
                    
};

