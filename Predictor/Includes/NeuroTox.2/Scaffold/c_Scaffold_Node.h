
//This class is used for loading nodes when rebuilding.
//The first index is [0]->Tier and [1]->Index.
class c_Scaffold_Node
{
public:
     
     int * Axons[2] [2];
     int Axons_Count[2];
     
     int Dendrites[2] [2];
     
     c_Scaffold_Node()
     {
          //Set the dendrites.
          Dendrites[0] [0] = 0;
          Dendrites[1] [0] = 0;
          Dendrites[0] [1] = 0;
          Dendrites[1] [1] = 0;
          
          //Set the axons.
          Axons[0] [0] = NULL;
          Axons[1] [0] = NULL;
          Axons[0] [1] = NULL;
          Axons[1] [1] = NULL;
          
          Axons_Count[0] = 0;
          Axons_Count[1] = 0;
     }
     
     ~c_Scaffold_Node()
     {
          delete [] Axons[0] [0]; Axons[0] [0] = NULL;
          delete [] Axons[1] [0]; Axons[1] [0] = NULL;
          delete [] Axons[0] [1]; Axons[0] [1] = NULL;
          delete [] Axons[1] [1]; Axons[1] [1] = NULL;
     }
     
     //Increases the number of axons.
     void increase_Axons(int p_Leg, int p_Index)
     {
          //cout << "\n\t  increase_Axons(" << p_Leg << ", " << p_Index << ")";
          
          if (p_Index <= Axons_Count[p_Leg]){ return; }
          
          //Setup the temporary array.
          int * tmp_Axons[2] [2];
          
          //Allocate the temporary axons.
          tmp_Axons[p_Leg] [0] = new int[Axons_Count[p_Leg]];
          tmp_Axons[p_Leg] [1] = new int[Axons_Count[p_Leg]];
          
          //Read the old ones into the newly allocated legs.
          for (int cou_Index=0;cou_Index<Axons_Count[p_Leg];cou_Index++)
          {
               tmp_Axons[p_Leg] [0] [cou_Index] = Axons[p_Leg] [0] [cou_Index];
               tmp_Axons[p_Leg] [1] [cou_Index] = Axons[p_Leg] [1] [cou_Index];
          }
          
          //Delete the old array for reallocation.
          delete [] Axons[p_Leg] [0]; Axons[p_Leg] [0] = NULL;
          delete [] Axons[p_Leg] [1]; Axons[p_Leg] [1] = NULL;
          
          Axons[p_Leg] [0] = new int[p_Index];
          Axons[p_Leg] [1] = new int[p_Index];
          
          for (int cou_Index=0;cou_Index<Axons_Count[p_Leg];cou_Index++)
          {
               Axons[p_Leg] [0] [cou_Index] = tmp_Axons[p_Leg] [0] [cou_Index];
               Axons[p_Leg] [1] [cou_Index] = tmp_Axons[p_Leg] [1] [cou_Index];
          }
          
          for (int cou_Index=Axons_Count[p_Leg];cou_Index<p_Index;cou_Index++)
          {
               Axons[p_Leg] [0] [cou_Index] = 0;
               Axons[p_Leg] [1] [cou_Index] = 0;
          }
          
          Axons_Count[p_Leg] = p_Index;
          
          //Delete the tmps arrays.
          delete [] tmp_Axons[p_Leg] [0]; tmp_Axons[p_Leg] [0] = NULL;
          delete [] tmp_Axons[p_Leg] [1]; tmp_Axons[p_Leg] [1] = NULL;
     }
     
     //Outputs the node.
     void output_Node()
     {    
          cout << "D.0(";
          
          oint(0, 15, Dendrites[0] [0]);
          cout << ", ";
          oint(0, 14, Dendrites[0] [1]);
          cout << ")";
          
          cout << " D.1(";
          
          oint(0, 15, Dendrites[1] [0]);
          cout << ", ";
          oint(0, 14, Dendrites[1] [1]);
          cout << ")";
          
          cout << " A.0->" << Axons_Count[0];
          for (int cou_Index=0;cou_Index<Axons_Count[0];cou_Index++)
          {
               cout << "(";
               oint(0, 15, Axons[0] [0] [cou_Index]);
               cout << ", ";
               oint(0, 14, Axons[0] [1] [cou_Index]);
               cout << ")";
          }
          
          cout << " A.1->" << Axons_Count[1];
          for (int cou_Index=0;cou_Index<Axons_Count[1];cou_Index++)
          {
               cout << "(";
               oint(0, 15, Axons[1] [0] [cou_Index]);
               cout << ", ";
               oint(0, 14, Axons[1] [1] [cou_Index]);
               cout << ")";
          }
     }
};