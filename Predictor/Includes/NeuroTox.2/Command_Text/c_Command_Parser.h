//This file contains the command parser whose job it is to accept input in the form of command text and parse it accordingly.
//Tables can be submitted for parsing on a row by row basis with each cell containing an argument. Or perhaps on a row by row basis with comments being in the secondary cells.

class c_Command_Parser
{
protected:
     
     //The whole string to collect during rcon.
     string Command_String;
     
     //The table used to build scripts before saving them to a dedicated table.
     c_Table_1D * CMD_Sandbox;
     
     //The flag for the current variable when reading in tokens.
     int flg_Var_Pos;
     
     //This flag represents whether or not there are undefined variables being used resulting in NULL values.
     int flg_Undefined;
     
     //The flag allows for string inputs multiple tokens long.
     int flg_String;
     
     //Holds all of the variables and their values.
     c_Lookup_Tree Var_Lookup_Tree;
     
     //Holds the CMD tables.
     c_Table_1D * CMD_Script_Registry;
     c_Lookup_Tree CMD_Script_Registry_Lookup_Tree;
     
     //The table handler linking this to the tables.
     c_Table_Handler * Tables;
     
     c_Base_Gathering * Gathering;
     
public:
     
     c_Command_Parser(c_Table_Handler * p_Tables, c_Base_Gathering * p_Gathering)
     {
          //Set the table handler.
          Tables = p_Tables;
          
          //Set the gathering this parser will be handling.
          Gathering = p_Gathering;
          
          
          CMD_Sandbox = Tables->register_New_Table_1D("CMD_Sandbox");
          CMD_Script_Registry = Tables->register_New_Table_1D("CMD_Script_Registry");
          
          Command_String = "";
          
          
          eval_Command_String("load_CMD_script autoexec");
          eval_Command_String("eval_CMD_table autoexec");
          
     }
     
     ~c_Command_Parser()
     {
     }
     
     //Allows for live input into the parser for debug and root control.
     void rcon()
     {
          //Until the string "return" is found do not exit the command loop.
          cout << "\n\n\t\t NeuroTox rcon";
          cout << "\n\t\t   ->help -outputs information on available commands.";
          cout << "\n\t\t   ->return -exits the rcon.";
          
          
          while (1)
          {
               cout << "\n\n";
          
               cout << "\n\n\t ->";
               
               getline(cin, Command_String);
               
               if (Command_String == "return"){ return; }
               if (Command_String == "help" || Command_String == "?"){ system("type Command_Text_Reference.dat"); }
               
               eval_Command_String(Command_String);
               
               Tables->output_Table(0);
          }
     }
     
     //This is used to search the table for labels and and submit them with their index to the Label_Tree.
     //Each instance of the table parser has its own labels.
     string labelize_Command_Table(c_Table_1D * p_Command_Table, c_Lookup_Tree * p_Labels)
     {
          //This keeps track of the current position in the table, it is passed to the parser for modification with goto statements.
          int EIP = 0;
          
          //The current command string.
          string CMD_Line = "";
          
          //Loop through each line in the command table evaluating them as it goes.
          for (EIP=0;EIP<p_Command_Table->get_Row_Count();EIP++)
          {
               //Get the current command string.
               p_Command_Table->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(EIP, 0, &CMD_Line);
               
               //Labelize the current command text string.
               labelize_Command_Text(CMD_Line, p_Labels, &EIP);
          }
          
          //Debug
          p_Labels->output_Tree();
          
          return "LABELIZER_COMPLETED_TASK";
     }
     
     //If the submitted string is a label then submit it to the p_Labels tree.
     string labelize_Command_Text(string p_Command_Text, c_Lookup_Tree * p_Labels, int * p_EIP)
     {
          if (!p_Command_Text.size()){ return "ERR_NO_TEXT_SUBMITTED->LABELIZE_COMMAND_STRING->p_Command_Text"; }
          
          //Labels are identified by the character ':'.
          if (p_Command_Text[0] == ':')
          {
               p_Labels->set_int(p_Command_Text, *p_EIP);
               return ("LABEL_FOUND->" + p_Command_Text);
          }
          return "NO_LABEL_FOUND";
     }
     
     //Evaluates a given table.
     //This is done by feeding the table row by row into the eval_Command_String().
     //A for loop controls the reading of the table, however, different commands will be able to modify the counter.
     //By modifying the counter we can have jumps and loops within the table.
     //To avoid variable conflicts this will have its own Binary_Tree, however, it will be used to store labels and their positions.
     //Command tables will be one dimensional.
     string eval_Command_Table(c_Table_1D * p_Command_Table)
     {
          //This keeps track of the current position in the table, it is passed to the parser for modification with goto statements.
          int EIP = 0;
          
          //The current command string.
          string CMD_Line = "";
          
          //The labels for the current table.
          c_Lookup_Tree Labels;
          
          //Find the labels in the table and store them in the lookup tree for the goto commands to use.
          labelize_Command_Table(p_Command_Table, &Labels);
          
          //Loop through each line in the command table evaluating them as it goes.
          for (EIP=0;EIP<p_Command_Table->get_Row_Count();EIP++)
          {
               //Get the current command string.
               p_Command_Table->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(EIP, 0, &CMD_Line);
               eval_Command_String(CMD_Line, &EIP, &Labels);
          }
          
          return "COMMAND_PARSER->EVAL_COMMAND_TABLE_SUCCESS";
     }
     
     //Overloaded for single string submission.
     string eval_Command_String(string p_Command_Text)
     {
          string tmp_Return = "";
          
          int tmp_EIP = 0;
          c_Lookup_Tree tmp_Labels;
          c_Table_1D * Tokens = Tables->register_New_Table_1D("Token_Table");
          tmp_Return = eval_Command_String(p_Command_Text, &tmp_EIP, &tmp_Labels, Tokens);
          Tables->delete_Table(Tokens->get_Table_ID());
          
          return tmp_Return;
     }
     
     //Overloaded for non-recursive submission.
     string eval_Command_String(string p_Command_Text, int * p_EIP, c_Lookup_Tree * p_Labels)
     {
          string tmp_Return = "";
          
          c_Table_1D * Tokens = Tables->register_New_Table_1D("Token_Table");
          tmp_Return = eval_Command_String(p_Command_Text, p_EIP, p_Labels, Tokens);
          Tables->delete_Table(Tokens->get_Table_ID());
          
          return tmp_Return;
     }
     
     
     
     //Evaluates a submitted string.
     //It does this by breaking the string down into componets.
     //Each componet is stored in a separate string, in an array of strings.
     //Spaces act as deliminators between command componets, the semicolon acts as an eol character.
     string eval_Command_String(string p_Command_Text, int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          //Break the command text down into tokens.
          tokenize_Command_Text(p_Command_Text, p_Tokens);
          
          //cout << "\n\n\t Tokens:";
          //p_Tokens->output_Table();
          
          //Replace the variables with their values.
          convert_Variables_To_Values(p_Tokens);
          
          //Evaluates the tokens.
          return eval_Tokens(p_EIP, p_Labels, p_Tokens);
          
          
          //cout << "\n\n\t Var_Lookup_Tree:";
          //Var_Lookup_Tree.output_Tree();
     }
     
     //Breaks the command string down into tokens.
     void tokenize_Command_Text(string p_Command_Text, c_Table_1D * p_Tokens)
     {
          //Prep for new submission.
          p_Tokens->reset_Table();
          flg_Var_Pos = 0;
          flg_String = 0;
          
          //Loop through the command text finding variable identifiers and extracting them.
          for (unsigned int cou_Index=0;cou_Index<p_Command_Text.size();cou_Index++)
          {
               //cout << "\n\t p_Command_Text[" << cou_Index << "]->" << p_Command_Text[cou_Index];
               
               
               //This allows for special characters to be read in, useful for setting table entries where a variable is needed.
               if (p_Command_Text[cou_Index] == '^' && (cou_Index + 1) < p_Command_Text.size())
               {
                    p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_C(p_Command_Text[cou_Index + 1]);
                    cou_Index++;
                    continue;
               }
               
               //Check for strings.
               if (p_Command_Text[cou_Index] == '"')
               {
                    
                    //If a string is being read in then stop the reading.
                    if (flg_String)
                    { 
                         flg_String = 0; 
                         
                         //End the current string.
                         p_Tokens->pop_Cell_In_Current_Row();
                         p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_I(0);
                         p_Tokens->pop_Row();
                    } 
                    else 
                    {
                         flg_String = 1;
                         //If the current token does not contain whitespace then increment the current token.
                         if (p_Tokens->get_Set_State_For_Given_Cell_In_Current_Row(0))
                         { 
                              p_Tokens->pop_Cell_In_Current_Row();
                              p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_I(0);
                              p_Tokens->pop_Row();
                         }
                    }
                    
                    //Do not read in the '"' character.
                    continue;
               }
               
               //If there is currently a string being read in then add to the current token irregardless of special characters, whitespace, or variables.
               if (flg_String)
               {
                    p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_C(p_Command_Text[cou_Index]);
                    continue;
               }
               
               
               
               //If a space is encountered then start on the next token.
               if (p_Command_Text[cou_Index] == ' ')
               {
                    //If the current token does not contain whitespace then increment the current token.
                    if (p_Tokens->get_Set_State_For_Given_Cell_In_Current_Row(0))
                    { 
                         p_Tokens->pop_Cell_In_Current_Row();
                         p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_I(0);
                         p_Tokens->pop_Row();
                    }
                    continue; 
               }
               
               //If a '%' is encountered then check the flg_Var_Pos to determine if a new token is needed, or if the current token is ending.
               if (p_Command_Text[cou_Index] == '%' && flg_Var_Pos == 0)
               { 
                    //cout << "\n\t Found a variable...";
                    
                    //If the current token does not contain whitespace then increment the current token.
                    if (p_Tokens->get_Set_State_For_Given_Cell_In_Current_Row(0))
                    {
                         p_Tokens->pop_Cell_In_Current_Row();
                         p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_I(0);
                         p_Tokens->pop_Row();
                    }
                    flg_Var_Pos = 1; 
                    continue; 
               }
               
               if (p_Command_Text[cou_Index] == '%' && flg_Var_Pos == 1)
               {
                    //cout << "\n\t\t End of variable string, setting flag on cell...";
                    p_Tokens->pop_Cell_In_Current_Row(); p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_I(1);
                    if (cou_Index < (p_Command_Text.size() - 1)){ p_Tokens->pop_Row(); }
                    flg_Var_Pos = 0; 
                    continue; 
               }
               
               //Add the current character to the current token.
               p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_C(p_Command_Text[cou_Index]);
               //cout << "\n\t\t p_Command_Text[" << cou_Index << "]->" << int (p_Command_Text[cou_Index]);
               
               if (cou_Index == (p_Command_Text.size() - 1) && p_Tokens->get_Set_State_For_Given_Cell_In_Current_Row(0)){ p_Tokens->pop_Cell_In_Current_Row(); p_Tokens->push_Data_Bit_Into_Current_Cell_On_Current_Row_I(0); }
               
               
          }
          
          //Debug information.
          //p_Tokens->output_Table();
     }
     
     //Replaces any token that represents a variable with its value.
     void convert_Variables_To_Values(c_Table_1D * p_Tokens)
     {
          //Reset the undefined flag.
          flg_Undefined = 0;
          
          //Temp for current var type.
          int tmp_Var_Type = 0;
          
          //Temps string for the variable name.
          string tmp_Var_Name = "";
          
          //Stores the temporary data. 
          string tmp_Var_Data = "";
          
          //Loop through the tokens querying the lookup tree for the values. If no variable yet exists with that name then it is added to the table and a null value returned. The undefined error is thrown.
          for (int cou_Index=0;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
          {
               //cout << "\n\t p_Tokens->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(cou_Index, 1, 0)->" << p_Tokens->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(cou_Index, 1, 0); 
               
               //cout << "\n\n\n\tconvert_Variables_To_Values();";
               //p_Tokens->output_Table();
               
               if (p_Tokens->get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(cou_Index, 1, 0) == 1)
               {
                    //cout << "\n\n\t Variable Detected, Token Number " << cou_Index << "...";
                    //Get the variable name.
                    p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Var_Name);
                    
                    //cout << "\n\t Variable Name->" << tmp_Var_Name;
                    
                    //Google it.
                    tmp_Var_Type = Var_Lookup_Tree.get_Var_Type(tmp_Var_Name);
                    
                    //cout << "\n\t Variable Type->" << tmp_Var_Type;
                    
                    
                    //Replace the variable with the data found.
                    if (tmp_Var_Type == 0){ tmp_Var_Data = Var_Lookup_Tree.get_string(tmp_Var_Name); }
                    if (tmp_Var_Type == 1){ tmp_Var_Data = int2str(Var_Lookup_Tree.get_int(tmp_Var_Name)); }
                    if (tmp_Var_Type == 2){ tmp_Var_Data = float2str(Var_Lookup_Tree.get_float(tmp_Var_Name)); }
                    //if (tmp_Var_Type == 3){ p_Tokens->set_Data_Bit_For_Given_Cell_In_Given_Row_NR(cou_Index, 0, Var_Lookup_Tree.get_reference(tmp_Var_Name)); }
                    if (tmp_Var_Type == 4){ tmp_Var_Data = bool2str(Var_Lookup_Tree.get_bool(tmp_Var_Name)); }
                    
                    //cout << "\n\t Variable Data->" << tmp_Var_Data; 
                    
                    //cout << "\n\t Setting Cell[0] in Row[" << cou_Index << "] to Data->" << tmp_Var_Data;
                    p_Tokens->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(cou_Index, 0, tmp_Var_Data);
               }
          }
          
     }
     
     //Checks a token to determine if it is a command or callback. If so it is evaluated and the value returned.
     string test_Command_Token(string p_Token, int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          string tmp_Token = "";
          
          //If the first character is a tilde then extract the rest of the string and evaluate_Command_Text(), returning the returned value, otherwise return the token.
          if (p_Token[0] == '~')
          {
               //Gather the token.
               for (unsigned int cou_Index=1;cou_Index<p_Token.size();cou_Index++)
               {
                    tmp_Token += p_Token[cou_Index];
               }
               
               //Return the result of the eval.
               return eval_Command_String(tmp_Token, p_EIP, p_Labels);
          }
          
          //If it is not a command then return the token.
          return p_Token;
     }
     
     //Evaluates the tokens in the token table.
     string eval_Tokens(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          //The temporary string for the token being evaluated.
          string tmp_Token[25];
          
          //Debug
          //cout << "\n\n\n\t eval_Tokens()";
          //p_Tokens->output_Table();
          
          //Get the current token in string format.
          p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(0, 0, &tmp_Token[0]);
          
          
          //These are the basic commands for the scripting language.
          if (tmp_Token[0] == "echo")           { return CMD_echo          (p_EIP, p_Labels, p_Tokens); }
          if (tmp_Token[0] == "table")          { return CMD_table         (p_EIP, p_Labels, p_Tokens); }
          if (tmp_Token[0] == "if")             { return CMD_if            (p_EIP, p_Labels, p_Tokens); } //Passing EIP as this calls the eval_Command_String.
          if (tmp_Token[0] == "output")         { return CMD_output        (p_EIP, p_Labels, p_Tokens); }
               
          //Data manipulation commands
          if (tmp_Token[0] == "cin")            { return CMD_cin           (p_EIP, p_Labels, p_Tokens); }
          if (tmp_Token[0] == "set")            { return CMD_set           (p_EIP, p_Labels, p_Tokens); }
          if (tmp_Token[0] == "gdt")            { return CMD_get_datatype  (p_EIP, p_Labels, p_Tokens); }
          if (tmp_Token[0] == "com")            { return CMD_operation     (p_EIP, p_Labels, p_Tokens); }
               
          //Commands related to the Command tables.
          if (tmp_Token[0] == "eval_CMD_table") { return CMD_eval          (p_EIP, p_Labels, p_Tokens); } //The EIP and Labels are only for the test_Command_Token.
          if (tmp_Token[0] == "beval_CMD_table"){ return CMD_beval         (p_EIP, p_Labels, p_Tokens); } //The EIP and Labels are only for the test_Command_Token.
          if (tmp_Token[0] == "goto")           { return CMD_goto          (p_EIP, p_Labels, p_Tokens); }
               
          //Commands related to saving and loading command tables.
          //if (tmp_Token[0] == "save_CMD_table") { return CMD_save  (p_EIP, p_Labels, p_Tokens); }
          if (tmp_Token[0] == "load_CMD_script"){ return CMD_load_script   (p_EIP, p_Labels, p_Tokens); }
               
          //These are the callbacks for the engine.
          if (tmp_Token[0] == "gathering")      { return NT_gathering      (p_EIP, p_Labels, p_Tokens); }
          if (tmp_Token[0] == "assembly")       { return NT_assembly       (p_EIP, p_Labels, p_Tokens); }
          if (tmp_Token[0] == "construct")      { return NT_construct      (p_EIP, p_Labels, p_Tokens); }
               
          //Allows for CMD_tables to be evaluated as commands.
          if (CMD_Script_Registry_Lookup_Tree.exists(tmp_Token[0])){ p_Tokens->shift_All_Rows(0); return CMD_eval(p_EIP, p_Labels, p_Tokens); }
          
          
          return "0";
     }
     
     
     //--     CMD FUNCTIONS
     
     string CMD_echo(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          string tmp_echo_Token;
          
          //p_Tokens->output_Table();
          
          cout << "\n\t\t ->";
          for (int cou_Index=1;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
          {
               p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_echo_Token);
               
               //Checks for command tokens.
               tmp_echo_Token = test_Command_Token(tmp_echo_Token, p_EIP, p_Labels, p_Tokens);
               
               cout << tmp_echo_Token << " ";
          }
          return "1";
     }
     
     
     //cin Var_Name / = 
     // 0     1     2 3
     string CMD_cin(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          string tmp_Token[10];
          
          p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(1, 0, &tmp_Token[1]);
          tmp_Token[1] = test_Command_Token(tmp_Token[1], p_EIP, p_Labels, p_Tokens);
          
          p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(2, 0, &tmp_Token[2]);
          tmp_Token[2] = test_Command_Token(tmp_Token[2], p_EIP, p_Labels, p_Tokens);
          
          p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(3, 0, &tmp_Token[3]);
          tmp_Token[3] = test_Command_Token(tmp_Token[3], p_EIP, p_Labels, p_Tokens);
          
          p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(4, 0, &tmp_Token[4]);
          tmp_Token[4] = test_Command_Token(tmp_Token[4], p_EIP, p_Labels, p_Tokens);
          
          cout << "\n\n\t" << tmp_Token[4];
          string tmp_Input = "";
          getline(cin, tmp_Input);
          
          //cout << "\n\n\t tmp_Token[2]->" << tmp_Token[2] << "  tmp_Token[3]->" << tmp_Token[3] << "  tmp_Token[4]->" << tmp_Token[4];
          
          //Simple assignment.
          if (tmp_Token[1] == "/s"){ if (tmp_Token[3] == "="){ Var_Lookup_Tree.set_string(tmp_Token[2], tmp_Input); } }
          if (tmp_Token[1] == "/i"){ if (tmp_Token[3] == "="){ Var_Lookup_Tree.set_int(tmp_Token[2], str2int(tmp_Input)); } }
          if (tmp_Token[1] == "/f"){ if (tmp_Token[3] == "="){ Var_Lookup_Tree.set_float(tmp_Token[2], str2float(tmp_Input)); } }
          if (tmp_Token[1] == "/b"){ if (tmp_Token[3] == "="){ Var_Lookup_Tree.set_bool(tmp_Token[2], str2bool(tmp_Input)); } }
          
          //Addition
          if (tmp_Token[1] == "/s"){ if (tmp_Token[3] == "+="){ Var_Lookup_Tree.set_string(tmp_Token[2], (Var_Lookup_Tree.get_string(tmp_Token[2]) + tmp_Input)); } }
          if (tmp_Token[1] == "/i"){ if (tmp_Token[3] == "+="){ Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) + str2int(tmp_Input))); } }
          if (tmp_Token[1] == "/f"){ if (tmp_Token[3] == "+="){ Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) + str2float(tmp_Input))); } }
          
          //Subtraction
          if (tmp_Token[1] == "/i"){ if (tmp_Token[3] == "-="){ Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) - str2int(tmp_Input))); } }
          if (tmp_Token[1] == "/f"){ if (tmp_Token[3] == "-="){ Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) - str2float(tmp_Input))); } }
          
          //Multiplication
          if (tmp_Token[1] == "/i"){ if (tmp_Token[3] == "*="){ Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) * str2int(tmp_Input))); } }
          if (tmp_Token[1] == "/f"){ if (tmp_Token[3] == "*="){ Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) * str2float(tmp_Input))); } }
          
          //Division
          if (tmp_Token[1] == "/i"){ if (tmp_Token[3] == "/="){ Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) / str2int(tmp_Input))); } }
          if (tmp_Token[1] == "/f"){ if (tmp_Token[3] == "/="){ Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) / str2float(tmp_Input))); } }
          
          //References will be treated as integers.
          //if (tmp_Token[1] == "/r"){ Var_Lookup_Tree.set_reference(tmp_Token[1], tmp_Token[3]);
          //cout << "\n\n\n\t ";
          //Var_Lookup_Tree.output_Current_Node();
          return "1";
     }
     
     
     string CMD_set(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          
          string tmp_Token[100];
          
          //Gather all the tokens up to the limit of 10.
          for (int cou_Index=0;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
          {
               if (cou_Index >= 100){ break; }
               p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
               tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
               //cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
          }
          
          //cout << "\n\n\t tmp_Token[2]->" << tmp_Token[2] << "  tmp_Token[3]->" << tmp_Token[3] << "  tmp_Token[4]->" << tmp_Token[4];
          
          //Simple assignment.
          if (tmp_Token[1] == "/s"){ if (tmp_Token[3] == "="){ Var_Lookup_Tree.set_string(tmp_Token[2], tmp_Token[4]); } }
          for (int cou_Index=5;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
          {
               if (tmp_Token[1] == "/s"){ if (tmp_Token[3] == "="){ Var_Lookup_Tree.set_string(tmp_Token[2], (Var_Lookup_Tree.get_string(tmp_Token[2]) + tmp_Token[cou_Index])); } }
          }
          if (tmp_Token[1] == "/i"){ if (tmp_Token[3] == "="){ Var_Lookup_Tree.set_int(tmp_Token[2], str2int(tmp_Token[4])); } }
          if (tmp_Token[1] == "/f"){ if (tmp_Token[3] == "="){ Var_Lookup_Tree.set_float(tmp_Token[2], str2float(tmp_Token[4])); } }
          if (tmp_Token[1] == "/b"){ if (tmp_Token[3] == "="){ Var_Lookup_Tree.set_bool(tmp_Token[2], str2bool(tmp_Token[4])); } }
          
          //Addition
          if (tmp_Token[1] == "/s"){ if (tmp_Token[3] == "+="){ Var_Lookup_Tree.set_string(tmp_Token[2], (Var_Lookup_Tree.get_string(tmp_Token[2]) + tmp_Token[4])); } }
          if (tmp_Token[1] == "/i"){ if (tmp_Token[3] == "+="){ Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) + str2int(tmp_Token[4]))); } }
          if (tmp_Token[1] == "/f"){ if (tmp_Token[3] == "+="){ Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) + str2float(tmp_Token[4]))); } }
          
          //Subtraction
          if (tmp_Token[1] == "/i"){ if (tmp_Token[3] == "-="){ Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) - str2int(tmp_Token[4]))); } }
          if (tmp_Token[1] == "/f"){ if (tmp_Token[3] == "-="){ Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) - str2float(tmp_Token[4]))); } }
          
          //Multiplication
          if (tmp_Token[1] == "/i"){ if (tmp_Token[3] == "*="){ Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) * str2int(tmp_Token[4]))); } }
          if (tmp_Token[1] == "/f"){ if (tmp_Token[3] == "*="){ Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) * str2float(tmp_Token[4]))); } }
          
          //Division
          if (tmp_Token[1] == "/i"){ if (tmp_Token[3] == "/="){ Var_Lookup_Tree.set_int(tmp_Token[2], (Var_Lookup_Tree.get_int(tmp_Token[2]) / str2int(tmp_Token[4]))); } }
          if (tmp_Token[1] == "/f"){ if (tmp_Token[3] == "/="){ Var_Lookup_Tree.set_float(tmp_Token[2], (Var_Lookup_Tree.get_float(tmp_Token[2]) / str2float(tmp_Token[4]))); } }
          
          //References will be treated as integers.
          //if (tmp_Token[1] == "/r"){ Var_Lookup_Tree.set_reference(tmp_Token[1], tmp_Token[3]);
          //cout << "\n\n\n\t ";
          //Var_Lookup_Tree.output_Current_Node();
          return "1";
     }
     
     //The good ol 'if' command. Essential to almost all algorithms.
     string CMD_if(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          
          //The result of the comparision being performed.
          bool flg_Result = false;
          
          //The temporary command string to submit for eval.
          string tmp_Command_Text = "";
          
          string tmp_Token[100];
          
          //Gather all the tokens up to the limit of 10.
          for (int cou_Index=0;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
          {
               if (cou_Index >= 100){ break; }
               p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
               tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
               //cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
          }
          
          //if TYPE ( F1 Op F2 ) EVAL
          // 0    1 2  3  4  5 6    7
          //Get the result of the comparison based on the datatype of the first factor.
          if (tmp_Token[1] == "s")
          {
               if (tmp_Token[4] == "=="){ flg_Result = (tmp_Token[3] == tmp_Token[5]); }
               if (tmp_Token[4] == ">="){ flg_Result = (tmp_Token[3] >= tmp_Token[5]); }
               if (tmp_Token[4] == "<="){ flg_Result = (tmp_Token[3] <= tmp_Token[5]); }
               if (tmp_Token[4] == ">"){ flg_Result = (tmp_Token[3] > tmp_Token[5]); }
               if (tmp_Token[4] == "<"){ flg_Result = (tmp_Token[3] < tmp_Token[5]); }
          }
          if (tmp_Token[1] == "i")
          {
               int tmp_int[2];
               tmp_int[0] = str2int(tmp_Token[3]);
               tmp_int[1] = str2int(tmp_Token[5]);
               if (tmp_Token[4] == "=="){ flg_Result = (tmp_int[0] == tmp_int[1]); }
               if (tmp_Token[4] == ">="){ flg_Result = (tmp_int[0] >= tmp_int[1]); }
               if (tmp_Token[4] == "<="){ flg_Result = (tmp_int[0] <= tmp_int[1]); }
               if (tmp_Token[4] == ">"){ flg_Result = (tmp_int[0] > tmp_int[1]); }
               if (tmp_Token[4] == "<"){ flg_Result = (tmp_int[0] < tmp_int[1]); }
          }
          if (tmp_Token[1] == "f")
          {
               int tmp_float[2];
               tmp_float[0] = str2float(tmp_Token[3]);
               tmp_float[1] = str2float(tmp_Token[5]);
               if (tmp_Token[4] == "=="){ flg_Result = (tmp_float[0] == tmp_float[1]); }
               if (tmp_Token[4] == ">="){ flg_Result = (tmp_float[0] >= tmp_float[1]); }
               if (tmp_Token[4] == "<="){ flg_Result = (tmp_float[0] <= tmp_float[1]); }
               if (tmp_Token[4] == ">"){ flg_Result = (tmp_float[0] > tmp_float[1]); }
               if (tmp_Token[4] == "<"){ flg_Result = (tmp_float[0] < tmp_float[1]); }
          }
          if (tmp_Token[1] == "b")
          {
               int tmp_bool[2];
               tmp_bool[0] = str2bool(tmp_Token[3]);
               tmp_bool[1] = str2bool(tmp_Token[5]);
               if (tmp_Token[4] == "=="){ flg_Result = (tmp_bool[0] == tmp_bool[1]); }
               if (tmp_Token[4] == ">="){ flg_Result = (tmp_bool[0] >= tmp_bool[1]); }
               if (tmp_Token[4] == "<="){ flg_Result = (tmp_bool[0] <= tmp_bool[1]); }
               if (tmp_Token[4] == ">"){ flg_Result = (tmp_bool[0] > tmp_bool[1]); }
               if (tmp_Token[4] == "<"){ flg_Result = (tmp_bool[0] < tmp_bool[1]); }
          }
          
          cout << "\n\n\t\t (" << tmp_Token[3] << " " << tmp_Token[4] << " " << tmp_Token[5] << ") " << flg_Result;
          
          //If true then format the remaining tokens into a string and evaluate them.
          if (flg_Result)
          {
               for (int cou_Index=7;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
               {
                    //Gather the tokens and concatinate them onto the temporary command text.
                    p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S((cou_Index), 0, &tmp_Token[0]);
                    tmp_Command_Text += tmp_Token[0] + " ";
               }
               
               //Evaluate the gathered command text gathered.
               eval_Command_String(tmp_Command_Text, p_EIP, p_Labels);
          }
          
          //Return the result of the comparision.
          return bool2str(flg_Result);
     }
     
     
     string CMD_table(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          string tmp_Token[100];
          
          //Gather all the tokens up to the limit of 10.
          for (int cou_Index=0;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
          {
               if (cou_Index >= 100){ break; }
               p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
               tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
               //cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
          }
          
          //Do the right thing.
          
          //Sets a given row and index in a given table to the given data.
          //if (tmp_Token[1] == "set_int"){ Tables->set_int(str2int());
          
          //==--     BY NAME
     
          //Sets a given rows cell to the given data.
          //set_X( Table_ID, Row, Cell, Data)
          //Simple assignment.
          string tmp_String = "";
          if (tmp_Token[1] == "set_string")
          {
               tmp_String = tmp_Token[5];
               
               for (int cou_Index=5;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
               {
                    tmp_String += tmp_Token[cou_Index];
               }
          }
          
          if (tmp_Token[1] == "push_string")
          {
               tmp_String = tmp_Token[4];
               
               for (int cou_Index=5;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
               {
                    tmp_String += tmp_Token[cou_Index];
               }
          }
          
          if (tmp_Token[1] == "push_pop_string" || tmp_Token[1] == "pop_push_string")
          {
               tmp_String = tmp_Token[3];
               
               for (int cou_Index=4;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
               {
                    tmp_String += tmp_Token[cou_Index];
               }
          }
          
          if (tmp_Token[1] == "set_string"){ Tables->set_string( tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), tmp_String); return "1"; }
          if (tmp_Token[1] == "set_int")   { Tables->set_int(    tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2int(tmp_Token[5])); return "1"; }
          if (tmp_Token[1] == "set_float") { Tables->set_float(  tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2float(tmp_Token[5])); return "1"; }
          if (tmp_Token[1] == "set_bool")  { Tables->set_bool(   tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2bool(tmp_Token[5])); return "1"; }
          
          if (tmp_Token[1] == "push_string"){ Tables->push_string( tmp_Token[2], str2int(tmp_Token[3]), tmp_String); return "1"; }
          if (tmp_Token[1] == "push_int")   { Tables->push_int(    tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4])); return "1"; }
          if (tmp_Token[1] == "push_float") { Tables->push_float(  tmp_Token[2], str2int(tmp_Token[3]), str2float(tmp_Token[4])); return "1"; }
          if (tmp_Token[1] == "push_bool")  { Tables->push_bool(   tmp_Token[2], str2int(tmp_Token[3]), str2bool(tmp_Token[4])); return "1"; }
          
          if (tmp_Token[1] == "pop_push_string"){ Tables->pop_push_string( tmp_Token[2], tmp_String); return "1"; }
          if (tmp_Token[1] == "pop_push_int")   { Tables->pop_push_int(    tmp_Token[2], str2int(tmp_Token[3])); return "1"; }
          if (tmp_Token[1] == "pop_push_float") { Tables->pop_push_float(  tmp_Token[2], str2float(tmp_Token[3])); return "1"; }
          if (tmp_Token[1] == "pop_push_bool")  { Tables->pop_push_bool(   tmp_Token[2], str2bool(tmp_Token[3])); return "1"; }
          
          if (tmp_Token[1] == "push_pop_string"){ Tables->push_pop_string( tmp_Token[2], tmp_String); return "1"; }
          if (tmp_Token[1] == "push_pop_int")   { Tables->push_pop_int(    tmp_Token[2], str2int(tmp_Token[3])); return "1"; }
          if (tmp_Token[1] == "push_pop_float") { Tables->push_pop_float(  tmp_Token[2], str2float(tmp_Token[3])); return "1"; }
          if (tmp_Token[1] == "push_pop_bool")  { Tables->push_pop_bool(   tmp_Token[2], str2bool(tmp_Token[3])); return "1"; }
          
          
          //Gets a given cells data in a given row.
          //get_X( Table_ID, Row, Cell)
          if (tmp_Token[1] == "get_string"){ return          (Tables->get_string( tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]))); }
          if (tmp_Token[1] == "get_int")   { return int2str  (Tables->get_int(    tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2int(tmp_Token[5]))); }
          if (tmp_Token[1] == "get_float") { return float2str(Tables->get_float(  tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2int(tmp_Token[5]))); }
          if (tmp_Token[1] == "get_bool")  { return bool2str (Tables->get_bool(   tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2int(tmp_Token[5]))); }
          
          //==--     BY ID
          
          //Sets a given rows cell to the given data.
          //set_X( Table_ID, Row, Cell, Data)
          if (tmp_Token[1] == "bset_string"){ Tables->set_string( str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]), tmp_Token[5]); return "1"; }
          if (tmp_Token[1] == "bset_int")   { Tables->set_int(    str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2int(tmp_Token[5])); return "1"; }
          if (tmp_Token[1] == "bset_float") { Tables->set_float(  str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2float(tmp_Token[5])); return "1"; }
          if (tmp_Token[1] == "bset_bool")  { Tables->set_bool(   str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2bool(tmp_Token[5])); return "1"; }
          
          
          //Gets a given cells data in a given row.
          //get_X( Table_ID, Row, Cell)
          if (tmp_Token[1] == "bget_string"){ return          (Tables->get_string( str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]))); }
          if (tmp_Token[1] == "bget_int")   { return int2str  (Tables->get_int(    str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]))); }
          if (tmp_Token[1] == "bget_float") { return float2str(Tables->get_float(  str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]))); }
          if (tmp_Token[1] == "bget_bool")  { return bool2str (Tables->get_bool(   str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]))); }
          
          //Copies the data from the submitted cell into itself.
          //copy(int p_Table, int p_Row, int p_Cell, c_Cell_1D * p_Cell_To_Copy)
          //table-0 copy-1 from_Tbl-2 from_Row-3 from_Cell-4 To_Tbl-5 To_Row-6 To_Cell-7
          if (tmp_Token[1] == "copy_cell")
          { 
               if (p_Tokens->get_Row_Count() < 7){ cout << "\n\n\n\t SYN_ERR_TABLE_COPY: #tokens = " << p_Tokens->get_Row_Count(); return "SYN_ERR_TABLE_COPY"; }
               
               //Holds the cell to help with reading the code.
               void * tmp_Cell = Tables->get_Table_Cell_Reference(tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]));
               
               Tables->copy(tmp_Token[5], str2int(tmp_Token[6]), str2int(tmp_Token[7]), tmp_Cell); 
               return "TABLE_CELL_COPY";
          }
          if (tmp_Token[1] == "bcopy_cell")
          { 
               if (p_Tokens->get_Row_Count() < 7){ cout << "\n\n\n\t SYN_ERR_TABLE_COPY: #tokens = " << p_Tokens->get_Row_Count(); return "SYN_ERR_TABLE_COPY"; }
               
               //Holds the cell to help with reading the code.
               void * tmp_Cell = Tables->get_Table_Cell_Reference(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]));
               
               Tables->copy(str2int(tmp_Token[5]), str2int(tmp_Token[6]), str2int(tmp_Token[7]), tmp_Cell); 
               return "TABLE_CELL_COPY";
          }
          
          //table copy_row FROM_TABLE FROM_ROW TO_TABLE TO_ROW
          //  0       1        2          3      4        5
          if (tmp_Token[1] == "copy_row")
          {
               if (p_Tokens->get_Row_Count() < 5){ cout << "\n\n\n\t SYN_ERR_TABLE_COPY: #tokens = " << p_Tokens->get_Row_Count(); return "SYN_ERR_TABLE_COPY"; }
               
               //Holds the row to help with reading the code.
               void * tmp_Row = Tables->get_Table_Row_Reference(tmp_Token[2], str2int(tmp_Token[3]));
               
               Tables->copy_Row(tmp_Token[4], str2int(tmp_Token[5]), tmp_Row); 
               return "TABLE_ROW_COPY";
          }
          if (tmp_Token[1] == "bcopy_row")
          {
               if (p_Tokens->get_Row_Count() < 5){ cout << "\n\n\n\t SYN_ERR_TABLE_COPY: #tokens = " << p_Tokens->get_Row_Count(); return "SYN_ERR_TABLE_COPY"; }
               
               //Holds the row to help with reading the code.
               void * tmp_Row = Tables->get_Table_Row_Reference(str2int(tmp_Token[2]), str2int(tmp_Token[3]));
               
               Tables->copy_Row(str2int(tmp_Token[4]), str2int(tmp_Token[5]), tmp_Row); 
               return "TABLE_ROW_COPY";
          }
          
          //Copies one table into another.
          //table copy FROM_TABLE TO_TABLE
          //    0    1          2        3
          if (tmp_Token[1] == "copy_table")
          {
               Tables->copy_Table(tmp_Token[3], tmp_Token[2]); 
               return "TABLE_ROW_COPY";
          }
          if (tmp_Token[1] == "bcopy_table")
          {
               Tables->copy_Table(str2int(tmp_Token[3]), str2int(tmp_Token[2])); 
               return "TABLE_ROW_COPY";
          }
          
          
          
          //Copies a set of rows from one table to another with an offset.
          //table copy_rows FROM_TABLE FROM_ROW_START FROM_ROW_END TO_TABLE TO_OFFSET
          //table copy_rows     2            5              10       8        3 //example
          //would copy rows 5-10 from table 2 into table 8 starting at index 3 and ending at index 8.
          //
          //table copy_rows FROM_TABLE FROM_ROW_START FROM_ROW_END TO_TABLE TO_OFFSET
          //  0       1         2          3              4          5        6 //Tokens
          if (tmp_Token[1] == "copy_row_set")
          {
               //Holds the current row being copied.
               void * tmp_Current_Row = NULL;
               
               //From table.
               string tmp_From_Table = tmp_Token[2];
               
               //Starting index to copy from.
               int tmp_Starting_Copy_Index = str2int(tmp_Token[3]);
               
               //Ending index to copy to.
               int tmp_Ending_Copy_Index = str2int(tmp_Token[4]);
               
               //To Table
               string tmp_To_Table = tmp_Token[5];
               
               //Offset.
               int tmp_To_Offset = str2int(tmp_Token[6]);
               
               for (int cou_Row=0;cou_Row<=(tmp_Ending_Copy_Index - tmp_Starting_Copy_Index);cou_Row++)
               {
                    //Get the current row.
                    tmp_Current_Row = Tables->get_Table_Row_Reference(tmp_From_Table, (cou_Row + tmp_Starting_Copy_Index));
                    
                    //Copy it to the appropiate table with the appropriate offset.
                    Tables->copy_Row(tmp_To_Table, (cou_Row + tmp_To_Offset), tmp_Current_Row);
                    
               }
               return "TABLE_ROW_SET_COPY";
          }
          if (tmp_Token[1] == "bcopy_row_set")
          {
               //Holds the current row being copied.
               void * tmp_Current_Row = NULL;
               
               //From table.
               int tmp_From_Table = str2int(tmp_Token[2]);
               
               //Starting index to copy from.
               int tmp_Starting_Copy_Index = str2int(tmp_Token[3]);
               
               //Ending index to copy to.
               int tmp_Ending_Copy_Index = str2int(tmp_Token[4]);
               
               //To Table
               int tmp_To_Table = str2int(tmp_Token[5]);
               
               //Offset.
               int tmp_To_Offset = str2int(tmp_Token[6]);
               
               for (int cou_Row=0;cou_Row<=(tmp_Ending_Copy_Index - tmp_Starting_Copy_Index);cou_Row++)
               {
                    //Get the current row.
                    tmp_Current_Row = Tables->get_Table_Row_Reference(tmp_From_Table, (cou_Row + tmp_Starting_Copy_Index));
                    
                    //Copy it to the appropiate table with the appropriate offset.
                    Tables->copy_Row(tmp_To_Table, (cou_Row + tmp_To_Offset), tmp_Current_Row);
                    
               }
               return "TABLE_ROW_SET_COPY";
          }
          
          //table tanslate_row_set From_Table Row_To_Translate To_Table Offset_Index
          //  0          1              2            3              4       5
          if (tmp_Token[1] == "translate_row")
          {
               
               //From table.
               string tmp_From_Table = tmp_Token[2];
               
               //Starting index to copy from.
               int tmp_Row_To_Translate = str2int(tmp_Token[3]);
               
               //To Table
               string tmp_To_Table = tmp_Token[4];
               
               //Offset.
               int tmp_To_Offset = str2int(tmp_Token[5]);
               
               //The current cell to copy from.
               void * tmp_Cell = NULL;
               
               
               for (int cou_Index=0;cou_Index<Tables->get_Row_Cell_Count(tmp_From_Table, tmp_Row_To_Translate);cou_Index++)
               {
                    //Holds the current cell in the row.
                    tmp_Cell = Tables->get_Table_Cell_Reference(tmp_From_Table, tmp_Row_To_Translate, cou_Index);
                    //cout << "\n\n\t Current_Cell->";
                    
                    Tables->copy(tmp_To_Table, (cou_Index + tmp_To_Offset), 0, tmp_Cell); 
               }
               return "TABLE_TRANSLATE_ROW";
          }
          if (tmp_Token[1] == "btranslate_row")
          {
               
               //From table.
               int tmp_From_Table = str2int(tmp_Token[2]);
               
               //Starting index to copy from.
               int tmp_Row_To_Translate = str2int(tmp_Token[3]);
               
               //To Table
               int tmp_To_Table = str2int(tmp_Token[4]);
               
               //Offset.
               int tmp_To_Offset = str2int(tmp_Token[5]);
               
               //The current cell to copy from.
               void * tmp_Cell = NULL;
               
               
               for (int cou_Index=0;cou_Index<Tables->get_Row_Cell_Count(tmp_From_Table, tmp_Row_To_Translate);cou_Index++)
               {
                    //Holds the current cell in the row.
                    tmp_Cell = Tables->get_Table_Cell_Reference(tmp_From_Table, tmp_Row_To_Translate, cou_Index);
                    //cout << "\n\n\t Current_Cell->";
                    
                    Tables->copy(tmp_To_Table, (cou_Index + tmp_To_Offset), 0, tmp_Cell); 
               }
               return "TABLE_TRANSLATE_ROW";
          }
          
          //This shifts a column to the right, overwriting anything that was there.
          if (tmp_Token[1] == "shift_column")
          {
               
               //From table.
               string tmp_Table = tmp_Token[2];
               
               //Starting index to copy from.
               int tmp_Column_To_Shift = str2int(tmp_Token[3]);
               
               Tables->shift_Column(tmp_Table, tmp_Column_To_Shift);
               
               return "TABLE_TRANSLATE_ROW";
          }
          if (tmp_Token[1] == "bshift_column")
          {
               
               //From table.
               int tmp_Table = str2int(tmp_Token[2]);
               
               //Starting index to copy from.
               int tmp_Column_To_Shift = str2int(tmp_Token[3]);
               
               //The current cell to copy from.
               void * tmp_Cell = NULL;
               
               
               for (int cou_Index=0;cou_Index<Tables->get_Row_Count(tmp_Table);cou_Index++)
               {
                    //Holds the current cell in the row.
                    tmp_Cell = Tables->get_Table_Cell_Reference(tmp_Table, cou_Index, tmp_Column_To_Shift);
                    
                    //cout << "\n\n\t Current_Cell->";
                    
                    Tables->copy(tmp_Table, cou_Index, (tmp_Column_To_Shift + 1), tmp_Cell); 
                    
                    Tables->set_string(tmp_Table, cou_Index, tmp_Column_To_Shift, " ");
               }
               return "TABLE_TRANSLATE_ROW";
          }
          
          //This copies a row onto the given index of another row, only copies the first bit of the first row given.
          //table overlay_row From_Table From_Row To_Table To_Row To_Index
          //  0        1           2         3        4        5     6
          if (tmp_Token[1] == "overlay_row")
          {
               //Holds the row to help with reading the code.
               void * tmp_Row = Tables->get_Table_Row_Reference(tmp_Token[2], str2int(tmp_Token[3]));
               
               Tables->overlay_Row(tmp_Token[4], str2int(tmp_Token[5]), str2int(tmp_Token[6]), tmp_Row); 
               return "TABLE_ROW_COPY";
          }
          if (tmp_Token[1] == "boverlay_row")
          {
               //Holds the row to help with reading the code.
               void * tmp_Row = Tables->get_Table_Row_Reference(str2int(tmp_Token[2]), str2int(tmp_Token[3]));
               
               Tables->overlay_Row(str2int(tmp_Token[4]), str2int(tmp_Token[5]), str2int(tmp_Token[6]), tmp_Row); 
               return "TABLE_ROW_COPY";
          }
          
          
          //Shifts the rows starting at the given index towards the end of the table making room for a new row.
          //table shift_all_rows [Table_Name] [Index_To_Start_Shift_At]
          if (tmp_Token[1] == "shift_all_rows")
          {
               Tables->shift_All_Rows(tmp_Token[2], str2int(tmp_Token[3])); 
               return "TABLE_SHIFT_ALL_ROWS";
          }
          if (tmp_Token[1] == "bshift_all_rows")
          {
               Tables->shift_All_Rows(str2int(tmp_Token[2]), str2int(tmp_Token[3])); 
               return "TABLE_SHIFT_ALL_ROWS";
          }
          
          //Renames a table.
          //table rename -TABLE_ID -NEW_NAME
          //  0      1         2       3
          if (tmp_Token[1] == "rename")
          {
               Tables->set_Table_Name(tmp_Token[2], tmp_Token[3]);
               return "TABLE_RENAME";
          }
          if (tmp_Token[1] == "brename")
          {
               Tables->set_Table_Name(str2int(tmp_Token[2]), tmp_Token[3]);
               return "TABLE_RENAME";
          }
          
          //creates a new table.
          //table new -TABLE_NAME
          //  0    1        2    
          if (tmp_Token[1] == "new_1D")
          {
               Tables->register_New_Table_1D(tmp_Token[2]);
               return "TABLE_NEW_1D";
          }
                    
          //creates a new CMD table and registers it with the CMD tables.
          //table new -TABLE_NAME
          //  0    1        2    
          if (tmp_Token[1] == "new_CMD")
          {
               Tables->register_New_Table_1D(tmp_Token[2]);
               
               CMD_Script_Registry->push_pop_string(tmp_Token[2]);
               CMD_Script_Registry->push_int((CMD_Script_Registry->get_Row_Count() - 1), str2int(tmp_Token[2]));
               CMD_Script_Registry_Lookup_Tree.set_int(tmp_Token[2], Tables->get_Table_ID(tmp_Token[2]));
               
               return "TABLE_NEW_CMD";
          }
          
          //Deletes a table.
          if (tmp_Token[1] == "delete")
          {
               Tables->delete_Table(tmp_Token[2]);
          }
          if (tmp_Token[1] == "bdelete")
          {
               Tables->delete_Table(str2int(tmp_Token[2]));
          }
          
          //Gets a table ID.
          if (tmp_Token[1] == "get_ID")
          {
               return int2str(Tables->get_Table_ID(tmp_Token[2]));
          }
                    
          //Gets a table Name.
          if (tmp_Token[1] == "get_name")
          {
               return Tables->get_Table_Name(str2int(tmp_Token[2]));
          }
          
          //Resets the given table.
          if (tmp_Token[1] == "reset")
          {
               Tables->reset_Table(tmp_Token[2]);
          }
          if (tmp_Token[1] == "breset")
          {
               Tables->reset_Table(str2int(tmp_Token[2]));
          }
          
          
          return "0";
     }
     
     string CMD_goto(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          
          //if (p_Tokens->get_Row_Count() < 2){ return "ERR_INVALID_TOKEN_COUNT->CMD_goto"; }
          
          string tmp_Label = "";
          
          //Check for callbacks.
          tmp_Label = test_Command_Token(tmp_Label, p_EIP, p_Labels, p_Tokens);
          
          //Gets the name of the label.
          p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(1, 0, &tmp_Label);
          
          //Insert the : character for the tree search.
          tmp_Label = ":" + tmp_Label;
          
          
          //Gets the labels index.
          *p_EIP = p_Labels->get_int(tmp_Label);
          
          
          return "1";
     }
     
     string CMD_eval(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          
          string tmp_Table = "";
          
          p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(1, 0, &tmp_Table);
          
          //Callback checking.
          tmp_Table = test_Command_Token(tmp_Table, p_EIP, p_Labels, p_Tokens);
          
          void * tmp_Table_Ref = Tables->get_Table_Reference(tmp_Table);
          
          if (tmp_Table_Ref == NULL){ return "0"; }
          
          eval_Command_Table((c_Table_1D*) tmp_Table_Ref);
          
          return "1";
     }
     
     string CMD_beval(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          if (p_Tokens->get_Row_Count() < 2){ return "0"; }
          
          string tmp_Table = "";
          
          p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(1, 0, &tmp_Table);
          
          //Callback checking.
          tmp_Table = test_Command_Token(tmp_Table, p_EIP, p_Labels, p_Tokens);
          
          eval_Command_Table((c_Table_1D*) Tables->get_Table_Reference(str2int(tmp_Table)));
          
          return "1";
     }
     
     //Handles all the outputs.
     string CMD_output(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          //The temporary tokens to use.
          string tmp_Token[100];
          
          //Gather all the tokens up to the limit of 10.
          for (int cou_Index=0;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
          {
               if (cou_Index >= 100){ break; }
               p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
               
               //Callback checking.
               tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
               //cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
          }
          
          //Find the appropriate output and do it.
          if (tmp_Token[1] == "table")
          {
               
               if (tmp_Token[2] == "header")
               { 
                    if (tmp_Token[3] == "all"){ Tables->output_All_Table_Headers(); return "1"; }
                    
                    Tables->output_Table_Header(tmp_Token[3]); return "1"; 
               }
               if (tmp_Token[2] == "bheader")
               { 
                    Tables->output_Table_Header(str2int(tmp_Token[3])); return "1"; 
               }
               
               if (tmp_Token[2] == "data")
               { 
                    if (tmp_Token[3] == "all"){ Tables->output_All_Tables(); return "1"; }
                    
                    Tables->output_Table(tmp_Token[3]); return "1"; 
               }
               if (tmp_Token[2] == "bdata")
               { 
                    Tables->output_Table(str2int(tmp_Token[3])); return "1"; 
               }
               
               if (tmp_Token[2] == "verbose")
               { 
                    if (tmp_Token[3] == "all"){ Tables->output_All_Tables_Verbose(); return "1"; }
                    
                    Tables->output_Table_Verbose(tmp_Token[3]); return "1"; 
               }
               if (tmp_Token[2] == "bverbose")
               { 
                    Tables->output_Table_Verbose(str2int(tmp_Token[3])); return "1"; 
               }
               
          }
                    //Find the appropriate output and do it.
          if (tmp_Token[1] == "CMD_tables")
          {
               
               if (tmp_Token[2] == "header")
               { 
                    for (int cou_S=0;cou_S<CMD_Script_Registry->get_Row_Count();cou_S++)
                    {
                         Tables->output_Table_Header(CMD_Script_Registry->get_Table_Name()); 
                    }
               }
               
               if (tmp_Token[2] == "data")
               { 
                    for (int cou_S=0;cou_S<CMD_Script_Registry->get_Row_Count();cou_S++)
                    {
                         Tables->output_Table(CMD_Script_Registry->get_Table_Name()); 
                    }
               }
               
               if (tmp_Token[2] == "verbose")
               { 
                    for (int cou_S=0;cou_S<CMD_Script_Registry->get_Row_Count();cou_S++)
                    {
                         Tables->output_Table_Verbose(CMD_Script_Registry->get_Table_Name()); 
                    }
               }
               
          }
          
          
          
          if (tmp_Token[1] == "Var_Lookup_Tree"){ Var_Lookup_Tree.output_Tree(); }
          if (tmp_Token[1] == "Label_Lookup_Tree"){ p_Labels->output_Tree(); }
          if (tmp_Token[1] == "Table_Registry"){ Tables->output_Table_Registry(); }
          if (tmp_Token[1] == "Table_Registry_Tree"){ Tables->output_Table_Registry(); }
          
          return "1";
     }
     
     
     
     //==--      LOADING FUNCTION FOR COMMAND SCRIPTS
     //Loads a command script.
     //It reads each line of the script into a CMD_Table labeled the same as the file.
     string CMD_load_script(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          string tmp_Token[100];
          
          //Gather all the tokens up to the limit of 10.
          for (int cou_Index=0;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
          {
               if (cou_Index >= 100){ break; }
               p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
               tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
               //cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
          }
          
          //The stream for reading in the file.
          ifstream Script_File;
          
          //The current line.
          string Command_Text = "";
          
          //The temporary filename.
          string tmp_File = tmp_Token[1] + ".ntxs";
          
          Script_File.open(tmp_File.c_str());
          
          //This string is to extract the filename only for the table.
          string tmp_Table_Name = "";
          int tmp_Table_Name_Offset = 0;
          
          if (Script_File.is_open())
          {
               //Get the filename only, no directories.
               for (int cou_Index=tmp_File.size();cou_Index>=0;cou_Index--)
               {
                    if (tmp_File[cou_Index] == char (92))
                    {
                         tmp_Table_Name_Offset = cou_Index + 1;
                         break;
                    }
               }
               
               for (unsigned int cou_Index=(tmp_Table_Name_Offset);(cou_Index<tmp_File.size() - 5);cou_Index++)
               {
                    tmp_Table_Name += tmp_File[cou_Index];
               }
               
               
               //If the file exists, create a table with the same name, sans the .ntxs extension.
               c_Table_1D * tmp_Table = Tables->register_New_Table_1D(tmp_Table_Name);
               
               CMD_Script_Registry->pushc_string(tmp_Table_Name);
               CMD_Script_Registry->push_pop_int(Tables->get_Table_ID(tmp_Table_Name));
               CMD_Script_Registry_Lookup_Tree.set_int(tmp_Table_Name, Tables->get_Table_ID(tmp_Table_Name));
               
               while (!Script_File.eof() )
               {
                    getline (Script_File, Command_Text);
                    cout << Command_Text << endl;
                    if (Command_Text.size())
                    {
                         tmp_Table->push_Data_Set_S(Command_Text);
                    }
               }
               Script_File.close();
          }
          
          cout << "\n\n\t Done Loading scriptfile(" << tmp_File << ") into Table->" << tmp_Table_Name;
          return "Done";
     }
     
     
     //This function saves a command table.
     
     
     //--      CALLBACKS FOR THE NT ENGINE      --//
     
     
     
     //This handles the callbacks for the overlord.
     string NT_overlord(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          string tmp_Token[100];
          
          //Gather all the tokens up to the limit of 10.
          for (int cou_Index=0;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
          {
               if (cou_Index >= 100){ break; }
               p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
               tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
               //cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
          }
          
          return "0";
     }
     
     //This handles the callbacks for the gathering.
     string NT_gathering(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          string tmp_Token[100];
          
          //Gather all the tokens up to the limit of 10.
          for (int cou_Index=0;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
          {
               if (cou_Index >= 100){ break; }
               p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
               tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
               //cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
          }
                    
                    
          //Submit an input to the input table.
          //construct submit_input -ASSEMBLY -ROW -"INPUT"
          //   0            1          2       3      4     
          if (tmp_Token[1] == "push_input")
          {
               Gathering->push_Input(str2int(tmp_Token[2]), tmp_Token[3]);
          }
          if (tmp_Token[1] == "bpush_input")
          {
               Gathering->push_Input(str2int(tmp_Token[2]), tmp_Token[3]);
          }
          
          //Submit an input to the input table.
          //construct submit_input -ASSEMBLY -ROW -CELL -"INPUT"
          //   0            1          2       3     4      5
          if (tmp_Token[1] == "set_input")
          {
               Gathering->set_Input(str2int(tmp_Token[2]), str2int(tmp_Token[3]), tmp_Token[4]);
          }
          if (tmp_Token[1] == "bset_input")
          {
               Gathering->set_Input(str2int(tmp_Token[2]), str2int(tmp_Token[3]), tmp_Token[4]);
          }
                    
                    
          //Submit an input to the input table.
          //construct submit_input -ASSEMBLY -ROW -"INPUT"
          //   0            1          2       3      4     
          if (tmp_Token[1] == "push_input_truth")
          {
               Gathering->push_Input_Truth(str2int(tmp_Token[2]), tmp_Token[3]);
          }
          if (tmp_Token[1] == "bpush_input_truth")
          {
               Gathering->push_Input_Truth(str2int(tmp_Token[2]), tmp_Token[3]);
          }
          
          //Submit an input to the input table.
          //construct submit_input -ASSEMBLY -ROW -CELL -"INPUT"
          //   0            1          2       3     4      5
          if (tmp_Token[1] == "set_input_truth")
          {
               Gathering->set_Input_Truth(str2int(tmp_Token[2]), str2int(tmp_Token[3]), tmp_Token[4]);
          }
          if (tmp_Token[1] == "bset_input_truth")
          {
               Gathering->set_Input_Truth(str2int(tmp_Token[2]), str2int(tmp_Token[3]), tmp_Token[4]);
          }
          
          return "0";
     }
     
     //This handles the callbacks for the assembly.
     string NT_assembly(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          string tmp_Token[100];
          
          //Gather all the tokens up to the limit of 10.
          for (int cou_Index=0;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
          {
               if (cou_Index >= 100){ break; }
               p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
               tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
               //cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
          }
          
          
          //Create a new assembly.
          if (tmp_Token[1] == "new")
          {
               cout << "\n\n\t new Assembly->" << tmp_Token[2];
               Gathering->create_Assembly(tmp_Token[2]);
               cout << "\n\t    Assembly->" << tmp_Token[2] << " created.";
               return "1";
          }
          
          
          //Create a new assembly.
          if (tmp_Token[1] == "new_Basic_Chrono_Predictor")
          {
               cout << "\n\n\t new Assembly->" << tmp_Token[2];
               Gathering->create_Assembly(tmp_Token[2], "Basic_Chrono_Predictor");
               cout << "\n\t    Assembly->" << tmp_Token[2] << " created.";
               return "1";
          }
          
          //==--     INTERFACE COMMANDS
          
          if (tmp_Token[1] == "Build")
          {
               return Gathering->Build(tmp_Token[2]);
          }
          if (tmp_Token[1] == "bBuild")
          {
               return Gathering->Build(str2int(tmp_Token[2]));
          }
          
          if (tmp_Token[1] == "Eval")
          {
               return Gathering->Eval(tmp_Token[2]);
          }
          if (tmp_Token[1] == "bEval")
          {
               return Gathering->Eval(str2int(tmp_Token[2]));
          }
          
          if (tmp_Token[1] == "Train")
          {
               return Gathering->Train(tmp_Token[2]);
          }
          if (tmp_Token[1] == "bTrain")
          {
               return Gathering->Train(str2int(tmp_Token[2]));
          }
          
          if (tmp_Token[1] == "Add_Raw_Construct")
          {
               return Gathering->Add_Raw_Construct(tmp_Token[2], tmp_Token[3]);
          }
          if (tmp_Token[1] == "bAdd_Raw_Construct")
          {
               return Gathering->Add_Raw_Construct(str2int(tmp_Token[2]), tmp_Token[3]);
          }
          
          
          //Checks to see if an assembly already has the given name.
          if (tmp_Token[1] == "exists")
          {
               cout << "\n\n\n\t Checking for existing assembly->" << tmp_Token[2] << " Status->" << Gathering->exists_Assembly(tmp_Token[2]);
               return Gathering->exists_Assembly(tmp_Token[2]);
          }
          
          //Delete an assembly.
          if (tmp_Token[1] == "delete")
          {
               cout << "\n\n\t delete Assembly->" << tmp_Token[2];
               Gathering->delete_Assembly(tmp_Token[2]);
               return "1";
          }
          if (tmp_Token[1] == "bdelete")
          {
               cout << "\n\n\t delete Assembly->" << tmp_Token[2];
               Gathering->delete_Assembly(str2int(tmp_Token[2]));
               return "1";
          }
          
          //Sets the name of an assembly.
          if (tmp_Token[1] == "set_name")
          {
               Gathering->set_Assembly_Name(tmp_Token[2], tmp_Token[3]);
               return "1";
          }
          if (tmp_Token[1] == "bset_name")
          {
               Gathering->set_Assembly_Name(str2int(tmp_Token[2]), tmp_Token[3]);
               return "1";
          }
          
          //Sets the name of an assembly.
          if (tmp_Token[1] == "get_name")
          {
               Gathering->get_Assembly_Name(str2int(tmp_Token[2]));
               return "1";
          }
          
          //Gets the ID of an assembly.
          if (tmp_Token[1] == "get_ID")
          {
               //cout << "\n\n\t get_ID(" << tmp_Token[2] << ")->" << Gathering->get_Assembly_ID(tmp_Token[2]);
               return Gathering->get_Assembly_ID(tmp_Token[2]);
          }
          
          
          
          //Submit an input to the input table.
          //construct submit_input -ASSEMBLY -ROW -"INPUT"
          //   0            1          2       3      4     
          if (tmp_Token[1] == "push_input")
          {
               Gathering->push_Input_To_Assembly(tmp_Token[2], str2int(tmp_Token[3]), tmp_Token[4]);
          }
          if (tmp_Token[1] == "bpush_input")
          {
               Gathering->push_Input_To_Assembly(str2int(tmp_Token[2]), str2int(tmp_Token[3]), tmp_Token[4]);
          }
          
          //Submit an input to the input table.
          //construct submit_input -ASSEMBLY -ROW -CELL -"INPUT"
          //   0            1          2       3     4      5
          if (tmp_Token[1] == "set_input")
          {
               Gathering->set_Input_In_Assembly(tmp_Token[2], str2int(tmp_Token[3]), str2int(tmp_Token[4]), tmp_Token[5]);
          }
          if (tmp_Token[1] == "bset_input")
          {
               Gathering->set_Input_In_Assembly(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]), tmp_Token[5]);
          }
          
          
          return "0";
     }
     
     //Handles the construct callbacks.
     string NT_construct(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          string tmp_Token[100];
          
          //Gather all the tokens up to the limit of 10.
          for (int cou_Index=0;cou_Index<p_Tokens->get_Row_Count();cou_Index++)
          {
               if (cou_Index >= 100){ break; }
               p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
               tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
               //cout << "\n\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index];
          }
          
          
          
          //Create a new construct in the given assembly.
          //construct new -ASSEMBLY -NAME -TYPE
          //   0       1       2       3     4
          if (tmp_Token[1] == "new")
          {
               cout << "\n\n\t new Construct->" << tmp_Token[3];
               Gathering->create_Construct(tmp_Token[2], tmp_Token[3], 0);
               return "1";
          }
          if (tmp_Token[1] == "bnew")
          {
               cout << "\n\n\t new Construct->" << tmp_Token[3];
               Gathering->create_Construct(str2int(tmp_Token[2]), tmp_Token[3], 0);
               return "1";
          }
          
          
          if (tmp_Token[1] == "new_Raw")
          {
               cout << "\n\n\t new Raw Construct->" << tmp_Token[3];
               Gathering->create_Construct(tmp_Token[2], tmp_Token[3], 1);
               return "1";
          }
          if (tmp_Token[1] == "bnew_Raw")
          {
               cout << "\n\n\t new Raw Construct->" << tmp_Token[3];
               Gathering->create_Construct(str2int(tmp_Token[2]), tmp_Token[3], 1);
               return "1";
          }
          
          
          if (tmp_Token[1] == "new_Chrono")
          {
               cout << "\n\n\t new Chrono Construct->" << tmp_Token[3];
               Gathering->create_Construct(tmp_Token[2], tmp_Token[3], 2);
               return "1";
          }
          if (tmp_Token[1] == "bnew_Chrono")
          {
               cout << "\n\n\t new Chrono Construct->" << tmp_Token[3];
               Gathering->create_Construct(str2int(tmp_Token[2]), tmp_Token[3], 2);
               return "1";
          }
          
          //Delete a construct from the assembly.
          if (tmp_Token[1] == "bdelete")
          {
               cout << "\n\n\t delete Construct->";
               Gathering->delete_Construct(str2int(tmp_Token[2]), str2int(tmp_Token[3]));
               return "1";
          }
          if (tmp_Token[1] == "delete")
          {
               cout << "\n\n\t delete Construct->";
               Gathering->delete_Construct(tmp_Token[2], tmp_Token[3]);
               return "1";
          }
          
          //Rename
          //construct set_name -ASSEMBLY -CONSTRUCT -NEW_NAME
          if (tmp_Token[1] == "set_name")
          {
               Gathering->set_Construct_Name(tmp_Token[2], tmp_Token[3], tmp_Token[4]);
          }
          if (tmp_Token[1] == "bset_name")
          {
               Gathering->set_Construct_Name(str2int(tmp_Token[2]), str2int(tmp_Token[3]), tmp_Token[4]);
          }
          
          //Get ID.
          if (tmp_Token[1] == "get_ID")
          {
               Gathering->get_Construct_ID(tmp_Token[2], tmp_Token[3]);
          }
          if (tmp_Token[1] == "bget_ID")
          {
               Gathering->get_Construct_ID(str2int(tmp_Token[2]), tmp_Token[3]);
          }
          
          //Get Name.
          if (tmp_Token[1] == "get_Name")
          {
               Gathering->get_Construct_Name(tmp_Token[2], str2int(tmp_Token[3]));
          }
          if (tmp_Token[1] == "bget_Name")
          {
               Gathering->get_Construct_Name(str2int(tmp_Token[2]), str2int(tmp_Token[3]));
          }
          
          //Checks to see if an assembly already has the given name.
          if (tmp_Token[1] == "exists")
          {
               cout << "\n\n\n\t Checking for existing construct->" << tmp_Token[2] << " Status->" << Gathering->exists_Construct(tmp_Token[2], tmp_Token[3]);
               return Gathering->exists_Construct(tmp_Token[2], tmp_Token[3]);
          }
          
          //Submit an input to the input table.
          //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -"INPUT"
          //   0            1          2          3       4      5     
          if (tmp_Token[1] == "push_input")
          {
               Gathering->push_Input_To_Construct(tmp_Token[2], tmp_Token[3], str2int(tmp_Token[4]), tmp_Token[5]);
          }
          if (tmp_Token[1] == "bpush_input")
          {
               Gathering->push_Input_To_Construct(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]), tmp_Token[5]);
          }
          
          //Submit an input to the input table.
          //construct submit_input -ASSEMBLY -CONSTRUCT -ROW -CELL -"INPUT"
          //   0            1          2          3       4    5       6    
          if (tmp_Token[1] == "set_input")
          {
               Gathering->set_Input_In_Construct(tmp_Token[2], tmp_Token[3], str2int(tmp_Token[4]), str2int(tmp_Token[5]), tmp_Token[6]);
          }
          if (tmp_Token[1] == "bset_input")
          {
               Gathering->set_Input_In_Construct(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2int(tmp_Token[5]), tmp_Token[6]);
          }
          
          //Eval an input table.
          //construct eval -ASSEMBLY -CONSTRUCT -BUILDING_METHOD
          //   0        1      2          3            4
          if (tmp_Token[1] == "eval")
          {
               Gathering->eval_Construct(tmp_Token[2], tmp_Token[3], tmp_Token[4]);
          }
          if (tmp_Token[1] == "beval")
          {
               Gathering->eval_Construct(str2int(tmp_Token[2]), str2int(tmp_Token[3]), tmp_Token[4]);
          }
          
          //Evals the output tables passed to it as its own input tables.
          //The Node_Table is used for inputs, and the Charge_Table for the input charges.
          //It's enough to make a man throw up. Sure is hard to figure How any decent girl could fuck a crazy nigger.
          //construct eval_foreign_output -ASSEMBLY -CONSTRUCT -Node_Table -Charge_Table
          //    0               1              2         3           4           5
          if (tmp_Token[1] == "eval_foreign_output")
          {
               Gathering->eval_Foreign_Output_Table(tmp_Token[2], tmp_Token[3], tmp_Token[4], tmp_Token[5]);
          }
          if (tmp_Token[1] == "beval_foreign_output")
          {
               Gathering->eval_Foreign_Output_Table(str2int(tmp_Token[2]), str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2int(tmp_Token[5]));
          }
          
          //Build an input table.
          //construct build -ASSEMBLY -CONSTRUCT -BUILDING_METHOD
          //   0        1      2          3            4
          if (tmp_Token[1] == "build")
          {
               Gathering->build_Construct(tmp_Token[2], tmp_Token[3], tmp_Token[4]);
          }
          if (tmp_Token[1] == "bbuild")
          {
               Gathering->build_Construct(str2int(tmp_Token[2]), str2int(tmp_Token[3]), tmp_Token[4]);
          }
          
          //==--     OUTPUT FUNCTIONS FOR THE CONSTRUCT
          
          if (tmp_Token[1] == "output")
          {
               if (tmp_Token[2] == "CAN")
               {
                    if (tmp_Token[3] == "all"){ Gathering->output_Every_CAN(tmp_Token[4], tmp_Token[5]); }
                    Gathering->output_given_CAN(tmp_Token[4], tmp_Token[5], str2int(tmp_Token[6]));
               }
               
               if (tmp_Token[2] == "node_network")
               {
                    Gathering->output_Node_Network(tmp_Token[3], tmp_Token[4]);
               }
               
               if (tmp_Token[2] == "tables")
               {
                    Gathering->output_Construct_Tables(tmp_Token[3], tmp_Token[4]);
               }
               
               if (tmp_Token[2] == "internals")
               {
                    Gathering->output_Construct_Internals(tmp_Token[3], tmp_Token[4]);
               }
               
               if (tmp_Token[2] == "full")
               {
                    Gathering->output_Construct_Full(tmp_Token[3], tmp_Token[4]);
               }
          }
          
          if (tmp_Token[1] == "boutput")
          {
               if (tmp_Token[2] == "CAN")
               {
                    if (tmp_Token[3] == "all"){ Gathering->output_Every_CAN(str2int(tmp_Token[4]), str2int(tmp_Token[5])); }
                    Gathering->output_given_CAN(str2int(tmp_Token[3]), str2int(tmp_Token[4]), str2int(tmp_Token[5]));
               }
               
               if (tmp_Token[2] == "node_network")
               {
                    Gathering->output_Node_Network(str2int(tmp_Token[3]), str2int(tmp_Token[4]));
               }
               
               if (tmp_Token[2] == "tables")
               {
                    Gathering->output_Construct_Tables(str2int(tmp_Token[3]), str2int(tmp_Token[4]));
               }
               
               if (tmp_Token[2] == "internals")
               {
                    Gathering->output_Construct_Internals(str2int(tmp_Token[3]), str2int(tmp_Token[4]));
               }
               
               if (tmp_Token[2] == "full")
               {
                    Gathering->output_Construct_Full(str2int(tmp_Token[3]), str2int(tmp_Token[4]));
               }
          }
          
          return "0";
     }
     
     string CMD_operation(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          string tmp_Token[100];
          bool tmp_Token_Done[100];
          
          p_Tokens->output_Table();
          
          //This flag is set when searching for assignment operators, when nothing is left to evaluate the result is returned.
          int flg_Done = 0;
          string flg_Datatype = "int";
          string tmp_Datatype = "int";
          string tmp_Data = "";
          int tmp_Number_Of_Tokens = p_Tokens->get_Row_Count();
          int tmp_Token_Offset = 0;
          
          //Gather all the tokens up to the limit of 10.
          for (int cou_Index=0;cou_Index<tmp_Number_Of_Tokens;cou_Index++)
          {
               if (cou_Index >= 100){ break; }
               p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(cou_Index, 0, &tmp_Token[cou_Index]);
               tmp_Token[cou_Index] = test_Command_Token(tmp_Token[cou_Index], p_EIP, p_Labels, p_Tokens);
               
          }
          
          //This is so it does not try to operate on the com token.
          tmp_Token_Done[0] = 1;
          
          //Gather all the tokens up to the limit of 10.
          for (int cou_Index=1;cou_Index<tmp_Number_Of_Tokens;cou_Index++)
          {
               if (cou_Index >= 100){ break; }
               
               //Set the done flags.
               tmp_Token_Done[cou_Index] = 0;
               
               //If the operation is found then continue.
               if (tmp_Token[cou_Index] == "+" || tmp_Token[cou_Index] == "-" || tmp_Token[cou_Index] == "*" || tmp_Token[cou_Index] == "/"){ tmp_Token_Done[cou_Index] = 1; continue; }
               
               //Test the datatypes and set to higher type if found.
               tmp_Datatype = CMD_get_datatype(tmp_Token[cou_Index]);
               
               ////cout << "\n\t\t tmp_Token[" << cou_Index << "]->" << tmp_Token[cou_Index] << " tmp_Data_Type->" << tmp_Datatype;
               
               if (tmp_Datatype == "string"){ flg_Datatype = "string"; }
               if (tmp_Datatype == "float" && flg_Datatype == "int"){ flg_Datatype = "float"; }
          }
          
          ////cout << "\n\n\t DataType->" << flg_Datatype;
          
          if (flg_Datatype == "string")
          {          
               for (int cou_Index=1;cou_Index<tmp_Number_Of_Tokens;cou_Index++)
               {
                    if (tmp_Token[cou_Index] == "+" || tmp_Token[cou_Index] == "-" || tmp_Token[cou_Index] == "*" || tmp_Token[cou_Index] == "/"){ continue; }
                    
                    tmp_Data += tmp_Token[cou_Index];
               }
               return tmp_Data;
               
          }
          
          if (flg_Datatype == "float")
          {
               //Get the highest priority items.
               flg_Done = 4;
               while (flg_Done)
               {
                    flg_Done = 0;
                    for (int cou_Index=1;cou_Index<tmp_Number_Of_Tokens;cou_Index++)
                    {
                         if (tmp_Token[cou_Index] == "*"){ flg_Done = 4;  break; }
                         if (tmp_Token[cou_Index] == "/"){ flg_Done = 3;  continue; }
                         if (tmp_Token[cou_Index] == "+" && flg_Done < 3){ flg_Done = 2; continue; }
                         if (tmp_Token[cou_Index] == "-" && flg_Done < 2){ flg_Done = 1; }
                    }
                    
                    
                    
                    if (flg_Done == 4)
                    {
                         //Loop through looking for *, when found multiply the tokens on either side.
                         for (int cou_Index=1;cou_Index<tmp_Number_Of_Tokens;cou_Index++)
                         {
                              if (tmp_Token[cou_Index] == "*")
                              {
                                   //Finds a good token on the left to use.
                                   tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);
                                   
                                   tmp_Token[cou_Index - tmp_Token_Offset] = float2str(str2float(tmp_Token[cou_Index - tmp_Token_Offset]) * str2float(tmp_Token[cou_Index + 1]));
                                   tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;
                                   
                                   tmp_Token_Done[cou_Index] = 1;
                                   tmp_Token[cou_Index] = "";
                                   tmp_Token_Done[cou_Index + 1] = 1;
                                   tmp_Token[cou_Index + 1] = "";
                              }
                         }
                    }
                    
                    if (flg_Done == 3)
                    {
                         //Loop through looking for *, when found multiply the tokens on either side.
                         for (int cou_Index=1;cou_Index<tmp_Number_Of_Tokens;cou_Index++)
                         {
                              if (tmp_Token[cou_Index] == "/")
                              {
                                   //Finds a good token on the left to use.
                                   tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);
                                   tmp_Token[cou_Index - tmp_Token_Offset] = float2str(str2float(tmp_Token[cou_Index - tmp_Token_Offset]) / str2float(tmp_Token[cou_Index + 1]));
                                   tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;
                                   
                                   tmp_Token_Done[cou_Index] = 1;
                                   tmp_Token[cou_Index] = "";
                                   tmp_Token_Done[cou_Index + 1] = 1;
                                   tmp_Token[cou_Index + 1] = "";
                              }
                         }
                         
                    }
                    
                    if (flg_Done == 2)
                    {
                         //Loop through looking for *, when found multiply the tokens on either side.
                         for (int cou_Index=1;cou_Index<tmp_Number_Of_Tokens;cou_Index++)
                         {
                              if (tmp_Token[cou_Index] == "+")
                              {
                                   //Finds a good token on the left to use.
                                   tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);
                                   
                                   tmp_Token[cou_Index - tmp_Token_Offset] = float2str(str2float(tmp_Token[cou_Index - tmp_Token_Offset]) + str2float(tmp_Token[cou_Index + 1]));
                                   tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;
                                   
                                   tmp_Token_Done[cou_Index] = 1;
                                   tmp_Token[cou_Index] = "";
                                   tmp_Token_Done[cou_Index + 1] = 1;
                                   tmp_Token[cou_Index + 1] = "";
                              }
                         }
                    }
                    
                    if (flg_Done == 1)
                    {
                         //Loop through looking for *, when found multiply the tokens on either side.
                         for (int cou_Index=1;cou_Index<tmp_Number_Of_Tokens;cou_Index++)
                         {
                              if (tmp_Token[cou_Index] == "-")
                              {
                                   //Finds a good token on the left to use.
                                   tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);
                                   tmp_Token[cou_Index - tmp_Token_Offset] = float2str(str2float(tmp_Token[cou_Index - tmp_Token_Offset]) - str2float(tmp_Token[cou_Index + 1]));
                                   tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;
                                   
                                   tmp_Token_Done[cou_Index] = 1;
                                   tmp_Token[cou_Index] = "";
                                   tmp_Token_Done[cou_Index + 1] = 1;
                                   tmp_Token[cou_Index + 1] = "";
                              }
                         }
                    }
               }
          }
          
          
          if (flg_Datatype == "int")
          {
               //Get the highest priority items.
               flg_Done = 4;
               while (flg_Done)
               {
                    flg_Done = 0;
                    for (int cou_Index=1;cou_Index<tmp_Number_Of_Tokens;cou_Index++)
                    {
                         if (tmp_Token[cou_Index] == "*"){ flg_Done = 4;  break; }
                         if (tmp_Token[cou_Index] == "/"){ flg_Done = 3;  continue; }
                         if (tmp_Token[cou_Index] == "+" && flg_Done < 3){ flg_Done = 2; continue; }
                         if (tmp_Token[cou_Index] == "-" && flg_Done < 2){ flg_Done = 1; }
                    }
                    
                    
                    
                    if (flg_Done == 4)
                    {
                         //Loop through looking for *, when found multiply the tokens on either side.
                         for (int cou_Index=1;cou_Index<tmp_Number_Of_Tokens;cou_Index++)
                         {
                              if (tmp_Token[cou_Index] == "*")
                              {
                                   //Finds a good token on the left to use.
                                   tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);
                                   
                                   tmp_Token[cou_Index - tmp_Token_Offset] = int2str(str2int(tmp_Token[cou_Index - tmp_Token_Offset]) * str2int(tmp_Token[cou_Index + 1]));
                                   tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;
                                   
                                   tmp_Token_Done[cou_Index] = 1;
                                   tmp_Token[cou_Index] = "";
                                   tmp_Token_Done[cou_Index + 1] = 1;
                                   tmp_Token[cou_Index + 1] = "";
                              }
                         }
                    }
                    
                    if (flg_Done == 3)
                    {
                         //Loop through looking for *, when found multiply the tokens on either side.
                         for (int cou_Index=1;cou_Index<tmp_Number_Of_Tokens;cou_Index++)
                         {
                              if (tmp_Token[cou_Index] == "/")
                              {
                                   //Finds a good token on the left to use.
                                   tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);
                                   tmp_Token[cou_Index - tmp_Token_Offset] = int2str(str2int(tmp_Token[cou_Index - tmp_Token_Offset]) / str2int(tmp_Token[cou_Index + 1]));
                                   tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;
                                   
                                   tmp_Token_Done[cou_Index] = 1;
                                   tmp_Token[cou_Index] = "";
                                   tmp_Token_Done[cou_Index + 1] = 1;
                                   tmp_Token[cou_Index + 1] = "";
                              }
                         }
                         
                    }
                    
                    if (flg_Done == 2)
                    {
                         //Loop through looking for *, when found multiply the tokens on either side.
                         for (int cou_Index=1;cou_Index<tmp_Number_Of_Tokens;cou_Index++)
                         {
                              if (tmp_Token[cou_Index] == "+")
                              {
                                   //Finds a good token on the left to use.
                                   tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);
                                   
                                   tmp_Token[cou_Index - tmp_Token_Offset] = int2str(str2int(tmp_Token[cou_Index - tmp_Token_Offset]) + str2int(tmp_Token[cou_Index + 1]));
                                   tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;
                                   
                                   tmp_Token_Done[cou_Index] = 1;
                                   tmp_Token[cou_Index] = "";
                                   tmp_Token_Done[cou_Index + 1] = 1;
                                   tmp_Token[cou_Index + 1] = "";
                              }
                         }
                    }
                    
                    if (flg_Done == 1)
                    {
                         //Loop through looking for *, when found multiply the tokens on either side.
                         for (int cou_Index=1;cou_Index<tmp_Number_Of_Tokens;cou_Index++)
                         {
                              if (tmp_Token[cou_Index] == "-")
                              {
                                   //Finds a good token on the left to use.
                                   tmp_Token_Offset = find_good_token(tmp_Token_Done, cou_Index);
                                   tmp_Token[cou_Index - tmp_Token_Offset] = int2str(str2int(tmp_Token[cou_Index - tmp_Token_Offset]) - str2int(tmp_Token[cou_Index + 1]));
                                   tmp_Token_Done[cou_Index - tmp_Token_Offset] = 0;
                                   
                                   tmp_Token_Done[cou_Index] = 1;
                                   tmp_Token[cou_Index] = "";
                                   tmp_Token_Done[cou_Index + 1] = 1;
                                   tmp_Token[cou_Index + 1] = "";
                              }
                         }
                    }
               }
          }
          
          return tmp_Token[1];
          
     }
     
     //When setting the tokens they will always be set to the left of the operation. In this way I will only need to search to the lft for good tokens to use.
     int find_good_token(bool p_Tokens[], int p_Token_Length)
     {
          int tmp_Shift = -1;
          for (int cou_Index=p_Token_Length;cou_Index>0;cou_Index--)
          {
               //If a good token is found then return the offset to the current index.
               tmp_Shift++;
               if (p_Tokens[cou_Index] == 0){ return tmp_Shift; }
          }
          return -1;
     }
     
     string CMD_get_datatype(int * p_EIP, c_Lookup_Tree * p_Labels, c_Table_1D * p_Tokens)
     {
          string tmp_Data;
          p_Tokens->get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(1, 0, &tmp_Data);
          return CMD_get_datatype(tmp_Data);
     }
     
     string CMD_get_datatype(string p_Data)
     {
          // > 57 = char
          // < 48 && !46 = char
          // = 46 = float
          bool flg_Int = true;
          bool flg_Float = false;
          
          for (unsigned int cou_Index=0;cou_Index<(p_Data.size());cou_Index++)
          {
               if (int (p_Data[cou_Index]) > 57){ return "string"; }
               if (int (p_Data[cou_Index]) < 48 && int (p_Data[cou_Index]) != 46 && int (p_Data[cou_Index]) != 45){ return "string"; }
               if (int (p_Data[cou_Index]) == 45 && cou_Index > 0){ return "string"; }
               if (int (p_Data[cou_Index]) == 46){ flg_Float = true; flg_Int = false; }
          }
          if (flg_Float){ return "float"; }
          if (flg_Int){ return "int"; }
          return "string";
     }
     
};
