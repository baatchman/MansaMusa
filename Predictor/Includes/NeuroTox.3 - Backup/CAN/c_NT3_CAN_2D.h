

//The class for the 1D CAN.
class c_NT3_CAN_2D
{
private:
     
     
public:
     
     //Reference to the input table to use for building.
     c_Raw_Table_Cell_2D * Input;
     
     //The current width of the input.
     int Input_Width;
     
     //The current height of the input.
     int Input_Height;
     
     //Reference to the node network to use for the node handling.
     c_NT3_Node_Network_2D * Nodes;
     
     //The nodes that make up the CAN.
     c_NT3_Base_Node_2D **** CAN;
     
     //The current number of tiers in the CAN.
     int Number_Of_Tiers;
     
     //The treetop node.
     c_NT3_Base_Node_2D *** Treetop;
     
     c_NT3_CAN_2D()
     {
          Input = NULL;
          Input_Width = 0;
          Input_Height = 0;
          Nodes = NULL;
          CAN = NULL;
          Treetop = NULL;
          Number_Of_Tiers = 0;
     }
     
     ~c_NT3_CAN_2D()
     {
          reset();
     }
     
     //Initializes the CAN with an input cell and the node netowrk.
     void init(c_NT3_Node_Network_2D * p_Nodes)
     {
          Nodes = p_Nodes;
     }
     
     //Sets the current input cell. p_Number_Of_Tiers is for the max height to build at.
     void set_Input(c_Raw_Table_Cell_2D * p_Input, int p_Number_Of_Tiers = -1)
     {
          //**cout << "\n     CAN_2D::set_Input(p_Input->" << p_Input << ")"; cout.flush();
          //**cout << "\n        reset()"; cout.flush();
          reset();
          Input = p_Input;
          //**cout << "\n         calculate_Size(" << p_Number_Of_Tiers << ")"; cout.flush();
          calculate_Size(p_Number_Of_Tiers);
          //**cout << "\n         resize()"; cout.flush();
          resize();
     }
     
     void calculate_Size(int p_Number_Of_Tiers = -1)
     {
          Input_Width = Input->Width;
          Input_Height = Input->Height;
          
          if (Input_Height > Input_Width){ Number_Of_Tiers = Input_Width; } else { Number_Of_Tiers = Input_Height; }
          if ((p_Number_Of_Tiers < Number_Of_Tiers) && (p_Number_Of_Tiers >= 0)){ Number_Of_Tiers = p_Number_Of_Tiers; }
     }
     
     //Sets the max number of tiers.
     void set_Max_Height(int p_Number_Of_Tiers)
     {
          if ((p_Number_Of_Tiers < Number_Of_Tiers) && (p_Number_Of_Tiers >= 0)){ Number_Of_Tiers = p_Number_Of_Tiers; }
     }
     
     //Resets the CAN.
     void reset()
     {
          if (Number_Of_Tiers != 0)
          {
               for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
               {
                    if (CAN[cou_T] == NULL){ continue; }
                    for (int cou_X=0;cou_X<(Input_Width - cou_T);cou_X++)
                    {
                         for (int cou_Y=0;cou_Y<(Input_Height - cou_T);cou_Y++)
                         {
                              CAN[cou_T] [cou_X] [cou_Y] = NULL;
                         }
                              delete [] CAN[cou_T] [cou_X];
                              CAN[cou_T] [cou_X] = NULL;
                    }
                    delete [] CAN[cou_T];
                    CAN[cou_T] = NULL;
               }
               delete [] CAN;
               CAN = NULL;
               
               //Treetops.
               if (Treetop != NULL)
               { 
                    for (int cou_X=0;cou_X<(Input_Width - Number_Of_Tiers);cou_X++)
                    {
                         if (Treetop[cou_X] == NULL){ continue; }
                         for (int cou_Y=0;cou_Y<(Input_Height - Number_Of_Tiers);cou_Y++)
                         {
                              Treetop[cou_X] [cou_Y] = NULL;
                         }
                         delete [] Treetop[cou_X]; Treetop[cou_X] = NULL;
                    }
                    delete [] Treetop; Treetop = NULL;
               }
          }
          Number_Of_Tiers = 0;
          Input_Height = 0;
          Input_Width = 0;
          Input = NULL;
     }
     
     //Resizes the CAN to build the current input set.
     void resize()
     {
          
          //--cout << "\n   --Number_Of_Tiers->" << Number_Of_Tiers;
          //--cout << "\n   --Input->Width->" << Input->Width;
          //--cout << "\n   --Input->Height->" << Input->Height;
          //--cout << "\n   --Input_Width->" << Input_Width;
          //--cout << "\n   --Input_Height->" << Input_Height;
          
          //Setup the tiers.
          //--cout << "\n   ...CAN = new c_NT3_Base_Node_2D***[Number_Of_Tiers];";
          CAN = new c_NT3_Base_Node_2D***[Number_Of_Tiers];
          
          //Init the tier sss and setup the indexes NULLing them along the way.
          //--cout << "\n   ...for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++);";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               //--cout << "\n   ......CAN[" << cou_T << "] = new c_NT3_Base_Node_2D**[(" << Input_Width << " - " << cou_T << ")];";
               CAN[cou_T] = new c_NT3_Base_Node_2D**[(Input_Width - cou_T)];
               //--cout << "\n   ......for (int cou_X=0;cou_X<(" << Input_Width << " - " << cou_T << ");cou_X++)";
               for (int cou_X=0;cou_X<(Input_Width - cou_T);cou_X++)
               {
                    //--cout << "\n   .........CAN[" << cou_T << "] [" << cou_X << "] = new c_NT3_Base_Node_2D*[(" << Input_Height << " - " << cou_T << ")];";
                    CAN[cou_T] [cou_X] = new c_NT3_Base_Node_2D*[(Input_Height - cou_T)];
                    //--cout << "\n   .........";
                    for (int cou_Y=0;cou_Y<(Input_Height - cou_T);cou_Y++)
                    {
                         CAN[cou_T] [cou_X] [cou_Y] = NULL;
                    }
               }
          }
          //--cout << "\n   end();";
     }
     
     
     //Builds full.
     void Full()
     {
          fill_State();
          build_Tiers_Full();
     }
     
     //Builds full.
     void Full(c_Raw_Table_Cell_2D * p_Input, int p_Number_Of_Tiers = -1)
     {
          set_Input(p_Input, p_Number_Of_Tiers);
          Full();
     }
     
     //Builds full with RC reinforcing.
     void RC()
     {
          fill_State();
          build_Tiers_Full();
          reinforce();
     }
     
     //Builds full with RC reinforcing.
     void RC(c_Raw_Table_Cell_2D * p_Input, int p_Number_Of_Tiers = -1)
     {
          set_Input(p_Input, p_Number_Of_Tiers);
          RC();
     }
     
     //Builds for Query.
     void Query()
     {
          query_State();
          build_Tiers_Query();
     }
     
     //Fills out the State tier.
     void fill_State()
     {
          if (Input == NULL){ return; }
          for (int cou_X=0;cou_X<Input->Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<Input->Height;cou_Y++)
               {
                    CAN[0] [cou_X] [cou_Y] = Nodes->get_State_Node(Input->Data[cou_X] [cou_Y]);
                    //**cout << "\n CAN[0] [" << cou_X << "] [" << cou_Y << "] = " << CAN[0] [cou_X] [cou_Y] << " (((" << Input->Data[cou_X] [cou_Y].I << ")))  ";
                    //**cout << (CAN[0] [cou_X] [cou_Y]->get_State()).I;
               }
          }
          //**output_CAN_State();
     }
          
     //Fills out the State tier.
     void query_State()
     {
          if (Input == NULL){ return; }
          for (int cou_X=0;cou_X<Input->Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<Input->Height;cou_Y++)
               {
                    CAN[0] [cou_X] [cou_Y] = Nodes->get_State_Node_Query(Input->Data[cou_X] [cou_Y]);
               }
          }
     }
     
     
     
     //Builds the Tiers full.
     void build_Tiers_Full()
     {
          c_NT3_Base_Node_2D * tmp_Dendrites[4];
          for (int cou_Index=0;cou_Index<4;cou_Index++)
          {
               tmp_Dendrites[cou_Index] = NULL;
          }
          
          //Init the tiers and setup the indexes NULLing them along the way.
          for (int cou_T=0;cou_T<(Number_Of_Tiers - 2);cou_T++)
          {
               //**ostr(0, 12, "\n --=======================================--");
               //**cout << "\n cou_T->"; oindex(cou_T);
               for (int cou_X=0;cou_X<(Input->Width - cou_T - 1);cou_X++)
               {
                    //**cout << "\n   cou_X->"; oindex(cou_X);
                    for (int cou_Y=0;cou_Y<(Input->Height - cou_T - 1);cou_Y++)
                    {
                         //**cout << "\n      cou_Y->"; oindex(cou_Y);
                         tmp_Dendrites[0] = CAN[cou_T] [cou_X] [cou_Y];
                         tmp_Dendrites[1] = CAN[cou_T] [cou_X] [cou_Y + 1];
                         tmp_Dendrites[2] = CAN[cou_T] [cou_X + 1] [cou_Y];
                         tmp_Dendrites[3] = CAN[cou_T] [cou_X + 1] [cou_Y + 1];
                         
                         //**for (int cou_Index=0;cou_Index<4;cou_Index++)
                         //**{
                         //**     cout << "\n         Den[" << cou_Index << "]->" << tmp_Dendrites[cou_Index];
                         //**}
                         CAN[cou_T + 1] [cou_X] [cou_Y] = Nodes->get_Upper_Tier_Connection(tmp_Dendrites);
                         //**cout << "\n            ~" << CAN[cou_T + 1] [cou_X] [cou_Y];
                    }
               }
          }
          
          
          //Gets the treetops.
          for (int cou_X=0;cou_X<(Input->Width - (Number_Of_Tiers - 2) - 1);cou_X++)
          {
               //cout << "\n   cou_X->"; oindex(cou_X);
               for (int cou_Y=0;cou_Y<(Input->Height - (Number_Of_Tiers - 2) - 1);cou_Y++)
               {
                    //cout << "\n      cou_Y->"; oindex(cou_Y);
                    tmp_Dendrites[0] = CAN[(Number_Of_Tiers - 2)] [cou_X] [cou_Y];
                    tmp_Dendrites[1] = CAN[(Number_Of_Tiers - 2)] [cou_X] [cou_Y + 1];
                    tmp_Dendrites[2] = CAN[(Number_Of_Tiers - 2)] [cou_X + 1] [cou_Y];
                    tmp_Dendrites[3] = CAN[(Number_Of_Tiers - 2)] [cou_X + 1] [cou_Y + 1];
                    
                    //for (int cou_Index=0;cou_Index<4;cou_Index++)
                    //{
                         //cout << "\n         Den[" << cou_Index << "]->" << tmp_Dendrites[cou_Index];
                    //}
                    CAN[(Number_Of_Tiers) - 1] [cou_X] [cou_Y] = Nodes->get_Treetop_Connection(tmp_Dendrites);
                    //cout << "\n            ~" << CAN[cou_T + 1] [cou_X] [cou_Y];
               }
          }
          //If there are not enough tiers to create a treetop connection then return NULL.
          if (Number_Of_Tiers < 1){ Treetop = NULL; return; }
          if (Number_Of_Tiers == 1){ Nodes->convert_To_Treetop_Node(CAN[0] [0] [0]); /*Treetop = CAN[0] [0] [0];*/ return; }
          
          //Gather treetop node.
          //Treetop = CAN[Number_Of_Tiers - 1] [0] [0];
     }
     
     
     
     //Builds the Tiers full.
     void build_Tiers_Query()
     {
          c_NT3_Base_Node_2D * tmp_Dendrites[4];
          for (int cou_Index=0;cou_Index<4;cou_Index++)
          {
               tmp_Dendrites[cou_Index] = NULL;
          }
          
          //Init the tiers and setup the indexes NULLing them along the way.
          for (int cou_T=0;cou_T<(Number_Of_Tiers - 1);cou_T++)
          {
               //ostr(0, 12, "\n --=======================================--");
               //cout << "\n cou_T->"; oindex(cou_T);
               for (int cou_X=0;cou_X<(Input->Width - cou_T - 1);cou_X++)
               {
                    //cout << "\n   cou_X->"; oindex(cou_X);
                    for (int cou_Y=0;cou_Y<(Input->Height - cou_T - 1);cou_Y++)
                    {
                         //cout << "\n      cou_Y->"; oindex(cou_Y);
                         tmp_Dendrites[0] = CAN[cou_T] [cou_X] [cou_Y];
                         tmp_Dendrites[1] = CAN[cou_T] [cou_X] [cou_Y + 1];
                         tmp_Dendrites[2] = CAN[cou_T] [cou_X + 1] [cou_Y];
                         tmp_Dendrites[3] = CAN[cou_T] [cou_X + 1] [cou_Y + 1];
                         
                         //for (int cou_Index=0;cou_Index<4;cou_Index++)
                         //{
                              //cout << "\n         Den[" << cou_Index << "]->" << tmp_Dendrites[cou_Index];
                         //}
                         CAN[cou_T + 1] [cou_X] [cou_Y] = Nodes->does_Upper_Tier_Connection_Exist(tmp_Dendrites);
                         //cout << "\n            ~" << CAN[cou_T + 1] [cou_X] [cou_Y];
                    }
               }
          }
     }
     
     //Reinforces the nodes in the CAN.
     void reinforce()
     {
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               for (int cou_X=0;cou_X<(Input_Width - cou_T);cou_X++)
               {
                    for (int cou_Y=0;cou_Y<(Input_Height - cou_T);cou_Y++)
                    {
                         if (CAN[cou_T] [cou_X] [cou_Y] == NULL){ continue; }
                         CAN[cou_T] [cou_X] [cou_Y]->reinforce();
                    }
               }
          }
     }
     
     
     
     //Outputs the CAN.
     void output_CAN()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_X=0;cou_X<Input->Width - cou_T;cou_X++)
               {
                    cout << "\n"; oindex(cou_X);
                    for (int cou_Y=0;cou_Y<Input->Height - cou_T;cou_Y++)
                    {
                         cout << " {";
                         if (CAN[cou_T] [cou_X] [cou_Y] != NULL)
                         {
                              opoi(0, int (CAN[cou_T] [cou_X] [cou_Y]->get_RC_Score()), CAN[cou_T] [cou_X] [cou_Y]);
                         }
                         else 
                         {
                              cout << "NULL";
                         }
                         cout << "}";
                    }
               }
          }
     }     
     
     //Outputs the CAN.
     void output_CAN_Condensed()
     {
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n";
               for (int cou_Y=0;cou_Y<Input->Height - cou_T;cou_Y++)
               {
                    cout << "\n";
                    for (int cou_X=0;cou_X<Input->Width - cou_T;cou_X++)
                    {
                         //if (CAN[cou_T] [cou_X] [cou_Y] == NULL){ continue; }
                         if (CAN[cou_T] [cou_X] [cou_Y] != NULL)
                         {
                              ochr(0, int ((CAN[cou_T] [cou_X] [cou_Y]->get_RC_Score()) / 5), char (197));
                         }
                         else
                         {
                              cout << "N";
                         }
                    }
               }
          }
     }
     
     //Outputs the CAN.
     void output_CAN_RC()
     {
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n";
               for (int cou_Y=0;cou_Y<Input->Height - cou_T;cou_Y++)
               {
                    cout << "\n";
                    for (int cou_X=0;cou_X<Input->Width - cou_T;cou_X++)
                    {
                         //if (CAN[cou_T] [cou_X] [cou_Y] == NULL){ continue; }
                         if (CAN[cou_T] [cou_X] [cou_Y] != NULL)
                         {
                              ochr(0, int ((CAN[cou_T] [cou_X] [cou_Y]->get_RC_Score()) / 5), char (197));
                         }
                         else
                         {
                              cout << "?";
                         }
                    }
               }
          }
     }
     
     /*
     //Outputs the CAN.
     void output_CAN_RC()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    cout << " {" << CAN[cou_T] [cou_Index]->get_RC_Score() << "}";
               }
          }
     }
          
     */
     //Outputs the CAN.
     void output_CAN_State()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_X=0;cou_X<Input->Width - cou_T;cou_X++)
               {
                    cout << "\n"; oindex(cou_X);
                    for (int cou_Y=0;cou_Y<Input->Height - cou_T;cou_Y++)
                    {
                         cout << " {";
                         if (CAN[cou_T] [cou_X] [cou_Y] != NULL)
                         {
                              Nodes->bp_Output_Only(CAN[cou_T] [cou_X] [cou_Y]);
                         }
                         else
                         {
                              cout << "NULL";
                         }
                         cout << "}";
                    }
               }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_State_F()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_X=0;cou_X<Input->Width - cou_T;cou_X++)
               {
                    cout << "\n"; oindex(cou_X);
                    for (int cou_Y=0;cou_Y<Input->Height - cou_T;cou_Y++)
                    {
                         if (CAN[cou_T] [cou_X] [cou_Y] == NULL){ continue; }
                         cout << " {";
                         if (CAN[cou_T] [cou_X] [cou_Y] != NULL)
                         {
                              cout << (CAN[cou_T] [cou_X] [cou_Y]->get_State()).F;
                         }
                         else
                         {
                              cout << "-";
                         }
                         cout << "}";
                    }
               }
          }
     }
          
     /*
     //Outputs the CAN.
     void output_CAN_Type()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    cout << " {" << (CAN[cou_T] [cou_Index]->get_Type()) << "}";
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
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    cout << " {" << CAN[cou_T] [cou_Index] << " (" << (CAN[cou_T] [cou_Index]->Axon_Count_L) << ", " << (CAN[cou_T] [cou_Index]->Axon_Count_R) << ")}";
               }
          }
     }
     */

};