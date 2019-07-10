
class c_Int_Node_Bin_Node_1D
{
     friend class c_Int_Node_Bin_Tree_1D; 
     private:
          c_Int_Node_Bin_Node_1D * Right;
          c_Int_Node_Bin_Node_1D * Left;
          
     public:
          u_Data Data;
          c_Num_Bin_Node_Int * NID;
          
          c_Int_Node_Bin_Node_1D()
          {
               Right = NULL;
               Left = NULL;
          }
          
          ~c_Int_Node_Bin_Node_1D()
          {
               delete Right; Right = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_Int_Node_Bin_Tree_1D
{
     private:
             
     public:
          c_Int_Node_Bin_Node_1D * Root;
          c_Int_Node_Bin_Node_1D ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          c_Int_Node_Bin_Tree_1D()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_Int_Node_Bin_Tree_1D()
          {
               delete Root;
          }
          
          void search(u_Data p_Data)
          {
               flg_Foundit = false;
               query_Node(Root, p_Data);
          }
          
          int query_Node(c_Int_Node_Bin_Node_1D *&p_Node, u_Data p_Data)
          {
               if (p_Node == NULL){      
                    p_Node = new c_Int_Node_Bin_Node_1D;
                    p_Node->Right = NULL;
                    p_Node->Left = NULL;
                    p_Node->Data = p_Data;
                    p_Node->NID = NULL;
                    }
               if (p_Node->Data.I == p_Data.I)
               {
                    Current = &p_Node;  
                    if (p_Node->NID != NULL){ flg_Foundit = true; }
                    return 0;
               }
                    
               if (p_Data.I < p_Node->Data.I)
               {
                    query_Node(p_Node->Left, p_Data);
                    return 0;
               }
                    
               if (p_Data.I > p_Node->Data.I)
               {
                    query_Node(p_Node->Right, p_Data);
                    return 0;
               }
                    
               return 0;
          }
          
          c_Num_Bin_Node_Int * exists(u_Data p_Data)
          {
               flg_Foundit = false;
               return exists(Root, p_Data);
          }
          
          c_Num_Bin_Node_Int * exists(c_Int_Node_Bin_Node_1D *&p_Node, u_Data p_Data)
          {
               if (p_Node == NULL)
               {
                    return NULL;
               }
               if (p_Node->Data.I == p_Data.I)
               {
                    Current = &p_Node;  
                    if (p_Node->NID != NULL){ flg_Foundit = true; } else { return NULL; }
                    return p_Node->NID;
               }
                    
               if (p_Data.I < p_Node->Data.I)
               {
                    return exists(p_Node->Left, p_Data);
               }
                    
               if (p_Data.I > p_Node->Data.I)
               {
                    return exists(p_Node->Right, p_Data);
               }
                    
               return NULL;
          }
          
          void output_Tree()
          {
               output_Node(Root);
          }
          
          void output_Node(c_Int_Node_Bin_Node_1D *&p_Node)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left);
               cout << "\n\t\t &" << p_Node->NID << " >" << p_Node->Data.I << " >" << char (p_Node->Data.I);
               output_Node(p_Node->Right);
          }
          
          c_Num_Bin_Node_Int * get_Node_NID(c_Int_Node_Bin_Node_1D * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->NID;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_NID(c_Int_Node_Bin_Node_1D * &p_Node, c_Num_Bin_Node_Int * p_NID)
          {
               if (p_Node != NULL){
                    p_Node->NID = p_NID;
                    }
          }
          
          c_Num_Bin_Node_Int * set_Current_Node_NID(c_Num_Bin_Node_Int * p_NID)
          {
               set_Node_NID(*Current, p_NID);
               return p_NID;
          }
          
          c_Num_Bin_Node_Int * get_Current_Node_NID()
          {
               return get_Node_NID(*Current);
          }
                    
};





//==--     THE BIN NODE TREE FOR THE FLOAT CONSTRUCT

class c_Float_Node_Bin_Node_1D
{
     friend class c_Float_Node_Bin_Tree_1D; 
     private:
          c_Float_Node_Bin_Node_1D * Right;
          c_Float_Node_Bin_Node_1D * Left;
          
     public:
          u_Data Data;
          c_Num_Bin_Node_Float * NID;
          
          c_Float_Node_Bin_Node_1D()
          {
               Right = NULL;
               Left = NULL;
          }
          
          ~c_Float_Node_Bin_Node_1D()
          {
               delete Right; Right = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_Float_Node_Bin_Tree_1D
{
     private:
             
     public:
          c_Float_Node_Bin_Node_1D * Root;
          c_Float_Node_Bin_Node_1D ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          c_Float_Node_Bin_Tree_1D()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_Float_Node_Bin_Tree_1D()
          {
               delete Root;
          }
          
          void search(u_Data p_Data)
          {
               flg_Foundit = false;
               query_Node(Root, p_Data);
          }
          
          int query_Node(c_Float_Node_Bin_Node_1D *&p_Node, u_Data p_Data)
          {
               if (p_Node == NULL){      
                    p_Node = new c_Float_Node_Bin_Node_1D;
                    p_Node->Right = NULL;
                    p_Node->Left = NULL;
                    p_Node->Data = p_Data;
                    p_Node->NID = NULL;
                    }
               if (p_Node->Data.F == p_Data.F)
               {
                    Current = &p_Node;  
                    if (p_Node->NID != NULL){ flg_Foundit = true; }
                    return 0;
               }
                    
               if (p_Data.F < p_Node->Data.F)
               {
                    query_Node(p_Node->Left, p_Data);
                    return 0;
               }
                    
               if (p_Data.F > p_Node->Data.F)
               {
                    query_Node(p_Node->Right, p_Data);
                    return 0;
               }
                    
               return 0;
          }
          
          c_Num_Bin_Node_Float * exists(u_Data p_Data)
          {
               flg_Foundit = false;
               return exists(Root, p_Data);
          }
          
          c_Num_Bin_Node_Float * exists(c_Float_Node_Bin_Node_1D *&p_Node, u_Data p_Data)
          {
               if (p_Node == NULL)
               {
                    return NULL;
               }
               if (p_Node->Data.F == p_Data.F)
               {
                    Current = &p_Node;  
                    if (p_Node->NID != NULL){ flg_Foundit = true; } else { return NULL; }
                    return p_Node->NID;
               }
                    
               if (p_Data.F < p_Node->Data.F)
               {
                    return exists(p_Node->Left, p_Data);
               }
                    
               if (p_Data.F > p_Node->Data.F)
               {
                    return exists(p_Node->Right, p_Data);
               }
                    
               return NULL;
          }
          
          void output_Tree()
          {
               output_Node(Root);
          }
          
          void output_Node(c_Float_Node_Bin_Node_1D *&p_Node)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left);
               cout << "\n\t\t &" << p_Node->NID << " >" << p_Node->Data.F << " >" << char (p_Node->Data.F);
               output_Node(p_Node->Right);
          }
          
          c_Num_Bin_Node_Float * get_Node_NID(c_Float_Node_Bin_Node_1D * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->NID;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_NID(c_Float_Node_Bin_Node_1D * &p_Node, c_Num_Bin_Node_Float * p_NID)
          {
               if (p_Node != NULL){
                    p_Node->NID = p_NID;
                    }
          }
          
          c_Num_Bin_Node_Float * set_Current_Node_NID(c_Num_Bin_Node_Float * p_NID)
          {
               set_Node_NID(*Current, p_NID);
               return p_NID;
          }
          
          c_Num_Bin_Node_Float * get_Current_Node_NID()
          {
               return get_Node_NID(*Current);
          }
                    
};

