
//This class is used for loading nodes when rebuilding.
class c_Scaffold
{
public:
     
     //The nodes for the scaffold.
     c_Scaffold_Node *** Nodes;
     
     //The number of tiers.
     int Number_Of_Tiers;
     
     //The Node Count For Each Tier.
     int ** Node_Count;
     
     
     c_Scaffold()
     {
          //Create the array to hold the nodes.
          Nodes = new c_Scaffold_Node**[1];
          Nodes[0] = new c_Scaffold_Node*[1];
          Nodes[0] [0] = NULL;
          
          //Currently no nodes.
          Node_Count = new int*[1];
          Node_Count[0] = new int;
          *Node_Count[0] = 0;
          
          //Sets the number of tiers.
          Number_Of_Tiers = 0;
     }
     
     ~c_Scaffold()
     {
          for (int cou_Index=0;cou_Index<Number_Of_Tiers;cou_Index++)
          {
               delete [] Nodes[cou_Index];
               Nodes[cou_Index] = NULL;
          }
          
          delete [] Nodes;
          Nodes = NULL;
          
          delete [] Node_Count;
          Node_Count = NULL;
          
     }
     
     //Resets the scaffold for an input.
     void reset_Scaffold()
     {
          for (int cou_Index=0;cou_Index<Number_Of_Tiers;cou_Index++)
          {
               delete [] Nodes[cou_Index];
               Nodes[cou_Index] = NULL;
          }
          
          delete [] Nodes;
          Nodes = NULL;
          
          delete [] Node_Count;
          Node_Count = NULL;
          
          //Create the array to hold the nodes.
          Nodes = new c_Scaffold_Node**[1];
          Nodes[0] = new c_Scaffold_Node*[1];
          Nodes[0] [0] = NULL;
          
          //Currently no nodes.
          Node_Count = new int*[1];
          Node_Count[0] = new int;
          *Node_Count[0] = 0;
          
          //Sets the number of tiers.
          Number_Of_Tiers = 0;
          
     }
     
     //Increases the number of tiers to a given level.
     void increase_Number_Of_Tiers(int p_Tier)
     {
          //Setup the temporary array that will hold the tiers.
          c_Scaffold_Node *** tmp_Nodes;
          tmp_Nodes = new c_Scaffold_Node ** [Number_Of_Tiers];
          
          //Setup the temporary array that will hold the node count.
          int ** tmp_Node_Count = new int * [Number_Of_Tiers];
          
          //Loop through each tier allocating the appropriate number of nodes and copying the old ones into the new array.
          //The node count will be copied as well.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               //Allocate the number of nodes necessary.
               tmp_Nodes[cou_T] = new c_Scaffold_Node * [*Node_Count[cou_T]];
               
               //Copy the old nodes into the temporary tier.
               for (int cou_NID=0;cou_NID<*Node_Count[cou_T];cou_NID++)
               {
                    tmp_Nodes[cou_T] [cou_NID] = Nodes[cou_T] [cou_NID];
               }
               
               //Delete the current tier in the original nodes array.
               delete [] Nodes[cou_T];
               Nodes[cou_T] = NULL;
               
               //Copy the node count.
               tmp_Node_Count[cou_T] = Node_Count[cou_T];
          }
          
          //Delete the Node_Count array and reallocate it to the appropriate height, then copy the old values into it.
          delete [] Node_Count;
          Node_Count = NULL;
          
          Node_Count = new int * [p_Tier];
          
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               Node_Count[cou_T] = tmp_Node_Count[cou_T];
          }
          
          //Create the count for the new tiers.
          for (int cou_T=Number_Of_Tiers;cou_T<p_Tier;cou_T++)
          {
               Node_Count[cou_T] = new int;
               *Node_Count[cou_T] = 0;
          }
          
          //Delete the temporary array.
          delete [] tmp_Node_Count;
          tmp_Node_Count = NULL;
          
          //Delete the node array and reallocate it to the appropriate height.
          delete [] Nodes;
          Nodes = NULL;
          Nodes = new c_Scaffold_Node ** [p_Tier];
          
          //Allocate the nodes for the tiers and copy them into the node array from the temporary array.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               Nodes[cou_T] = new c_Scaffold_Node * [*Node_Count[cou_T]];
               
               for (int cou_NID=0;cou_NID<*Node_Count[cou_T];cou_NID++)
               {
                    Nodes[cou_T] [cou_NID] = tmp_Nodes[cou_T] [cou_NID];
               }
               
               //Cleanup the tiers on the temporary after it is copied.
               delete [] tmp_Nodes[cou_T];
               tmp_Nodes[cou_T] = NULL;
          }
          
          //Delete the tmp_Nodes array as it is no longer needed.
          delete [] tmp_Nodes;
          tmp_Nodes = NULL;
          
          //NULL the newly created tiers.
          for (int cou_T=Number_Of_Tiers;cou_T<p_Tier;cou_T++)
          {
               Nodes[cou_T] = new c_Scaffold_Node * [1];
               Nodes[cou_T] [0] = NULL;
          }
          
          //Set the number of tiers to the correct amount.
          Number_Of_Tiers = p_Tier;
          
     }
     
     
     //Increases the number of nodes on a given tier.
     void increase_Number_Of_Nodes_On_Tier(int p_Tier, int p_Index)
     {
          //If the tier has no nodes then reallocate the tier withouth the tedious copying.
          if (*Node_Count[p_Tier] == 0)
          {
               //Delete the current tier and allocate it.
               delete [] Nodes[p_Tier];
               Nodes[p_Tier] = NULL;
               
               Nodes[p_Tier] = new c_Scaffold_Node * [p_Index];
               for (int cou_Index=0;cou_Index<p_Index;cou_Index++)
               {
                    Nodes[p_Tier] [cou_Index] = new c_Scaffold_Node;
               }
               
               *Node_Count[p_Tier] = p_Index;
               return;
          }
          
          //Setup the temporay array to hold the nodes whilest it is reallocated.
          c_Scaffold_Node ** tmp_Tier;
          tmp_Tier = new c_Scaffold_Node * [*Node_Count[p_Tier]];
          
          //Copy the nodes into the temporary array.
          for (int cou_NID=0;cou_NID<*Node_Count[p_Tier];cou_NID++)
          {
               tmp_Tier[cou_NID] = Nodes[p_Tier] [cou_NID];
          }
          
          //Delete the original and reallocate it to the current size plus one.
          delete [] Nodes[p_Tier];
          Nodes[p_Tier] = NULL;
          
          Nodes[p_Tier] = new c_Scaffold_Node * [p_Index];
          
          //Copy the old nodes back into the tier.
          for (int cou_NID=0;cou_NID<*Node_Count[p_Tier];cou_NID++)
          {
               Nodes[p_Tier] [cou_NID] = tmp_Tier[cou_NID];
          }
          
          //Cleanup the temporary array.
          delete [] tmp_Tier;
          tmp_Tier = NULL;
          
          //NULL the newest nodes.
          for (int cou_Index=*Node_Count[p_Tier];cou_Index<p_Index;cou_Index++)
          {
               Nodes[p_Tier] [cou_Index] = new c_Scaffold_Node;
          }
          
          //Increment the counter.
          *Node_Count[p_Tier] = p_Index;
     }
     
     //Outputs the scaffolding.
     void output_Scaffold()
     {
          cout << "\n\n\t Scaffold #T->" << Number_Of_Tiers;
          
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n\t [";
               oint(0, 14, cou_T);
               cout << "]..";
               oint(0, 13, *Node_Count[cou_T]);
               
               for (int cou_Index=0;cou_Index<*Node_Count[cou_T];cou_Index++)
               {
                    cout << "\n\t   [";
                    oint(0, 15, cou_T);
                    cout << "] [";
                    oint(0, 14, cou_Index);
                    cout << "] ";
                    Nodes[cou_T] [cou_Index]->output_Node();
               }
          }
     }
     
};