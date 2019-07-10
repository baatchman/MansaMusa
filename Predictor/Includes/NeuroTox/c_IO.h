

class c_IO
{
     public:
     
     //the Output states
     u_IO_Data * State;
     
     //The charges for each state.
     float * State_Charge;
     
     //The current number of states.
     int Current_Number_Of_States;
     
     //The current max number of states. Used for determining when to increase the array size.
     int Max_Number_Of_States;
     
     c_IO()
     {
          State = NULL;
          State_Charge = NULL;
          Current_Number_Of_States = 0;
          Max_Number_Of_States = 0;
          reset_IO();
     }
     
     ~c_IO()
     {
          delete [] State;
          delete [] State_Charge;
     }
     
     
     //Increases the size of the array.
	 void increase_State_Array_Depth()
	 {
//          cout << "\n ~increase_State_Array_Depth() "; cout.flush();
          
          //create the temporary node array to hold the original while it is rebuilt
          u_IO_Data * tmp_Nodes;
          float * tmp_Charge;
          
          //increases the Max_Number_Of_States by 100 and creates the temporary array to hold the State references
          Max_Number_Of_States += 10;
          tmp_Nodes = new u_IO_Data[Max_Number_Of_States];
          tmp_Charge = new float[Max_Number_Of_States];
          
          //Initializes the newly allocated arrays.
          for (int cou_State=0;cou_State<Max_Number_Of_States;cou_State++){
               tmp_Nodes[cou_State].I = 0;
               tmp_Charge[cou_State] = 0.0;
          }
          
          //copy the original into the temporary array
          for (int cou_Index=0;cou_Index<Current_Number_Of_States;cou_Index++){
               tmp_Nodes[cou_Index].NR = State[cou_Index].NR;
               tmp_Charge[cou_Index] = State_Charge[cou_Index];
               }
          //delete the original
          delete [] State;
          delete [] State_Charge;
          
          //sets the newly created array reference to the newly allocated temporary array reference
          State = tmp_Nodes;
          State_Charge = tmp_Charge;
          
//          cout << "\n ~Finished: increase_State_Array_Depth() "; cout.flush();
    }    
     
     //Resets the node
     void reset_IO()
     {
          //Deletes the memory allocated for the state arrays.
          delete [] State;
          delete [] State_Charge;
          
          //Reset the counters.
          Current_Number_Of_States = 0;
          Max_Number_Of_States = 10;
          
          //Request new memory for the states.
          State = new u_IO_Data[Max_Number_Of_States];
          State_Charge = new float[Max_Number_Of_States];
          
          //Initializes the newly allocated arrays.
          for (int cou_State=0;cou_State<Max_Number_Of_States;cou_State++){
               State[cou_State].I = 0;
               State_Charge[cou_State] = 0.0;
          }
     }
     
     //Accepts a given state and charge.
     //Checks for whether or not the given state is in the IO
     // and if not adds it then sets the charge to the given charge. If the state is 
     //found then the charge is added to it
     void submit_State(u_IO_Data &p_State, float p_Charge)
     {
//          cout << "\n ~c_IO::submit_State(" << char (p_State.I) << ", " << p_Charge << ") "; cout.flush();
          
//          cout << "\n ~c_IO::Current_Number_Of_States:" << Current_Number_Of_States; cout.flush();
          
          //Checks each state to see if it is already added to this IO
          for (int cou_State=0;cou_State<Current_Number_Of_States;cou_State++){
//               cout << "\n ~c_IO::State[" << cou_State << "]:"; cout.flush();
//               cout << " I~" << State[cou_State].I  << " NR~" << State[cou_State].NR; cout.flush();
               if (State[cou_State].NR == p_State.NR){ State_Charge[cou_State] += p_Charge; return; }
          }
//          cout << "\n ~c_IO::!State found;"; cout.flush();
          
          //If the state is not found during the above search it is added and charged.
          
          //Sets the current state variables to the given ones.
          State[Current_Number_Of_States].NR = p_State.NR;
          State_Charge[Current_Number_Of_States] = p_Charge;
          
          //Increase the number of states in the IO.
          Current_Number_Of_States++;
          
          //Checks to make sure we do not overflow the arrays.
          if (Current_Number_Of_States == Max_Number_Of_States){ increase_State_Array_Depth(); }
          
//          cout << "\n ~Finished: c_IO::submit_State(" << char (p_State.I) << ", " << p_Charge << ") "; cout.flush();
     }
     
     //Accepts a given state and charge.
     //Sets the given State index to the given charge and state
     void set_State(u_IO_Data &p_State, float p_Charge, int p_State_Index = 0)
     {
          //Set the given State to the given data
          State[p_State_Index].NR = p_State.NR;
          State_Charge[p_State_Index] = p_Charge;
          
     }
     
     //Gets the given index.
     u_IO_Data get_State(int p_Index)
     {
          return State[p_Index];
     }
     
     //This function outputs all the IO states and their charges.
     void output_IO_I()
     {
          cout << "\n\t~#~" << Current_Number_Of_States;
          for (int cou_State=0;cou_State<Current_Number_Of_States;cou_State++){
               cout << "\t~S~" << State[cou_State].I << " ~C~" << State_Charge[cou_State] << "  \t";
          }
     }
     
     //This function outputs all the IO states and their charges.
     void output_IO_C()
     {
          cout << "\n\t~#~" << Current_Number_Of_States;
          for (int cou_State=0;cou_State<Current_Number_Of_States;cou_State++){
               cout << "\t~S~" << State[cou_State].C << " ~C~" << State_Charge[cou_State] << "  \t";
          }
     }
     
     //This function outputs all the IO states and their charges.
     void output_IO_C_Strongest()
     {
          float tmp_Charge = 0.0;
          int tmp_Index = 0;
          for (int cou_State=0;cou_State<Current_Number_Of_States;cou_State++){
               if (tmp_Charge < State_Charge[cou_State]){ tmp_Charge = State_Charge[cou_State]; tmp_Index = cou_State; }
          }
          cout << State[tmp_Index].C;
     }
     
};







//This class handles input. It resizes itself for the input set given.
class c_1D_Input
{
public:
     
     //The output nodes.
     c_IO ** IO_Node;
     
     //The Current number of nodes in the output.
     int Current_Number_Of_IO_Nodes;
     
     //The Current Max Number Of Nodes in the output.
     int Max_Number_Of_IO_Nodes;
     
     //The temporary data for output gathering.
     u_IO_Data * tmp_Data;
     
     c_1D_Input()
     {
          Max_Number_Of_IO_Nodes = 0;
          Current_Number_Of_IO_Nodes = 0;
          IO_Node = NULL;
          reset_IO_Nodes();
     }
     
     //Increases the size of the array.
	 void increase_IO_Array_Depth()
	 {
          //create the temporary node array to hold the original while it is rebuilt
          c_IO ** tmp_Nodes;
          
          //increases the Max_Number_Of_States by 100 and creates the temporary array to hold the State references
          Max_Number_Of_IO_Nodes += 10;
          tmp_Nodes = new c_IO*[Max_Number_Of_IO_Nodes];
          
          //NULL the tmp_Nodes
          for (int cou_Index=0;cou_Index<Max_Number_Of_IO_Nodes;cou_Index++){
               tmp_Nodes[cou_Index] = NULL;
          }
          
          //copy the original into the temporary array
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++){
               tmp_Nodes[cou_Index] = IO_Node[cou_Index];
               
               //NUll the IO_Node for deletion
               IO_Node[cou_Index] = NULL;
               }
               
          //delete the original
          delete [] IO_Node;
          
          //sets the newly created array reference to the newly allocated temporary array reference
          IO_Node = tmp_Nodes;
    }    
     
     //Takes an Input and puts it in the appropriate place
          //u_IO_Data &p_State -The State of The IO
          //float p_Charge -The Charge to Apply to the IO
          //int p_Index -The Index Of The IO To Set
     void submit_Single_State(u_IO_Data &p_State, float p_Charge, int p_Index)
     {
          //If the index is higher than the current number of indexes in the array then increase it.
          if (p_Index >= Max_Number_Of_IO_Nodes){ increase_IO_Array_Depth(); }
          
          //If the index is higher than the current number of nodes in the array set the current number of nodes to the given index.
          if (p_Index >= Current_Number_Of_IO_Nodes){ Current_Number_Of_IO_Nodes = p_Index + 1; }
          
          //If the node is not yet set, create it.
          if (IO_Node[p_Index] == NULL){ IO_Node[p_Index] = new c_IO; }
          
          //Sumit the state and charge to the given IO index.
          IO_Node[p_Index]->set_State(p_State, p_Charge);
          
     }
     
     //Takes an output set of characters and parses it.
     void submit_Multiple_C(char p_Input[], int p_Input_Length, float p_Charge)
     {
          //Tmp variable to hold the current state.
          u_IO_Data tmp_State;
          for (int cou_Index=0;cou_Index<p_Input_Length + 1;cou_Index++){
               tmp_State.I = int (p_Input[cou_Index]);
               submit_Single_State(tmp_State, p_Charge, cou_Index);
               
          }
     }
     
     //Resets the node
     void reset_IO_Nodes()
     {
          //Delete all the individual indexes in the IO_Node array.
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++){
               delete IO_Node[cou_Index];
          }
          
          //Delete the IO_Node array to free up the pointer.
          delete [] IO_Node;
          
          //Reset the current number of IO nodes.
          Current_Number_Of_IO_Nodes = 0;
          
          //Reset the Max Number Of IO Nodes.
          Max_Number_Of_IO_Nodes = 10;
          
          //Create the IO_Node array with the current Max Number Of Nodes.
          IO_Node = new c_IO*[Max_Number_Of_IO_Nodes];
          
          //Create a new c_IO in each index in the newly allocated IO_Node array.
          for (int cou_Index=0;cou_Index<Max_Number_Of_IO_Nodes;cou_Index++){
               IO_Node[cou_Index] = new c_IO;
          }
     }
     
     //Gets the current set for the 0 index, used for the output gathering.
     u_IO_Data * get_Current_Output(int p_Index = 0)
     {
          //Create the temporary array.
          delete [] tmp_Data;
          tmp_Data = new u_IO_Data[Current_Number_Of_IO_Nodes];
          
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++)
          {
               tmp_Data[cou_Index].NR = IO_Node[cou_Index]->get_State(p_Index).NR;
          }
          
          return tmp_Data;
     }
     
     //Outputs all the IO_Nodes
     void output_C()
     {
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++){
               IO_Node[cou_Index]->output_IO_C();
          }
     }
     
     //Outputs all the IO_Nodes
     void output_C_Strongest()
     {
          cout << "\n";
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++){
               IO_Node[cou_Index]->output_IO_C_Strongest();
          }
     }
};




//This class handles the output.
class c_1D_Output
{
public:
     
     //The output nodes.
     c_IO ** IO_Node;
     
     //The Current number of nodes in the output.
     int Current_Number_Of_IO_Nodes;
     
     //The Current Max Number Of Nodes in the output.
     int Max_Number_Of_IO_Nodes;
     
     //The current index of the state being pushed onto the output.
     int Push_Index;
     
     //The current offset of the output.
     int Push_Offset;
     
     //The temporary data for output gathering.
     u_IO_Data * tmp_Data;
     
     //The type of output, when set to 0 it is normal, if 1 then it is a direct node reference and return -1 on push() or pop();
     int Output_Type;
     
     // -p_Output_Type:
     //   0: Normal Output
     //   1: Direct Node Reference Output
     c_1D_Output(int p_Output_Type = 0)
     {
          Max_Number_Of_IO_Nodes = 0;
          Current_Number_Of_IO_Nodes = 0;
          IO_Node = NULL;
          reset_IO_Nodes();
          Push_Index = 0;
          Push_Offset = 0;
          Output_Type = p_Output_Type;
          
          tmp_Data = new u_IO_Data[5];
          
     }
     
     //Increases the size of the array.
	 void increase_IO_Array_Depth()
	 {
//          cout << "\n ~increase_IO_Array_Depth() "; cout.flush();
          
          //create the temporary node array to hold the original while it is rebuilt
          c_IO ** tmp_Nodes;
          
          //increases the Max_Number_Of_States by 100 and creates the temporary array to hold the State references
          Max_Number_Of_IO_Nodes += 10;
          tmp_Nodes = new c_IO*[Max_Number_Of_IO_Nodes];
          
          //NULL the tmp_Nodes
          for (int cou_Index=0;cou_Index<Max_Number_Of_IO_Nodes;cou_Index++){
               tmp_Nodes[cou_Index] = NULL;
          }
          
          //copy the original into the temporary array
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++){
               tmp_Nodes[cou_Index] = IO_Node[cou_Index];
               
               //NUll the IO_Node for deletion
               IO_Node[cou_Index] = NULL;
               }
               
          //delete the original
          delete [] IO_Node;
          
          //sets the newly created array reference to the newly allocated temporary array reference
          IO_Node = tmp_Nodes;
          
//          cout << "\n ~Finished: increase_IO_Array_Depth() "; cout.flush();
    }    
     
     //Takes an output and puts it in the appropriate place
          //u_IO_Data &p_State -The State of The IO
          //float p_Charge -The Charge to Apply to the IO
          //int p_Index -The Index Of The IO To Set
     void submit_Single_State(u_IO_Data &p_State, float p_Charge, int p_Index)
     {
//          cout << "\n ~submit_Single_State(" << char (p_State.I) << ", " << p_Charge << ", " << p_Index << ") "; cout.flush();
          
          //If the index is higher than the current number of indexes in the array then increase it.
          if (p_Index >= Max_Number_Of_IO_Nodes){ increase_IO_Array_Depth(); }
          
          //If the index is higher than the current number of nodes in the array set the current number of nodes to the given index.
          if (p_Index >= Current_Number_Of_IO_Nodes){ Current_Number_Of_IO_Nodes = p_Index + 1; }
          
          //cout << "\n\t\t Submit state to IO_Node[p_Index]"; cout.flush();
          
          //If the node is not yet set, create it.
          if (IO_Node[p_Index] == NULL){ IO_Node[p_Index] = new c_IO; }
          
          //Sumit the state and charge to the given IO index.
          IO_Node[p_Index]->submit_State(p_State, p_Charge);
          
//          cout << "\n ~Finished: submit_Single_State(" << char (p_State.I) << ", " << p_Charge << ", " << p_Index << ") "; cout.flush();
     }
     
     //Takes an output set of characters and parses it.
     void submit_Multiple_C(char p_Output[], int p_Input_Length, float p_Charge)
     {
          //Tmp variable to hold the current state.
          u_IO_Data tmp_State;
          for (int cou_Index=0;cou_Index<p_Input_Length + 1;cou_Index++){
               tmp_State.I = int (p_Output[cou_Index]);
               submit_Single_State(tmp_State, p_Charge, cou_Index);
               
          }
     }
     
     //Gets the current set for the 0 index, used for the output gathering.
     u_IO_Data * get_Current_Output(int p_Index = 0)
     {
          //Create the temporary array.
          delete [] tmp_Data;
          tmp_Data = new u_IO_Data[Current_Number_Of_IO_Nodes];
          
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++)
          {
               tmp_Data[cou_Index].NR = IO_Node[cou_Index]->get_State(p_Index).NR;
          }
          
          return tmp_Data;
     }
     
     //Gets the curren tnumber of nodes.
     int get_Data_Length()
     {
          return Current_Number_Of_IO_Nodes;
     }
     
     //Takes an integer and sets the output offset to the given number.
     void push_Offset(int p_Push_Offset = 0)
     {
          //Sets the offset to the given offset.
          Push_Offset = p_Push_Offset;
     }
     
     //Takes an output and pushes it onto the output stack.
     int push(u_IO_Data &p_State, float p_Charge)
     {
//          cout << "\n ~push(" << char (p_State.I) << ") "; cout.flush();

          //If this is a direct reference output then reutrn -1.
          if (Output_Type == 1)
          {
               return -1;
          }
          
          //Submits the state to the current push index.
          submit_Single_State(p_State, p_Charge, Push_Index);
          
          //Increment the push index.
          Push_Index++;
          
          return Push_Index;
//          cout << " ~Finished: push(" << char (p_State.I) << ") "; cout.flush();
     }
     
     //Resets the Push_Index.
     int pop()
     {
//          cout << " pop() "; cout.flush();
//          cout << "\n pop()";

          //If this is a direct reference output then reutrn -1.
          if (Output_Type == 1)
          {
               return -1;
          }
          
          Push_Index = 0;
          
          return Push_Index;
     }
     
     
     
     //Resets the node
     void reset_IO_Nodes()
     {
          //Delete all the individual indexes in the IO_Node array.
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++){
               delete IO_Node[cou_Index];
          }
          
          //Delete the IO_Node array to free up the pointer.
          delete [] IO_Node;
          
          //Reset the current number of IO nodes.
          Current_Number_Of_IO_Nodes = 0;
          
          //Reset the Max Number Of IO Nodes.
          Max_Number_Of_IO_Nodes = 10;
          
          //Create the IO_Node array with the current Max Number Of Nodes.
          IO_Node = new c_IO*[Max_Number_Of_IO_Nodes];
          
          //Create a new c_IO in each index in the newly allocated IO_Node array.
          for (int cou_Index=0;cou_Index<Max_Number_Of_IO_Nodes;cou_Index++){
               IO_Node[cou_Index] = new c_IO;
          }
          
          Push_Index = 0;
     }
     
     
     //Outputs all the IO_Nodes
     void output_C()
     {
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++){
               IO_Node[cou_Index]->output_IO_C();
          }
     }
     
     //Outputs all the IO_Nodes
     void output_C_Strongest()
     {
          //cout << "\n\t Output_C_Strongest:";
          cout << "\n ~{";
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++){
               IO_Node[cou_Index]->output_IO_C_Strongest();
          }
          cout << "}~";
     }
};










/*

//This class handles the output.
class c_2D_Output
{
public:
     
     //The output nodes.
     c_IO *** IO_Node;
     
     //The Current number of nodes in the output X axis.
     int Current_Number_Of_IO_Nodes_X;
     
     //The Current number of nodes in the output Y axis.
     int Current_Number_Of_IO_Nodes_Y;
     
     //The Current Max Number Of Nodes in the output X.
     int Max_Number_Of_IO_Nodes_X;
     
     //The Current Max Number Of Nodes in the output Y.
     int Max_Number_Of_IO_Nodes_Y;
     
     //The current index of the state being pushed onto the output in the X axis.
     int Push_Index_X;
     
     //The current index of the state being pushed onto the output in the X axis.
     int Push_Index_Y;
     
     //The current offset of the output.
     int Push_Offset;
     
     //The temporary data for output gathering.
     u_IO_Data * tmp_Data;
     
     //The type of output, when set to 0 it is normal, if 1 then it is a direct node reference and return -1 on push() or pop();
     int Output_Type;
     
     // -p_Output_Type:
     //   0: Normal Output
     //   1: Direct Node Reference Output
     c_2D_Output(int p_Output_Type = 0)
     {
          Max_Number_Of_IO_Nodes_X = 0;
          Max_Number_Of_IO_Nodes_Y = 0;
          Current_Number_Of_IO_Nodes_X = 0;
          Current_Number_Of_IO_Nodes_Y = 0;
          IO_Node = NULL;
          reset_IO_Nodes();
          Push_Index_X = 0;
          Push_Index_Y = 0;
          Push_Offset = 0;
          Output_Type = p_Output_Type;
     }
     
     //Increases the size of the array.
	 void increase_IO_Array_Depth_X()
	 {
//          cout << "\n ~increase_IO_Array_Depth() "; cout.flush();
          
          //create the temporary node array to hold the original while it is rebuilt
          c_IO *** tmp_Nodes;
          
          //increases the Max_Number_Of_States by 100 and creates the temporary array to hold the State references
          Max_Number_Of_IO_Nodes_X += 10;
          tmp_Nodes = new c_IO**[Max_Number_Of_IO_Nodes_X];
          for (int cou_Index=0;cou_Index<Max_Number_Of_IO_Nodes_X;cou_Index++)
          {
               tmp_Nodes[cou_Index] = new c_IO*[Max_Number_Of_IO_Nodes_Y];
          }
          
          
          //NULL the tmp_Nodes
          for (int cou_Index=0;cou_Index<Max_Number_Of_IO_Nodes_X;cou_Index++){
               for (int cou_IndexY;cou_IndexY<Max_Number_Of_IO_Nodes_Y;cou_IndexY++)
               {
                    tmp_Nodes[cou_Index] [cou_IndexY]= NULL;
               }
          }
          
          //copy the original into the temporary array
          for (int cou_Index=0;cou_Index<Max_Number_Of_IO_Nodes_X;cou_Index++){
               for (int cou_IndexY;cou_IndexY<Max_Number_Of_IO_Nodes_Y;cou_IndexY++)
               {
                    tmp_Nodes[cou_Index] [cou_IndexY] = IO_Node[cou_Index] [cou_IndexY];
               
                    //NUll the IO_Node for deletion
                    IO_Node[cou_Index] [cou_IndexY] = NULL;
               }
               delete [] IO_Node[cou_Index];
          }
               
          //delete the original
          delete [] IO_Node;
          
          //sets the newly created array reference to the newly allocated temporary array reference
          IO_Node = tmp_Nodes;
          
//          cout << "\n ~Finished: increase_IO_Array_Depth() "; cout.flush();
    }    
     
     //Increases the size of the array.
	 void increase_IO_Array_Depth_Y()
	 {
//          cout << "\n ~increase_IO_Array_Depth() "; cout.flush();
          
          //create the temporary node array to hold the original while it is rebuilt
          c_IO *** tmp_Nodes;
          
          //increases the Max_Number_Of_States by 100 and creates the temporary array to hold the State references
          Max_Number_Of_IO_Nodes_Y += 10;
          tmp_Nodes = new c_IO**[Max_Number_Of_IO_Nodes_X];
          for (int cou_Index=0;cou_Index<Max_Number_Of_IO_Nodes_X;cou_Index++)
          {
               tmp_Nodes[cou_Index] = new c_IO*[Max_Number_Of_IO_Nodes_Y];
          }
          
          
          //NULL the tmp_Nodes
          for (int cou_Index=0;cou_Index<Max_Number_Of_IO_Nodes_X;cou_Index++){
               for (int cou_IndexY;cou_IndexY<Max_Number_Of_IO_Nodes_Y;cou_IndexY++)
               {
                    tmp_Nodes[cou_Index] [cou_IndexY]= NULL;
               }
          }
          
          //copy the original into the temporary array
          for (int cou_Index=0;cou_Index<Max_Number_Of_IO_Nodes_X;cou_Index++){
               for (int cou_IndexY;cou_IndexY<Max_Number_Of_IO_Nodes_Y;cou_IndexY++)
               {
                    tmp_Nodes[cou_Index] [cou_IndexY] = IO_Node[cou_Index] [cou_IndexY];
               
                    //NUll the IO_Node for deletion
                    IO_Node[cou_Index] [cou_IndexY] = NULL;
               }
               delete [] IO_Node[cou_Index];
          }
               
          //delete the original
          delete [] IO_Node;
          
          //sets the newly created array reference to the newly allocated temporary array reference
          IO_Node = tmp_Nodes;
          
//          cout << "\n ~Finished: increase_IO_Array_Depth() "; cout.flush();
     }    
    
    
    
     //Takes an output set of characters and parses it.
     void submit_Multiple_C(char p_Output[], int p_Input_Length, float p_Charge)
     {
          //Tmp variable to hold the current state.
          u_IO_Data tmp_State;
          for (int cou_Index=0;cou_Index<p_Input_Length + 1;cou_Index++){
               tmp_State.I = int (p_Output[cou_Index]);
               submit_Single_State(tmp_State, p_Charge, cou_Index);
               
          }
     }
     
     //Gets the current set for the 0 index, used for the output gathering.
     u_IO_Data * get_Current_Output(int p_Index = 0)
     {
          //Create the temporary array.
          delete [] tmp_Data;
          tmp_Data = new u_IO_Data[Current_Number_Of_IO_Nodes];
          
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++)
          {
               tmp_Data[cou_Index].NR = IO_Node[cou_Index]->get_State(p_Index).NR;
          }
          
          return tmp_Data;
     }
     
     //Gets the current number of nodes.
     int get_Data_Length_X()
     {
          return Current_Number_Of_IO_Nodes_X;
     }
     
     //Gets the current number of nodes.
     int get_Data_Length_Y()
     {
          return Current_Number_Of_IO_Nodes_Y;
     }
     
     //Takes an integer and sets the output offset to the given number.
     void push_Offset(int p_Push_Offset = 0)
     {
          //Sets the offset to the given offset.
          Push_Offset = p_Push_Offset;
     }
     
     //Takes an output and pushes it onto the output stack.
     int push(u_IO_Data &p_State, float p_Charge)
     {
//          cout << "\n ~push(" << char (p_State.I) << ") "; cout.flush();

          //If this is a direct reference output then reutrn -1.
          if (Output_Type == 1)
          {
               return -1;
          }
          
          //Submits the state to the current push index.
          submit_Single_State(p_State, p_Charge, Push_Index);
          
          //Increment the push index.
          Push_Index++;
          
          return Push_Index;
//          cout << " ~Finished: push(" << char (p_State.I) << ") "; cout.flush();
     }
     
     //Increments the Y index.
     void pop_Y()
     {
          
     }
     
     //Resets the Push_Index.
     int pop()
     {
//          cout << " pop() "; cout.flush();
//          cout << "\n pop()";

          //If this is a direct reference output then reutrn -1.
          if (Output_Type == 1)
          {
               return -1;
          }
          
          Push_Index = 0;
          
          return Push_Index;
     }
     
     
     
     //Resets the node
     void reset_IO_Nodes()
     {
          //Delete all the individual indexes in the IO_Node array.
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++){
               delete IO_Node[cou_Index];
          }
          
          //Delete the IO_Node array to free up the pointer.
          delete [] IO_Node;
          
          //Reset the current number of IO nodes.
          Current_Number_Of_IO_Nodes = 0;
          
          //Reset the Max Number Of IO Nodes.
          Max_Number_Of_IO_Nodes = 10;
          
          //Create the IO_Node array with the current Max Number Of Nodes.
          IO_Node = new c_IO*[Max_Number_Of_IO_Nodes];
          
          //Create a new c_IO in each index in the newly allocated IO_Node array.
          for (int cou_Index=0;cou_Index<Max_Number_Of_IO_Nodes;cou_Index++){
               IO_Node[cou_Index] = new c_IO;
          }
          
          Push_Index = 0;
     }
     
     
     //Outputs all the IO_Nodes
     void output_C()
     {
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++){
               IO_Node[cou_Index]->output_IO_C();
          }
     }
     
     //Outputs all the IO_Nodes
     void output_C_Strongest()
     {
          //cout << "\n\t Output_C_Strongest:";
          cout << "\n ~{";
          for (int cou_Index=0;cou_Index<Current_Number_Of_IO_Nodes;cou_Index++){
               IO_Node[cou_Index]->output_IO_C_Strongest();
          }
          cout << "}~";
     }
};

*/















/*
 * counter for the current number of states.
 * state node array
 * 
 * push() state onto the output
 * pop() the state output, submits the output set to the main output 
*/

//This class holds an output that the sensory constructs connect to.
class c_Construct_Output
{
public:
     
     //Allows for pushing a state onto the output array
     void push()
     {
          
     }
     
     //Submits the output to the main output.
     void pop()
     {
          
     }
     
private:
     
     
};