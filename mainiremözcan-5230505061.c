#include <windows.h>
#define ID_BUTTON_00 1000
#define ID_BUTTON_01 1001
#define ID_BUTTON_02 1002
#define ID_BUTTON_03 1003
#define ID_BUTTON_04 1004
#define ID_BUTTON_05 1005
#define ID_BUTTON_06 1006
#define ID_BUTTON_07 1007
#define ID_BUTTON_08 1008
#define ID_BUTTON_09 1009
#define ID_BUTTON_ADD 1010
#define ID_BUTTON_SUBTRACT 1011
#define ID_BUTTON_MULTIPLY 1012
#define ID_BUTTON_DIVIDE 1013
#define ID_BUTTON_EQUAL 1014
#define ID_BUTTON_CLEAR 1015

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

HWND hEditResult;

char buffer[256] = ""; // Sayýlarý ve operatörleri saklamak için tampon
double num1 = 0.0;
double num2 = 0.0;
char operatorChar = '\0';

int main() {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    memset(&wc, 0, sizeof(wc));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = "CalculatorClass";
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "CalculatorClass", "Calculator", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // satir ve sutunlar ile matris þeklinde düzenlenmiþ düðmeler oluþturmak için kullanýldý. 
    int buttonID = ID_BUTTON_00;
    int sira,sutun;
    for (sira = 0; sira < 3; ++sira) {
        for (sutun= 0; sutun< 3; ++sutun) {
            char buttonText[2];
            //sprintf fonksiyonu ile belirtilen formata göre bir diziye veri yazdirdik
            sprintf(buttonText, "%d", sira * 3 + sutun);
            CreateWindow("BUTTON", buttonText, WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                10 + sutun * 80, 10 + sira* 50, 70, 40, hwnd, (HMENU)buttonID, GetModuleHandle(NULL), NULL);
            buttonID++;
        }
    }

    // 4 islem butonlarý
    CreateWindow("BUTTON", "+", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10 + 3 * 80, 10, 70, 40, hwnd, (HMENU)ID_BUTTON_ADD, GetModuleHandle(NULL), NULL);
    CreateWindow("BUTTON", "-", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10 + 3 * 80, 60, 70, 40, hwnd, (HMENU)ID_BUTTON_SUBTRACT, GetModuleHandle(NULL), NULL);
    CreateWindow("BUTTON", "*", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10 + 3 * 80, 110, 70, 40, hwnd, (HMENU)ID_BUTTON_MULTIPLY, GetModuleHandle(NULL), NULL);
    CreateWindow("BUTTON", "/", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10 + 3 * 80, 160, 70, 40, hwnd, (HMENU)ID_BUTTON_DIVIDE, GetModuleHandle(NULL), NULL);

    // Clear ve eþittir butonlari
    CreateWindow("BUTTON", "=", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10 + 3 * 80, 210, 70, 40, hwnd, (HMENU)ID_BUTTON_EQUAL, GetModuleHandle(NULL), NULL);
    CreateWindow("BUTTON", "Clear", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
        10 + 3 * 80, 260, 70, 40, hwnd, (HMENU)ID_BUTTON_CLEAR, GetModuleHandle(NULL), NULL);

    // Textbox kutusu
    hEditResult = CreateWindow("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT | ES_READONLY,
        10, 310, 370, 40, hwnd, (HMENU)ID_BUTTON_EQUAL, GetModuleHandle(NULL), NULL);

    if (hEditResult == NULL) {
        MessageBox(NULL, "Result Edit Box Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}
//Bu kod, farklý düðmelere iþlevlerini ekler.
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch (Message) {
    case WM_COMMAND:
        switch (HIWORD(wParam)) {
        case BN_CLICKED: {
            int buttonID = LOWORD(wParam);
            switch (buttonID) {
            case ID_BUTTON_00:
            case ID_BUTTON_01:
            case ID_BUTTON_02:
            case ID_BUTTON_03:
            case ID_BUTTON_04:
            case ID_BUTTON_05:
            case ID_BUTTON_06:
            case ID_BUTTON_07:
            case ID_BUTTON_08:
            case ID_BUTTON_09:
                // Sayý butonlarýna týklandýðýnda
                sprintf(buffer, "%s%c", buffer, '0' + buttonID - ID_BUTTON_00);
                SetWindowText(hEditResult, buffer);
                break;
            case ID_BUTTON_ADD:
            case ID_BUTTON_SUBTRACT:
            case ID_BUTTON_MULTIPLY:
            case ID_BUTTON_DIVIDE:
                // Ýþlem butonlarýna týklandýðýnda
                if (strlen(buffer) > 0) {
                    num1 = atof(buffer);
                    operatorChar = (char)(wParam == ID_BUTTON_ADD ? '+' : (wParam == ID_BUTTON_SUBTRACT ? '-' : (wParam == ID_BUTTON_MULTIPLY ? '*' : '/')));
                    buffer[0] = '\0';
                }
                break;
            case ID_BUTTON_EQUAL:
                // Eþittir butonuna týklandýðýnda çalýtýrýr.
                if (strlen(buffer) > 0) {
                    num2 = atof(buffer);
                    switch (operatorChar) {
                    case '+':
                        num1 += num2;
                        break;
                    case '-':
                        num1 -= num2;
                        break;
                    case '*':
                        num1 *= num2;
                        break;
                    case '/':
                        if (num2 != 0.0) {
                            num1 /= num2;
                        } else {
                        	//herhangi bir sayi 0 bölündüðünde tanýmsýz olarak tanýmlar.
                           MessageBox(hwnd, "tanimsiz", "hata", MB_OK | MB_ICONERROR);
                            num1 = 0.0;
                            //sayý tuþlarýna basýnca, bu sayýlar buffer içinde saklanýyor ve ardýndan bu sayýlar üzerinde iþlemler gerçekleþtiriliyor.
                            buffer[0] = '\0';
                            break;
                        }
                        break;
                    }
                    sprintf(buffer, "%g", num1);
                    SetWindowText(hEditResult, buffer);
                    buffer[0] = '\0';
                }
                break;
            case ID_BUTTON_CLEAR:
                // Clear butonuna týklandýðýnda çalýsmasý için.
                buffer[0] = '\0';
                SetWindowText(hEditResult, buffer);
                num1 = num2 = 0.0;
                break;
            }
            break;
        }
        }
        break;
    case WM_DESTROY: //penceriyi kapatmayý saðlar.
        PostQuitMessage(0);
        break;
        //eþleþmeyen tüm durumlar için kullanýlýr.
    default:
        return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}


