#include <fstream>
#include <windows.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
//#include <stdio.h>
#include <time.h>
#include "Cube.h"

using namespace std;

Cube cube[10];
Cube cStop[100];
Cube star[15][15];
Cube Pad[4];
Cube StSqr[15][15];

char //path[100] = "//Aelserver/Public/Jocsc/Scores.dat",
     //ver[100] = "//Aelserver/Public/Jocsc/V.dat",
     path[100] = "Scores.txt",
     ver[100] = "V.txt",
     user[100],
     name[100][100];

void display(),
     Dcl(),
     ScRead(),
     ScWrite(),
     Order(),
     GenStars(),
     GenStarsRow( int i, int j, int start ),
     PadClr( float color ),
     Clr( int i ),
     Draw( float X, float sX, float Y, float sY, float C1r, float C1g, float C1b, float C1a, float C2r, float C2g, float C2b, float C2a, float C3r, float C3g, float C3b, float C3a, float C4r, float C4g, float C4b, float C4a ),
     Nkeys(unsigned char key, int x, int y),
     keys ( int key, int x, int y ),
     keysUp ( int key, int x, int y );
     
long long GetTime();
     
int NameChk();

float vodka(float i);

float rnd,
      fps,
      Speed,
      StarspdX = 0.0f,
      StarspdY = 0.0f,
      PposY = -0.7f,
      PsizX = 0.4f,
      PsizY = 0.05f,
      CsizX = 0.1f,
      CsizY = 0.1f,
      CspdV = 0.0075,
      PspdV = 0.015,
      dist[10],
      mindist = 0;
      
int i, j, k, l,
    gameversion = 12,
    verchk = 0,
    lives = 5,
    end = 0,
    pts = 0,
    score = 0,
    highscore[100],
    users = 0,
    rplc = 0,
    ptloss = 5,
    userpos = 0,
    chk = 0,
    MLeft = 0,
    MRight = 0,
    BMLeft = 0,
    BMRight = 0,
    Cubes = 1,
    Stop = 0,
    colorRow = 0,
    mindistpos = 0;
    
int frame;

bool bot = false,
     good = false;

long long Time,
          RTime,
          eTime,
          timebase;
     
char FPS[10]   = {'\0'},
     LIVE[10]  = {'\0'},
     HGSC[10]  = {'\0'},
     ORDER[10] = {'\0'},
     SC[10]    = {'\0'},
     COMBO[10] = {'\0'};

void renderBitmapString( float x, float y, float z, void *font, char *string )
{
     char *c = NULL;
     glRasterPos3f( x, y, z );
     for ( c = string; *c != '\0'; c++ )
     {
		 glutBitmapCharacter( font, *c );
	 }
}
int main ( int argc, char** argv )
{
    Dcl();
    ifstream ferb(ver);
    ferb >> verchk;
    ferb.close();
    if ( verchk > gameversion )
    {
         gameversion = verchk;
         system("start Upd.exe");
         exit(0);
    }
    srand (time(NULL));
    
    ifstream ferb2("NUME.txt");
    ferb2 >> user;
    ferb2.close();
    
    glutInit(&argc,argv);
    glutInitWindowSize( 800, 800 );
    glutInitWindowPosition( 200, 50 );
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    int Mwin = glutCreateWindow("CubeFall");
    
    glutDisplayFunc(display);
    glutIdleFunc(display);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glutKeyboardFunc(Nkeys);
    glutSpecialFunc(keys);
    glutSpecialUpFunc(keysUp);
    
    glutMainLoop();
}
void display ()
{
     Speed = GetTime();
     Speed = Speed / 10;
     if ( end == 0 )
     {
          for ( i = 1; i <= 3; i++ )
          {
              if ( bot == false )
              {
                   if ( MLeft == 1 )
                   {
                        Pad[i].posX -= PspdV * Speed;
                   }
                   else if ( MRight == 1 )
                   {
                        Pad[i].posX += PspdV * Speed;
                   }
              }
              else if ( bot == true )
              {
                   if ( BMLeft == 1 )
                   {
                        Pad[i].posX -= PspdV * Speed;
                   }
                   else if ( BMRight == 1 )
                   {
                        Pad[i].posX += PspdV * Speed;
                   }
              }
          }
     }
     if ( bot == true )
     {
          good = false;
          for ( i = 1; i <= 3; i++ )
          {
              float cubepos = cube[2].posX + ( cube[2].sizX / 2 );
              float padpos  = Pad[i].posX + ( PsizX / 2 );
              dist[i] = cubepos - padpos;
          }
          mindist = dist[1];
          mindistpos = 1;
          for ( i = 1; i <= 3; i++ )
          {
              if ( vodka(dist[i]) < vodka(mindist) )
              {
                    mindist = vodka(dist[i]);
                    mindistpos = i;
              }
          }
          if ( mindist < 0.1f && mindist > -0.1f )
          {
               BMLeft = 0;
               BMRight = 0;
               good = true;
          }
          if ( good == false )
          {
                if ( dist[mindistpos] < 0 )
                {
                     BMLeft = 1;
                     BMRight = 0;
                }
                else if ( dist[mindistpos] > 0 )
                {
                     BMRight = 1;
                     BMLeft = 0;
                }
          }
     }
     if ( end != 2 )
     {
          for ( i = 0; i <= 11; i ++ )
          {
              for ( j = 0; j <= 11; j++ )
              {
                  star[i][j].posX += StarspdX * Speed;
                  StSqr[i][j].posX += StarspdX * Speed;
                  star[i][j].posY += StarspdY * Speed;
                  StSqr[i][j].posY += StarspdY * Speed;
              }
          }
     }
     if ( StSqr[10][0].posX > 1.0f )
     {
          for ( i = 11; i >= 1; i-- )
          {
              for ( j = 11; j >= 0; j-- )
              {
                  star[i][j].posX = star[i-1][j].posX;
                  star[i][j].posY = star[i-1][j].posY;
                  star[i][j].sizX = star[i-1][j].sizX;
                  star[i][j].sizY = star[i-1][j].sizY;
                  StSqr[i][j].posX = StSqr[i-1][j].posX;
                  StSqr[i][j].posY = StSqr[i-1][j].posY;
              }
          }
          for ( j = 0; j <= 11; j++ )
          {
              GenStarsRow( 0, j, 1 );
          }
     }
     else if ( StSqr[1][0].posX < -1.2f )
     {
          for ( i = 0; i <= 10; i++ )
          {
              for ( j = 0; j <= 11; j++ )
              {
                  star[i][j].posX = star[i+1][j].posX;
                  star[i][j].posY = star[i+1][j].posY;
                  star[i][j].sizX = star[i+1][j].sizX;
                  star[i][j].sizY = star[i+1][j].sizY;
                  StSqr[i][j].posX = StSqr[i+1][j].posX;
                  StSqr[i][j].posY = StSqr[i+1][j].posY;
              }
          }
          for ( j = 0; j <= 11; j++ )
          {
              GenStarsRow( 11, j, 1 );
          }
     }
     if ( StSqr[0][1].posY < -1.0f )
     {
          for ( i = 0; i <= 11; i++ )
          {
              for ( j = 0; j <= 10; j++ )
              {
                  star[i][j].posX = star[i][j+1].posX;
                  star[i][j].posY = star[i][j+1].posY;
                  star[i][j].sizX = star[i][j+1].sizX;
                  star[i][j].sizY = star[i][j+1].sizY;
                  StSqr[i][j].posX = StSqr[i][j+1].posX;
                  StSqr[i][j].posY = StSqr[i][j+1].posY;
              }
          }
          for ( i = 0; i <= 11; i++ )
          {
              GenStarsRow( i, 11, 2 );
          }
     }
     else if ( StSqr[0][10].posY > 1.2f )
     {
          for ( i = 11; i >= 0; i-- )
          {
              for ( j = 11; j >= 1; j-- )
              {
                  star[i][j].posX = star[i][j-1].posX;
                  star[i][j].posY = star[i][j-1].posY;
                  star[i][j].sizX = star[i][j-1].sizX;
                  star[i][j].sizY = star[i][j-1].sizY;
                  StSqr[i][j].posX = StSqr[i][j-1].posX;
                  StSqr[i][j].posY = StSqr[i][j-1].posY;
              }
          }
          for ( i = 0; i <= 11; i++ )
          {
              GenStarsRow( i, 0, 2 );
          }
     }
     glClear( GL_COLOR_BUFFER_BIT );
     glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
     glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     
     for ( i = 0; i <= 11; i ++ )
     {
         for ( j = 0; j <= 11; j++ )
         {
             Draw( star[i][j].posX, star[i][j].sizX, star[i][j].posY, star[i][j].sizY, 0.75f, 0.75f, 0.75f, 1.0f, 0.75f, 0.75f, 0.75f, 1.0f, 0.75f, 0.75f, 0.75f, 1.0f, 0.75f, 0.75f, 0.75f, 1.0f );
         }
     }
     for ( i = 1; i <= 3; i++ )
     {
         Draw( Pad[i].posX, PsizX, Pad[i].posY, PsizY, Pad[i].clrR, Pad[i].clrG, Pad[i].clrB, 1.0f, Pad[i].clrR, Pad[i].clrG, Pad[i].clrB, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f );
     }
     for ( i = 2; i <= Cubes; i++ )
     {
         if ( cube[i].type == 1 )
         {
              Draw( cube[i].posX, cube[i].sizX, cube[i].posY, cube[i].sizY, cube[i].clrR, cube[i].clrG, cube[i].clrB, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f );
         }
         else if ( cube[i].type == 2 )
         {
              glBegin(GL_QUADS);
              glColor3f ( 1.0f, 1.0f, 1.0f );
              glVertex3f ( cube[i].posX + cube[i].sizX / 2, cube[i].posY                   , 0.0f );
              glVertex3f ( cube[i].posX + cube[i].sizX    , cube[i].posY - cube[i].sizY / 2, 0.0f );
              glVertex3f ( cube[i].posX + cube[i].sizX / 2, cube[i].posY - cube[i].sizY    , 0.0f );
              glVertex3f ( cube[i].posX                   , cube[i].posY - cube[i].sizY / 2, 0.0f );
              glEnd();
         }
         for ( j = 0; j < Stop; j++ )
         {
              Draw( cStop[j].posX, cStop[j].sizX, cStop[j].posY, cStop[j].sizY, cStop[j].clrR, cStop[j].clrG, cStop[j].clrB, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f );
         }
         if ( end == 0 )
         {
              for ( j = 0; j < Stop; j++ )
              {
                  if ( cube[i].posX + cube[i].sizX > cStop[j].posX && cube[i].posX < cStop[j].posX + cStop[j].sizX && cube[i].posY - cube[i].sizY < cStop[j].posY )
                  {
                       if ( cube[i].type != 2 )
                       {
                            cStop[Stop] = cube[i];
                            cStop[Stop].posY = cStop[j].posY + cube[i].sizY;
                            Stop++;
                            Cubes--;
                            for ( k = i; k <= Cubes; k++ )
                            {
                                cube[k] = cube[k+1];
                            }
                            end = 1;
                       }
                  }
              }
              if ( end == 0 )
              {  
                   if ( cube[i].posY - cube[i].sizY >= -1.0f )
                   {
                        cube[i].posY -= CspdV * Speed;
                   }
                   else 
                   {
                        if ( cube[i].type != 2 )
                        {
                             cStop[Stop] = cube[i];
                             cStop[Stop].posY = -1.0f + cube[i].sizY;
                             Stop++;
                             lives--;
                        }
                        for ( k = i; k <= Cubes; k++ )
                        {
                            cube[k] = cube[k+1];
                        }
                        Cubes--;
                        
                   }
              }
         }
     }
     if ( cube[Cubes].posY < 0.5f )
     {
          cube[Cubes+1].posY = 1.0f + CsizY;
          rnd = rand() % 1900 - 1000;
          cube[Cubes+1].posX = rnd / 1000;
          cube[Cubes+1].sizX = CsizX;
          cube[Cubes+1].sizY = CsizY;
          
          rnd = rand() % 100;
          if ( rnd == 17 )
          {
               cube[Cubes+1].type = 2;
          }
          else 
          {
               cube[Cubes+1].type = 1;
               Clr(Cubes+1);
          }
          Cubes++;
     }
     for ( i = 2; i <= Cubes; i++ )
     {
         for ( j = 1; j <= 3; j++ )
         {
             if ( cube[i].posX + cube[i].sizX > Pad[j].posX && cube[i].posX < Pad[j].posX + PsizX && cube[i].posY - cube[i].sizY < Pad[j].posY && cube[i].posY > Pad[j].posY - PsizY )
             {
                  if ( cube[i].type == 2 )
                  {
                       lives++;
                       PadClr( 0.5f );
                  }
                  else if ( cube[i].type == 1 )
                  {
                       if ( Pad[2].clrR == cube[i].clrR && Pad[2].clrG == cube[i].clrG && Pad[2].clrB == cube[i].clrB )
                       {
                            colorRow++;
                            pts += colorRow;
                       }
                       else
                       {
                           colorRow = 1;
                           pts += colorRow;
                       }
                       CspdV += 0.00005 * colorRow;
                       PspdV += 0.00005 * colorRow;
                       for ( l = 1; l <= 3; l++ )
                       {
                           Pad[l].clrR = cube[i].clrR;
                           Pad[l].clrG = cube[i].clrG;
                           Pad[l].clrB = cube[i].clrB;
                       }
                  }
                  for ( j = i; j <= Cubes; j++ )
                  {
                      cube[j] = cube[j+1];
                  }
                  Cubes--;
             }
         }
     }
     if ( Pad[2].posX <= -1.0f - PsizX )
     {
          Pad[2].posX += 2.0f;
          Pad[1].posX = Pad[2].posX - 2.0f;
          Pad[3].posX = Pad[2].posX + 2.0f;
     }
     if ( Pad[2].posX >= 1.0f )
     {
          Pad[2].posX -= 2.0f;
          Pad[1].posX = Pad[2].posX - 2.0f;
          Pad[3].posX = Pad[2].posX + 2.0f;
     }
     frame++;
     Time = glutGet(GLUT_ELAPSED_TIME);
     if ( Time - timebase > 1000)
     {
          sprintf(FPS,"FPS:%d  ", frame);
          timebase = Time;
          fps = frame;
          frame = 0;
     }
     if ( lives == 0 )
     {
          end = 1;
     }
     for ( i = 1; i <= lives; i++ )
     {
         glBegin(GL_QUADS);
         glColor3f ( 1.0f, 1.0f, 1.0f );
         glVertex3f ( -0.028f + i * 0.05f, 0.98f, 0.0f );
         glVertex3f ( -0.048f + i * 0.05f, 0.96f, 0.0f );
         glVertex3f ( -0.028f + i * 0.05f, 0.94f, 0.0f );
         glVertex3f ( -0.008f + i * 0.05f, 0.96f, 0.0f );
         glEnd();
     }
     if ( end != 0 )
     {
          Draw( -0.45, 0.9f, 0.6f, 1.3f, 0.0f, 0.0f, 0.0f, 0.75f, 0.0f, 0.0f, 0.0f, 0.75f, 0.0f, 0.0f, 0.0f, 0.75f, 0.0f, 0.0f, 0.0f, 0.75f );
          glBegin(GL_LINE_LOOP);
          glColor4f ( 0.5f, 0.5f, 0.5f, 1.0f );
          glVertex3f ( -0.45f,  0.6f, 0.0f );
          glVertex3f (  0.45f,  0.6f, 0.0f );
          glVertex3f (  0.45f, -0.7f, 0.0f );
          glVertex3f ( -0.45f, -0.7f, 0.0f );
          glEnd();
          glColor4f ( 0.5f, 0.5f, 1.0f, 1.0f );
          if ( score == 0 )
          {
               ScRead();
               Order();
               ScWrite();
               score = 1;
          }
          for ( j = 1; j < 11; j++ )
          {
              sprintf(HGSC,"  :  %d", highscore[j]);
              sprintf(ORDER,"%d", j);
              renderBitmapString(-0.4f, 0.4f - 0.1f * j, 0, GLUT_BITMAP_TIMES_ROMAN_24, ORDER);
              if ( highscore[j] )
              {
                   renderBitmapString(-0.3f, 0.4f - 0.1f * j, 0, GLUT_BITMAP_TIMES_ROMAN_24, name[j]);
                   renderBitmapString(0.2f, 0.4f - 0.1f * j, 0, GLUT_BITMAP_TIMES_ROMAN_24, HGSC);
              }
          }
          glColor4f ( 1.0f, 0.25f, 0.25f, 1.0f );
          if ( end == 1 )
          {
               PadClr( 0.5f );
               renderBitmapString( -0.2f, 0.5f, 0, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER" );
          }
     }
     glColor4f ( 1.0f, 1.0f, 1.0f, 1.0f );
     sprintf(SC,"SCORE: %d", pts);
     renderBitmapString(0.6f,0.94f,0,GLUT_BITMAP_TIMES_ROMAN_24,SC);
     renderBitmapString(-0.99f, -0.99f, 0, GLUT_BITMAP_TIMES_ROMAN_10, user);
     renderBitmapString(0.86f, -0.99f, 0, GLUT_BITMAP_TIMES_ROMAN_10, "Version 1.12");
     sprintf(COMBO,"COMBO: %d", colorRow);
     renderBitmapString(0.86f, -0.96f, 0, GLUT_BITMAP_TIMES_ROMAN_10, COMBO);
     renderBitmapString(-0.2f,0.94f,0,GLUT_BITMAP_TIMES_ROMAN_24,"LIVES");
     renderBitmapString(-1.0f,0.98f,0,GLUT_BITMAP_TIMES_ROMAN_10,FPS);
     glutSwapBuffers();
}
void Dcl()
{
     ifstream ferb(ver);
     ferb >> verchk;
     ferb.close();
     if ( verchk > gameversion )
     {
          gameversion = verchk;
          system("start Upd.exe");
          exit(0);
     }
     CspdV = 0.0075,
     PspdV = 0.015;
     score = 0;
     end = 0;
     lives = 5;
     pts = 0;
     Cubes = 1;
     chk = 0;
     colorRow = 0;
     cube[2].posX = 0.0f;
     cube[2].posY = 0.0f;
     cube[2].sizX = CsizX;
     cube[2].sizY = CsizY;
     Pad[2].posX = -0.2f;
     Pad[1].posX = Pad[2].posX - 2.0f;
     Pad[3].posX = Pad[2].posX + 2.0f;
     for ( int i = 1; i <= 3; i++ )
     {
         Pad[i].posY = -0.7f;
         Pad[i].clrR = 0.5f;
         Pad[i].clrG = 0.5f;
         Pad[i].clrB = 0.5f;
     }
     Clr(2);
     GenStars();
     Stop = 0;
     
     StarspdX = ( rand() % 18 - 9 ) / 10000.0f;
     StarspdY = ( rand() % 18 - 9 ) / 10000.0f;
}
int NameChk()
{
    int i, j;
    bool good = true;
    for ( i = 1; i < users; i++ )
    {
        good = true;
        if ( strlen(user) == strlen(name[i]) )
        {
             for ( j = 0; j < strlen(user); j++ )
             {
                 if ( user[j] != name[i][j] )
                 {
                      good = false;
                 }
             }
        }
        else
        {
            good = false;
        }
        if ( good == true )
        {
             userpos = i;
             return 1;
             break;
        }
    }
    return 0;
}
void ScRead()
{
     ifstream fin(path);
     int i = 1;
     while (!fin.eof() )
     {
           fin >> name[i] >> highscore[i];
           i++;
     }
     users = i;
     fin.close();
}
void Order()
{
     int scorechange,
         maxpos,
         max,
         nameletter;
     char namechange[100] = {'\0'};
     for ( i = 1; i < users; i++ )
     {
         if ( NameChk() && chk == 0 )
         {
              if ( pts > highscore[userpos] )
              {
                   highscore[userpos] = pts;
              }
              chk = 1;
         }
         else if ( NameChk() == 0 )
         {
              for ( k = 0; k < strlen(user); k++ )
              {
                  name[users][k] = user[k];
              }
              highscore[users] = pts;
              users++;
         }
     }
     for ( i = 1; i < users; i++ )
     {
         max = highscore[i];
         maxpos = i;
         for ( j = i; j < users; j++ )
         {
             if ( highscore[j] > max )
             {
                  max = highscore[j];
                  maxpos =  j;
             }
         }
         if ( strlen(name[i]) > strlen(name[maxpos]) )
         {
              nameletter = strlen(name[i]);
         }
         else
         {
              nameletter = strlen(name[maxpos]);
         }
         for ( j = 0; j < nameletter; j++ )
         {
             namechange[j] = name[i][j];
         }
         scorechange = highscore[i];
         for ( j = 0; j < nameletter; j++ )
         {
             name[i][j] = name[maxpos][j];
         }
         highscore[i] = highscore[maxpos];
         for ( j = 0; j < nameletter; j++ )
         {
             name[maxpos][j] = namechange[j];
         }
         highscore[maxpos] = scorechange;
     }
}
void ScWrite()
{
     int i;
     ofstream fout(path);
     for ( i = 1; i < users; i++ )
     {
         if ( i == 1 )
         {
              fout << name[i] << "    " << highscore[i];
         }
         else
         {
              fout << endl << name[i] << "    " << highscore[i];
         }
     }
     fout.close();
}
void Clr( int i )
{
     int color = rand() % 7;
     if ( color == 0 )                // Red
     {
          cube[i].clrR =  1.0f;
          cube[i].clrG = 0.25f;
          cube[i].clrB = 0.25f;
     }
     else if ( color == 1 )           // Green
     {
          cube[i].clrR = 0.25f;
          cube[i].clrG = 1.0f;
          cube[i].clrB = 0.25f;
     }
     else if ( color == 2 )           // Blue
     {
          cube[i].clrR = 0.25f;
          cube[i].clrG = 0.25f;
          cube[i].clrB =  1.0f;
     }
     else if ( color == 3 )           // Yellow
     {
          cube[i].clrR =  1.0f;
          cube[i].clrG =  1.0f;
          cube[i].clrB = 0.25f;
     }
     else if ( color == 4 )           // Purple
     {
          cube[i].clrR =  1.0f;
          cube[i].clrG = 0.25f;
          cube[i].clrB =  1.0f;
     }
     else if ( color == 5 )           // Cyan
     {
          cube[i].clrR = 0.25f;
          cube[i].clrG =  1.0f;
          cube[i].clrB =  1.0f;
     }
     else if ( color == 6 )           // Orange
     {
          cube[i].clrR =  1.0f;
          cube[i].clrG =  0.5f;
          cube[i].clrB =  0.25f;
     }
}
long long GetTime()
{
     long long a = glutGet(GLUT_ELAPSED_TIME) - eTime - RTime;
     eTime = glutGet(GLUT_ELAPSED_TIME) - RTime;
     return a;
}
void GenStars()
{
     for ( i = 0; i <= 11; i++ )
     {
         for ( j = 0; j <= 11; j++ )
         {
             GenStarsRow( i, j, 0 );
         }
     }
}
void GenStarsRow( int i, int j, int start )
{
     int size,
         posX,
         posY;
     
     size = rand() % 7 + 4;
     star[i][j].sizX = size / 1000.0f;
     star[i][j].sizY = size / 1000.0f;
     
     if ( start == 0 )
     {
          StSqr[i][j].posX = 0.2f * ( i - 6 );
          StSqr[i][j].posY = 0.2f * ( j - 5 );
     }
     else if ( start == 1 )
     {
         StSqr[i][j].posX = 0.2f * ( i - 6 );
         StSqr[i][j].posY = StSqr[1][j].posY ;
     }
     else if ( start == 2 )
     {
         StSqr[i][j].posX = StSqr[i][1].posX;
         StSqr[i][j].posY = 0.2f * ( j - 5 );
     }
     posX = rand() % ( 200 - size ) + size;
     posY = rand() % ( 200 - size ) + size;
     star[i][j].posX = StSqr[i][j].posX + ( posX / 1000.0f );
     star[i][j].posY = StSqr[i][j].posY - ( posY / 1000.0f );
}
void Draw( float X, float sX, float Y, float sY, float C1r, float C1g, float C1b, float C1a, float C2r, float C2g, float C2b, float C2a, float C3r, float C3g, float C3b, float C3a, float C4r, float C4g, float C4b, float C4a )
{
     glBegin(GL_QUADS);          // DRAW CUBE
     glColor4f ( C1r, C1g, C1b, C1a );
     glVertex3f ( X     , Y     , 0.0f );
     glColor4f ( C2r, C2g, C2b, C2a );
     glVertex3f ( X + sX, Y     , 0.0f );
     glColor4f ( C3r, C3g, C3b, C3a );
     glVertex3f ( X + sX, Y - sY, 0.0f );
     glColor4f ( C4r, C4g, C4b, C4a );
     glVertex3f ( X     , Y - sY, 0.0f );
     glEnd();
}
void keys ( int key, int x, int y )
{
     switch (key)
     {
          case GLUT_KEY_LEFT:
          {
               MLeft = 1;
               break;
          }
          case GLUT_KEY_RIGHT:
          {
               MRight = 1;
               break;
          }
     }
}
void keysUp ( int key, int x, int y )
{
     if ( bot == false )
     {
          switch (key)
          {
               
               case GLUT_KEY_LEFT:
               {
                    MLeft = 0;
                    break;
               }
               case GLUT_KEY_RIGHT:
               {
                    MRight = 0;
                    break;
               }
          }
     }
}
void Nkeys(unsigned char key, int x, int y)
{
     if ( key == 32 )
     {
          if ( end == 0  )
          {
               end = 2;
          }
          else if ( end == 2 )
          {
               end = 0;
          }
     }
     if ( key == 27 )
     {
          exit(0);
     }
     else if ( key == 114 )
     {
          Dcl();
          RTime = glutGet(GLUT_ELAPSED_TIME);
     }
     else if ( key == 98 )
     {
          if ( bot == false )
          {
               bool good = false;
               bot = true;
               char bot[100] = { 'B', 'o', 't' };
               for ( i = 0; i < 100; i++ )
               {
                   user[i] = bot[i];
               }
          }
          else if ( bot == true )
          {
               bot = false;
               ifstream ferb2("NUME.txt");
               ferb2 >> user;
               ferb2.close();
               Dcl();
          }
     }
}
float vodka( float i )
{
    if ( i < 0 )
    {
         return -i;
    }
    return i;
}
void PadClr( float color )
{
     for ( int i = 1; i <= 3; i++ )
     {
         Pad[i].clrR = 0.5f;
         Pad[i].clrG = 0.5f;
         Pad[i].clrB = 0.5f;
     }
}
