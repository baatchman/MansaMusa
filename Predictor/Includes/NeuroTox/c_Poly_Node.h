class c_Poly_Node_Settings
{
public:
     
     //Member variables
     
     //The Reinforcement_Counter_Rate
     int Reinforcement_Counter_Rate;
     
     //The action potential threshold.
     float Action_Potential_Threshold;
     
     //The neuroplasticity modifier, used when determining the charge to add to the current charge during postsynaptic potentials.
     float Neuroplasticity_Modifier; 
     
     //Settings, used for varied charging strategies.
     //Default is to fire all legs.
     //If the node is a state node these values are ignored and every leg is fired.
     bool * setting_Charge_Legs;
     
     //The dimension of the construct.
     u_IO_Data setting_Dimension;
     
     //The number of legs.
     u_IO_Data setting_Number_Of_Legs;
     
     //THe construct this settings instance belongs to.
     int CID;
     
     c_Poly_Node_Settings()
     {
          
     }
     
     //Sets the Dimension, which sets the number of legs & charge_Legs
     void set_Dimension(int p_Dimension)
     {
          setting_Dimension.I = p_Dimension;
          setting_Number_Of_Legs.I = pow_Int(2, 2, setting_Dimension.I);
          
          setting_Charge_Legs = new bool[setting_Number_Of_Legs.I];
          for (int cou_Index=0;cou_Index<setting_Number_Of_Legs.I;cou_Index++)
          {
               setting_Charge_Legs[cou_Index] = true;
          }
     }
     
};

struct s_Node_Target
{
     unsigned int Construct;
     unsigned int Node;
     unsigned int Target_Synaptic_Index;
};

/*
class c_Poly_Node_NEW
{
public:
     
     friend class c_Poly_Network;
     
     //The instance of the settings class this node draws from.
     c_Poly_Node_Settings * Settings;
     
     //-- Members for general use
     
     //The tier that the node is located on, in this version the tier is an abstraction.
     int Tier;
     
     //The position of the node in its respective tier.
     int NID;
     
     //The position of the node in the construct. 
     unsigned int UNID;//Universal Node ID
     
     //The construct that the node belongs to.
     unsigned int CID;
     
     //The flag to keep track of whether or not this node is a treetop.
     bool flg_Treetop;
     
     //The address of the output this node is connected to.
     void * Output_Address;
     
     //The toch member, it is a flag for determining the nodes ot backpropagate.
     bool flg_Torch;
     
     //-- Members dealing with the node structure.
     
     //The nodes current X position.
     int X;
     
     //The nodes current Y posistion.
     int Y;
     
     //The current output charge.
     int Output_Charge;
     
     //The current backpropagation output charge
     int Output_Backpropagation_Charge;
     
     //The axons.
     void ** Axons;
     
     //The axon construct and node targets.
     s_Node_Target * Axon_Target_Reference;
     
     //The current axon count.
     int Axon_Count;
     
     //Maximun number of axons that will fit into the current array.
     int Max_Number_Of_Axons;
     
     //The target synaptic index for the axons to connect to, used for determining which leg to fire when charging.
     int * Axon_Target_Synaptic_Index;
     
     //The dendrite connections.
     void ** Dendrites;
     
     //The dendrite node targets using the s_Node_Target structure.
     s_Node_Target * Dendrite_Target_Reference;
     
     //The current number of dendrite connections.
     int Dendrite_Count;
     
     //The maximun number of dendrites that can be created until the array is resized.
     int Max_Number_Of_Dendrites;
     
     //Whether this node is a state node or not.
     bool flg_State;
     
     //Whether this node is a reference state node, for firing connecting node.
     bool flg_State_Reference; //Will be combined with flg_State, made into a character. 0 - no, 1 - Yes, 2 - Reference
     
     
     //The pattern this node represents.
     u_IO_Data * State;
     
     //The length of the pattern that this node represents.
     int State_Length;
     
     
     //-- Members related to node charging.
     
     //The charges, allows for multiple simultanios simulation, multisim is no longer needed.
     float Charge;
     
     //The reinforcement counter.
     int Reinforcement_Counter;
     
     //The Reinforcement_Counter_Tracker, when this reaches RC*RCR the RC is incremented.
     int Reinforcement_Counter_Tracker;
     
     //Settings, used for varied charging strategies.
     //Default is to fire all legs.
     //If the node is a state node these values are ignored and every leg is fired.
     bool * setting_Charge_Legs;
     
     //Whether or not the node is multi legged.
     int * flg_Multi_Leg;
     
     //The charge depth starts at two because the regular charging and the backpropagation.
     c_Poly_Node(c_Poly_Node_Settings * p_Settings, int p_CID, int p_UNID, void * p_Output_Address = NULL, float p_Neuroplasticity_Modifier = 0.001, int p_Dimension = 1)
     {
          
          //-- Setup the general use members.
          Tier = 0;
          flg_Treetop = 0;
          Output_Address = p_Output_Address;
          flg_Torch = false;
          UNID = p_UNID;
          CID = p_CID;
          Settings = p_Settings;
          
          //-- Setup the nodes structure.
          
          //The nodes coordinates.
          X = 0;
          Y = 0;
          
          //Setup the variables for the output charge.
          Output_Charge = 0;
          Output_Backpropagation_Charge = 0;
          
          //Setup the variables for the axon.
          Axons = NULL;
          Axon_Count = 0;
          Axon_Target_Synaptic_Index = NULL;
          Max_Number_Of_Axons = 0;
          
          //Setup the dendrites.
          Dendrites = NULL;
          Dendrite_Count = 0;
          Max_Number_Of_Dendrites = 0;
          
          //Setup the state variables.
          flg_State = 0;
          flg_State_Reference = 0;
          State = NULL;
          State_Length = 0;
          
          //Setup the reinforcement counter.
          Reinforcement_Counter = 0;
          Reinforcement_Counter_Tracker = 0;
          
          //Setup the connection references.
          Axon_Target_Reference = NULL;
          Dendrite_Target_Reference = NULL;
          
          //-- Setup the members dealing with the nodes charging.
          
          //Setup the charges.
          Charge = 0.0f;
          
          //Charge depth is 2 for regular charging and backprogation.
          //increase_Charge_Array_Size_To_Given_Depth(2);
          
          //Settings, used for varied charging strategies.
          
          
          
          //Initialize the multi leg flag.
          flg_Multi_Leg = NULL;
          
     
     }
     
     ~c_Poly_Node()
     {
          //Release the memory allocated for the arrays.
          delete [] Axons;
          delete [] Axon_Target_Synaptic_Index;
          delete [] Dendrites;
          delete [] State;
          delete [] flg_Multi_Leg;
     }
     
     //Sets the reinforcement counter.
     void set_Reinforcement_Counter(int p_Reinforcement_Counter)
     {
          Reinforcement_Counter = p_Reinforcement_Counter;
     }
     
     //Returns the current reinforcement counter.
     int get_Reinforcement_Counter()
     {
          return Reinforcement_Counter;
     }
          
     //gets the Reinforcement_Counter_Tracker of type int
     int get_Reinforcement_Counter_Tracker(){ return Reinforcement_Counter_Tracker; }

     //sets the Reinforcement_Counter_Tracker of type int to the given value
     int set_Reinforcement_Counter_Tracker(int p_Reinforcement_Counter_Tracker){ Reinforcement_Counter_Tracker = p_Reinforcement_Counter_Tracker; return Reinforcement_Counter_Tracker; }

     //gets the Reinforcement_Counter_Rate of type int
     int get_Reinforcement_Counter_Rate(){ return Settings->Reinforcement_Counter_Rate; }

     //sets the Reinforcement_Counter_Rate of type int to the given value
     int set_Reinforcement_Counter_Rate(int p_Reinforcement_Counter_Rate){ Settings->Reinforcement_Counter_Rate = p_Reinforcement_Counter_Rate; return Settings->Reinforcement_Counter_Rate; }

     
     
     //Adds an axon connection.
     int new_Axon_Connection(void * p_Axon, int p_Axon_Target_Synaptic_Index)
     {
          if (!Axon_Count)
          {
               //if no axon connections are found it creates the first one and shoves the given shit into it
               
               //create the first indexes
               Axons = new void*[1];
               Axons[0] = p_Axon;
               Axon_Target_Synaptic_Index = new int[1];
               Axon_Target_Synaptic_Index[Axon_Count] = p_Axon_Target_Synaptic_Index;
               
               Axon_Target_Reference = new s_Node_Target[1];
               
               //this node now has one connection
               Axon_Count = 1;
               return Axon_Count;
          } else {
               //Allocate a temporary array and copy the original into it, then delete the original and set it to the temporary array.
               
               //Increase the number of axons.
               Axon_Count++;
               
               //Creates the array to copy the information into with the new number of axons.
               void ** tmp_Axon;
               tmp_Axon = new void*[Axon_Count];
               
               //Create the axon target reference array.
               s_Node_Target * tmp_Axon_Target_Reference;
               tmp_Axon_Target_Reference = new s_Node_Target[Axon_Count];
               
               //Creates a temporary array to hold the synaptic index values.
               int * tmp_Axon_Target_Synaptic_Index;
               tmp_Axon_Target_Synaptic_Index = new int[Axon_Count];
               
               //Copy the addresses from the original array into the newly allocated one, then NULL the axon.
               for (int cou_A=0;cou_A<(Axon_Count - 1);cou_A++){
                    
                    //Copy the information from the Axons into the new array at the current index.
                    tmp_Axon[cou_A] = Axons[cou_A];
                    
                    //Copy the axon target synaptic index array into the termporary axon target postsynaptic array.
                    tmp_Axon_Target_Synaptic_Index[cou_A] = Axon_Target_Synaptic_Index[cou_A];
                    
                    //Copy the Axon_Target_Reference;
                    tmp_Axon_Target_Reference[cou_A] = Axon_Target_Reference[cou_A];
                    
                    //NULL the current axon.
                    Axons[cou_A] = NULL;
               }
                    
               //Delete the allocated memory for the original Axons array.
               delete [] Axons;
               Axons = NULL;
               
               //Delete the allocated memory for the original axon target synaptic index array.
               delete [] Axon_Target_Synaptic_Index;
               Axon_Target_Synaptic_Index = NULL;
               
               //Delete the original Axon_Target_Reference array.
               delete [] Axon_Target_Reference;
               Axon_Target_Reference = NULL;
               
               //Sets the Axons array to the tmp_Axons array.
               Axons = tmp_Axon;
               
               //Sets the Axon_Target_Synaptic_Index array to the tmp_Axon_Target_Synaptic_Index.
               Axon_Target_Synaptic_Index = tmp_Axon_Target_Synaptic_Index;
               
               //read the current information into the recently created index
               Axons[Axon_Count - 1] = p_Axon;
               
               //Read the current synaptic index into the new index.
               Axon_Target_Synaptic_Index[Axon_Count - 1] = p_Axon_Target_Synaptic_Index;
               
               //Set the Axon_Target_Reference array to the temp one.
               Axon_Target_Reference = tmp_Axon_Target_Reference;
               
               //Return the Axon count.
               return Axon_Count;
               }
               
     }
     
     //Adds a dendrites connection.
     int new_Dendrite_Connection(void * p_Dendrite)
     {
          if (!Dendrite_Count)
          {
               //if no dendrite connections are found it creates the first one and shoves the given shit into it
               
               //create the first indexes
               Dendrites = new void*;
               *Dendrites = p_Dendrite;
               
               //Create the dendrite target references.
               Dendrite_Target_Reference = new s_Node_Target[1];
               
               //this node now has one connection
               Dendrite_Count = 1;
               
               //Add the Multi legged flag to avoid bounding errors.
               flg_Multi_Leg = new int[1];
               flg_Multi_Leg[0] = 1;
               
               return Dendrite_Count;
               
          } else {
               
               //Allocate a temporary array and copy the original into it, then delete the original and set it to the temporary array.
               
               //Increase the number of dendrites.
               Dendrite_Count++;
               
               //Creates the array to copy the information into with the new number of dendrites.
               void ** tmp_Dendrite;
               tmp_Dendrite = new void*[Dendrite_Count];
               
               delete [] flg_Multi_Leg;
               flg_Multi_Leg = new int[Dendrite_Count];
               
               s_Node_Target * tmp_Dendrite_Target;
               tmp_Dendrite_Target = new s_Node_Target [Dendrite_Count];
               
               //Copy the addresses from the original array into the newly allocated one, then NULL the dendrite.
               for (int cou_D=0;cou_D<(Dendrite_Count - 1);cou_D++){
                    
                    //Copy the information from the Dendrites into the new array at the current index.
                    tmp_Dendrite[cou_D] = Dendrites[cou_D];
                    
                    //Copy any set dendrite references.
                    tmp_Dendrite_Target[cou_D] = Dendrite_Target_Reference[cou_D];
                    
                    //NULL the current dendrite.
                    Dendrites[cou_D] = NULL;
               }
                    
               //Delete the allocated memory for the original Dendrites array.
               delete [] Dendrites;
               delete [] Dendrite_Target_Reference;
               
               //Sets the Dendrites to the tmp_Dendrites array.
               Dendrites = tmp_Dendrite;
               Dendrite_Target_Reference = tmp_Dendrite_Target;
               
               //read the current information into the recently created index
               Dendrites[Dendrite_Count - 1] = p_Dendrite;
               
               //Checks for double legged nodes
               
               //Initialize the flg_Double legged.
               for (int cou_D=0;cou_D<Dendrite_Count;cou_D++)
               {
                    flg_Multi_Leg[cou_D] = 1;
               }
               
               for (int cou_D=0;cou_D<Dendrite_Count;cou_D++)
               {
                    for (int cou_DD=0;cou_DD<Dendrite_Count;cou_DD++)
                    {
                         //So that the dendrite does not match itself.
                         if (cou_DD == cou_D){ continue; }
                         
                         //If a match is found increase the flg_Multi_Leg for that leg.
                         if (Dendrites[cou_D] == Dendrites[cou_DD]){ flg_Multi_Leg[cou_D]++; }
                    }
               }
               
               
               //Return the Dendrite count.
               return Dendrite_Count;
               }
               
     }
     
     //Gathers the NID and CID of the target  axons and dendrites, used before saving.
     void gather_Connection_References()
     {
          //cout << "\n gather_Connection_References()"; cout.flush();
          
          //Cleanup and old.
          //cout << " cleanup..."; cout.flush();
          delete [] Axon_Target_Reference;
          delete [] Dendrite_Target_Reference;
          
          //Allocate the new.
          //cout << " allocating..."; cout.flush();
          Axon_Target_Reference = new s_Node_Target[Axon_Count];
          Dendrite_Target_Reference = new s_Node_Target[Dendrite_Count];
               
          //Gather the axons.
          //cout << " gathering_Axons..."; cout.flush();
          for (int cou_Axon=0;cou_Axon<Axon_Count;cou_Axon++)
          {
               Axon_Target_Reference[cou_Axon].Construct = ((c_Poly_Node*) Axons[cou_Axon])->CID;
               Axon_Target_Reference[cou_Axon].Node = ((c_Poly_Node*) Axons[cou_Axon])->UNID;
               Axon_Target_Reference[cou_Axon].Target_Synaptic_Index = Axon_Target_Synaptic_Index[cou_Axon];
          }
          
          //Gather the dendrites.
          //cout << " gathering_Dendrites...."; cout.flush();
          for (int cou_Dendrite=0;cou_Dendrite<Dendrite_Count;cou_Dendrite++)
          {
               Dendrite_Target_Reference[cou_Dendrite].Construct = ((c_Poly_Node*) Dendrites[cou_Dendrite])->CID;
               Dendrite_Target_Reference[cou_Dendrite].Node = ((c_Poly_Node*) Dendrites[cou_Dendrite])->UNID;
          }
     }
     
     //Sets the connection references.
     void add_Dendrite_Connection_Reference(int p_CID, int p_UNID)
     {
          new_Dendrite_Connection(NULL);
          Dendrite_Target_Reference[Dendrite_Count -1].Construct = p_CID;
          Dendrite_Target_Reference[Dendrite_Count -1].Node = p_UNID;
     }
     
     //Sets the connection references.
     void add_Axon_Connection_Reference(int p_CID, int p_UNID, int p_Target_Synaptic_Index)
     {
          new_Axon_Connection(NULL, 0);
          Axon_Target_Reference[Axon_Count -1].Construct = p_CID;
          Axon_Target_Reference[Axon_Count -1].Node = p_UNID;
     }
     
     //Neutralizes the charges in the node.
     void neutralize_Node()
     {
          //bool tmp_flg_Output = false;
          
          //This node no longer carries a torch.
          flg_Torch = false;
          
          
          //Neutralize the charge.
          Charge = 0.0f;
          
          //Neutralizes the output charge counter.
          //tmp_flg_Output = (bool) Output_Charge;
          Output_Charge = 0;
          Output_Backpropagation_Charge = 0;
          
          //Outputs the node when neutralized if the node was charged.
          //if (tmp_flg_Output){ output_Poly_Node_Graphical_Absolute(); }
     }
     
     
     //Creates a connection to a given node, the axon for this node and the dendrite for the given node.
     int create_Connection_To_Given_Node(void* p_Node)
     {
          //Create the axon connection for this node, it gets the dendrite synaptic index from the dendrite creation function.
          new_Axon_Connection(p_Node, (((c_Poly_Node*) p_Node)->new_Dendrite_Connection(this) - 1));
          
          
          //Returns 1.
          return 1;
     }
     
     //Checks to see if the given nodes match the dendrites.
     bool does_Connection_Exist(void * p_Nodes[])
     {
               


*/











































































































class c_Poly_Node
{
public:
     
     friend class c_Poly_Network;
     
     //The networks that this node belongs to.
     c_Poly_Network * Network;
     
     //-- Members for general use
     
     //The tier that the node is located on, in this version the tier is an abstraction.
     int Tier;
     
     //The position of the node in its respective tier.
     int NID;
     
     //The position of the node in the construct. 
     unsigned int UNID;//Universal Node ID
     
     //The construct that the node belongs to.
     unsigned int CID;
     
     //The flag to keep track of whether or not this node is a treetop.
     bool flg_Treetop;
     
     //The address of the output this node is connected to.
     void * Output_Address;
     
     //The toch member, it is a flag for determining the nodes ot backpropagate.
     bool flg_Torch;
     
     //-- Members dealing with the node structure.
     
     //The nodes current X position.
     int X;
     
     //The nodes current Y posistion.
     int Y;
     
     //The current output charge.
     int Output_Charge;
     
     //The current backpropagation output charge
     int Output_Backpropagation_Charge;
     
     //The axons.
     void ** Axons;
     
     //The axon construct and node targets.
     s_Node_Target * Axon_Target_Reference;
     
     //The current axon count.
     int Axon_Count;
     
     //Maximun number of axons that will fit into the current array.
     int Max_Number_Of_Axons;
     
     //The target synaptic index for the axons to connect to, used for determining which leg to fire when charging.
     int * Axon_Target_Synaptic_Index;
     
     //The dendrite connections.
     void ** Dendrites;
     
     //The dendrite node targets using the s_Node_Target structure.
     s_Node_Target * Dendrite_Target_Reference;
     
     //The current number of dendrite connections.
     int Dendrite_Count;
     
     //The maximun number of dendrites that can be created until the array is resized.
     int Max_Number_Of_Dendrites;
     
     //Whether this node is a state node or not.
     bool flg_State;
     
     //Whether this node is a reference state node, for firing connecting node.
     bool flg_State_Reference;
     
     //The pattern this node represents.
     u_IO_Data * State;
     
     //The length of the pattern that this node represents.
     int State_Length;
     
     
     //-- Members related to node charging.
     
     //The charges, allows for multiple simultanios simulation, multisim is no longer needed.
     float Charge;
     
     //The reinforcement counter.
     int Reinforcement_Counter;
     
     //The Reinforcement_Counter_Tracker, when this reaches RC*RCR the RC is incremented.
     int Reinforcement_Counter_Tracker;
     
     //The Reinforcement_Counter_Rate
     int Reinforcement_Counter_Rate;
     
     //The action potential threshold.
     float Action_Potential_Threshold;
     
     //The neuroplasticity modifier, used when determining the charge to add to the current charge during postsynaptic potentials.
     float Neuroplasticity_Modifier; 
     
     //Settings, used for varied charging strategies.
     //Default is to fire all legs.
     //If the node is a state node these values are ignored and every leg is fired.
     bool * setting_Charge_Legs;
     
     //The dimension of the construct.
     u_IO_Data setting_Dimension;
     
     //The number of legs.
     u_IO_Data setting_Number_Of_Legs;
     
     //Whether or not the node is multi legged.
     int * flg_Multi_Leg;
     
     //The charge depth starts at two because the regular charging and the backpropagation.
     c_Poly_Node(c_Poly_Network * p_Network, int p_CID, int p_UNID, void * p_Output_Address = NULL, float p_Action_Potential_Threshold = 0.01, float p_Neuroplasticity_Modifier = 0.001, int p_Dimension = 1)
     {
          
          //-- Setup the general use members.
          Tier = 0;
          flg_Treetop = 0;
          Output_Address = p_Output_Address;
          flg_Torch = false;
          UNID = p_UNID;
          CID = p_CID;
          Network = p_Network;
          
          //-- Setup the nodes structure.
          
          //The nodes coordinates.
          X = 0;
          Y = 0;
          
          //Setup the variables for the output charge.
          Output_Charge = 0;
          Output_Backpropagation_Charge = 0;
          
          //Setup the variables for the axon.
          Axons = NULL;
          Axon_Count = 0;
          Axon_Target_Synaptic_Index = NULL;
          Max_Number_Of_Axons = 0;
          
          //Setup the dendrites.
          Dendrites = NULL;
          Dendrite_Count = 0;
          Max_Number_Of_Dendrites = 0;
          
          //Setup the state variables.
          flg_State = 0;
          flg_State_Reference = 0;
          State = NULL;
          State_Length = 0;
          
          //Setup the reinforcement counter.
          Reinforcement_Counter = 0;
          Reinforcement_Counter_Tracker = 0;
          Reinforcement_Counter_Rate = 2;
          
          //Setup the connection references.
          Axon_Target_Reference = NULL;
          Dendrite_Target_Reference = NULL;
          
          //-- Setup the members dealing with the nodes charging.
          
          //Setup the charges.
          Charge = 0.0f;
          
          //Charge depth is 2 for regular charging and backprogation.
          //increase_Charge_Array_Size_To_Given_Depth(2);
          
          //Setup the action potential threshold which is used when determining whether or not to fire.
          Action_Potential_Threshold = p_Action_Potential_Threshold;
          
          //Initialize the neuroplasticity_Modifier.
          Neuroplasticity_Modifier = p_Neuroplasticity_Modifier;
          
          //Settings, used for varied charging strategies.
          
          //The dimension of the construct.
          if (p_Dimension > 254){ p_Dimension = 254; }
          if (p_Dimension <= 0){ p_Dimension = 1; }
          setting_Dimension.I = p_Dimension;
          
          //Default is to fire all legs.
          //If the node is a state node these values are ignored and every leg is fired.
          setting_Number_Of_Legs.I = pow_Int(2, 2, p_Dimension);
          setting_Charge_Legs = new bool[setting_Number_Of_Legs.I];
          
          for (int cou_Index=0;cou_Index<setting_Number_Of_Legs.I;cou_Index++)
          {
               setting_Charge_Legs[cou_Index] = true;
          }
          
          //Initialize the multi leg flag.
          flg_Multi_Leg = NULL;
          
     
     }
     
     ~c_Poly_Node()
     {
          //Release the memory allocated for the arrays.
          delete [] Axons;
          delete [] Axon_Target_Synaptic_Index;
          delete [] Dendrites;
          delete [] State;
          delete [] flg_Multi_Leg;
     }
     
     //Sets the reinforcement counter.
     void set_Reinforcement_Counter(int p_Reinforcement_Counter)
     {
          Reinforcement_Counter = p_Reinforcement_Counter;
     }
     
     //Returns the current reinforcement counter.
     int get_Reinforcement_Counter()
     {
          return Reinforcement_Counter;
     }
          
     //gets the Reinforcement_Counter_Tracker of type int
     int get_Reinforcement_Counter_Tracker(){ return Reinforcement_Counter_Tracker; }

     //sets the Reinforcement_Counter_Tracker of type int to the given value
     int set_Reinforcement_Counter_Tracker(int p_Reinforcement_Counter_Tracker){ Reinforcement_Counter_Tracker = p_Reinforcement_Counter_Tracker; return Reinforcement_Counter_Tracker; }

     //gets the Reinforcement_Counter_Rate of type int
     int get_Reinforcement_Counter_Rate(){ return Reinforcement_Counter_Rate; }

     //sets the Reinforcement_Counter_Rate of type int to the given value
     int set_Reinforcement_Counter_Rate(int p_Reinforcement_Counter_Rate){ Reinforcement_Counter_Rate = p_Reinforcement_Counter_Rate; return Reinforcement_Counter_Rate; }

     
     
     //Adds an axon connection.
     int new_Axon_Connection(void * p_Axon, int p_Axon_Target_Synaptic_Index)
     {
          if (!Axon_Count)
          {
               //if no axon connections are found it creates the first one and shoves the given shit into it
               
               //create the first indexes
               Axons = new void*[1];
               Axons[0] = p_Axon;
               Axon_Target_Synaptic_Index = new int[1];
               Axon_Target_Synaptic_Index[Axon_Count] = p_Axon_Target_Synaptic_Index;
               
               Axon_Target_Reference = new s_Node_Target[1];
               
               //this node now has one connection
               Axon_Count = 1;
               return Axon_Count;
          } else {
               //Allocate a temporary array and copy the original into it, then delete the original and set it to the temporary array.
               
               //Increase the number of axons.
               Axon_Count++;
               
               //Creates the array to copy the information into with the new number of axons.
               void ** tmp_Axon;
               tmp_Axon = new void*[Axon_Count];
               
               //Create the axon target reference array.
               s_Node_Target * tmp_Axon_Target_Reference;
               tmp_Axon_Target_Reference = new s_Node_Target[Axon_Count];
               
               //Creates a temporary array to hold the synaptic index values.
               int * tmp_Axon_Target_Synaptic_Index;
               tmp_Axon_Target_Synaptic_Index = new int[Axon_Count];
               
               //Copy the addresses from the original array into the newly allocated one, then NULL the axon.
               for (int cou_A=0;cou_A<(Axon_Count - 1);cou_A++){
                    
                    //Copy the information from the Axons into the new array at the current index.
                    tmp_Axon[cou_A] = Axons[cou_A];
                    
                    //Copy the axon target synaptic index array into the termporary axon target postsynaptic array.
                    tmp_Axon_Target_Synaptic_Index[cou_A] = Axon_Target_Synaptic_Index[cou_A];
                    
                    //Copy the Axon_Target_Reference;
                    tmp_Axon_Target_Reference[cou_A] = Axon_Target_Reference[cou_A];
                    
                    //NULL the current axon.
                    Axons[cou_A] = NULL;
               }
                    
               //Delete the allocated memory for the original Axons array.
               delete [] Axons;
               
               //Delete the allocated memory for the original axon target synaptic index array.
               delete [] Axon_Target_Synaptic_Index;
               
               //Delete the original Axon_Target_Reference array.
               delete [] Axon_Target_Reference;
               
               //Sets the Axons array to the tmp_Axons array.
               Axons = tmp_Axon;
               
               //Sets the Axon_Target_Synaptic_Index array to the tmp_Axon_Target_Synaptic_Index.
               Axon_Target_Synaptic_Index = tmp_Axon_Target_Synaptic_Index;
               
               //read the current information into the recently created index
               Axons[Axon_Count - 1] = p_Axon;
               
               //Read the current synaptic index into the new index.
               Axon_Target_Synaptic_Index[Axon_Count - 1] = p_Axon_Target_Synaptic_Index;
               
               //Set the Axon_Target_Reference array to the temp one.
               Axon_Target_Reference = tmp_Axon_Target_Reference;
               
               //Return the Axon count.
               return Axon_Count;
               }
               
     }
     
     //Adds a dendrites connection.
     int new_Dendrite_Connection(void * p_Dendrite)
     {
          if (!Dendrite_Count)
          {
               //if no dendrite connections are found it creates the first one and shoves the given shit into it
               
               //create the first indexes
               Dendrites = new void*;
               *Dendrites = p_Dendrite;
               
               //Create the dendrite target references.
               Dendrite_Target_Reference = new s_Node_Target[1];
               
               //this node now has one connection
               Dendrite_Count = 1;
               
               //Add the Multi legged flag to avoid bounding errors.
               flg_Multi_Leg = new int[1];
               flg_Multi_Leg[0] = 1;
               
               return Dendrite_Count;
               
          } else {
               
               //Allocate a temporary array and copy the original into it, then delete the original and set it to the temporary array.
               
               //Increase the number of dendrites.
               Dendrite_Count++;
               
               //Creates the array to copy the information into with the new number of dendrites.
               void ** tmp_Dendrite;
               tmp_Dendrite = new void*[Dendrite_Count];
               
               delete [] flg_Multi_Leg;
               flg_Multi_Leg = new int[Dendrite_Count];
               
               s_Node_Target * tmp_Dendrite_Target;
               tmp_Dendrite_Target = new s_Node_Target [Dendrite_Count];
               
               //Copy the addresses from the original array into the newly allocated one, then NULL the dendrite.
               for (int cou_D=0;cou_D<(Dendrite_Count - 1);cou_D++){
                    
                    //Copy the information from the Dendrites into the new array at the current index.
                    tmp_Dendrite[cou_D] = Dendrites[cou_D];
                    
                    //Copy any set dendrite references.
                    tmp_Dendrite_Target[cou_D] = Dendrite_Target_Reference[cou_D];
                    
                    //NULL the current dendrite.
                    Dendrites[cou_D] = NULL;
               }
                    
               //Delete the allocated memory for the original Dendrites array.
               delete [] Dendrites;
               delete [] Dendrite_Target_Reference;
               
               //Sets the Dendrites to the tmp_Dendrites array.
               Dendrites = tmp_Dendrite;
               Dendrite_Target_Reference = tmp_Dendrite_Target;
               
               //read the current information into the recently created index
               Dendrites[Dendrite_Count - 1] = p_Dendrite;
               
               //Checks for double legged nodes
               
               //Initialize the flg_Double legged.
               for (int cou_D=0;cou_D<Dendrite_Count;cou_D++)
               {
                    flg_Multi_Leg[cou_D] = 1;
               }
               
               for (int cou_D=0;cou_D<Dendrite_Count;cou_D++)
               {
                    for (int cou_DD=0;cou_DD<Dendrite_Count;cou_DD++)
                    {
                         //So that the dendrite does not match itself.
                         if (cou_DD == cou_D){ continue; }
                         
                         //If a match is found increase the flg_Multi_Leg for that leg.
                         if (Dendrites[cou_D] == Dendrites[cou_DD]){ flg_Multi_Leg[cou_D]++; }
                    }
               }
               
               
               //Return the Dendrite count.
               return Dendrite_Count;
               }
               
     }
     
     //Gathers the NID and CID of the target  axons and dendrites, used before saving.
     void gather_Connection_References()
     {
          //cout << "\n gather_Connection_References()"; cout.flush();
          
          //Cleanup and old.
          //cout << " cleanup..."; cout.flush();
          delete [] Axon_Target_Reference;
          delete [] Dendrite_Target_Reference;
          
          //Allocate the new.
          //cout << " allocating..."; cout.flush();
          Axon_Target_Reference = new s_Node_Target[Axon_Count];
          Dendrite_Target_Reference = new s_Node_Target[Dendrite_Count];
               
          //Gather the axons.
          //cout << " gathering_Axons..."; cout.flush();
          for (int cou_Axon=0;cou_Axon<Axon_Count;cou_Axon++)
          {
               Axon_Target_Reference[cou_Axon].Construct = ((c_Poly_Node*) Axons[cou_Axon])->CID;
               Axon_Target_Reference[cou_Axon].Node = ((c_Poly_Node*) Axons[cou_Axon])->UNID;
               Axon_Target_Reference[cou_Axon].Target_Synaptic_Index = Axon_Target_Synaptic_Index[cou_Axon];
          }
          
          //Gather the dendrites.
          //cout << " gathering_Dendrites...."; cout.flush();
          for (int cou_Dendrite=0;cou_Dendrite<Dendrite_Count;cou_Dendrite++)
          {
               Dendrite_Target_Reference[cou_Dendrite].Construct = ((c_Poly_Node*) Dendrites[cou_Dendrite])->CID;
               Dendrite_Target_Reference[cou_Dendrite].Node = ((c_Poly_Node*) Dendrites[cou_Dendrite])->UNID;
          }
     }
     
     //Sets the connection references.
     void add_Dendrite_Connection_Reference(int p_CID, int p_UNID)
     {
          new_Dendrite_Connection(NULL);
          Dendrite_Target_Reference[Dendrite_Count -1].Construct = p_CID;
          Dendrite_Target_Reference[Dendrite_Count -1].Node = p_UNID;
     }
     
     //Sets the connection references.
     void add_Axon_Connection_Reference(int p_CID, int p_UNID, int p_Target_Synaptic_Index)
     {
          new_Axon_Connection(NULL, 0);
          Axon_Target_Reference[Axon_Count -1].Construct = p_CID;
          Axon_Target_Reference[Axon_Count -1].Node = p_UNID;
     }
     
     //Neutralizes the charges in the node.
     void neutralize_Node()
     {
          //bool tmp_flg_Output = false;
          
          //This node no longer carries a torch.
          flg_Torch = false;
          
          
          //Neutralize the charge.
          Charge = 0.0f;
          
          //Neutralizes the output charge counter.
          //tmp_flg_Output = (bool) Output_Charge;
          Output_Charge = 0;
          Output_Backpropagation_Charge = 0;
          
          //Outputs the node when neutralized if the node was charged.
          //if (tmp_flg_Output){ output_Poly_Node_Graphical_Absolute(); }
     }
     
     
     //Creates a connection to a given node, the axon for this node and the dendrite for the given node.
     int create_Connection_To_Given_Node(void* p_Node)
     {
          //Create the axon connection for this node, it gets the dendrite synaptic index from the dendrite creation function.
          new_Axon_Connection(p_Node, (((c_Poly_Node*) p_Node)->new_Dendrite_Connection(this) - 1));
          
          
          //Returns 1.
          return 1;
     }
     
     //Checks to see if the given nodes match the dendrites.
     bool does_Connection_Exist(void * p_Nodes[])
     {
               /*
          output_Poly_Node_Info_Absolute();
          cout << "\n";
          output_State();
          cout << "\n ~does_Connection_Exist";*/

          //The node order has to match the dendrite order exactly, with the way the networks are built the nodes will always be the same order to maintain data integrety,
          
          //Loop through the dendrites checking to see if they match the given nodes.
          for (int cou_D=0;cou_D<Dendrite_Count;cou_D++){
               
               //If the dendrites do not match then return false.
               if (Dendrites[cou_D] != p_Nodes[cou_D])
               {
                    return false;
               }
          }
          
               /*
          output_Poly_Node_Info_Absolute();
          cout << "\n";
          output_State();
          cout << "\n ~Finished:does_Connection_Exist " << 1;
*/
          //If all the dendrites match and the function makes it to here then the connection was found.
          return true;
     }
     
     //Checks the upper tier nodes to see if a connection exists between this node and the given nodes, it does this by checking each axon target for a connection using
     // the does_Connection_Exist() member.
     void* does_Upper_Tier_Connection_Exist(void * p_Nodes[])
     {
               /*
          output_Poly_Node_Info_Absolute();
          cout << "\n";
          output_State();
          cout << "\n ~does_Upper_Tier_Connection_Exist";*/

          //If there are no axons then there is no upper tier connection right now.
          if (Axon_Count == 0)     
          {
               //If there are no axons then return NULL.
               return NULL;
          }
          
          //Loop through all the axon connections querying the target nodes for the upper tier connection.
          for (int cou_A=0;cou_A<Axon_Count;cou_A++){
               
               //Keep it in the memory tree.
               if (((c_Poly_Node*) Axons[cou_A])->CID != CID){ continue; }
                    
               //Query the current axon target.
               if (((c_Poly_Node*) Axons[cou_A])->does_Connection_Exist(p_Nodes))
               {
                    //Returns the address of the node containing the upper tier connections.
                    return Axons[cou_A];
               }
          }
          
          //If the function reaches this point return NULL;
          return NULL;
               /*
          output_Poly_Node_Info_Absolute(); 
          cout << "\n";
          output_State();
          cout << "\n ~Finished:does_Upper_Tier_Connection_Exist";*/
     }
     
     //Sets the state to the given state, uses the given length.
     void set_State(u_IO_Data p_State[], int p_State_Length)
     {
          
          //Disabling state setting.
          //return;
               /*
          output_Poly_Node_Info_Absolute();
          cout << "\n";
          output_State();
          cout << "\n ~set_State";*/
          
          //If the state is already set then return.
          if (State != NULL){ return; }
          
          //Sets the state length to the given length.
          State_Length = p_State_Length;
          
          //Initializes the state array.
          State = new u_IO_Data[State_Length];
          
          //Loops through each of the state indexes setting the state to the value of the given state.
          for (int cou_Index=0;cou_Index<State_Length;cou_Index++){
               State[cou_Index].NR = p_State[cou_Index].NR;
          }
               /*
          output_Poly_Node_Info_Absolute();
          cout << "\n";
          output_State();
          cout << "\n ~Finished:set_State";*/
     }
     
     //Reinforces the node, could use a direct reference, however, in the future there may be more to it.
     //AHAHA NOW THERE IS MORE TOO IT! THE FUNCTION CALL FINALLY PAID OFF AHAHAHAH!
     void reinforce()
     {
          //Iterate the tracker variable.
          Reinforcement_Counter_Tracker++;
          
          //If the RCT is greater than or equal to the RC / RCT then increment the RC, this allows for gradient increase instead of linear.
          if (Reinforcement_Counter_Tracker >= int (Reinforcement_Counter / Reinforcement_Counter_Tracker))
          {
               Reinforcement_Counter++;
          }
          
          //Iterate the reinforcement counter.
          //Reinforcement_Counter++;
          
     }
     
     //Initiates a postsynaptic potential.
     void initiate_Postsynaptic_Potential(int p_Output_Info, int p_Output_Graphical, int p_Charging_Tier, bool p_flg_Torch, float p_Charge, int p_Synaptic_Index)
     {
          //cout << "\n\t\t c_Poly_Node::initiate_Postsynaptic_Potential(" << p_Charge << ")" << this;
          
          if (p_Output_Info)
          {
               //output_Poly_Node_Info_Absolute();
               cout << " ~initiate_Postsynaptic_Potential";    
          }
          
//          cout << "\n\t\t\t c_Poly_Node::initiate_Postsynaptic_Potential(" <<  << ")" << this;
          //If the charge array cannot handle the request then the resizing function is called.
          //if ( >= Charge_Depth){ increase_Charge_Array_Size_To_Given_Depth( + 1); }
          
          //Increases the current charge index by the reinforcement counter.
          if (Charge < 99999)
          {
               //Charge += ((((float) Reinforcement_Counter * (float) Reinforcement_Counter) * Neuroplasticity_Modifier) * p_Charge);
               Charge += (((Neuroplasticity_Modifier * float (Reinforcement_Counter)) * p_Charge) / float (flg_Multi_Leg[p_Synaptic_Index]));
          }
          
          
          
          if (flg_Torch < p_flg_Torch)
          {
               flg_Torch = p_flg_Torch;
               
               //Updates the torch output.
               //output_Poly_Node_Graphical_Absolute_Torch();
          }
          
          //if the membrane potential is over the action potential threshold an action potential is initiated in the node
          if (Charge >= Action_Potential_Threshold)
          {
               initiate_Action_Potential(p_Output_Info, p_Output_Graphical, p_Charging_Tier, p_Charge);
          }
               
          if (p_Output_Info)
          {
               //output_Poly_Node_Info_Absolute();
               cout << " ~Finished:initiate_Postsynaptic_Potential";
          }
     }
     
     //Initiates an action potential.
     void initiate_Action_Potential(int p_Output_Info, int p_Output_Graphical, int p_Charging_Tier, float p_Charge)
     {
          //cout << "\n\t\t c_Poly_Node::initiate_Action_Potential(" << p_Charge << ")" << this;
          
          if (p_Output_Info)
          {
               
               //output_Poly_Node_Info_Absolute();
               cout << "\n\t\t\t -initiate_Action_Potential";
          }
          
//          cout << "\n //If the charge array cannot handle the request then the resizing function is called.";
          
          //If the charge array cannot handle the request then the resizing function is called.
          //if ( > Charge_Depth){ increase_Charge_Array_Size_To_Given_Depth( + 1); }
          
//          cout << "\n //For every axon in the current array initiate a postsynaptic potential.";
          
          //For every axon in the current array initiate a postsynaptic potential.
          for (int cou_A=0;cou_A<Axon_Count;cou_A++){
               
               if (((c_Poly_Node*) Axon_Target_Reference)->flg_State){ ((c_Poly_Node*) Axons[cou_A])->initiate_Postsynaptic_Potential(p_Output_Info, p_Output_Graphical, p_Charging_Tier, flg_Torch, p_Charge, Axon_Target_Synaptic_Index[cou_A]); return; }
               if (setting_Charge_Legs[Axon_Target_Synaptic_Index[cou_A]]){ ((c_Poly_Node*) Axons[cou_A])->initiate_Postsynaptic_Potential(p_Output_Info, p_Output_Graphical, p_Charging_Tier, flg_Torch, p_Charge, Axon_Target_Synaptic_Index[cou_A]); }
               }
          
          
//          cout << "\n //If the node is not a treetop the membrane potential is reset.";
          
          //If the node is not a treetop the membrane potential is reset.
          //if (!flg_Treetop){ Charge[] = 0; }
          if (Axon_Count){ Charge = 0.0f; }
          
          //This node no longer carries the torch.
          if (flg_Torch && Axon_Count)
          {
               flg_Torch = false;
               
               //Updates the torch output.
               //output_Poly_Node_Graphical_Absolute_Torch();
          }
          
//          cout << "\n if (Tier >= p_Charging_Tier)"; 
          
          //If this node has no nodes to pass the charge too then initiate the backpropagation.
          /*
          if (!Axon_Count)// >= p_Charging_Tier)
          {
               initiate_Backpropagation_Both(p_Charging_Tier, p_Output_Info, p_Output_Graphical); 
          }
           * */
          //initiate_Backpropagation_Both(); 
          
          //Increments the output charge.
          Output_Charge++;
          
          /*
          output_Poly_Node_Info_Absolute();
          cout << "\n";
          output_State();
          cout << "\n ~Finished:initiate_Action_Potential";*/
          
          //Outputs the change in the nodes charge.
          //if (p_Output_Graphical){ output_Poly_Node_Graphical_Absolute(); }
          
          //This resets the membrane potential to the default, however, this may need to just lower it by the action potential amount
          //if (setting_Reset_Membrane_Potential){ Charge[] = 0; }
     }
     
     //Initiates an action potential for CAN nodes.
     void initiate_CAN_Action_Potential(int p_Output_Info, int p_Output_Graphical, int p_Charging_Tier, float p_Charge)
     {
          //cout << "\n\t c_Poly_Node::initiate_CAN_Action_Potential(" << p_Charge << ")" << this;
          
          if (p_Output_Info)
          {
               //output_Poly_Node_Info_Absolute();
               cout << "\n\t\t\t -initiate_CAN_Action_Potential";
          }
          
          //If the charge array cannot handle the request then the resizing function is called.
          //if ( >= Charge_Depth){ increase_Charge_Array_Size_To_Given_Depth( + 1); }
          
          //For every axon in the current array initiate a postsynaptic potential.
          for (int cou_A=0;cou_A<Axon_Count;cou_A++){
               
               if (setting_Charge_Legs[Axon_Target_Synaptic_Index[cou_A]]){ ((c_Poly_Node*) Axons[cou_A])->initiate_Postsynaptic_Potential(p_Output_Info, p_Output_Graphical, p_Charging_Tier, 1, p_Charge, Axon_Target_Synaptic_Index[cou_A]); }
               
               }
          
          //Increases the current charge index by the reinforcement counter.
          Charge += (((float) Reinforcement_Counter) * Neuroplasticity_Modifier) + Neuroplasticity_Modifier;
          
          //If the current charge is over the action potential threshold fire, then initiate an action potential.
          if (Charge >= Action_Potential_Threshold){
               initiate_Action_Potential(p_Output_Info, p_Output_Graphical, p_Charging_Tier, p_Charge);
               }
               
          
          if (p_Output_Info)
          {
               //output_Poly_Node_Graphical_Absolute();
               cout << "\n\t\t\t    ~Finished:initiate_CAN_Action_Potential";
          }
          
     }
     
     //Initiates an action potential for CAN nodes.
     void initiate_Backpropagation_Left(float p_Charge)
     {
          /*
          output_Poly_Node_Info_Absolute();
          cout << "\n";
          output_State();
          cout << "\n ~initiate_Backpropagation_Left";*/
          
          //Increments the backpropagation output charge.
          Output_Backpropagation_Charge++;
          
          //If the Output_Backpropagation_Charge is too high and will cause errors reset the value.
          if (Output_Backpropagation_Charge == 200)
          {
               Output_Backpropagation_Charge = 1;
          }
          
//          cout << "\n ~flg_State: "; cout.flush();
          
          //If this node is a state node then fire the state pushing member.
          if (flg_State)
          {
//               cout << "true "; cout.flush();
                   
               //If the output address is not NULL push and pop the current state.
               if (Output_Address != NULL)
               {
//                    cout << "Output_Address: true " << Output_Address; cout.flush();
                   
                    
                    
                    ((c_1D_Output*) Output_Address)->push(State[0], p_Charge);
               }
               else
               {
//                    cout <<  "Output_Address: false "; cout.flush();
                    
               }
               
               //If the state node links to a node reference then fire that nodes backpropagation member.
               if (flg_State_Reference)
               {
//                    cout << "flg_State_Reference: true "; cout.flush();
                    
                    ((c_Poly_Node*) State[0].NR)->initiate_Backpropagation_Both(p_Charge);
               }
               else
               {
//                    cout <<  "flg_State_Reference: false "; cout.flush();
                    
               }
          }
          
//          cout << "\n ~Dendrite_Count: " << Dendrite_Count << " "; cout.flush();
          
          //If this node has dendrites then initiate the backpropagation.
          if (Dendrite_Count >= 1)
          {
//               cout << "true "; cout.flush();
         
               //Initiates a backpropagation on the left leg of this nodes left leg.
               ((c_Poly_Node*) Dendrites[0])->initiate_Backpropagation_Left(p_Charge);
          }
          
          //If this node has only one dendrite then initiate_Backpropagation_Both();
          if (Dendrite_Count == 1)
          {
               cout << "\n Setting torchies.";
               ((c_Poly_Node*) Dendrites[0])->output_Poly_Node();
               ((c_Poly_Node*) Dendrites[0])->flg_Torch = 1;
               ((c_Poly_Node*) Dendrites[0])->Charge = p_Charge;
               ((c_Poly_Node*) Dendrites[0])->output_Poly_Node();
          }
          
          
          //Outputs the change in the nodes charge.
          //output_Poly_Node_Graphical_Absolute();
               /*
          output_Poly_Node_Info_Absolute();
          cout << "\n";
          output_State();
          cout << "\n ~Finished:initiate_Backpropagation_Left";*/
     }
     
     //Initiates an action potential for CAN nodes.
     void initiate_Backpropagation_Both(float p_Charge,  int p_Output_Info = 0, int p_Output_Graphical = 0, int p_Torch = 0)
     {
          /*
          if (p_Output_Info)
          {
               output_Poly_Node_Info_Absolute();
               cout << " ~initiate_Backpropagation_Both";
          }*/
          
          //Increments the backpropagation output charge.
          Output_Backpropagation_Charge++;
          
          //If the Output_Backpropagation_Charge is too high and will cause errors reset the value.
          if (Output_Backpropagation_Charge == 200)
          {
               Output_Backpropagation_Charge = 1;
          }
          
          //If this node is a state node then fire the state pushing member.
          if (flg_State)
          {
               //cout << "\n     flg_State:" << this << " " << char (State[0].I) << " " << p_Charge;
               
               //If the output address is not NULL push and pop the current state.
               if (Output_Address != NULL)
               {
                    
                    ((c_1D_Output*) Output_Address)->push(State[0], p_Charge);
                    ((c_1D_Output*) Output_Address)->pop();
               }
          }
          
          //If this node has dendrites then initiate the backpropagation.
          if (Dendrite_Count == 2)
          {
               //Initiates a backpropagation on the left leg of this nodes left leg.
               ((c_Poly_Node*) Dendrites[0])->initiate_Backpropagation_Left(p_Charge);
               
               //Initiates a backpropagation on both legs of this nodes right leg.
               ((c_Poly_Node*) Dendrites[1])->initiate_Backpropagation_Both(p_Charge);
               
          }
          
          //If this node has only one dendrite then initiate_Backpropagation_Both();
          if (Dendrite_Count == 1)
          {
               cout << "\n Setting torchies.";
               ((c_Poly_Node*) Dendrites[0])->output_Poly_Node();
               ((c_Poly_Node*) Dendrites[0])->flg_Torch = 1;
               ((c_Poly_Node*) Dendrites[0])->Charge = p_Charge;
               ((c_Poly_Node*) Dendrites[0])->output_Poly_Node();
          }
          
          //Outputs the change in the nodes charge.
          //output_Poly_Node_Graphical_Absolute();
          /*
          if (p_Output_Info)
          {
               output_Poly_Node_Info_Absolute();
               cout << " ~Finished:initiate_Backpropagation_Both";
          }*/
     }
     
     
     //Backpropagation for two dimensional data.
     
     //UR [0]->LL [1]->LR [2]->UL [3]->UR
     //UL [0]->LL [1]->LR [2]->UL
     //LR [0]->LL [1]->LR
     //LL [0]->LL
     
     //The Treetop is initiated with the UR.
     
     //UR [0]->LL [1]->LR [2]->UL [3]->UR
     void initiate_Backpropagation_UR(float p_Charge)
     {
          //If this node has dendrites then initiate the backpropagation.
          if (Dendrite_Count == 4)
          {
               //Initiates a backpropagation on the left leg of this nodes left leg.
               ((c_Poly_Node*) Dendrites[0])->initiate_Backpropagation_LL(p_Charge);
               
               //Initiates a backpropagation on the left leg of this nodes left leg.
               ((c_Poly_Node*) Dendrites[1])->initiate_Backpropagation_LR(p_Charge);
               
               //Initiates a backpropagation on the left leg of this nodes left leg.
               ((c_Poly_Node*) Dendrites[2])->initiate_Backpropagation_UL(p_Charge);
               
               //Initiates a backpropagation on the left leg of this nodes left leg.
               ((c_Poly_Node*) Dendrites[3])->initiate_Backpropagation_UR(p_Charge);
          }
          
          //If this is a state node push the output.
          if (flg_State)
          {
               push_2D_Output(p_Charge);
          }
          
     }
     
     //UL [0]->LL [1]->LR [2]->UL
     void initiate_Backpropagation_UL(float p_Charge)
     {
          //If this node has dendrites then initiate the backpropagation.
          if (Dendrite_Count == 4)
          {
               //Initiates a backpropagation on the left leg of this nodes left leg.
               ((c_Poly_Node*) Dendrites[0])->initiate_Backpropagation_LL(p_Charge);
               
               //Initiates a backpropagation on the left leg of this nodes left leg.
               ((c_Poly_Node*) Dendrites[1])->initiate_Backpropagation_LR(p_Charge);
               
               //Initiates a backpropagation on the left leg of this nodes left leg.
               ((c_Poly_Node*) Dendrites[2])->initiate_Backpropagation_UL(p_Charge);
               
               //Initiates a backpropagation on the left leg of this nodes left leg.
               ((c_Poly_Node*) Dendrites[3])->initiate_Backpropagation_UR(p_Charge);
          }
          
          //If this is a state node push the output.
          if (flg_State)
          {
               push_2D_Output(p_Charge);
          }
     }
     
     //LR [0]->LL [1]->LR
     void initiate_Backpropagation_LR(float p_Charge)
     {
          //If this node has dendrites then initiate the backpropagation.
          if (Dendrite_Count == 4)
          {
               //Initiates a backpropagation on the left leg of this nodes left leg.
               ((c_Poly_Node*) Dendrites[0])->initiate_Backpropagation_LL(p_Charge);
               
               //Initiates a backpropagation on the left leg of this nodes left leg.
               ((c_Poly_Node*) Dendrites[1])->initiate_Backpropagation_LR(p_Charge);
          }
          
          //If this is a state node push the output then pop the Y.
          if (flg_State)
          {
               push_2D_Output(p_Charge);
               //((c_2D_Output*) Output_Address)->pop_Y();
          }
     }
     
     //LL [0]->LL
     void initiate_Backpropagation_LL(float p_Charge)
     {
          //If this node has dendrites then initiate the backpropagation.
          if (Dendrite_Count == 4)
          {
               //Initiates a backpropagation on the left leg of this nodes left leg.
               ((c_Poly_Node*) Dendrites[0])->initiate_Backpropagation_LL(p_Charge);
          }
          
          //If this is a state node push the output.
          if (flg_State)
          {
               push_2D_Output(p_Charge);
          }
     }
     
     //Handles state nodes for two dimensional backpropagation.
     void push_2D_Output(int p_Charge)
     {
          //((c_2D_Output*) Output_Address)->push(State[0], p_Charge);
     }
     
     
     
     //Outputs the setting_Charging_Legs
     void set_Charging_Legs(bool p_Legs[])
     {
          if (flg_State){ return; }
          for (int cou_Index=0;cou_Index<setting_Number_Of_Legs.I;cou_Index++)
          {
               setting_Charge_Legs[cou_Index] = p_Legs[cou_Index];
          }
          
     }
     
     //SEts the charging state for an individual leg.
     void set_Charging_Leg(int p_Leg, bool p_Charge)
     {
          setting_Charge_Legs[p_Leg] = p_Charge;
     }
     
     //Increases the size of the charge array.
     /*
     void increase_Charge_Array_Size_To_Given_Depth(int p_Charge_Depth)
     {
          //cout << "\n\t c_Poly_Node::increase_Charge_Array_Size_To_Given_Depth(int p_Charge_Depth~~" << p_Charge_Depth << ") " << this;
          
          //If the depth is larger than or equal to the given depth then return.
          if (Charge_Depth >= p_Charge_Depth){ return; }
          
          //Creates a temporary array to hold the values.
          float * tmp_Charge;
          tmp_Charge = new float[p_Charge_Depth];
          
          //Zero the newly creates array to avoid junk data in the future.
          for (int cou_Index=0;cou_Index<p_Charge_Depth;cou_Index++){
               
               //Zeros the temporary charge index.
               tmp_Charge[cou_Index] = 0;
          }
          
          //Read the current values into the new array.
          for (int cou_Index=0;cou_Index<Charge_Depth;cou_Index++){
               
               //Reads the value into the temporary array.
               tmp_Charge[cou_Index] = Charge[cou_Index];
          }
          
          //Free the allocated memory.
          delete [] Charge;
          
          //Sets the charge array to the newly created tmp_Charge array.
          Charge = tmp_Charge;
          
          //Sets the Charge_Depth.
          Charge_Depth = p_Charge_Depth;
     }*/
     
     //Outputs the poly nodes members.
     void output_Poly_Node()
     {
          cout << "\n";
          cout << "(" << this << ")";
          
          output_State();
          
          cout << " T:" << Tier;
          
          cout << " NID:" << NID;
          
          cout << " CID:" << CID;
          
          cout << " UNID:" << UNID;
          
          //cout << " O&:" << Output_Address;
          
          cout << "\t OC:" << Output_Charge;
          
          //cout << " OBC:" << Output_Backpropagation_Charge;
          
          cout << "\t flg_Tt:" << flg_Treetop;
          
          cout << "\t flg_St:" << flg_State;
          
          cout << "\t flg_Tch:" << flg_Torch;
          
          cout << "\t RC:" << Reinforcement_Counter;
          
          output_Charges();
          //output_Node_Dendrites();
          //output_Node_Axons();
          //output_Node_References();
          //output_Charging_Legs();
          
     }
     
     //Outputs the setting_Charging_Legs
     void output_Charging_Legs()
     {
          if (flg_State){ return; }
          cout << "  #CL:" << setting_Number_Of_Legs.I;
          for (int cou_Index=0;cou_Index<setting_Number_Of_Legs.I;cou_Index++)
          {
               cout << " ^" << cou_Index << "-" << setting_Charge_Legs[cou_Index];
          }
          
     }
     
     //Outputs the references to the node indexes.
     void output_Node_References()
     {
          if (!Dendrite_Count && !Axon_Count){ return; }
          
          //cout << "\n output_Node_References()"; cout.flush();
          
          gather_Connection_References();
          //cout << " connection_References_Gathered..."; cout.flush();
          
          if (Dendrite_Count){ cout << " DR:"; }
          for (int cou_Index=0;cou_Index<Dendrite_Count;cou_Index++)
          {
               cout << " " << Dendrite_Target_Reference[cou_Index].Construct << "-" << Dendrite_Target_Reference[cou_Index].Node;
               cout.flush();
          }
          
          if (Axon_Count){ cout << " AR:"; }
          for (int cou_Index=0;cou_Index<Axon_Count;cou_Index++)
          {
               cout << " " << Axon_Target_Reference[cou_Index].Construct << "-" << Axon_Target_Reference[cou_Index].Node;
               cout.flush();
          }
     }
     
     //Outputs the poly node in a visual mode.
     void output_Poly_Node_Graphical()
     {
          if (flg_Treetop)
          {
               output_Console_Character_Colored(0, Output_Charge, char (30));
          } 
          else
          {
               output_Console_Character_Colored(0, Output_Charge, char (30));
          }
     }
     
     //Neutralizes the charges in the node.
     void output_Poly_Node_Graphical_Absolute_Charged()
     {
          //Outputs the node when neutralized if the node was charged.
          if (Output_Charge){ output_Poly_Node_Graphical_Absolute(); }
     }
     
     //Outputs the poly node in a visual mode.
     void output_Poly_Node_Graphical_Absolute()
     {
          console_Move_Cursor_To_Given_X_And_Y((NID + X), (Tier + Y));
          
          output_Console_Character_Colored(0, Output_Charge, char (30 + Output_Backpropagation_Charge));
          
     }
     
     //Outputs the poly node in a visual mode.
     void output_Poly_Node_Graphical_Absolute_Full(int p_Separator = 15, int p_Aspect_Ratio = 5)
     {
          console_Move_Cursor_To_Given_X_And_Y(((NID * p_Separator) + X), ((Tier * (p_Separator * p_Aspect_Ratio)) + Y));
          
          //output_Console_Character_Colored(0, Output_Charge, char (30 + Output_Backpropagation_Charge));
          
          for (int cou_D=0;cou_D<Dendrite_Count;cou_D++){
                console_Draw_Line(((NID * p_Separator) + (X * p_Separator)), (((Tier * (p_Separator * p_Aspect_Ratio)) - 1) + Y), ((((c_Poly_Node*) Dendrites[cou_D])->NID * p_Separator) + (((c_Poly_Node*) Dendrites[cou_D])->X * p_Separator)), (((((c_Poly_Node*) Dendrites[cou_D])->Tier * (p_Separator * p_Aspect_Ratio)) + 2) + ((c_Poly_Node*) Dendrites[cou_D])->Y), 8, 177);
                console_Draw_Line(((NID * p_Separator) + (X * p_Separator)), (((Tier * (p_Separator * p_Aspect_Ratio)) - 1) + Y), ((((c_Poly_Node*) Dendrites[cou_D])->NID * p_Separator) + (((c_Poly_Node*) Dendrites[cou_D])->X * p_Separator)), (((((c_Poly_Node*) Dendrites[cou_D])->Tier * (p_Separator * p_Aspect_Ratio)) + 2) + ((c_Poly_Node*) Dendrites[cou_D])->Y), int (((cou_D + 10) + (cou_D % 2))), (177 + cou_D), .1);
                console_Draw_Line((((NID * p_Separator) + (X * p_Separator)) + 1), (((Tier * (p_Separator * p_Aspect_Ratio)) - 1) + Y), ((((c_Poly_Node*) Dendrites[cou_D])->NID * p_Separator) + (((c_Poly_Node*) Dendrites[cou_D])->X * p_Separator) + 1),  (((((c_Poly_Node*) Dendrites[cou_D])->Tier * (p_Separator * p_Aspect_Ratio)) + 2) + ((c_Poly_Node*) Dendrites[cou_D])->Y), Reinforcement_Counter + 9, (178 + Reinforcement_Counter));
          }
          //output_Poly_Node_Graphical();
          console_Move_Cursor_To_Given_X_And_Y(((NID * p_Separator) + (X * p_Separator)), ((Tier * (p_Separator * p_Aspect_Ratio)) + Y));
          output_State();
          //cout << "(" << this << ")";
     }
     
     //Outputs the poly node in a visual mode.
     void scribe_Poly_Node_Graphical_Absolute_Full(ofstream * p_File, int p_Separator = 15, int p_Aspect_Ratio = 5)
     {
          int tmp_X = ((NID * p_Separator) + X);
          int tmp_Y = ((Tier * (p_Separator * p_Aspect_Ratio)) + Y);
          int tmp_TX = 0;
          int tmp_TY = 0;
          //Save_File << "crt " << Constructs[cou_Index]->Name << " " << Constructs[cou_Index]->Construct_Type << "\n";
          
          //cout << "\n\n --" << State << "  NID:" << NID << "  Tier:" << Tier << "  tmp_X:" << tmp_X << "  tmp_Y:" << tmp_Y;
          
          //console_Move_Cursor_To_Given_X_And_Y(((NID * p_Separator) + X), ((Tier * (p_Separator * p_Aspect_Ratio)) + Y));
          *p_File << "\n\nvar Node_X = " << tmp_X << ";";
          *p_File << "\tvar Node_Y = " << tmp_Y << ";";
          
          //console_Move_Cursor_To_Given_X_And_Y(((NID * p_Separator) + (X * p_Separator)), ((Tier * (p_Separator * p_Aspect_Ratio)) + Y));
          //output_State();
          *p_File << "\nctx.fillText(\"" << Tier << ":" << NID << "\");";
          
          //output_Console_Character_Colored(0, Output_Charge, char (30 + Output_Backpropagation_Charge));
          
          for (int cou_D=0;cou_D<Dendrite_Count;cou_D++){
                tmp_TX = ((((c_Poly_Node*) Dendrites[cou_D])->NID * p_Separator) + (((c_Poly_Node*) Dendrites[cou_D])->X * p_Separator));
                tmp_TY = (((((c_Poly_Node*) Dendrites[cou_D])->Tier * (p_Separator * p_Aspect_Ratio)) + 2) + ((c_Poly_Node*) Dendrites[cou_D])->Y);
                
                //cout << "\n tmp_TX:" << tmp_TX << " tmp_TY:" << tmp_TY;
                
                //console_Draw_Line(((NID * p_Separator) + (X * p_Separator)), (((Tier * (p_Separator * p_Aspect_Ratio)) - 1) + Y), ((((c_Poly_Node*) Dendrites[cou_D])->NID * p_Separator) + (((c_Poly_Node*) Dendrites[cou_D])->X * p_Separator)), (((((c_Poly_Node*) Dendrites[cou_D])->Tier * (p_Separator * p_Aspect_Ratio)) + 2) + ((c_Poly_Node*) Dendrites[cou_D])->Y), 8, 177);
                *p_File << "\nctx.moveTo(" << ((NID * p_Separator) + (X * p_Separator)) << ", " << (((Tier * (p_Separator * p_Aspect_Ratio)) - 1) + Y) << ");";
                *p_File << "\tctx.lineTo(" << tmp_TX << ", " << tmp_TY << "\");";
                *p_File << "\tctx.stroke()";
                
                //console_Draw_Line((((NID * p_Separator) + (X * p_Separator)) + 1), (((Tier * (p_Separator * p_Aspect_Ratio)) - 1) + Y), ((((c_Poly_Node*) Dendrites[cou_D])->NID * p_Separator) + (((c_Poly_Node*) Dendrites[cou_D])->X * p_Separator) + 1),  (((((c_Poly_Node*) Dendrites[cou_D])->Tier * (p_Separator * p_Aspect_Ratio)) + 2) + ((c_Poly_Node*) Dendrites[cou_D])->Y), Reinforcement_Counter + 9, (178 + Reinforcement_Counter));
                *p_File << "\nctx.moveTo(" << (((NID * p_Separator) + (X * p_Separator)) + 1) << ", " << (((Tier * (p_Separator * p_Aspect_Ratio)) - 1) + Y) << ");";
                *p_File << "\tctx.lineTo(" << (tmp_TX + 1) << ", " <<  tmp_TY << ");";
                *p_File << "\tctx.stroke();";
          }
          //output_Poly_Node_Graphical();
          //cout << "(" << this << ")";
     }
     
     //Outputs the poly node in a visual mode.
     void output_Poly_Node_Graphical_Absolute_Torch()
     {
          console_Move_Cursor_To_Given_X_And_Y((NID + X), (Tier + Y));
           
          output_Console_Character_Colored(0, flg_Torch + 1, flg_Torch + 1);
     }
     /*
     //Outputs the poly node in a visual mode.
     void output_Poly_Node_Info_Absolute()
     {
          return;
          console_Move_Cursor_To_Given_X_And_Y(0, 0);
          for (int cou_Index=0;cou_Index<1;cou_Index++){
               cout << "                                                                                                                                                \n";
          }
          console_Move_Cursor_To_Given_X_And_Y(0, 0);
          //output_Poly_Node();

     }*/
     
     //Outputs the poly node in a visual mode.
     void output_Poly_Node_Graphical_Absolute_RC()
     {
          console_Move_Cursor_To_Given_X_And_Y(NID, (Tier + 50));
          
          if (flg_Treetop)
          {
               output_Console_Character_Colored(0, Output_Charge, char (30 + Reinforcement_Counter));
          } 
          else
          {
               output_Console_Character_Colored(0, Output_Charge, char (30 + Reinforcement_Counter));
          }
     }
     
     //Outputs the node dendrites.
     void output_Node_Dendrites()
     {
          cout << " #D" << Dendrite_Count;
          for (int cou_D=0;cou_D<Dendrite_Count;cou_D++){
               cout << " " << cou_D << ":" << Dendrites[cou_D];
          }
     }
     
     //Ouputs the nodes axons.
     void output_Node_Axons()
     {
          cout << " #A" << Axon_Count;
          for (int cou_A=0;cou_A<Axon_Count;cou_A++){
               cout << " " << cou_A << ":" << Axons[cou_A];
          }
     }
     
     //outputs the nodes state.
     void output_State()
     {
          //cout << " " << State_Length;
          
          //If this node has a state set then output it.
          if (State_Length)
          {
               //Opening quotations.
               cout << " \"";
               
               //Loops through each of the state nodes outputing their values.
               for (int cou_State=0;cou_State<State_Length;cou_State++){
                    cout << char (State[cou_State].I);
               }
               
               //Ending quotations.
               cout << "\" ";
          }
          else
          {
               cout << " -NO STATE- ";
          }
     }
     
     //outputs the nodes charges.
     void output_Charges()
     {
          //If this node has a state set then output it.

               //Ouputs the value.
               cout << "   Ch:" << Charge << "  ";
     }
};