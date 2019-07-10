
class c_Num_Bin_Node_Float
{
     friend class c_Num_Bin_Tree_Float; 
     private:
          c_Num_Bin_Node_Float * Right;
          c_Num_Bin_Node_Float * Left;
          
     public:
          float Pattern;
          int TRID;
          int RC;
          int RC_LVL;
          
          c_Num_Bin_Node_Float()
          {
               Right = NULL;
               Left = NULL;
               Pattern = 0.0;
               TRID = -1;
               RC = 0;
               RC_LVL = 1;
          }
          
          ~c_Num_Bin_Node_Float()
          {
               delete Right; Right = NULL;
               delete Left; Left = NULL;
          }
          
          //Reinforces the node.
          void reinforce()
          {
               //Increment the RC_LVL.
               RC++;
               
               //If the RC_LVL == RC (increase the RC and reset the RC_LVL)
               if (RC >= RC_LVL){ RC_LVL++; RC = 0; }
          }
          
          int get_Reinforcement_Counter()
          {
               return RC;
          }
          
          int get_Reinforcement_Counter_Level()
          {
               return RC_LVL;
          }
          
          float get_RC_Score()
          {
               return (float (RC_LVL) + (float (RC) / float (RC_LVL)));
          }
          
                
};

class c_Num_Bin_Tree_Float
{
     private:
             
     public:
          c_Num_Bin_Node_Float * Root;
          c_Num_Bin_Node_Float ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          int Number_Of_Nodes;
          float RC_Total;
          
          c_Num_Bin_Tree_Float()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
               Number_Of_Nodes = 0;
               RC_Total = 0;
          }
          
          ~c_Num_Bin_Tree_Float()
          {
               delete Root;
          }
          
          void search(float p_Pattern)
          {
               flg_Foundit = false;
               query_Node(Root, p_Pattern);
          }
          
          int query_Node(c_Num_Bin_Node_Float *&p_Node, float p_Pattern)
          {
               if (p_Node == NULL)
               {      
                    p_Node = new c_Num_Bin_Node_Float;
                    p_Node->Right = NULL;
                    p_Node->Left = NULL;
                    p_Node->Pattern = p_Pattern;
                    Number_Of_Nodes++;
               }
               //Fall through.
               if (p_Node->Pattern == p_Pattern)
               {
                    Current = &p_Node;  
                    if (p_Node->TRID != -1){ flg_Foundit = true; }
                    return 0;
               }
                    
               if (p_Pattern < p_Node->Pattern)
               {
                    query_Node(p_Node->Left, p_Pattern);
                    return 0;
               }
                    
               if (p_Pattern > p_Node->Pattern)
               {
                    query_Node(p_Node->Right, p_Pattern);
                    return 0;
               }
                    
               return 0;
          }
          
          int exists(float p_Pattern)
          {
               flg_Foundit = false;
               return exists(Root, p_Pattern);
          }
          
          int exists(c_Num_Bin_Node_Float *&p_Node, float p_Pattern)
          {
               if (p_Node == NULL)
               {      
                    return 0;
               }
               //Fall through.
               if (p_Node->Pattern == p_Pattern)
               {
                    Current = &p_Node;
                    return 1;
               }
                    
               if (p_Pattern < p_Node->Pattern)
               {
                    return exists(p_Node->Left, p_Pattern);
               }
                    
               if (p_Pattern > p_Node->Pattern)
               {
                    return exists(p_Node->Right, p_Pattern);
               }
                    
               return 0;
          }
          
          //==--      Output members
          
          void output_Tree()
          {
               output_Node(Root);
          }
          
          void output_Node(c_Num_Bin_Node_Float *&p_Node)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left);
               cout << "\n\t\t &" << p_Node->Pattern << " >" << p_Node->TRID;
               output_Node(p_Node->Right);
          }
          
          //==--      Internal handling.
          
          int get_Node_TRID(c_Num_Bin_Node_Float * &p_Node)
          {
               if (p_Node != NULL)
               {     
                    return p_Node->TRID;      
               } 
               else 
               {
                    return -1;
               }
          }
          int get_Current_Node_TRID()
          {
               return get_Node_TRID(*Current);
          }
          
          void set_Node_TRID(c_Num_Bin_Node_Float * &p_Node, int p_TRID)
          {
               if (p_Node != NULL)
               {
                    p_Node->TRID = p_TRID;
               }
          }
          void set_Current_Node_TRID(int p_TRID)
          {
               set_Node_TRID(*Current, p_TRID);
          }
          
          //==--      Dealing with reinforcement
          
          //Reinforces the node.
          void reinforce(c_Num_Bin_Node_Float * &p_Node)
          {
               if (p_Node == NULL){ return; }
               
               //RC_Total -= p_Node->get_RC_Score();
               p_Node->reinforce();
               //RC_Total += p_Node->get_RC_Score();
               
          }
          void reinforce_Current_Node()
          {
               reinforce(*Current);
          }
          
          int get_Reinforcement_Counter(c_Num_Bin_Node_Float * &p_Node)
          {
               if (p_Node == NULL){ return 0.0; }
               
               return p_Node->get_Reinforcement_Counter();
          }
          int get_Reinforcement_Counter_Current_Node()
          {
               return get_Reinforcement_Counter(*Current);
          }
          
          int get_Reinforcement_Counter_Level(c_Num_Bin_Node_Float * &p_Node)
          {
               if (p_Node == NULL){ return 0.0; }
               
               return p_Node->get_Reinforcement_Counter_Level();
          }
          int get_Reinforcement_Counter_Level_Current_Node()
          {
               return get_Reinforcement_Counter_Level(*Current);
          }
          
          float get_RC_Score(c_Num_Bin_Node_Float * &p_Node)
          {
               if (p_Node == NULL){ return 0.0; }
               
               return p_Node->get_RC_Score();
          }
          float get_RC_Score_Current_Node()
          {
               return get_RC_Score(*Current);
          }
          
          float get_RC_Average()
          {
               if (Number_Of_Nodes == 0){ return 0.0f; }
               return (get_RC_Average(Root) / float (Number_Of_Nodes));
          }
          
          float get_RC_Average(c_Num_Bin_Node_Float *&p_Node)
          {
               if (p_Node == NULL){ return 0.0; }
               return get_RC_Average(p_Node->Left) + get_RC_Score(p_Node) + get_RC_Average(p_Node->Right);
          }
          
          
};



