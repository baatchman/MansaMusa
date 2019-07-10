
//This is the abstract base class that the other nodes will be based off of.
class c_MT_Base_Node_1D
{
protected:
     
public:
     
     c_MT_Base_Node_1D *** Axons;
     c_MT_Base_Node_1D ** Dendrites;
     int Axon_Count[2];
     //int Dendrite_Count[2];
     
     int Index;
     
     //Initialize the base node.
     c_MT_Base_Node_1D()
     {
          Axon_Count[0] = 0;
          Axon_Count[1] = 0;
          
          //Dendrite_Count[0] = 0;
          //Dendrite_Count[1] = 0;
          
          //Initiate the axons with a NULL.
          Axons = NULL;
          //Axons = new c_MT_Base_Node_1D**[2];
          
          //Axons[0] = new c_MT_Base_Node_1D*[1];
          //Axons[0] [0] = NULL;
          
          //Axons[1] = new c_MT_Base_Node_1D*[1];
          //Axons[1] [0] = NULL;
          
          //Initialize the dendrites with the number of legs passed to the node.
          Dendrites = new c_MT_Base_Node_1D*[2];
          Dendrites[0] = NULL;
          Dendrites[1] = NULL;
          
          Index = 0;
     }
     
     //Deconstructor.
     ~c_MT_Base_Node_1D()
     {
          delete [] Dendrites;
          Dendrites = NULL;
          
          delete [] Axons[0];
          Axons[0] = NULL;
          
          delete [] Axons[1];
          Axons[1] = NULL;
          
          delete [] Axons;
          Axons = NULL;
     }
     
     
     //--     Base members.
     
     void add_Axon(int p_Leg, c_MT_Base_Node_1D * p_Node)
     {
          //If no Axons then allocate them.
          if (Axons == NULL)
          {
               Axons = new c_MT_Base_Node_1D**[2];
               Axons[0] = NULL;
               Axons[1] = NULL;
          }
          
          if (Axons[p_Leg] == NULL)
          { 
               //Reallocate the leg to be larger.
               Axons[p_Leg] = new c_MT_Base_Node_1D * [1];
               
               //Set the newly allocated axon to the given node.
               Axons[p_Leg] [0] = p_Node;
               
               //Increases the count for the current leg.
               Axon_Count[p_Leg]++;
               
               return;
          }
          
          //Setup the temporary array.
          c_MT_Base_Node_1D ** tmp_Array;
          tmp_Array = new c_MT_Base_Node_1D * [Axon_Count[p_Leg] + 1];
          
          //Copy the old addresses into the temporary array.
          for (int cou_I=0;cou_I<Axon_Count[p_Leg];cou_I++)
          {
               tmp_Array[cou_I] = Axons[p_Leg] [cou_I];
          }
          
          //Assign the given node.
          tmp_Array[Axon_Count[p_Leg]] = p_Node;
          
          //Prep the leg of the Axon for expansion.
          delete [] Axons[p_Leg];
          
          //Reallocate the leg to be larger.
          Axons[p_Leg] = new c_MT_Base_Node_1D * [Axon_Count[p_Leg] + 1];
          
          //Copy the old info into the reallocated array.
          for (int cou_I=0;cou_I<Axon_Count[p_Leg] + 1;cou_I++)
          {
               Axons[p_Leg] [cou_I] = tmp_Array [cou_I];
          }
          
          //Cleanup the temporary array to avoid memory leaks.
          delete [] tmp_Array;
          
          //Increment the axon count.
          Axon_Count[p_Leg]++;
     }
     
     //Loops through the axons for the given leg until a NULL is reached, in which case the count is returned.
     int get_Axon_Count(int p_Leg)
     {
          return Axon_Count[p_Leg];
     }
     
     //Sets a dendrite to the given node.
     void set_Dendrite(int p_Leg, c_MT_Base_Node_1D * p_Node)
     {
          //Set the dendrite.
          Dendrites[p_Leg] = p_Node;
     }
     
     //Returns whether or not a connection exists on the given leg.
     virtual bool does_Lower_Connection_Exist(int p_Leg, c_MT_Base_Node_1D * p_Node)
     {
          return (Dendrites[p_Leg] == p_Node);
     }
     
     //Returns true of false depending on if an upper tier connection already exists.
     //Always checks with the current node being the left leg.
     virtual c_MT_Base_Node_1D * does_Upper_Tier_Connection_Exist(c_MT_Base_Node_1D * p_Right)
     {
          //Check the axons, if it does not connect the asnwer is no.
          for (int cou_A=0;cou_A<Axon_Count[0];cou_A++)
          {
               //If a connection is found then return the node.
               if (Axons[0] [cou_A]->does_Lower_Connection_Exist(1, p_Right))
               {
                    return Axons[0] [cou_A];
               }
          }
          
          //If no connection was found return 0.
          return NULL;
     }
     

     
     //--     PURE VIRTUAL MEMBERS
     
     //Abstract members.
     virtual void reinforce()=0;
     virtual int get_Reinforcement_Counter()=0;
     virtual int get_Reinforcement_Counter_Level()=0;
     virtual int set_Reinforcement_Counter(int p_RC)=0;
     virtual int set_Reinforcement_Counter_Level(int p_RC_LVL)=0;
     virtual float get_RC_Score()=0;
     virtual void set_State(u_Data State)=0;
     virtual u_Data get_State()=0;
     virtual void set_Treetop(u_Data State)=0;
     virtual u_Data get_Treetop()=0;
     virtual string get_Type()=0;
     
     //The coordinates for the treetop nodes.
     virtual int get_CID()=0;
     virtual int set_CID(int p_CID)=0;
     virtual int get_Tier()=0;
     virtual int set_Tier(int p_Tier)=0;
     virtual int get_Index()=0;
     virtual int set_Index(int p_Index)=0;
     
     //Output the node.
     virtual void output_Node()=0;
     virtual void output_Node_Header()=0;
};
