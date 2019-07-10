//This class extends the base node class.

class c_Simple_Node: public c_Base_Node_1D
{
private:
     
public:
     
     //There is no RC for simple nodes so these function do nothing and return nothing.
     
     //Reinforces the node if this were a node with a reinforcement counter.
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
     
     //Sets the state were this a state node.
     void set_State(u_Data p_Data)
     {
     }
     
     u_Data get_State()
     {
          u_Data tmp_Data;
          tmp_Data.I = 0;
          return tmp_Data;
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
     
     //Outputs the node.
     void output_Node()
     {
          cout << " -" << this;
          
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
     
     //Outputs the node header.
     void output_Node_Header()
     {
          cout << " -" << this;
     }
     
     string get_Type()
     {
          return "Simple_Node";
     }
};