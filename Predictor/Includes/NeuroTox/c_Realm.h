//This file contains the classes for the organism simulator.




class c_Unit
{
private:
     
     //Units health
     int HP;
     
     //The amount of damage dealt to another units HP.
     int Attack;
     
     //The tiles per tick that a unit can move.
     int Speed;
     
     //The radius of the units visual input.
     int Sight_Radius;
     
     //The units current position.
     int Position[3];
     
     c_Unit(int p_HP = 10, int p_Attack = 2, int p_Speed = 10, int p_Sight_Radius = 4, int p_Z = 0, int p_X = 0, int p_Y = 0)
     {
          HP = p_HP;
          Attack = p_Attack;
          Speed = p_Speed;
          Sight_Radius = p_Sight_Radius;
          Position[0] = p_Z;
          Position[1] = p_X;
          Position[2] = p_Y;
          cout << " HP" << ":" << HP << " Attack" << ":" << Attack << " Speed" << ":" << Speed << " Sight_Radius" << ":" << Sight_Radius;
     }
      
     ~c_Unit()
     {
          
     }
     
     int roll_Damage()
     {
          //Damage is Attack || Attack * 2;
          return (rand() % Attack) + Attack;
     }
     
     int die()
     {
          HP = 0;
          
          //Death.
          
          return 0;
     }
     
public:
     
     int in_Attack(c_Unit * p_Target_Unit = NULL)
     {
          //If there is no target unit return 0.
          if (p_Target_Unit == NULL){ return 0; }
          
          //Set temporary damage tracker.
          int tmp_Damage = roll_Damage();
          
          p_Target_Unit->in_Take_Damage(tmp_Damage);
          
          return tmp_Damage;
     }
     
     int in_Take_Damage(int p_Damage)
     {
          HP = HP - p_Damage;
          
          //If the units HP is below 0 then the unit dies.
          if (HP < 0){ die(); return 0; }
     }
     
     void display_Unit()
     {
          cout << "#";
     }
};

//This class is the unit handler for the simulator.
class c_Unit_Handler
{
private:
     
     //The units themselves.
     c_Unit ** Units;
     
     //The unit ID and tracker.
     u_IO_Data Units_Current_Count;
     
     c_Unit_Handler()
     {
          
     }
     
     ~c_Unit_Handler()
     {
          delete [] Units;
     }
     
public:
     
}; 

class c_Map_Tile
{
private:
     
     //The tiles display character.
     u_IO_Data Tile_Skin;
     
     //The tiles type; water, dirt, rock, grass, etc.
     int Type;
     
public:
     
     //p_Type:0 is default of dirt.
     c_Map_Tile(char p_Tile_Skin = 'x', int p_Type = 0)
     {
          Tile_Skin.I = int (p_Tile_Skin);
          Type = p_Type;
     }
     
     void in_Output_Map_Tile()
     {
          cout << char (Tile_Skin.I);
     }
};

class c_Map
{
private:
     
     //Map dimensions.
     int Dimension;
     
     //The map tiles.
     c_Map_Tile *** Tiles;
     
     c_Map(int p_Dimension = 10)
     {
          //Set the dimension.
          Dimension = p_Dimension;
          
          //Create the map.
          Tiles = new c_Map_Tile**[Dimension];
          
          for (int cou_Z=0;cou_Z<Dimension;cou_Z++){
               Tiles[cou_Z] = new c_Map_Tile*[Dimension];
               
               for (int cou_X=0;cou_X<Dimension;cou_X++){
                    Tiles[cou_Z] [cou_X] = new c_Map_Tile[Dimension];
               }
          }
     }
     
public:
     
     void in_Output_Map(int p_Z = 0)
     {
          for (int cou_X=0;cou_X<Dimension;cou_X++){
               for (int cou_Y=0;cou_Y<Dimension;cou_Y++){
                    console_Move_Cursor_To_Given_X_And_Y(cou_X, cou_Y);
                    Tiles[p_Z] [cou_X] [cou_Y].in_Output_Map_Tile();
               }
          }
     }
     
};








