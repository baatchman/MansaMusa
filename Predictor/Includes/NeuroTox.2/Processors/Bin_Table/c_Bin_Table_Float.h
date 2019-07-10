


//This is a pseudoconstruct, it has a table and binary tree as its processor. It only deals with numbers.

class c_Bin_Table_Float
{
protected:
     
     
     
public:
     
     
     //==--     Processor
     
     //This table will be housed in the construct, not registered with the other tables.
     c_Table_1D Table;
     
     //This binary tree will allow for quickly searching the table.
     //Contains -
     c_Num_Bin_Tree_Float Bin_Tree[1000];
     
     //The tracker for active binary trees.
     int Bin_Trees_Active;
     
     //The current total Average.
     float Current_RCA;
     
     //Current number of nodes active in the binary trees.
     long int Active_Nodes;
     
     
     c_Bin_Table_Float()
     {
          Bin_Trees_Active = 0;
          Current_RCA = 0;
          Active_Nodes = 0;
     }
     
     ~c_Bin_Table_Float()
     {
          
     }
     
     //Sets the tables name to the given string.
     void set_Table_Name(string p_Name)
     {
          Table.set_Table_Name(p_Name);
     }
     
     c_Num_Bin_Node_Float * submit_Input(float p_Data)
     {
          int tmp_FDF = gf3df(p_Data);
          
          //Check if the number is already in, if so then 
          bool flg_Exists = Bin_Tree[tmp_FDF].exists(p_Data);
          
          //If the node already exists then return.
          //If the node does not exist then add it to the bin_tree, then to the table, then link them together using references.
          if (flg_Exists)
          {
               return *Bin_Tree[tmp_FDF].Current;
          }
          else
          {
               
               
               if (!Table.table_Is_Empty()){ Table.pop_Row(); }
               
               //Add to the table.
               Table.pushc_float(p_Data);
               
               //Add the index to the Bin_Tree associated with the submission.
               Bin_Tree[tmp_FDF].search(p_Data);
               Bin_Tree[tmp_FDF].set_Current_Node_TRID(Table.get_Current_Row());
               Active_Nodes++;
               return *Bin_Tree[tmp_FDF].Current;
          }
          return NULL;
          
     }
          
     //Takes a submission and reinforces it at the same time.
     c_Num_Bin_Node_Float * submit_Input_RC(float p_Data)
     {
          int tmp_FDF = gf3df(p_Data);
          
          //Check if the number is already in, if so then 
          bool flg_Exists = Bin_Tree[tmp_FDF].exists(p_Data);
          
          //If the node already exists then return.
          //If the node does not exist then add it to the bin_tree, then to the table, then link them together using references.
          if (flg_Exists)
          {
               Current_RCA -= Bin_Tree[tmp_FDF].get_RC_Score_Current_Node();
               Bin_Tree[tmp_FDF].reinforce_Current_Node();
               Current_RCA += Bin_Tree[tmp_FDF].get_RC_Score_Current_Node();
               return *Bin_Tree[tmp_FDF].Current;
          }
          else
          {
               
               
               if (!Table.table_Is_Empty()){ Table.pop_Row(); }
               
               //Add to the table.
               Table.pushc_float(p_Data);
               
               //Add the index to the Bin_Tree associated with the submission.
               Bin_Tree[tmp_FDF].search(p_Data);
               Bin_Tree[tmp_FDF].set_Current_Node_TRID(Table.get_Current_Row());
               Current_RCA += Bin_Tree[tmp_FDF].get_RC_Score_Current_Node();
               Active_Nodes++;
               return *Bin_Tree[tmp_FDF].Current;
          }
          return NULL;
     }
     
     //Gets the TRID of an input. It is used to then gather inputs from the table.
     int get_TRID(float p_Data)
     {
          Bin_Tree[gf3df(p_Data)].search(p_Data);
          return Bin_Tree[gf3df(p_Data)].get_Current_Node_TRID();
     }
     
     //Gets the pattern from the table for the given row (TRID).
     float get_Pattern(int p_Index)
     {
          return Table.get_float(p_Index, 0);
     }
     
     float get_RC_Score(int p_Index)
     {
          float tmp_Pattern = get_Pattern(p_Index);
          int tmp_FDF = gf3df(tmp_Pattern);
          Bin_Tree[tmp_FDF].exists(tmp_Pattern);
          return Bin_Tree[tmp_FDF].get_RC_Score_Current_Node();
     }
     
     float get_RC_Average()
     {
          //return Bin_Tree.get_RC_Average();
          if (Active_Nodes == 0){ return 0.0; }
          return Current_RCA / float (Active_Nodes);
     }
     
     //Gets the number of rows in the bin table.
     int get_Row_Count()
     {
          return Table.get_Row_Count();
     }
     
     
     
     //Bubble sorts the output based on pattern as a float.
     void sort_Nodes_Float()
     {
          //Sort the table.
          Table.bubble_Sort_Raw_Float();
          
          //tmp_String
          float tmp_Pattern = 0.0;
          
          //Now to look through and assign indexes to the binary tree.
          for (int cou_Index=0;cou_Index<Table.get_Row_Count();cou_Index++)
          {
               tmp_Pattern = Table.get_float(cou_Index, 0);
               Bin_Tree[gf3df(tmp_Pattern)].search(tmp_Pattern);
               Bin_Tree[gf3df(tmp_Pattern)].set_Current_Node_TRID(cou_Index);
          }
     }
     
     
     //Outputs the table and tree for the Bin_Table.
     void output_Bin_Tree()
     {
          Table.output_Table();
          Bin_Tree->output_Tree();
     }
     
};







