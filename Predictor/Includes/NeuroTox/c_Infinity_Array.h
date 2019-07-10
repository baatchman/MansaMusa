//   This class holds the infinity array, which is a class
// that handles an array of elements that can grow to any length needed.
// When declaring an inifinty array you must specify a datatype to use.

template <typename t_Type>
class c_Infinity_Array
{
private:

public:

          //The t_Type array to hold all of the variable references.
          t_Type ** Index;
          
          //The current number of variables in the pointer array. Used as the max as well.
          int Index_Item_Count;
          
          c_Infinity_Array()
          {
               Index = NULL;
               Index_Item_Count = 0;
          }
          
          //Submit a variable to the array.
          
          //Get an index.
          
          //Set an index.
          
          //push()
          void push(t_Type * p_Item)
          {
               //Make room for the new entry.
               increase_Index();
               
               //Write the new item down in the list.
               Index[Index_Item_Count - 1] = p_Item;
          }
          
          //pop()
          void pop()
          {
               
          }
          
          //Set the given index. If the array is not large enough it is resized.
          void set_Index(int p_Index, t_Type p_Data)
          {
               if (p_Index >= Index_Item_Count){ increase_Index(p_Index); }
               
               //Sets the data.
               Index[p_Index] = *p_Data;
          }
          
          //Returns a reference to the given index, if the index does not yet exist it is created.
          t_Type * get_Index(int p_Index)
          {
               cout << "\n\t\t ~~c_Infinity_Array::get_Index(" << p_Index << ")";
               //Checks for bounding errors.
               if (p_Index >= Index_Item_Count){ increase_Index(p_Index); }
               
               return Index[p_Index];
          }
          
          //Resize the Index array.
          void increase_Index(int p_Index)
          {
               //If the given index is less than the current index, return.
               if (p_Index <= Index_Item_Count){ return; }
               
               //Create temporary array.
               t_Type ** tmp_Index;
               
               //Allocate the new t_Type array.
               tmp_Index = new t_Type*[p_Index];
               
               //NULL the newly created array.
               for (int cou_Index=0;cou_Index<p_Index;cou_Index++){
                    tmp_Index[cou_Index] = NULL;
               }
               
               //Copy current data.
               for (int cou_Index=0;cou_Index<Index_Item_Count;cou_Index++){
                    tmp_Index[cou_Index] = Index[cou_Index];
               }
               
               //Destroy the old array.
               delete Index;
               
               //Point the old array handle to the current array.
               Index = tmp_Index;
               
               //Increment the Index_Item_Count.
               Index_Item_Count = p_Index;
          }
          
          //Resize the Index array.
          void increment_Index()
          {
               //Create temporary array.
               t_Type ** tmp_Index;
               
               //Increment the Index_Item_Count.
               Index_Item_Count++;
               
               //Allocate the new t_Type array.
               tmp_Index = new t_Type*[Index_Item_Count];
               
               //Copy current data.
               //  (Index_Item_Count - 1) --Because the Index_Item_Count has been incremented.
               for (int cou_Index=0;cou_Index<(Index_Item_Count - 1);cou_Index++){
                    tmp_Index[cou_Index] = Index[cou_Index];
               }
               
               //Destroy the old array.
               delete Index;
               
               //Point the old array handle to the current array.
               Index = tmp_Index;
          }
          
          //Outputs all item in the array.
          void output_Items()
          {
               for (int cou_Index=0;cou_Index<Index_Item_Count;cou_Index++){
                    cout << " [" << cou_Index << "]" << Index[cou_Index];
               }
          }
}; 

