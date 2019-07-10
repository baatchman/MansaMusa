



class c_Vision_1D
{
     
protected:
     
     //The current depth of the pattern.
     int Current_Depth;
     
     //The lookup trees that make up the pattern.
     c_State_Vision_Tree ** Pattern;
     
     
public:
     
     c_Vision_1D()
     {
          //Init
          Current_Depth = 0;
          Pattern = NULL;
     }
     
     ~c_Vision_1D()
     {
          delete Pattern;
     }
     
     //Takes a pattern as submission.
     void submit_Charged_Pattern(u_Data * p_Data, int p_Length, float p_Charge)
     {
          //Make sure there is enough room.
          expand_Pattern_Array(p_Length);
          
          //Loop through submitting the bits of the pattern to the lookup trees.
          for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
          {
               if (Pattern[cou_Index] == NULL){ continue; }
               
               Pattern[cou_Index]->search(p_Data[cou_Index], p_Charge);
          }
     }
     
     //Takes a string as the pattern.
     void submit_Charged_Pattern(string p_Data, float p_Charge)
     {
          //Make sure there is enough room.
          expand_Pattern_Array(p_Data.size());
          
          //Holds the current character.
          u_Data tmp_Data;
          
          //Loop through submitting the bits of the pattern to the lookup trees.
          for (unsigned int cou_Index=0;cou_Index<p_Data.size();cou_Index++)
          {
               if (Pattern[cou_Index] == NULL){ continue; }
               
               tmp_Data.I = int (p_Data[cou_Index]);
               Pattern[cou_Index]->search(tmp_Data, p_Charge);
          }
     }
     
     //Gets the current Pattern length.
     int get_Pattern_Length()
     {
          return Current_Depth;
     }
     
     //Gets the strongest pattern. Will crash if the called does not initialize p_Data.
     int get_Strongest_Pattern_Passed_Data(u_Data * &p_Data)
     {
          //Delete the data before we begin, if this has not been set at all this will cause a crash.
          delete [] p_Data;
          
          //Reallocate the data.
          p_Data = new u_Data[Current_Depth];
          
          //Get the strongest for each index.
          for (int cou_Index=0;cou_Index<Current_Depth;cou_Index++)
          {
               Pattern[cou_Index]->get_Strongest(&p_Data[cou_Index]);
          }
          
          //Return the length of the current input.
          return Current_Depth;
     }
     
     //Gets the strongest pattern and formats it as a string.
     string get_Strongest_Pattern_S()
     {
          //tmp Bit for the pattern.
          u_Data tmp_Data;
          
          //Temporary string to hold the pattern.
          string tmp_Pattern;
          
          //Get the strongest for each index.
          for (int cou_Index=0;cou_Index<Current_Depth;cou_Index++)
          {
               Pattern[cou_Index]->get_Strongest(&tmp_Data);
               tmp_Pattern += char (tmp_Data.I);
          }
          
          
          return tmp_Pattern;
     }
     
     //Expands the Pattern array to the given index w/o erasing old data.
     void expand_Pattern_Array(int p_Depth)
     {
          if (p_Depth <= Current_Depth){ return; }
          
          //Setup the temporary array to hold the data while reallocating the pattern array.
          c_State_Vision_Tree ** tmp_Pattern = new c_State_Vision_Tree*[Current_Depth];
          
          //Read the pattern array into the temporary array and then NULL the pattern indexes.
          for (int cou_Index=0;cou_Index<Current_Depth;cou_Index++)
          {
               tmp_Pattern[cou_Index] = Pattern[cou_Index];
               Pattern[cou_Index] = NULL;
          }
          
          //Reallocate the pattern array.
          delete [] Pattern;
          Pattern = new c_State_Vision_Tree*[p_Depth];
          
          //Initialize the new indexes.
          for (int cou_Index=Current_Depth;cou_Index<p_Depth;cou_Index++)
          {
               Pattern[cou_Index] = new c_State_Vision_Tree;
          }
          
          //Read in the old data.
          for (int cou_Index=0;cou_Index<Current_Depth;cou_Index++)
          {
               Pattern[cou_Index] = tmp_Pattern[cou_Index];
               tmp_Pattern[cou_Index] = NULL;
          }
          
          //Cleanup.
          delete [] tmp_Pattern;
          
          //Reset the counter to account for the new indexes.
          Current_Depth = p_Depth;
          
          return;
     }
     
     //Erases the current patterns.
     void reset_Vision()
     {
          //Loop through deleting the current lookup trees.
          for (int cou_Index=0;cou_Index<Current_Depth;cou_Index++)
          {
               delete Pattern[cou_Index];
               Pattern[cou_Index] = NULL;
          }
          
          //Delete and reset.
          delete [] Pattern;
          Pattern = NULL;
          
          Current_Depth = 0;
          
          return;
     }
};