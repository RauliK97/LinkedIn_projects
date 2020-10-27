#include <windows.h>
#include <time.h>
int i, j, k,          // variabile pt for-uri
    r,                // variabila random pt calculator
    o,                // contor pt calculator
    x, y,             // delimitarea patratelor de pe tabla
    l1, l2, l3, l4;   // pozitile liniei de castigare
int xImg, yImg;       // coordonatele afisarii imaginii
int s[9];             // vectorul pentru fiecare patrat: 0 = gol, 1 = X, 2 = O
int l[9];             // vectorul pentru linia de castig: 1,2,3 = verticala, 4,5,6 = orizontala, 7,8 = diagonala
int win = 0;          // contorul pentru sfarsit: 0 = continuare, 1 = X castiga, 2 = O castiga, 3 = remiza
bool X = true;        // contorul pentru X sau O: true = X, false = O
char diff = 'E';      // contorul pentru dificultate: E = usor, M = mediu, H = greu
int computer = 0;     // contorul pt calculator: 0 = 2 jucatori, 1 = jucator contra calculator
int xPos, yPos;       // coordonatele click-ului si aoe calculatorului ( coordonatele pentru patrate )
int Theme = 1;        // variabila pt teme: 1 = foc, 2 = apa, 3 = fum, 4 = fulger, 5 = gheata

void Dcl()            // functia pentru resetarea jocului
{
     X = true;        // X incepe
     for ( i = 1; i <= 9; i++ )
     {
         s[i] = 0;    // stergerea X-urilor si O-urilor de pe tabla
         l[i] = 0;    // resetarea liniei castigului
     }
}
void WinChk()         // functia de verificare pt sfarsitul jocului
{
     for ( i = 1; i <= 9; i++ )   //   REMIZA
     {
         if ( s[i] == 0 )   // verificarea fiecarui patrat
         {
              break;        // daca se gaseste un patrat gol se continua
         }
         if ( i == 9 )      // daca nu se gaseste niciun patrat gol
         {
              win = 3;      // jocul se termina cu remiza
         }
     }
     for ( i = 1; i <= 9; i += 3 )          //    ORIZONTALA
     {
         if ( s[i] == s[i+1] && s[i+1] == s[i+2] && s[i+2] == 1 )       // verificarea liniilor pt 3 X-uri pe orizontala
         {
              l[(i+2)/3] = 1;       // se deseneaza linia pt castig
              win = 1;              // X-ul castiga
              break;                // opreste verificarea in continuare
         }
         else if ( s[i] == s[i+1] && s[i+1] == s[i+2] && s[i+2] == 2 )  // verificarea liniilor pt 3 O-uri pe orizontala
         {
              l[(i+2)/3] = 1;       // se deseneaza linia pt castig
              win = 2;              // O-ul castiga
              break;                // opreste verificarea in continuare
         }
     }
     for ( i = 1; i <= 3; i++ )             //   VERTICALA
     {
         if ( s[i] == s[i+3] && s[i+3] == s[i+6] && s[i+6] == 1 )       // verificarea liniilor pt 3 X-uri pe verticala
         {
              l[i+3] = 1;           // se deseneaza linia pt castig
              win = 1;              // X-ul castiga
              break;                // opreste verificarea in continuare
         }
         else if ( s[i] == s[i+3] && s[i+3] == s[i+6] && s[i+6] == 2 )  // verificarea liniilor pt 3 O-uri pe verticala
         {
              l[i+3] = 2;           // se deseneaza linia pt castig
              win = 2;              // O-ul castiga
              break;                // opreste verificarea in continuare
         }
     }
     for ( i = 1; i <= 3; i += 2 )            // DIAGONALA
     {
         if ( s[i] == s[5] && s[5] == s[10-i] && s[i] == 1 )            // verificarea liniilor pt 3 X-uri pe diagonala
         {
              l[(i+13)/2] = 1;      // se deseneaza linia pt castig
              win = 1;              // X-ul castiga
         }
         if ( s[i] == s[5] && s[5] == s[10-i] && s[i] == 2 )            // verificarea liniilor pt 3 X-uri pe diagonala
         {
              l[(i+13)/2] = 2;      // se deseneaza linia pt castig
              win = 2;              // X-ul castiga
         }
     }
}
void Comp( int &xPos, int &yPos, HWND hwnd )         // calculatorul ( functia de calculare a O-urilor )
{
     if ( diff == 'E' )          // verificarea dificultatii: E = usor
     {
          srand (time(NULL));    // resetarea timpului pt functia random
          r = rand() % 9 + 1;    // generarea unui numar la intamplare
          while ( s[r] != 0 )    // daca patratul care corespunde numarului generat este ocupat
          {
                r = rand() % 9 + 1;  // se genereaza alt numar la intamplare pana cand patratul va fi liber
          }
          xPos = 100 * ( ( r - 1 ) % 3 ) + 50;       // calcularea coordonatei X pentru punerea O-ului
          yPos = 100 * ( ( r - 1 ) / 3 ) + 50;       // calcularea coordonatei Y pentru punerea O-ului
     }
     else if ( diff == 'M' )     // verificarea dificultatii: M = mediu
     {
          srand (time(NULL));    // resetarea timpului pt functia random
          r = rand() % 9 + 1;    // generarea unui numar la intamplare
          while ( s[r] != 0 )    // daca patratul care corespunde numarului generat este ocupat
          {
                r = rand() % 9 + 1;  // se genereaza alt numar la intamplare pana cand patratul va fi liber
          }
          xPos = 100 * ( ( r - 1 ) % 3 ) + 50;   // calcularea coordonatei X pentru punerea O-ului
          yPos = 100 * ( ( r - 1 ) / 3 ) + 50;   // calcularea coordonatei Y pentru punerea O-ului
          for ( i = 1; i <= 2; i++ )      // verificarea pt X-uri sau O-uri: 1 = X, 2 = O
          {
              for ( j = 1; j <= 9; j += 3 )          //    ORIZONTALA
              {
                  if ( ( ( s[j] == s[j+1] ) && ( s[j] == i ) ) || ( ( s[j+1] == s[j+2] ) && ( s[j+1] == i ) ) || ( ( s[j] == s[j+2] ) && ( s[j] == i ) ) )   // verificarea daca 2 X-uri sau O-uri
                  {                                                                                                                                          // se afla pe orizontala
                       for ( k = j; k <= j + 2; k++ )     // verificarea fiecarui patrat
                       {
                           if ( s[k] == 0 )        // cautarea patratului liber
                           {
                                xPos = 100 * ( ( k - 1 ) % 3 ) + 50;     // calcularea coordonatei X pentru punerea O-ului
                                yPos = 100 * ( ( k - 1 ) / 3 ) + 50;     // calcularea coordonatei Y pentru punerea O-ului
                           }
                       }
                  }
              }
              for ( j = 1; j <= 3; j++ )             //   VERTICALA
              {
                  if ( ( ( s[j] == s[j+3] ) && ( s[j] == i ) ) || ( ( s[j+3] == s[j+6] ) && ( s[j+3] == i ) ) || ( ( s[j] == s[j+6] ) && ( s[j] == i ) ) )   // verificarea daca 2 X-uri sau O-uri
                  {                                                                                                                                          // se afla pe verticala
                       for ( k = j; k <= j + 6; k += 3 )     // verificarea fiecarui patrat
                       {
                           if ( s[k] == 0 )        // cautarea patratului liber
                           {
                                xPos = 100 * ( ( k - 1 ) % 3 ) + 50;     // calcularea coordonatei X pentru punerea O-ului
                                yPos = 100 * ( ( k - 1 ) / 3 ) + 50;     // calcularea coordonatei Y pentru punerea O-ului
                           }
                       }
                  }
              }
              for ( j = 1; j <= 3; j += 2 )            // DIAGONALA
              {
                  if ( ( ( s[j] == s[5] ) && ( s[j] == i ) ) || ( ( s[5] == s[10-j] ) && ( s[5] == i ) ) || ( ( s[j] == s[10-j] ) && ( s[j] == i ) ) )   // verificarea daca 2 X-uri sau O-uri
                  {                                                                                                                                      // se afla pe diagonala
                       for ( k = j; k <= 10 - j; k += -j + 5 )          // verificarea fiecarui patrat
                       {
                           if ( s[k] == 0 )        // cautarea patratului liber
                           {
                                xPos = 100 * ( ( k - 1 ) % 3 ) + 50;     // calcularea coordonatei X pentru punerea O-ului
                                yPos = 100 * ( ( k - 1 ) / 3 ) + 50;     // calcularea coordonatei Y pentru punerea O-ului
                           }
                       }
                  }
              }
          }
     }
     else if ( diff = 'H' )     // verificarea dificultatii: H = greu
     {
         if ( s[5] == 0 )       // verificarea daca patratul din mijloc este ocupat
         {
              xPos = 150;       // daca patratul din mijloc este liber
              yPos = 150;       // atunci se va pune O in el
         }
         else                   // daca patratul din mijloc nu este liber...
         {
              srand (time(NULL));    // resetarea timpului pt functia random
              do
              {
                    r = rand() % 4 + 1;  // se genereaza un numar la intamplare pana cand patratul va fi liber
                    if ( r <= 2 )        // se verifica in ce colt se va pune O-ul
                    {
                         r = r * 2 - 1;  // se calculeaza colturile in functie de numarul generat
                    }
                    else
                    {
                         r = r * 2 + 1;  // se calculeaza colturile in functie de numarul generat
                    }
                    if ( s[1] != 0 && s[3] != 0 && s[7] != 0 && s[9] != 0 )  // daca toate colturile sunt pline
                    {
                         r = rand() % 9 + 1;  // se genereaza un numar la intamplare pana cand patratul va fi liber
                    }
              } while ( s[r] != 0 );    // daca patratul care corespunde numarului generat este ocupat
              xPos = 100 * ( ( r - 1 ) % 3 ) + 50;   // calcularea coordonatei X pentru punerea O-ului
              yPos = 100 * ( ( r - 1 ) / 3 ) + 50;   // calcularea coordonatei Y pentru punerea O-ului
         }
//##############################################################################################################################################
// calcularea O-urilor astfel incat castigul sa fie imposibil
         if ( s[1] == s[9] && s[1] == 1 )
         {
              xPos = 150;
              yPos = 250;
         }
         else if ( s[3] == s[7] && s[3] == 1 )
         {
              xPos = 150;
              yPos = 250;
         }
         else if ( ( ( s[1] == s[6] && s[1] != 0 ) || ( s[9] == s[2] && s[9] != 0 ) ) && s[3] == 0 )
         {
              xPos = 250;
              yPos = 50;
         }
         else if ( ( ( s[1] == s[8] && s[1] != 0 ) || ( s[9] == s[4] && s[9] != 0 ) ) && s[7] == 0 )
         {
              xPos = 50;
              yPos = 250;
         }
         else if ( ( ( s[3] == s[4] && s[3] != 0 ) || ( s[7] == s[2] && s[7] != 0 ) ) && s[1] == 0 )
         {
              xPos = 50;
              yPos = 50;
         }
         else if ( ( ( s[3] == s[8] && s[3] != 0 ) || ( s[7] == s[6] && s[7] != 0 ) ) && s[9] == 0 )
         {
              xPos = 250;
              yPos = 250;
         }
         else if ( s[2] == s[4] && s[2] != 0 && s[1] == 0 )
         {
              xPos = 50;
              yPos = 50;
         }
         else if ( s[2] == s[6] && s[2] != 0 && s[3] == 0 )
         {
              xPos = 250;
              yPos = 50;
         }
         else if ( s[8] == s[4] && s[8] != 0 && s[7] == 0 )
         {
              xPos = 50;
              yPos = 250;
         }
         else if ( s[8] == s[6] && s[8] != 0 && s[9] == 0 )
         {
              xPos = 250;
              yPos = 250;
         }
//##############################################################################################################################################
         for ( i = 1; i <= 2; i++ )      // verificarea pt X-uri sau O-uri: 1 = X, 2 = O
          {
              for ( j = 1; j <= 9; j += 3 )          //    ORIZONTALA
              {
                  if ( ( ( s[j] == s[j+1] ) && ( s[j] == i ) ) || ( ( s[j+1] == s[j+2] ) && ( s[j+1] == i ) ) || ( ( s[j] == s[j+2] ) && ( s[j] == i ) ) )   // verificarea daca 2 X-uri sau O-uri
                  {                                                                                                                                          // se afla pe orizontala
                       for ( k = j; k <= j + 2; k++ )     // verificarea fiecarui patrat
                       {
                           if ( s[k] == 0 )        // cautarea patratului liber
                           {
                                xPos = 100 * ( ( k - 1 ) % 3 ) + 50;     // calcularea coordonatei X pentru punerea O-ului
                                yPos = 100 * ( ( k - 1 ) / 3 ) + 50;     // calcularea coordonatei Y pentru punerea O-ului
                           }
                       }
                  }
              }
              for ( j = 1; j <= 3; j++ )             //   VERTICALA
              {
                  if ( ( ( s[j] == s[j+3] ) && ( s[j] == i ) ) || ( ( s[j+3] == s[j+6] ) && ( s[j+3] == i ) ) || ( ( s[j] == s[j+6] ) && ( s[j] == i ) ) )   // verificarea daca 2 X-uri sau O-uri
                  {                                                                                                                                          // se afla pe verticala
                       for ( k = j; k <= j + 6; k += 3 )     // verificarea fiecarui patrat
                       {
                           if ( s[k] == 0 )        // cautarea patratului liber
                           {
                                xPos = 100 * ( ( k - 1 ) % 3 ) + 50;     // calcularea coordonatei X pentru punerea O-ului
                                yPos = 100 * ( ( k - 1 ) / 3 ) + 50;     // calcularea coordonatei Y pentru punerea O-ului
                           }
                       }
                  }
              }
              for ( j = 1; j <= 3; j += 2 )            // DIAGONALA
              {
                  if ( ( ( s[j] == s[5] ) && ( s[j] == i ) ) || ( ( s[5] == s[10-j] ) && ( s[5] == i ) ) || ( ( s[j] == s[10-j] ) && ( s[j] == i ) ) )   // verificarea daca 2 X-uri sau O-uri
                  {                                                                                                                                      // se afla pe diagonala
                       for ( k = j; k <= 10 - j; k += -j + 5 )          // verificarea fiecarui patrat
                       {
                           if ( s[k] == 0 )        // cautarea patratului liber
                           {
                                xPos = 100 * ( ( k - 1 ) % 3 ) + 50;     // calcularea coordonatei X pentru punerea O-ului
                                yPos = 100 * ( ( k - 1 ) / 3 ) + 50;     // calcularea coordonatei Y pentru punerea O-ului
                           }
                       }
                  }
              }
          }
     }
}
//###################################################################################################################################################
// afisarea si pregatirea ferestrei

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
char szClassName[ ] = "WindowsApp";

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(0,0,0));

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "Tic Tac Toe",       /* Title Text */
           WS_SYSMENU,          /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           305,                 /* The programs width */
           345,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
HBITMAP bitmap[100];
HDC hdcSource[100];

/*  This function is called by the Windows function DispatchMessage()  */
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{

//###################################################################################################################################################

#define ID_NG 10     // definirea butonului de joc nou
#define ID_EXIT 11   // definirea butonului de iesire
#define ID_PvP 12    // definirea butonului de 2 jucatori
#define ID_PvC 13    // definirea butonului de jucator contra calculator
#define ID_EASY 14   // definirea butonului de dificultate usoara
#define ID_MED 15    // definirea butonului de dificultate medie
#define ID_HARD 16   // definirea butonului de dificultate grea
#define ID_ThW 17
#define ID_ThF 18
#define ID_ThS 19
#define ID_ThL 20
#define ID_ThI 21

WNDCLASSEX wincl;
HDC hdc;             // declararea functiei de creare
PAINTSTRUCT ps ;     // declararea functiei de desenare
HDC hdcMem, hMemDC;  // declararea functiilor de afisare a imaginilor
static HBITMAP bmpSource = NULL;    // locatia salvarii imaginilor in memorie
static HDC hdcSource = NULL;        // locatia salvarii butoanelor si meniului in memorie
    switch (message)                  /* handle the messages */
    {
        case WM_LBUTTONDOWN:        // verificarea daca click-ul stang este apasat
        {
             for ( o = computer; o >= 0; o-- )    // numarul de X-uri si O-uri puse la apasarea unui click
             {                                    // daca se joaca 2 jucatori se va afisa un X sau un O
                                                  // daca se joaca impotriva calculatorului se va afisa si un X si un O ( O-ul find al calculatorului )
                 if ( win == 0 )                  // verificarea daca jocul nu sa sfarsit
                 {
                      xPos = LOWORD(lParam);                  // gasirea coordonatei X a click-ului
                      yPos = HIWORD(lParam);                  // gasirea coordonatei Y a click-ului
                      if ( computer == 1 && X == false )      // daca se joaca contra calculatorului, el isi va calcula coordonatele X si Y
                      {
                           Comp( xPos, yPos, hwnd );          // apelarea functiei de calculare a coordonatelor O-urilor
                      }
                      for ( i = 1; i <= 9; i++ )              // parcurgerea fiecarui patrat
                      {
                          x = 100 * ( ( i - 1 ) % 3 );        // delimitarea patratelor in latime
                          y = 100 * ( ( i - 1 ) / 3 );        // delimitarea patratelor in lungime
                          if ( ( xPos >= x && xPos <= x + 100 ) && ( yPos >= y && yPos <= y + 100 ) )  // gasirea patratului in care trebuie afisat X-ul sau O-ul
                          {
                               if ( s[i] == 0 )    // verificarea daca patratul este gol
                               {
                                    if ( X == true )       // verificarea daca va fi pus un X
                                    {
                                         s[i] = 1;         // se pune un X
                                         X = false;        // urmeaza sa fie pus un O
                                    }
                                    else if ( X == false ) // verificarea daca va fi pus un O
                                    {
                                         s[i] = 2;         // se pune un O
                                         X = true;         // urmeaza sa fie pus un X
                                    }
                                    WinChk();              // apelarea functiei de verificare pt sfarsitul jocului
                                    InvalidateRect(hwnd, 0, TRUE);  // redesenarea jocului cu X-ul sau O-ul pus
                               }
                               else        // daca patratul apasat nu este gol
                               {
                                         MessageBox(hwnd, "Invalid Move!", "STOP", MB_ICONERROR );    // se va afisa o eroare
                                         o--;   // evitarea ca mesajul sa apara de 2 ori
                               }
                               break;      // oprirea parcurgerii patratelor
                          }
                      }
                 }
                 if ( win == 1  )        // daca primul jucator a castigat
                 {
                      int MsBox = MessageBox(hwnd, "Player 1 Won!\nPlay again ?", "Winner", MB_ICONINFORMATION | MB_YESNO ); // se afiseaza mesajul de sfarsit
                      if ( MsBox == IDYES )    // verificarea daca butonul de joc nou a fost apasat
                      {
                           Dcl();                             // apelarea funtiei pentru resetarea jocului
                           InvalidateRect(hwnd, 0, TRUE);     // redesennarea tablei goale
                      }
                      break;            // evitarea continuarii jocului
                 }
                 else if ( win == 2  )
                 {
                      int MsBox = MessageBox(hwnd, "Player 2 Won!\nPlay again ?", "Winner", MB_ICONINFORMATION | MB_YESNO ); // se afiseaza mesajul de sfarsit
                      if ( MsBox == IDYES )    // verificarea daca butonul de joc nou a fost apasat
                      {
                           Dcl();                             // apelarea funtiei pentru resetarea jocului
                           InvalidateRect(hwnd, 0, TRUE);     // redesennarea tablei goale
                      }
                      break;            // evitarea continuarii jocului
                 }
                 else if ( win == 3  )
                 {
                      int MsBox = MessageBox(hwnd, "It's a draw!\nPlay again ?", "Draw", MB_ICONINFORMATION | MB_YESNO ); // se afiseaza mesajul de sfarsit
                      if ( MsBox == IDYES )    // verificarea daca butonul de joc nou a fost apasat
                      {
                           Dcl();                             // apelarea funtiei pentru resetarea jocului
                           InvalidateRect(hwnd, 0, TRUE);     // redesennarea tablei goale
                      }
                      break;            // evitarea continuarii jocului
                 }
             }

             break;        // oprirea functiei
        }
        case WM_CREATE:    // pregatirea interfatei
        {
             Dcl();        // resetarea jocului la deschidere
             bitmap[1]=(HBITMAP)LoadImage(NULL,TEXT("Photos/X F.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  //  )
             HDC hdcSource1 = CreateCompatibleDC(GetDC(0));                                               //  }=> incarcarea imaginei cu X-ul
             SelectObject(hdcSource1, bmpSource);                                                         //  )_____
             bitmap[2]=(HBITMAP)LoadImage(NULL,TEXT("Photos/O F.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  //  )
             HDC hdcSource2 = CreateCompatibleDC(GetDC(0));                                               //  }=> incarcarea imaginei cu O-ul
             SelectObject(hdcSource2, bmpSource);                                                         //  )_____
             bitmap[3]=(HBITMAP)LoadImage(NULL,TEXT("Photos/X W.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  //  )
             HDC hdcSource3 = CreateCompatibleDC(GetDC(0));                                               //  }=> incarcarea imaginei cu X-ul
             SelectObject(hdcSource3, bmpSource);                                                         //  )_____
             bitmap[4]=(HBITMAP)LoadImage(NULL,TEXT("Photos/O W.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  //  )
             HDC hdcSource4 = CreateCompatibleDC(GetDC(0));                                               //  }=> incarcarea imaginei cu O-ul
             SelectObject(hdcSource4, bmpSource);                                                         //  )
             bitmap[5]=(HBITMAP)LoadImage(NULL,TEXT("Photos/X S.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  //  )
             HDC hdcSource5 = CreateCompatibleDC(GetDC(0));                                               //  }=> incarcarea imaginei cu X-ul
             SelectObject(hdcSource5, bmpSource);                                                         //  )_____
             bitmap[6]=(HBITMAP)LoadImage(NULL,TEXT("Photos/O S.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  //  )
             HDC hdcSource6 = CreateCompatibleDC(GetDC(0));                                               //  }=> incarcarea imaginei cu O-ul
             SelectObject(hdcSource6, bmpSource);                                                         //  )
             bitmap[7]=(HBITMAP)LoadImage(NULL,TEXT("Photos/X L.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  //  )
             HDC hdcSource7 = CreateCompatibleDC(GetDC(0));                                               //  }=> incarcarea imaginei cu X-ul
             SelectObject(hdcSource7, bmpSource);                                                         //  )_____
             bitmap[8]=(HBITMAP)LoadImage(NULL,TEXT("Photos/O L.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  //  )
             HDC hdcSource8 = CreateCompatibleDC(GetDC(0));                                               //  }=> incarcarea imaginei cu O-ul
             SelectObject(hdcSource8, bmpSource);                                                         //  )
             bitmap[9]=(HBITMAP)LoadImage(NULL,TEXT("Photos/X I.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  //  )
             HDC hdcSource9 = CreateCompatibleDC(GetDC(0));                                               //  }=> incarcarea imaginei cu X-ul
             SelectObject(hdcSource9, bmpSource);                                                         //  )_____
             bitmap[10]=(HBITMAP)LoadImage(NULL,TEXT("Photos/O I.bmp"),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);  //  )
             HDC hdcSource10 = CreateCompatibleDC(GetDC(0));                                               //  }=> incarcarea imaginei cu O-ul
             SelectObject(hdcSource10, bmpSource);                                                         //  )
             
             HMENU hMenuBar = CreateMenu();    // declarea barii de meniu
             HMENU hFile = CreateMenu();       // declarea menuilui File
             HMENU hOptions = CreateMenu();    // declarea meniului Options
             HMENU hPvC = CreateMenu();        // declarea menuilui de jucator contra calculator
             HMENU hThemes = CreateMenu();     // declarea menuilui de teme

             AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hFile, "File" );                // crearea meniului File
             AppendMenu(hFile, MF_STRING, ID_NG, "New Game" );                        // crearea butonului de joc now
             AppendMenu(hFile, MF_STRING, ID_EXIT, "Exit" );                          // crearea butonului de iesire

             AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hOptions, "Options" );          // crearea meniului Options
             AppendMenu(hOptions, MF_STRING, ID_PvP, "2 players" );                   // crearea butonului de 2 jucatori
             AppendMenu(hOptions, MF_POPUP, (UINT_PTR)hPvC, "Player vs Computer" );   // crearea submeniului de jucator contra calculator
             AppendMenu(hPvC, MF_STRING, ID_EASY, "Easy" );                           // crearea butonului de dificultate usoara
             AppendMenu(hPvC, MF_STRING, ID_MED, "Medium" );                          // crearea butonului de dificultate medie
             AppendMenu(hPvC, MF_STRING, ID_HARD, "Hard" );                           // crearea butonului de dificultate grea
             
             AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hThemes, "Themes" );            // crearea meniului Teme
             AppendMenu(hThemes, MF_STRING, ID_ThF, "Fire" );                         // crearea butonului de Tema foc
             AppendMenu(hThemes, MF_STRING, ID_ThS, "Smoke" );                        // crearea butonului de Tema fum
             AppendMenu(hThemes, MF_STRING, ID_ThW, "Water" );                        // crearea butonului de tema apa
             AppendMenu(hThemes, MF_STRING, ID_ThL, "Lightning" );                    // crearea butonului de tema fulger
             AppendMenu(hThemes, MF_STRING, ID_ThI, "Ice" );                          // crearea butonului de tema fulger

             SetMenu(hwnd, hMenuBar);                                                 // afisarea meniului

             break;       // oprirea functiei
        }
        case WM_COMMAND:                           // functia de verificare a butoanelor
        {
             if ( LOWORD(wParam) == ID_NG )        // verificarea daca butonul de joc nou a fost apasat
             {
                  Dcl();                           // apelarea functiei de resetare a jocului
                  InvalidateRect(hwnd, 0, TRUE);   // redesenarea tablei goale
             }
             else if ( LOWORD(wParam) == ID_EXIT ) // verificarea daca butonul de iesire a fost apasat
             {
                  DestroyWindow(hwnd);             // inchiderea jocului
             }
             else if ( LOWORD(wParam) == ID_PvP )  // verificarea daca butonul de 2 jucatori a fost apasat
             {
                  computer = 0;                    // dezactivarea calculatorului
                  Dcl();                           // apelarea functiei de resetare a jocului
                  InvalidateRect(hwnd, 0, TRUE);   // redesenarea tablei goale
             }
             else if ( LOWORD(wParam) == ID_EASY ) // verificarea daca butonul de dificultate usoara a fost apasat
             {
                  computer = 1;                    // activarea calculatorului
                  diff = 'E';                      // setarea dificultatii usoare
                  Dcl();                           // apelarea functiei de resetare a jocului
                  InvalidateRect(hwnd, 0, TRUE);   // redesenarea tablei goale
             }
             else if ( LOWORD(wParam) == ID_MED )  // verificarea daca butonul de dificultate medie a fost apasat
             {
                  computer = 1;                    // activarea calculatorului
                  diff = 'M';                      // setarea dificultatii
                  Dcl();                           // apelarea functiei de resetare a jocului
                  InvalidateRect(hwnd, 0, TRUE);   // redesenarea tablei goale
             }
             else if ( LOWORD(wParam) == ID_HARD ) // verificarea daca butonul de dificultate medie a fost apasat
             {
                  computer = 1;                    // activarea calculatorului
                  diff = 'H';                      // setarea dificultatii
                  Dcl();                           // apelarea functiei de resetare a jocului
                  InvalidateRect(hwnd, 0, TRUE);   // redesenarea tablei goale
             }
             else if ( LOWORD(wParam) == ID_ThF )  // verificarea daca butonul de dificultate tema normala a fost apasat
             {
                  Theme = 1;                       // activarea calculatorului
                  InvalidateRect(hwnd, 0, TRUE);   // redesenarea tablei goale
             }
             else if ( LOWORD(wParam) == ID_ThW )  // verificarea daca butonul de dificultate tema normala a fost apasat
             {
                  Theme = 2;                       // activarea calculatorului
                  InvalidateRect(hwnd, 0, TRUE);   // redesenarea tablei goale
             }
             else if ( LOWORD(wParam) == ID_ThS )  // verificarea daca butonul de dificultate tema normala a fost apasat
             {
                  Theme = 3;                       // activarea calculatorului
                  InvalidateRect(hwnd, 0, TRUE);   // redesenarea tablei goale
             }
             else if ( LOWORD(wParam) == ID_ThL )  // verificarea daca butonul de dificultate tema normala a fost apasat
             {
                  Theme = 4;                       // activarea calculatorului
                  InvalidateRect(hwnd, 0, TRUE);   // redesenarea tablei goale
             }
             else if ( LOWORD(wParam) == ID_ThI )  // verificarea daca butonul de dificultate tema normala a fost apasat
             {
                  Theme = 5;                       // activarea calculatorului
                  InvalidateRect(hwnd, 0, TRUE);   // redesenarea tablei goale
             }
             break;                                // oprirea functiei
        }
        case WM_PAINT:        // functia de desenare
        {
             hdc = BeginPaint (hwnd, &ps);                      // inceperea desenarii
             HPEN hPenOld, hLinePen;                            // declararea instrumentelor de desenare
             COLORREF qLineColor;                               // declararea culorii
             qLineColor = RGB ( 255, 255, 255 );                      // setarea culorii
             hLinePen = CreatePen (PS_SOLID, 4, qLineColor );   // setarea liniilor
             hPenOld = (HPEN) SelectObject(hdc, hLinePen);      // selectarea instrumentului de desenare

             for ( i = 1; i <= 2; i++ )                         // desenarea tablii de joc
             {
                 MoveToEx ( hdc, 100*i, 0, NULL );              // } => desenarea liniilor verticale
                 LineTo ( hdc, 100*i, 300 );                    // )
                 MoveToEx ( hdc, 0, 100*i, NULL );              // } => desenarea liniilor orizontale
                 LineTo ( hdc, 300, 100*i );                    // )
             }
             for ( i = 1; i <= 9; i++ )                         // afisarea imaginii ci X-ul sau O-ul
             {
                 for ( j = 1; j <= 5; j++ )
                 {
                     if ( Theme == j )
                     {
                          k = 2 * j - 1;
                     }
                 }
                 if ( s[i] == 1 )                               // verificarea daca va fi afisat un X
                 {
                      xImg = 100 * ( ( i - 1 ) % 3 ) + 2;       // calcularea coordonatei X de afisare a X-ului
                      yImg = 100 * ( ( i - 1 ) / 3 ) + 2;       // calcularea coordonatei Y de afisare a X-ului
                      HDC hMemDC1=CreateCompatibleDC(hdc);                           // )
                      SelectObject(hMemDC1,bitmap[k]);                                 // )
                      BitBlt( hdc, xImg, yImg, 96, 96, hMemDC1, 0, 0, SRCCOPY );     // } => afisarea imaginii cu X-ul
                      DeleteDC(hMemDC1);                                             // )
                 }
                 else if ( s[i] == 2 )                          // verificarea daca va fi afisat un X
                 {
                      xImg = 100 * ( ( i - 1 ) % 3 ) + 2;       // calcularea coordonatei X de afisare a O-ului
                      yImg = 100 * ( ( i - 1 ) / 3 ) + 2;       // calcularea coordonatei Y de afisare a O-ului
                      HDC hMemDC2=CreateCompatibleDC(hdc);                           // )
                      SelectObject(hMemDC2,bitmap[k+1]);                                 // )
                      BitBlt( hdc, xImg, yImg, 96, 96, hMemDC2, 0, 0, SRCCOPY );     // } => afisarea imaginii cu O-ul
                      DeleteDC(hMemDC2);                                             // )
                 }
             }
             qLineColor = RGB ( 0, 150, 0 );                    // setarea culorii pt linia de castig
             hLinePen = CreatePen (PS_SOLID, 7, qLineColor );   // setarea liniei de castig
             hPenOld = (HPEN) SelectObject(hdc, hLinePen);      // selectarea instrumentului de desenare
             for ( i = 1; i <= 9; i++ )                         // parcurgerea patratelor
             {
                 if ( l[i] != 0 )                               // verificarea daca patratul nu este gol
                 {
                      l1 = 10;                                  // )
                      l2 = 100 * ( ( i - 1 ) % 3 ) + 50;        // } => calcularea coordonatelor de
                      l3 = 290;                                 // )    desenare a liniei de castig
                      l4 = l2;                                  // )
                      if ( i <= 3 )                             // verificarea daca linia este orizontala
                      {
                           MoveToEx( hdc, l1, l2, NULL);        // } => desenarea liniei
                           LineTo( hdc, l3, l4 );               // )
                      }
                      else if ( i >= 4 && i <= 6 )              // verificarea daca linia este verticala
                      {
                           MoveToEx( hdc, l2, l1, NULL);        // } => desenarea liniei
                           LineTo( hdc, l4, l3 );               // )
                      }
                      else if ( i == 7 )                        // verificarea daca se afla pe diagonala principala
                      {
                           MoveToEx( hdc, l1, l1, NULL);        // } => desenarea liniei
                           LineTo( hdc, l3, l3 );               // )
                      }
                      else if ( i == 8 )                        // verificarea daca se afla pe diagonala principala
                      {
                           MoveToEx( hdc, l3, l1, NULL);        // } => desenarea liniei
                           LineTo( hdc, l1, l3 );               // )
                      }
                 }
             }

             SelectObject(hdc, hPenOld);                        // selectarea instrumentului de scris
             DeleteObject(hLinePen);                            // eliminarea instrumentelor de desenare

             EndPaint (hwnd, &ps) ;                             // terminarea desenarii
             break;                                             // oprirea functiei
        }
        case WM_DESTROY:              // functia de inchidere
            PostQuitMessage (0);      /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }
    return 0;
}
