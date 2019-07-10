
class c_State_Node_Bin_Node_1D
{
     friend class c_State_Node_Bin_Tree_1D; 
     private:
          c_State_Node_Bin_Node_1D * Right;
          c_State_Node_Bin_Node_1D * Left;
          
     public:
          u_Data Data;
          c_Base_Node_1D * NID;
          
          c_State_Node_Bin_Node_1D()
          {
               Right = NULL;
               Left = NULL;
               NID = NULL;
               Data.I = 0;
          }
          
          ~c_State_Node_Bin_Node_1D()
          {
               delete Right; Right = NULL;
               delete Left; Left = NULL;
               NID = NULL;
          }
};

class c_State_Node_Bin_Tree_1D
{
     private:
             
     public:
          c_State_Node_Bin_Node_1D * Root;
          c_State_Node_Bin_Node_1D ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          string Tree_Name;
          
          
          long int Number_Of_Nodes;
          
          //For the optimization routines.
          u_Data * srt_Data;
          c_Base_Node_1D ** srt_NID;
          int srt_Position;
          
          
          //The file for the debugging.
          ofstream DBug;
          
          c_State_Node_Bin_Tree_1D()
          {
               Tree_Name = "Dunno_No_One_Named_Me :( ";
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
               Number_Of_Nodes = 0;
               
               srt_Data = NULL;
               srt_NID = NULL;
               
               DBug.close();
               DBug.open("Bin_Tree_Debug.dat");
          }
          
          ~c_State_Node_Bin_Tree_1D()
          {
               delete Root;
               DBug.close();
          }
          
          void set_Name(string p_Name)
          {
               Tree_Name = p_Name;
          }
          
          //Avoid the ini in the function.
          c_State_Node_Bin_Node_1D ** tmp_Search_Node;
          
          //Searches the tree using an iterative method rather than a recursive method.
          void search(u_Data p_Data)
          {
               
               flg_Foundit = false;
               
               //Get the root node.
               tmp_Search_Node = &Root;
               
               //cout << "\n\n\t Searching NR   p_Data->" << p_Data.I; 
               
               while ((*tmp_Search_Node) != NULL)
               {
                    //cout << "\n\t  tmp_SN->" << tmp_Search_Node;
                    
                    //If the data matches a match has been found.
                    if ((*tmp_Search_Node)->Data.NR == p_Data.NR)
                    {
                         //cout << "\n\t   Node Found tmp_Node->Data->" << (*tmp_Search_Node)->Data.I;
                         
                         if (tmp_Search_Node != NULL){ flg_Foundit = true; }
                    
                         Current = tmp_Search_Node;
                         return;
                    }
                    
                    //If the data is less than the current data search the left leg.
                    if ((*tmp_Search_Node)->Data.NR > p_Data.NR)
                    {
                         //cout << "\n\t   tmp_Node->Data->" << (*tmp_Search_Node)->Data.I << " > p_Data.I->" << p_Data.I;
                         
                         tmp_Search_Node = &((*tmp_Search_Node)->Left);
                         continue;
                    }
                    
                    //If the data is greater than the current data search the right leg.
                    if ((*tmp_Search_Node)->Data.NR < p_Data.NR)
                    {
                         //cout << "\n\t   tmp_Node->Data->" << (*tmp_Search_Node)->Data.I << " < p_Data.I->" << p_Data.I;
                         
                         tmp_Search_Node = &((*tmp_Search_Node)->Right);
                         continue;
                    }
                    
               }
               
               //If the node equals NULL then create a new node.
               if ((*tmp_Search_Node) == NULL)
               {
                    (*tmp_Search_Node) = new c_State_Node_Bin_Node_1D;
                    (*tmp_Search_Node)->Data.NR = p_Data.NR;
                    Current = tmp_Search_Node;
                    Number_Of_Nodes++;
                    return;
               }
               
               //Return.
               return;
          }
          
          void search_R(u_Data p_Data)
          {
               flg_Foundit = false;
               DBug << "\n\n p_Data->" << p_Data.I; DBug.flush();
               query_Node(Root, p_Data);
          }
          
          int query_Node(c_State_Node_Bin_Node_1D *&p_Node, u_Data p_Data)
          {
               //cout << "\n\t\t " << this << " p_Node->" << p_Node << " p_Data->" << p_Data.NR;
               
               if (p_Node == NULL)
               {     
                    DBug << " NULL NODE "; DBug.flush();
                    DBug << " p_Node->" << p_Node; DBug.flush();
                    p_Node = new c_State_Node_Bin_Node_1D;
                    p_Node->Data = p_Data;
                    Current = &p_Node; 
                    Number_Of_Nodes++;
                    
                    //DBug << "\n Number_Of_Nodes->" << Number_Of_Nodes << " p_Node->" << p_Node << " Left->" << p_Node->Left << " Right->" << p_Node->Right << " Current->" << *Current;
                    DBug.flush();
                    return 0;
               }
               if (p_Node->Data.NR == p_Data.NR)
               {
                    Current = &p_Node;  
                    if (p_Node->NID != NULL){ flg_Foundit = true; }
                    return 0;
               }
                    
               if (p_Data.NR < p_Node->Data.NR)
               {
                    query_Node(p_Node->Left, p_Data);
                    return 0;
               }
                    
               if (p_Data.NR > p_Node->Data.NR)
               {
                    query_Node(p_Node->Right, p_Data);
                    return 0;
               }
                    
               return 0;
          }
          
          u_Data search_NID(c_Base_Node_1D * p_NID)
          {
               flg_Foundit = false;
               return query_Node_NID(Root, p_NID);
          }
          
          u_Data query_Node_NID(c_State_Node_Bin_Node_1D *&p_Node, c_Base_Node_1D * p_NID)
          {
               //cout << "\n\t\t " << this << " p_Node->" << p_Node << " p_Data->" << p_Data.NR;
               
               if (p_Node == NULL)
               {    
               }
               if (p_Node->NID == p_NID)
               {
                    Current = &p_Node;  
                    return p_Node->Data;
               }
                    
               if (p_NID < p_Node->NID)
               {
                    return query_Node_NID(p_Node->Left, p_NID);
               }
                    
               if (p_NID > p_Node->NID)
               {
                    return query_Node_NID(p_Node->Right, p_NID);
               }
                    
               u_Data tmp_Dat;
               tmp_Dat.I = -1;
               return tmp_Dat;
          }
          void output_Tree()
          {
               cout << "\n\t  Outputting Tree With " << Number_Of_Nodes << " Nodes";
               output_Node(Root);
          }
          
          void output_Node(c_State_Node_Bin_Node_1D *&p_Node)
          {
               if (p_Node == NULL){ return; }
               
               cout << "\n\t\t &" << p_Node->NID << " >" << p_Node->Data.NR << " >" << char (p_Node->Data.I);
               
               if (p_Node->Left != NULL){ cout << "\n\t     ~L"; }
               output_Node(p_Node->Left);
               
               if (p_Node->Right != NULL){ cout << "\n\t     ~R"; }
               output_Node(p_Node->Right);
               
               cout << "\n\t      ~U";
          }
          
          c_Base_Node_1D * get_Node_NID(c_State_Node_Bin_Node_1D * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->NID;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_NID(c_State_Node_Bin_Node_1D * &p_Node, c_Base_Node_1D * p_NID)
          {
               if (p_Node != NULL){
                    p_Node->NID = p_NID;
                    }
          }
          
          c_Base_Node_1D * set_Current_Node_NID(c_Base_Node_1D * p_NID)
          {
               set_Node_NID(*Current, p_NID);
               return p_NID;
          }
          
          c_Base_Node_1D * get_Current_Node_NID()
          {
               return get_Node_NID(*Current);
          }
          
          
          //==--      Used for optimization of the tree.
          //  First all of the nodes will be read into an array.
          //  Then they will be bubble sorted.
          //  Adfter that they will be read in starting in the middles of the array and reading outward in both directions.
          
          void optimize()
          {
               
               srt_Data = new u_Data[Number_Of_Nodes];
               srt_NID = new c_Base_Node_1D*[Number_Of_Nodes];
               srt_Position = -1;
               int tmp_Number_Of_Nodes = Number_Of_Nodes;
               
               //Read the nodes into the arrays.
               //gather_Node(Root);
               gather_Nodes();
               
               //Now to sort the arrays by NID;
               bool tmp_Done = 0;
               u_Data tmp_Data;
               c_Base_Node_1D * tmp_NID;
               
               //BUBBLE SOOORRRTTTT!!!!!!!
               while(!tmp_Done)
               {
                    //Set the false flag.
                    tmp_Done = true;
                    
                    //Loop through sorting the nodes.
                    for (int cou_Index=0;cou_Index<Number_Of_Nodes - 1;cou_Index++)
                    {
                         //if (srt_NID[cou_Index] > srt_NID[cou_Index + 1])
                         if (srt_Data[cou_Index].NR > srt_Data[cou_Index + 1].NR)
                         {
                              tmp_Data.NR = srt_Data[cou_Index].NR;
                              tmp_NID = srt_NID[cou_Index];
                              
                              srt_Data[cou_Index].NR = srt_Data[cou_Index + 1].NR;
                              srt_NID[cou_Index] = srt_NID[cou_Index + 1];
                              
                              srt_Data[cou_Index + 1].NR = tmp_Data.NR;
                              srt_NID[cou_Index + 1] = tmp_NID;
                              
                              tmp_Done = false;
                         }
                    }
               }
               
               //Delete Root and set to NULL to prep for rebuilding.
               delete Root;
               Root = NULL;
               Number_Of_Nodes = 0;
               
               //Now do the rebuilding using the tier by tier algorithm.
               for (float cou_I=tmp_Number_Of_Nodes;cou_I>1;cou_I*=.5)
               {
                    //cout << "\n\t " << cou_I;
                    for (float cou_N=(cou_I * .5);cou_N<tmp_Number_Of_Nodes;cou_N+=cou_I)
                    {
                         //cout << "\n\t   [" << (int (cou_N)) << "] NID->" << srt_NID[(int (cou_N))] << " Data->" << srt_Data[(int (cou_N))].I;
                         search(srt_Data[(int (cou_N))]);
                         set_Current_Node_NID(srt_NID[(int (cou_N))]);
                    } 
               }
               
               delete [] srt_Data;
               srt_Data = NULL;
               
               delete [] srt_NID;
               srt_NID = NULL;
               
               srt_Position = 0;
          }
          
          //Gathers nodes recursively.
          void gather_Node(c_State_Node_Bin_Node_1D *&p_Node)
          {
               if (p_Node == NULL){ return; }
               
               //Get this node.
               srt_Data[srt_Position].NR = p_Node->Data.NR;
               srt_NID[srt_Position] = p_Node->NID;
               srt_Position++;
               
               //Get the lower node.
               gather_Node(p_Node->Left);
               
               //Now for the higher node.
               gather_Node(p_Node->Right);
          }
          
          //Gathers the nodes non recursively.
          void gather_Nodes()
          {
               //cout << "\n  gather_Nodes() Number_Of_Nodes->" << Number_Of_Nodes;
               
               if (Number_Of_Nodes == 0){ return; }
               
               //This integer is used to track the current node in the pseudostack.
               int Node_EIP = 0;
               
               //This is used to determine whether or not a node has been gathered when moving back up the array.
               int * Node_State;
               Node_State = new int[Number_Of_Nodes];
               
               //This acts as a temporary stack.
               c_State_Node_Bin_Node_1D ** Node_Stack;
               Node_Stack = new c_State_Node_Bin_Node_1D*[Number_Of_Nodes];
               
               for (int cou_Index=0;cou_Index<Number_Of_Nodes;cou_Index++)
               {
                    Node_State[cou_Index] = false;
                    Node_Stack[cou_Index] = NULL;
               }
               
               //State here with the gathering of the root node.
               Node_Stack[0] = Root;
               
               //Gather the Root node.
               srt_Position++;
               srt_Data[srt_Position].NR = Node_Stack[Node_EIP]->Data.NR;
               srt_NID[srt_Position] = Node_Stack[Node_EIP]->NID;
               
               //cout << "\n\t     Node_EIP->" << Node_EIP << "srt_P->" << srt_Position << " srt_Data->" << srt_Data[srt_Position].I << " srt_NID->" << srt_NID[srt_Position];
                              
               
               while (1)
               {
                    if (Node_EIP < 0){ break; }
                    
                    //cout << "\n\t EIP->" << Node_EIP << " srt_P->" << srt_Position;
                    
                    //Check State of the current node.
                    //cout << "\n\t   St";
                    
                    //If 0 then check the left leg and increment the state.
                    if (Node_State[Node_EIP] == 0)
                    {
                         //Increment the counter for the current node.
                         Node_State[Node_EIP]++;
                    
                         //If there is a left leg then add it to the pseudostack and set the EIP. Then continue with the investigation.
                         if (Node_Stack[Node_EIP]->Left != NULL)
                         {
                              //cout << "\n\t   L";
                              
                              Node_Stack[Node_EIP + 1] = Node_Stack[Node_EIP]->Left;
                              Node_EIP++;
                              
                              //Gather the left node.
                              srt_Position++;
                              srt_Data[srt_Position].NR = Node_Stack[Node_EIP]->Data.NR;
                              srt_NID[srt_Position] = Node_Stack[Node_EIP]->NID;
                              
                              //cout << "\n\t     Node_EIP->" << Node_EIP << "srt_P->" << srt_Position << " srt_Data->" << srt_Data[srt_Position].I << " srt_NID->" << srt_NID[srt_Position];
                              
                              
                              continue;
                         }
                    }
                    
                    //If it is 1 then check the right leg and increment it.
                    if (Node_State[Node_EIP] == 1)
                    {     
                         //Increment the counter for the current node.
                         Node_State[Node_EIP]++;
                         
                         //If there is a right leg then add it to the pseudostack and set the EIP accordingly.
                         if (Node_Stack[Node_EIP]->Right != NULL)
                         {
                              //cout << "\n\t   R";
                              
                              Node_Stack[Node_EIP + 1] = Node_Stack[Node_EIP]->Right;
                              Node_EIP++;
                              
                              //Gather the right node.
                              srt_Position++;
                              srt_Data[srt_Position].NR = Node_Stack[Node_EIP]->Data.NR;
                              srt_NID[srt_Position] = Node_Stack[Node_EIP]->NID;
                              
                              //cout << "\n\t     Node_EIP->" << Node_EIP << "srt_P->" << srt_Position << " srt_Data->" << srt_Data[srt_Position].I << " srt_NID->" << srt_NID[srt_Position];
                              
                              
                              continue;
                         }
                         
                         //If the right leg does not exist then fall through to the undocking procedures.
                    }
                    
                    //cout << "\n\t   U";
                    
                    //If the routine has fallen through to this point then do the backpedal pseudostack.
                    Node_Stack[Node_EIP] = NULL;
                    Node_State[Node_EIP] = 0;
                    Node_EIP--;
                    
               }
               
               delete [] Node_Stack;
               Node_Stack = NULL;
               
               delete [] Node_State;
               Node_State = NULL;
               
          }
     
};

