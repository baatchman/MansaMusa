

//The heaqder file will be ASCII.
class c_NT3_NodeDB_Header
{
public:
     
     //The header file is opened using a traditional stream as it will be extremely small.
     ifstream LF;
     ofstream SF;
     
     //The current node the network is on, same as CNID.
     unsigned long long int Node_Count;
     
     //The current block each axon file is on. 
     unsigned long long int Axon_Current_Block_L;
     unsigned long long int Axon_Current_Block_R;
     
     //The current size of the mmap file in MB
     unsigned long long int Node_File_Size;
     
     //The current size of the mmap file in MB
     unsigned long long int Axon_File_Size_L;
     unsigned long long int Axon_File_Size_R;
     
     //The current values of the tiers and their depths.
     long int Tier_Current_Depth;
     long int Tier_Depth[32768];
     
     //Name of the network to open, not neccesary the same as the filename.
     string Network_Name;
     
     c_NT3_NodeDB_Header()
     {
          Node_Count = 1;
          Axon_Current_Block_R = 1;
          Axon_Current_Block_L = 1;
          
          //The current size of the mmap file in MB
          Node_File_Size = 1;
          
          //The current size of the mmap file in MB
          Axon_File_Size_L = 1;
          Axon_File_Size_R = 1;
          
          //The current values of the tiers and their depths.
          Tier_Current_Depth = 0;
          for (int cou_Index=0;cou_Index<32768;cou_Index++)
          {
               Tier_Depth[cou_Index] = 0;
          }
     }
     
     //Sets up a new DB header file.
     void new_File(string p_Dir, string p_Name, bool p_Disp_Output = 1)
     {
          if (p_Disp_Output){ cout << "\n --== new_File() " << p_Dir << "/" << p_Name; }
          
          if (SF.is_open()){ SF.close(); }
          string tmp_fName = p_Dir + "/" + p_Name + ".header.dat";
          
          //Try to create the file.
          SF.open(tmp_fName, ios::app);
          
          if (!SF.is_open())
          {
               cout << "\n No luck, abandon ship.";
               SF.close();
               return;
          }
          
          SF << "Network_Name " << Network_Name;
          SF << "\nNode_Count " << Node_Count;
          SF << "\nAxon_Current_Block_L " << Axon_Current_Block_L;
          SF << "\nAxon_Current_Block_R " << Axon_Current_Block_R;
          SF << "\nNode_File_Size " << Node_File_Size;
          SF << "\nAxon_File_Size_L " << Axon_File_Size_L;
          SF << "\nAxon_File_Size_R " << Axon_File_Size_R;
          SF << "\nTier_Current_Depth " << Tier_Current_Depth;
          SF << "\nTier " << 0 << " " << 0;
               
          if (p_Disp_Output){ cout << "\n Network " << Network_Name << " with " << Node_Count << " has been saved..."; }
          
          SF.close();
     }
     
     //Loads a DB header file.
     void load_File(string p_Dir, string p_Name)
     {
          //Throwaway ASCII junk.
          string tmp_Input;
          
          cout << "\n --==" << p_Dir << "/" << p_Name;
          
          //Can't hurt to make sure. That may not be true.
          
          if (LF.is_open()){ LF.close(); }
          string tmp_fName = p_Dir + "/" + p_Name + ".header.dat";
          LF.open(tmp_fName);
          
          if (!LF.is_open())
          {
               cout << "\n Header file \"" << p_Name << "\" did not open...";
          }
          
          if ((LF.peek() != std::ifstream::traits_type::eof()))
          {
               cout << "\n Header file found, loading...";
               
               //File is in IDENTIFIER VALUE format, so throw away ident and load value.
               LF >> tmp_Input;
               LF >> Network_Name;
               
               LF >> tmp_Input;
               LF >> Node_Count;
               
               LF >> tmp_Input;
               LF >> Axon_Current_Block_L;
               
               LF >> tmp_Input;
               LF >> Axon_Current_Block_R;
               
               //Get the sizes.
               LF >> tmp_Input;
               LF >> Node_File_Size;
               
               LF >> tmp_Input;
               LF >> Axon_File_Size_L;
               
               LF >> tmp_Input;
               LF >> Axon_File_Size_R;
               
               LF >> tmp_Input;
               LF >> Tier_Current_Depth;
               
               cout << "\n\n Tier_Current_Depth:" << Tier_Current_Depth;
               for (int cou_T=0;cou_T<(Tier_Current_Depth);cou_T++)
               {
                    //The "Tier"
                    LF >> tmp_Input;
                    //The Tier Value
                    LF >> tmp_Input;
                    LF >> Tier_Depth[cou_T];
                    cout << "\n ---- Tier_Depth[" << cou_T << ":" << Tier_Depth[cou_T];
               }
          
          }
          else
          {
               cout << "\n Header file not found, creating new datas...";
               Network_Name = p_Name;
               Node_Count = 1;
               Axon_Current_Block_R = 0;
               Axon_Current_Block_L = 0;
               
               //The current size of the mmap file in MB
               Node_File_Size = 1;
               
               //The current size of the mmap file in MB
               Axon_File_Size_L = 1;
               Axon_File_Size_R = 1;
          }
          cout << "\n Network " << Network_Name << " with " << Node_Count << " has been loaded...";
          LF.close();
     }
     
     //Saves a DB header file.
     void save_File(string p_Dir, string p_Name, bool p_Disp_Output = 1)
     {
          if (p_Disp_Output){ cout << "\n --==" << p_Dir << "/" << p_Name; }
          
          if (SF.is_open()){ SF.close(); }
          string tmp_fName = p_Dir + "/" + p_Name + ".header.dat";
          SF.open(tmp_fName, ios::trunc);
          
          if (!SF.is_open())
          {
               cout << "\n Header file \"" << tmp_fName << "\" did not open for saving, attempting creation...";
               
               //Try to create the file.
               SF.close();
               SF.open(tmp_fName, ios::app);
               if (!SF.is_open())
               {
                    cout << "\n No luck, abandon ship.";
                    SF.close();
                    return;
               }
          }
          
          SF << "Network_Name " << Network_Name;
          SF << "\nNode_Count " << Node_Count;
          SF << "\nAxon_Current_Block_L " << Axon_Current_Block_L;
          SF << "\nAxon_Current_Block_R " << Axon_Current_Block_R;
          SF << "\nNode_File_Size " << Node_File_Size;
          SF << "\nAxon_File_Size_L " << Axon_File_Size_L;
          SF << "\nAxon_File_Size_R " << Axon_File_Size_R;
               
          SF << "\nTier_Current_Depth " << Tier_Current_Depth;
          for (int cou_T=0;cou_T<(Tier_Current_Depth + 1);cou_T++)
          {
               SF << "\nTier " << cou_T << " " << Tier_Depth[cou_T];
          }
          
          if (p_Disp_Output){ cout << "\n Network " << Network_Name << " with " << Node_Count << " has been saved..."; }
          
          SF.close();
     }
     
     //Returns the current number of nodes in the network.
     unsigned long long int get_CNID()
     {
          return Node_Count;
     }
};

//This class holds the temporary node data for the currently loaded node.
class c_NT3_NodeDB_Node
{
private:

     //Axons and their counters.
     long int Axon_Count_L;
     u_Data_3 Axon_Block_L;
     long int Axon_Count_R;
     u_Data_3 Axon_Block_R;
     
public:
     
     //Holds the save datas.
     char tmp_Data[128];
     
     //==--  All members to save
     
     //Whether or not the node is a state node.
     //0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     char Type;
     
     //The NID for the current node.
     u_Data_3 NID;
     
     //Dendrites.
     u_Data_3 Dendrite_L;
     u_Data_3 Dendrite_R;
     
     //Reinforcement Counters.
     double RC_Lvl;
     
     //The nodes state.
     u_Data_3 State;
     
     //The Tier the node is on.
     long int Tier;
     
     //The index in the tier the node sits at.
     long int Tier_Index;
     
     c_NT3_NodeDB_Node()
     {
          wipe_Data();
          
          //0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
          //Whether or not the node is a state node.
          Type = 1;
          
          //Axons and their counters.
          Axon_Count_L = 0;
          
          Axon_Count_R = 0;
          Axon_Block_R.U = 0;
          Axon_Block_L.U = 0;
          
          //Dendrites.
          Dendrite_L.U = 0;
          
          Dendrite_R.U = 0;
          
          //Reinforcement Counters.
          RC_Lvl = 0.0;
          
          State.U = 0;
          
          Tier = 0;
          
          Tier_Index = 0;
     }
     
     //==--  GETS AND SETS
     
     unsigned long long int get_Tier(){ return Tier; }
     unsigned long long int get_Tier_Index(){ return Tier_Index; }
     
     long int get_Axon_Count_L(){ return Axon_Count_L; }
     unsigned long long int get_Axon_Block_L(){ return Axon_Block_L.U; }
     long int get_Axon_Count_R(){ return Axon_Count_R; }
     unsigned long long int get_Axon_Block_R(){ return Axon_Block_R.U; }
     
     void set_Axon_Count_L(long int p_Axon_Count_L)
     { 
          //ostr(0, 11, "\n -->set_Axon_Count_L"); cout << " " << p_Axon_Count_L << " NID " << NID.U;
          Axon_Count_L = p_Axon_Count_L; 
     }
     void set_Axon_Block_L(unsigned long long int p_Axon_Block_L)
     { 
          //ostr(0, 11, "\n -->set_Axon_Block_L"); cout << " " << p_Axon_Block_L << " NID " << NID.U;
          Axon_Block_L.U = p_Axon_Block_L; 
     }
     void set_Axon_Count_R(long int p_Axon_Count_R)
     { 
          //ostr(0, 11, "\n -->set_Axon_Count_R"); cout << " " << p_Axon_Count_R << " NID " << NID.U;
          Axon_Count_R = p_Axon_Count_R; 
     }
     void set_Axon_Block_R(unsigned long long int p_Axon_Block_R)
     { 
          //ostr(0, 11, "\n -->set_Axon_Block_R"); cout << " " << p_Axon_Block_R << " NID " << NID.U;
          Axon_Block_R.U = p_Axon_Block_R; 
     }
     
     //Wipes the data from tmp_Data;
     void wipe_Data()
     {
          for (int cou_Index=0;cou_Index<128;cou_Index++)
          {
               tmp_Data[cou_Index] = char(0);
          }
     }
     
     void reset()
     {
          
          //Whether or not the node is a state node.
          Type = 1;
          
          //Axons and their counters.
          Axon_Count_L = 0;
          
          Axon_Count_R = 0;
          
          Axon_Block_R.U = 0;
          Axon_Block_L.U = 0;
          
          //Dendrites.
          Dendrite_L.U = 0;
          Dendrite_R.U = 0;
     
          //Reinforcement Counters.
          
          RC_Lvl = 0.0;
          
          State.I = 0;
          
          wipe_Data();
     }
     
     //Compiles the data into the data string.
     void compile()
     {
          wipe_Data();
          
          int tmp_Offset = 0;
          
          //*-----cout << "\n\n Compile()";
          
          //Whether or not the node is a state node.
          tmp_Data[0] = Type;
          tmp_Offset += sizeof(Type);
          //*-----cout << "\n\n Type-->" << tmp_Offset;
          
          //Dendrites.
          *reinterpret_cast<long long unsigned int*> (tmp_Data + tmp_Offset) = Dendrite_L.U;
          tmp_Offset += sizeof(Dendrite_L);
          //*-----cout << "\n Dendrite_L-->" << tmp_Offset;
          *reinterpret_cast<unsigned long long int*> (tmp_Data + tmp_Offset) = Dendrite_R.U;
          tmp_Offset += sizeof(Dendrite_R);
          //*-----cout << "\n Dendrite_R-->" << tmp_Offset;
     
          //Axons and their counters.
          *reinterpret_cast<unsigned long long int*> (tmp_Data + tmp_Offset) = Axon_Count_L;
          tmp_Offset += sizeof(Axon_Count_L);
          //*-----cout << "\n Axon_Count_L-->" << tmp_Offset;
          
          *reinterpret_cast<unsigned long long int*> (tmp_Data + tmp_Offset) = Axon_Count_R;
          tmp_Offset += sizeof(Axon_Count_R);
          //*-----cout << "\n Axon_Count_R-->" << tmp_Offset;
     
          //Axons and their counters.
          *reinterpret_cast<unsigned long long int*> (tmp_Data + tmp_Offset) = Axon_Block_L.U;
          tmp_Offset += sizeof(Axon_Block_L);
          //*-----cout << "\n Axon_Block_L-->" << tmp_Offset;
          
          *reinterpret_cast<unsigned long long int*> (tmp_Data + tmp_Offset) = Axon_Block_R.U;
          tmp_Offset += sizeof(Axon_Block_R);
          //*-----cout << "\n Axon_Block_R-->" << tmp_Offset;
          
          //Reinforcement Counters.
          *reinterpret_cast<double*> (tmp_Data + tmp_Offset) = RC_Lvl;
          tmp_Offset += sizeof(RC_Lvl);
          //*-----cout << "\n RC_Lvl-->" << tmp_Offset;
          
          //The Tier.
          *reinterpret_cast<long int*> (tmp_Data + tmp_Offset) = Tier;
          tmp_Offset += sizeof(Tier);
          //*-----cout << "\n Tier-->" << tmp_Offset << " Tier: " << Tier;
          
          //The Index the Tier Falls Upon.
          *reinterpret_cast<long int*> (tmp_Data + tmp_Offset) = Tier_Index;
          tmp_Offset += sizeof(Tier_Index);
          //*-----cout << "\n Tier_Index-->" << tmp_Offset << " Tier_Index: " << Tier_Index;
          
          *reinterpret_cast<unsigned long long int*> (tmp_Data + 110) = State.U;
     }
     
     //Extracts the data from the string.
     void extract()
     {
          //*-----cout << "\n\n extract()";
          
          int tmp_Offset = 0;
          
          //Whether or not the node is a state node.
          Type = tmp_Data[0];
          tmp_Offset += sizeof(Type);
          //*-----cout << "\n\n Type-->" << tmp_Offset;
          
          //Dendrites.
          Dendrite_L.U = *reinterpret_cast<long long unsigned int*> (tmp_Data + tmp_Offset);
          tmp_Offset += sizeof(Dendrite_L);
          //*-----cout << "\n Dendrite_L-->" << tmp_Offset << " Dendrite_L.U: " << Dendrite_L.U;
          Dendrite_R.U = *reinterpret_cast<long long unsigned int*> (tmp_Data + tmp_Offset);
          tmp_Offset += sizeof(Dendrite_R);
          //*-----cout << "\n Dendrite_R-->" << tmp_Offset << " Dendrite_R.U: " << Dendrite_R.U;
          
          //Axons and their counters.
          Axon_Count_L = long(*reinterpret_cast<unsigned long long int*> (tmp_Data + tmp_Offset));
          tmp_Offset += sizeof(Axon_Count_L);
          //*-----cout << "\n Axon_Count_L-->" << tmp_Offset << " Axon_Count_L: " << Axon_Count_L;
          
          Axon_Count_R = *reinterpret_cast<unsigned long long int*>(tmp_Data + tmp_Offset);
          tmp_Offset += sizeof(Axon_Count_R);
          //*-----cout << "\n Axon_Count_R-->" << tmp_Offset << " Axon_Count_R: " << Axon_Count_R;
          
          //Axons and their counters.
          Axon_Block_L.U = *reinterpret_cast<unsigned long long int*> (tmp_Data + tmp_Offset);
          tmp_Offset += sizeof(Axon_Block_L);
          //*-----cout << "\n Axon_Block_L-->" << tmp_Offset << " Axon_Block_L: " << Axon_Block_L.U;
          
          Axon_Block_R.U = *reinterpret_cast<unsigned long long int*>(tmp_Data + tmp_Offset);
          tmp_Offset += sizeof(Axon_Block_R);
          //*-----cout << "\n Axon_Block_R-->" << tmp_Offset << " Axon_Block_R.U: " << Axon_Block_R.U;
          
          //Reinforcement Counters.
          RC_Lvl = *reinterpret_cast<double*> (tmp_Data + tmp_Offset);
          tmp_Offset += sizeof(RC_Lvl);
          //*-----cout << "\n RC_Lvl-->" << tmp_Offset << " RC_Lvl: " << RC_Lvl;
          
          //The Tier.
          Tier = *reinterpret_cast<long int*> (tmp_Data + tmp_Offset);
          tmp_Offset += sizeof(Tier);
          //*-----cout << "\n Tier-->" << tmp_Offset << " Tier: " << Tier;
          
          //The Index the Tier Falls Upon.
          Tier_Index = *reinterpret_cast<long int*> (tmp_Data + tmp_Offset);
          tmp_Offset += sizeof(Tier_Index);
          //*-----cout << "\n Tier_Index-->" << tmp_Offset << " Tier_Index: " << Tier_Index;
          
          State.U = *reinterpret_cast<unsigned long long int*> (tmp_Data + 110);
     }
     
     //Outputs a node.
     void output_Condensed()
     {
          //Whether or not the node is a state node.
          cout << "\n Type-->" << Type;
          
          //Axons and their counters.
          cout << " A_Cou_L-->" << Axon_Count_L;
          cout << " A_Cou_R-->" << Axon_Count_R;
          
          //Axons and their counters.
          cout << " A_Blo_L-->" << Axon_Block_L.U;
          cout << " A_Blo_R-->" << Axon_Block_R.U;
          
          //Dendrites.
          cout << " D_L-->" << Dendrite_L.U;
          cout << " D_R-->" << Dendrite_R.U;
     
          //Reinforcement Counters.
          cout << " RC_Lvl-->" << RC_Lvl;
          
          cout << " State-->" << char(State.U);
     }
     
     //Outputs a node.
     void output()
     {
          //Whether or not the node is a state node.
          cout << "\n\n Type-->" << Type;
          
          //Axons and their counters.
          cout << "\n Axon_Count_L-->" << Axon_Count_L;
          cout << "\n Axon_Count_R-->" << Axon_Count_R;
          
          //Dendrites.
          cout << "\n Dendrite_L-->" << Dendrite_L.U;
          cout << "\n Dendrite_R-->" << Dendrite_R.U;
     
          //Reinforcement Counters.
          cout << "\n RC_Lvl-->" << RC_Lvl;
          
          cout << "\n State-->" << char(State.U);
          
          cout << "\n tmp_Data::int-->\n";
          for (int cou_Index=0;cou_Index<128;cou_Index++)
          {
               if (!(cou_Index % 16)){ cout << "\n"; }
               cout << "." << int(tmp_Data[cou_Index]);
          }
          cout << "\n tmp_Data::char-->\n";
          for (int cou_Index=0;cou_Index<128;cou_Index++)
          {
               if (!(cou_Index % 16)){ cout << "\n"; }
               cout << "." << tmp_Data[cou_Index];
          }
     }
     
     //Outputs a node.
     void output_HTML(ofstream * p_File)
     {
          
          //Whether or not the node is a state node.
          //cout << "\n Type-->" << Type;
          
          //Axons and their counters.
          //cout << " A_Cou_L-->" << Axon_Count_L;
          //cout << " A_Cou_R-->" << Axon_Count_R;
          
          //Axons and their counters.
          //cout << " A_Blo_L-->" << Axon_Block_L.U;
          //cout << " A_Blo_R-->" << Axon_Block_R.U;
          
          //Dendrites.
          cout << " D_L-->" << Dendrite_L.U;
          cout << " D_R-->" << Dendrite_R.U;
          
          //Get Coords of dendrites to link them.
          
     
          //Reinforcement Counters.
          cout << " RC_Lvl-->" << RC_Lvl;
          
          cout << " State-->" << char(State.U);
          
          cout << " Tier-->" << char(State.U);
          
          cout << " Tier_Index-->" << char(State.U);
          //ctx.strokeStyle="#00FF00";

          //ctx.beginPath();
          //ctx.arc(100,100,5,0,2*Math.PI);
          //ctx.stroke();

          //ctx.moveTo(100, 100);
          //ctx.lineTo(200, 200);
          //ctx.stroke();
     }
     
     
};


class c_NT3_NodeDB
{
private:
     
public:
     
     //The databse header.
     c_NT3_NodeDB_Header NodeDB_Header;
     
     //The mapping tool for the database.
     c_MMap MMap;
     
     //The mapping tool for the axon database.
     c_MMap Axons_L;
     c_MMap Axons_R;
     
     //The filename of the construct to load.
     string p_Filename;

	 //The log file.
	 ofstream LOGLOG;
     
     //Size in bytes of the block.
     int block_Size;
     
     //flag: initialized
     bool flg_Initialized;
     
     //The current number of nodes, used in creation as well.
     u_Data_3 CNID;
     
     //The tier the node is currently on.
     int Tier;
     
     //Holds the information for the number of nodes on each tier.
     long int Tier_Depth[32768];
     long int Tier_Current_Depth;
     
     //The current block each axon file is on. 
     unsigned long long int Axon_Current_Block_L;
     unsigned long long int Axon_Current_Block_R;
     
     //Since everything is stored in chunks of 128B the max size is ((current_Size * 1024) * 1024) / 128)
     unsigned long long int Nodes_Current_Max_Node_Count;
     unsigned long long int Axon_Current_Max_Block_Count_L;
     unsigned long long int Axon_Current_Max_Block_Count_R;
     
     //The temporary string to stuff the 128 bytes of info into.
     string current_Node_Data;
     
     //The node to temporarily hold all the data for the node.
     c_NT3_NodeDB_Node current_Node;
     
     //Holds the list of axons retrieved.
     u_Data_3 * Axon_List_L;
     u_Data_3 * Axon_List_R;
     
     //Current number of cells allocated for tmp_Axon_List
     unsigned long int Axon_List_Allocated_Count_L;
     unsigned long int Axon_List_Allocated_Count_R;
     
     //Current number of cells in use by tmp_Axon_List
     unsigned long int Axon_List_Count_L;
     unsigned long int Axon_List_Count_R;
     
     //THe current axon block data.
     char current_Axon_Block_Data_L[128];
     char current_Axon_Block_Data_R[128];
     
     
     
     //-- NOTE: CNID CANNOT BE 0, 0 IS RESERVED FOR NULL
     c_NT3_NodeDB()
     {
          block_Size = 512;
          p_Filename = "NONE";
          flg_Initialized = 0;
          CNID.U = 1;
          Tier = 0;
          current_Node_Data = "";
          Axon_Current_Block_L = 0;
          Axon_Current_Block_R = 0;
          Axon_List_L = NULL;
          Axon_List_Allocated_Count_L = 0;
          Axon_List_Count_L = 0;
          Axon_List_R = NULL;
          Axon_List_Allocated_Count_R = 0;
          Axon_List_Count_R = 0;
          
          Nodes_Current_Max_Node_Count = 0;
          Axon_Current_Max_Block_Count_L = 0;
          Axon_Current_Max_Block_Count_R = 0;
          
          
          
          for (int cou_Index=0;cou_Index<128;cou_Index++)
          {
               current_Axon_Block_Data_L[cou_Index] = 0;
               current_Axon_Block_Data_R[cou_Index] = 0;
          }
          
          Tier_Current_Depth = 0;
          for (int cou_Index=0;cou_Index<32768;cou_Index++)
          {
               Tier_Depth[cou_Index] = 0;
          }
     }
     
     ~c_NT3_NodeDB()
     {
          delete_Axon_Lists();
		  LOGLOG.close();
     }
     
     void delete_Axon_Lists()
     {
          if (Axon_List_L != NULL){ delete Axon_List_L; Axon_List_L = NULL; }
          if (Axon_List_R != NULL){ delete Axon_List_R; Axon_List_R = NULL; }
          Axon_List_Allocated_Count_L = 0;
          Axon_List_Count_L = 0;
          Axon_List_Allocated_Count_R = 0;
          Axon_List_Count_R = 0;
     }
     
     //Sets the files up for a new network.
     int new_Network(string p_Dir, string p_Filename)
     {
          NodeDB_Header.new_File(p_Dir, p_Filename);
          
          CNID.U = NodeDB_Header.get_CNID();
          Axon_Current_Block_L = NodeDB_Header.Axon_Current_Block_L;
          Axon_Current_Block_R = NodeDB_Header.Axon_Current_Block_R;
          
          //After figuring out the half available memory mapping allocation problem remove the /2
          Nodes_Current_Max_Node_Count = ((((NodeDB_Header.Node_File_Size * 1024) * 1024) / 128)) - 1;
          Axon_Current_Max_Block_Count_L = ((((NodeDB_Header.Axon_File_Size_L * 1024) * 1024) / 128)) - 1;
          Axon_Current_Max_Block_Count_R = ((((NodeDB_Header.Axon_File_Size_R * 1024) * 1024) / 128)) - 1;
          
          ostr(0, 7, "\n Max_Node_Count: "); cout << Nodes_Current_Max_Node_Count;
          ostr(0, 7, "\n Max_Axon_L_Count: "); cout << Axon_Current_Max_Block_Count_L;
          ostr(0, 7, "\n Max_Axon_R_Count: "); cout << Axon_Current_Max_Block_Count_R;
          
          string p_FName = p_Dir + "/" + p_Filename;
          flg_Initialized = MMap.open_File((p_FName + "_Nodes.dat"), NodeDB_Header.Node_File_Size);
          flg_Initialized = Axons_L.open_File((p_FName + "_Axons_L.dat"), NodeDB_Header.Axon_File_Size_L);
          flg_Initialized = Axons_R.open_File((p_FName + "_Axons_R.dat"), NodeDB_Header.Axon_File_Size_R);

		  string tmp_LOGName = p_Dir + "/" + p_Filename + ".Log.dat";
		  LOGLOG.open(tmp_LOGName, ios::app);

          return flg_Initialized;
     }
     
     int load_Network(string p_Dir, string p_Filename="NULL")
     {
          if (p_Filename == "NULL"){ return 0; }
          
          NodeDB_Header.load_File(p_Dir, p_Filename);
          
          CNID.U = NodeDB_Header.get_CNID();
          Axon_Current_Block_L = NodeDB_Header.Axon_Current_Block_L;
          Axon_Current_Block_R = NodeDB_Header.Axon_Current_Block_R;
          
          //After figuring out the half available memory mapping allocation problem remove the /2
          Nodes_Current_Max_Node_Count = ((((NodeDB_Header.Node_File_Size * 1024) * 1024) / 128)) - 1;
          Axon_Current_Max_Block_Count_L = ((((NodeDB_Header.Axon_File_Size_L * 1024) * 1024) / 128)) - 1;
          Axon_Current_Max_Block_Count_R = ((((NodeDB_Header.Axon_File_Size_R * 1024) * 1024) / 128)) - 1;
          
          ostr(0, 7, "\n Max_Node_Count: "); cout << Nodes_Current_Max_Node_Count;
          ostr(0, 7, "\n Max_Axon_L_Count: "); cout << Axon_Current_Max_Block_Count_L;
          ostr(0, 7, "\n Max_Axon_R_Count: "); cout << Axon_Current_Max_Block_Count_R;
          
          string p_FName = p_Dir + "/" + p_Filename;
          flg_Initialized = MMap.open_File((p_FName + "_Nodes.dat"), NodeDB_Header.Node_File_Size);
          flg_Initialized = Axons_L.open_File((p_FName + "_Axons_L.dat"), NodeDB_Header.Axon_File_Size_L);
          flg_Initialized = Axons_R.open_File((p_FName + "_Axons_R.dat"), NodeDB_Header.Axon_File_Size_R);

		  string tmp_LOGName = p_Dir + "/" + p_Filename + ".Log.dat";
		  LOGLOG.open(tmp_LOGName, ios::app);

          return flg_Initialized;
     }
     
     //Saves a network header.
     void save_Network(string p_Dir, string p_Filename="NULL", bool p_Disp_Output = 1)
     {
          if (p_Filename == "NULL"){ p_Filename = NodeDB_Header.Network_Name; }
          
          NodeDB_Header.Node_Count = CNID.U;
          NodeDB_Header.Axon_Current_Block_L = Axon_Current_Block_L;
          NodeDB_Header.Axon_Current_Block_R = Axon_Current_Block_R;
          NodeDB_Header.Tier_Current_Depth = Tier_Current_Depth;
          for (int cou_T=0;cou_T<Tier_Current_Depth;cou_T++)
          {
               NodeDB_Header.Tier_Depth[cou_T] = Tier_Depth[cou_T];
          }
          
          NodeDB_Header.save_File(p_Dir, p_Filename, p_Disp_Output);
     }
     
     //Increases the size of the files when the max counters are reached.
     int expand_Node_File()
     {
          //The new filesize is the int (current * 1.5)
          NodeDB_Header.Node_File_Size = (((unsigned long long int) (NodeDB_Header.Node_File_Size * 1.1)) + 1);
          
          ostr(0, 12, "\n Expanding Node File To "); cout << (NodeDB_Header.Node_File_Size + 1);
          ostr(0, 12, "\n ---Current Nodes_Current_Max_Node_Count "); cout << Nodes_Current_Max_Node_Count;
          
          if (MMap.expand(NodeDB_Header.Node_File_Size))
          {
               Nodes_Current_Max_Node_Count = ((((NodeDB_Header.Node_File_Size * 1024) * 1024) / 128)) - 1;
               ostr(0, 12, "\n -----New Nodes_Current_Max_Node_Count = "); cout << Nodes_Current_Max_Node_Count << "\n";
               return 1;
          }
          else
          {
               return 0;
          }
     }
     
     //Increases the size of the files when the max counters are reached.
     int expand_Axon_L_File()
     {
		  if (LOGLOG.is_open())
		  {
			  LOGLOG << "\n\n Expanding Axon_L File To " << (NodeDB_Header.Axon_File_Size_L + 1);

			  LOGLOG << "\n  CNID " << CNID.U;
			  LOGLOG << "\n  Nodes_Current_Max_Node_Count " << Nodes_Current_Max_Node_Count;

			  LOGLOG << "\n  Axon_Current_Block_L " << Axon_Current_Block_L;
			  LOGLOG << "\n  Axon_Current_Max_Block_Count_L " << Axon_Current_Max_Block_Count_L;

			  LOGLOG << "\n  Axon_Current_Block_R " << Axon_Current_Block_R;
			  LOGLOG << "\n  Axon_Current_Max_Block_Count_R " << Axon_Current_Max_Block_Count_R;
		  }

          //The new filesize is the int (current * 1.5)
          NodeDB_Header.Axon_File_Size_L = (((unsigned long long int) (NodeDB_Header.Axon_File_Size_L * 1.1)) + 1);
          
          ostr(0, 12, "\n Expanding Axon_L File To "); cout << (NodeDB_Header.Axon_File_Size_L + 1);
          ostr(0, 12, "\n ---Current Axon_Current_Max_Block_Count_L "); cout << Axon_Current_Max_Block_Count_L;
          
          if (Axons_L.expand(NodeDB_Header.Axon_File_Size_L))
          {
               Axon_Current_Max_Block_Count_L = ((((NodeDB_Header.Axon_File_Size_L * 1024) * 1024) / 128)) - 1;
               ostr(0, 12, "\n -----New Axon_Current_Max_Block_Count_L = "); cout << Axon_Current_Max_Block_Count_L << "\n";

			   if (LOGLOG.is_open())
			   {
				   LOGLOG << "\n -----New Axon_Current_Max_Block_Count_L = " << Axon_Current_Max_Block_Count_L;
			   }

               return 1;
          }
          else
          {
               return 0;
          }
     }
     
     //Increases the size of the files when the max counters are reached.
     int expand_Axon_R_File()
     {
		 if (LOGLOG.is_open())
		 {
			 LOGLOG << "\n\n Expanding Axon_R File To " << NodeDB_Header.Axon_File_Size_R + 1;

			 LOGLOG << "\n  CNID " << CNID.U;
			 LOGLOG << "\n  Nodes_Current_Max_Node_Count " << Nodes_Current_Max_Node_Count;

			 LOGLOG << "\n  Axon_Current_Block_L " << Axon_Current_Block_L;
			 LOGLOG << "\n  Axon_Current_Max_Block_Count_L " << Axon_Current_Max_Block_Count_L;

			 LOGLOG << "\n  Axon_Current_Block_R " << Axon_Current_Block_R;
			 LOGLOG << "\n  Axon_Current_Max_Block_Count_R " << Axon_Current_Max_Block_Count_R;
		 }

          //The new filesize is the int (current * 1.5)
          NodeDB_Header.Axon_File_Size_R = (((unsigned long long int) (NodeDB_Header.Axon_File_Size_R * 1.1)) + 1);
          
          ostr(0, 12, "\n Expanding Axon_R File To "); cout << (NodeDB_Header.Axon_File_Size_R + 1);
          ostr(0, 12, "\n ---Current Axon_Current_Max_Block_Count_R "); cout << Axon_Current_Max_Block_Count_R;
          
          if (Axons_R.expand(NodeDB_Header.Axon_File_Size_R))
          {
               Axon_Current_Max_Block_Count_R = ((((NodeDB_Header.Axon_File_Size_R * 1024) * 1024) / 128)) - 1;
               ostr(0, 12, "\n -----New Axon_Current_Max_Block_Count_R = "); cout << Axon_Current_Max_Block_Count_R << "\n";

			   if (LOGLOG.is_open())
			   {
				   LOGLOG << "\n -----New Axon_Current_Max_Block_Count_R = " << Axon_Current_Max_Block_Count_R;
			   }

               return 1;
          }
          else
          {
               return 0;
          }
     }
          
          
          
     
     //======--                --======//
     //==--     NODE FUNCTIONS     --==//
     //======--                --======//
     
     //Creates a new node using the CNID to determine the position.
     unsigned long long int new_Node(int p_Tier, bool p_Save_YN = 1)
     {
          //*-----ostr(0, 13, "\n new_Node "); cout << CNID.U << " ";
          
          
          if ((CNID.U + 1) >= Nodes_Current_Max_Node_Count)
          {
               expand_Node_File();
          }
          
          if ((Axon_Current_Block_L + 1) >= Axon_Current_Max_Block_Count_L)
          {
               expand_Axon_L_File();
          }
          if ((Axon_Current_Block_R + 1) >= Axon_Current_Max_Block_Count_R)
          {
               expand_Axon_R_File();
          }
          
          //Goto the current CNID.
          //-- NOTE: CNID CANNOT BE 0, 0 IS RESERVED FOR NULL
          goto_Node_No_Load(CNID.U);
          CNID.U++;
          
          
          current_Node.reset();
          
          //Assign the node an axon block.
          current_Node.set_Axon_Block_L(Axon_Current_Block_L);
          Axon_Current_Block_L++;
          current_Node.set_Axon_Block_R(Axon_Current_Block_R);
          Axon_Current_Block_R++;
          
          //Set the current RC level to 1.
          current_Node.RC_Lvl = 1;
          
          current_Node.Tier = p_Tier;
          current_Node.Tier_Index = Tier_Depth[p_Tier];
          Tier_Depth[p_Tier]++;
          if (Tier_Current_Depth <= p_Tier){ Tier_Current_Depth = p_Tier + 1; }
          
          if (p_Save_YN){ current_Node.compile(); save_Node(); }
          
          //current_Node_Data
          
          //*-----cout << " node creation complete...";
          //system("PAUSE > NULL");
          //Faulture
          return CNID.U - 1;
     }
          
     //Creates a new node using the CNID to determine the position.
     unsigned long long int new_State_Node(u_Data_3 p_State)
     {
          //Create a new node
          new_Node(0, 0);
          
          //Change it to a state node and set the state.
          current_Node.Type = 0;
          current_Node.State = p_State;
          
          current_Node.compile();
          save_Node();
          
          //Change the type to state
          
          //Inputs the state, stored in the 128-sizeof(u_Data_3) position in the node data
          
          //Faulture
          return CNID.U - 1;
     }
     
     //The axon files function separately. This allows for loading axons of one node, and then checking them while loading different nodes to check their dendrites.
     unsigned long long int does_Upper_Tier_Connection_Exist(unsigned long long int p_L, unsigned long long int p_R)
     {
          //*-----ostr(0, 14, "\n\n does_Upper_Tier_Connection_Exist( p_L.U ");
          //*-----cout << p_L;
          //*-----ostr(0, 14, ", p_R.U ");
          //*-----cout << p_R;
          //*-----ostr(0, 14, ");");
          
          //Gather the axons from the left node
          goto_Node(p_L);
          
          //Get the axon count.
          unsigned long long int tmp_Axon_Count = current_Node.get_Axon_Count_L();
          
          //If no axons return.
          //*-----cout << "\n Axon Count:" << tmp_Axon_Count;
          if (tmp_Axon_Count == 0){ return 0; }
          
          //*-----ostr(0, 15, "\n Gathering Axons..."); cout << tmp_Axon_Count;
          
          goto_Axon_Block_L(current_Node.get_Axon_Block_L(), tmp_Axon_Count);
          
          //Check all of the axons for a lower right dendrite matching p_R
          for (unsigned long long int cou_A=0;cou_A<tmp_Axon_Count;cou_A++)
          {
               
               goto_Node(Axon_List_L[cou_A].U);
               //*-----ostr(0, 15, "\n    Testing Axon "); cout << "Axon_List[" << cou_A << "].U " << Axon_List_L[cou_A].U;
               //*-----ostr(0, 12, "     current_Node.Dendrite_R.U = "); cout << current_Node.Dendrite_R.U;
               //if (current_Node.Dendrite_R.U == p_R.U){ cout << " connection found"; return Axon_List_L[cou_A].U; }
               if (current_Node.Dendrite_R.U == p_R){ return Axon_List_L[cou_A].U; }
          }
          
          //*-----cout << " no connection found";
          //Faulture
          return 0;
     }
     
     unsigned long long int get_Upper_Tier_Connection(int p_Tier, unsigned long long int p_L, unsigned long long int p_R)
     {
          //*-----ostr(0, 9, "\n\n\n get_Upper_Tier_Connection(");
          //*-----oint(0, 14, p_L);
          //*-----cout << ", ";
          //*-----oint(0, 14, p_R);
          //*-----ostr(0, 15, ")");
          
          //If either submitted node is NULL then return NULL.
          if (p_L == 0 || p_R == 0){ return 0; }
          
          //A tmp var to hold the upper tier node for returning.
          u_Data_3 tmp_NID;
          tmp_NID.U = 0;
          
          //Check for an upper tier connection already, if one exists then return it.
          tmp_NID.U = does_Upper_Tier_Connection_Exist(p_L, p_R);
          
          if (tmp_NID.U != 0)
          {
               return tmp_NID.U;
          }
          
          //If one does not exist then create it.
          tmp_NID.U = new_Node((p_Tier + 1), 0);
          
          //goto_Node_No_Load(tmp_NID.U);
          
          current_Node.Dendrite_L.U = p_L;
          current_Node.Dendrite_R.U = p_R;
          
          //current_Node.compile();
          save_Node();
          
          add_Axon_L(p_L, tmp_NID.U);
          add_Axon_R(p_R, tmp_NID.U);
          //*-----cout << "\n  finished gathering_Upper_Tier_Connection()"; cout.flush();
          return tmp_NID.U;
     }
     
     //Creates a connection between two nodes, used because without it only lower nodes chosen by CAN will be appropriately set.
     void create_Connection(unsigned long long int  p_From, unsigned long long int  p_To)
     {
          
     }
     
     //Used for linking nodes together.
     int add_Axon_L(unsigned long long int  p_From, unsigned long long int  p_To)
     {
          //*-----ostr(0, 7, "\n add_Axon_L("); cout << p_From << ", " << p_To << ")";
          goto_Node(p_From);
          
          //Goes to the axon block that holds the index wanted.
          goto_Axon_Block_And_Set_Index_L(current_Node.get_Axon_Block_L(), p_To);
          
          
          save_Node();
          
          return 1;
     }
     
     int add_Axon_R(unsigned long long int  p_From, unsigned long long int  p_To)
     {          
          //*-----ostr(0, 7, "\n add_Axon_R("); cout << p_From << ", " << p_To << ")";
          goto_Node(p_From);
          
          //Goes to the axon block that holds the index wanted.
          goto_Axon_Block_And_Set_Index_R(current_Node.get_Axon_Block_R(), p_To);
          
          save_Node();
          
          return 1;
     }

     
     
     //Reinforces a node.
     void reinforce(unsigned long long int p_Node)
     {
          goto_Node(p_Node);
          
          current_Node.extract();
          current_Node.RC_Lvl += double (1.0f / int (current_Node.RC_Lvl));
          save_Node();
          
          //Faulture
          return;
     }
     
     void goto_Node(unsigned long long int p_Node)
     {
          //*-----ostr(0, 7, "\ngoto_Node("); cout << p_Node << ")";
          current_Node.reset();
          MMap.create_Mapping_View(p_Node * 128);
          MMap.get_Data(current_Node.tmp_Data);
          current_Node.extract();
          current_Node.NID.U = p_Node;
          //*-----current_Node.output_Condensed();
     }
     
     //Goes to a node but does not load it, used for creating new nodes.
     void goto_Node_No_Load(unsigned long long int p_Node)
     {
          //ostr(0, 7, "\ngoto_Node_No_Load("); cout << p_Node << ")";
          MMap.create_Mapping_View(p_Node * 128);
          current_Node.NID.U = p_Node;
     }
     
     //Resizes the axon block. Only called from goto_Axon_Block(), so no bounds checking is done.
     //This does not save the data in the list.
     //It is assumed that it will be a fresh list so the count starts at 0.
     void resize_Axon_List_L(unsigned long long int p_Size)
     {
          if (Axon_List_L != NULL){ delete Axon_List_L; Axon_List_L = NULL; }
          
          Axon_List_L = new u_Data_3[p_Size];
          
          Axon_List_Allocated_Count_L = p_Size;
          Axon_List_Count_L = 0;
     }
     
     void resize_Axon_List_R(unsigned long int p_Size)
     {
          if (Axon_List_R != NULL){ delete Axon_List_R; Axon_List_R = NULL; }
          
          Axon_List_R = new u_Data_3[p_Size];
          
          Axon_List_Allocated_Count_R = p_Size;
          Axon_List_Count_R = 0;
     }
     
     //Goes to axon chunk. Used for checking axonic connections in does_Upper_Tier_Connection_Exist().
     void goto_Axon_Block_L(unsigned long long int p_Axon_Block, unsigned long long int p_Axon_Count)
     {
          //*-----ostr(0, 7, "\n\n goto_Axon_Block_L(p_Axon_Block "); cout << p_Axon_Block; ostr(0, 7, ", p_Axon_Count "); cout << p_Axon_Count << ")";
          
          //Go to the starting block.
          Axons_L.create_Mapping_View(p_Axon_Block * 128);
          char tmp_Axons[128];
          Axons_L.get_Data(tmp_Axons);
          int tmp_Offset = 0;
          
          resize_Axon_List_L(p_Axon_Count);
          
          //Track separate from the axon count, used for jumping without messing up the array of datas.
          int tmp_Block_Index = 0;
          
          //This assumes that u_Data_3 is 8 bytes.
          for (int cou_A=0;cou_A<p_Axon_Count;cou_A++)
          {
               //*-----cout << "\n A:" << cou_A;
               
               //Axons and their counters.
               Axon_List_L[cou_A].U = *reinterpret_cast<unsigned long long int*> (tmp_Axons + tmp_Offset);
               //*-----cout << "     --offset>" << tmp_Offset; cout << " " << (cou_A / 8);
               
               //Check to see if it is the last index in the block, if so move the axon block too it. 
               if ((tmp_Block_Index) == 15)
               {
                    //*-----cout << " --load_Next_Block>"; 
                    
                    //*-----cout << " cou_A:" << cou_A;
                    //*-----ostr(0, 13, " Datas jmp data:");
                    //*-----cout << Axon_List_L[cou_A].U;
                    
                    //Reset the offset to avoid bounding errors.
                    tmp_Offset = 0;
                    
                    //Move the mapping view to the next block. Do not change cou_A, we want it to track it as if nothing had happened.
                    //*-----cout << " Axons_L.create_Mapping_View(" << Axon_List_L[cou_A].U << " * 128);";
                    Axons_L.create_Mapping_View(Axon_List_L[cou_A].U * 128);
                    Axons_L.get_Data(tmp_Axons);
                    
                    //*-----cout << Axon_List_L[cou_A].U;
                    
                    //Now that we have the new offset and tmp_Axons data set we can extract the current axons NID.
                    Axon_List_L[cou_A].U = *reinterpret_cast<unsigned long long int*> (tmp_Axons + tmp_Offset);
                    
                    tmp_Block_Index = 0;
                    cou_A--;
                    continue;
               }
               
               //*-----cout << " cou_A:" << cou_A;
               //*-----ostr(0, 12, " Datas:");
               //*-----cout << Axon_List_L[cou_A].U;
               //*-----cout << " block_Index:" << tmp_Block_Index;
               tmp_Offset += 8;
               tmp_Block_Index++;
          }
          //*-----cout << "\n Axon_Block_L_Visited"; cout.flush();
     }
     
     //Goes to axon chunk. Used for checking axonic connections in does_Upper_Tier_Connection_Exist().
     void goto_Axon_Block_R(unsigned long long int p_Axon_Block, long int p_Axon_Count)
     {
          //*-----ostr(0, 7, "\n\n goto_Axon_Block_L(p_Axon_Block "); cout << p_Axon_Block; ostr(0, 7, ", p_Axon_Count "); cout << p_Axon_Count << ")";
          
          //Go to the starting block.
          Axons_R.create_Mapping_View(p_Axon_Block * 128);
          char tmp_Axons[128];
          Axons_R.get_Data(tmp_Axons);
          int tmp_Offset = 0;
          
          resize_Axon_List_R(p_Axon_Count);
          
          //Track separate from the axon count, used for jumping without messing up the array of datas.
          int tmp_Block_Index = 0;
          
          //This assumes that u_Data_3 is 8 bytes.
          for (int cou_A=0;cou_A<p_Axon_Count;cou_A++)
          {
               //*-----cout << "\n A:" << cou_A;
               
               //Axons and their counters.
               Axon_List_R[cou_A].U = *reinterpret_cast<unsigned long long int*> (tmp_Axons + tmp_Offset);
               //*-----cout << "     --offset>" << tmp_Offset; cout << " " << (cou_A / 8);
               
               //Check to see if it is the last index in the block, if so move the axon block too it. 
               if ((tmp_Block_Index) == 15)
               {
                    //*-----cout << " --load_Next_Block>"; 
                    
                    //*-----cout << " cou_A:" << cou_A;
                    //*-----ostr(0, 13, " Datas jmp data:");
                    //*-----cout << Axon_List_R[cou_A].U;
                    
                    //Reset the offset to avoid bounding errors.
                    tmp_Offset = 0;
                    
                    //Move the mapping view to the next block. Do not change cou_A, we want it to track it as if nothing had happened.
                    //*-----cout << " Axons_R.create_Mapping_View(" << Axon_List_R[cou_A].U << " * 128);";
                    Axons_R.create_Mapping_View(Axon_List_R[cou_A].U * 128);
                    Axons_R.get_Data(tmp_Axons);
                    
                    //*-----cout << Axon_List_R[cou_A].U;
                    
                    //Now that we have the new offset and tmp_Axons data set we can extract the current axons NID.
                    Axon_List_R[cou_A].U = *reinterpret_cast<unsigned long long int*> (tmp_Axons + tmp_Offset);
                    
                    tmp_Block_Index = 0;
                    cou_A--;
                    continue;
               }
               
               //*-----cout << " cou_A:" << cou_A;
               //*-----ostr(0, 12, " Datas:");
               //*-----cout << Axon_List_R[cou_A].U;
               //*-----cout << " block_Index:" << tmp_Block_Index;
               
               tmp_Offset += 8;
               tmp_Block_Index++;
          }
          //*-----cout << "\n Axon_Block_R_Visited"; cout.flush();
     }
     
     
     //Goes to axon chunk. Used for checking axonic connections in does_Upper_Tier_Connection_Exist().
     void goto_Axon_Block_And_Set_Index_L(unsigned long long int p_Axon_Block, unsigned long long int p_Axon)
     {
          //*-----ostr(0, 11, "\n\n goto_Axon_Block_And_Set_Index_L(p_Axon_Block "); cout << p_Axon_Block << ", p_Axon " << p_Axon << ")";
          
          unsigned long long int tmp_Current_Position = p_Axon_Block;
          
          //Go to the starting block.
          Axons_L.create_Mapping_View(p_Axon_Block * 128);
          char tmp_Axons[128];
          Axons_L.get_Data(tmp_Axons);
          
          const unsigned int Nodes_Per_Block = 15;
          //Going with 8 bytes per node.
          const unsigned int Bytes_Per_Node = 8;
          
          //Doing a (-1) on the axon count because we need the index ! the count. 
          //(Deprecated)NOTE: Now doing the (-1) after we check for a positive number, otherwise
          //it subtracts 1 from 0 resulting in a high number due to unsigned int.
          //NOTE: Now the (-1) will not be done, the axon count represents the current
          //axon count, meaning the new axon will be placed in that index.
          unsigned long long int tmp_Count = current_Node.get_Axon_Count_L();
          unsigned long long int tmp_Blocks_To_Skip = 0;
          unsigned long long int tmp_Index = 0;
          if (tmp_Count > 0)
          {
               //tmp_Count--;
               tmp_Blocks_To_Skip = int (tmp_Count / Nodes_Per_Block);
               tmp_Index = tmp_Count - (tmp_Blocks_To_Skip * Nodes_Per_Block);
          }
          
          //*-----ostr(0, 11, "\n Current_NID = "); cout <<  current_Node.NID.U;
          //*-----ostr(0, 11, "\n tmp_Count = "); cout <<  tmp_Count << ";";
          //*-----ostr(0, 11, "\n tmp_Blocks_To_Skip =  "); cout << tmp_Blocks_To_Skip;
          //*-----ostr(0, 11, "\n tmp_Index = "); cout << tmp_Index;
          
          //Loop through the blocks until we get the one we need.
          //Iterate to (Blocks_To_Skip - 1) because this gatheres and jumps to the next block. So going until the 
          //number of blocks to skip has been reached would actually get (Blocks_To_Skip + 1).
          for (unsigned int cou_Block=0;cou_Block<tmp_Blocks_To_Skip;cou_Block++)
          {
               //*-----cout << "\n cou_Block->" << cou_Block;
               
               //Check to see if the end of the block has been reached, if so then assign the next block.
               if (((tmp_Blocks_To_Skip - cou_Block) == 1) && (tmp_Index == 0))
               {
                    *reinterpret_cast<unsigned long long int*> (tmp_Axons + ((Nodes_Per_Block) * Bytes_Per_Node)) = Axon_Current_Block_L;
                    //*-----ostr(0, 9, "\n     *reinterpret_cast<unsigned long long int*> (tmp_Axons + (Nodes_Per_Block * Bytes_Per_Node)) = Axon_Current_Block_L;");
                    //*-----cout << "\n     *reinterpret_cast<unsigned long long int*> (tmp_Axons + " << ((Nodes_Per_Block) * Bytes_Per_Node) << ") = " << Axon_Current_Block_L << ";";
                    Axon_Current_Block_L++;
                    Axons_L.set_Data(tmp_Axons, 128);
                    Axons_L.save_Data();
                    tmp_Current_Position = Axon_Current_Block_L - 1;
                    Axons_L.create_Mapping_View(tmp_Current_Position * 128);
                    Axons_L.get_Data(tmp_Axons);
                    break;
                    //Now go to the new block and set the 
               }
               
               //Goto the current block and retrieve the end position.
               Axons_L.create_Mapping_View(tmp_Current_Position * 128);
               
               //Get teh data from the current block.
               Axons_L.get_Data(tmp_Axons);
               
               //Gather the last index.
               tmp_Current_Position = *reinterpret_cast<unsigned long long int*> (tmp_Axons + ((Nodes_Per_Block) * Bytes_Per_Node));
               
               //*-----cout << "\n    tmp_Current_Position->" << tmp_Current_Position;
               
               Axons_L.create_Mapping_View(tmp_Current_Position * 128);
               Axons_L.get_Data(tmp_Axons);
               
               //We now either iterate again to follow the blockchain, or we continue.
               
          }
          
          //If we are at this point we should have the block with the index we want.
          *reinterpret_cast<unsigned long long int*> (tmp_Axons + (tmp_Index * Bytes_Per_Node)) = p_Axon;
          //*-----ostr(0, 12, "\n   *reinterpret_cast<unsigned long long int*> (tmp_Axons + (tmp_Index * Bytes_Per_Node)) = p_Axon;");
          //*-----cout << "\n     *reinterpret_cast<unsigned long long int*> (tmp_Axons +  " << (tmp_Index * Bytes_Per_Node) << ") = " << p_Axon;
          
          //*-----output_Datas(current_Node.NID.U, tmp_Axons);
          Axons_L.set_Data(tmp_Axons, 128);
          Axons_L.save_Data();
          current_Node.set_Axon_Count_L(current_Node.get_Axon_Count_L() + 1);
     }
     
     
     //Goes to axon chunk. Used for checking axonic connections in does_Upper_Tier_Connection_Exist().
     void goto_Axon_Block_And_Set_Index_R(unsigned long long int p_Axon_Block, unsigned long long int p_Axon)
     {
          //*-----ostr(0, 11, "\n\n goto_Axon_Block_And_Set_Index_R(unsigned long long int "); cout << p_Axon_Block << ")";
          
          unsigned long long int tmp_Current_Position = p_Axon_Block;
          
          //Go to the starting block.
          Axons_R.create_Mapping_View(p_Axon_Block * 128);
          char tmp_Axons[128];
          Axons_R.get_Data(tmp_Axons);
          
          const unsigned int Nodes_Per_Block = 15;
          const unsigned int Bytes_Per_Node = 8;
          
          //(Deprecated)NOTE: Now doing the (-1) after we check for a positive number, otherwise
          //it subtracts 1 from 0 resulting in a high number due to unsigned int.
          //NOTE: Now the (-1) will not be done, the axon count represents the current
          //axon count, meaning the new axon will be placed in that index.
          unsigned long long int tmp_Count = current_Node.get_Axon_Count_R();
          unsigned long long int tmp_Blocks_To_Skip = 0;
          unsigned long long int tmp_Index = 0;
          if (tmp_Count > 0)
          {
               //tmp_Count--;
               tmp_Blocks_To_Skip = int (tmp_Count / Nodes_Per_Block);
               tmp_Index = tmp_Count - (tmp_Blocks_To_Skip * Nodes_Per_Block);
          }
          
          //*-----ostr(0, 11, "\n Current_NID = "); cout <<  current_Node.NID.U;
          //*-----ostr(0, 11, "\n tmp_Count = "); cout <<  current_Node.get_Axon_Count_R() << ";";
          //*-----ostr(0, 11, "\n tmp_Blocks_To_Skip =  "); cout << tmp_Blocks_To_Skip;
          //*-----ostr(0, 11, "\n tmp_Index = "); cout << tmp_Index;
          
          //Loop through the blocks until we get the one we need.
          //Iterate to (Blocks_To_Skip - 1) because this gatheres and jumps to the next block. So going until the 
          //number of blocks to skip has been reached would actually get (Blocks_To_Skip + 1).
          for (unsigned int cou_Block=0;cou_Block<tmp_Blocks_To_Skip;cou_Block++)
          {
               //*-----cout << "\n cou_Block->" << cou_Block;
               
               //Check to see if the end of the block has been reached, if so then assign the next block.
               if (((tmp_Blocks_To_Skip - cou_Block) == 1) && (tmp_Index == 0))
               {
                    //*reinterpret_cast<unsigned long long int*> (tmp_Axons + ((Nodes_Per_Block) * Bytes_Per_Node)) = Axon_Current_Block_R;
                    *reinterpret_cast<unsigned long long int*> (tmp_Axons + ((Nodes_Per_Block) * Bytes_Per_Node)) = Axon_Current_Block_R;
                    //*-----ostr(0, 9, "\n     *reinterpret_cast<unsigned long long int*> (tmp_Axons + (Nodes_Per_Block * Bytes_Per_Node)) = Axon_Current_Block_R;");
                    //*-----cout << "\n     *reinterpret_cast<unsigned long long int*> (tmp_Axons + " << ((Nodes_Per_Block) * Bytes_Per_Node) << ") = " << Axon_Current_Block_R << ";";
                    Axon_Current_Block_R++;
                    Axons_R.set_Data(tmp_Axons, 128);
                    Axons_R.save_Data();
                    tmp_Current_Position = Axon_Current_Block_R - 1;
                    Axons_R.create_Mapping_View(tmp_Current_Position * 128);
                    Axons_R.get_Data(tmp_Axons);
                    break;
                    //Now go to the new block and set the 
               }
               
               //Goto the current block and retrieve the end position.
               Axons_R.create_Mapping_View(tmp_Current_Position * 128);
               
               //Get teh data from the current block.
               Axons_R.get_Data(tmp_Axons);
               
               //Gather the last index.
               tmp_Current_Position = *reinterpret_cast<unsigned long long int*> (tmp_Axons + ((Nodes_Per_Block) * Bytes_Per_Node));
               
               //*-----cout << "\n    tmp_Current_Position->" << tmp_Current_Position;
               
               Axons_R.create_Mapping_View(tmp_Current_Position * 128);
               Axons_R.get_Data(tmp_Axons);
               
               //We now either iterate again to follow the blockchain, or we continue.
          }
          
          //If we are at this point we should have the block with the index we want.
          *reinterpret_cast<unsigned long long int*> (tmp_Axons + (tmp_Index * Bytes_Per_Node)) = p_Axon;
          
          Axons_R.set_Data(tmp_Axons, 128);
          Axons_R.save_Data();
          current_Node.set_Axon_Count_R(current_Node.get_Axon_Count_R() + 1);
          //*-----output_Datas(current_Node.NID.U, tmp_Axons);
     }
     
     void view_Current_Node()
     {
          MMap.output_View(1);
     }
      
     //Saves the node in the current position.
     void save_Node()
     {
          //ostr(0, 9, "\n   SAVING NODE "); cout << current_Node.NID.U;
          current_Node.compile();
          MMap.set_Data(current_Node.tmp_Data, 128);
          MMap.save_Data();
     }
     
     //Loads the node information into the given node.
     void load_Node(char * p_Node[128])
     {
          
     }
     
     void close()
     {
          MMap.close();
     }
     
     ///==-----------------
     //==--   GETS AND SETS
     ///==-----------------
     
     double get_RC_Score(unsigned long long int p_Node)
     {
          //Faulture
          goto_Node(p_Node);
          return current_Node.RC_Lvl;
     }
     
     //Gets the type of node.
     unsigned long long int get_Type(unsigned long long int p_Node)
     {
          goto_Node(p_Node);
          return current_Node.Type;
     }
     
     //Gets the type of node.
     void set_Type(unsigned long long int p_Node, int p_Data)
     {
          goto_Node(p_Node);
          current_Node.Type = p_Data;
          save_Node();
     }
     
     //Gets the tier of node.
     unsigned long long int get_Tier(unsigned long long int p_Node)
     {
          goto_Node(p_Node);
          return current_Node.Tier;
     }
     
     //Gets the tier of node.
     void set_Tier(unsigned long long int p_Node, int p_Data)
     {
          goto_Node(p_Node);
          current_Node.Tier = p_Data;
          save_Node();
     }
     
     long int get_Axon_Count_L(unsigned long long int p_Node)
     {
          //Faulture
          goto_Node(p_Node);
          return current_Node.get_Axon_Count_L();
     }
     
     long int get_Axon_Count_R(unsigned long long int p_Node)
     {
          //Faulture
          goto_Node(p_Node);
          return current_Node.get_Axon_Count_R();
     }
     
     void get_Axons_L(unsigned long long int p_Node, u_Data_3 * p_Axons)
     {
          //*-----ostr(0, 14, "\n get_Axons_L("); cout << p_Node;
          //Faulture
          goto_Node(p_Node);
          goto_Axon_Block_L(current_Node.get_Axon_Block_L(), current_Node.get_Axon_Count_L());
          //*-----ostr(0, 14, "\n gathering axons "); cout << current_Node.get_Axon_Count_L() << "...";
          for (int cou_Index=0;cou_Index<current_Node.get_Axon_Count_L();cou_Index++)
          {
               p_Axons[cou_Index].U = Axon_List_L[cou_Index].U;
               //*-----cout << "\n     " << cou_Index << ": p_A" << p_Axons[cou_Index].U << " ALL:" << Axon_List_L[cou_Index].U;
          }
          //*-----ostr(0, 14, "\n Done Gathering Axons...");
     }
     
     void get_Axons_R(unsigned long long int p_Node, u_Data_3 * p_Axons)
     {
          //*-----ostr(0, 14, "\n get_Axons_R("); cout << p_Node;
          //Faulture
          goto_Node(p_Node);
          goto_Axon_Block_R(current_Node.get_Axon_Block_R(), current_Node.get_Axon_Count_R());
          //*-----ostr(0, 14, "\n gathering axons "); cout << current_Node.get_Axon_Count_R() << "...";
          for (int cou_Index=0;cou_Index<current_Node.get_Axon_Count_R();cou_Index++)
          {
               p_Axons[cou_Index].U = Axon_List_R[cou_Index].U;
               //*-----cout << "\n     " << cou_Index << ": p_A" << p_Axons[cou_Index].U << " ALR:" << Axon_List_R[cou_Index].U;
          }
          //*-----ostr(0, 14, "\n Done Gathering Axons...");
     }
     
     ///==-----------
     //==--   OUTPUTS
     ///==-----------
     
     
     //Starts the Backprop procedures for output only.
     void bp_Output_Only(unsigned long long int p_Node)
     {
          //For when directly output the dendrites of a nodes on the base tier.
          if (p_Node == 0){ return; }
          ochr(0, 12, '<');
          bp_Output_Only_L(p_Node);
          ochr(0, 12, '>');
     }
     
     //bp_Output the left node.
     void bp_Output_Only_L(unsigned long long int p_Node)
     {
          goto_Node(p_Node);
          char tmp_State = ' ';
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (current_Node.Dendrite_L.U != 0)
          {
               bp_Output_Only_L(current_Node.Dendrite_L.U);
               goto_Node(p_Node);
               bp_Output_Only_R(current_Node.Dendrite_R.U);
          }
          else
          {
               if (current_Node.Type == 0)
               {
                    tmp_State = char (current_Node.State.U);
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    cout << tmp_State;
               }
               if (current_Node.Type == 1)
               {
                    cout << current_Node.State.U << " ";
               }
          }
     }
     
     //bp_Output the right node.
     void bp_Output_Only_R(unsigned long long int p_Node)
     {
          goto_Node(p_Node);
          char tmp_State = ' ';
          //If a right leg exists then initiate a backpropagation.
          if (current_Node.Dendrite_R.U != 0)
          {
               bp_Output_Only_R(current_Node.Dendrite_R.U);
          }
          else
          {
               if (current_Node.Type == 0)
               {
                    tmp_State = char (current_Node.State.U);
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    cout << tmp_State;
               }
               if (current_Node.Type == 1)
               {
                    cout << current_Node.State.U << " ";
               }
          }
     }
     
     
     

     //Starts the Backprop procedures for output only.
     void bp(c_Raw_Table_1D * p_Pattern_Output, int p_Flat_Output, int p_Input, unsigned long long int p_Node)
     {
          //cout << "\n\n void bp(c_Raw_Table_1D * " << p_Pattern_Output << ", int " << p_Flat_Output << ", int " << p_Input << ", unsigned long long int " << p_Node << ")";
          //For when directly output the dendrites of a nodes on the base tier.
          if (p_Node == 0){ return; }
          
          if (p_Flat_Output)
          {
               p_Pattern_Output->new_Cell(p_Input);
          }
          else
          {
               p_Pattern_Output->new_Cell(0);
          }
          bp_L(p_Pattern_Output, p_Input, p_Node);
     }
     
     //bp_Output the left node.
     void bp_L(c_Raw_Table_1D * p_Pattern_Output, int p_Input, unsigned long long int p_Node)
     {
          //cout << "\n   void bp_L(c_Raw_Table_1D * " << p_Pattern_Output << ", int " << p_Input << ", unsigned long long int " << p_Node << ")";
          goto_Node(p_Node);
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (current_Node.Dendrite_L.U != 0)
          {
               bp_L(p_Pattern_Output, p_Input, current_Node.Dendrite_L.U);
               goto_Node(p_Node);
               bp_R(p_Pattern_Output, p_Input, current_Node.Dendrite_R.U);
          }
          else
          {
               p_Pattern_Output->add_Data_Int(p_Input, current_Node.State.U);
               //oint(0, 12, current_Node.State.U);
          }
     }
     
     //bp_Output the right node.
     void bp_R(c_Raw_Table_1D * p_Pattern_Output, int p_Input, unsigned long long int p_Node)
     {
          //cout << "\n      void bp_R(c_Raw_Table_1D * " << p_Pattern_Output << ", int " << p_Input << ", unsigned long long int " << p_Node << ")";
          goto_Node(p_Node);
          //If a right leg exists then initiate a backpropagation.
          if (current_Node.Dendrite_R.U != 0)
          {
               bp_R(p_Pattern_Output, p_Input, current_Node.Dendrite_R.U);
          }
          else
          {
               p_Pattern_Output->add_Data_Int(p_Input, current_Node.State.U);
               //oint(0, 12, current_Node.State.U);
          }
     }
     
     
     
     void output_Datas(unsigned long long int p_NID, char * p_Datas, int p_Color = 12)
     {
          for (int cou_Index=0;cou_Index<16;cou_Index++)
          {
               cout << "\n" << cou_Index;
               ostr(0, 7, " Datas:");
               //cout << Axon_List_L[cou_A].U;
               cout << *reinterpret_cast<unsigned long long int*> (p_Datas +  (cou_Index * 8));
          }
     }
     
     //Outputs all of the nodes.
     void output_Nodes()
     {
          for (unsigned int cou_Index=0;cou_Index<CNID.U;cou_Index++)
          {
               goto_Node(cou_Index);
               cout << "\n\n NID:" << cou_Index << "\n";

               cout << "\n";
               for (int cou_Index=0;cou_Index<128;cou_Index+=32)
               {
                    cout << "\n";
                    for (int cou_U=0;cou_U<32;cou_U++)
                    {
                         ochr(8, 7, current_Node.tmp_Data[(cou_U + cou_Index)]);
                    }
                    for (int cou_U=0;cou_U<32;cou_U++)
                    {
                         cout << " " << int(current_Node.tmp_Data[cou_U + cou_Index]);
                    }
               }
          }
          
          //Faulture
          return;
     }
     
     //Outputs all of the nodes.
     void output_Nodes_Compact()
     {
          for (unsigned int cou_Index=0;cou_Index<CNID.U;cou_Index++)
          {
               goto_Node(cou_Index);
               current_Node.output_Condensed();
          }
          
          //Faulture
          return;
     }
          
     //Outputs a node in compact form.
     void output_Node_Compact(unsigned long long int p_Node)
     {
          goto_Node(p_Node);
          current_Node.output_Condensed();
          
          //Faulture
          return;
     }
     
     //Outputs the nodes to a canvas HTML document.
     long int current_Highest_Depth;
     int tmp_Text_PX;
     void output_Nodes_As_HTML(string p_Dir, string p_File, int p_X_Padd, int p_Y_Padd)
     {
          ofstream SF;
          string tmp_File = p_Dir + "/" + p_File;
          SF.open(tmp_File, ios::ate);
          
          if (!SF.is_open())
          {
               cout << "\n\n FILE " + tmp_File + " CANNOT BE OPENED!\n\n";
               return;
          }
          
          //Get the highest index.
          current_Highest_Depth = 0;
          for (int cou_Index=0;cou_Index<Tier_Current_Depth;cou_Index++)
          {
               if (Tier_Depth[cou_Index] > current_Highest_Depth)
               {
                    current_Highest_Depth = Tier_Depth[cou_Index];
               }
          }
          
          //Create and safe the header.
          SF << "\n\n\n<canvas id=\"myCanvas\" width=\"" << ((current_Highest_Depth * p_X_Padd) + (p_X_Padd * 2)) << "\" height=\"" << ((Tier_Current_Depth * p_Y_Padd) + (p_Y_Padd * 2)) << "\" style=\"border:1px solid #d3d3d3;\">";
          ///SF << "\n\n\n<canvas id=\"myCanvas\" width=\"1000\" height=\"1000\" style=\"border:1px solid #d3d3d3;\">";
          SF << "\nYour browser does not support the HTML5 canvas tag.</canvas>";
          
          SF << "\n<script>";
          SF << "\nvar c = document.getElementById(\"myCanvas\");";
          SF << "\nvar ctx = c.getContext(\"2d\");";

          SF << "\nctx.strokeStyle=\"#FFFFFF\";";
          SF << "\nctx.lineWidth=1;";
          
          tmp_Text_PX = p_X_Padd / 2;
          SF << "\nctx.font = \"" << p_X_Padd << "px Courier\"";
          //Now save every node and their connection.
          for (unsigned int cou_Index=1;cou_Index<CNID.U;cou_Index++)
          {
               goto_Node(cou_Index);
               output_Node_As_HTML(&SF, p_X_Padd, p_Y_Padd);
          }
          SF << "\n</script> ";

          SF << "\n</body>";
          SF << "\n</html>";
          SF.close();
     }
     
     //Outputs a single node to the canvas HTML document.
     //This is done from this level so that we can follow the dendrites.
     void output_Node_As_HTML(ofstream * p_SF, int p_X_Padd, int p_Y_Padd)
     {
          *p_SF << "\n";
          unsigned long long int tmp_DL = 0;
		  unsigned long long int tmp_DR = 0;
		  unsigned long long int tmp_X = 0;
		  unsigned long long int tmp_Y = 0;
          
          //Whether or not the node is a state node.
          //cout << "\n Type-->" << Type;
          
          //Axons and their counters.
          //cout << " A_Cou_L-->" << Axon_Count_L;
          //cout << " A_Cou_R-->" << Axon_Count_R;
          
          //Axons and their counters.
          //cout << " A_Blo_L-->" << Axon_Block_L.U;
          //cout << " A_Blo_R-->" << Axon_Block_R.U;
          
          long int tmp_Offset = (((long int) (current_Highest_Depth / 2)) - ((long int) (Tier_Depth[current_Node.Tier] / 2)));
          tmp_X = (((current_Node.Tier_Index + tmp_Offset) * p_X_Padd) + p_X_Padd);
          tmp_Y = ((current_Node.Tier * p_Y_Padd) + p_Y_Padd);
               
          if (current_Node.Type == 0)
          { 
               //cout << " State-->" << char(current_Node.State.U); 
               
               //cout << " Tier-->" << current_Node.Tier;
               
               //cout << " Tier_Index-->" << current_Node.Tier_Index;
               *p_SF << "\nctx.beginPath();";
               *p_SF << "\n\nctx.moveTo(" << tmp_X << ", " << tmp_Y << ");";
               *p_SF << "\nctx.strokeStyle=\"#0F0F0F\";";
               *p_SF << "\nctx.arc(" << tmp_X << "," << tmp_Y << ",1,0,2*Math.PI);";
               *p_SF << "\nctx.stroke();";
               
               *p_SF << "\nctx.fillText(\"" << current_Node.State.C << "\", " << (tmp_X - (tmp_Text_PX / 2)) << " , " << (tmp_Y - (tmp_Text_PX / 2) - 2) << ");";
          }
          else
          {
               //Dendrites.
               //cout << " D_L-->" << current_Node.Dendrite_L.U;
               //cout << " D_R-->" << current_Node.Dendrite_R.U;
               
               //Reinforcement Counters.
               //cout << " RC_Lvl-->" << RC_Lvl;
               
               
               //cout << " Tier-->" << current_Node.Tier;
               
               //cout << " Tier_Index-->" << current_Node.Tier_Index;
               
               *p_SF << "\nctx.beginPath();";
               *p_SF << "\n\nctx.moveTo(" << tmp_X << ", " << tmp_Y << ");";
               *p_SF << "\nctx.strokeStyle=\"#0F0F0F\";";
               *p_SF << "\nctx.arc(" << tmp_X << "," << tmp_Y << ",1,0,2*Math.PI);";
               *p_SF << "\nctx.stroke();";
               
               //Get Coords of dendrites to link them.
               tmp_DL = unsigned long long int (current_Node.Dendrite_L.U);
               tmp_DR = unsigned long long int (current_Node.Dendrite_R.U);
               
               goto_Node(tmp_DL);
               tmp_Offset = (((long int) (current_Highest_Depth / 2)) - ((long int) (Tier_Depth[current_Node.Tier] / 2)));
               long int tmp_DL_T = ((current_Node.Tier * p_Y_Padd) + p_Y_Padd);
               long int tmp_DL_TI = (((current_Node.Tier_Index + tmp_Offset) * p_X_Padd) + p_X_Padd);
               
               goto_Node(tmp_DR);
               tmp_Offset = (((long int) (current_Highest_Depth / 2)) - ((long int) (Tier_Depth[current_Node.Tier] / 2)));
               long int tmp_DR_T = ((current_Node.Tier * p_Y_Padd) + p_Y_Padd);
               long int tmp_DR_TI = (((current_Node.Tier_Index + tmp_Offset) * p_X_Padd) + p_X_Padd);
               
               //cout << "\n     [Left] (" << tmp_DL_T << ", " << tmp_DL_TI << ")";
               //cout << "\n     [Right] (" << tmp_DR_T << ", " << tmp_DR_TI << ")";
               
               *p_SF << "\nctx.beginPath();"; 
               *p_SF << "\nctx.moveTo(" << tmp_X << ", " << tmp_Y << ");";
               //*p_SF << "\nctx.strokeStyle=\"#0000FF\";";
               *p_SF << "\nctx.strokeStyle=\"#0000FF\";";
               *p_SF << "\nctx.lineTo(" << tmp_DL_TI << ", " << tmp_DL_T << ");";
               *p_SF << "\nctx.stroke();";
               
               *p_SF << "\nctx.beginPath();";
               *p_SF << "\nctx.moveTo(" << tmp_X << ", " << tmp_Y << ");";
               *p_SF << "\nctx.strokeStyle=\"#FF0000\";";
               //*p_SF << "\nctx.strokeStyle=\"#A0A0A0\";";
               *p_SF << "\nctx.lineTo(" << tmp_DR_TI << ", " << tmp_DR_T << ");";
               *p_SF << "\nctx.stroke();";
          }
          //ctx.strokeStyle="#00FF00";

          //ctx.beginPath();
          //ctx.arc(100,100,5,0,2*Math.PI);
          //ctx.stroke();

          //ctx.moveTo(100, 100);
          //ctx.lineTo(200, 200);
          //ctx.stroke();
     }
};
























//The node network that handles the nodes.
class c_NT3_Node_Network_MMap_1D
{
private:
     
public:
     
     //Friend classes.
     friend class c_NT3_Buffer_MMap;
     
     //The node Database to use.
     c_NT3_NodeDB Nodes;
     
     //For all node operations query c_NT3_NodeDB
     //The one that started it all.
     //c_NT3_Base_Node_MMap_1D * Root;
     
     //The current node to attach to.
     //c_NT3_Base_Node_MMap_1D ** Current_Node;
     
     //The Fractional tree for handling state queries.
     c_NT3_Fractal_State_Tree_MMap State_Tree;
     
     //The tree to hold the Treetops for backpropagation.
     c_NT3_Fractal_State_Tree_MMap Treetops_Tree;
     
     //This tracks the current Nodes ID to be entered into the node tracking tree for saving and loading.
     u_Data_3 CNID;
     
     //Tree to track the nodes and their IDs.
     //Only used during saving and loading, then it is destroyed to save memory.
     c_NT3_Fractal_State_Tree_MMap NID_Tree;
     
     //The nodes will be loaded, read into the state tree, and then wiped from this list.
     //During runtime new state nodes will be added to the list.
     //A save function can be called to flush the recently created nodes into the state nodes file.
     c_LoTd_Linked_List_Handler State_List;
     string State_List_Filename;
     
     c_NT3_Node_Network_MMap_1D()
     {
          CNID.U = 0;
          //Root = NULL;
          //Current_Node = &Root;
     }
     
     ~c_NT3_Node_Network_MMap_1D()
     {
          cout << "\n ~c_NT3_Node_Network_MMap_1D " << CNID.I << " " << this << ".........."; cout.flush();
          //delete Root;
          
          /*
          c_NT3_Base_Node_MMap_1D * tmp_LL = Root;
          Root = NULL;
          c_NT3_Base_Node_MMap_1D * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               tmp_LL->Next = NULL;
               delete tmp_LL;
               tmp_LL = NULL;
               tmp_LL = tmp_LL_Next;
          }
          
          delete Root;
          Root = NULL;
          */
          cout << "\n ~~~c_NT3_Node_Network_MMap_1D " << CNID.I << " " << this << ".........."; cout.flush();
     }
     
     //Initializes the network with a file to use. Has to be done in order to use the network.
     int init(string p_Dir, string p_File)
     {
          State_List_Filename = p_Dir + "/" + p_File + ".states.dat";
          State_List.reset();
          load_State_Nodes();
          return Nodes.load_Network(p_Dir, p_File);
     }
     
     //Resets the tree.
     void reset()
     {
          //delete Root;
     }
     
     
     ////==------------------+
     //==--   NODE CREATION
     ////==------------------+
     
     //Creates a new node.
     u_Data_3 new_Node(int p_Tier)
     {
          //A tmp Node because we iterate the Current_Node.
          u_Data_3 tmp_Return_Node;
          tmp_Return_Node.U = 0;
          
          //Use nodeDB to create the new node.
          tmp_Return_Node.U = Nodes.new_Node(p_Tier);
          
          //Return the node that was created.
          return tmp_Return_Node;
     }
     
     //Creates a treetop state node for cases where input has only one tier.
     void convert_To_Treetop_Node(unsigned long long int p_Node)
     {
          u_Data_3 tmp_NID; tmp_NID.U = p_Node;
          if (Nodes.get_Type(p_Node) == 0){ Nodes.set_Type(p_Node, 2); Nodes.save_Node(); } //If the node is a state node then set it to a state treetop node.
          if (Nodes.get_Type(p_Node) == 1){ Nodes.set_Type(p_Node, 3); Nodes.save_Node(); } //IF the node is a normal node then set the type to treetop.
          
          //Search for the node in the treetop tree.
          Treetops_Tree.search(tmp_NID);
          
          //If the node has not been found then add it to the tree.
          if (Treetops_Tree.get_Current_Node_NID() == 0)
          {
               Treetops_Tree.set_Current_Node_NID(tmp_NID);
          }
     }
     
     
     //Creates a new state node.
     u_Data_3 new_State_Node(u_Data_3 p_State)
     {
          //A tmp Node because we iterate the Current_Node.
          u_Data_3 tmp_Return_Node;
          tmp_Return_Node.U = 0;
          
          //Use nodeDB to create the new node.
          tmp_Return_Node.U = Nodes.new_State_Node(p_State);
          
          //Add it to the state tree, assuming that the current state has already been queried.
          State_Tree.set_Current_Node_NID(tmp_Return_Node);
          
          //Add the newly created state node to the linked list for saving.
          State_List.new_LL(tmp_Return_Node, p_State);
          
          /*
          //Create the state node.
          *Current_Node = new c_NT3_State_Node_MMap_1D;
          
          //Sets the new nodes ID.
          (*Current_Node)->NID.I = CNID.I;
          
          //Increment the node tracker.
          CNID.I++;
          
          //If axon couts are given expand the axon arrays.
          if (p_A_L){ (*Current_Node)->expand_Axon_L(p_A_L); }
          if (p_A_R){ (*Current_Node)->expand_Axon_R(p_A_R); }
          
          //Add it to the state tree, assuming that the current state has already been queried.
          State_Tree.set_Current_Node_NID(*Current_Node);
          
          //Set the current nodes state.
          (*Current_Node)->set_State(p_State);
          
          //Set the Current node to the next one in the chain.
          Current_Node = &(*Current_Node)->Next;
                    
          //Return the created state node using the state tree as Current_Node is changed.
          return State_Tree.get_Current_Node_NID();
           * */
           
          //Return the node that was created.
          return tmp_Return_Node;
     }
     
     
     ////==------------------------------------+
     //==--   CONNECTION TESTING AND CREATION
     ////==------------------------------------+
     
          
     //Creates a connection from p_From to p_To.
     void create_Connection_L(unsigned long long int p_From, unsigned long long int p_To)
     {
          //Nodes.add_Axon_L(p_From, p_To);
          //Nodes.set_Dendrite_L(p_To, p_From);
     }
     
     //Creates a connection from p_From to p_To.
     void create_Connection_R(unsigned long long int p_From, unsigned long long int p_To)
     {
          //Nodes.add_Axon_R(p_From, p_To);
          //Nodes.set_Dendrite_R(p_To, p_From);
     }
     
     //Checks for an upper tier connection.
     unsigned long long int does_Upper_Tier_Connection_Exist(unsigned long long int p_L, unsigned long long int p_R)
     {
          if (p_L == 0 || p_R == 0){ return 0; }
          return Nodes.does_Upper_Tier_Connection_Exist(p_L, p_R);
     }
     
     
     double get_RC_Score(u_Data_3 p_Node)
     {
          return get_RC_Score(p_Node.U);
     }
     
     double get_RC_Score(unsigned long long int p_Node)
     {
          return Nodes.get_RC_Score(p_Node);
     }
     
     //Gets the current node count.
     unsigned long long int get_Node_Count()
     {
          return Nodes.CNID.U;
     }
     
     ////==---------------------+
     //==--   RETRIEVING NODES
     ////==---------------------+
     
     //Assigns a given node to a state, used for loading.
     /*
     c_NT3_Base_Node_MMap_1D * assign_State_Node(u_Data_3 p_State, c_NT3_Base_Node_MMap_1D * p_Node, int p_A_L = 0, int p_A_R = 0)
     {
          //Search for the node.
          State_Tree.search(p_State);
          
          //If the node has not been found then create it.
          if (State_Tree.get_Current_Node_NID() == NULL)
          {
               new_State_Node(p_State);
          }
          
          //Return the current node NID.
          return State_Tree.get_Current_Node_NID();
     }*/
     
     //Get a state node.
     unsigned long long int get_State_Node(u_Data_3 p_State)
     {
          //Search for the node.
          State_Tree.search(p_State);

          //If the node has not been found then create it.
          if (State_Tree.get_Current_Node_NID() == 0)
          {
               new_State_Node(p_State);
          }

          //Return the current node NID.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Get a state node for a query, if none exist return NULL.
     unsigned long long int get_State_Node_Query(u_Data_3 p_State)
     {
          //Search for the node.
          State_Tree.search(p_State);
          
          //Return the current node NID.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Gets an upper tier connection even if one has to be created.
     unsigned long long int get_Upper_Tier_Connection(int p_Tier, unsigned long long int p_L, unsigned long long int p_R)
     {
          return Nodes.get_Upper_Tier_Connection(p_Tier, p_L, p_R);
     }
     
     //Gets a treetop connection.
     unsigned long long int get_Treetop_Connection(int p_Tier, unsigned long long int p_L, unsigned long long int p_R)
     {
          u_Data_3 tmp_Node;
          tmp_Node.U = get_Upper_Tier_Connection(p_Tier, p_L, p_R);
          
          convert_To_Treetop_Node(tmp_Node.U);
          
          return tmp_Node.U;
     }
     
     //Reinforces a node.
     void reinforce(unsigned long long int p_Node)
     {
          Nodes.reinforce(p_Node);
     }
     
     
     //Gets the type of node.
     int get_Type(unsigned long long int p_Node)
     {
          return int(Nodes.get_Type(p_Node));
     }
     
     //Gets the axon counts.
     int get_Axon_Count_L(unsigned long long int p_Node)
     {
          return Nodes.get_Axon_Count_L(p_Node);
     }
     
     //Gets the axon counts.
     int get_Axon_Count_R(unsigned long long int p_Node)
     {
          return Nodes.get_Axon_Count_R(p_Node);
     }
     
     //Takes an array of u_Data_3 objects and fills them out with the axons for the node.
     void get_Axons_L(unsigned long long int p_Node, u_Data_3 * p_Axons)
     {
          Nodes.get_Axons_L(p_Node, p_Axons);
     }
     
     //Takes an array of u_Data_3 objects and fills them out with the axons for the node.
     void get_Axons_R(unsigned long long int p_Node, u_Data_3 * p_Axons)
     {
          Nodes.get_Axons_R(p_Node, p_Axons);
     }
     
     //Gets the axon.
     unsigned long long int get_Axon_L(unsigned long long int p_Node, int p_Axon)
     {
          return 0;
     }
     
     //Gets the axon.
     unsigned long long int get_Axon_R(unsigned long long int p_Node, int p_Axon)
     {
          return 0;
     }
     
     ////==----------------------------+
     //==--   BACKPROPAGATION STATION
     ////==----------------------------+
     
     //Starts the Backprop procedures for output only.
     void bp_Output_Only(unsigned long long int p_Node)
     {
          Nodes.bp_Output_Only(p_Node);
          return;
     }
     
     //bp_Output the left node.
     void bp_Output_Only_L(unsigned long long int p_Node, int p_Type)
     {
          return;
     }
     
     //bp_Output the right node.
     void bp_Output_Only_R(unsigned long long int p_Node, int p_Type)
     {
          return;
     }
     
     //Starts the Backprop procedures for output only.
     void bp(c_Raw_Table_1D * p_Pattern_Output, int p_Flat_Output, int p_Input, u_Data_3 p_Node)
     {
          bp(p_Pattern_Output, p_Flat_Output, p_Input, p_Node.U);
     }
     
     //Starts the Backprop procedures for output only.
     void bp(c_Raw_Table_1D * p_Pattern_Output, int p_Flat_Output, int p_Input, unsigned long long int p_Node)
     {
          //For when directly output the dendrites of a nodes on the base tier.
          if (p_Node == 0){ return; }
          
          Nodes.bp(p_Pattern_Output, p_Flat_Output, p_Input, p_Node);
     }
     
     //bp_Output the left node.
     void bp_L(c_Raw_Table_1D * p_Pattern_Output, int p_Input, unsigned long long int p_Node)
     {
          /*
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (p_Node->Dendrite_L != NULL)
          {
               bp_L(p_Pattern_Output, p_Input, p_Node->Dendrite_L);
               bp_R(p_Pattern_Output, p_Input, p_Node->Dendrite_R);
          }
          else
          {
               p_Pattern_Output->add_Data_Int(p_Input, p_Node->get_State());
          }
          */
     }
     
     //bp_Output the right node.
     void bp_R(c_Raw_Table_1D * p_Pattern_Output, int p_Input, unsigned long long int p_Node)
     {
          /*
          //If a right leg exists then initiate a backpropagation.
          if (p_Node->Dendrite_R != NULL)
          {
               bp_R(p_Pattern_Output, p_Input, p_Node->Dendrite_R);
          }
          else
          {
               p_Pattern_Output->add_Data_Int(p_Input, p_Node->get_State());
          }
           * */
     }
     
     
     //Backpropagates a given Treetop. Returns the treetops address.
     unsigned long long int bp_Treetop(c_Raw_Table_1D * p_Pattern_Output,int p_Flat_Output, int p_Input, u_Data_3 p_NID)
     {
          return bp_Treetop(p_Pattern_Output, p_Flat_Output, p_Input, p_NID.U);
     }
     
     //Backpropagates a given Treetop. Returns the treetops address.
     unsigned long long int bp_Treetop(c_Raw_Table_1D * p_Pattern_Output,int p_Flat_Output, int p_Input, unsigned long long int p_NID)
     {
          /*
          u_Data_3 tmp_Node;
          current_Node.U = 0;
          
          //Search for the node.
          Treetops_Tree.search(p_NID);
          
          //Return the current node NID.
          tmp_Node = Treetops_Tree.get_Current_Node_NID();
          
          //Backpropagate the treetop node. It is assumed this will always be a valid treetop, however, the bp() will catch a NULL with no error.
          bp(p_Pattern_Output, p_Flat_Output, p_Input, tmp_Node);
          
          return tmp_Node;
          */
          return 0;
     }
     
     ////==---------------------------------+
     //==--   SAVING AND LOADING FUNCTIONS
     ////==---------------------------------+
     
     //Used to save the header updated info. Namely CNID
     void update_Header(string p_Dir, string p_FName = "NULL")
     {
          Nodes.save_Network(p_Dir, p_FName, 0);
          save_State_Nodes();
     }
     
     //Loads the state nodes.
     void load_State_Nodes()
     {
          ifstream SF;
          SF.open(State_List_Filename);
          
          if (!SF.is_open()){ cout << "\n State Loading FAILED..."; return; }
          
          //The temp vars for loading all of the states.
          u_Data_3 tmp_State;
          u_Data_3 tmp_Node;
          
          if ((SF.peek() == std::ifstream::traits_type::eof())){ SF.close(); return; }
               
          while (!SF.eof())
          {
               SF >> tmp_Node.U;
               SF >> tmp_State.U;
               //State_List.new_LL(tmp_State, tmp_Node);
               
               //Search for the node.
               State_Tree.search(tmp_State);
               
               //Add it to the state tree, assuming that the current state has already been queried.
               State_Tree.set_Current_Node_NID(tmp_Node);
               
               //cout << "\n" << tmp_Node.U << " " << tmp_State.U << " " << char (tmp_State.U);
          }
          SF.close();
     }
     
     //Saves teh state nodes.
     void save_State_Nodes()
     {
          ofstream SF;
          SF.open(State_List_Filename, ios::app);
          
          if (!SF.is_open()){ cout << "\n State Saving FAILED..."; return; }
          
          c_LoTd_Linked_List * tmp_LL = State_List.Root;
          
          if (tmp_LL == NULL){ SF.close(); return; }
          
          while (tmp_LL != NULL)
          {
               //State_List.new_LL(tmp_State, tmp_Node);
               SF << "\n" << tmp_LL->ID.U  << " " << tmp_LL->Data.U;
               //cout << "\n" << tmp_LL->ID.U << " " << tmp_LL->Data.U << " " << char (tmp_LL->Data.U);
               tmp_LL = tmp_LL->Next;
          }
          tmp_LL = NULL;
          State_List.reset();
          SF.close();
     }
     
     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     void output_Node_Compact(unsigned long long int p_Node)
     {
          Nodes.output_Node_Compact(p_Node);
     }
     
     void output_Treetops()
     {
          Treetops_Tree.output_Tree();
     }
     
     /*
     //Outputs the treetops as backpropagated nodes.
     void output_Treetops_BP()
     {
          Treetops_Tree.output_Tree_BP();
     }
     */
     
     //Oututs all of the nodes.
     void output_Nodes()
     {
          Nodes.output_Nodes();
          /*
          c_NT3_Base_Node_MMap_1D * tmp_LL = Root;
          //Root = NULL;
          //Root = NULL;
          c_NT3_Base_Node_MMap_1D * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               cout << "\n  ";
               tmp_LL->bp_O();
               tmp_LL = tmp_LL_Next;
          }
           * */
     }
     
     //Oututs all of the nodes to a HTML file with a <canvas>
     void output_Nodes_As_HTML(string p_DIR, string p_File, int p_X_Padd, int p_Y_Padd)
     {
          Nodes.output_Nodes_As_HTML(p_DIR, p_File, p_X_Padd, p_Y_Padd);
          /*
          c_NT3_Base_Node_MMap_1D * tmp_LL = Root;
          //Root = NULL;
          //Root = NULL;
          c_NT3_Base_Node_MMap_1D * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               cout << "\n  ";
               tmp_LL->bp_O();
               tmp_LL = tmp_LL_Next;
          }
           * */
     }
     
};





















































/*

class c_MMap
{
private:
     
     //Size of the memory chunk.
     const int CONST_BUFFER_SIZE = 128;
     
     //The file to open.
     HANDLE Data_File;
     
     //Handle to the mapped file
     HANDLE MMap_File_Mapping;
     
     //Handle to the mapped file view
     HANDLE MMap_View;
     
     //System information. Used to get the system granularity.
     SYSTEM_INFO system_Info;
     DWORD system_Granulation;
     
     //Get the nearest multiple of system grains to the starting offset. Where to start the mapping.
     DWORD MMap_Starting_Offset;
     
     //The size of the file mapping view.
     //MMap_View_Size = (MMap_Starting_Offset % system_Granulation) + BUFFSIZE
     DWORD MMap_View_Size;
     
     //The size of the file mapping object.
     DWORD MMap_File_Map_Size;
     
     //The current position of the data pointer in the file.
     int MMap_Current_Position;
     
public:
     
     string file_Name;
     
     void * MMap_Current_Position_Data;
     
     c_MMap()
     {
          Data_File = NULL;
          MMap_Current_Position = 0;
          MMap_File_Map_Size = 0;
          MMap_File_Mapping = NULL;
          MMap_Starting_Offset = 0;
          MMap_View = NULL;
          MMap_View_Size = 0;
     }
     
     
     void get_System_Granulation()
     {
          // Get the system allocation granularity.
          GetSystemInfo(&system_Info);
          system_Granulation = system_Info.dwAllocationGranularity;
          
     }
     
     void calculate_Mapping_Parameters(int p_Map_Size = 512)
     {
          cout << "\n p_Map_Size->" << p_Map_Size;
          
          //Get the system granulation.
          get_System_Granulation();
          
          cout << "\n system_Granulation->" << system_Granulation;
          
          //Get the MMap_Starting_Offset for where to start the mapping in the file grains.
          MMap_Starting_Offset = 0;//(p_File_Map_Start / system_Granulation) * system_Granulation;
          
          cout << "\n MMap_Starting_Offset->" << MMap_Starting_Offset;
          
          //Calculate the size of the mapping view.
          MMap_View_Size = CONST_BUFFER_SIZE;//(p_File_Map_Start % system_Granulation) + CONST_BUFFER_SIZE;
          
          cout << "\n MMap_View_Size->" << MMap_View_Size;
          
          //Figure size of the file mapping object.
          MMap_File_Map_Size = (p_Map_Size * (1024 * 1024)) + CONST_BUFFER_SIZE;
          
          cout << "\n MMap_File_Map_Size->" << MMap_File_Map_Size;
          
          //Find the current position of the data pointer to use when referencing file contents.
          //MMap_Current_Position = p_File_Map_Start - MMap_Starting_Offset;
          MMap_Current_Position = MMap_Starting_Offset;
          
          cout << "\n MMap_Current_Position->" << MMap_Current_Position;
     }
     
     //Maps the files, does not create the view.
     int create_Mapping()
     {
          MMap_File_Mapping = CreateFileMapping(Data_File,          // current file handle
               NULL,           // default security
               PAGE_READWRITE, // read/write permission
               0,              // size of mapping object, high
               MMap_File_Map_Size,  // size of mapping object, low
               NULL);  
          
          if (MMap_File_Mapping == NULL)
          {
               cout << "\n MMap_File_Mapping == NULL LastError():" << GetLastError();
               return 2;
          }
          
          return 1;
     }
     
     
     void calculate_View_Parameters(int p_Position)
     {
          //Get the MMap_Starting_Offset for where to start the mapping in the file grains.
          MMap_Starting_Offset = (p_Position / system_Granulation) * system_Granulation;
          
          //cout << "\n MMap_Starting_Offset->" << MMap_Starting_Offset;
          
          //Calculate the size of the mapping view.
          MMap_View_Size = (p_Position % system_Granulation) + CONST_BUFFER_SIZE;
          
          //cout << "\n MMap_View_Size->" << MMap_View_Size;
          
          MMap_Current_Position = p_Position;
          
          cout << "\n MMap_Current_Position->" << MMap_Current_Position;
     }
     
     //Creates a view at the given offset.
     int create_Mapping_View(unsigned int p_Position = 0)
     {
          calculate_View_Parameters(p_Position);
          
          //Do this in case a view is already mapped.
          if (MMap_View != NULL)
          {
               UnmapViewOfFile(MMap_View);
               //cout << "\n Unmapped MMap_View:" << GetLastError();
               MMap_View = NULL;
          }
          
          MMap_View = MapViewOfFile(MMap_File_Mapping,            // handle to
                                // mapping object
               FILE_MAP_ALL_ACCESS, // read/write
               0,                   // high-order 32
                                   // bits of file
                                   // offset
               MMap_Starting_Offset,      // low-order 32
                                   // bits of file
                                   // offset
               MMap_View_Size);      // number of bytes
                                   // to map
                                   
          if (MMap_View == NULL)
          {
               cout << "\n MMap_View == NULL LastError():" << GetLastError();
               return 3;
          }
          
          MMap_Current_Position_Data = ((char*) MMap_View + MMap_Current_Position);
          return 1;
     }
     
     //The p_File_Map_Start is the offset to use in the beginning. 
     //There will be a function for moving the view.
     int open_File(string p_File_Name, int p_Map_Size_MB = 512)
     {
          
          Data_File = NULL;
          
          cout << "\n Opening " << p_File_Name;
          
          calculate_Mapping_Parameters(p_Map_Size_MB);
          
          Data_File = CreateFile(p_File_Name.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
          
          //Map the file
          create_Mapping();
          
          //Create the view.
          create_Mapping_View(0);
          
          //Determine if the mapping was a success.
          if (MMap_Current_Position_Data == NULL){ return 0; }
          
          return 1;
     }
     
     void close()
     {
          UnmapViewOfFile(MMap_View);
          CloseHandle(MMap_File_Mapping); 
          CloseHandle(Data_File);   
     }
     
     //Sets the current MMap_View to the given data.
     void set_Data(char * p_Data, int p_Len)
     {
          for (int cou_Index=0;cou_Index<p_Len;cou_Index++)
          {
               ((char*)MMap_Current_Position_Data)[cou_Index] = p_Data[cou_Index];
          }
     }
     
     void save_Data()
     {
          FlushViewOfFile(MMap_Current_Position_Data, 128);
     }
     
     //Gets the current datas.
     char * get_Data(int p_Len)
     {
          return ((char*)MMap_Current_Position_Data);
     }
     
     //Displays the contents of the currently mapped view block.
     void output_View(int p_Mode = 0)
     {
          char * tmp_Data = NULL;
          if (p_Mode == 0)
          {
               cout << "\n -----------------------------------------------------------------------------\n";
               for (int cou_Index=0;cou_Index<CONST_BUFFER_SIZE;cou_Index++)
               {
                    tmp_Data = ((char*) MMap_View + MMap_Current_Position + cou_Index);
                    
                    cout << (*tmp_Data);
               }
               cout << "\n -----------------------------------------------------------------------------\n";
          }
          if (p_Mode == 1)
          {
               for (int cou_Index=0;cou_Index<CONST_BUFFER_SIZE;cou_Index++)
               {
                    tmp_Data = ((char*) MMap_View + MMap_Current_Position + cou_Index);
                    
                    cout << "\n " << cou_Index << "->" << (*tmp_Data) << "<-  int->" << ((int) (*tmp_Data));
               }
          }
     }
};


*/