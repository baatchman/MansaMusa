
class c_Bin_Node__Base_Node
{
     friend class c_Bin_Tree__Base_Node; 
     private:
          c_Bin_Tree__Base_Node * Right;
          c_Bin_Tree__Base_Node * Center;
          c_Bin_Tree__Base_Node * Left;
          
     public:
          u_Data Data;
          c_Base_Node * Node;
          
          virtual ~c_Bin_Node__Base_Node()=0;
                
};
