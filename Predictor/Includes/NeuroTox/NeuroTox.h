/*
 *   This is the main header file for the NeuroTox library. The NeuroTox library contains
 * classes that allow one to work with memory trees through a black box. Memory trees can
 * be built, queried, linked together, and hooked to ports for IO.
*/

//--     Includes for this library.

//Icludes for declarations.
#include "Declarations.h"

//Includes for definitions
#include "Definitions.h"

//Includes for the infinity array.
#include "c_Infinity_Array.h"

//Icludes for math functions.
#include "Math.h"

//Includes for the Class tables.
#include "c_Table.h"

//Includes for the binary trees and other structures.
#include "Bin_Trees/c_State_Bin_Tree.h"

//Includes for IO
#include "c_IO.h"
#include "c_Console.h"

//Includes for the network structure.
#include "c_Poly_Node.h"
#include "c_Poly_Network.h"

//Includes for the CAN
#include "c_Poly_CAN_Node.h"
#include "c_Poly_CAN.h"

//Includes for the assembly
#include "c_Poly_Assembly.h"


/*
 * :submit_Input_Set
 * 
 * 
 * Input Types
 *   -Normal: Accepts an array for input.
 *   -Treetop: Has an array of construct treetops for the input.
 *   -Treetop_Chrono: An array of construct treetops and their previous states.
 *   -Normal_Chrono: Accepts an array for input and has the previous submissions in an array.  
 * 
*/



/*
 *  class c_Map_Tile
 *   -update_Tile(int p_Tile_Color, int p_Tile_Character)
 *   -output_Tile()  
 * 
 *  class c_Map
 *   -c_Map_Tile ** Map
 *   -output_Map
*/