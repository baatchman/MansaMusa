/*****************************************************************  
**                                                              **
**                                                              **
**     c_Console                                                **
**                                                              **
**     This class is for the console handling functions         **
**                                                              **
**                                                              **
**     Functions:                                               **
**                                                              **
**          /set_Console_Color -This functions takes two        **
**               arguments, one for background color and        **
**               one for foreground color                       **
**                                                              **
**                                                              **
**                                                              **
**     Classes:                                                 **
**                                                              **
*****************************************************************/



//console color thingy
HANDLE  hConsole;
int bg;//current background
int fg;//current foreground


void set_Console_Color(int p_bg=0, int p_fg=0)
{
     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     bg = p_bg;
     fg = p_fg;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}

void output_Console_Character_Colored(int p_bg, int p_fg, char p_Char)
{
     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     cout << p_Char;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}


void output_Console_Int_Colored(int p_bg, int p_fg, int p_Int)
{
     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     cout << p_Int;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
     set_Console_Color(bg, fg);
}

void output_Console_Float_Colored(int p_bg, int p_fg, float p_Float)
{
     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     cout << p_Float;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
     set_Console_Color(bg, fg);
}

void output_Console_Float_Colored_With_Additional_Charge_For_Precision_Setting(int p_bg, int p_fg, float p_Float, float p_Precision = .0000001)
{
     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     p_Float += p_Precision;
     cout << p_Float;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
     set_Console_Color(bg, fg);
}


void output_Console_Float_Colored_With_Precision_5(int p_bg, int p_fg, float p_Float)
{
     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     int tmp = int (p_Float * 100000);
     p_Float = float (tmp * .000001);
     cout << p_Float;
     
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
     set_Console_Color(bg, fg);
}


void console_Move_Cursor_To_Given_X_And_Y( short int p_X, short int p_Y )
{
     //system("PAUSE > NULL");
    COORD tmp_Coordinate = { p_X, p_Y };
    SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), tmp_Coordinate );
}



//Outputs a line in the console window from the first set of coordinates to the second set of coordinates.
void console_Draw_Line(int p_X1, int p_Y1, int p_X2, int p_Y2, int p_Color = 1, int p_Char = 177, float p_Percentage = 1.0)
{
     //Bounds checking for the console window.
     if (p_X1 > 9998 || p_Y1 > 9998 || p_X2 > 9998 || p_Y2 > 9998){ return; }
     
     //Get the rise over run.
     float tmp_Rise = p_Y2 - p_Y1;
     float tmp_Run = p_X2 - p_X1;
     
     //The increment for rise and run.
     float tmp_Rise_Inc = 0;
     float tmp_Run_Inc = 0;
     
     //The temporary positions.
     float tmp_X = 0;
     float tmp_Y = 0;
     
     if (absolute(tmp_Rise) >= absolute(tmp_Run))
     {
          //If the tmp_Rise is larger then reduce by the rise.
          tmp_Rise_Inc = tmp_Rise / absolutef(tmp_Rise);
          tmp_Run_Inc = tmp_Run / absolutef(tmp_Rise);
          
//          cout << "\n ~Ri >= ~Ru " << tmp_Rise_Inc << "\\" << tmp_Run_Inc;
          
          for (int cou_Y=0;cou_Y<(int (absolute(tmp_Rise) * p_Percentage));cou_Y++)
          {
               //Increase the current position.
               tmp_X += tmp_Run_Inc;
               tmp_Y += tmp_Rise_Inc;
               
               //Output the current position.
               console_Move_Cursor_To_Given_X_And_Y(tmp_X + p_X1, tmp_Y + p_Y1);
               output_Console_Character_Colored(0, p_Color, p_Char);
          }
     }
     if (absolute(tmp_Rise) < absolute(tmp_Run))
     {
          //If the tmp_Run is larger then reduce by the tmp_Run.
          tmp_Rise_Inc = tmp_Rise / absolutef(tmp_Run);
          tmp_Run_Inc = tmp_Run / absolutef(tmp_Run);
          
//          cout << "\n ~Ri < ~Ru " << tmp_Rise_Inc << "\\" << tmp_Run_Inc;
          
          for (int cou_X=0;cou_X<absolute(int ((tmp_Run) * p_Percentage));cou_X++)
          {
               //Increase the current position.
               tmp_X += tmp_Run_Inc;
               tmp_Y += tmp_Rise_Inc;
               
               //Output the current position.
               console_Move_Cursor_To_Given_X_And_Y(tmp_X + p_X1, tmp_Y + p_Y1);
               output_Console_Character_Colored(0, p_Color, p_Char);
          }
     }
}


















/*
 *   This class holds the information for a console window that different objects ttach to. for example the one attached to the node network
 * would hold the postition for the graphical output of the individual polyu nodes to use when outputting to the screen using console coordinates.
 * used dynamic arrays of coordinates to be shared among different objects and referenced by all to allow the constroller to manipulate the posisions of 
 * its members.
*/