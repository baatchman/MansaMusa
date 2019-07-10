

//The class for the 1D CAN.
class c_NT3_Poly_CAN_NonStrict
{
private:
     
     
public:
     
     //Reference to the input table to use for building.
     c_Raw_Table_Row_1D * Input;
     
     //Reference to the node network to use for the node handling.
     c_NT3_Node_Network_NonStrict * Nodes;
     
     //The nodes that make up the CAN.
     c_NT3_Base_Node_NonStrict **** CAN;
     
     //The current number of tiers in the CAN.
     int Number_Of_Tiers;
     
     //The number of nodes in each position on the tiers.
     int ** Number_Of_Nodes;
     int ** Number_Of_Nodes_MAX;
     
     //The treetop node.
     c_NT3_Base_Node_NonStrict * Treetop;
     
     c_NT3_Poly_CAN_NonStrict()
     {
          Input = NULL;
          Nodes = NULL;
          CAN = NULL;
          Treetop = NULL;
          Number_Of_Tiers = 0;
          Number_Of_Nodes = NULL;
          Number_Of_Nodes_MAX = NULL;
     }
     
     ~c_NT3_Poly_CAN_NonStrict()
     {
          reset();
     }
     
     //Initializes the CAN with an input cell and the node netowrk.
     void init(c_NT3_Node_Network_NonStrict * p_Nodes)
     {
          Nodes = p_Nodes;
     }
     
     //Sets the current input cell.
     void set_Input(c_Raw_Table_Row_1D * p_Input)
     {
          Input = p_Input;
     }
     
     //Builds for Query.
     void Query()
     {
          //*cout << "\n Query"; cout.flush();
          //*cout << " resize()"; cout.flush();
          resize();
          
          //*cout << " query_State()"; cout.flush();
          query_State();
          
          //*cout << " build_Tiers_Query()"; cout.flush();
          build_Tiers_Query();
          //*cout << " ~~Fin"; cout.flush();
     }
     
     //Fills out the State tier.
     void query_State()
     {
          for (int cou_Position=0;cou_Position<Number_Of_Tiers;cou_Position++)
          {
               resize_Position(0, cou_Position, Input->Cells[cou_Position]->Length);
               for (int cou_Index=0;cou_Index<(Input->Cells[cou_Position]->Length);cou_Index++)
               {
                    CAN[0] [cou_Position] [cou_Index] = Nodes->get_State_Node_Query(Input->Cells[cou_Position]->Data[cou_Index]);
               }
          }
     }
     
     //Builds the Tiers full.
     void build_Tiers_Query()
     {
          c_NT3_Base_Node_NonStrict * tmp_Node = NULL;
          //Init the tiers and setup the indexes NULLing them along the way.
          for (int cou_T=0;cou_T<(Number_Of_Tiers - 1);cou_T++)
          {
               for (int cou_P=0;cou_P<((Number_Of_Tiers - cou_T) - 1);cou_P++)
               {
                    for (int cou_Index=0;cou_Index<Number_Of_Nodes[cou_T] [cou_P];cou_Index++)
                    {
                         for (int cou_NIndex=0;cou_NIndex<Number_Of_Nodes[cou_T] [cou_P + 1];cou_NIndex++)
                         {
                              tmp_Node = NULL;
                              tmp_Node = Nodes->does_Upper_Tier_Connection_Exist(CAN[cou_T] [cou_P] [cou_Index], CAN[cou_T] [cou_P + 1] [cou_NIndex]);
                              if (tmp_Node != NULL)
                              {
                                   expand_Position((cou_T + 1), cou_P);
                                   CAN[cou_T + 1] [cou_P] [Number_Of_Nodes[cou_T + 1] [cou_P] - 1] = tmp_Node;
                              }
                         }
                    }
               }
          }
     }
     
     //Resets the CAN.
     void reset()
     {
          if (Number_Of_Tiers != 0)
          {
               for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
               {
                    if (CAN[cou_T] == NULL){ continue; }
                    for (int cou_Position=0;cou_Position<(Number_Of_Tiers - cou_T);cou_Position++)
                    {
                         if (Number_Of_Nodes != NULL)
                         {
                              for (int cou_Node=0;cou_Node<Number_Of_Nodes[cou_T] [cou_Position];cou_Node++)
                              {
                                   CAN[cou_T] [cou_Position] [cou_Node] = NULL;
                              }
                         }
                         delete [] CAN[cou_T] [cou_Position];
                         CAN[cou_T] [cou_Position] = NULL;
                    }
                    delete [] CAN[cou_T];
                    CAN[cou_T] = NULL;
               }
               delete [] CAN;
               CAN = NULL;
          }
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               delete [] Number_Of_Nodes[cou_T];
               delete [] Number_Of_Nodes_MAX[cou_T];
               Number_Of_Nodes[cou_T] = NULL;
               Number_Of_Nodes_MAX[cou_T] = NULL;
          }
          delete [] Number_Of_Nodes;
          delete [] Number_Of_Nodes_MAX;
          Number_Of_Nodes = NULL;
          Number_Of_Nodes_MAX = NULL;
          
          Number_Of_Tiers = 0;
          
          
     }
     
     //Resizes the CAN to build the current input set.
     void resize()
     {
          //Reset the CAN before we begin.
          reset();
          
          //Gather the number of tiers to build.
          Number_Of_Tiers = Input->Depth;
          
          //Setup the tiers.
          CAN = new c_NT3_Base_Node_NonStrict***[Number_Of_Tiers];
          
          Number_Of_Nodes = new int*[Number_Of_Tiers];
          Number_Of_Nodes_MAX = new int*[Number_Of_Tiers];
          
          //Init the tiers and setup the indexes NULLing them along the way.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               CAN[cou_T] = new c_NT3_Base_Node_NonStrict**[(Number_Of_Tiers - cou_T)];
               Number_Of_Nodes[cou_T] = new int[(Number_Of_Tiers - cou_T)];
               Number_Of_Nodes_MAX[cou_T] = new int[(Number_Of_Tiers - cou_T)];
               
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    CAN[cou_T] [cou_Index] = NULL;
                    Number_Of_Nodes[cou_T] [cou_Index] = 0;
                    Number_Of_Nodes_MAX[cou_T] [cou_Index] = 0;
               }
          }
     }
     
     //Expands a given position by 1.
     void expand_Position(int p_Tier, int p_Position)
     {
          resize_Position(p_Tier, p_Position, (Number_Of_Nodes[p_Tier] [p_Position] + 1));
     }
     
     //Adds nodes to a given position on a given tier.
     void resize_Position(int p_Tier, int p_Position, int p_Size)
     {
          if (p_Tier >= Number_Of_Tiers){ cout << "\n\n POLY_CAN ERROR! Attempted resize past tier limit. T->" << Number_Of_Tiers << " p_T->" << p_Tier; }
          if (p_Position >= (Number_Of_Tiers - p_Tier)){ cout << "\n\n POLY_CAN ERROR! Attempted resize past position limit. T->" << Number_Of_Tiers << " p_T->" << p_Tier << " I->" << (Number_Of_Tiers - p_Tier) << " p_I->" << p_Position; }
          
          if (p_Size <= Number_Of_Nodes[p_Tier] [p_Position]){ return; }
          if (p_Size < Number_Of_Nodes_MAX[p_Tier] [p_Position]){ Number_Of_Nodes[p_Tier] [p_Position] = p_Size; return; }
          
          //Setup the tmp vars.
          c_NT3_Base_Node_NonStrict ** tmp_Nodes;
          tmp_Nodes = new c_NT3_Base_Node_NonStrict*[Number_Of_Nodes[p_Tier] [p_Position]];
          int tmp_MAX = p_Size + 100;
          
          //Copy the old nodes into the tmp.
          for (int cou_Index=0;cou_Index<Number_Of_Nodes[p_Tier] [p_Position];cou_Index++)
          {
               tmp_Nodes[cou_Index] = CAN[p_Tier] [p_Position] [cou_Index];
               CAN[p_Tier] [p_Position] [cou_Index] = NULL;
          }
          
          //Reallocate the old with the new max.
          delete [] CAN[p_Tier] [p_Position];
          CAN[p_Tier] [p_Position] = new c_NT3_Base_Node_NonStrict*[tmp_MAX];
          
          //NULL the newly created position.
          for (int cou_Index=0;cou_Index<tmp_MAX;cou_Index++)
          {
               CAN[p_Tier] [p_Position] [cou_Index] = NULL;
          }
          
          //Read the old nodes back in.
          for (int cou_Index=0;cou_Index<Number_Of_Nodes[p_Tier] [p_Position];cou_Index++)
          {
               CAN[p_Tier] [p_Position] [cou_Index] = tmp_Nodes[cou_Index];
               tmp_Nodes[cou_Index] = NULL;
          }
          
          //Cleanup the tmp.
          delete [] tmp_Nodes;
          tmp_Nodes = NULL;
          
          //Set the sizes appropriately.
          Number_Of_Nodes[p_Tier] [p_Position] = p_Size;
          Number_Of_Nodes_MAX[p_Tier] [p_Position] = tmp_MAX;
     }
     
     //Outputs the CAN.
     void output_CAN()
     {
          cout << "\n\n";
          cout << "#T->" << Number_Of_Tiers;
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_P=0;cou_P<(Number_Of_Tiers - cou_T);cou_P++)
               {
                    cout << "   <"; oint(0, 14, cou_P); cout << ">";
                    for (int cou_Index=0;cou_Index<Number_Of_Nodes[cou_T] [cou_P];cou_Index++)
                    {
                         cout << " {" << CAN[cou_T] [cou_P] [cou_Index] << "}";
                    }
               }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_RC()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_P=0;cou_P<(Number_Of_Tiers - cou_T);cou_P++)
               {
                    cout << "   <"; oint(0, 14, cou_P); cout << ">";
                    for (int cou_Index=0;cou_Index<Number_Of_Nodes[cou_T] [cou_P];cou_Index++)
                    {
                         cout << " {" << CAN[cou_T] [cou_P] [cou_Index]->get_RC_Score() << "}";
                    }
               }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_State()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_P=0;cou_P<(Number_Of_Tiers - cou_T);cou_P++)
               {
                    cout << "   <"; oint(0, 14, cou_P); cout << ">";
                    for (int cou_Index=0;cou_Index<Number_Of_Nodes[cou_T] [cou_P];cou_Index++)
                    {
                         Nodes->bp_Output_Only(CAN[cou_T] [cou_P] [cou_Index]);
                    }
               }
          }
     }

     //Outputs the CAN.
     void output_CAN_State_I()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_P=0;cou_P<(Number_Of_Tiers - cou_T);cou_P++)
               {
                    cout << "   <"; oint(0, 14, cou_P); cout << ">";
                    for (int cou_Index=0;cou_Index<Number_Of_Nodes[cou_T] [cou_P];cou_Index++)
                    {
                         Nodes->bp_Output_Only(CAN[cou_T] [cou_P] [cou_Index], 1);
                    }
               }
          }
     }
     
     //Outputs the CAN.
     void output_CAN_Type()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_P=0;cou_P<(Number_Of_Tiers - cou_T);cou_P++)
               {
                    cout << "   <"; oint(0, 14, cou_P); cout << ">";
                    for (int cou_Index=0;cou_Index<Number_Of_Nodes[cou_T] [cou_P];cou_Index++)
                    {
                         cout << " {" << (CAN[cou_T] [cou_P] [cou_Index]->get_Type()) << "}";
                    }
               }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_Axons()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_P=0;cou_P<(Number_Of_Tiers - cou_T);cou_P++)
               {
                    cout << "   <"; oint(0, 14, cou_P); cout << ">";
                    for (int cou_Index=0;cou_Index<Number_Of_Nodes[cou_T] [cou_P];cou_Index++)
                    {
                         cout << " {" << CAN[cou_T] [cou_P] [cou_Index] << " (" << (CAN[cou_T] [cou_P] [cou_Index]->Axon_Count_L) << ", " << (CAN[cou_T] [cou_P] [cou_Index]->Axon_Count_R) << ")}";
                    }
               }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_Axons_State()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_P=0;cou_P<(Number_Of_Tiers - cou_T);cou_P++)
               {
                    cout << "   <"; oint(0, 14, cou_P); cout << ">";
                    for (int cou_Index=0;cou_Index<Number_Of_Nodes[cou_T] [cou_P];cou_Index++)
                    {
                         cout << " {";
                         Nodes->bp_Output_Only(CAN[cou_T] [cou_P] [cou_Index]);
                         cout << " (" << (CAN[cou_T] [cou_P] [cou_Index]->Axon_Count_L) << ", " << (CAN[cou_T] [cou_P] [cou_Index]->Axon_Count_R) << ")}";
                    }
               }
          }
     }

};