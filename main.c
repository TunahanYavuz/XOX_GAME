#include <windows.h>
#include <math.h>
#include <time.h>
#ifndef UNICODE
#define UNICODE
#endif

int Rand_O();
int Control();
int Winner();
void Clean(HWND hwnd);
void PrintWinner(int winnerNum);
int Spaces[9]={0,0,0,0,0,0,0,0,0};
int player=1;
int player2=2;
int PC=2;
int difficulty=0;
int gameMode=0;
int textcontrol=0;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int main ()
{
    SetConsoleOutputCP(CP_UTF8);
    srand(time(NULL));
    const char Game[]  = "XOX Game";

    WNDCLASS wc = {};

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = GetModuleHandle(0);
    wc.lpszClassName = Game;

    RegisterClass(&wc);


    HWND hwnd = CreateWindowEx(
            0, wc.lpszClassName, "XOX Game", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 240, 260,
            NULL, NULL, wc.hInstance, NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, SW_SHOW);



    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HWND ListBox1,ListBox2;
    switch (uMsg) {
        case WM_CLOSE:
            if (MessageBoxW(hwnd, L"Kapatmak İstiyor Musun?", L"UYARI", MB_YESNO) == IDYES) {
                DestroyWindow(hwnd);
            }
            return 0;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_CREATE:
            CreateWindowExW(0, L"STATIC", L"İşaretleme Kısmı",
                            WS_CHILD | WS_VISIBLE,
                            3, 100, 110, 20, hwnd, (HMENU) 31,
                            NULL, 0);
            CreateWindowExW(0, L"STATIC", L"<---",
                            WS_CHILD | WS_VISIBLE,
                            100, 140, 23, 20, hwnd, (HMENU) 32,
                            GetModuleHandle(0), 0);
            for (int menuNum = 0, i = 0; i < 3; ++i) {
                wchar_t buttonText[2];
                buttonText[0] = 'X';
                buttonText[1] = '\0';
                for (int j = 0; j < 3; ++j) {
                    CreateWindowExW(0, L"Edit", L"",
                                    WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED,
                                    3 + (j * 30), 5 + (i * 30), 30, 30, hwnd, (HMENU) (9 + menuNum),
                                    GetModuleHandle(0), 0);
                    CreateWindowExW(0, L"Button", (LPCWSTR) buttonText,
                                    WS_VISIBLE|WS_CHILD|WS_BORDER|BS_PUSHBUTTON,
                                    3 + (j * 30), 120 + (i * 30), 30, 30, hwnd, (HMENU) (0 + menuNum),
                                    GetModuleHandle(0), 0);
                    menuNum++;
                }
            }
            CreateWindowExW(0, L"STATIC", L"Zorluk Ayarı",
                            WS_CHILD | WS_VISIBLE,
                            140, 5, 80, 20, hwnd, (HMENU) 29,
                            GetModuleHandle(0), 0);
            ListBox1 = CreateWindowExW(0, L"LISTBOX", NULL,
                                      WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_NOTIFY,
                                      140, 30, 80, 60, hwnd, (HMENU) 33, GetModuleHandle(0), 0);

            SendMessageW(ListBox1, LB_ADDSTRING, 0, (LPARAM) L"EzCerEz");
            SendMessageW(ListBox1, LB_ADDSTRING, 0, (LPARAM) L"ORTA");
            SendMessageW(ListBox1, LB_ADDSTRING, 0, (LPARAM) L"ZOR");
            CreateWindowExW(0, L"STATIC", L"Oyun Modu",
                            WS_CHILD | WS_VISIBLE,
                            140, 95, 80, 20, hwnd, (HMENU) 29,
                            GetModuleHandle(0), 0);
            ListBox2 = CreateWindowExW(0, L"LISTBOX", NULL,
                                       WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_NOTIFY,
                                       140, 120, 80, 40, hwnd, (HMENU) 33, GetModuleHandle(0), 0);
            SendMessageW(ListBox2, LB_ADDSTRING, 0, (LPARAM) L"Bilgisayar");
            SendMessageW(ListBox2, LB_ADDSTRING, 0, (LPARAM) L"2V2");


            break;


        case WM_COMMAND:
            if (HIWORD(wParam) == BN_CLICKED) {
                wchar_t buttonText[2];
                int buttonId = LOWORD(wParam);
                if (gameMode == 1 && textcontrol == 0) {
                    if (Spaces[buttonId] != 0) {
                        MessageBoxW(NULL, L"Bölme Boş Değil", L"HATA", MB_ICONERROR);
                        break;
                    }
                    GetWindowTextW(GetDlgItem(hwnd, buttonId), (LPWSTR) buttonText, 2);
                    SendMessageW(GetDlgItem(hwnd, buttonId + 9), WM_SETTEXT, 0, (LPARAM) buttonText);
                    Spaces[buttonId] = player;
                    if (Winner()) {
                        Clean(hwnd);
                        break;
                    }

                    if (Control()) {
                        MessageBoxW(NULL, L"Oyun Berabere Bitti", L"Oyun Bitti", MB_ICONINFORMATION);
                        Clean(hwnd);
                        break;
                    }
                    for (int i = 0; i < 9; ++i) {
                        SendMessage(GetDlgItem(hwnd, i), WM_SETTEXT, 0, (LPARAM) "O");
                    }
                    textcontrol = 1;
                } else if (gameMode == 1 && textcontrol == 1) {
                    if (Spaces[buttonId] != 0) {
                        MessageBoxW(NULL, L"Bölme Boş Değil", L"HATA", MB_ICONERROR);
                        break;
                    }
                    GetWindowTextW(GetDlgItem(hwnd, buttonId), (LPWSTR) buttonText, 2);
                    SendMessageW(GetDlgItem(hwnd, buttonId + 9), WM_SETTEXT, 0, (LPARAM) buttonText);
                    Spaces[buttonId] = player2;
                    if (Winner()) {
                        Clean(hwnd);
                        break;
                    }

                    if (Control()) {
                        MessageBoxW(NULL, L"Oyun Berabere Bitti", L"Oyun Bitti", MB_ICONINFORMATION);
                        Clean(hwnd);
                        break;
                    }
                    for (int i = 0; i < 9; ++i) {
                        SendMessage(GetDlgItem(hwnd, i), WM_SETTEXT, 0, (LPARAM) "X");
                    }
                    textcontrol = 0;
                }
                if (gameMode==0){

                    if (Spaces[buttonId] != 0) {
                        MessageBoxW(NULL, L"Bölme Boş Değil", L"HATA", MB_ICONERROR);
                        break;
                    }
                    GetWindowTextW(GetDlgItem(hwnd, buttonId), (LPWSTR) buttonText, 2);
                    SendMessageW(GetDlgItem(hwnd, buttonId + 9), WM_SETTEXT, 0, (LPARAM) buttonText);
                    Spaces[buttonId] = player;
                    if (Winner()) {
                        Clean(hwnd);
                        break;
                    }

                    if (Control()) {
                        MessageBoxW(NULL, L"Oyun Berabere Bitti", L"Oyun Bitti", MB_ICONINFORMATION);
                        Clean(hwnd);
                        break;
                    }

                    int PC_Rnd = Rand_O();
                    SendMessage(GetDlgItem(hwnd, PC_Rnd + 9), WM_SETTEXT, 0, (LPARAM) "O");
                    Spaces[PC_Rnd] = PC;
                    if (Winner()) {
                        Clean(hwnd);
                        break;
                    }

                }
            }
            if (HIWORD(wParam) == LBN_SELCHANGE) {
                int selectedIndex1 = SendMessage(ListBox1, LB_GETCURSEL, 0, 0);
                int selectedIndex2 = SendMessage(ListBox2, LB_GETCURSEL, 0, 0);
                if(selectedIndex1!=LB_ERR&&selectedIndex2==LB_ERR){
                    difficulty = selectedIndex1;

                    wchar_t selectedText[256];
                    SendMessageW(ListBox1, LB_GETTEXT, selectedIndex1, (LPARAM) selectedText);
                    MessageBoxW(hwnd, selectedText, L"Zorluk Modu", MB_OK | MB_ICONINFORMATION);
                    Clean(hwnd);
                }
                else if(selectedIndex1==LB_ERR&&selectedIndex2!=LB_ERR){
                    gameMode = selectedIndex2;
                    wchar_t selectedText[256];
                    SendMessageW(ListBox2,LB_GETTEXT,selectedIndex2,(LPARAM)selectedText);
                    MessageBoxW(hwnd,selectedText,L"Oyun Modu",MB_OK|MB_ICONINFORMATION);
                    if(selectedIndex2==0){
                        for (int i = 0; i < 9; ++i) {
                            SendMessage(GetDlgItem(hwnd, i), WM_SETTEXT, 0, (LPARAM) "X");
                        }
                    }
                    Clean(hwnd);
                }
                break;
            }
            break;


        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 6));
            EndPaint(hwnd, &ps);
            break;
        }
        case WM_KEYDOWN:
            if(GetKeyState(VK_CONTROL)&&(wParam=='r'||wParam=='R')){
                Clean(hwnd);}
        break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
int Rand_O() {
    if (difficulty == 1||difficulty==2){
        if (Spaces[4] == PC && Spaces[8] == PC && Spaces[0] == 0)
            return 0;
        if (Spaces[0] == PC && Spaces[8] == PC && Spaces[4] == 0)
            return 4;
        if (Spaces[0] == PC && Spaces[4] == PC && Spaces[8] == 0)
            return 8;
        if (Spaces[2] == PC && Spaces[4] == PC && Spaces[6] == 0)
            return 6;
        if (Spaces[4] == PC && Spaces[6] == PC && Spaces[2] == 0)
            return 2;
        if (Spaces[2] == PC && Spaces[6] == PC && Spaces[4] == 0)
            return 4;
        for (int i = 0; i < 7; i += 3) {
            if (Spaces[i + 1] == PC && Spaces[i + 2] == PC && Spaces[i] == 0)
                return i;
            if (Spaces[i] == PC && Spaces[i + 2] == PC && Spaces[i + 1] == 0)
                return (i + 1);
            if (Spaces[i] == PC && Spaces[i + 1] == PC && Spaces[i + 2] == 0)
                return (i + 2);
        }
        for (int i = 0; i < 3; ++i) {

            if (Spaces[i + 3] == PC && Spaces[i + 6] == PC && Spaces[i] == 0)
                return i;
            if (Spaces[i] == PC && Spaces[i + 6] == PC && Spaces[i + 3] == 0)
                return i + 3;
            if (Spaces[i] == PC && Spaces[i + 3] == PC && Spaces[i + 6] == 0)
                return i + 6;
        }

        if (Spaces[4] == player && Spaces[8] == player && Spaces[0] == 0)
            return 0;
        if (Spaces[0] == player && Spaces[8] == player && Spaces[4] == 0)
            return 4;
        if (Spaces[0] == player && Spaces[4] == player && Spaces[8] == 0)
            return 8;
        if (Spaces[2] == player && Spaces[4] == player && Spaces[6] == 0)
            return 6;
        if (Spaces[4] == player && Spaces[6] == player && Spaces[2] == 0)
            return 2;
        if (Spaces[2] == player && Spaces[6] == player && Spaces[4] == 0)
            return 4;
        for (int i = 0; i < 7; i += 3) {
            if (Spaces[i + 1] == player && Spaces[i + 2] == player && Spaces[i] == 0)
                return i;
            if (Spaces[i] == player && Spaces[i + 2] == player && Spaces[i + 1] == 0)
                return (i + 1);
            if (Spaces[i] == player && Spaces[i + 1] == player && Spaces[i + 2] == 0)
                return (i + 2);
        }
        for (int i = 0; i < 3; ++i) {

            if (Spaces[i + 3] == player && Spaces[i + 6] == player && Spaces[i] == 0)
                return i;
            if (Spaces[i] == player && Spaces[i + 6] == player && Spaces[i + 3] == 0)
                return i + 3;
            if (Spaces[i] == player && Spaces[i + 3] == player && Spaces[i + 6] == 0)
                return i + 6;
        }
        if(difficulty==2){

            if(Spaces[4]==0)
                return 4;

            if (Spaces[1]==player && Spaces[5]==player&&Spaces[2]==0)
                return 2;
            if (Spaces[1]==player && Spaces[3]==player&&Spaces[0]==0)
                return 0;
            if (Spaces[7]==player && Spaces[5]==player&&Spaces[8]==0)
                return 8;
            if (Spaces[7]==player && Spaces[3]==player&&Spaces[6]==0)
                return 6;
            if (((Spaces[2]==player&&Spaces[3]==player)||(Spaces[6]==player&&Spaces[1]==player))&&Spaces[0]==0)
                return 0;
            if (((Spaces[0]==player&&Spaces[5]==player)||(Spaces[8]==player&&Spaces[1]==player))&&Spaces[2]==0)
                return 2;
            if (((Spaces[3]==player&&Spaces[8]==player)||(Spaces[0]==player&&Spaces[7]==player))&&Spaces[6]==0)
                return 6;
            if (((Spaces[5]==player&&Spaces[6]==player)||(Spaces[2]==player&&Spaces[7]==player))&&Spaces[8]==0)
                return 8;
        }
    }
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
            if (Spaces[i]==j&&Spaces[i+1]==j&&Spaces[i+2]==j){
                PrintWinner(j);
                return 1;
            }
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 1; j <=2 ; ++j) {
            if (Spaces[i] == j && Spaces[i + 3] == j && Spaces[i + 6] == j){
                PrintWinner(j);
                return 1;
            }
        }

    }
    for (int i = 1; i <=2 ; ++i) {
        if(Spaces[0]==i&&Spaces[4]==i&&Spaces[8]==i){
            PrintWinner(i);
            return 1;
        }
        if (Spaces[2]==i&&Spaces[4]==i&&Spaces[6]==i){
            PrintWinner(i);
            return 1;
        }
    }

    return 0;
}
void PrintWinner(int winnerNum){
    if(gameMode==0){
        switch (winnerNum) {
            case 1:
                MessageBoxW(NULL,L"Oyunu Kaybettiniz",L"Oyun Bitti",MB_ICONINFORMATION);
                break;
            case 2:
                MessageBoxW(NULL,L"Oyunu Kazandınız",L"Oyun Bitti",MB_ICONINFORMATION);
                break;
            default:
                break;
        }
    }
    if(gameMode==1){
        switch (winnerNum) {
            case 1:
                MessageBoxW(NULL,L"1. Oyuncu Kazandı \"X\"",L"Oyun Bitti",MB_ICONINFORMATION);
                break;
            case 2:
                MessageBoxW(NULL,L"2. Oyuncu Kazandı \"O\"",L"Oyun Bitti",MB_ICONINFORMATION);                break;
            default:
                break;
        }
    }
}
void Clean(HWND hwnd){
    for (int i = 0; i <= 8; ++i) {
        SetWindowText(GetDlgItem(hwnd, i + 9), "");
    }
    memset(Spaces, 0, sizeof(Spaces));
}