//The basic polymorphic node to use for the node network.
class c_NT3_Base_Node_MMap_2D
{
public:
    
	 c_NT3_Base_Node_MMap_2D()
	 {
		 //The next node.
		 Next = NULL;

		 //Axons and their counters.
		 Axons_0 = NULL;
		 Axon_Count_0 = 0;

		 Axons_1 = NULL;
		 Axon_Count_1 = 0;

		 Axons_2 = NULL;
		 Axon_Count_2 = 0;

		 Axons_3 = NULL;
		 Axon_Count_3 = 0;

		 //Dendrites.
		 Dendrite_0 = NULL;
		 Dendrite_1 = NULL;
		 Dendrite_2 = NULL;
		 Dendrite_3 = NULL;

		 //Reinforcement Counters.
		 RC_Lvl = 1.0;

		 //The node type.
		 Type = 1;

		 Ref = NULL;

		 //Double legged starts at 1 because the charge is divided by this amount, so if two legs are double legged then the charge is halved to compesate,
		 //if there are three matching legs then it is / 3.
		 flg_Double_Legged[0] = 1;
		 flg_Double_Legged[1] = 1;
		 flg_Double_Legged[2] = 1;
		 flg_Double_Legged[3] = 1;
	 }

     virtual ~c_NT3_Base_Node_MMap_2D(){};
     
     //The ID of this node.
     u_Data_3 NID;
     
     //The next node in the Linked List.
     c_NT3_Base_Node_MMap_2D * Next;
     
     //Axons and their counters.
     c_NT3_Base_Node_MMap_2D ** Axons_0;
     int Axon_Count_0;
     
     c_NT3_Base_Node_MMap_2D ** Axons_1;
     int Axon_Count_1;
     
     c_NT3_Base_Node_MMap_2D ** Axons_2;
     int Axon_Count_2;
     
     c_NT3_Base_Node_MMap_2D ** Axons_3;
     int Axon_Count_3;
     
     //Dendrites.
     c_NT3_Base_Node_MMap_2D * Dendrite_0;
     c_NT3_Base_Node_MMap_2D * Dendrite_1;
     c_NT3_Base_Node_MMap_2D * Dendrite_2;
     c_NT3_Base_Node_MMap_2D * Dendrite_3;
     
     //WHether or not the nodes are a doublelegged member.
     BYTE flg_Double_Legged[4];
     
     //Reinforcement Counters.
     double RC_Lvl;
     
     //Whether or not the node is a state node.
     char Type;
     
     //This is used for things such as the buffer trees or anything that relies on references.
     void * Ref;
     
     //This determines whether or not to backpropagate the node when watching for trigger nodes. Trigger nodes can be linked to other nodes.
     bool Trigger;
     
     //Member Functions.
     virtual void add_Axon_0(c_NT3_Base_Node_MMap_2D * p_Axon)=0; //Adds an axon on the  leg.
     virtual void add_Axon_1(c_NT3_Base_Node_MMap_2D * p_Axon)=0; //Adds an axon on the  leg.
     virtual void add_Axon_2(c_NT3_Base_Node_MMap_2D * p_Axon)=0; //Adds an axon on the  leg.
     virtual void add_Axon_3(c_NT3_Base_Node_MMap_2D * p_Axon)=0; //Adds an axon on the  leg.
     
     virtual void add_Predefined_Axon_0(c_NT3_Base_Node_MMap_2D * p_Axon)=0; //Adds an axon on the  leg.
     virtual void add_Predefined_Axon_1(c_NT3_Base_Node_MMap_2D * p_Axon)=0; //Adds an axon on the  leg.
     virtual void add_Predefined_Axon_2(c_NT3_Base_Node_MMap_2D * p_Axon)=0; //Adds an axon on the  leg.
     virtual void add_Predefined_Axon_3(c_NT3_Base_Node_MMap_2D * p_Axon)=0; //Adds an axon on the  leg.
     
     virtual void expand_Axon_0(int p_Count)=0; //Adds an axon on the left leg.
     virtual void expand_Axon_1(int p_Count)=0; //Adds an axon on the right leg.
     virtual void expand_Axon_2(int p_Count)=0; //Adds an axon on the left leg.
     virtual void expand_Axon_3(int p_Count)=0; //Adds an axon on the right leg.
     
     virtual c_NT3_Base_Node_MMap_2D * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_MMap_2D * p_Node[4])=0;//Searches the axons to see if an upper tier connection exists on the left leg.
     
     virtual bool does_Lower_Connection_Exist_L0(c_NT3_Base_Node_MMap_2D * p_Node)=0; //Checks if the given node matches a dendrite on the  leg.
     virtual bool does_Lower_Connection_Exist_L1(c_NT3_Base_Node_MMap_2D * p_Node)=0; //Checks if the given node matches a dendrite on the  leg.
     virtual bool does_Lower_Connection_Exist_L2(c_NT3_Base_Node_MMap_2D * p_Node)=0; //Checks if the given node matches a dendrite on the  leg.
     virtual bool does_Lower_Connection_Exist_L3(c_NT3_Base_Node_MMap_2D * p_Node)=0; //Checks if the given node matches a dendrite on the  leg.
     
     virtual void set_Dendrite_0(c_NT3_Base_Node_MMap_2D * p_Dendrite)=0; //Sets the  dendrite to the given dendrite.
     virtual void set_Dendrite_1(c_NT3_Base_Node_MMap_2D * p_Dendrite)=0; //Sets the  dendrite to the given dendrite.
     virtual void set_Dendrite_2(c_NT3_Base_Node_MMap_2D * p_Dendrite)=0; //Sets the  dendrite to the given dendrite.
     virtual void set_Dendrite_3(c_NT3_Base_Node_MMap_2D * p_Dendrite)=0; //Sets the  dendrite to the given dendrite.
     
     virtual void compute_Double_Legs()=0; //Finds double legs and sets their modifier accordinlgly.
     
     virtual void reinforce()=0; //Reinforces the node.
     virtual double get_RC_Score()=0; //Returns the Reinforcement score of the node. double (RC_Lvl + (RC_XP / RC_Lvl))
     virtual double get_RC_Lvl()=0; //Returns the current reinforcement Lvl.
     
     virtual void set_State(u_Data_3 p_State)=0; //Sets the state if the node is a state node, otherwise it does nothing.
     virtual u_Data_3 get_State()=0; //Returns the state if it is a state node otherwise it does nothing.
     
     virtual int get_Type()=0; //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     virtual void set_Type(int p_Type)=0; //Sets the type of node.
     
     virtual bool get_Trigger()=0; //Gets the current status of the node for determining whether or not to backpropagate upon activation.
     virtual void set_Trigger(bool p_Trigger)=0; //Sets the type of node.
     
     virtual void bp_O()=0; //Initiates a backpropagation that outputs to the given table with the given index.
     virtual void bp_0()=0;
     virtual void bp_00(int p_Leg, int p_X, int p_Y)=0;
     
     virtual void output()=0;
     
     
     //Outputs the node.
     virtual void output_BP_Sim()=0;
     virtual void output_BP_Sim_0()=0;
     virtual void output_BP_Sim_1()=0;
     virtual void output_BP_Sim_2()=0;
     virtual void output_BP_Sim_3()=0;
};

//The normal node to use.
class c_NT3_Node_MMap_2D: public c_NT3_Base_Node_MMap_2D
{
public:
     
     c_NT3_Node_MMap_2D()
     {
          //The next node.
          Next = NULL;
          
          //Axons and their counters.
          Axons_0 = NULL;
          Axon_Count_0 = 0;
          
          Axons_1 = NULL;
          Axon_Count_1 = 0;
          
          Axons_2 = NULL;
          Axon_Count_2 = 0;
          
          Axons_3 = NULL;
          Axon_Count_3 = 0;
          
          //Dendrites.
          Dendrite_0 = NULL;
          Dendrite_1 = NULL;
          Dendrite_2 = NULL;
          Dendrite_3 = NULL;
          
          //Reinforcement Counters.
          RC_Lvl = 1.0;
               
          //The node type.
          Type = 1;
          
          Ref = NULL;
          
          //Double legged starts at 1 because the charge is divided by this amount, so if two legs are double legged then the charge is halved to compesate,
          //if there are three matching legs then it is / 3.
          flg_Double_Legged[0] = 1;
          flg_Double_Legged[1] = 1;
          flg_Double_Legged[2] = 1;
          flg_Double_Legged[3] = 1;
     }
     
     ~c_NT3_Node_MMap_2D()
     {
          /*
          cout << "\n   ~c_NT3_Node_MMap_2D " << this << " NID->" << NID.U; cout.flush();
          cout << " Axons_L->" << Axons_L; cout.flush();
          cout << " Axon_Count_L->" << Axon_Count_L; cout.flush();
          cout << " Axons_R->" << Axons_R; cout.flush();
          cout << " Axon_Count_R->" << Axon_Count_R; cout.flush();
          cout << " Next->" << Next; cout.flush();
          */
          
          if (Axons_0 != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_0;cou_Index++)
               {
                    Axons_0[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_0;
               Axons_0 = NULL;
          }
          
          if (Axons_1 != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_1;cou_Index++)
               {
                    Axons_1[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_1;
               Axons_1 = NULL;
          }
          
          if (Axons_2 != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_2;cou_Index++)
               {
                    Axons_2[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_2;
               Axons_2 = NULL;
          }
          
          if (Axons_3 != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_3;cou_Index++)
               {
                    Axons_3[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_3;
               Axons_3 = NULL;
          }
          
          Dendrite_0 = NULL;
          Dendrite_1 = NULL;
          Dendrite_2 = NULL;
          Dendrite_3 = NULL;
          
          //cout << " delete Next->" << Next; cout.flush();
          //delete Next; //This one.
          Next = NULL;
          
          Ref = NULL;
     }
     
     
     //==--  Member Functions.
     
     //Adds an axon on the left leg.
     void add_Axon_0(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          c_NT3_Base_Node_MMap_2D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_MMap_2D*[Axon_Count_0];
          for (int cou_A=0;cou_A<Axon_Count_0;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_0[cou_A];
               Axons_0[cou_A] = NULL;
          }
          
          delete [] Axons_0;
          Axons_0 = NULL;
          
          Axons_0 = new c_NT3_Base_Node_MMap_2D*[Axon_Count_0 + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_0;cou_A++)
          {
               Axons_0[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_0[Axon_Count_0] = p_Axon; 
          Axon_Count_0++; 
     }
     
     //Adds an axon on the left leg.
     void add_Axon_1(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          c_NT3_Base_Node_MMap_2D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_MMap_2D*[Axon_Count_1];
          for (int cou_A=0;cou_A<Axon_Count_1;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_1[cou_A];
               Axons_1[cou_A] = NULL;
          }
          
          delete [] Axons_1;
          Axons_1 = NULL;
          
          Axons_1 = new c_NT3_Base_Node_MMap_2D*[Axon_Count_1 + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_1;cou_A++)
          {
               Axons_1[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_1[Axon_Count_1] = p_Axon; 
          Axon_Count_1++; 
     }
     
     //Adds an axon on the left leg.
     void add_Axon_2(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          c_NT3_Base_Node_MMap_2D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_MMap_2D*[Axon_Count_2];
          for (int cou_A=0;cou_A<Axon_Count_2;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_2[cou_A];
               Axons_2[cou_A] = NULL;
          }
          
          delete [] Axons_2;
          Axons_2 = NULL;
          
          Axons_2 = new c_NT3_Base_Node_MMap_2D*[Axon_Count_2 + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_2;cou_A++)
          {
               Axons_2[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_2[Axon_Count_2] = p_Axon; 
          Axon_Count_2++; 
     }
     
     //Adds an axon on the left leg.
     void add_Axon_3(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          c_NT3_Base_Node_MMap_2D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_MMap_2D*[Axon_Count_3];
          for (int cou_A=0;cou_A<Axon_Count_3;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_3[cou_A];
               Axons_3[cou_A] = NULL;
          }
          
          delete [] Axons_3;
          Axons_3 = NULL;
          
          Axons_3 = new c_NT3_Base_Node_MMap_2D*[Axon_Count_3 + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_3;cou_A++)
          {
               Axons_3[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_3[Axon_Count_3] = p_Axon; 
          Axon_Count_3++; 
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon_0(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          Axons_0[Axon_Count_0] = p_Axon; 
          Axon_Count_0++;
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon_1(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          Axons_1[Axon_Count_1] = p_Axon; 
          Axon_Count_1++;
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon_2(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          Axons_2[Axon_Count_2] = p_Axon; 
          Axon_Count_2++;
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon_3(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          Axons_3[Axon_Count_3] = p_Axon; 
          Axon_Count_3++;
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon_0(int p_Count)
     {
          if (Axons_0 != NULL){ delete [] Axons_0; }
          Axons_0 = NULL;
          
          Axons_0 = new c_NT3_Base_Node_MMap_2D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_0[cou_A] = NULL;
          }
           
          Axon_Count_0 = 0; 
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon_1(int p_Count)
     {
          if (Axons_1 != NULL){ delete [] Axons_1; }
          Axons_1 = NULL;
          
          Axons_1 = new c_NT3_Base_Node_MMap_2D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_1[cou_A] = NULL;
          }
           
          Axon_Count_1 = 0; 
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon_2(int p_Count)
     {
          if (Axons_2 != NULL){ delete [] Axons_2; }
          Axons_2 = NULL;
          
          Axons_2 = new c_NT3_Base_Node_MMap_2D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_2[cou_A] = NULL;
          }
           
          Axon_Count_2 = 0; 
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon_3(int p_Count)
     {
          if (Axons_3 != NULL){ delete [] Axons_3; }
          Axons_3 = NULL;
          
          Axons_3 = new c_NT3_Base_Node_MMap_2D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_3[cou_A] = NULL;
          }
           
          Axon_Count_3 = 0; 
     }
     
     //Searches the axons to see if an upper tier connection exists.
     c_NT3_Base_Node_MMap_2D * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_MMap_2D * p_Node[4])
     {
          //Check the axons on the right side for the 
          for (int cou_A=0;cou_A<Axon_Count_0;cou_A++)
          {
               if (Axons_0[cou_A]->does_Lower_Connection_Exist_L1(p_Node[1]))
               {
                    if (Axons_0[cou_A]->does_Lower_Connection_Exist_L2(p_Node[2]))
                    {
                         if (Axons_0[cou_A]->does_Lower_Connection_Exist_L3(p_Node[3]))
                         {
                              return Axons_0[cou_A];
                         }
                    }
               }
          }
          return NULL; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist_L0(c_NT3_Base_Node_MMap_2D * p_Node)
     { 
          if (Dendrite_0 == p_Node){ return 1; }
          return 0; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist_L1(c_NT3_Base_Node_MMap_2D * p_Node)
     { 
          if (Dendrite_1 == p_Node){ return 1; }
          return 0; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist_L2(c_NT3_Base_Node_MMap_2D * p_Node)
     { 
          if (Dendrite_2 == p_Node){ return 1; }
          return 0; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist_L3(c_NT3_Base_Node_MMap_2D * p_Node)
     { 
          if (Dendrite_3 == p_Node){ return 1; }
          return 0; 
     }
     
     //Sets the left dendrite to the given dendrite.
     void set_Dendrite_0(c_NT3_Base_Node_MMap_2D * p_Dendrite)
     {
          Dendrite_0 = p_Dendrite;
     }
     
     //Sets the left dendrite to the given dendrite.
     void set_Dendrite_1(c_NT3_Base_Node_MMap_2D * p_Dendrite)
     {
          Dendrite_1 = p_Dendrite;
     }
     
     //Sets the left dendrite to the given dendrite.
     void set_Dendrite_2(c_NT3_Base_Node_MMap_2D * p_Dendrite)
     {
          Dendrite_2 = p_Dendrite;
     }
     
     //Sets the left dendrite to the given dendrite.
     void set_Dendrite_3(c_NT3_Base_Node_MMap_2D * p_Dendrite)
     {
          Dendrite_3 = p_Dendrite;
     }
     
     //Computes double legged nodeness.
     void compute_Double_Legs()
     {
          //Check for double legged nodes
          if (Dendrite_0 == Dendrite_1){ flg_Double_Legged[0]++; flg_Double_Legged[1]++; }
          
          if (Dendrite_0 == Dendrite_2){ flg_Double_Legged[0]++; flg_Double_Legged[2]++; }
          if (Dendrite_1 == Dendrite_2){ flg_Double_Legged[1]++; flg_Double_Legged[2]++; }
          
          if (Dendrite_0 == Dendrite_3){ flg_Double_Legged[0]++; flg_Double_Legged[3]++; }
          if (Dendrite_1 == Dendrite_3){ flg_Double_Legged[1]++; flg_Double_Legged[3]++; }
          if (Dendrite_2 == Dendrite_3){ flg_Double_Legged[2]++; flg_Double_Legged[3]++; }
          
          //cout << "\n\n" << this << " 0:" << int (flg_Double_Legged[0]) << " " << Dendrite_0 << " 1:" << int (flg_Double_Legged[1]) << " " << Dendrite_1  << " 2:" << int (flg_Double_Legged[2]) << " " << Dendrite_2  << " 3:" << int (flg_Double_Legged[3]) << " " << Dendrite_3;
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
     u_Data_3 get_State(){ u_Data_3 tmp_Data; tmp_Data.NR = NULL; return tmp_Data; }
     
     //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     int get_Type(){ return Type; }
     
     //Sets the type of node.
     void set_Type(int p_Type){ Type = p_Type; }
     
     bool get_Trigger(){ return Trigger; } //Gets the current status of the node for determining whether or not to backpropagate upon activation.
     void set_Trigger(bool p_Trigger){ Trigger = p_Trigger; } //Sets the type of node.
     
     //Initiates a backpropagation that outputs to the given table with the given index.
     void bp_O() 
     {
          cout << "\n";
          
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_Y();
          
          ostr(0, 13, "<");
          if (Dendrite_0 != NULL){ Dendrite_0->bp_00(0, tmp_X, tmp_Y); }
          
          tmp_Y++;
          if (Dendrite_1 != NULL){ Dendrite_1->bp_00(1, tmp_X, tmp_Y); }
          tmp_Y--;
          
          tmp_X++;
          if (Dendrite_2 != NULL){ Dendrite_2->bp_00(2, tmp_X, tmp_Y); }
          tmp_X--;
          
          tmp_X++;
          tmp_Y++;
          if (Dendrite_3 != NULL){ Dendrite_3->bp_00(3, tmp_X, tmp_Y); }
          tmp_X--;
          tmp_Y++;
          
          xy(tmp_X, tmp_Y);
          
          ostr(0, 13, ">");
     }
     
     //bp_Output the left node.
     void bp_0()
     {
          bp_00(0, get_Console_Cursor_X(), get_Console_Cursor_Y());
     }
     
     
     
     //bp_Output the left node.
     void bp_00(int p_Leg, int p_X, int p_Y)
     {
          bool flg_State = true;
          
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if ((p_Leg == 0))
          {
               if (Dendrite_0 != NULL){ Dendrite_0->bp_00(0, p_X, p_Y); flg_State = false; }
          }
          
          if ((p_Leg == 0) || (p_Leg == 1))
          {
               p_Y++;
               if (Dendrite_1 != NULL){ Dendrite_1->bp_00(1, p_X, p_Y); flg_State = false; }
               p_Y--;
          }
          
          if ((p_Leg == 0) || (p_Leg == 2))
          {
               p_X++;
               if (Dendrite_2 != NULL){ Dendrite_2->bp_00(2, p_X, p_Y); flg_State = false; }
               p_X--;
          }
          
          if ((p_Leg == 0) || (p_Leg == 1) || (p_Leg == 2) || (p_Leg == 3))
          {
               p_X++;
               p_Y++;
               if (Dendrite_3 != NULL){ Dendrite_3->bp_00(3, p_X, p_Y); flg_State = false; }
               p_X--;
               p_Y++;
          }
          
          if (flg_State == true)
          {
               xy(p_X, p_Y);
               char tmp_State = char ((get_State()).I);
               if (tmp_State == char (0)){ tmp_State = '0'; }
               if (tmp_State == char (7)){ tmp_State = '7'; }
               if (tmp_State == char (8)){ tmp_State = '8'; }
               if (tmp_State == char (9)){ tmp_State = '9'; }
               if (tmp_State == char (10)){ tmp_State = 'a'; }
               if (tmp_State == char (13)){ tmp_State = 'd'; }
               cout << tmp_State;
          }
     }
     
     //Outputs the node.
     void output()
     {
          ostr(0, 9, "{");
          oint(0, 7, NID.I);
          //ostr(0, 12, "0"); cout << Dendrite_0;
          //ostr(0, 13, "1"); cout << Dendrite_1;
          //ostr(0, 14, "2"); cout << Dendrite_2;
          //ostr(0, 15, "3"); cout << Dendrite_3;
          if (Dendrite_0 != NULL){ Dendrite_0->output(); }
          if (Dendrite_2 != NULL){ Dendrite_2->output(); }
          if (Dendrite_1 != NULL){ Dendrite_1->output(); }
          if (Dendrite_3 != NULL){ Dendrite_3->output(); }
          ostr(0, 9, "}");
     }
     
     //Outputs the node.
     void output_BP_Sim()
     {
          ostr(0, 9, "{");
          oint(0, 7, NID.I);
          //ostr(0, 12, "0"); cout << Dendrite_0;
          //ostr(0, 13, "1"); cout << Dendrite_1;
          //ostr(0, 14, "2"); cout << Dendrite_2;
          //ostr(0, 15, "3"); cout << Dendrite_3;
          if (Dendrite_0 != NULL){ Dendrite_0->output_BP_Sim_0(); }
          if (Dendrite_2 != NULL){ Dendrite_2->output_BP_Sim_1(); }
          if (Dendrite_1 != NULL){ Dendrite_1->output_BP_Sim_2(); }
          if (Dendrite_3 != NULL){ Dendrite_3->output_BP_Sim_3(); }
          ostr(0, 9, "}");
     }
     
     //Outputs the node.
     void output_BP_Sim_0()
     {
          ostr(0, 9, "{");
          oint(0, 7, NID.I);
          //ostr(0, 12, "0"); cout << Dendrite_0;
          //ostr(0, 13, "1"); cout << Dendrite_1;
          //ostr(0, 14, "2"); cout << Dendrite_2;
          //ostr(0, 15, "3"); cout << Dendrite_3;
          if (Dendrite_0 != NULL){ Dendrite_0->output_BP_Sim_0(); }
          if (Dendrite_2 != NULL){ Dendrite_2->output_BP_Sim_1(); }
          if (Dendrite_1 != NULL){ Dendrite_1->output_BP_Sim_2(); }
          if (Dendrite_3 != NULL){ Dendrite_3->output_BP_Sim_3(); }
          ostr(0, 9, "}");
     }
     
     //Outputs the node.
     void output_BP_Sim_1()
     {
          ostr(0, 9, "{");
          oint(0, 7, NID.I);
          //ostr(0, 12, "0"); cout << Dendrite_0;
          //ostr(0, 13, "1"); cout << Dendrite_1;
          //ostr(0, 14, "2"); cout << Dendrite_2;
          //ostr(0, 15, "3"); cout << Dendrite_3;
          if (Dendrite_2 != NULL){ Dendrite_2->output_BP_Sim_1(); }
          if (Dendrite_1 != NULL){ Dendrite_1->output_BP_Sim_2(); }
          if (Dendrite_3 != NULL){ Dendrite_3->output_BP_Sim_3(); }
          ostr(0, 9, "}");
     }
     
     //Outputs the node.
     void output_BP_Sim_2()
     {
          ostr(0, 9, "{");
          oint(0, 7, NID.I);
          //ostr(0, 12, "0"); cout << Dendrite_0;
          //ostr(0, 13, "1"); cout << Dendrite_1;
          //ostr(0, 14, "2"); cout << Dendrite_2;
          //ostr(0, 15, "3"); cout << Dendrite_3;
          if (Dendrite_1 != NULL){ Dendrite_1->output_BP_Sim_2(); }
          if (Dendrite_3 != NULL){ Dendrite_3->output_BP_Sim_3(); }
          ostr(0, 9, "}");
     }
     
     //Outputs the node.
     void output_BP_Sim_3()
     {
          ostr(0, 9, "{");
          oint(0, 7, NID.I);
          //ostr(0, 12, "0"); cout << Dendrite_0;
          //ostr(0, 13, "1"); cout << Dendrite_1;
          //ostr(0, 14, "2"); cout << Dendrite_2;
          //ostr(0, 15, "3"); cout << Dendrite_3;
          if (Dendrite_3 != NULL){ Dendrite_3->output_BP_Sim_3(); }
          ostr(0, 9, "}");
     }
};

//State node.
class c_NT3_State_Node_MMap_2D: public c_NT3_Base_Node_MMap_2D
{
private:
     
public:
     
     //The state.
     u_Data_3 State;
     
     c_NT3_State_Node_MMap_2D()
     {
          State.I = 0;
          
          //The linked list.
          Next = NULL;
          
          //Axons and their counters.
          Axons_0 = NULL;
          Axon_Count_0 = 0;
          
          Axons_1 = NULL;
          Axon_Count_1 = 0;
          
          Axons_2 = NULL;
          Axon_Count_2 = 0;
          
          Axons_3 = NULL;
          Axon_Count_3 = 0;
          
          //Dendrites.
          Dendrite_0 = NULL;
          Dendrite_1 = NULL;
          Dendrite_2 = NULL;
          Dendrite_3 = NULL;
          
          //Reinforcement Counters.
          RC_Lvl = 1.0;
          
          //The nodes type.
          Type = 0;
          
          Ref = NULL;
     }
     
     ~c_NT3_State_Node_MMap_2D()
     {
          //cout << " ~c_NT3_State_Node_MMap_2D " << this << " NID->" << NID.U; cout.flush();
          if (Axons_0 != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_0;cou_Index++)
               {
                    Axons_0[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_0;
               Axons_0 = NULL;
          }
          
          if (Axons_1 != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_1;cou_Index++)
               {
                    Axons_1[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_1;
               Axons_1 = NULL;
          }
          
          if (Axons_2 != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_2;cou_Index++)
               {
                    Axons_2[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_2;
               Axons_2 = NULL;
          }
          
          if (Axons_3 != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_3;cou_Index++)
               {
                    Axons_3[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_3;
               Axons_3 = NULL;
          }
          
          Dendrite_0 = NULL;
          Dendrite_1 = NULL;
          Dendrite_2 = NULL;
          Dendrite_3 = NULL;
          
          
          //cout << " delete Next->" << Next; cout.flush();
          
          delete Next; //This one.
          
          //cout << "   ~~~c_NT3_State_Node_MMap_2D " << this << " NID->" << NID.U; cout.flush();
          
          Ref = NULL;
     }
     
     
     //==--  Member Functions.
     
     //Adds an axon on the left leg.
     void add_Axon_0(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          c_NT3_Base_Node_MMap_2D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_MMap_2D*[Axon_Count_0];
          for (int cou_A=0;cou_A<Axon_Count_0;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_0[cou_A];
               Axons_0[cou_A] = NULL;
          }
          
          delete [] Axons_0;
          Axons_0 = NULL;
          
          Axons_0 = new c_NT3_Base_Node_MMap_2D*[Axon_Count_0 + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_0;cou_A++)
          {
               Axons_0[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_0[Axon_Count_0] = p_Axon; 
          Axon_Count_0++; 
     }
     
     //Adds an axon on the left leg.
     void add_Axon_1(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          c_NT3_Base_Node_MMap_2D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_MMap_2D*[Axon_Count_1];
          for (int cou_A=0;cou_A<Axon_Count_1;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_1[cou_A];
               Axons_1[cou_A] = NULL;
          }
          
          delete [] Axons_1;
          Axons_1 = NULL;
          
          Axons_1 = new c_NT3_Base_Node_MMap_2D*[Axon_Count_1 + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_1;cou_A++)
          {
               Axons_1[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_1[Axon_Count_1] = p_Axon; 
          Axon_Count_1++; 
     }
     
     //Adds an axon on the left leg.
     void add_Axon_2(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          c_NT3_Base_Node_MMap_2D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_MMap_2D*[Axon_Count_2];
          for (int cou_A=0;cou_A<Axon_Count_2;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_2[cou_A];
               Axons_2[cou_A] = NULL;
          }
          
          delete [] Axons_2;
          Axons_2 = NULL;
          
          Axons_2 = new c_NT3_Base_Node_MMap_2D*[Axon_Count_2 + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_2;cou_A++)
          {
               Axons_2[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_2[Axon_Count_2] = p_Axon; 
          Axon_Count_2++; 
     }
     
     //Adds an axon on the left leg.
     void add_Axon_3(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          c_NT3_Base_Node_MMap_2D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_MMap_2D*[Axon_Count_3];
          for (int cou_A=0;cou_A<Axon_Count_3;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_3[cou_A];
               Axons_3[cou_A] = NULL;
          }
          
          delete [] Axons_3;
          Axons_3 = NULL;
          
          Axons_3 = new c_NT3_Base_Node_MMap_2D*[Axon_Count_3 + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_3;cou_A++)
          {
               Axons_3[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_3[Axon_Count_3] = p_Axon; 
          Axon_Count_3++; 
     }
     
          
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon_0(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          Axons_0[Axon_Count_0] = p_Axon; 
          Axon_Count_0++;
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon_1(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          Axons_1[Axon_Count_1] = p_Axon; 
          Axon_Count_1++;
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon_2(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          Axons_2[Axon_Count_2] = p_Axon; 
          Axon_Count_2++;
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon_3(c_NT3_Base_Node_MMap_2D * p_Axon)
     {
          Axons_3[Axon_Count_3] = p_Axon; 
          Axon_Count_3++;
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon_0(int p_Count)
     {
          if (Axons_0 != NULL){ delete [] Axons_0; }
          Axons_0 = NULL;
          
          Axons_0 = new c_NT3_Base_Node_MMap_2D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_0[cou_A] = NULL;
          }
           
          Axon_Count_0 = 0; 
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon_1(int p_Count)
     {
          if (Axons_1 != NULL){ delete [] Axons_1; }
          Axons_1 = NULL;
          
          Axons_1 = new c_NT3_Base_Node_MMap_2D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_1[cou_A] = NULL;
          }
           
          Axon_Count_1 = 0; 
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon_2(int p_Count)
     {
          if (Axons_2 != NULL){ delete [] Axons_2; }
          Axons_2 = NULL;
          
          Axons_2 = new c_NT3_Base_Node_MMap_2D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_2[cou_A] = NULL;
          }
           
          Axon_Count_2 = 0; 
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon_3(int p_Count)
     {
          if (Axons_3 != NULL){ delete [] Axons_3; }
          Axons_3 = NULL;
          
          Axons_3 = new c_NT3_Base_Node_MMap_2D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_3[cou_A] = NULL;
          }
           
          Axon_Count_3 = 0; 
     }
     
     
     //Searches the axons to see if an upper tier connection exists.
     c_NT3_Base_Node_MMap_2D * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_MMap_2D * p_Node[4])
     {
          //Check the axons on the right side for the 
          for (int cou_A=0;cou_A<Axon_Count_0;cou_A++)
          {
               if (Axons_0[cou_A]->does_Lower_Connection_Exist_L1(p_Node[1]))
               {
                    if (Axons_0[cou_A]->does_Lower_Connection_Exist_L2(p_Node[2]))
                    {
                         if (Axons_0[cou_A]->does_Lower_Connection_Exist_L3(p_Node[3]))
                         {
                              return Axons_0[cou_A];
                         }
                    }
               }
          }
          return NULL; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist_L0(c_NT3_Base_Node_MMap_2D * p_Node)
     { 
          if (Dendrite_0 == p_Node){ return 1; }
          return 0; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist_L1(c_NT3_Base_Node_MMap_2D * p_Node)
     { 
          if (Dendrite_1 == p_Node){ return 1; }
          return 0; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist_L2(c_NT3_Base_Node_MMap_2D * p_Node)
     { 
          if (Dendrite_2 == p_Node){ return 1; }
          return 0; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist_L3(c_NT3_Base_Node_MMap_2D * p_Node)
     { 
          if (Dendrite_3 == p_Node){ return 1; }
          return 0; 
     }
     
     //Sets the left dendrite to the given dendrite.
     void set_Dendrite_0(c_NT3_Base_Node_MMap_2D * p_Dendrite)
     {
          Dendrite_0 = p_Dendrite;
     }
     
     //Sets the left dendrite to the given dendrite.
     void set_Dendrite_1(c_NT3_Base_Node_MMap_2D * p_Dendrite)
     {
          Dendrite_1 = p_Dendrite;
     }
     
     //Sets the left dendrite to the given dendrite.
     void set_Dendrite_2(c_NT3_Base_Node_MMap_2D * p_Dendrite)
     {
          Dendrite_2 = p_Dendrite;
     }
     
     //Sets the left dendrite to the given dendrite.
     void set_Dendrite_3(c_NT3_Base_Node_MMap_2D * p_Dendrite)
     {
          Dendrite_3 = p_Dendrite;
     }
     
     //Computes double legged nodeness, in the state node there should be none.
     void compute_Double_Legs()
     {
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
     u_Data_3 get_State(){ return State; }
     
     //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     int get_Type(){ return Type; }
     
     //Sets the type of node.
     void set_Type(int p_Type){ Type = p_Type; }
     
     bool get_Trigger(){ return Trigger; } //Gets the current status of the node for determining whether or not to backpropagate upon activation.
     void set_Trigger(bool p_Trigger){ Trigger = p_Trigger; } //Sets the type of node.
     
     //Initiates a backpropagation that outputs to the given table with the given index.
     void bp_O() 
     {
          cout << "~<SN-";
          cout << State.F;
          cout << ">~";
     }
     
     //bp_Output the left node.
     void bp_0()
     {
          bp_00(0, get_Console_Cursor_X(), get_Console_Cursor_Y());
     }
     
     
     
     //bp_Output the left node.
     void bp_00(int p_Leg, int p_X, int p_Y)
     {
          bool flg_State = true;
          
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if ((p_Leg == 0))
          {
               if (Dendrite_0 != NULL){ Dendrite_0->bp_00(0, p_X, p_Y); flg_State = false; }
          }
          
          if ((p_Leg == 0) || (p_Leg == 1))
          {
               p_Y++;
               if (Dendrite_1 != NULL){ Dendrite_1->bp_00(1, p_X, p_Y); flg_State = false; }
               p_Y--;
          }
          
          if ((p_Leg == 0) || (p_Leg == 2))
          {
               p_X++;
               if (Dendrite_2 != NULL){ Dendrite_2->bp_00(2, p_X, p_Y); flg_State = false; }
               p_X--;
          }
          
          if ((p_Leg == 0) || (p_Leg == 1) || (p_Leg == 2) || (p_Leg == 3))
          {
               p_X++;
               p_Y++;
               if (Dendrite_3 != NULL){ Dendrite_3->bp_00(3, p_X, p_Y); flg_State = false; }
               p_X--;
               p_Y--;
          }
          
          if (flg_State == true)
          {
               xy(p_X, p_Y);
               char tmp_State = char ((get_State()).I);
               if (tmp_State == char (0)){ tmp_State = '0'; }
               if (tmp_State == char (7)){ tmp_State = '7'; }
               if (tmp_State == char (8)){ tmp_State = '8'; }
               if (tmp_State == char (9)){ tmp_State = '9'; }
               if (tmp_State == char (10)){ tmp_State = 'a'; }
               if (tmp_State == char (13)){ tmp_State = 'd'; }
               cout << tmp_State;
               
               cout << "\n"; ostr(0, 7, "("); oint(0, 13, p_X); ostr(0, 7, ", "); oint(0, 13, p_Y); ostr(0, 7, ") "); oflt(0, 13, (get_State()).F);
          }
     }
     
     //Outputs the node.
     void output()
     {
          ostr(0, 9, " {");
          oint(0, 7, NID.I);
          oint(0, 160, (get_State()).F);
          //ostr(0, 6, "0"); cout << Dendrite_0;
          //ostr(0, 13, "1"); cout << Dendrite_1;
          //ostr(0, 14, "2"); cout << Dendrite_2;
          //ostr(0, 15, "3"); cout << Dendrite_3;
          if (Dendrite_0 != NULL){ Dendrite_0->output(); }
          if (Dendrite_2 != NULL){ Dendrite_2->output(); }
          if (Dendrite_1 != NULL){ Dendrite_1->output(); }
          if (Dendrite_3 != NULL){ Dendrite_3->output(); }
          ostr(0, 9, "}");
     }
     
          //Outputs the node.
     void output_BP_Sim()
     {
          ostr(0, 9, "{");
          oint(0, 7, NID.I);
          oint(0, 160, (get_State()).I);
          //ostr(0, 12, "0"); cout << Dendrite_0;
          //ostr(0, 13, "1"); cout << Dendrite_1;
          //ostr(0, 14, "2"); cout << Dendrite_2;
          //ostr(0, 15, "3"); cout << Dendrite_3;
          if (Dendrite_0 != NULL){ Dendrite_0->output_BP_Sim_0(); }
          if (Dendrite_2 != NULL){ Dendrite_2->output_BP_Sim_1(); }
          if (Dendrite_1 != NULL){ Dendrite_1->output_BP_Sim_2(); }
          if (Dendrite_3 != NULL){ Dendrite_3->output_BP_Sim_3(); }
          ostr(0, 9, "}");
     }
     
     //Outputs the node.
     void output_BP_Sim_0()
     {
          ostr(0, 9, "{");
          oint(0, 7, NID.I);
          oint(0, 160, (get_State()).I);
          //ostr(0, 12, "0"); cout << Dendrite_0;
          //ostr(0, 13, "1"); cout << Dendrite_1;
          //ostr(0, 14, "2"); cout << Dendrite_2;
          //ostr(0, 15, "3"); cout << Dendrite_3;
          if (Dendrite_0 != NULL){ Dendrite_0->output_BP_Sim_0(); }
          if (Dendrite_2 != NULL){ Dendrite_2->output_BP_Sim_1(); }
          if (Dendrite_1 != NULL){ Dendrite_1->output_BP_Sim_2(); }
          if (Dendrite_3 != NULL){ Dendrite_3->output_BP_Sim_3(); }
          ostr(0, 9, "}");
     }
     
     //Outputs the node.
     void output_BP_Sim_1()
     {
          ostr(0, 9, "{");
          oint(0, 7, NID.I);
          oint(0, 160, (get_State()).I);
          //ostr(0, 12, "0"); cout << Dendrite_0;
          //ostr(0, 13, "1"); cout << Dendrite_1;
          //ostr(0, 14, "2"); cout << Dendrite_2;
          //ostr(0, 15, "3"); cout << Dendrite_3;
          if (Dendrite_2 != NULL){ Dendrite_2->output_BP_Sim_1(); }
          if (Dendrite_1 != NULL){ Dendrite_1->output_BP_Sim_2(); }
          if (Dendrite_3 != NULL){ Dendrite_3->output_BP_Sim_3(); }
          ostr(0, 9, "}");
     }
     
     //Outputs the node.
     void output_BP_Sim_2()
     {
          ostr(0, 9, "{");
          oint(0, 7, NID.I);
          oint(0, 160, (get_State()).I);
          //ostr(0, 12, "0"); cout << Dendrite_0;
          //ostr(0, 13, "1"); cout << Dendrite_1;
          //ostr(0, 14, "2"); cout << Dendrite_2;
          //ostr(0, 15, "3"); cout << Dendrite_3;
          if (Dendrite_1 != NULL){ Dendrite_1->output_BP_Sim_2(); }
          if (Dendrite_3 != NULL){ Dendrite_3->output_BP_Sim_3(); }
          ostr(0, 9, "}");
     }
     
     //Outputs the node.
     void output_BP_Sim_3()
     {
          ostr(0, 9, "{");
          oint(0, 7, NID.I);
          oint(0, 160, (get_State()).I);
          //ostr(0, 12, "0"); cout << Dendrite_0;
          //ostr(0, 13, "1"); cout << Dendrite_1;
          //ostr(0, 14, "2"); cout << Dendrite_2;
          //ostr(0, 15, "3"); cout << Dendrite_3;
          if (Dendrite_3 != NULL){ Dendrite_3->output_BP_Sim_3(); }
          ostr(0, 9, "}");
     }

};

