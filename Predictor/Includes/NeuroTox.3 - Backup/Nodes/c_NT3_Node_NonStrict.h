//The basic polymorphic node to use for the node network.
class c_NT3_Base_Node_NonStrict
{
public:
     
     virtual ~c_NT3_Base_Node_NonStrict(){};
     
     //The ID of this node.
     u_Data_3 NID;
     
     //The next node in the Linked List.
     c_NT3_Base_Node_NonStrict * Next;
     
     //Axons and their counters.
     c_NT3_Base_Node_NonStrict ** Axons;
     int Axon_Count;
     
     //Dendrites and their counters.
     c_NT3_Base_Node_NonStrict ** Dendrites;
     int Dendrite_Count;
     
     //Reinforcement Counters.
     double RC_Lvl;
     
     //Whether or not the node is a state node.
     char Type;
     
     //This is used for things such as the buffer trees or anything that relies on references.
     void * Ref;
     
     //Member Functions.
     virtual void add_Axon(c_NT3_Base_Node_NonStrict * p_Axon)=0; //Adds an axon
     
     virtual void add_Predefined_Axon(c_NT3_Base_Node_NonStrict * p_Axon)=0; //Adds an axon to the 
     
     virtual void add_Predefined_Dendrite(c_NT3_Base_Node_NonStrict * p_Dendrite)=0; //Adds an axon to the 
     
     virtual void expand_Axon(int p_Count)=0; //Adds an axon on the left leg.
     
     virtual void expand_Dendrite(int p_Count)=0; //Adds an axon on the left leg.
     
     virtual c_NT3_Base_Node_NonStrict * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_NonStrict ** p_Nodes, int p_Node_Count)=0;//Searches the axons to see if an upper tier connection exists on the left leg.
     
     virtual bool does_Lower_Connection_Exist(c_NT3_Base_Node_NonStrict ** p_Nodes, int p_Node_Count)=0; //Checks if the given nodes match the dendrites, not just in address but in position.
     
     virtual void add_Dendrite(c_NT3_Base_Node_NonStrict * p_Dendrite)=0; //Sets the left dendrite to the given dendrite.
     
     virtual void reinforce()=0; //Reinforces the node.
     virtual double get_RC_Score()=0; //Returns the Reinforcement score of the node. double (RC_Lvl + (RC_XP / RC_Lvl))
     virtual double get_RC_Lvl()=0; //Returns the current reinforcement Lvl.
     
     virtual void set_State(u_Data_3 p_State)=0; //Sets the state if the node is a state node, otherwise it does nothing.
     virtual long long int get_State()=0; //Returns the state if it is a state node otherwise it does nothing.
     
     virtual int get_Type()=0; //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     virtual void set_Type(int p_Type)=0; //Sets the type of node.
     
     virtual void bp_O()=0; //Initiates a backpropagation that outputs to the given table with the given index.
     virtual void bp()=0; //Initiates a backpropagation that outputs to the given table with the given index.
};

//The normal node to use.
class c_NT3_Node_NonStrict: public c_NT3_Base_Node_NonStrict
{
public:
     
     c_NT3_Node_NonStrict()
     {
          //The next node.
          Next = NULL;
          
          //Axons and their counters.
          Axons = NULL;
          Axon_Count = 0;
          
          //Dendrites.
          Dendrites = NULL;
          Dendrite_Count = 0;
          
          //Reinforcement Counters.
          RC_Lvl = 1.0;
               
          //The node type.
          Type = 1;
          
          Ref = NULL;
     }
     
     ~c_NT3_Node_NonStrict()
     {
          /*
          cout << "\n   ~c_NT3_Node_NonStrict " << this << " NID->" << NID.U; cout.flush();
          cout << " Axons_L->" << Axons_L; cout.flush();
          cout << " Axon_Count_L->" << Axon_Count_L; cout.flush();
          cout << " Axons_R->" << Axons_R; cout.flush();
          cout << " Axon_Count_R->" << Axon_Count_R; cout.flush();
          cout << " Next->" << Next; cout.flush();
          */
          
          if (Axons != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count;cou_Index++)
               {
                    Axons[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons;
               Axons = NULL;
          }
          
          if (Dendrites != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count;cou_Index++)
               {
                    Dendrites[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Dendrites;
               Dendrites = NULL;
          }
          
          //cout << " delete Next->" << Next; cout.flush();
          //delete Next; //This one.
          Next = NULL;
          
          Ref = NULL;
     }
     
     
     //==--  Member Functions.
     
     //Adds an axon on the left leg.
     void add_Axon(c_NT3_Base_Node_NonStrict * p_Axon)
     {
          c_NT3_Base_Node_NonStrict ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_NonStrict*[Axon_Count];
          for (int cou_A=0;cou_A<Axon_Count;cou_A++)
          {
               tmp_Axons[cou_A] = Axons[cou_A];
               Axons[cou_A] = NULL;
          }
          
          delete [] Axons;
          Axons = NULL;
          
          Axons = new c_NT3_Base_Node_NonStrict*[Axon_Count + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count;cou_A++)
          {
               Axons[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons[Axon_Count] = p_Axon; 
          Axon_Count++; 
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon(c_NT3_Base_Node_NonStrict * p_Axon)
     {
          Axons[Axon_Count] = p_Axon; 
          Axon_Count++;
     }
     
     //Adds an axon on the left leg.
     void add_Dendrite(c_NT3_Base_Node_NonStrict * p_Dendrite)
     {
          c_NT3_Base_Node_NonStrict ** tmp_Dendrites;
          tmp_Dendrites = new c_NT3_Base_Node_NonStrict*[Dendrite_Count];
          for (int cou_A=0;cou_A<Dendrite_Count;cou_A++)
          {
               tmp_Dendrites[cou_A] = Dendrites[cou_A];
               Dendrites[cou_A] = NULL;
          }
          
          delete [] Dendrites;
          Dendrites = NULL;
          
          Dendrites = new c_NT3_Base_Node_NonStrict*[Dendrite_Count + 1]; 
          
          for (int cou_A=0;cou_A<Dendrite_Count;cou_A++)
          {
               Dendrites[cou_A] = tmp_Dendrites[cou_A];
               tmp_Dendrites[cou_A] = NULL;
          }
          delete [] tmp_Dendrites;
          tmp_Dendrites = NULL;
          
          Dendrites[Dendrite_Count] = p_Dendrite; 
          Dendrite_Count++; 
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Dendrite(c_NT3_Base_Node_NonStrict * p_Dendrite)
     {
          Dendrites[Dendrite_Count] = p_Dendrite; 
          Dendrite_Count++;
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon(int p_Count)
     {
          if (Axons != NULL){ delete [] Axons; }
          Axons = NULL;
          
          Axons = new c_NT3_Base_Node_NonStrict*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons[cou_A] = NULL;
          }
           
          Axon_Count = 0; 
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Dendrite(int p_Count)
     {
          if (Dendrites != NULL){ delete [] Dendrites; }
          Dendrites = NULL;
          
          Dendrites = new c_NT3_Base_Node_NonStrict*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Dendrites[cou_A] = NULL;
          }
           
          Dendrite_Count = 0; 
     }
     
     //Searches the axons to see if an upper tier connection exists.
     //A block of nodes is given, if the nodes do not match the axons in the right order it is NULL.
     c_NT3_Base_Node_NonStrict * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_NonStrict ** p_Nodes, int p_Node_Count)
     {
          //Check the axons on the right side for the 
          for (int cou_A=0;cou_A<Axon_Count;cou_A++)
          {
               if (Axons[cou_A]->does_Lower_Connection_Exist(p_Nodes, p_Node_Count))
               {
                    return Axons[cou_A];
               }
          }
          return NULL; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist(c_NT3_Base_Node_NonStrict ** p_Nodes, int p_Node_Count)
     { 
          //If there are not enough dendrites then return 0.
          if (Dendrite_Count < p_Node_Count){ return 0; }
          
          //If the position of the dendrite does not match then return 0;
          for (int cou_Index=0;cou_Index<p_Node_Count;cou_Index++)
          {
               if (Dendrites[cou_Index] != p_Nodes[cou_Index]){ return 0; }
          }
          return 1; 
     }
     
     //Reinforces the node.
     void reinforce()
     {
          RC_Lvl += double (1.0f / int (RC_Lvl));
     }
     
     //Returns the Reinforcement score of the node. double (RC_Lvl + (RC_XP / RC_Lvl))
     double get_RC_Score(){ return RC_Lvl; }
     
     //Returns the current reinforcement Lvl.
     double get_RC_Lvl(){ return RC_Lvl; }
     
     //Sets the state if the node is a state node, otherwise it does nothing.
     void set_State(u_Data_3 p_State){}
     
     //Returns the state if it is a state node otherwise it does nothing.
     long long int get_State(){ return 0; }
     
     //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     int get_Type(){ return Type; }
     
     //Sets the type of node.
     void set_Type(int p_Type){ Type = p_Type; }
     
     //Initiates a backpropagation that outputs to the given table with the given index.
     void bp_O() 
     {
          cout << "<";
          if (Dendrites != NULL)
          { 
               for (int cou_Index=0;cou_Index<Dendrite_Count;cou_Index++)
               {
                    Dendrites[cou_Index]->bp(); 
               }
          } 
          else 
          {
               char tmp_State = char (get_State());
               if (tmp_State == char (0)){ tmp_State = '0'; }
               if (tmp_State == char (7)){ tmp_State = '7'; }
               if (tmp_State == char (8)){ tmp_State = '8'; }
               if (tmp_State == char (9)){ tmp_State = '9'; }
               if (tmp_State == char (10)){ tmp_State = 'a'; }
               if (tmp_State == char (13)){ tmp_State = 'd'; }
               cout << tmp_State;
          }
          cout << ">";
     }
     
     //bp_Output the left node.
     void bp()
     {
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (Dendrites != NULL)
          {
               for (int cou_Index=0;cou_Index<Dendrite_Count;cou_Index++)
               {
                    if (Dendrites[cou_Index] != NULL){ Dendrites[cou_Index]->bp(); }
               }
          }
          else
          {
                    char tmp_State = char (get_State());
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    cout << tmp_State;
          }
     }
};

//State node.
class c_NT3_State_Node_NonStrict: public c_NT3_Base_Node_NonStrict
{
private:
     
public:
     
     //The state.
     u_Data_3 State;
     
     c_NT3_State_Node_NonStrict()
     {
          State.I = 0;
          
          //The linked list.
          Next = NULL;
          
          //Axons and their counters.
          Axons = NULL;
          Axon_Count = 0;
          
          //Dendrites.
          Dendrites = NULL;
          Dendrite_Count = 0;
          
          //Reinforcement Counters.
          RC_Lvl = 1.0;
          
          //The nodes type.
          Type = 0;
          
          Ref = NULL;
     }
     
     ~c_NT3_State_Node_NonStrict()
     {
          //cout << " ~c_NT3_State_Node_NonStrict " << this << " NID->" << NID.U; cout.flush();
          
          if (Axons != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count;cou_Index++)
               {
                    Axons[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons;
               Axons = NULL;
          }
          
          if (Dendrites != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count;cou_Index++)
               {
                    Dendrites[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Dendrites;
               Dendrites = NULL;
          }
          
          //cout << " delete Next->" << Next; cout.flush();
          
          delete Next; //This one.
          
          //cout << "   ~~~c_NT3_State_Node_NonStrict " << this << " NID->" << NID.U; cout.flush();
          
          Ref = NULL;
     }
     
     
     //==--  Member Functions.
     
     //Adds an axon on the left leg.
     void add_Axon(c_NT3_Base_Node_NonStrict * p_Axon)
     {
          c_NT3_Base_Node_NonStrict ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_NonStrict*[Axon_Count];
          for (int cou_A=0;cou_A<Axon_Count;cou_A++)
          {
               tmp_Axons[cou_A] = Axons[cou_A];
               Axons[cou_A] = NULL;
          }
          
          delete [] Axons;
          Axons = NULL;
          
          Axons = new c_NT3_Base_Node_NonStrict*[Axon_Count + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count;cou_A++)
          {
               Axons[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons[Axon_Count] = p_Axon; 
          Axon_Count++; 
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon(c_NT3_Base_Node_NonStrict * p_Axon)
     {
          Axons[Axon_Count] = p_Axon; 
          Axon_Count++;
     }
          
     //Adds an axon on the left leg.
     void add_Dendrite(c_NT3_Base_Node_NonStrict * p_Dendrite)
     {
          c_NT3_Base_Node_NonStrict ** tmp_Dendrites;
          tmp_Dendrites = new c_NT3_Base_Node_NonStrict*[Dendrite_Count];
          for (int cou_A=0;cou_A<Dendrite_Count;cou_A++)
          {
               tmp_Dendrites[cou_A] = Dendrites[cou_A];
               Dendrites[cou_A] = NULL;
          }
          
          delete [] Dendrites;
          Dendrites = NULL;
          
          Dendrites = new c_NT3_Base_Node_NonStrict*[Dendrite_Count + 1]; 
          
          for (int cou_A=0;cou_A<Dendrite_Count;cou_A++)
          {
               Dendrites[cou_A] = tmp_Dendrites[cou_A];
               tmp_Dendrites[cou_A] = NULL;
          }
          delete [] tmp_Dendrites;
          tmp_Dendrites = NULL;
          
          Dendrites[Dendrite_Count] = p_Dendrite; 
          Dendrite_Count++; 
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Dendrite(c_NT3_Base_Node_NonStrict * p_Dendrite)
     {
          Dendrites[Dendrite_Count] = p_Dendrite; 
          Dendrite_Count++;
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon(int p_Count)
     {
          if (Axons != NULL){ delete [] Axons; }
          Axons = NULL;
          
          Axons = new c_NT3_Base_Node_NonStrict*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons[cou_A] = NULL;
          }
           
          Axon_Count = 0; 
     }
          
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Dendrite(int p_Count)
     {
          if (Dendrites != NULL){ delete [] Dendrites; }
          Dendrites = NULL;
          
          Dendrites = new c_NT3_Base_Node_NonStrict*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Dendrites[cou_A] = NULL;
          }
           
          Dendrite_Count = 0; 
     }
     
     //Searches the axons to see if an upper tier connection exists.
     //A block of nodes is given, if the nodes do not match the axons in the right order it is NULL.
     c_NT3_Base_Node_NonStrict * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_NonStrict ** p_Nodes, int p_Node_Count)
     {
          //Check the axons on the right side for the 
          for (int cou_A=0;cou_A<Axon_Count;cou_A++)
          {
               if (Axons[cou_A]->does_Lower_Connection_Exist(p_Nodes, p_Node_Count))
               {
                    return Axons[cou_A];
               }
          }
          return NULL; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist(c_NT3_Base_Node_NonStrict ** p_Nodes, int p_Node_Count)
     { 
          //If there are not enough dendrites then return 0.
          if (Dendrite_Count < p_Node_Count){ return 0; }
          
          //If the position of the dendrite does not match then return 0;
          for (int cou_Index=0;cou_Index<p_Node_Count;cou_Index++)
          {
               if (Dendrites[cou_Index] != p_Nodes[cou_Index]){ return 0; }
          }
          return 1; 
     }
     
     //Reinforces the node.
     void reinforce()
     {
          RC_Lvl += double (1.0f / int (RC_Lvl));
     }
     
     //Returns the Reinforcement score of the node. double (RC_Lvl + (RC_XP / RC_Lvl))
     double get_RC_Score(){ return RC_Lvl; }
     
     //Returns the current reinforcement Lvl.
     double get_RC_Lvl(){ return RC_Lvl; }
     
     //Sets the state if the node is a state node, otherwise it does nothing.
     void set_State(u_Data_3 p_State){ State = p_State; }
     
     //Returns the state if it is a state node otherwise it does nothing.
     long long int get_State(){ return State.I; }
     
     //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     int get_Type(){ return Type; }
     
     //Sets the type of node.
     void set_Type(int p_Type){ Type = p_Type; }
     
     //Initiates a backpropagation that outputs to the given table with the given index.
     void bp_O() 
     {
          cout << "<";
          if (Dendrites != NULL)
          { 
               for (int cou_Index=0;cou_Index<Dendrite_Count;cou_Index++)
               {
                    Dendrites[cou_Index]->bp(); 
               }
          } 
          else
          {
                    char tmp_State = char (get_State());
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    cout << tmp_State;
          }
          cout << ">";
     }
     
     //bp_Output the left node.
     void bp()
     {
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (Dendrites != NULL)
          {
               for (int cou_Index=0;cou_Index<Dendrite_Count;cou_Index++)
               {
                    if (Dendrites[cou_Index] != NULL){ Dendrites[cou_Index]->bp(); }
               }
          }
          else
          {
                    char tmp_State = char (get_State());
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    cout << tmp_State;
          }
     }
};

