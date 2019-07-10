
class c_MT_Fractal_State_Bin_Node
{
     friend class c_MT_Fractal_State_Bin_Tree; 
     private:
          c_MT_Fractal_State_Bin_Node * Right;
          c_MT_Fractal_State_Bin_Node * Center;
          c_MT_Fractal_State_Bin_Node * Left;
          
     public:
          u_Data Data;
          c_MT_Base_Node_1D * NID;
          
          c_MT_Fractal_State_Bin_Node()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
          }
          
          ~c_MT_Fractal_State_Bin_Node()
          {
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_MT_Fractal_State_Bin_Tree
{
private:
             
             
     public:
          c_MT_Fractal_State_Bin_Node * Root;
          c_MT_Fractal_State_Bin_Node ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          u_Data tmp_Data;
          
          c_MT_Fractal_State_Bin_Tree()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_MT_Fractal_State_Bin_Tree()
          {
               delete Root;
          }
          
          void search(u_Data p_Data, int p_Fractal = 10000000)
          {
               flg_Foundit = false;
               query_Node(Root, p_Data, p_Fractal);
          }
          
          int query_Node(c_MT_Fractal_State_Bin_Node *&p_Node, u_Data p_Data, int p_Fractal)
          {
               tmp_Data.I = int (p_Data.I / p_Fractal);// - (p_Data.I / (p_Fractal * 10));
               
               //cout << "\n\n\t __QN__ tmp_Data.I->" << tmp_Data.I << " p_Data.I->" << p_Data.I << " p_Fractal->" << p_Fractal;
               
               if (p_Node == NULL)
               {      
                    //cout << "\n\t   Node is NULL";
                    p_Node = new c_MT_Fractal_State_Bin_Node;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    p_Node->NID = NULL;
                    if (p_Fractal > 1)
                    {
                         //cout << "\n\t   p_Fractal > 1";
                         p_Node->Data = tmp_Data;
                    }
                    else 
                    {
                         //cout << "\n\t   p_Fractal !> 1";
                         p_Node->Data = p_Data;
                         Current = &p_Node; 
                         flg_Foundit = false;
                         return 0;
                    }
               }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.I == tmp_Data.I && p_Fractal != 1)
               {
                    //cout << "\n\t   p_Node->Data.I (" << p_Node->Data.I << ") == tmp_Data.I (" << tmp_Data.I << ")";
                    p_Fractal *= .1;
                    
                    query_Node(p_Node->Center, p_Data, p_Fractal);
                    return 0;  
               }
               
               //If the node data matches the given data exactly the node has been found.
               if (p_Node->Data.I == p_Data.I)
               {
                    //cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
                    Current = &p_Node;  
                    if (p_Node->NID != NULL){ flg_Foundit = true; }
                    return 0;
               }
               
               
               if (tmp_Data.I < p_Node->Data.I)
               {
                    //cout << "\n\t    tmp_Data.I (" << tmp_Data.I << ") < p_Node->Data.I(" << p_Node->Data.I << ")";
                    query_Node(p_Node->Left, p_Data, p_Fractal);
                    return 0;
               }
               if (tmp_Data.I > p_Node->Data.I)
               {
                    //cout << "\n\t    tmp_Data.I (" << tmp_Data.I << ") > p_Node->Data.I(" << p_Node->Data.I << ")";
                    query_Node(p_Node->Right, p_Data, p_Fractal);
                    return 0;
               }
               return 0;
          }
          
          
          void search_Verbose(u_Data p_Data, int p_Fractal = 10000000)
          {
               flg_Foundit = false;
               cout << "\n\n\t Commencing Search->" << p_Data.I << " p_F->" << p_Fractal;
               query_Node_Verbose(Root, p_Data, p_Fractal);
          }
          
          int query_Node_Verbose(c_MT_Fractal_State_Bin_Node *&p_Node, u_Data p_Data, int p_Fractal)
          {
               tmp_Data.I = int (p_Data.I / p_Fractal);// - (p_Data.I / (p_Fractal * 10));
               
               cout << "\n\n\t __QN__ tmp_Data.I->" << tmp_Data.I << " p_Data.I->" << p_Data.I << " p_Fractal->" << p_Fractal;
               
               if (p_Node == NULL)
               {      
                    cout << "\n\t   Node is NULL";
                    p_Node = new c_MT_Fractal_State_Bin_Node;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    p_Node->NID = NULL;
                    if (p_Fractal > 1)
                    {
                         cout << "\n\t   p_Fractal > 1";
                         p_Node->Data = tmp_Data;
                    }
                    else 
                    {
                         cout << "\n\t   p_Fractal !> 1";
                         p_Node->Data = p_Data;
                         Current = &p_Node; 
                         flg_Foundit = false;
                         return 0;
                    }
               }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.I == tmp_Data.I && p_Fractal != 1)
               {
                    cout << "\n\t   p_Node->Data.I (" << p_Node->Data.I << ") == tmp_Data.I (" << tmp_Data.I << ")";
                    p_Fractal *= .1;
                    
                    query_Node_Verbose(p_Node->Center, p_Data, p_Fractal);
                    return 0;  
               }
               
               //If the node data matches the given data exactly the node has been found.
               if (p_Node->Data.I == p_Data.I)
               {
                    cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
                    Current = &p_Node;  
                    if (p_Node->NID != NULL){ flg_Foundit = true; }
                    return 0;
               }
               
               
               if (tmp_Data.I < p_Node->Data.I)
               {
                    cout << "\n\t    tmp_Data.I (" << tmp_Data.I << ") < p_Node->Data.I(" << p_Node->Data.I << ")";
                    query_Node_Verbose(p_Node->Left, p_Data, p_Fractal);
                    return 0;
               }
               if (tmp_Data.I > p_Node->Data.I)
               {
                    cout << "\n\t    tmp_Data.I (" << tmp_Data.I << ") > p_Node->Data.I(" << p_Node->Data.I << ")";
                    query_Node_Verbose(p_Node->Right, p_Data, p_Fractal);
                    return 0;
               }
               cout << "\n\t Fractal_Tree_Request_Timed_Out...";
               cout << "\n\t  Node->" << p_Node << " Fractal->" << p_Fractal;
               return 0;
          }
          
          //This is used to get the reference to the node stored with the given handle.
          //Used for saving functions.
          c_MT_Base_Node_1D * get_NID(u_Data p_Data, int p_Fractal = 10000000)
          {
               return query_For_NID(Root, p_Data, p_Fractal);
          }
          
          c_MT_Base_Node_1D * query_For_NID(c_MT_Fractal_State_Bin_Node *&p_Node, u_Data p_Data, int p_Fractal)
          {
               tmp_Data.I = (p_Data.I / p_Fractal);// - (p_Data.I / (p_Fractal * 10));
               
               //cout << "\n\n\t __QN__ tmp_Data.I->" << tmp_Data.I << " p_Data.I->" << p_Data.I << " p_Fractal->" << p_Fractal;
               
               if (p_Node == NULL)
               {      
                    return NULL;
               }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.I == tmp_Data.I && p_Fractal != 1)
               {
                    //cout << "\n\t   p_Node->Data.I (" << p_Node->Data.I << ") == tmp_Data.I (" << tmp_Data.I << ")";
                    p_Fractal *= .1;
                    
                    return query_For_NID(p_Node->Center, p_Data, p_Fractal);
               }
               
               //If the node data matches the given data exactly the node has been found.
               if (p_Node->Data.I == p_Data.I)
               {
                    return p_Node->NID;
               }
               
               
               if (tmp_Data.I < p_Node->Data.I)
               {
                    //cout << "\n\t    tmp_Data.I (" << tmp_Data.I << ") < p_Node->Data.I(" << p_Node->Data.I << ")";
                    return query_For_NID(p_Node->Left, p_Data, p_Fractal);
               }
               if (tmp_Data.I > p_Node->Data.I)
               {
                    //cout << "\n\t    tmp_Data.I (" << tmp_Data.I << ") > p_Node->Data.I(" << p_Node->Data.I << ")";
                    return query_For_NID(p_Node->Right, p_Data, p_Fractal);
               }
               return NULL;
          }
          
          void output_Tree()
          {
               output_Node(Root, 0);
          }
          
          void output_Node(c_MT_Fractal_State_Bin_Node *&p_Node, int p_Tab)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left, (p_Tab));
               cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    cout << "  ";
               }
               cout << (p_Node->Data).I;
               output_Node(p_Node->Center, (p_Tab + 1));
               output_Node(p_Node->Right, (p_Tab));
          }
          
          c_MT_Base_Node_1D * get_Node_NID(c_MT_Fractal_State_Bin_Node * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->NID;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_NID(c_MT_Fractal_State_Bin_Node * &p_Node, c_MT_Base_Node_1D * p_NID)
          {
               if (p_Node != NULL)
               {
                    p_Node->NID = p_NID;
               }
          }
          
          c_MT_Base_Node_1D * set_Current_Node_NID(c_MT_Base_Node_1D * p_NID)
          {
               set_Node_NID(*Current, p_NID);
               return p_NID;
          }
          
          c_MT_Base_Node_1D * get_Current_Node_NID()
          {
               return get_Node_NID(*Current);
          }
                    
};

