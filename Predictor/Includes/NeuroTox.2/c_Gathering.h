//This class allows for the manipulation of a gathering. Gatherings are not dimension dependent.

class c_Base_Gathering
{
protected:

     //The name of the assembly.
     string Gathering_Name;

     //The assemblies contained in the assembly.
     c_Base_Assembly *** Assemblies;

     //The current number of assemblies in each tier.
     int ** Assembly_Count;

     //The total number of assemblies, used to assign ID.
     int Assembly_Total_Count;

     //The height of the assemblies structure.
     int Assembly_Height;

     //The assembly registry.
     c_Lookup_Tree Assembly_Registry;
     c_Lookup_Tree Assembly_Registry_Name;
     c_Lookup_Tree Assembly_Registry_Tier;
     c_Lookup_Tree Assembly_Registry_Index;

     //The Construct Registry. Used for indexing before loading.
     //Indexes U_CID with CID.
     c_Lookup_Tree Construct_Registry;

     //Indexes U_CID with AID.
     c_Lookup_Tree Construct_Registry_AID;

public:

     //--   TABLES


     //The table handler associated with this gathering.
     c_Table_Handler * Tables;

     //Input table to build the inputs from.
     c_Table_1D * Input_Table;

     //Input table to build the inputs from.
     c_Table_1D * Input_Table_Truth;

     //The pattern output table.
     c_Table_1D * Output_Table_Pattern;

     //The charge output table.
     c_Table_1D * Output_Table_Charge;

     //The nodes output table.
     c_Table_1D * Output_Table_Nodes;

     //The RC output table.
     c_Table_1D * Output_Table_RC;

     //The truth output table.
     c_Table_1D * Output_Table_Truth;

     //The setting table for the assembly.
     c_Table_1D * Settings_Table;



     c_Base_Gathering(c_Table_Handler * p_Tables, string p_Gathering_Name)
     {

          //Init the assembly handing members.
          Assemblies = new c_Base_Assembly**[1];
          Assemblies[0] = new c_Base_Assembly*[1];
          Assemblies[0] [0] = NULL;

          Assembly_Count = new int*[1];
          Assembly_Count[0] = new int;
          *Assembly_Count[0] = 0;

          Assembly_Total_Count = 0;

          Assembly_Height = 0;

          //Set the gathering name.
          Gathering_Name = p_Gathering_Name;

          //Set the Tables.
          Tables = p_Tables;


          //Setup the tables.
          Input_Table = Tables->register_New_Table_1D("g_Input");
          Input_Table_Truth = Tables->register_New_Table_1D("g_Input_Truth");

          Output_Table_Pattern = Tables->register_New_Table_1D("g_Output_Pattern");
          Output_Table_Charge = Tables->register_New_Table_1D("g_Output_Charge");
          Output_Table_Nodes = Tables->register_New_Table_1D("g_Output_Nodes");
          Output_Table_Truth = Tables->register_New_Table_1D("g_Output_Truth");
          Output_Table_RC = Tables->register_New_Table_1D("g_Output_RC");


          //With the settings table the first index is the name of the setting.
          Settings_Table = Tables->register_New_Table_1D("g_Settings");

     }

     ~c_Base_Gathering()
     {
          //Update the registry.
          Tables->delete_Table(Input_Table->get_Table_ID());
          Tables->delete_Table(Input_Table_Truth->get_Table_ID());

          Tables->delete_Table(Output_Table_Pattern->get_Table_ID());
          Tables->delete_Table(Output_Table_Charge->get_Table_ID());
          Tables->delete_Table(Output_Table_Nodes->get_Table_ID());
          Tables->delete_Table(Output_Table_Truth->get_Table_ID());
          Tables->delete_Table(Output_Table_RC->get_Table_ID());

          Tables->delete_Table(Settings_Table->get_Table_ID());

          //Delete the constructs.
          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               for (int cou_Ass=0;cou_Ass<*Assembly_Count[cou_Tier];cou_Ass++)
               {
                    delete Assemblies[cou_Tier] [cou_Ass];
                    Assemblies[cou_Tier] [cou_Ass] = NULL;
               }
               delete [] Assemblies[cou_Tier];
               Assemblies[cou_Tier] = NULL;
          }
          delete [] Assemblies;
          Assemblies = NULL;
     }



     //--     INPUT GATHERING

     //Resets the tables.
     void reset_Tables()
     {
          Input_Table->reset_Table();
          Input_Table_Truth->reset_Table();

          Output_Table_Charge->reset_Table();
          Output_Table_Pattern->reset_Table();
          Output_Table_Nodes->reset_Table();
          Output_Table_Truth->reset_Table();
          Output_Table_RC->reset_Table();
     }

     //Resets all of the input tables.
     void reset_All_Inputs()
     {
          reset_Tables();


          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               for (int cou_Assembly=0;cou_Assembly<*Assembly_Count[cou_Tier];cou_Assembly++)
               {
                    Assemblies[cou_Tier] [cou_Assembly]->reset_All_Inputs();
               }
          }
     }

     //Updates the values of the variables based on the settings table.
     void update_Settings()
     {
          //cout << "\n\t Gathering::update_Settings()"; cout.flush();
          Settings_Table->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(0, 0, &Gathering_Name);

     }

     //Updates the settings of the assemblies.
     void update_Assembly_Settings()
     {
          //cout << "\n\t Gathering::update_Assembly_Settings()"; cout.flush();

          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               for (int cou_Index=0;cou_Index<*Assembly_Count[cou_Tier];cou_Index++)
               {
                    //cout << "\n\t Assembly[" << cou_Tier << "] [" << cou_Index << "]->" << Assemblies[cou_Tier] [cou_Index]; cout.flush();
                    if (Assemblies[cou_Tier] [cou_Index] != NULL){ Assemblies[cou_Tier] [cou_Index]->update_All_Settings(); }
               }

          }
     }

     //Updates all settings.
     void update_All_Settings()
     {
          //cout << "\n\t Gathering::update_All_Settings()"; cout.flush();
          update_Settings();
          update_Assembly_Settings();
     }

     //Submit an input to the input table.
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -"INPUT"
     //   0            1          2          3       4      5
     void push_Input(int p_Row, string p_Input){ Input_Table->push_string(p_Row, p_Input); }
     void set_Input(int p_Row, int p_Cell, string p_Input){ Input_Table->set_string(p_Row, p_Cell, p_Input); }

     void push_Input_Truth(int p_Row, string p_Input){ Input_Table_Truth->push_string(p_Row, p_Input); }
     void set_Input_Truth(int p_Row, int p_Cell, string p_Input){ Input_Table_Truth->set_string(p_Row, p_Cell, p_Input); }


     //--     ASSEMBLY INTERFACE

     //Creates a assembly of the type specified by the parameter passed, returns a reference to the new assembly.
     int create_Assembly(string p_Assembly_Name, string p_Assembly_Type = "Basic", int p_Tier = 0)
     {
          if (p_Assembly_Type == "Basic")
          {
               cout << "\n\t Basic Assembly Chosen For Creation.";
               cout << "\n\t Searching for NULL indexes.";
               for (int cou_Index=0;cou_Index<*Assembly_Count[0];cou_Index++)
               {
                    if (Assemblies[0] [cou_Index] == NULL)
                    {
                         cout << "\n\t NULL Index Found->" << cou_Index;
                         //cout << "\n\n\t Creating Assembly At NULL Index [" << cou_Index << "]";
                         Assemblies[p_Tier] [cou_Index] = new c_Base_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);
                         cout << "\n\t Assemblies[p_Tier] [cou_Index]->" << Assemblies[p_Tier] [cou_Index];
                         //Add the assembly to the registry.
                         Assembly_Registry.set_int(p_Assembly_Name, Assembly_Total_Count);
                         Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
                         Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
                         Assembly_Registry_Index.set_int(p_Assembly_Name, cou_Index);

                         //Increment the counter.
                         Assembly_Total_Count++;

                         //cout << "\n\t return;";
                         return Assembly_Total_Count - 1;
                    }
               }

               cout << "\n\t No NULL found, expanding the assemblies on tier 0.";
               expand_Assemblies(0);

               cout << "\n\t Creating Assembly At New Index [" << p_Tier << "] [" << *Assembly_Count[0] - 1 << "]";
               Assemblies[p_Tier] [*Assembly_Count[0] - 1] = new c_Base_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);

               cout << "\n\t Assembly [" << p_Tier << "] [" << *Assembly_Count[0] - 1 << "]->" << Assemblies[p_Tier] [*Assembly_Count[0] - 1];
               //Add the assembly to the registry.
               Assembly_Registry.set_int(p_Assembly_Name, (Assembly_Total_Count));
               Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
               Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
               Assembly_Registry_Index.set_int(p_Assembly_Name, (*Assembly_Count[0] - 1));

               //Increment the counter.
               Assembly_Total_Count++;

               return Assembly_Total_Count - 1;
          }

          if (p_Assembly_Type == "BCP")
          {
               for (int cou_Index=0;cou_Index<*Assembly_Count[0];cou_Index++)
               {
                    if (Assemblies[0] [cou_Index] == NULL)
                    {
                         //cout << "\n\n\t Creating Assembly At NULL Index [" << cou_Index << "]";
                         Assemblies[p_Tier] [cou_Index] = new c_BCP_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);

                         //Add the assembly to the registry.
                         Assembly_Registry.set_int(p_Assembly_Name, Assembly_Total_Count);
                         Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
                         Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
                         Assembly_Registry_Index.set_int(p_Assembly_Name, cou_Index);

                         //Increment the counter.
                         Assembly_Total_Count++;

                         return Assembly_Total_Count - 1;
                    }
               }

               expand_Assemblies(0);
               //cout << "\n\n\t Creating Assembly At New Index [" << Assembly_Count - 1 << "]";
               Assemblies[p_Tier] [*Assembly_Count[0] - 1] = new c_BCP_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);

               //Add the assembly to the registry.
               Assembly_Registry.set_int(p_Assembly_Name, (Assembly_Total_Count));
               Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
               Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
               Assembly_Registry_Index.set_int(p_Assembly_Name, (*Assembly_Count[0] - 1));

               //Increment the counter.
               Assembly_Total_Count++;

               return Assembly_Total_Count - 1;
          }

          if (p_Assembly_Type == "BCMSCP")
          {
               for (int cou_Index=0;cou_Index<*Assembly_Count[0];cou_Index++)
               {
                    if (Assemblies[0] [cou_Index] == NULL)
                    {
                         //cout << "\n\n\t Creating Assembly At NULL Index [" << cou_Index << "]";
                         Assemblies[p_Tier] [cou_Index] = new c_BCMSCP_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);

                         //Add the assembly to the registry.
                         Assembly_Registry.set_int(p_Assembly_Name, Assembly_Total_Count);
                         Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
                         Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
                         Assembly_Registry_Index.set_int(p_Assembly_Name, cou_Index);

                         //Increment the counter.
                         Assembly_Total_Count++;

                         return Assembly_Total_Count - 1;
                    }
               }

               expand_Assemblies(0);
               //cout << "\n\n\t Creating Assembly At New Index [" << Assembly_Count - 1 << "]";
               Assemblies[p_Tier] [*Assembly_Count[0] - 1] = new c_BCMSCP_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);

               //Add the assembly to the registry.
               Assembly_Registry.set_int(p_Assembly_Name, (Assembly_Total_Count));
               Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
               Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
               Assembly_Registry_Index.set_int(p_Assembly_Name, (*Assembly_Count[0] - 1));

               //Increment the counter.
               Assembly_Total_Count++;

               return Assembly_Total_Count - 1;
          }

          if (p_Assembly_Type == "MSC")
          {
               cout << "\n\t Basic MultiSensory Assembly Chosen.";
               cout << "\n\t Searching for NULL indexes.";

               for (int cou_Index=0;cou_Index<*Assembly_Count[0];cou_Index++)
               {
                    if (Assemblies[0] [cou_Index] == NULL)
                    {
                         cout << "\n\t NULL Index Found->" << cou_Index;
                         //cout << "\n\n\t Creating Assembly At NULL Index [" << cou_Index << "]";
                         Assemblies[p_Tier] [cou_Index] = new c_MSC_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);
                         cout << "\n\t Assemblies[p_Tier] [cou_Index]->" << Assemblies[p_Tier] [cou_Index];

                         //Add the assembly to the registry.
                         Assembly_Registry.set_int(p_Assembly_Name, Assembly_Total_Count);
                         Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
                         Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
                         Assembly_Registry_Index.set_int(p_Assembly_Name, cou_Index);

                         //Increment the counter.
                         Assembly_Total_Count++;

                         return Assembly_Total_Count - 1;
                    }
               }

               cout << "\n\t No NULL found, expanding the assemblies on tier 0.";
               expand_Assemblies(0);

               cout << "\n\t Creating Assembly At New Index [" << p_Tier << "] [" << *Assembly_Count[0] - 1 << "]";
               Assemblies[p_Tier] [*Assembly_Count[0] - 1] = new c_MSC_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);

               cout << "\n\t Assembly [" << p_Tier << "] [" << *Assembly_Count[0] - 1 << "]->" << Assemblies[p_Tier] [*Assembly_Count[0] - 1];
               //Add the assembly to the registry.
               Assembly_Registry.set_int(p_Assembly_Name, (Assembly_Total_Count));
               Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
               Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
               Assembly_Registry_Index.set_int(p_Assembly_Name, (*Assembly_Count[0] - 1));

               //Increment the counter.
               Assembly_Total_Count++;

               return Assembly_Total_Count - 1;
          }

          if (p_Assembly_Type == "MAC")
          {
               cout << "\n\t Multi Assembly Construct Chosen For Creation.";
               cout << "\n\t Searching for NULL indexes.";

               expand_Assembly_Tier(p_Tier + 1);

               for (int cou_Index=0;cou_Index<*Assembly_Count[p_Tier];cou_Index++)
               {
                    if (Assemblies[p_Tier] [cou_Index] == NULL)
                    {
                         cout << "\n\t NULL Index Found->" << cou_Index;
                         //cout << "\n\n\t Creating Assembly At NULL Index [" << cou_Index << "]";
                         Assemblies[p_Tier] [cou_Index] = new c_MAC_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);
                         cout << "\n\t Assemblies[p_Tier] [cou_Index]->" << Assemblies[p_Tier] [cou_Index];

                         //Add the assembly to the registry.
                         Assembly_Registry.set_int(p_Assembly_Name, Assembly_Total_Count);
                         Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
                         Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
                         Assembly_Registry_Index.set_int(p_Assembly_Name, cou_Index);

                         //Increment the counter.
                         Assembly_Total_Count++;

                         return Assembly_Total_Count - 1;
                    }
               }

               cout << "\n\t No NULL found, expanding the assemblies on tier p_Tier.";
               expand_Assemblies(p_Tier);

               cout << "\n\t Creating Assembly At New Index [" << p_Tier << "] [" << *Assembly_Count[p_Tier] - 1 << "]";
               Assemblies[p_Tier] [*Assembly_Count[p_Tier] - 1] = new c_MAC_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);

               cout << "\n\t Assembly [" << p_Tier << "] [" << *Assembly_Count[p_Tier] - 1 << "]->" << Assemblies[p_Tier] [*Assembly_Count[p_Tier] - 1];
               //Add the assembly to the registry.
               Assembly_Registry.set_int(p_Assembly_Name, (Assembly_Total_Count));
               Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
               Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
               Assembly_Registry_Index.set_int(p_Assembly_Name, (*Assembly_Count[p_Tier] - 1));

               //Increment the counter.
               Assembly_Total_Count++;

               return Assembly_Total_Count - 1;
          }

          if (p_Assembly_Type == "MAC_FLAT")
          {
               cout << "\n\t Multi Assembly Flat Building Construct Chosen For Creation.";
               cout << "\n\t Searching for NULL indexes.";

               expand_Assembly_Tier(p_Tier + 1);

               for (int cou_Index=0;cou_Index<*Assembly_Count[p_Tier];cou_Index++)
               {
                    if (Assemblies[p_Tier] [cou_Index] == NULL)
                    {
                         cout << "\n\t NULL Index Found->" << cou_Index;
                         //cout << "\n\n\t Creating Assembly At NULL Index [" << cou_Index << "]";
                         Assemblies[p_Tier] [cou_Index] = new c_MAC_FLAT_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);
                         cout << "\n\t Assemblies[p_Tier] [cou_Index]->" << Assemblies[p_Tier] [cou_Index];

                         //Add the assembly to the registry.
                         Assembly_Registry.set_int(p_Assembly_Name, Assembly_Total_Count);
                         Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
                         Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
                         Assembly_Registry_Index.set_int(p_Assembly_Name, cou_Index);

                         //Increment the counter.
                         Assembly_Total_Count++;

                         return Assembly_Total_Count - 1;
                    }
               }

               cout << "\n\t No NULL found, expanding the assemblies on tier p_Tier.";
               expand_Assemblies(p_Tier);

               cout << "\n\t Creating Assembly At New Index [" << p_Tier << "] [" << *Assembly_Count[p_Tier] - 1 << "]";
               Assemblies[p_Tier] [*Assembly_Count[p_Tier] - 1] = new c_MAC_FLAT_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);

               cout << "\n\t Assembly [" << p_Tier << "] [" << *Assembly_Count[p_Tier] - 1 << "]->" << Assemblies[p_Tier] [*Assembly_Count[p_Tier] - 1];
               //Add the assembly to the registry.
               Assembly_Registry.set_int(p_Assembly_Name, (Assembly_Total_Count));
               Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
               Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
               Assembly_Registry_Index.set_int(p_Assembly_Name, (*Assembly_Count[p_Tier] - 1));

               //Increment the counter.
               Assembly_Total_Count++;

               return Assembly_Total_Count - 1;
          }

          if (p_Assembly_Type == "MAC_COLUMN")
          {
               cout << "\n\t Multi Assembly COLUMN Building Construct Chosen For Creation.";
               cout << "\n\t Searching for NULL indexes.";

               expand_Assembly_Tier(p_Tier + 1);

               for (int cou_Index=0;cou_Index<*Assembly_Count[p_Tier];cou_Index++)
               {
                    if (Assemblies[p_Tier] [cou_Index] == NULL)
                    {
                         cout << "\n\t NULL Index Found->" << cou_Index;
                         //cout << "\n\n\t Creating Assembly At NULL Index [" << cou_Index << "]";
                         Assemblies[p_Tier] [cou_Index] = new c_MAC_COLUMN_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);
                         cout << "\n\t Assemblies[p_Tier] [cou_Index]->" << Assemblies[p_Tier] [cou_Index];

                         //Add the assembly to the registry.
                         Assembly_Registry.set_int(p_Assembly_Name, Assembly_Total_Count);
                         Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
                         Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
                         Assembly_Registry_Index.set_int(p_Assembly_Name, cou_Index);

                         //Increment the counter.
                         Assembly_Total_Count++;

                         return Assembly_Total_Count - 1;
                    }
               }

               cout << "\n\t No NULL found, expanding the assemblies on tier p_Tier.";
               expand_Assemblies(p_Tier);

               cout << "\n\t Creating Assembly At New Index [" << p_Tier << "] [" << *Assembly_Count[p_Tier] - 1 << "]";
               Assemblies[p_Tier] [*Assembly_Count[p_Tier] - 1] = new c_MAC_COLUMN_Assembly(Tables, p_Assembly_Name, Assembly_Total_Count);

               cout << "\n\t Assembly [" << p_Tier << "] [" << *Assembly_Count[p_Tier] - 1 << "]->" << Assemblies[p_Tier] [*Assembly_Count[p_Tier] - 1];
               //Add the assembly to the registry.
               Assembly_Registry.set_int(p_Assembly_Name, (Assembly_Total_Count));
               Assembly_Registry_Name.set_string(int2str(Assembly_Total_Count), p_Assembly_Name);
               Assembly_Registry_Tier.set_int(p_Assembly_Name, p_Tier);
               Assembly_Registry_Index.set_int(p_Assembly_Name, (*Assembly_Count[p_Tier] - 1));

               //Increment the counter.
               Assembly_Total_Count++;

               return Assembly_Total_Count - 1;
          }

          //Invalid type.
          return -1;
     }

     //Deletes an assembly from the array.
     void delete_Assembly(int p_AID)
     {
          if (!bounds_AID(p_AID)){ return; }

          Assembly_Registry.remove_Node(get_Assembly_Name(p_AID));
          delete Assemblies[p_AID];
          Assemblies[p_AID] = NULL;
     }
     void delete_Assembly(string p_Assembly)
     {
          //cout << "\n\n\t void delete_Assembly(string p_Assembly)";

          int tmp_AID = str2int(get_Assembly_ID(p_Assembly));

          if (!bounds_AID(tmp_AID)){ return; }

          Assembly_Registry.remove_Node(p_Assembly);
          delete Assemblies[tmp_AID];
          Assemblies[tmp_AID] = NULL;
     }



     //Sets the name of an assembly.
     void set_Assembly_Name(int p_AID, string p_Name)
     {
          if (!bounds_AID(p_AID)){ return; }

          //Update the registry.
          Assembly_Registry_Name.change_name(int2str(p_AID), p_Name);
          Assembly_Registry_Tier.change_name(get_Assembly_Name(p_AID), p_Name);
          Assembly_Registry_Index.change_name(get_Assembly_Name(p_AID), p_Name);
          Assembly_Registry.change_name(get_Assembly_Name(p_AID), p_Name);

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          Assemblies[tmp_Tier] [tmp_Index]->set_Assembly_Name(p_Name);

          Assembly_Registry.output_Tree();
     }
     void set_Assembly_Name(string p_Assembly, string p_Name)
     {
          //cout << "\n\n\t void set_Assembly_Name(string p_Assembly, string p_Name)";

          int tmp_AID = str2int(get_Assembly_ID(p_Assembly));

          if (!bounds_AID(tmp_AID)){ return; }

          set_Assembly_Name(tmp_AID, p_Name);
     }

     //Gets the name of an assembly.
     string get_Assembly_Name(int p_AID){ return Assembly_Registry_Name.get_string(int2str(p_AID)); }

     //Gets the ID of an assembly.
     string get_Assembly_Tier(string p_Assembly){ return int2str(Assembly_Registry_Tier.get_int(p_Assembly)); }
     string get_Assembly_Tier(int p_AID){ return get_Assembly_Tier(get_Assembly_Name(p_AID)); }

     //Gets the ID of an assembly.
     string get_Assembly_Index(string p_Assembly){ return int2str(Assembly_Registry_Index.get_int(p_Assembly)); }
     string get_Assembly_Index(int p_AID){ return get_Assembly_Index(get_Assembly_Name(p_AID)); }

     //Gets the ID of an assembly.
     string get_Assembly_ID(string p_Assembly){ return int2str(Assembly_Registry.get_int(p_Assembly)); }


     //Gets whether or not a given assembly exists.
     string exists_Assembly(string p_AName){ return int2str(Assembly_Registry.exists(p_AName)); }

     //Gets a reference to the given assembly to save space when referencing members.
     c_Base_Assembly * get_Assembly_Ref(string p_AName){ return Assemblies[str2int(get_Assembly_Tier(p_AName))] [str2int(get_Assembly_Index(p_AName))]; }
     c_Base_Assembly * get_Assembly_Ref(int p_AID){ string p_AName = get_Assembly_Name(p_AID); return Assemblies[str2int(get_Assembly_Tier(p_AName))] [str2int(get_Assembly_Index(p_AName))]; }

     //Gets the type of an assembly.
     string get_Assembly_Type(string p_Assembly)
     {
          if (get_Assembly_Ref(p_Assembly) != NULL)
          {
               return (get_Assembly_Ref(p_Assembly))->get_Assembly_Type();
          }
          return "Empty";
     }

     //Submit an input to the input table.
     //construct submit_input -ASSEMBLY  -ROW -"INPUT"
     //   0            1          2        3       4
     void push_Input_To_Assembly(string p_Assembly, int p_Row, string p_Input)
     {
          //cout << "\n\n\t push_Input_To_Assembly(string p_Assembly" << p_Assembly << ", int p_Row " << p_Row << ", string p_Input " << p_Input << ")";

          int tmp_AID = str2int(get_Assembly_ID(p_Assembly));

          if (!bounds_AID(tmp_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(tmp_AID));
          int tmp_Index = str2int(get_Assembly_Index(tmp_AID));

          Assemblies[tmp_Tier] [tmp_Index]->push_Input(p_Row, p_Input);
     }
     void push_Input_To_Assembly(int p_AID, int p_Row, string p_Input)
     {
          if (!bounds_AID(p_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          Assemblies[tmp_Tier] [tmp_Index]->push_Input(p_Row, p_Input);
     }

     //Submit an input to the input table.
     //construct submit_input -ASSEMBLY -ROW -CELL -"INPUT"
     //   0            1          2          3       4
     void set_Input_In_Assembly(string p_Assembly, int p_Row, int p_Cell, string p_Input)
     {
          //cout << "\n\n\t set_Input_In_Assembly(string p_Assembly" << p_Assembly << ", int p_Row " << p_Row << ", int p_Cell " << p_Cell << ", string p_Input " << p_Input << ")";

          int tmp_AID = str2int(get_Assembly_ID(p_Assembly));

          if (!bounds_AID(tmp_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(tmp_AID));
          int tmp_Index = str2int(get_Assembly_Index(tmp_AID));

          Assemblies[tmp_Tier] [tmp_Index]->set_Input(p_Row, p_Cell, p_Input);
     }
     void set_Input_In_Assembly(int p_AID, int p_Row, int p_Cell, string p_Input)
     {
          //cout << "\n\n\t set_Input_In_Assembly(itn p_AID" << p_AID << ", int p_Row " << p_Row << ", int p_Cell " << p_Cell << ", string p_Input " << p_Input << ")";

          if (!bounds_AID(p_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          Assemblies[tmp_Tier] [tmp_Index]->set_Input(p_Row, p_Cell, p_Input);
     }

     //Creates a hotlink based on assembly type.
     void create_Hotlink(string p_Assembly_From, string p_Construct_From, int p_Row_From, string p_Assembly_To)
     {
          int p_AFr = 0;
          int p_CFr = 0;
          //int p_ATo = str2int(get_Assembly_ID(p_Assembly_To));
          int p_AToTier = str2int(get_Assembly_Tier(p_Assembly_To));
          int p_AToIndex = str2int(get_Assembly_Index(p_Assembly_To));

          cout << "\n\t Creating Hotlink To_Tier->" << p_AToTier << " To_Index->" << p_AToIndex;

          //Gather the Assembly IDs to gather from.
          if (p_Assembly_From != "Gathering")
          {
               p_AFr = str2int(get_Assembly_ID(p_Assembly_From));
               p_CFr = str2int(get_Construct_ID(p_Assembly_From, p_Construct_From));

               cout << " From_Assembly->" << p_AFr << " From_Construct->" << p_CFr;
          }

          //If the assembly to gather from is the gathering then the gatherings input table is used.
          if (p_Assembly_From == "Gathering")
          {
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink(Input_Table->get_Table_ID(), p_Row_From, p_AFr, p_CFr, p_Row_From);
               return;
          }

          //If the Assembly is a MSC then create the link to the treetop table in the target construct.
          if (get_Assembly_Type(p_Assembly_To) == "MAC")
          {
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink(get_Assembly_Ref(p_AFr)->get_Construct_Treetop_Table_ID(p_CFr), 0, p_AFr, p_CFr, p_Row_From);
               return;
          }

          //If the Assembly is a MSC then create the link to the treetop table in the target construct.
          if (get_Assembly_Type(p_Assembly_To) == "MAC_FLAT")
          {
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink(get_Assembly_Ref(p_AFr)->get_Construct_Treetop_Table_ID(p_CFr), 0, p_AFr, p_CFr, p_Row_From);
               return;
          }

          //If the Assembly is a MSC then create the link to the treetop table in the target construct.
          if (get_Assembly_Type(p_Assembly_To) == "MAC_COLUMN")
          {
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink(get_Assembly_Ref(p_AFr)->get_Construct_Treetop_Table_ID(p_CFr), 0, p_AFr, p_CFr, p_Row_From);
               return;
          }
     }

     //Creates a hotlink based on assembly type.
     void create_Hotlink(int p_AFr, int p_CFr, int p_Row_From, int p_ATo)
     {
          string p_Assembly_From = get_Assembly_Name(p_AFr);
          string p_Construct_From = get_Construct_Name(p_AFr, p_CFr);
          string p_Assembly_To = get_Assembly_Name(p_ATo);

          //int p_ATo = str2int(get_Assembly_ID(p_Assembly_To));
          int p_AToTier = str2int(get_Assembly_Tier(p_Assembly_To));
          int p_AToIndex = str2int(get_Assembly_Index(p_Assembly_To));

          cout << "\n\t Creating Hotlink To_Tier->" << p_AToTier << " To_Index->" << p_AToIndex;

          //Gather the Assembly IDs to gather from.
          if (p_Assembly_From != "Gathering")
          {
               p_AFr = str2int(get_Assembly_ID(p_Assembly_From));
               p_CFr = str2int(get_Construct_ID(p_Assembly_From, p_Construct_From));

               cout << " From_Assembly->" << p_AFr << " From_Construct->" << p_CFr;
          }

          //If the assembly to gather from is the gathering then the gatherings input table is used.
          if (p_Assembly_From == "Gathering")
          {
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink(Input_Table->get_Table_ID(), p_Row_From, p_AFr, p_CFr, p_Row_From);
               return;
          }

          //If the Assembly is a MSC then create the link to the treetop table in the target construct.
          if (get_Assembly_Type(p_Assembly_To) == "MAC")
          {
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink(get_Assembly_Ref(p_AFr)->get_Construct_Treetop_Table_ID(p_CFr), 0, p_AFr, p_CFr, p_Row_From);
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink_Charges(get_Assembly_Ref(p_AFr)->get_Construct_Output_Table_Charge_ID(p_CFr), 0, p_AFr, p_CFr, p_Row_From);
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink_Nodes(get_Assembly_Ref(p_AFr)->get_Construct_Output_Table_Nodes_ID(p_CFr), 0, p_AFr, p_CFr, p_Row_From);
               return;
          }

          //If the Assembly is a MSC then create the link to the treetop table in the target construct.
          if (get_Assembly_Type(p_Assembly_To) == "MAC_FLAT")
          {
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink(get_Assembly_Ref(p_AFr)->get_Construct_Treetop_Table_ID(p_CFr), 0, p_AFr, p_CFr, p_Row_From);
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink_Charges(get_Assembly_Ref(p_AFr)->get_Construct_Output_Table_Charge_ID(p_CFr), 0, p_AFr, p_CFr, p_Row_From);
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink_Nodes(get_Assembly_Ref(p_AFr)->get_Construct_Output_Table_Nodes_ID(p_CFr), 0, p_AFr, p_CFr, p_Row_From);
               return;
          }

          //If the Assembly is a MSC then create the link to the treetop table in the target construct.
          if (get_Assembly_Type(p_Assembly_To) == "MAC_COLUMN")
          {
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink(get_Assembly_Ref(p_AFr)->get_Construct_Treetop_Table_ID(p_CFr), 0, p_AFr, p_CFr, p_Row_From);
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink_Charges(get_Assembly_Ref(p_AFr)->get_Construct_Output_Table_Charge_ID(p_CFr), 0, p_AFr, p_CFr, p_Row_From);
               Assemblies[p_AToTier] [p_AToIndex]->set_Hotlink_Nodes(get_Assembly_Ref(p_AFr)->get_Construct_Output_Table_Nodes_ID(p_CFr), 0, p_AFr, p_CFr, p_Row_From);
               return;
          }
     }



     //==--     ASSEMBLY INTERFACE MEMBERS

     //Resets the input tables for the gathering.
     void reset_Input()
     {
          Input_Table->reset_Table();

          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               for (int cou_Assembly=0;cou_Assembly<*Assembly_Count[cou_Tier];cou_Assembly++)
               {
                    Assemblies[cou_Tier] [cou_Assembly]->reset_Tables();
               }
          }
     }

     //Gathers the inputs based on the assemblies settings.
     string Gather_Inputs()
     {
          int tmp_Gathering_Input = 0;
          int tmp_Input_Count = 0;
          int tmp_Assembly_Input_Table = 0;

          //Input_Table->output_Table();

          //Gather the raw inputs.
          for (int cou_Index=0;cou_Index<*Assembly_Count[0];cou_Index++)
          {
               //cout << "\n\t Gathering Assembly Input->" << cou_Index << get_Assembly_Name(cou_Index);
               //cout << "\n\t Gathering input->" << cou_Index << " " << *Assembly_Count[0] << " " << Assemblies[0] [cou_Index];
               tmp_Gathering_Input = Assemblies[0] [cou_Index]->get_Setting_Int(2);
               tmp_Input_Count = Assemblies[0] [cou_Index]->get_Internal_Int(1) + 1;
               tmp_Assembly_Input_Table = Assemblies[0] [cou_Index]->get_Input_Table_ID();

               Tables->reset_Table(tmp_Assembly_Input_Table);

               //cout << "\n\t tmp_Gathering_Input->" << tmp_Gathering_Input << " tmp_Input_Count->" << tmp_Input_Count << " tmp_Assembly_Input_Table->" << tmp_Assembly_Input_Table;
               //cout << "\n\t tmp_Gathering_Input->" << tmp_Gathering_Input << " tmp_Input_Count->" << tmp_Input_Count << " tmp_Assembly_Input_Table->" << tmp_Assembly_Input_Table;

               for (int cou_Con=tmp_Gathering_Input;cou_Con<(tmp_Input_Count + tmp_Gathering_Input);cou_Con++)
               {
                    //cout << "\n\t Tables->copy_Row(Input_Table->get_Table_ID()~~" << Input_Table->get_Table_ID() << ", cou_Con~~" << cou_Con << ", tmp_Assembly_Input_Table~~" << tmp_Assembly_Input_Table << ", (cou_Con - tmp_Gathering_Input)~~" << (cou_Con - tmp_Gathering_Input) << ");";
                    //cout << "\n\t  [" << (cou_Con - tmp_Gathering_Input) << "]->" << Input_Table->row_Is_Empty((cou_Con - tmp_Gathering_Input));

                    if (Input_Table->row_Is_Empty((cou_Con - tmp_Gathering_Input))){ /*cout << "\n\t  ROW " << (cou_Con - tmp_Gathering_Input) << " NO INPUT!";*/ continue; }

                    Tables->copy_Row(Input_Table->get_Table_ID(), cou_Con, tmp_Assembly_Input_Table, (cou_Con - tmp_Gathering_Input));
               }
               //Tables->output_Table(tmp_Assembly_Input_Table);
          }

          //The upper tier assemblies are hotlinked to the constructs they draw from.
          //Each assembly hooks to different tables, one may hook to the nodes and charges, whereas another may hook to the Internals.

          //system("PAUSE > NULL");
          return "Inputs Possibly Gathered";
     }

     //Builds all
     string Build()
     {
          //The raw tier.
          //cout << "\n\t  Build....";

          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               //for (int cou_Index=0;cou_Index<*Assembly_Count[cou_Tier];cou_Index++){  cout << " Index[" << cou_Index << "]"; cout.flush(); Build(cou_Tier, cou_Index); }
               for (int cou_Index=0;cou_Index<*Assembly_Count[cou_Tier];cou_Index++){ Build(cou_Tier, cou_Index); }

          }
          return "Build";
     }

     //Builds all
     string Build_BM()
     {
          //The raw tier.
          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               for (int cou_Index=0;cou_Index<*Assembly_Count[cou_Tier];cou_Index++){ Build_BM(cou_Tier, cou_Index); }
          }
          return "Build_BM";
     }


     string Eval()
     {
          //The raw tier.
          //cout << "\n\t  Evaluating....";
          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               //cout << "\n\t     T[" << cou_Tier << "]";
               for (int cou_Index=0;cou_Index<*Assembly_Count[cou_Tier];cou_Index++)
               {
                    //cout << " I[" << cou_Index << "]"; cout.flush();
                    //cout << " " << Eval(cou_Tier, cou_Index);
                    Eval(cou_Tier, cou_Index);
                    //cout << " Eval_Complete...";

               }
          }

          //Output all of the tables.
          //Tables->output_All_Tables();
          return "Eval";
     }

     string Train()
     {
          //The raw tier.
          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               for (int cou_Index=0;cou_Index<*Assembly_Count[cou_Tier];cou_Index++){ Train(cou_Tier, cou_Index); }
          }
          return "Train";
     }

     //Build
     string Build(int p_AID){ if (!bAID(p_AID)){ return "BE_AID"; } return Build(get_Assembly_Name(p_AID)); }
     string Build(string p_AName){ return Assemblies[str2int(get_Assembly_Tier(p_AName))] [str2int(get_Assembly_Index(p_AName))]->Build(); }
     string Build(int p_Tier, int p_Index){ return Assemblies[p_Tier] [p_Index]->Build(); }

     //Build For Benchmarking
     string Build_BM(int p_AID){ if (!bAID(p_AID)){ return "BE_AID"; } return Build_BM (get_Assembly_Name(p_AID)); }
     string Build_BM(string p_AName){ return Assemblies[str2int(get_Assembly_Tier(p_AName))] [str2int(get_Assembly_Index(p_AName))]->Build_BM (); }
     string Build_BM(int p_Tier, int p_Index){ return Assemblies[p_Tier] [p_Index]->Build_BM (); }

     //Eval
     string Eval(int p_AID){ if (!bAID(p_AID)){ return "BE_AID"; } return Eval(get_Assembly_Name(p_AID)); }
     string Eval(string p_AName){ /*cout << "\n\t Eval(" << p_AName << ");";*/ return Assemblies[str2int(get_Assembly_Tier(p_AName))] [str2int(get_Assembly_Index(p_AName))]->Eval(); }
     string Eval(int p_Tier, int p_Index){ /*cout << "\n\t\t      Eval("; oint(0, 74, p_Tier); cout << ", "; oint(0, 74, p_Index); cout << ");";*/ return Assemblies[p_Tier] [p_Index]->Eval(); }

     //Train
     string Train(int p_AID){ if (!bAID(p_AID)){ return "BE_AID"; } return Train(get_Assembly_Name(p_AID)); }
     string Train(string p_AName){ return Assemblies[str2int(get_Assembly_Tier(p_AName))] [str2int(get_Assembly_Index(p_AName))]->Train(); }
     string Train(int p_Tier, int p_Index){ return Assemblies[p_Tier] [p_Index]->Train(); }



     //Adds a construct chain to the assembly.
     string Add_Raw_Construct(int p_AID, string p_CName, int p_Dimension = 1)
     {
          cout << "\n\t  p_AID->" << p_AID << " p_CName->" << p_CName << " p_Dimension->" << p_Dimension;
          if (!bAID(p_AID)){ return "BE_AID"; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          cout << "\n\t   Creating construct-> Assemblies[" << tmp_Tier << "] [" << tmp_Index << "]";
          return Assemblies[tmp_Tier] [tmp_Index]->Add_Raw_Construct(p_CName, p_Dimension);
     }
     string Add_Raw_Construct(string p_AName, string p_CName, int p_Dimension = 1)
     {
          return Add_Raw_Construct(str2int(get_Assembly_ID(p_AName)), p_CName, p_Dimension);
     }

     //Adds a construct chain to the assembly.
     string Add_Raw_Int_Construct(int p_AID, string p_CName, int p_Dimension = 1)
     {
          cout << "\n\t  p_AID->" << p_AID << " p_CName->" << p_CName << " p_Dimension->" << p_Dimension;
          if (!bAID(p_AID)){ return "BE_AID"; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          cout << "\n\t   Creating raw int construct-> Assemblies[" << tmp_Tier << "] [" << tmp_Index << "]";
          return Assemblies[tmp_Tier] [tmp_Index]->Add_Raw_Int_Construct(p_CName, p_Dimension);
     }
     string Add_Raw_Int_Construct(string p_AName, string p_CName, int p_Dimension = 1)
     {
          return Add_Raw_Int_Construct(str2int(get_Assembly_ID(p_AName)), p_CName, p_Dimension);
     }

     //Adds a construct chain to the assembly.
     string Add_Raw_Float_Construct(int p_AID, string p_CName, int p_Dimension = 1)
     {
          cout << "\n\t  p_AID->" << p_AID << " p_CName->" << p_CName << " p_Dimension->" << p_Dimension;
          if (!bAID(p_AID)){ return "BE_AID"; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          cout << "\n\t   Creating raw float construct-> Assemblies[" << tmp_Tier << "] [" << tmp_Index << "]";
          return Assemblies[tmp_Tier] [tmp_Index]->Add_Raw_Float_Construct(p_CName, p_Dimension);
     }
     string Add_Raw_Float_Construct(string p_AName, string p_CName, int p_Dimension = 1)
     {
          return Add_Raw_Float_Construct(str2int(get_Assembly_ID(p_AName)), p_CName, p_Dimension);
     }

     //--     CONSTRUCT INTERFACE

     //Creates a construct.
     void * create_Construct(int p_AID, string p_Construct_Name, int p_Type)
     {
          if (!bounds_AID(p_AID)){ return NULL; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          return Assemblies[tmp_Tier] [tmp_Index]->create_Construct(p_Construct_Name, p_Type);
     }
     void * create_Construct(string p_Assembly, string p_Construct_Name, int p_Type)
     {
          //cout << "\n\n\t create_Construct(string p_Assembly, string p_Construct_Name, int p_Type)";

          int tmp_AID = str2int(get_Assembly_ID(p_Assembly));

          if (!bounds_AID(tmp_AID)){ return NULL; }

          int tmp_Tier = str2int(get_Assembly_Tier(tmp_AID));
          int tmp_Index = str2int(get_Assembly_Index(tmp_AID));

          return Assemblies[tmp_Tier] [tmp_Index]->create_Construct(p_Construct_Name, p_Type);
     }

     //Deletes a construct from the assembly.
     void delete_Construct(int p_AID, int p_CID)
     {
          if (!bounds_AID(p_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          Assemblies[tmp_Tier] [tmp_Index]->delete_Construct(p_CID);
     }
     void delete_Construct(string p_Assembly, string p_Construct)
     {
          //cout << "\n\n\t void delete_Construct(string p_Assembly, string p_Construct)";

          int tmp_AID = str2int(get_Assembly_ID(p_Assembly));

          if (!bounds_AID(tmp_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(tmp_AID));
          int tmp_Index = str2int(get_Assembly_Index(tmp_AID));

          Assemblies[tmp_Tier] [tmp_Index]->delete_Construct(p_Construct);
     }


     //Sets the name of a construct.
     void set_Construct_Name(int p_AID, int p_CID, string p_Name)
     {
          if (!bounds_AID(p_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          Assemblies[tmp_Tier] [tmp_Index]->set_Construct_Name(p_CID, p_Name);
     }
     void set_Construct_Name(string p_Assembly, string p_Construct, string p_Name)
     {
          //cout << "\n\n\t void set_Construct_Name(string p_Assembly, string p_Construct, string p_Name)";

          set_Construct_Name(str2int(get_Assembly_ID(p_Assembly)), str2int(get_Construct_ID(p_Assembly, p_Construct)), p_Name);
     }

     //Gets the name of a construct.
     string get_Construct_Name(int p_AID, int p_CID)
     {
          if (!bounds_AID(p_AID)){ return "0"; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          return Assemblies[tmp_Tier] [tmp_Index]->get_Construct_Name(p_CID);
     }
     string get_Construct_Name(string p_Assembly, int p_CID)
     {
          ////cout << "\n\n\t string get_Construct_Name(string p_Assembly, int p_CID)";

          return get_Construct_Name(str2int(get_Assembly_ID(p_Assembly)), p_CID);
     }

     //Gets the ID of a construct.
     string get_Construct_ID(int p_AID, string p_Construct)
     {
          if (!bounds_AID(p_AID)){ return "0"; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          return Assemblies[tmp_Tier] [tmp_Index]->get_Construct_ID(p_Construct);
     }
     string get_Construct_ID(string p_Assembly, string p_Construct)
     {
          ////cout << "\n\n\t get_Construct_ID(" << p_Assembly << ", " << p_Construct << ")";

          return get_Construct_ID(str2int(get_Assembly_ID(p_Assembly)), p_Construct);
     }

     //Gets whether or not a construct exists.
     string exists_Construct(int p_AID, string p_Construct)
     {
          if (!bounds_AID(p_AID)){ return "0"; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          return Assemblies[tmp_Tier] [tmp_Index]->exists_Construct(p_Construct);
     }
     string exists_Construct(string p_AName, string p_Construct)
     {
          return exists_Construct(str2int(get_Assembly_ID(p_AName)), p_Construct);
     }

     //Submit an input to the input table.
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -"INPUT"
     //   0            1          2          3       4      5
     void push_Input_To_Construct(string p_Assembly, string p_Construct, int p_Row, string p_Input)
     {
          ////cout << "\n\n\t push_Input_To_Construct(string p_Assembly, string p_Construct, int p_Row, string p_Input)";

          int tmp_AID = str2int(get_Assembly_ID(p_Assembly));

          if (!bounds_AID(tmp_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(tmp_AID));
          int tmp_Index = str2int(get_Assembly_Index(tmp_AID));

          Assemblies[tmp_Tier] [tmp_Index]->push_Input_To_Construct(p_Construct, p_Row, p_Input);
     }
     void push_Input_To_Construct(int p_Assembly, int p_Construct, int p_Row, string p_Input)
     {
          ////cout << "\n\n\t push_Input_To_Construct(int p_Assembly, int p_Construct, int p_Row, string p_Input)";

          push_Input_To_Construct(get_Assembly_Name(p_Assembly), get_Construct_Name(p_Assembly, p_Construct), p_Row, p_Input);
     }

     //Submit an input to the input table.
     //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -CELL -"INPUT"
     //   0            1          2          3       4    5       6
     void set_Input_In_Construct(string p_Assembly, string p_Construct, int p_Row, int p_Cell, string p_Input)
     {
          ////cout << "\n\n\t void set_Input_In_Construct(string p_Assembly, string p_Construct, int p_Row, int p_Cell, string p_Input)";

          int tmp_AID = str2int(get_Assembly_ID(p_Assembly));

          if (!bounds_AID(tmp_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(tmp_AID));
          int tmp_Index = str2int(get_Assembly_Index(tmp_AID));

          Assemblies[tmp_Tier] [tmp_Index]->set_Input_In_Construct(p_Construct, p_Row, p_Cell, p_Input);
     }
     void set_Input_In_Construct(int p_Assembly, int p_Construct, int p_Row, int p_Cell, string p_Input)
     {
          set_Input_In_Construct(get_Assembly_Name(p_Assembly), get_Construct_Name(p_Assembly, p_Construct), p_Row, p_Cell, p_Input);
     }


     //Reads in the NID as a query, then charges using the charges in the charge table.
     void eval_Foreign_Output_Table(string p_Assembly, string p_Construct, string p_Output_Table_Nodes, string p_Output_Table_Charges)
     {

          int tmp_AID = str2int(get_Assembly_ID(p_Assembly));

          if (!bounds_AID(tmp_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(tmp_AID));
          int tmp_Index = str2int(get_Assembly_Index(tmp_AID));

          Assemblies[tmp_Tier] [tmp_Index]->eval_Foreign_Output_Table(p_Construct, p_Output_Table_Nodes, p_Output_Table_Charges);
     }
     void eval_Foreign_Output_Table(int p_AID, int p_CID, int p_Output_Table_Nodes_ID, int p_Output_Table_Charges_ID)
     {
          if (!bounds_AID(p_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          Assemblies[tmp_Tier] [tmp_Index]->eval_Foreign_Output_Table(p_CID, p_Output_Table_Nodes_ID, p_Output_Table_Charges_ID);
     }

     //Evals a given construct with the given build method.
     //construct submit_input -ASSEMBLY -CONSTRUCT -BUILDING METHOD
     //   0            1          2          3               4
     void eval_Construct(string p_Assembly, string p_Construct, string p_Method)
     {
          ////cout << "\n\n\t eval_Construct(string p_Assembly, string p_Construct, string p_Method)";

          int tmp_AID = str2int(get_Assembly_ID(p_Assembly));

          if (!bounds_AID(tmp_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(tmp_AID));
          int tmp_Index = str2int(get_Assembly_Index(tmp_AID));

          Assemblies[tmp_Tier] [tmp_Index]->eval_Construct(p_Construct, p_Method);
     }
     void eval_Construct(int p_Assembly, int p_Construct, string p_Method)
     {
          ////cout << "\n\n\t eval_Construct(int p_Assembly, int p_Construct, string p_Method)";
          eval_Construct(get_Assembly_Name(p_Assembly), get_Construct_Name(p_Assembly, p_Construct), p_Method);
     }

     //Builds a given construct with the given build method.
     //construct submit_input -ASSEMBLY -CONSTRUCT -BUILDING METHOD
     //   0            1          2          3               4
     void build_Construct(string p_Assembly, string p_Construct, string p_Method)
     {
          ////cout << "\n\n\t void build_Construct(string p_Assembly, string p_Construct, string p_Method)";

          int tmp_AID = str2int(get_Assembly_ID(p_Assembly));

          if (!bounds_AID(tmp_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(tmp_AID));
          int tmp_Index = str2int(get_Assembly_Index(tmp_AID));

          Assemblies[tmp_Tier] [tmp_Index]->build_Construct(p_Construct, p_Method);
     }
     void build_Construct(int p_AID, int p_CID, string p_Method)
     {

          if (!bounds_AID(p_AID)){ return; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));

          Assemblies[tmp_Tier] [tmp_Index]->build_Construct(p_CID, p_Method);
     }

     //Gets the assembly ID of a construct based on the given CID. Uses brute force searching.
     int get_Construct_Parent(int p_CID)
     {
          //cout << "\n\t    ";
          //ostr(0, 14, "get_Construct_Parent(");
          //oint(0, 8, p_CID);
          //ostr(0, 14, ")");
          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               //cout << "\n\t    Tier->";
               //oint(0, 15, cou_Tier);
               for (int cou_A=0;cou_A<*Assembly_Count[cou_Tier];cou_A++)
               {
                    //cout << "\n\t    Assembly->";
                    //oint(0, 11, cou_A);
                    if (Assemblies[cou_Tier] [cou_A]->does_Assembly_Contain_Construct(p_CID))
                    {
                         //cout << "\n\t    AID->";
                         //oint(0, 12, Assemblies[cou_Tier] [cou_A]->get_Assembly_ID());
                         //cout << "\n\n";
                         return Assemblies[cou_Tier] [cou_A]->get_Assembly_ID();
                    }
               }
          }
          //cout << "\n\n";
          return -1;
     }


     //--     ARRAY MANAGMENT FUNCTIONS

     //Expands the assembly array by one.
     void expand_Assemblies(int p_Tier)
     {
          expand_Assemblies(*Assembly_Count[p_Tier] + 1, p_Tier);
     }

     //Expands the assembly array to the given dimensions keeping the current assemblies intact. After this is called the new indexes will be
     //NULL and will have to be created separately.
     void expand_Assemblies(int p_Assembly_Count, int p_Tier)
     {
          //cout << "\n\t expanding the assembly tier.";
          //system("PAUSE > NULL");
          expand_Assembly_Tier(p_Tier + 1);

          //cout << "\n\t checking if the expansion is neccesary";
          //system("PAUSE > NULL");
          //Defeat the enemy known as unecessary array copying and reallocation followed by more copying and deletion.
          if (p_Assembly_Count <= *Assembly_Count[p_Tier]){ return; }

          //cout << "\n\t creating tmp_Assemblies.";
          //system("PAUSE > NULL");
          //Setup the temporary array to hold the current assemblies.
          c_Base_Assembly ** tmp_Assemblies;
          tmp_Assemblies = new c_Base_Assembly*[*Assembly_Count[p_Tier]];

          //cout << "\n\t copy old tier into tmp tier.";
          //system("PAUSE > NULL");
          //Copy the old assemblies into the temporary array nullifying the old array as it goes.
          for (int cou_Index=0;cou_Index<*Assembly_Count[p_Tier];cou_Index++)
          {
               tmp_Assemblies[cou_Index] = Assemblies[p_Tier] [cou_Index];
               Assemblies[p_Tier] [cou_Index] = NULL;
          }


          //cout << "\n\t delete the old assembly tier.";
          //system("PAUSE > NULL");
          //Delete the Assemblies array and reallocate it to the appropriate dimensions.
          delete [] Assemblies[p_Tier];
          Assemblies[p_Tier] = NULL;

          //cout << "\n\t recreate the tier.";
          //system("PAUSE > NULL");
          Assemblies[p_Tier] = new c_Base_Assembly*[p_Assembly_Count];

          //cout << "\n\t copy the tmp tier back into the old tier.";
          //system("PAUSE > NULL");
          //Copy the old assembly addresses into the reallocated assemblies array.
          for (int cou_Index=0;cou_Index<*Assembly_Count[p_Tier];cou_Index++)
          {
               Assemblies[p_Tier] [cou_Index] = tmp_Assemblies[cou_Index];
               tmp_Assemblies[cou_Index] = NULL;
          }

          //cout << "\n\t delete the tmp";
          //system("PAUSE > NULL");
          //Cleanup the temporary array to avoid memory leaks.
          delete [] tmp_Assemblies;
          tmp_Assemblies = NULL;

          //cout << "\n\t ini the new tier nodes to NULL";
          //system("PAUSE > NULL");
          //Initialize the newly allocated portion of the assemblies array to NULL.
          for (int cou_Index=*Assembly_Count[p_Tier];cou_Index<p_Assembly_Count;cou_Index++)
          {
               Assemblies[p_Tier] [cou_Index] = NULL;
          }

          //cout << "\n\t set the new count.";
          //system("PAUSE > NULL");
          //Set the assembly count to the appropriate amount.
          *Assembly_Count[p_Tier] = p_Assembly_Count;
          /*
          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               cout << "\n\t Assembly_Count[" << cou_Tier << "]->" << *Assembly_Count[cou_Tier];
               for (int cou_Index=0;cou_Index<*Assembly_Count[cou_Tier];cou_Index++)
               {
                    cout << "\n\t     Assemblies[" << cou_Tier << "] [" << cou_Index << "]->" << Assemblies[cou_Tier] [cou_Index];
               }
          }
          */
          return;
     }

     //Expands the assembly structure.
     void expand_Assembly_Tier(int p_Tier)
     {
          if (p_Tier <= Assembly_Height){ return; }

          //Setup the temporary array to hold the current assemblies.
          c_Base_Assembly *** tmp_Assemblies = new c_Base_Assembly**[Assembly_Height];
          int ** tmp_Assembly_Count = new int*[Assembly_Height];

          //Copy the old assemblies into the new construct.
          for (int cou_Index=0;cou_Index<Assembly_Height;cou_Index++)
          {
               tmp_Assemblies[cou_Index] = Assemblies[cou_Index];
               Assemblies[cou_Index] = NULL;

               tmp_Assembly_Count[cou_Index] = Assembly_Count[cou_Index];
               Assembly_Count[cou_Index] = NULL;
          }

          //Reallocate the assemblies.
          delete [] Assemblies;
          Assemblies = NULL;

          delete [] Assembly_Count;
          Assembly_Count = NULL;

          Assemblies = new c_Base_Assembly**[p_Tier];
          Assembly_Count = new int*[p_Tier];

          //Allocate the newly minted tiers to NULL.
          for (int cou_Index=Assembly_Height;cou_Index<p_Tier;cou_Index++)
          {
               Assemblies[cou_Index] = NULL;
               Assembly_Count[cou_Index] = new int;
               *Assembly_Count[cou_Index] = 0;
          }

          //Copy the old tiers back into the assembly.
          for (int cou_Index=0;cou_Index<Assembly_Height;cou_Index++)
          {
               Assemblies[cou_Index] = tmp_Assemblies[cou_Index];
               tmp_Assemblies[cou_Index] = NULL;

               Assembly_Count[cou_Index] = tmp_Assembly_Count[cou_Index];
               tmp_Assembly_Count[cou_Index] = NULL;
          }

          //Set the height.
          Assembly_Height = p_Tier;

          //Cleanup.
          delete [] tmp_Assemblies;
          tmp_Assemblies = NULL;
          delete [] tmp_Assembly_Count;
          tmp_Assembly_Count = NULL;
     }

     //Resets the assembly array.
     void reset_assemblies()
     {
          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               for (int cou_Assembly=0;cou_Assembly<*Assembly_Count[cou_Tier];cou_Assembly++)
               {
                    delete Assemblies[cou_Tier] [cou_Assembly];
                    Assemblies[cou_Tier] [cou_Assembly] = NULL;
               }
               delete Assemblies[cou_Tier];
               Assemblies[cou_Tier] = NULL;
          }
          delete [] Assemblies;
          Assemblies = NULL;
          Assembly_Count = 0;
     }

     //--     CHECKS FOR BOUNDING ERRORS
     bool bounds_AID(int p_AID)
     {
          if (p_AID < 0){ cout << "\n\t BOUNDING_ERR < 0."; return 0; }
          if (p_AID >= Assembly_Total_Count){ cout << "\n\t BOUNDING_ERR > ATC."; return 0; }

          int tmp_Tier = str2int(get_Assembly_Tier(p_AID));
          int tmp_Index = str2int(get_Assembly_Index(p_AID));
          cout << "\n\t tmp_Tier->" << tmp_Tier << " tmp_Index->" << tmp_Index;

          if (tmp_Tier >= Assembly_Height){ cout << "\n\t BOUNDING_ERR >= A_H"; return 0; }
          if (tmp_Index >= *Assembly_Count[tmp_Tier]){ cout << "\n\t BOUNDING_ERR >= AC[tmp_Tier]"; return 0; }

          if (Assemblies[tmp_Tier] == NULL){ cout << "\n\t NULL_ERR >TIER";return 0; }
          if (Assemblies[tmp_Tier] [tmp_Index] == NULL){ cout << "\n\t NULL_ERR >INDEX"; return 0; }
          return 1;
     }
     bool bounds_AID(string p_Assembly)
     {
          int tmp_AID = str2int(get_Assembly_ID(p_Assembly));

          if (tmp_AID < 0){ return 0; }
          if (tmp_AID >= Assembly_Total_Count){ return 0; }

          int tmp_Tier = str2int(get_Assembly_Tier(tmp_AID));
          int tmp_Index = str2int(get_Assembly_Index(tmp_AID));

          if (Assemblies[tmp_Tier] == NULL){ return 0; }
          if (Assemblies[tmp_Tier] [tmp_Index] == NULL){ return 0; }
          return 1;
     }

     bool bAID(string p_Assembly){ return bounds_AID(p_Assembly); }
     bool bAID(int p_AID){ return bounds_AID(p_AID); }

     //--     OUTPUT FUNCTIONS

     //Outputs a given Assembly.
     void output_given_Assembly(int p_Tier, int p_Index)
     {
          if (Assemblies[p_Tier] [p_Index] == NULL){ return; }
          Assemblies[p_Tier] [p_Index]->output_Assembly();
     }

     //Outputs all the Assemblies
     void output_Every_Assembly()
     {
          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               for (int cou_Assembly=0;cou_Assembly<*Assembly_Count[cou_Tier];cou_Assembly++)
               {
                    cout << "\n";
                    output_given_Assembly(cou_Tier, cou_Assembly);
               }
          }
     }

     //Outputs the tables containted within.
     void output_Tables()
     {
          Input_Table->output_Table();
          Output_Table_Charge->output_Table();
          Output_Table_Pattern->output_Table();
     }



     //Outputs the gathering.
     void output_Gathering()
     {
          cout << "\n\n\t----------";
          cout << "\n\t Gathering->" << Gathering_Name << this;
          cout << "\n\t   Assembly_Count->" << Assembly_Count;
          output_Tables();
          output_Every_Assembly();
     }

     //==--     OUTPUT FUNCTIONS FOR THE CONSTRUCTS

	//Outputs the given CAN.
     void output_given_CAN(int p_AID, int p_CID, int p_CAN){ if (!bounds_AID(p_AID)){ return; } get_Assembly_Ref(p_AID)->output_given_CAN(p_CID, p_CAN); }
     void output_given_CAN(string p_Assembly, string p_Construct, int p_CAN){ output_given_CAN(str2int(get_Assembly_ID(p_Assembly)), str2int(get_Construct_ID(p_Assembly, p_Construct)), p_CAN); }

     //Outputs all the CANs
     void output_Every_CAN(int p_AID, int p_CID){ if (!bounds_AID(p_AID)){ return; } get_Assembly_Ref(p_AID)->output_Every_CAN(p_CID); }
     void output_Every_CAN(string p_Assembly, string p_Construct){ output_Every_CAN(str2int(get_Assembly_ID(p_Assembly)), str2int(get_Construct_ID(p_Assembly, p_Construct))); }

     //Output Node Network.
     void output_Node_Network(int p_AID, int p_CID){ if (!bounds_AID(p_AID)){ return; } get_Assembly_Ref(p_AID)->output_Node_Network(p_CID); }
     void output_Node_Network(string p_Assembly, string p_Construct){ output_Node_Network(str2int(get_Assembly_ID(p_Assembly)), str2int(get_Construct_ID(p_Assembly, p_Construct))); }

     //Outputs the tables containted within.
     void output_Construct_Tables(int p_AID, int p_CID){ if (!bounds_AID(p_AID)){ return; } get_Assembly_Ref(p_AID)->output_Construct_Tables(p_CID); }
     void output_Construct_Tables(string p_Assembly, string p_Construct){ output_Construct_Tables(str2int(get_Assembly_ID(p_Assembly)), str2int(get_Construct_ID(p_Assembly, p_Construct))); }

     //Outputs the construct internals.
     void output_Construct_Internals(int p_AID, int p_CID){ if (!bounds_AID(p_AID)){ return; } get_Assembly_Ref(p_AID)->output_Construct_Internals(p_CID); }
     void output_Construct_Internals(string p_Assembly, string p_Construct){ output_Construct_Internals(str2int(get_Assembly_ID(p_Assembly)), str2int(get_Construct_ID(p_Assembly, p_Construct)));
     }

     //Outputs the construct.
     void output_Construct_Full(int p_AID, int p_CID){ if (!bounds_AID(p_AID)){ return; } get_Assembly_Ref(p_AID)->output_Construct_Full(p_CID); }
     void output_Construct_Full(string p_Assembly, string p_Construct){ output_Construct_Full(str2int(get_Assembly_ID(p_Assembly)), str2int(get_Construct_ID(p_Assembly, p_Construct))); }

     //Outputs the stats for the node networks.
     void output_Node_Network_Stats()
     {

          //Setup the temporary variables.
          c_Base_Assembly * tmp_Assembly = NULL;
          c_Base_Construct * tmp_Construct = NULL;

          //Loop through all constructs gathering the state coordinates.
          for (int cou_ATier=0;cou_ATier<Assembly_Height;cou_ATier++)
          {
               for (int cou_Assembly=0;cou_Assembly<*Assembly_Count[cou_ATier];cou_Assembly++)
               {
                    tmp_Assembly = Assemblies[cou_ATier] [cou_Assembly];

                    for (int cou_Construct=0;cou_Construct<tmp_Assembly->Construct_Count;cou_Construct++)
                    {
                         tmp_Construct = tmp_Assembly->Constructs[cou_Construct];
                         tmp_Construct->output_Node_Network_Stats();
                    }
               }
          }
     }

     //==--    OUTPUTS THE CURRENT DEBUG INFO

     void output_Debug()
     {
          Input_Table->output_Table();
          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               for (int cou_Index=0;cou_Index<*Assembly_Count[cou_Tier];cou_Index++)
               {
                    Assemblies[cou_Tier] [cou_Index]->output_Debug();
               }
          }
     }

     string get_Stats()
     {
          string tmp_Stats = "";
          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               for (int cou_Index=0;cou_Index<*Assembly_Count[cou_Tier];cou_Index++)
               {
                    tmp_Stats += Assemblies[cou_Tier] [cou_Index]->get_Stats();
               }
          }
          return tmp_Stats;
     }

     //==--     SAVING AND LOADING

     //Save
     void Save(int p_Ticket_Number)
     {
          //Setup the temporary variables.
          c_Base_Assembly * tmp_Assembly = NULL;
          c_Base_Construct * tmp_Construct = NULL;
          c_Base_Node_Network_1D * tmp_NN = NULL;
          int tmp_CID = 0;
          int tmp_U_CID = 0;
          u_Data tmp_State;
          int tmp_AID = 0;

          c_Base_Assembly * tmp_Assembly_Target = NULL;
          c_Base_Construct * tmp_Construct_Target = NULL;
          c_Base_Node_Network_1D * tmp_NNT = NULL;
          c_Base_Node_1D * tmp_TN = NULL;


          cout << "\n\n\t Saving Gathering->" << Gathering_Name;
          //Gathers the directory and creates it if need be.
          string tmp_Dir = "Save\\" + Gathering_Name;
          if (p_Ticket_Number != -1){ tmp_Dir += "." + int2str(p_Ticket_Number); }
          string tmp_MD = "md " + tmp_Dir;

          cout << "\n\n\t->" << tmp_Dir;
          system(tmp_MD.c_str());

          //Create the debug file.
          ofstream DBug;
          string tmp_FN = tmp_Dir + "\\Save_Log.dat";
          DBug.open(tmp_FN.c_str());

          //Gathers the file to save the assembly to.
          ofstream SF;
          tmp_FN = tmp_Dir + "\\" + Gathering_Name + ".gth";
          cout << "\n\t tmp_FN->" << tmp_FN;
          SF.open(tmp_FN.c_str());

          //Save the gathering information.
          SF << Gathering_Name;
          SF << "\n" << Assembly_Height;
          for (int cou_Index=0;cou_Index<Assembly_Height;cou_Index++)
          {
               SF << "\n" << *Assembly_Count[cou_Index];
          }

          //Loop through all constructs gathering the state coordinates.
          cout << "\n\t Number Of ATiers->" << Assembly_Height;
          DBug << "\n\t Number Of ATiers->" << Assembly_Height;
          for (int cou_ATier=0;cou_ATier<Assembly_Height;cou_ATier++)
          {
               cout << "\n\t  ATier[" << cou_ATier << "]->Depth->" << *Assembly_Count[cou_ATier];
               DBug << "\n\t  ATier[" << cou_ATier << "]->Depth->" << *Assembly_Count[cou_ATier];
               for (int cou_Assembly=0;cou_Assembly<*Assembly_Count[cou_ATier];cou_Assembly++)
               {
                    tmp_Assembly = Assemblies[cou_ATier] [cou_Assembly];
                    cout << "\n\t   A[" << cou_Assembly << "]->" << tmp_Assembly;
                    DBug << "\n\t   A[" << cou_Assembly << "]->" << tmp_Assembly;

                    for (int cou_Construct=0;cou_Construct<tmp_Assembly->Construct_Count;cou_Construct++)
                    {
                         tmp_Construct = tmp_Assembly->Constructs[cou_Construct];
                         cout << "\n\t    C[" << cou_Construct << "]->" << tmp_Construct;
                         DBug << "\n\t     Construct_Type->" << tmp_Construct->get_Input_Type();

                         cout << "\n\t    C[" << cou_Construct << "]->" << tmp_Construct;
                         DBug << "\n\t     Construct_Type->" << tmp_Construct->get_Input_Type();

                         //If the construct is of the MSC type then loop through each node gathering the reference from
                         //the other constructs using the CID contained in the last two digits of the treetop state.
                         if (tmp_Construct->get_Input_Type() == 1)
                         {
                              tmp_NN = tmp_Construct->get_Node_Network_Reference();
                              //cout << "\n\t      tmp_NN->" << tmp_NN;
                              DBug << "\n\t      tmp_NN->" << tmp_NN;

                              tmp_Construct->inflate_State_Coordinates();

                              //Loop through the state tier using the dereferenced node network node count.
                              for (int cou_Index=0;cou_Index<*(tmp_NN->Node_Count[0]);cou_Index++)
                              {
                                   //The problem is I am gathering the state, however, the state is only the node reference.
                                   //In order to get the contruct U_CID I need to get the treetop, if done using brute force
                                   // I will have already gathered the U_CID. Typecasting as a base node may work.
                                   tmp_State.NR = NULL;

                                   tmp_State = (tmp_NN->Nodes[0] [cou_Index]->get_State());
                                   //cout << "\n\t      tmp_State->" << tmp_State.I;
                                   DBug << "\n\n\t      tmp_State->" << tmp_State.I;
                                   DBug << "\n\t       tmp_State.NR->" << tmp_State.NR;
                                   tmp_U_CID = gl2di(tmp_State.I);
                                   /*
                                   tmp_State = (tmp_NN->Nodes[0] [cou_Index]->get_State());
                                   if (tmp_State.I == 0){ continue; }
                                   //cout << "\n\t      tmp_State->" << tmp_State.I;
                                   c_Base_Node_1D * tmp_TNode = reinterpret_cast<c_Base_Node_1D*>(tmp_State.NR);
                                   //cout << "\n\t      tmp_TN->" << ((tmp_TNode)->get_Treetop()).I;*/
                                   //tmp_CID = gl2di(tmp_State.I);

                                   //cout << "\n\t      tmp_U_CID->" << tmp_U_CID;
                                   DBug << "\n\t      tmp_U_CID->" << tmp_U_CID;

                                   tmp_AID = get_Construct_Parent(tmp_U_CID);
                                   //cout << "\n\t      tmp_AID->" << tmp_AID;
                                   DBug << "\n\t      tmp_AID->" << tmp_AID;

                                   if (tmp_AID != -1)
                                   {
                                        tmp_Assembly_Target = get_Assembly_Ref(tmp_AID);
                                        //cout << "\n\t       tmp_Assembly_Target->" << tmp_Assembly_Target;
                                        DBug << "\n\t       tmp_Assembly_Target->" << tmp_Assembly_Target; DBug.flush();

                                        tmp_CID = tmp_Assembly_Target->get_Construct_Index(tmp_U_CID);
                                        //cout << "\n\t       tmp_CID->" << tmp_CID;
                                        DBug << "\n\t       tmp_CID->" << tmp_CID; DBug.flush();

                                        tmp_Construct_Target = tmp_Assembly_Target->Constructs[tmp_CID];
                                        //cout << "\n\t       tmp_Construct_Target->" << tmp_Construct_Target;
                                        DBug << "\n\t       tmp_Construct_Target->" << tmp_Construct_Target;

                                        tmp_NNT = tmp_Construct_Target->get_Node_Network_Reference();
                                        //cout << "\n\t       tmp_NNT->" << tmp_NNT;
                                        DBug << "\n\t       tmp_NNT->" << tmp_NNT; DBug.flush();

                                        tmp_TN = tmp_NNT->get_Treetop_Node_Reference(tmp_State);
                                        //cout << "\n\t       tmp_TN->" << tmp_TN;
                                        DBug << "\n\t       tmp_TN->" << tmp_TN; DBug.flush();

                                        tmp_NN->tmp_State_Coordinates[0] [cou_Index] = tmp_CID;
                                        //cout << "\n\t       tmp_NN->tmp_State_Coordinates[0] [" << cou_Index << "]->" << tmp_NN->tmp_State_Coordinates[0] [cou_Index];
                                        DBug << "\n\t       tmp_NN->tmp_State_Coordinates[0] [" << cou_Index << "]->" << tmp_NN->tmp_State_Coordinates[0] [cou_Index]; DBug.flush();
                                        tmp_NN->tmp_State_Coordinates[1] [cou_Index] = tmp_TN->get_Tier();
                                        //cout << "\n\t       tmp_NN->tmp_State_Coordinates[1] [" << cou_Index << "]->" << tmp_NN->tmp_State_Coordinates[1] [cou_Index];
                                        DBug << "\n\t       tmp_NN->tmp_State_Coordinates[1] [" << cou_Index << "]->" << tmp_NN->tmp_State_Coordinates[1] [cou_Index]; DBug.flush();
                                        tmp_NN->tmp_State_Coordinates[2] [cou_Index] = tmp_TN->get_Index();
                                        //cout << "\n\t       tmp_NN->tmp_State_Coordinates[2] [" << cou_Index << "]->" << tmp_NN->tmp_State_Coordinates[2] [cou_Index];
                                        DBug << "\n\t       tmp_NN->tmp_State_Coordinates[2] [" << cou_Index << "]->" << tmp_NN->tmp_State_Coordinates[2] [cou_Index]; DBug.flush();
                                   }
                              }
                         }
                    }
               }
          }

          //Now that the State node coordinates are gathered the assemblies can be saved.
          cout << "\n\t Saving The Assemblies...";
          cout << "\n\t Number Of ATiers->" << Assembly_Height;
          DBug << "\n\t Saving The Assemblies...";
          DBug << "\n\t Number Of ATiers->" << Assembly_Height;
          for (int cou_ATier=0;cou_ATier<Assembly_Height;cou_ATier++)
          {
               cout << "\n\t  ATier[" << cou_ATier << "]->Depth->" << *Assembly_Count[cou_ATier];
               DBug << "\n\t  ATier[" << cou_ATier << "]->Depth->" << *Assembly_Count[cou_ATier];
               for (int cou_Assembly=0;cou_Assembly<*Assembly_Count[cou_ATier];cou_Assembly++)
               {
                    SF << "\n" << Assemblies[cou_ATier] [cou_Assembly]->get_Assembly_Name();
                    SF << "\n" << Assemblies[cou_ATier] [cou_Assembly]->get_Assembly_Type();
                    SF << "\n" << cou_ATier;
                    SF << "\n" << Assemblies[cou_ATier] [cou_Assembly]->Construct_Count;

                    for (int cou_Con=0;cou_Con<Assemblies[cou_ATier] [cou_Assembly]->Construct_Count;cou_Con++)
                    {
                         SF << "\n" << Assemblies[cou_ATier] [cou_Assembly]->Constructs[cou_Con]->get_Construct_Name();
                    }
                    Assemblies[cou_ATier] [cou_Assembly]->Save(tmp_Dir);
               }
          }

          SF.close();

          int tmp_Assembly_From = 0;
          int tmp_Construct_From = 0;
          int tmp_Row_From = 0;
          c_Table_1D * tmp_Table;

          //Gathers the file to save the hotlinks to. They will be loaded last.
          tmp_FN = tmp_Dir + "\\" + Gathering_Name + ".htk";
          cout << "\n\t tmp_FN->" << tmp_FN;
          DBug << "\n\t tmp_FN->" << tmp_FN;
          SF.open(tmp_FN.c_str());

          //Loop through all assemblies gathering the hotlinks.
          cout << "\n\t Number Of ATiers->" << Assembly_Height;
          DBug << "\n\t Number Of ATiers->" << Assembly_Height;
          for (int cou_ATier=0;cou_ATier<Assembly_Height;cou_ATier++)
          {
               cout << "\n\t  ATier[" << cou_ATier << "]->Depth->" << *Assembly_Count[cou_ATier];
               DBug << "\n\t  ATier[" << cou_ATier << "]->Depth->" << *Assembly_Count[cou_ATier];
               for (int cou_Assembly=0;cou_Assembly<*Assembly_Count[cou_ATier];cou_Assembly++)
               {
                    tmp_Assembly = Assemblies[cou_ATier] [cou_Assembly];
                    cout << "\n\t   A[" << cou_Assembly << "]->" << tmp_Assembly;
                    DBug << "\n\t   A[" << cou_Assembly << "]->" << tmp_Assembly;

                    //Get the ID of the hotlinks table and extract the first three parameters from the last 3 indexes of each row.
                    //The parameters are From_Assembly, From_Construct, From_Row.
                    tmp_Table = tmp_Assembly->get_Input_Table_Hotlinks_Reference();

                    tmp_AID = tmp_Assembly->get_Assembly_ID();

                    for (int cou_Row=0;cou_Row<tmp_Table->get_Row_Count();cou_Row++)
                    {
                         if (tmp_Table->row_Is_Empty(cou_Row)){ continue; }

                         tmp_Assembly_From = tmp_Table->get_int(cou_Row, 2);
                         tmp_Construct_From = tmp_Table->get_int(cou_Row, 3);
                         tmp_Row_From = tmp_Table->get_int(cou_Row, 4);

                         cout << "\n\t     tmp_Assembly_From->";
                         DBug << "\n\t     tmp_Assembly_From->";
                         oint(0, 7, tmp_Assembly_From);

                         cout << "\n\t     tmp_Construct_From->";
                         DBug << "\n\t     tmp_Construct_From->";
                         oint(0, 7, tmp_Construct_From);

                         cout << "\n\t     tmp_Row_From->";
                         DBug << "\n\t     tmp_Row_From->";
                         oint(0, 7, tmp_Row_From);

                         SF << "\n" << tmp_Assembly_From << " " << tmp_Construct_From << " " << tmp_Row_From << " " << tmp_AID;
                    }

               }
          }

          SF.close();

     }

     //Loads a gathering.
     void Load(string p_Dir, string p_Gathering)
     {
          string tmp_String = "";
          string tmp_SD = "";
          int tmp_Int = 0;

          string tmp_Assembly_Name;
          string tmp_Type;
          int tmp_Assembly_Tier;
          int tmp_Construct_Count;
          string tmp_Construct_Name;
          int tmp_Assembly_ID = 0;

          //Setup the temporary variables.
          c_Base_Assembly * tmp_Assembly = NULL;
          c_Base_Construct * tmp_Construct = NULL;
          c_Base_Node_Network_1D * tmp_NN = NULL;
          int tmp_CID = 0;
          int tmp_AID = 0;

          c_Base_Assembly * tmp_Assembly_Target = NULL;
          c_Base_Construct * tmp_Construct_Target = NULL;
          c_Base_Node_Network_1D * tmp_NNT = NULL;
          c_Base_Node_1D * tmp_TN = NULL;

          int tmp_Target_Construct = 0;
          int tmp_Target_Tier = 0;
          int tmp_Target_Index = 0;


          //Gathers the file to save the node network to.
          ifstream SF;
          tmp_SD = "Save\\" + p_Dir + "\\";
          string tmp_FN = tmp_SD + p_Gathering + ".gth";
          cout << "\n\t Opening File->" << tmp_FN;
          SF.open(tmp_FN.c_str());

          if (!SF.is_open()){ cout << "\n\n\t FILE " << tmp_FN << " NOT OPENED!>!>!>!"; return; }

          getline(SF, tmp_String);

          cout << "\n\t Setting_Name(" << tmp_String << ")";
          Gathering_Name = tmp_String;

          //Load the settings from the file.
          SF >> tmp_Int;
          cout << "\n\t Increasing Number Of Tiers To->";
          oint(0, 7, tmp_Int);
          expand_Assembly_Tier(tmp_Int);
          cout << "  ";
          oint(0, 7, Assembly_Height);

          //Load the assemblies
          for (int cou_Tier=0;cou_Tier<Assembly_Height;cou_Tier++)
          {
               SF >> tmp_Int;
               expand_Assemblies(tmp_Int, cou_Tier);
               cout << "\n\t   expand_Assemblies(";
               oint(0, 7, cou_Tier);
               cout << ", ";
               oint(0, 7, tmp_Int);
               cout << ")";
          }

          while (!SF.eof())
          {
               //Get the ini parameters for the assemblies.
               SF >> tmp_Assembly_Name;
               SF >> tmp_Type;
               SF >> tmp_Assembly_Tier;
               SF >> tmp_Construct_Count;

               cout << "\n\t AName->" << tmp_Assembly_Name << " Type->" << tmp_Type << " Tier->";
               oint(0, 7, tmp_Assembly_Tier);
               cout << " CCount->";
               oint(0, 7, tmp_Construct_Count);
               tmp_Assembly_ID = create_Assembly(tmp_Assembly_Name, tmp_Type, tmp_Assembly_Tier);
               tmp_Assembly = get_Assembly_Ref(tmp_Assembly_ID);

               tmp_Assembly->Load_Assembly(tmp_SD);

               //Gather the constructs.
               for (int cou_Index=0;cou_Index<tmp_Construct_Count;cou_Index++)
               {
                    SF >> tmp_Construct_Name;

                    tmp_Assembly->Load_Construct(tmp_SD, tmp_Construct_Name);
                    cout << "\n\t  Construct->" << tmp_Construct_Name;
               }
          }

          //Index The Constructs Before Proceding.
          cout << "\n\t Indexing_Constructs.";
          cout << "\n\t Number Of ATiers->" << Assembly_Height;
          for (int cou_ATier=0;cou_ATier<Assembly_Height;cou_ATier++)
          {
               cout << "\n\t  ATier[" << cou_ATier << "]->Depth->" << *Assembly_Count[cou_ATier];
               for (int cou_Assembly=0;cou_Assembly<*Assembly_Count[cou_ATier];cou_Assembly++)
               {
                    tmp_Assembly = Assemblies[cou_ATier] [cou_Assembly];
                    cout << "\n\t   A[" << cou_Assembly << "]->" << tmp_Assembly;

                    for (int cou_Construct=0;cou_Construct<tmp_Assembly->Construct_Count;cou_Construct++)
                    {
                         Construct_Registry.set_int(int2str((tmp_Assembly->Constructs[cou_Construct])->get_Construct_U_CID()), cou_Construct);
                         Construct_Registry_AID.set_int(int2str((tmp_Assembly->Constructs[cou_Construct])->get_Construct_U_CID()), tmp_Assembly->get_Assembly_ID());
                    }
               }
          }
          cout << "\n\t Registered Constructs:";
          Construct_Registry.output_Tree();
          cout << "\n\t Registered Constructs And Their Owners.";
          Construct_Registry_AID.output_Tree();


          //Now that the constructs are loaded the state node references for multi-sensory constructs can be set.
          //output_Gathering();

          cout << "\n\t Number Of ATiers->" << Assembly_Height;
          for (int cou_ATier=0;cou_ATier<Assembly_Height;cou_ATier++)
          {
               cout << "\n\t  ATier[" << cou_ATier << "]->Depth->" << *Assembly_Count[cou_ATier];
               for (int cou_Assembly=0;cou_Assembly<*Assembly_Count[cou_ATier];cou_Assembly++)
               {
                    tmp_Assembly = Assemblies[cou_ATier] [cou_Assembly];
                    cout << "\n\t   A[" << cou_Assembly << "]->" << tmp_Assembly;

                    for (int cou_Construct=0;cou_Construct<tmp_Assembly->Construct_Count;cou_Construct++)
                    {
                         tmp_Construct = tmp_Assembly->Constructs[cou_Construct];
                         cout << "\n\t    C[" << cou_Construct << "]->" << tmp_Construct;
                         cout << "\n\t     Construct_Type->" << tmp_Construct->get_Input_Type();

                         //If the construct is of the MSC type then loop through each node gathering the reference from
                         //the other constructs using the CID contained in the last two digits of the treetop state.
                         if (tmp_Construct->get_Input_Type() == 1)
                         {
                              tmp_NN = tmp_Construct->get_Node_Network_Reference();

                              //Loop through the state tier using the dereferenced node network node count.
                              for (int cou_Index=0;cou_Index<(tmp_NN->tmp_State_Coordinates_Depth);cou_Index++)
                              {
                                   //Gather the coordinates.
                                   tmp_Target_Construct = tmp_NN->tmp_State_Coordinates[0] [cou_Index];
                                   //cout << "\n\t      tmp_Target_Construct->" << tmp_Target_Construct;

                                   tmp_Target_Tier = tmp_NN->tmp_State_Coordinates[1] [cou_Index];
                                   //cout << "\n\t      tmp_Target_Tier->" << tmp_Target_Tier;

                                   tmp_Target_Index = tmp_NN->tmp_State_Coordinates[2] [cou_Index];
                                   //cout << "\n\t      tmp_Target_Index->" << tmp_Target_Index;

                                   tmp_AID = Construct_Registry_AID.get_int(int2str(tmp_Target_Construct));
                                   //cout << "\n\t      tmp_AID->" << tmp_AID;


                                   if (tmp_AID != -1)
                                   {
                                        tmp_Assembly_Target = get_Assembly_Ref(tmp_AID);
                                        //cout << "\n\t       tmp_Assembly_Target->" << tmp_Assembly_Target;

                                        tmp_CID = Construct_Registry.get_int(int2str(tmp_Target_Construct));

                                        tmp_Construct_Target = tmp_Assembly_Target->Constructs[tmp_CID];
                                        //cout << "\n\t       tmp_Construct_Target->" << tmp_Construct_Target;

                                        tmp_NNT = tmp_Construct_Target->get_Node_Network_Reference();
                                        //cout << "\n\t       tmp_NNT->" << tmp_NNT;

                                        tmp_TN = tmp_NNT->Nodes[tmp_Target_Tier] [tmp_Target_Index];
                                        //cout << "\n\t       tmp_TN->" << tmp_TN;

                                        //cout << "\n\t       ";
                                        //tmp_TN->output_Node();
                                        //cout << "\n\t       Ttop->" << (tmp_TN->get_Treetop()).I;

                                        tmp_NN->Nodes[0] [cou_Index]->set_State(tmp_TN->get_Treetop());
                                        //tmp_NN->Nodes[0] [cou_Index]->output_Node();

                                        //cout << "\n\n";
                                   }
                              }
                         }
                    }
               }
          }

          SF.close();


          //Loading the hotlinks is done last so that the assemblies already exist.
          int tmp_AFrom;
          int tmp_CFrom;
          int tmp_RFrom;
          int tmp_ATo;

          tmp_FN = tmp_SD + p_Gathering + ".htk";
          cout << "\n\t Opening File->" << tmp_FN;
          SF.open(tmp_FN.c_str());

          if (!SF.is_open()){ cout << "\n\n\t FILE " << tmp_FN << " NOT OPENED!>!>!>!"; return; }

          cout << "\n\t  Gathering Hotlinks...";
          while (!SF.eof())
          {
               SF >> tmp_AFrom;
               if (SF.eof()){ break; }

               SF >> tmp_CFrom;
               if (SF.eof()){ break; }

               SF >> tmp_RFrom;
               if (SF.eof()){ break; }

               SF >> tmp_ATo;
               if (SF.eof()){ break; }

               cout << "\n\t   create_Hotlink(";
               oint(0, 7, tmp_AFrom);
               cout << ", ";
               oint(0, 7, tmp_CFrom);
               cout << ", ";
               oint(0, 7, tmp_RFrom);
               cout << ", ";
               oint(0, 7, tmp_ATo);
               cout << ")";

               create_Hotlink(tmp_AFrom, tmp_CFrom, tmp_RFrom, tmp_ATo);


          }

          SF.close();


          cout << "\n\n\t\b Done Loading...\n\n";
          //system("PAUSE > NULL");
     }

     //Saves the input table.
     void save_Input_Table()
     {
          Input_Table->save_Table();
     }
};
