/*
 *   This is the main header file for the NeuroTox library. The NeuroTox library contains
 * classes that allow one to work with memory trees through a black box. Memory trees can
 * be built, queried, linked together, and hooked to ports for IO.
*/

/*
 *   The new algorithm has the dimensions hardcoded for efficiency.
 *
*/

//--     Includes for this library.


//Includes for the vision class.
#include "c_Vision.h"

//Includes for the scaffolding used to rebuild the node network.
#include "Scaffold/c_Scaffold_Node.h"
#include "Scaffold/c_Scaffold.h"


//Includes for the MT_Nodes.
//#include "MT_Nodes/c_MT_Base_Node.h"
//#include "MT_Nodes/c_MT_Simple_Node.h"
//#include "MT_Nodes/c_MT_RC_Node.h"
//#include "MT_Nodes/c_MT_State_Node.h"
//#include "MT_Nodes/c_MT_RC_State_Node.h"


//Includes for the nodes.
#include "Nodes/c_Base_Node.h"
#include "Nodes/c_Simple_Node.h"
#include "Nodes/c_RC_Node.h"
#include "Nodes/c_State_Node.h"
#include "Nodes/c_RC_State_Node.h"


//Includes for the MT_treetop nodes.
//#include "MT_Nodes/Treetop_Nodes/c_MT_Treetop_Simple_Node.h"
//#include "MT_Nodes/Treetop_Nodes/c_MT_Treetop_RC_Node.h"
//#include "MT_Nodes/Treetop_Nodes/c_MT_Treetop_State_Node.h"
//#include "MT_Nodes/Treetop_Nodes/c_MT_Treetop_RC_State_Node.h"


//Includes for the treetop nodes.
#include "Nodes/Treetop_Nodes/c_Treetop_Simple_Node.h"
#include "Nodes/Treetop_Nodes/c_Treetop_RC_Node.h"
#include "Nodes/Treetop_Nodes/c_Treetop_State_Node.h"
#include "Nodes/Treetop_Nodes/c_Treetop_RC_State_Node.h"


//These are being replaced by the buffer classes.
//The binary trees for the node network.
#include "Bin_Trees/c_State_Node_Bin_Tree.h"
#include "Bin_Trees/c_Fractal_State_Node_Bin_Tree.h"
//#include "Bin_Trees/c_MT_Fractal_State_Node_Bin_Tree.h"

//Includes for the buffers
#include "Buffers/c_Charging_Buffer.h"
#include "Buffers/c_Torch_Buffer.h"

//Includes for the node network.
//#include "Processors/c_MT_Base_Node_Network.h"
#include "Processors/c_Base_Node_Network.h"

//Includes for the Number Constructs Processor.
#include "Processors/Bin_Table/c_Bin_Tree_Int.h"
#include "Processors/Bin_Table/c_Bin_Tree_Float.h"
#include "Bin_Trees/c_Number_Node_Bin_Tree.h"
#include "Processors/Bin_Table/c_Bin_Table_Int.h"
#include "Processors/Bin_Table/c_Bin_Table_Float.h"

//Includes for the CAN
//#include "Processors/c_MT_CAN.h"
#include "Processors/c_CAN.h"

//Includes for the Construct.
#include "Constructs/c_Base_Construct.h"
//#include "Constructs/c_MT_Construct.h"
#include "Constructs/c_Construct.h"
#include "Constructs/c_Int_Construct.h"
#include "Constructs/c_Float_Construct.h"

//Includes for the Assembly.
#include "Assemblies/c_Assembly.h"
#include "Assemblies/c_BCP.h"
#include "Assemblies/c_MSC.h"
#include "Assemblies/c_BCMSCP.h"

//Upper tier assemblies.
#include "Assemblies/c_MAC.h"
#include "Assemblies/c_MAC_FLAT.h"
#include "Assemblies/c_MAC_COLUMN.h"

//Includes for the Gathering.
#include "c_Gathering.h"

//Includes for the Command Script, contains the ability to perform callbacks upon the lesses classes.
#include "Command_Text\c_Command_Parser.h"

//Includes for the Overlord, the overlord functions as a controller for the gatherings and an interface for outside manipulation.
#include "c_Overlord.h"





