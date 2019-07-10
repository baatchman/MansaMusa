//This node extends the c_Base_Class to allow for reinforcement counters.
class c_State_Node_1D: public c_Base_Node_1D

{
private:
     
     //The state for the node, only one item long as it is the raw tier.
     u_Data State;
     
     //int Axon_Count[2];
     //int Dendrite_Count[2];
     
     
public:
     
     //Initialize the State node.
     c_State_Node_1D()
     {
          Axon_Count[0] = 0;
          Axon_Count[1] = 0;
          
          //Dendrite_Count[0] = 0;
          //Dendrite_Count[1] = 0;
          
          
          //Sets the state to the given state.
          State.NR = 0;
          
          //Initiate the axons with a NULL.
          Axons = NULL;
          //Axons = new c_Base_Node_1D**[2];
          
          //Axons[0] = new c_Base_Node_1D*[1];
          //Axons[0] [0] = NULL;
          
          //Axons[1] = new c_Base_Node_1D*[1];
          //Axons[1] [0] = NULL;
          
          //Initialize the dendrites with the number of legs passed to the node.
          
          Dendrites = new c_Base_Node_1D*[2];
          Dendrites[0] = NULL;
          Dendrites[1] = NULL;
          
     }
     
     //Sets the state of the node.
     void set_State(u_Data p_Data)
     {
          State.NR = p_Data.NR;
     }
     
     u_Data get_State()
     {
          return State;
     }
     
     //Sets the treetop of the node.
     virtual void set_Treetop(u_Data State){ }
     u_Data get_Treetop(){ u_Data tmp_Data; tmp_Data.I = 0; return tmp_Data; }
          
     //The coordinates for the treetop nodes.
     int get_CID(){ return 0; }
     int set_CID(int p_CID){ return 0; }
     int get_Tier(){ return 0; }
     int set_Tier(int p_Tier){ return 0; }
     int get_Index(){ return Index; }
     int set_Index(int p_Index){ Index = p_Index; return Index; }
     
     
     //Reinforces the node.
     void reinforce()
     {
          
     }
     
     int get_Reinforcement_Counter()
     {
          return 1;
     }
     
     int get_Reinforcement_Counter_Level()
     {
          return 1;
     }
     
     float get_RC_Score()
     {
          return 1.0;
     }
     
     int set_Reinforcement_Counter(int p_RC)
     {
          return 1;
     }
     
     int set_Reinforcement_Counter_Level(int p_RC_LVL)
     {
          return 1;
     }
     
     void output_Node()
     {
          cout << " -" << this << " S->" << State.I;
          
          cout << " ~D";
          cout << " &" << Dendrites[0];
          cout << " &" << Dendrites[1];
          
          cout << " ~A[0]";
               
          for (int cou_A=0;cou_A<get_Axon_Count(0);cou_A++)
          {
               cout << " &" << Axons[0] [cou_A];
          }
               
          cout << " ~A[1]";
          
          for (int cou_A=0;cou_A<get_Axon_Count(1);cou_A++)
          {
               cout << " &" << Axons[1] [cou_A];
          }
     }
     
     void output_Node_Header()
     {
          cout << " -" << this << " NR:" << State.I;
     }
     
     string get_Type()
     {
          return "State_Node";
     }
     
};


