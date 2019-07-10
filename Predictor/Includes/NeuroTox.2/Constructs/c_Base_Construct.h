//This class allows for polymorphic manipulation of constructs from the assembly.
class c_Base_Construct
{
protected:


public:

     //The ID to be used for all of the constructs.
     static int UCID;

     c_Base_Construct()
     {

     }

     virtual ~c_Base_Construct()
     {

     }


     virtual int get_Input_Table_ID()=0;
     virtual int get_Input_Table_Truth_ID()=0;

     virtual int get_Output_Table_Charge_ID()=0;
     virtual int get_Output_Table_Pattern_ID()=0;
     virtual int get_Output_Table_Nodes_ID()=0;
     virtual int get_Output_Table_Truth_ID()=0;
     virtual int get_Output_Table_RC_ID()=0;

     virtual int get_Settings_Table_ID()=0;
     virtual int get_Internal_Table_ID()=0;
     virtual int get_Treetop_Table_ID()=0;


     //Gets the Name of the table.
     virtual string get_Input_Table_Name()=0;
     virtual string get_Input_Table_Truth_Name()=0;

     virtual string get_Output_Table_Charge_Name()=0;
     virtual string get_Output_Table_Pattern_Name()=0;
     virtual string get_Output_Table_Nodes_Name()=0;
     virtual string get_Output_Table_Truth_Name()=0;
     virtual string get_Output_Table_RC_Name()=0;
     virtual string get_Output_Table_Vision_Name()=0;

     virtual string get_Settings_Table_Name()=0;
     virtual string get_Internal_Table_Name()=0;
     virtual string get_Treetop_Table_Name()=0;

     //Gets a reference to a table.
     virtual c_Table_1D * get_Input_Table_Reference()=0;
     virtual c_Table_1D * get_Input_Table_Truth_Reference()=0;

     virtual c_Table_1D * get_Output_Table_Charge_Reference()=0;
     virtual c_Table_1D * get_Output_Table_Pattern_Reference()=0;
     virtual c_Table_1D * get_Output_Table_Nodes_Reference()=0;
     virtual c_Table_1D * get_Output_Table_Truth_Reference()=0;
     virtual c_Table_1D * get_Output_Table_RC_Reference()=0;
     virtual c_Table_1D * get_Output_Table_Vision_Reference()=0;

     virtual c_Table_1D * get_Settings_Table_Reference()=0;
     virtual c_Table_1D * get_Internal_Table_Reference()=0;
     virtual c_Table_1D * get_Treetop_Table_Reference()=0;

     virtual void update_Settings_Table()=0;
     virtual void update_Internal_Table()=0;
     virtual void update_Settings()=0;

     virtual void change_Settings(string p_Construct_Name, string p_Assembly_Name, int p_Construct_Input_Type, int p_Base_Charge, float p_Filter_Charge, int p_Charge_Precision, int p_Query_Cap, int p_Charging_Tier, int p_Tolerance)=0;

     virtual void set_Construct_Name(string p_Construct_Name)=0;
     virtual void set_Assembly_Name(string p_Assembly)=0;

     virtual string get_Construct_Name()=0;
     virtual string get_Assembly_Name()=0;

     virtual int get_Construct_ID()=0;

     virtual int set_Input_Type(int p_Type)=0;
     virtual int get_Input_Type()=0;

     virtual void set_Input(int p_Row, int p_Cell, string p_Input)=0;
     virtual void push_Input(int p_Row, string p_Input)=0;

     virtual void eval_Input_Table(int p_Building_Method=0, int p_Charging_Method=0)=0;
     virtual void build_Input_Table(int p_Building_Method=0, int p_Charging_Method=0)=0;
     virtual void build_Input_Table_BM(int p_Building_Method=0, int p_Charging_Method=0)=0;
     virtual void eval_Foreign_Output_Table(c_Table_1D * p_Output_Table_Nodes, c_Table_1D * p_Output_Table_Charges)=0;

     virtual void Eval()=0;
     virtual void Build()=0;

     virtual void output_Construct()=0;
     virtual void output_given_CAN(int p_CAN)=0;
     virtual void output_Every_CAN()=0;
     virtual void output_Node_Network()=0;
     virtual void output_Node_Network_Tier(int p_Tier)=0;
     virtual void output_Node_Network_Stats()=0;
     virtual void output_Construct_Tables()=0;
     virtual void output_Construct_Internals()=0;
     virtual void output_Construct_Full()=0;
     virtual void output_Tables()=0;

     virtual void reset_Tables()=0;
     virtual void reset_Output_Tables()=0;

     virtual void initiate_Backpropagation_Foreign_Node(u_Data p_Data, int p_Row, int p_Charge)=0;
     virtual void gather_Output_Tables()=0;

     virtual string get_Stats()=0;

     virtual void Save(string p_Directory)=0;
     virtual void load(string p_Directory, string p_File)=0;

     virtual void inflate_State_Coordinates()=0;
     virtual c_Base_Node_Network_1D * get_Node_Network_Reference()=0;
     virtual int get_Construct_U_CID()=0;
     
};


int c_Base_Construct::UCID = 0;
