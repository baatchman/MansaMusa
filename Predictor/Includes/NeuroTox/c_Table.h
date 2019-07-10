//   This file contains the classes neccesary to use Internal and external tables for communication between classes.

//This class holds the properties for the table data in each cell.
class c_Data_Properties
{
private:
     
     //The dimension of the data.
     int Dimension;
     
     //The depths of each dimension.
     int * Dimension_Depths;  
     
     //The data flg, has generic usage.
     u_IO_Data flg[10];
     
     //The set flag for the flags.
     bool flg_Set[10];
     
     //--     FLAG INDEX RESERVATIONS
     //     0: Cell Charge.
     
     //     7: Table reference.
     //     8: Index reference.
     //     9: Table_Handle //For getting data of another cell.
     
public:
     
     c_Data_Properties()
     {
          Dimension = 0;
          Dimension_Depths = NULL;
          for (int cou_Index=0;cou_Index<10;cou_Index++)
          {
               flg[cou_Index].I = 10;
               flg_Set[cou_Index] = false;
          }
     }
     
     ~c_Data_Properties()
     {
          delete [] Dimension_Depths;
     }
     
     //Bounds checking for the p_Flag && p_Set_Flag
     bool b_flg(int p_Flag)
     {
          if (p_Flag < 0){ return 0; }
          if (p_Flag >= 10){ return 0; }
          return 1;
     }
     
     //Bounds chekcing for the dimension.
     bool b_Dimension(int p_Dimension)
     {
          if (p_Dimension < 0){ return 0; }
          if (p_Dimension >= Dimension){ return 0; }
          return 1;
     }
     
     //This function sets the properties to the given properties.
     void set_Properties(int p_Dimension_Depths[], int p_Dimension)
     {
          //Set the dimension.
          Dimension = p_Dimension;
          
          //Allocate the dimension array after deleting it.
          delete [] Dimension_Depths;
          Dimension_Depths = new int[Dimension];
          
          //Loop through and set each dimension.
          for (int cou_D=0;cou_D<Dimension;cou_D++)
          {
               Dimension_Depths[cou_D] = p_Dimension_Depths[cou_D];
          }
     }
     
     //Sets a flag.
     u_IO_Data set_Flag(int p_Flag, u_IO_Data p_State)
     {
          //Bounds checking.
          u_IO_Data tmp_Data; tmp_Data.I = 0;
          if (!b_flg(p_Flag)){ return tmp_Data; }
          
          flg[p_Flag] = p_State;
          flg_Set[p_Flag] = true;
          return flg[p_Flag];
     }
     
     //Gets a flag state.
     u_IO_Data get_Flag(int p_Flag)
     {
          //Bounds checking.
          u_IO_Data tmp_Data; tmp_Data.I = 0;
          if (!b_flg(p_Flag)){ return tmp_Data; }
          
          
          return flg[p_Flag];
     }
     
     //Resets all the flags.
     void reset_Flags()
     {
          for (int cou_Index=0;cou_Index<10;cou_Index++)
          {
               flg[cou_Index].I = 0;
               flg_Set[cou_Index] = false;
          }
     }
     
     //Gets the dimension.
     int get_Dimension()
     {
          return Dimension;
     }
     
     //Gets a depth for a given dimension.
     int get_Depth(int p_Dimension)
     {
          //If the dimension is too high then return.
          if (b_Dimension(p_Dimension)){ return 0; }
          
          return Dimension_Depths[p_Dimension];
     }

     //Outputs the flag index.
     void output_Flags(int p_Flags)
     {
          if (!p_Flags){ return; }
          if (p_Flags < 0){ return; }
          if (p_Flags > 10){ p_Flags = 10; }
          
          bool tmp_Flagset = false;
          
          for (int cou_Index=0;cou_Index<p_Flags;cou_Index++)
          {
               if (flg_Set[cou_Index])
               {
                    tmp_Flagset = true;
               }
          }
          
          if (!tmp_Flagset){ return; }
          
          cout << " Flags:";
          for (int cou_Index=0;cou_Index<p_Flags;cou_Index++)
          {
               if (flg_Set[cou_Index])
               {
                    cout << " [" << cou_Index << "](" << flg[cou_Index].F << ")";
               }
          }
          cout << " "; 
     }

};











//This class holds the data for each cell in the table.
class c_Cell_Data
{
private:

     //The cells data.
     u_IO_Data * Index;
     
     //The current number of indexes.
     int Index_Counter;
     
     //The properties of the cell. This is public for direct access to the get_Dimension & get_Depth members.
     c_Data_Properties Properties;
     
     //Whether or not the data in the cell represents a table reference.
     bool is_Reference_Table;
     
     //The cells name.
     string Name;
     
     friend class c_Table_Handler;
     
public:
     
     c_Cell_Data()
     {
          //cout << "\n\n c_Cell_Data()" << this;
          //Initiate the members.
          Index_Counter = 0;
          Index = NULL;
          
          
          is_Reference_Table = false;
          Name = "Cell";
     }
     
     ~c_Cell_Data()
     {
          //cerr << "\n\n ~c_Cell_Data()" << this;
          delete [] Index;
     }
     
     //Bounds checking for the Index_Counter.
     bool b_Index(int p_Index)
     {
          if (p_Index < 0){ return 0; }
          if (p_Index >= Index_Counter){ return 0; }
          return 1;
     }
     
     //Sets the properties of the cells data.
     void set_Properties(int p_Dimension_Depths[], int p_Dimension)
     {
          Properties.set_Properties(p_Dimension_Depths, p_Dimension);
     }
     
     //Sets a flag.
     u_IO_Data set_Flag(int p_Flag, u_IO_Data p_State)
     {
          return Properties.set_Flag(p_Flag, p_State);
     }
     
     //Gets a flag state.
     u_IO_Data get_Flag(int p_Flag)
     {
          return Properties.get_Flag(p_Flag);
     }
     
     //Resets all the flags to 0.0;
     void reset_Flags()
     {
          Properties.reset_Flags();
     }
     
     //Gets the dimension of the data.
     int get_Dimension()
     {
          return Properties.get_Dimension();
     }
     
     //Gets the depth of a given dimension.
     int get_Depth(int p_Dimension)
     {
          return Properties.get_Depth(p_Dimension);
     }
     
     //Get the number of indexes.
     int get_Data_Length()
     {
          return Index_Counter;
     }
     
     //Get the state of the is_Reference_Table flag.
     bool get_isReference()
     {
          return is_Reference_Table;
     }
     
     //Set the reference flag.
     bool set_Reference_Table(bool p_Reference_Table)
     {
          is_Reference_Table = p_Reference_Table;
          return p_Reference_Table;
     }
     
     //This function sets the cell name.
     void set_Cell_Name(string p_Cell_Name = "Cell")
     {
          Name = p_Cell_Name;
     }
     
     //Gets the cell name.
     string get_Cell_Name()
     {
          return Name;
     }
     
     //Accepts an array of u_IO_Data with a given length and sets the current data to the given data.
     int set_Data(u_IO_Data p_Data[], int p_Data_Length)
     {
          //cout << "\n\t\t ~~c_Cell_Data::set_Index(p_Data:" << p_Data << ", p_Data_Length:" << p_Data_Length << ")"; cout.flush();
          
          //cout << "\n\t\t -->>Given: ";
          /*
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               cout << " " << p_Data[cou_Index].I;
          }
          */
          
          //Erase the old data.
          //cout << "\n\t\t\t -->delete [] Index;"; cout.flush();
          delete [] Index;
          Index = NULL;
          
          //cout << "\n\t\t\t -->Index_Counter = p_Data_Length;"; cout.flush();
          Index_Counter = p_Data_Length;
          
          //Reallocate the array to the correct size.
          //cout << "\n\t\t\t -->Index = new u_IO_Data[Index_Counter];"; cout.flush();
          Index = new u_IO_Data[Index_Counter];
          
          //Copy the given data into to the index array.
          //cout << "\n\t\t -->>Copying: "; cout.flush();
          for (int cou_D=0;cou_D<Index_Counter;cou_D++)
          {
               Index[cou_D].NR = p_Data[cou_D].NR;
               //cout << " " << Index[cou_D].I; cout.flush();
          }
          
          //cout << "\n\t\t -->>Stored: ";
          //for (int cou_Index=0;cou_Index<Index_Counter;cou_Index++)
          //{
          //     cout << " " << Index[cou_Index].I; cout.flush();
          //}
          
          //Resets flags when data is set.
          //reset_Flags();
          
            return Index_Counter;
     }
     
     //Accepts a single unit of data and sets the 0 index to it. Does not erase, but will overwrite.
     void set_Data_Unit(u_IO_Data p_Data, int p_Index)
     {    
          //If there are not enough indexes, then increase the size of the index array.
          if (p_Index >= Index_Counter){ increase_Index_Depth(p_Index + 1); }
          
          //Set the given index to the given data.
          Index[p_Index].NR = p_Data.NR;
          
          //reset_Flags();
          
     }
     
     //Accepts a single unit of data and sets the 0 index to it. Erases the old data.
     void set_Data_Unit(u_IO_Data p_Data)
     {    
          delete [] Index;
          Index_Counter = 1;
          
          Index = new u_IO_Data[Index_Counter];
          
          //Set the given index to the given data.
          Index[0].NR = p_Data.NR;
          
          //Resets the flags
          //reset_Flags();
     }
     
     //Pushes a unit of data onto the data array.
     void push_Data_Unit(u_IO_Data p_Data)
     {
          //Increases the data count.
          Index_Counter++;
          increase_Index_Depth(Index_Counter);
          
          Index[Index_Counter - 1].NR = p_Data.NR;
     }
     
     //Increases the depth of the Index array.
     void increase_Index_Depth(int p_New_Index_Count)
     {
          if (p_New_Index_Count < Index_Counter){ return; }
          
          u_IO_Data * tmp_Index = new u_IO_Data[p_New_Index_Count];
          
          //Inititalize the newly created indexes to 0.
          for (int cou_Index=0;cou_Index<p_New_Index_Count;cou_Index++)
          {
               tmp_Index[cou_Index].I = 0;
          }
          
          //Save the old data.
          for (int cou_Index=0;cou_Index<Index_Counter;cou_Index++)
          {
               tmp_Index[cou_Index].NR = Index[cou_Index].NR;
          }
          
          delete [] Index;
          Index = new u_IO_Data[p_New_Index_Count];
          
          //Save the old data.
          for (int cou_Index=0;cou_Index<Index_Counter;cou_Index++)
          {
               Index[cou_Index].NR = tmp_Index[cou_Index].NR;
          }
          
          //Sets the index counter.
          Index_Counter = p_New_Index_Count;
          
          delete [] tmp_Index;
          
          return;
     }
     
     //Returns the data.
     u_IO_Data * get_Data()
     { 
          return Index;
     }
     
     //Outputs the data.
     void output_Data(int p_I = 1, int p_Flags = 1)
     {
          //If the cell holds no data then do not display it.
          if (Index_Counter == 0 && !is_Reference_Table){ return; }
          
          if (is_Reference_Table)
          {
               
               cout << "\"" << Name << "\" ";
               
               cout << "&T";
               cout << Properties.get_Flag(7).I;
               cout << "H";
               cout << Properties.get_Flag(8).I;
               
               cout << " {";
               for (int cou_Index=0;cou_Index<Index_Counter;cou_Index++)
               {
                    if (p_I == 0){ cout << Index[cou_Index].I << "~"; }
                    if (p_I == 1){ cout << Index[cou_Index].C << ""; }
                    if (p_I == 2){ cout << Index[cou_Index].F << "."; }
                    if (p_I == 2){ cout << Index[cou_Index].NR << "."; }
               }
               cout << "}";
               return;
          }
          
          cout << "\"" << Name << "\" ";
          Properties.output_Flags(p_Flags);
          for (int cou_Index=0;cou_Index<Index_Counter;cou_Index++)
          {
               if (p_I == 0){ cout << Index[cou_Index].I << "~"; }
               if (p_I == 1){ cout << Index[cou_Index].C << ""; }
               if (p_I == 2){ cout << Index[cou_Index].F << "."; }
               if (p_I == 2){ cout << Index[cou_Index].NR << "."; }
          }
     }
};
 
 
 
 
 
 
 
 
 
 
 
//This class stores data using a single dimension array. The members use the dimension_Depth to determine proper position in the array.
class c_Table_Data_Field
{
     //Holds all of the data cells in an infinity array.
     
     //The cell data.
     c_Cell_Data ** Data;
     
     //The cell data current count.
     int Data_Count;
     
     //The current index for the push members.
     int Current_Push_Index;
     
     
public:
     
     c_Table_Data_Field()
     {
          Data = NULL;
          Data_Count = 0;
          Current_Push_Index = 0;
     }
     
     
     //Bounds checking for the Data_Count.
     bool b_Data(int p_Data)
     {
          if (p_Data < 0){ return 0; }
          if (p_Data >= Data_Count){ return 0; }
          return 1;
     }
     
     //Increases the data index to the given number of indexes.
     void increase_Data_Index(int p_Index)
     {
               //If the given index is less than the current index, return.
               if (p_Index <= Data_Count){ return; }
               
               //Create temporary array.
               c_Cell_Data ** tmp_Data;
               
               //Allocate the new c_Cell_Data array.
               tmp_Data = new c_Cell_Data*[p_Index];
               
               //NULL the newly created array.
               for (int cou_Index=0;cou_Index<p_Index;cou_Index++){
                    tmp_Data[cou_Index] = NULL;
               }
               
               //Copy current data.
               for (int cou_Index=0;cou_Index<Data_Count;cou_Index++){
                    tmp_Data[cou_Index] = Data[cou_Index];
                    //cout << " " << tmp_Data[cou_Index];
                    Data[cou_Index] = NULL;
               }
               
               //Destroy the old array.
               delete [] Data;
               
               //Recreate the old array.
               Data = new c_Cell_Data*[p_Index];
               
               //Read the old data into the newly created array.
               //With the new array having null members this will not cause overflow errors, would't anyway though.
               for (int cou_Index=0;cou_Index<p_Index;cou_Index++)
               {
                    Data[cou_Index] = tmp_Data[cou_Index];
                    tmp_Data[cou_Index] = NULL;
               }
               
               //Clean up the temporary array.
               delete [] tmp_Data;
               tmp_Data = NULL;
               
               //Increment the Data_Count.
               Data_Count = p_Index;
     }
     
     //Resets the data field.
     void reset()
     {
               if (Data == NULL){ return; }
               
               //Delete current data.
               for (int cou_Index=0;cou_Index<Data_Count;cou_Index++){
                    delete Data[cou_Index];
               }
               
               //Destroy the old array.
               delete [] Data;
               Data = NULL;
               
               //Increment the Data_Count.
               Data_Count = 0;
               Current_Push_Index = 0;
     }
     
     void reset_Index(int p_Index)
     {
          delete Data[p_Index];
          Data[p_Index] = NULL;
     }
     
     //Returns a cells data.
     u_IO_Data * get_Data(int p_Index = 0)
     {
          //Prevent bounding errors
          if (p_Index < 0){ return 0; }
          if (p_Index >= Data_Count){ increase_Data_Index(p_Index + 1); }
          if (Data[p_Index] == NULL){ Data[p_Index] = new c_Cell_Data; }
          
          return Data[p_Index]->get_Data();
     }
     
     //Sets a cells data.
     void set_Data(u_IO_Data * p_Data, int p_Data_Length = 0, int p_Index = 0)
     {
          //cout << "\n\t\t ~~c_Table_Data_Field::set_Index(p_Data:" << p_Data << ", p_Data_Length:" << p_Data_Length << ", p_Index:" << p_Index << ")";
          
          //Prevent bounding errors 
          if (p_Index < 0){ return; cout << "\n\n\n\t\t\t\t --==:: WARNING: Set Data Does Nothing (p_Index < 0)"; }
          if (p_Index >= Data_Count){ increase_Data_Index(p_Index + 1); }
          if (Data[p_Index] == NULL){ Data[p_Index] = new c_Cell_Data; }
          
          //Sets the given indexes data field to the given data using the given data length.
          Data[p_Index]->set_Data(p_Data, p_Data_Length);
     }
     
     //Sets the name a given index.
     void set_Cell_Name(string p_Name, int p_Target_Index)
     {
          //Prevent bounding errors
          if (p_Target_Index < 0){ return; cout << "\n\n\n\t\t\t\t --==:: WARNING: Set Data Does Nothing (p_Target_Index < 0)"; }
          if (p_Target_Index >= Data_Count){ increase_Data_Index(p_Target_Index + 1); }
          if (Data[p_Target_Index] == NULL){ Data[p_Target_Index] = new c_Cell_Data; }
          
          Data[p_Target_Index]->set_Cell_Name(p_Name);
     }
     
     //Pushes the given cells data to the next cell and puts the given data into the current cell.
     void shove(u_IO_Data * p_Data, int p_Data_Length)
     {
          //Prevent bounding error.
          if (Data_Count == 0)
          {
               push(p_Data, p_Data_Length);
               return;
          }
          
          //Push the currently last cell into the next slot.
          increase_Data_Index(Data_Count + 1);
          Current_Push_Index = Data_Count - 1;
          if (Data[Current_Push_Index] == NULL){ Data[Current_Push_Index] = new c_Cell_Data; }
          
          //Set the next cell to this ones data before overwriting.
          Data[Current_Push_Index]->set_Data(get_Data(Current_Push_Index - 1), get_Data_Length(Current_Push_Index - 1));
          
          //cout << "\n\t CPI:" << Current_Push_Index;
          
          //Set the cell before the last one to the given data.
          Data[Current_Push_Index - 1]->set_Data(p_Data, p_Data_Length);
          
          //cout << "\t CPI-1:" << Current_Push_Index - 1;
     }
     
     //Pushes data into the table in the next available index.
     void push(u_IO_Data * p_Data, int p_Data_Length)
     {
          
          increase_Data_Index(Data_Count + 1);
          Current_Push_Index = Data_Count - 1;
          if (Data[Current_Push_Index] == NULL){ Data[Current_Push_Index] = new c_Cell_Data; }
          //cout << Data[Current_Push_Index];
          Data[Current_Push_Index]->set_Data(p_Data, p_Data_Length);
          
     }
     
     
     //Sets the current push_Index to the next cell.
     void push_Table_Index()
     {
          increase_Data_Index(Data_Count + 1);
          Current_Push_Index = Data_Count - 1;
          if (Data[Current_Push_Index] == NULL){ Data[Current_Push_Index] = new c_Cell_Data; }
     }
     
     //Pushes a unit of data into the next index of the current cell.
     void push_Cell_Data(u_IO_Data p_Data)
     { 
          //cout << "\n\t Data[Current_Push_Index]->push_Data_Unit(p_Data);";
          Data[Current_Push_Index]->push_Data_Unit(p_Data);
     }
     
     //Returns the current push index.
     int get_Current_Push_Index()
     {
          return Current_Push_Index;
     }
     
     //Sets the current push index.
     void set_Current_Push_Index(int p_Index = 0)
     {
          Current_Push_Index = p_Index;
     }
     
     //Sets the properties of the cells data.
     void set_Properties(int p_Dimension_Depths[], int p_Dimension, int p_Index = 0)
     {
          //Prevent bounding errors
          if (p_Index < 0){ return; }
          if (p_Index >= Data_Count){ increase_Data_Index(p_Index + 1); }
          if (Data[p_Index] == NULL){ Data[p_Index] = new c_Cell_Data; }
          
          Data[p_Index]->set_Properties(p_Dimension_Depths, p_Dimension);
     }
     
     //Sets a flag.
     u_IO_Data set_Flag(int p_Index, int p_Flag, u_IO_Data p_State)
     {
          //Prevent bounding errors
          if (p_Index < 0){ return p_State; }
          if (p_Index >= Data_Count){ increase_Data_Index(p_Index + 1); }
          if (Data[p_Index] == NULL){ Data[p_Index] = new c_Cell_Data; }
          
          return Data[p_Index]->set_Flag(p_Flag, p_State);
     }
     
     //Gets a flag state.
     u_IO_Data get_Flag(int p_Index, int p_Flag)
     {
          //Prevent bounding errors
          if (p_Index < 0){ p_Index = 0; }
          if (p_Index >= Data_Count){ increase_Data_Index(p_Index + 1); }
          if (Data[p_Index] == NULL){ Data[p_Index] = new c_Cell_Data; }
          
          return Data[p_Index]->get_Flag(p_Flag);
     }
     
     //Resets the flags for an index.
     void reset_Flags(int p_Index)
     {
          //Prevent bounding errors
          if (p_Index < 0){ p_Index = 0; }
          if (p_Index >= Data_Count){ increase_Data_Index(p_Index + 1); }
          if (Data[p_Index] == NULL){ Data[p_Index] = new c_Cell_Data; }
          
          Data[p_Index]->reset_Flags();
     }
     
     //Sorts the data in a table.
     bool sort_Table_By_Flag(int p_Flag = 0)
     {
          
          
          //The flag for the bubble.
          bool tmp_Sort_Done = 0;
          
          //The temporary storage spot for switching.
          c_Cell_Data * tmp_Cell;
          
          while(!tmp_Sort_Done)
          {
               tmp_Sort_Done = 1;
               for (int cou_Cell=0;cou_Cell<(Data_Count - 1);cou_Cell++)
               {
                    if (get_Flag(cou_Cell, p_Flag).F < get_Flag((cou_Cell +1), p_Flag).F)
                    {
                          tmp_Cell = Data[cou_Cell];
                          Data[cou_Cell] = Data[cou_Cell +1];
                          Data[cou_Cell +1] = tmp_Cell;
                    }
               }
          }
          
          
          //tmp_Cell never used a new;
          //delete tmp_Cell;
          
          return tmp_Sort_Done;
     }
     
     //Gets the dimension of the data.
     int get_Dimension(int p_Index = 0)
     {
          //Prevent bounding errors
          if (p_Index < 0){ return 0; }
          if (p_Index >= Data_Count){ increase_Data_Index(p_Index + 1); }
          if (Data[p_Index] == NULL){ Data[p_Index] = new c_Cell_Data; }
          
          return Data[p_Index]->get_Dimension();
     }
     
     //Gets the depth of a given dimension.
     int get_Depth(int p_Dimension, int p_Index = 0)
     {
          //Prevent bounding errors
          if (p_Index < 0){ return 0; }
          if (p_Index >= Data_Count){ increase_Data_Index(p_Index + 1); }
          if (Data[p_Index] == NULL){ Data[p_Index] = new c_Cell_Data; }
          
          return Data[p_Index]->get_Depth(p_Dimension);
     }
     
     //Get the number of indexes.
     int get_Data_Length(int p_Index = 0)
     {
          //Prevent bounding errors
          if (p_Index < 0){ return 0; }
          if (p_Index >= Data_Count){ increase_Data_Index(p_Index + 1); }
          if (Data[p_Index] == NULL){ Data[p_Index] = new c_Cell_Data; }
          
          return Data[p_Index]->get_Data_Length();
     }
     
     //Returns whether or not the reference flag is set.
     bool get_isReference(int p_Index = 0)
     {
          //Prevent bounding errors
          if (p_Index < 0){ return 0; }
          if (p_Index >= Data_Count){ increase_Data_Index(p_Index + 1); }
          if (Data[p_Index] == NULL){ Data[p_Index] = new c_Cell_Data; }
          
          return Data[p_Index]->get_isReference();
     }
     
     //Returns a pointer to the given cells data.
     c_Cell_Data * get_Reference_To_Cell(int p_Index)
     {
          //Prevent bounding errors
          if (p_Index < 0){ return 0; }
          if (p_Index >= Data_Count){ increase_Data_Index(p_Index + 1); }
          if (Data[p_Index] == NULL){ Data[p_Index] = new c_Cell_Data; }
          
          return Data[p_Index];
     }
     
     //Sets the reference flag.
     bool set_Reference_Flag(int p_Index, bool p_flg_Reference)
     {
          //Prevent bounding errors
          if (p_Index < 0){ return 0; }
          if (p_Index >= Data_Count){ increase_Data_Index(p_Index + 1); }
          if (Data[p_Index] == NULL){ Data[p_Index] = new c_Cell_Data; }
          
          return Data[p_Index]->set_Reference_Table(p_flg_Reference);
     }
     
     //Returns the number of items in the current table.
     int get_Data_Item_Count()
     {
          return Data_Count;
     }
     
     //Outputs a cell.
     void output_Cell(int p_Index = 0, int p_Display = 0)
     {
          if (p_Index < 0 || Data[p_Index] == NULL || p_Index >= Data_Count){ return; }
          Data[p_Index]->output_Data(p_Display);
     }
     
     
     //Outputs all cells.
     void output_All_Data(int p_Display)
     {
          for (int cou_Index=0;cou_Index<Data_Count;cou_Index++)
          {
               cout << "\n          [" << cou_Index << "] ";
               output_Cell(cou_Index, p_Display);
          }
     }
     
};











//This class is the abstract base class for the tables.
class c_Base_Table
{
public:
     
     //This function allows for the setting of an index.
     virtual void set_Index(u_IO_Data * p_Data, int p_Data_Length = 0, int p_Index = 0) = 0;
     
     //This funtion returns the data from the current index.
     virtual u_IO_Data * get_Index(int p_Index = 0) = 0;
     
};









class c_Data_Table: public c_Base_Table
{
private:
     
     //The tables name.
     string Table_Name;
     
     //The ID of the table.
     int TID;
     
     //The Table_Data_Field.
     c_Table_Data_Field Data;
     
public:
     
     c_Data_Table()
     {
          
     }
     
     ~c_Data_Table()
     {
          
     }
     
     //This function sets the tables name.
     void set_Table_Name(string p_Table_Name = "Default Table Name")
     {
          Table_Name = p_Table_Name;
     }
     
     //Gets the table name.
     string get_Table_Name()
     {
          return Table_Name;
     }
     
     //Sets the table ID.
     void set_Table_ID(int p_ID)
     {
          TID = p_ID;
     }
     
     //Gets the Table ID.
     int get_Table_ID()
     {
          return TID;
     }
     
     //--     DATA FIELD MEMBER FUNCTIONS
     
     //Resets a tables data.
     void reset_Data()
     {
          Data.reset();
          
     }
     
     //Resets a table index.
     void reset_Index(int p_Index)
     {
          Data.reset_Index(p_Index);
     }
     
     //Resets flags.
     void reset_Flags(int p_Index)
     {
          Data.reset_Flags(p_Index);
     }
     
     //Returns a cells data.
     u_IO_Data * get_Index(int p_Index = 0)
     {
          //cout << "\n\t\t ~~c_Data_Table::get_Index(p_Index:" << p_Index << ")";
          return Data.get_Data(p_Index);
     }
     
     //Sets a cells data.
     void set_Index(u_IO_Data * p_Data, int p_Data_Length = 0, int p_Index = 0)
     {
          //cout << "\n\t\t ~~c_Data_Table::set_Index(p_Data:" << p_Data << ", p_Data_Length:" << p_Data_Length << ", p_Index:" << p_Index << ")";
          //Sets the given indexes data field to the given data using the given data length.
          Data.set_Data(p_Data, p_Data_Length, p_Index);
     }
     
     //Sets the name a given index.
     void set_Index_Name(string p_Name, int p_Target_Index)
     {
          Data.set_Cell_Name(p_Name, p_Target_Index);
     }
     
     //Pushes data into the table in the next available index.
     void push(u_IO_Data * p_Data, int p_Data_Length)
     {
          //cout << "\n\t\t ~~>c_Data_Table::push(p_Data:" << p_Data << ", p_Data_Length:" << p_Data_Length << ")";
          Data.push(p_Data, p_Data_Length);
     }
     
     
     //Pushes data into the table in the next available index.
     void shove(u_IO_Data * p_Data, int p_Data_Length)
     {
          //cout << "\n\t\t ~~>c_Data_Table::push(p_Data:" << p_Data << ", p_Data_Length:" << p_Data_Length << ")";
          Data.shove(p_Data, p_Data_Length);
     }
     
     //Sets the current push_Index to the next cell.
     void push_Table_Index()
     {
          Data.push_Table_Index();
     }
     
     //Sets the push index.
     void set_Current_Push_Index(int p_Index = 0)
     {
          Data.set_Current_Push_Index(p_Index);
     }
     
     //Returns the current push index.
     int get_Current_Push_Index()
     {
          return Data.get_Current_Push_Index();
     }
     
     //Pushes a unit of data into the next index of the current cell.
     void push_Cell_Data(u_IO_Data p_Data)
     { 
          Data.push_Cell_Data(p_Data);
     }
     
     //Sets the reference state flag.
     bool set_Reference_Flag(int p_Target_Index, bool p_flg_Reference)
     {
          Data.set_Reference_Flag(p_Target_Index, p_flg_Reference);
          return Data.get_isReference(p_Target_Index);
     }
     
     //Sets the properties of the cells data.
     void set_Properties(int p_Dimension_Depths[], int p_Dimension, int p_Index = 0)
     {
          Data.set_Properties(p_Dimension_Depths, p_Dimension, p_Index);
     }
     
     //Gets the dimension of the data.
     int get_Dimension(int p_Index = 0)
     {
          return Data.get_Dimension(p_Index);
     } 
     
     //Sets a flag.
     u_IO_Data set_Flag(int p_Index, int p_Flag, u_IO_Data p_State)
     {
          return Data.set_Flag(p_Index, p_Flag, p_State);
     }
     //Gets a flag state.
     u_IO_Data get_Flag(int p_Index, int p_Flag)
     {
          return Data.get_Flag(p_Index, p_Flag);
     }
     
     //Gets the depth of a given dimension.
     int get_Depth(int p_Dimension, int p_Index = 0)
     {
          return Data.get_Depth(p_Dimension, p_Index);
     }
     
     //Get the number of indexes.
     int get_Data_Length(int p_Index = 0)
     {
          cout << "\n\t\t\t\t c_Table::get_Data_Length(" << p_Index << ")";
          return Data.get_Data_Length(p_Index);
     }
     
     //Gets the number of items in the table.
     int get_Table_Item_Count()
     {
          return Data.get_Data_Item_Count();
     }
     
     //Sorts the table.
     bool sort_Table_By_Flag(int p_Flag)
     {
          return Data.sort_Table_By_Flag(p_Flag);
     }
     
     //Returns whether or not the given cell is a table reference.
     bool get_isReference(int p_Index)
     {
          return Data.get_isReference(p_Index);
     }
     
     //Sets whether or not the cell is a table reference.
     void set_Reference(int p_Index, bool p_flg_Reference = 0)
     {
          Data.set_Reference_Flag(p_Index, p_flg_Reference);
     }
     
     //Returns a pointer to the given cells data.
     c_Cell_Data * get_Reference_To_Cell(int p_Index)
     {
          return Data.get_Reference_To_Cell(p_Index);
     }
     
     //Outputs the table.
     void output_Table(int p_Display = 0)
     {
          cout << Table_Name;
          Data.output_All_Data(p_Display);
     }
     
     //Outputs a cell.
     void output_Specific_Cell(int p_Index)
     {
          Data.output_Cell(p_Index);
     }
     
};











//This class contains the handler for the tables, used by the assembly for proper table referencing.   
class c_Table_Handler
{
private:
     
     //The c_Table array.
     c_Data_Table ** Tables;
     
     //The current number of tables.
     int Table_Count;
     
     
     //Increases the Tables[] index count.
     void increase_Table_Array()
     {
          //Create temporary array.
          c_Data_Table ** tmp_Tables;
          
          //Increment the Table_Count.
          Table_Count++;
          
          //Allocate the new c_Data_Table array.
          tmp_Tables = new c_Data_Table*[Table_Count];
          
          //Copy current data.
          //  (Table_Count - 1) --Because the Table_Count has been incremented.
          for (int cou_Table=0;cou_Table<(Table_Count - 1);cou_Table++)
          {
               tmp_Tables[cou_Table] = Tables[cou_Table];
          }
          
          //Destroy the old array.
          delete Tables;
          Tables = NULL;
          
          //Reallocate the old array.
          Tables = new c_Data_Table*[Table_Count];
          
          //NULL the newly created array.
          for (int cou_Table=0;cou_Table<Table_Count;cou_Table++)
          {
               Tables[cou_Table] = NULL;
          }
          
          //Copy current data.
          //  (Table_Count - 1) --Because the Table_Count has been incremented.
          for (int cou_Table=0;cou_Table<(Table_Count - 1);cou_Table++)
          {
               Tables[cou_Table] = tmp_Tables[cou_Table];
               tmp_Tables[cou_Table] = NULL;
          }
          
          delete [] tmp_Tables;
          tmp_Tables = NULL;
     }
     
     bool b_Tables(int p_Table)
     {
          if (p_Table >= Table_Count){ return 0; }
          if (p_Table < 0){ return 0; }
          return 1;
     }
     
public:
     
     c_Table_Handler()
     {
          Tables = NULL;
          Table_Count = 0;
     }
     
     //new_Table() This function creates a new table and returns a reference to the newly created table.
     int new_Table()
     {
          //Adds a new table to the array.
          increase_Table_Array();
          
          //Initializes the current index.
          Tables[Table_Count - 1] = new c_Data_Table;
          
          //Returns the Index value of the new table.
          return (Table_Count - 1);
     }
     
     //This function gets the table count.
     int get_Table_Count()
     {
          return Table_Count;
     }
     
     //This function sets the tables name.
     void set_Table_Name(string p_Table_Name = "Default Table Name", int p_Table = 0)
     {
          if (!b_Tables(p_Table)){ return; }
          Tables[p_Table]->set_Table_Name(p_Table_Name);
     }
     
     //Gets the table name.
     string get_Table_Name(int p_Table = 0)
     {
          if (!b_Tables(p_Table)){ return "Ghost"; }
          return Tables[p_Table]->get_Table_Name();
     }
     
     //Returns the number of items in the given table.
     int get_Table_Item_Count(int p_Table)
     {
          if (!b_Tables(p_Table)){ return 0; }
          return Tables[p_Table]->get_Table_Item_Count();
     }
     
     //Returns the length data in the given cell for the given table.
     int get_Data_Length(int p_Table, int p_Index)
     {
          //Bounds checking.
          if (!b_Tables(p_Table)){ return 0; }
          
          if (Tables[p_Table] == NULL){ Tables[Table_Count - 1] = new c_Data_Table; }
          
          //If the table index is a refernce then return the length of the reference data.
          if (Tables[p_Table]->get_isReference(p_Index))
          {
               //Returns the target cells data. The first index of the temporary array is a reference to the table, the second index is the indexes address for the target cell.
               return get_Data_Length((Tables[p_Table]->get_Flag(p_Index, 7)).I, (Tables[p_Table]->get_Flag(p_Index, 8)).I);
          }
          else
          {
               //Returns the length of the data in the given index.
               return Tables[p_Table]->get_Data_Length(p_Index);
          }
     }
     
     //Resets the tables data field.
     void reset_Table(int p_Table)
     {  
          if (!b_Tables(p_Table)){ return; }
          //Tables[p_Table]->reset_Data();
          for (int cou_Index=0;cou_Index<Tables[p_Table]->get_Current_Push_Index();cou_Index++)
          {
               /*
               set_Data("", p_Table, cou_Index);
               Tables[p_Table]->reset_Flags(cou_Index);
               Tables[p_Table]->set_Current_Push_Index(0);
               */
               
               Tables[p_Table]->reset_Data();
          }
     }
     
     //Resets a table index.
     void reset_Table_Index(int p_Table, int p_Index)
     {
          if (!b_Tables(p_Table)){ return; }
          Tables[p_Table]->reset_Index(p_Index);
     }
     
     //set_Data(int p_Target_Table, int p_Target_Index, u_IO_Data p_Data[])
     void set_Data(u_IO_Data * p_Data, int p_Data_Length, int p_Table, int p_Target_Index)
     {
          //Bounds checking.
          if (!b_Tables(p_Table)){ return; }
          if (Tables[p_Table] == NULL){ Tables[Table_Count - 1] = new c_Data_Table; }
          
          Tables[p_Table]->set_Index(p_Data, p_Data_Length, p_Target_Index);
     }
     
     //Allows for string setting
     void set_Data(string p_Data, int p_Table, int p_Index)
     {
          //Create the temporary variables.
          u_IO_Data * tmpl_Data;
          tmpl_Data = new u_IO_Data[p_Data.size() + 1];
          
          char * tmpl_Char;
          tmpl_Char = new char[p_Data.size() + 1];
          
          //Read the string into the character array.
          strcpy(tmpl_Char, p_Data.c_str());
          
          //Gather the string into the tmp_Data.
          for (unsigned int cou_Index=0;cou_Index<p_Data.size();cou_Index++)
          {
               tmpl_Data[cou_Index].I = int (tmpl_Char[cou_Index]);
          }
          
          //Push tmp_Data.
          set_Data(tmpl_Data, int (p_Data.size()), p_Table, p_Index);
          
          //Cleanup.
          delete [] tmpl_Char;
          delete [] tmpl_Data;
     }
     
     //Gets a reference to the given table.
     void * get_Table_Handle(int p_Table)
     {
          //Bounds checking.
          if (!b_Tables(p_Table)){ return NULL; }
          if (Tables[p_Table] == NULL){ Tables[Table_Count - 1] = new c_Data_Table; }
          
          return Tables[p_Table];
     }
     
     //set_Data(int p_Target_Table, int p_Target_Index, u_IO_Data p_Data[])
     void set_Data_Int(int * p_Data, int p_Data_Length, int p_Table, int p_Target_Index)
     {
          //Bounds checking.
          if (!b_Tables(p_Table)){ return; }
          if (Tables[p_Table] == NULL){ Tables[Table_Count - 1] = new c_Data_Table; }
          
          u_IO_Data * tmp_Data;
          tmp_Data = new u_IO_Data[p_Data_Length];
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               tmp_Data[cou_Index].I = p_Data[cou_Index];
          }
          
          Tables[p_Table]->set_Index(tmp_Data, p_Data_Length, p_Target_Index);
     }
     
     //set_Data(int p_Target_Table, int p_Target_Index, u_IO_Data p_Data[])
     void set_Data_Int(int p_Data, int p_Table, int p_Target_Index)
     {
          //Bounds checking.
          if (!b_Tables(p_Table)){ return; }
          if (Tables[p_Table] == NULL){ Tables[Table_Count - 1] = new c_Data_Table; }
          
          u_IO_Data tmp_Data;
          tmp_Data.I = p_Data;
          
          Tables[p_Table]->set_Index(&tmp_Data, 1, p_Target_Index);
     }
     
     //Sets a given indexes name to the given name.
     void set_Cell_Name(string p_Name, int p_Table, int p_Target_Index)
     {
          //Bounds checking.
          if (!b_Tables(p_Table)){ return; }
          if (Tables[p_Table] == NULL){ Tables[Table_Count - 1] = new c_Data_Table; }
          
          Tables[p_Table]->set_Index_Name(p_Name, p_Target_Index);
     }
     
     //Pushes data into the table in the next available index.
     void push(u_IO_Data * p_Data, int p_Data_Length, int p_Table)
     {
          //Bounds checking.
          if (!b_Tables(p_Table)){ return; }
          if (Tables[p_Table] == NULL){ Tables[Table_Count - 1] = new c_Data_Table; }
          
          //cout << "\n\t\t ~~>c_Table_Handler::push(p_Data:" << p_Data << ", p_Data_Length:" << p_Data_Length << ", p_Table:" << p_Target_Table << ")";
          Tables[p_Table]->push(p_Data, p_Data_Length);
     } 
    
     //Allows for string pushing rather than u_IO_Data.
     void push(string p_Data, int p_Table)
     {
          //Create the temporary variables.
          u_IO_Data * tmpl_Data;
          tmpl_Data = new u_IO_Data[p_Data.size() + 1];
          
          char * tmpl_Char;
          tmpl_Char = new char[p_Data.size() + 1];
          
          //Read the string into the character array.
          strcpy(tmpl_Char, p_Data.c_str());
          
          //Gather the string into the tmp_Data.
          for (unsigned int cou_Index=0;cou_Index<p_Data.size();cou_Index++)
          {
               tmpl_Data[cou_Index].I = int (tmpl_Char[cou_Index]);
          }
          
          //Push tmp_Data.
          push(tmpl_Data, int (p_Data.size()), p_Table);
          
          //Cleanup.
          delete [] tmpl_Char;
          delete [] tmpl_Data;
     }
     
    
     //Pushes data into the table in the next available index.
     void shove(u_IO_Data * p_Data, int p_Data_Length, int p_Table)
     {
          //Bounds checking.
          if (!b_Tables(p_Table)){ return; }
          if (Tables[p_Table] == NULL){ Tables[Table_Count - 1] = new c_Data_Table; }
          
          //cout << "\n\t\t ~~>c_Table_Handler::push(p_Data:" << p_Data << ", p_Data_Length:" << p_Data_Length << ", p_Table:" << p_Target_Table << ")";
          Tables[p_Table]->shove(p_Data, p_Data_Length);
     }
  
     //Allows for string pushing rather than u_IO_Data.
     void shove(string p_Data, int p_Table)
     {
          //Create the temporary variables.
          u_IO_Data * tmp_Data;
          tmp_Data = new u_IO_Data[p_Data.size() + 1];
          
          char * tmp_Char;
          tmp_Char = new char[p_Data.size() + 1];
          
          //Read the string into the character array.
          strcpy(tmp_Char, p_Data.c_str());
          
          //Gather the string into the tmp_Data.
          for (unsigned int cou_Index=0;cou_Index<p_Data.size();cou_Index++)
          {
               tmp_Data[cou_Index].I = int (tmp_Char[cou_Index]);
          }
          
          //Push tmp_Data.
          shove(tmp_Data, int (p_Data.size()), p_Table);
          
          //Cleanup.
          delete [] tmp_Char;
          delete [] tmp_Data;
     }
     
     
     //Sets the current push_Index to the next cell.
     void push_Table_Index(int p_Table)
     {
          if (!b_Tables(p_Table)){ return; }
          Tables[p_Table]->push_Table_Index();
     }
     
     //Pushes a unit of data into the next index of the current cell.
     void push_Cell_Data(int p_Table, u_IO_Data p_Data)
     { 
          if (!b_Tables(p_Table)){ return; }
          Tables[p_Table]->push_Cell_Data(p_Data);
     }
     
     //Pushes data into the table in the next available index.
     void push_NR(void * p_Data, int p_Target_Table)
     {
          u_IO_Data tmp_IO;
          tmp_IO.NR = p_Data;
          
          //Push the data.
          push(&tmp_IO, 1, p_Target_Table);
     }
     
     //Returns the current push index for the given table.
     int get_Current_Push_Index(int p_Table)
     {
          if (!b_Tables(p_Table)){ return -1; }
          return Tables[p_Table]->get_Current_Push_Index();
     }
     
     //Sets the data in a cell as a reference, takes a target table and target index.
     void set_Reference(int p_Data_Table, int p_Data_Index, int p_Target_Table, int p_Target_Index)
     {
          
          
          if (!b_Tables(p_Data_Table)){ return; }
          
          //cout << "\n\t\t c_Table_Handler::set_Reference(" << p_Data_Table << ", " << p_Data_Index << ", " << p_Target_Table << ", " << p_Target_Index << ")"; cout.flush();
          
          u_IO_Data tmp_Data[10];
          
          tmp_Data[0].I = p_Target_Table;
          tmp_Data[1].I = p_Target_Index;
          
          //cout << "\n\n\n\t 0:" << tmp_Data[0].I << " 1:" << tmp_Data[1].I; cout.flush();
          Tables[p_Data_Table]->set_Flag(p_Data_Index, 7, tmp_Data[0]);
          Tables[p_Data_Table]->set_Flag(p_Data_Index, 8, tmp_Data[1]);
          
          set_Reference_Flag(p_Data_Table, p_Data_Index, true);
     }
     
     //Gets a reference to a given cell.
     void * get_Reference_To_Cell(int p_Table, int p_Cell)
     {
          if (!b_Tables(p_Table)){ return NULL; }
          return Tables[p_Table]->get_Reference_To_Cell(p_Cell);
     }
     
     //Sets the reference state flag.
     bool set_Reference_Flag(int p_Table, int p_Target_Index, bool p_flg_Reference)
     {
          //cout << "\n\t\t c_Table_Handler::set_Reference_Flag(" << p_Table << ", " << p_Target_Index << ")"; cout.flush();
          //Bounds checking.
          if (!b_Tables(p_Table)){ return false; }
          if (Tables[p_Table] == NULL){ Tables[Table_Count - 1] = new c_Data_Table; }
          
          Tables[p_Table]->set_Reference_Flag(p_Target_Index, p_flg_Reference);
          return true;
     }
     
     //Gets the state of the reference state flag.
     bool get_isReference(int p_Target_Table, int p_Target_Index)
     {
          //Bounds checking.
          if (!b_Tables(p_Target_Table)){ return 0; }
          if (Tables[p_Target_Table] == NULL){ Tables[Table_Count - 1] = new c_Data_Table; }
          
          return Tables[p_Target_Table]->get_isReference(p_Target_Index);
     }
     
     //get_Data(int p_Target_Table, int p_Target_Index)
     u_IO_Data * get_Data(int p_Target_Table, int p_Target_Index)
     {
          //Bounds checking.
          if (!b_Tables(p_Target_Table)){ return NULL; }
          if (Tables[p_Target_Table] == NULL){ Tables[Table_Count - 1] = new c_Data_Table; }
          
          if (Tables[p_Target_Table]->get_isReference(p_Target_Index))
          {
               //Returns the target cells data. The first index of the temporary array is a reference to the table, the second index is the indexes address for the target cell.
               return get_Data((Tables[p_Target_Table]->get_Flag(p_Target_Index, 7)).I, (Tables[p_Target_Table]->get_Flag(p_Target_Index, 8)).I);
          }
          else
          {
               //Returns the data in the given index.
               return Tables[p_Target_Table]->get_Index(p_Target_Index);
          }
     }
     
     //Gets a single data item and returns it as an integer.
     int get_Data_Item_Int(int p_Target_Table, int p_Target_Index, int p_Target_Item = 0)
     {
          //Bounds checking.
          if (!b_Tables(p_Target_Table)){ return 0; }
          if (Tables[p_Target_Table] == NULL){ Tables[Table_Count - 1] = new c_Data_Table; }
          
          if (Tables[p_Target_Table]->get_isReference(p_Target_Index))
          {
               //Temporary cell reference.
               u_IO_Data * tmp_Data;
               
               //Gets the reference.
               tmp_Data = Tables[p_Target_Table]->get_Index(p_Target_Index);
               
               //Returns the target cells data. The first index of the temporary array is a reference to the table, the second index is the indexes address for the target cell.
               return get_Data_Item_Int(tmp_Data[0].I, tmp_Data[1].I, p_Target_Item);
          }
          else
          {
               //Returns the data in the given index.
               
               //Temporary cell reference.
               u_IO_Data * tmp_Data;
               
               //Gets the reference.
               tmp_Data = Tables[p_Target_Table]->get_Index(p_Target_Index);
               
               return tmp_Data[p_Target_Item].I;
          } 
     }
     
     
     //Sets a flag.
     float set_Flag(int p_Table, int p_Index, int p_Flag, u_IO_Data p_State)
     {
          if (!b_Tables(p_Table)){ return 0; }
          return Tables[p_Table]->set_Flag(p_Index, p_Flag, p_State).F;
     }
     //Gets a flag state.
     float get_Flag(int p_Table, int p_Index, int p_Flag)
     {
          if (!b_Tables(p_Table)){ return 0; }
          return Tables[p_Table]->get_Flag(p_Index, p_Flag).F;
     }
     
     
     //Sorts the given table.
     bool sort_Table_By_Flag(int p_Table, int p_Flag)
     {
          if (!b_Tables(p_Table)){ return 0; }
          return Tables[p_Table]->sort_Table_By_Flag(p_Flag);
     }
     
     //Sorts all the tables by a flag.
     bool sort_All_Tables_By_Flag(int p_Flag)
     {
          //Loops through all of the tables and sorts them by a given flag.
          for (int cou_Index=0;cou_Index<Table_Count;cou_Index++)
          {
               sort_Table_By_Flag(cou_Index, p_Flag);
          }
          
          return 1;
     }
     
     //Updates the data for references in a given table.
     void update_Reference_Data(int p_Table)
     {
          //Bounds checking.
          if (!b_Tables(p_Table)){ return; }
          
          //Temporary data.
          u_IO_Data tmp_Data[10];
          
          //For every cell get_is_Reference()
          for (int cou_Cell=0;cou_Cell<Tables[p_Table]->get_Table_Item_Count();cou_Cell++)
          {
               if (Tables[p_Table]->get_isReference(cou_Cell))
               {
                    //Get reference coordinates.
                    tmp_Data[0] = Tables[p_Table]->get_Flag(cou_Cell, 7);
                    tmp_Data[1] = Tables[p_Table]->get_Flag(cou_Cell, 8);
                    
                    //Bounds checking.
                    if (tmp_Data[0].I < 0 || tmp_Data[0].I >= Table_Count){ continue; }
                    if (tmp_Data[1].I < 0 || tmp_Data[1].I >= Tables[tmp_Data[0].I]->get_Table_Item_Count()){ continue; }
                    
                    //Get data length.
                    tmp_Data[2].I = Tables[tmp_Data[0].I]->get_Data_Length(tmp_Data[1].I);
                    
                    Tables[p_Table]->set_Index(Tables[tmp_Data[0].I]->get_Index(tmp_Data[1].I), tmp_Data[2].I);
               }
          }
               //Checks if the cell is a reference.
               
               //If so then update the data for the cell to the data in the referenced cell.
     }
     
     
     //Outputs a given Table.
     /*
      * 
      *      --TABLE_NAME
      *        +TABLE_DATA
      * 
      */
      
      
     
     //Outputs a cell in a given table..
     void output_Specific_Cell(int p_Target_Table, int p_Index)
     {
          if (!b_Tables(p_Target_Table)){ return; }
          Tables[p_Target_Table]->output_Specific_Cell(p_Index);
     }
      
      void output_Table(int p_Target_Table = 0, int p_Display = 0)
      {
           if (!b_Tables(p_Target_Table)){ return; }
           update_Reference_Data(p_Target_Table);
           Tables[p_Target_Table]->output_Table(p_Display);
      }
      
      //Outputs all of the tables.
      void output_All_Tables(int p_Display = 0)
      {
           cout << "\n    ~TABLES";
           for (int cou_Index=0;cou_Index<Table_Count;cou_Index++)
           {
                cout << "\n\n      [" << cou_Index << "]";
                output_Table(cou_Index, p_Display);
           }
      }
};