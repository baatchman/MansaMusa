
class c_State_Bin_Node
{
     friend class c_State_Bin_Tree; 
     private:
          c_State_Bin_Node * Right;
          c_State_Bin_Node * Center;
          c_State_Bin_Node * Left;
          
     public:
          char Data;
          void * NID;
          
          c_State_Bin_Node()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
          }
          
          ~c_State_Bin_Node()
          {
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_State_Bin_Tree
{
     private:
             
     public:
          c_State_Bin_Node * Root;
          c_State_Bin_Node ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          c_State_Bin_Tree()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_State_Bin_Tree()
          {
               delete Root;
          }
          
          void search(char p_Data[], int p_Data_Depth)
          {
               flg_Foundit = false;
               query_Node(Root, p_Data, p_Data_Depth, 0);
          }
          
          int query_Node(c_State_Bin_Node *&p_Node, char p_Data[], int p_Data_Depth, int p_Index)
          {
               if (p_Node == NULL){      
                    p_Node = new c_State_Bin_Node;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    p_Node->Data = p_Data[p_Index];
                    p_Node->NID = NULL;
                    }
               if (p_Node->Data == p_Data[p_Index]){
                    p_Index++;
                    if (p_Index < p_Data_Depth){
                         query_Node(p_Node->Center, p_Data, p_Data_Depth, p_Index);
                         return 0;
                    } else {
                         Current = &p_Node;  
                         if (p_Node->NID != NULL){ flg_Foundit = true; }
                         return 0;
                         }
                          
                    }
               if (p_Data[p_Index] < p_Node->Data){
                    query_Node(p_Node->Left, p_Data, p_Data_Depth, p_Index);
                    return 0;
                    }
               if (p_Data[p_Index] > p_Node->Data){
                    query_Node(p_Node->Right, p_Data, p_Data_Depth, p_Index);
                    return 0;
                    }
               return 0;
          }
          
          void output_Tree()
          {
               output_Node(Root);
          }
          
          void output_Node(c_State_Bin_Node *&p_Node)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left);
               output_Node(p_Node->Right);
          }
          
          void* get_Node_NID(c_State_Bin_Node * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->NID;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_NID(c_State_Bin_Node * &p_Node, void * p_NID)
          {
               if (p_Node != NULL){
                    p_Node->NID = p_NID;
                    }
          }
          
          void set_Current_Node_NID(void * p_NID)
          {
               set_Node_NID(*Current, p_NID);
          }
          
          void * get_Current_Node_NID()
          {
               return get_Node_NID(*Current);
          }
                    
};

