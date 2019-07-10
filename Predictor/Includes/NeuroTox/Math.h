

//Gets the absolute value of a given int.
int absolute(int p_Int)
{
     if (p_Int < 0){ p_Int *= -1; }
     return p_Int;
}

//Gets the absolute value of a given float.
float absolutef(float p_Float)
{
     if (p_Float < 0){ p_Float *= -1; }
     return p_Float;
}

//Gets the power of an integer.
int pow_Int(int p_Number, int p_Factor, int p_Power)
{
     if (p_Power == 0){ return 1; }
     if (p_Power == 1){ return p_Number; }
     return pow_Int((p_Number * p_Factor), p_Factor, (p_Power -1));
}
