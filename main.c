#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
int Rand_O();
int Control();
int Winner();
int Spaces[9]={0,0,0,0,0,0,0,0,0};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main ()
{
    srand(time(NULL));
    const char Game[]  = "XOX Game";

    WNDCLASS wc = {};

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = GetModuleHandle(0);
    wc.lpszClassName = Game;

    RegisterClass(&wc);


    HWND hwnd = CreateWindowEx(
            0, wc.lpszClassName, "XOX Game", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 235, 305,
            NULL, NULL, wc.hInstance, NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOWNORMAL);



    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {
        case WM_CLOSE:
            if (MessageBox(hwnd, "Kapatmak Istiyor Musun?", "UYARI", MB_YESNO) == IDYES) {
                DestroyWindow(hwnd);
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CREATE:
            CreateWindowEx(0,"Edit","Isaretleme Kismi",
                           WS_CHILD|WS_VISIBLE|WS_DISABLED,
                           3,100,110,10,hwnd,(HMENU)31,
                           GetModuleHandle(0),0);
            CreateWindowEx(0,"Edit","<---",
                           WS_CHILD|WS_VISIBLE|WS_DISABLED,
                           100,140,23,10,hwnd,(HMENU)32,
                           GetModuleHandle(0),0);
            for (int menuNum = 0, i = 0; i < 3; ++i) {
                wchar_t buttonText[2];
                buttonText[0]='X';
                buttonText[1]='\0';
                for (int j = 0; j < 3; ++j) {
                    CreateWindowEx(0, "Edit", "",
                                   WS_CHILD | WS_VISIBLE| ES_AUTOHSCROLL | WS_BORDER | WS_DISABLED,
                                   3 + (j * 30), 5 + (i * 30), 30, 30, hwnd, (HMENU)(9+menuNum),
                                   GetModuleHandle(0), 0);
                    CreateWindowEx(0, "Button", (LPCSTR)buttonText,
                                    WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON,
                                   3 + (j * 30), 120 + (i * 30), 30, 30, hwnd, (HMENU)(0+menuNum),
                                   GetModuleHandle(0), 0);
                    menuNum++;
                }
            }
            break;




        case WM_COMMAND:
            if (HIWORD(wParam) == BN_CLICKED) {
                wchar_t butonMetni[2];
                int butonId = LOWORD(wParam);
                printf("%d",butonId);
                if (Spaces[butonId]!=0){
                    MessageBox(NULL,"Bolme Bos Degil","HATA",MB_ICONERROR);
                    break;
                }
                GetWindowText(GetDlgItem(hwnd, butonId), (LPSTR) butonMetni, 2);
                SendMessage(GetDlgItem(hwnd, butonId + 9), WM_SETTEXT, 0, (LPARAM)butonMetni);
                Spaces[butonId]=2;
                int winner=Winner();
                if (winner){
                    if (winner==2){
                        MessageBox(NULL,"OYUNU KAZANDINIZ","OYUN BITTI",MB_ICONINFORMATION);
                    } else
                        MessageBox(NULL,"OYUNU KAYBETTINIZ","OYUN BITTI",MB_ICONINFORMATION);
                    for (int i = 0; i <= 8; ++i) {
                        SetWindowText(GetDlgItem(hwnd,i+9),"");
                    }
                    for (int i = 0; i <=8 ; ++i) {
                        Spaces[i]=0;
                    }
                    break;
                }
                if(Control()){
                    MessageBox(NULL,"OYUN BERABERE BITTI","OYUN BITTI",MB_ICONINFORMATION);
                    for (int i = 0; i <= 8; ++i) {
                        SetWindowText(GetDlgItem(hwnd,i+9),"");
                    }
                    for (int i = 0; i <=8 ; ++i) {
                        Spaces[i]=0;
                    }
                    break;
                }
                int PC_Rnd=Rand_O();
                SendMessage(GetDlgItem(hwnd,PC_Rnd+9),WM_SETTEXT,0,(LPARAM)"O");
                Spaces[PC_Rnd]=1;
                winner=Winner();
                if (winner){
                    if (winner==2){
                        MessageBox(NULL,"OYUNU KAZANDINIZ","OYUN BITTI",MB_ICONINFORMATION);
                    } else
                        MessageBox(NULL,"OYUNU KAYBETTINIZ","OYUN BITTI",MB_ICONINFORMATION);
                    for (int i = 0; i <= 8; ++i) {
                        SetWindowText(GetDlgItem(hwnd,i+9),"");
                    }
                    for (int i = 0; i <=8 ; ++i) {
                        Spaces[i]=0;
                    }
                    break;
                }

            }
        break;



        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 6));
            EndPaint(hwnd, &ps);
        break;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

}
int Rand_O(){
    int randNum=rand()%9;
    while(Spaces[randNum]!=0){
        randNum=rand()%9;
    }
    return randNum;
}
int Control(){
    int control=0;
    for (int i = 0; i <9 ; ++i) {
        if(Spaces[i]!=0)
            control++;

    }

    if (control==9)
        return 1;
    return 0;
}
int Winner(){
    for (int i = 0; i < 7;i+=3) {
        for (int j = 1; j <=2 ; ++j) {
            if (Spaces[i]==j&&Spaces[i+1]==j&&Spaces[i+2]==j)
                return j;
        }

    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 1; j <=2 ; ++j) {
            if (Spaces[i]==j&&Spaces[i+3]==j&&Spaces[i+6]==j)
                return j;
        }

    }
    for (int i = 1; i <=2 ; ++i) {
        if(Spaces[0]==i&&Spaces[4]==i&&Spaces[8]==i)
            return i;
        if (Spaces[2]==i&&Spaces[4]==i&&Spaces[6]==i)
            return i;
    }
    return 0;
}