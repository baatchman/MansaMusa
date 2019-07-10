


//   This class holds the CAN for any sized construct.
// When a network is being built or discharged, a temporary 
// construct must be created to keep track of what nodes are
// currently being manipulated. The CAN allows for all of the nodes 
// to be public. This way it is easy to use the CAN in an algorithm
// when referencing. The CAN is an array of references, each node in
// the CAN holds a reference to the lower and upper CAN nodes, as well
// as a reference to the current c_Poly_Node.
class c_Poly_CAN_Node
{
public:
     
     //The node that this CAN is currently holding.    
     void * Current_Target_Node;
     
     //The default constructor initializes the nodes as they are created.
     c_Poly_CAN_Node()
     {
          //Initialize the data members.
          
          //There currently is no target node.
          Current_Target_Node = NULL;
     }
     
};