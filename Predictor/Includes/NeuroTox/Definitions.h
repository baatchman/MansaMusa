//for the leakage charge have each node contain an int that keeps track of its last update tick


//this set of globals is to control the neurotoxicity settings, such as charge left, right, or both

//--     NODE SETTINGS

//charges both legs as normal
bool setting_Charge_Both_Legs = 1;

//charges only the left leg when charging
bool setting_Charge_Left_Leg = 0;

//charges only the right leg when charging
bool setting_Charge_Right_Leg = 0;

//resets the membrane potential to 0 when charging
bool setting_Reset_Membrane_Potential = 0;

//--     CONSTRUCT SETTINGS

//reinforces the nodes when charging
bool setting_Reinforce_On_Charge = 0;

//reinforces the nodes when building
bool setting_Reinforce_On_Build = 1;

//builds the current input set when charging
//when on the network will reinforcing on build while charging, obvious here but will have to put into menu
bool setting_Build_When_Charging = 1;

//the tier to start charging the construct on
int setting_Construct_Charging_Tier = 5;

//the rating system to use, 0 == [0], 1 == [1], 2 == [0] / [1], 3 == [0] - [1]
int setting_Rating = 2;

//whether or not to recompute the node postsynaptic charges on build
//bool setting_Evaluate_Postsynaptic_Charges = 0;

//used for the binary tree search for the proper node references to input into the lowest CAN tier for further evaluation
union u_IO_Data
{
     //I for integer 
     int I;
     
     //F for float
     float F;
     
     //NR for Node Reference, at this level only the references should be checked, will have to typecast the reference if need be
     void * NR;
     
     //holds the character interpretation of the data
     char C[4];
};



union u_IO
{
     int INT;
     float FLOAT;
     //used when the inputs are the treetops on another construct
     void * NODE;
};





//----====================================----
//--==      VIRTUAL CLASS DEFINITIONS     ==--
//----====================================----










//--     Commonly used small goodies.


int char2int(char p_Char[], int p_Length)
{
     string tmp_String = p_Char;
     int tmp_Num = 0;
     int tmp_EIP = p_Length - 1;
     int cur_Multiple = 1;
     for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
     {
          tmp_Num += ((int (p_Char[tmp_EIP - cou_Index]) - 48) * cur_Multiple);
          cur_Multiple *= 10;
     }
     
     return tmp_Num;
}

int char2int(u_IO_Data p_Char[], int p_Length)
{
     int tmp_Num = 0;
     int tmp_EIP = p_Length - 1;
     int cur_Multiple = 1;
     for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
     {
          tmp_Num += ((p_Char[tmp_EIP - cou_Index].I - 48) * cur_Multiple);
          cur_Multiple *= 10;
     }
     
     return tmp_Num;
}


int get_int_Place(int p_Num)
{
     if (p_Num == 0){ return 1; }
     int tmp_Place = 0;
     
     while(p_Num)
     {
          tmp_Place++;
          p_Num = p_Num / 10;
     }
     
     return tmp_Place;
}



void int2char(int p_Int, char p_Char[4])
{
     p_Char[0] = '0';
     p_Char[1] = '0';
     p_Char[2] = '0';
     p_Char[3] = '0';
     
     int tmp_Num = 0;
     int tmp_Place = get_int_Place(p_Int);
     
     for (int cou_Index=3;cou_Index>=(4 - tmp_Place);cou_Index--)
     {
          //Get the current digit.
          tmp_Num = (p_Int % 10);
          
          //Convert to ASCII
          p_Char[cou_Index] = (tmp_Num + 48);
          
          //Reduce the integer for the next digit to be collected.
          p_Int *= .1;
     }
     
     return;
}


int int2data(int p_Int, u_IO_Data p_Data[25])
{
     //Clear the data array.
     for (int cou_Index=0;cou_Index<25;cou_Index++)
     {
          p_Data[cou_Index].I = 0;
     }
     
     int tmp_Num = 0;
     int tmp_Place = get_int_Place(p_Int);
     
     //cout << "\n\n\t " << p_Int;
     
     for (int cou_Index=(tmp_Place - 1);cou_Index>=0;cou_Index--)
     {
          //Get the current digit.
          tmp_Num = (p_Int % 10);
          
          //Convert to ASCII
          p_Data[cou_Index].I = (tmp_Num + 48);
          
          //Reduce the integer for the next digit to be collected.
          p_Int *= .1;
          
          //cout << "\n\t p_Data[" << cou_Index << "]:" << p_Data[cou_Index].I;
     }
     
     
     return tmp_Place;
}


int float2data(float p_Float, u_IO_Data p_Data[25])
{
     //Clear the data array.
     for (int cou_Index=0;cou_Index<25;cou_Index++)
     {
          p_Data[cou_Index].I = 0;
     }
     
     int tmp_Num = 0;
     int tmp_Int = 0;
     int tmp_Place_Whole = get_int_Place(int (p_Float));
     
     float tmp_Remainder = (p_Float - int (p_Float));
     for (int cou_Index=0;cou_Index<6;cou_Index++)
     {
          tmp_Remainder *= 10;
     }
     
     //cout << "\n\n\t p_Float:" << p_Float << "  tmp_Remainder:" << tmp_Remainder << "                                                                            ";
     
     int tmp_Place_Remainder = get_int_Place(int (tmp_Remainder));
     
     //cout << "\n\n\t " << p_Int;
     
     tmp_Num = int (p_Float);
     for (int cou_Index=(tmp_Place_Whole - 1);cou_Index>=0;cou_Index--)
     {
          //Get the current digit.
          tmp_Int = (tmp_Num % 10);
          
          //Convert to ASCII
          p_Data[cou_Index].I = (tmp_Int + 48);
          
          //Reduce the integer for the next digit to be collected.
          tmp_Num *= .1;
          
          //cout << "\n\t p_Data[" << cou_Index << "]:" << p_Data[cou_Index].I;
     }
     
     //Get the decimal.
     p_Data[tmp_Place_Whole].I = int ('.');
     
     //Gather the remainder.
     tmp_Num = int (tmp_Remainder);
     //cout << "\n tmp_Num:" << tmp_Num << " for_get_Remainder:";
     for (int cou_Index=(tmp_Place_Whole + tmp_Place_Remainder);cou_Index>(tmp_Place_Whole);cou_Index--)
     {
          //Get the current digit.
          tmp_Int = (tmp_Num % 10);
          
          //Convert to ASCII
          p_Data[cou_Index].I = (tmp_Int + 48);
          //cout << " " << cou_Index << "." << tmp_Num;
          
          //Reduce the integer for the next digit to be collected.
          tmp_Num *= .1;
          
     }
     
     return tmp_Place_Whole + tmp_Place_Remainder + 1;
}


void char2u_IO_Data(char * p_Data, u_IO_Data * p_Converted, int p_Data_Length)
{
     for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
     {
          p_Converted[cou_Index].I = int (p_Data[cou_Index]);
     }
}

void clear_Char(char * p_Char, int p_Length)
{
     for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
     {
          p_Char[cou_Index] = char (0);
     }
}

void clear_u_IO_Data(u_IO_Data * p_Data, int p_Length)
{
     for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
     {
          p_Data[cou_Index].I = 0;
     }
}



