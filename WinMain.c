#include <windows.h>
#define printf __builtin_printf

INT CompareDisplaySettingsRefreshLowW(CONST VOID *vA, CONST VOID *vB)
{
    DEVMODEW *dmA = *(DEVMODEW **)vA,
             *dmB = *(DEVMODEW **)vB;
    if (dmA->dmPelsWidth != dmB->dmPelsWidth)
        return dmA->dmPelsWidth < dmB->dmPelsWidth
                   ? -1
                   : 1;
    if (dmA->dmPelsHeight != dmB->dmPelsHeight)
        return dmA->dmPelsHeight < dmB->dmPelsHeight
                   ? -1
                   : 1;
    return dmA->dmDisplayFrequency > dmB->dmDisplayFrequency
               ? -1
               : 1;
}
INT CompareDisplaySettingsRefreshHighW(CONST VOID *vA, CONST VOID *vB)
{
    DEVMODEW *dmA = *(DEVMODEW **)vA,
             *dmB = *(DEVMODEW **)vB;
    if (dmA->dmPelsWidth != dmB->dmPelsWidth)
        return dmA->dmPelsWidth < dmB->dmPelsWidth
                   ? -1
                   : 1;
    if (dmA->dmPelsHeight != dmB->dmPelsHeight)
        return dmA->dmPelsHeight < dmB->dmPelsHeight
                   ? -1
                   : 1;
    return dmA->dmDisplayFrequency < dmB->dmDisplayFrequency
               ? -1
               : 1;
}

LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        SendNotifyMessageW(hWnd, WM_POWERBROADCAST, PBT_APMPOWERSTATUSCHANGE, 0);
        return 0;

    case WM_POWERBROADCAST:
        if (wParam != PBT_APMPOWERSTATUSCHANGE)
            break;
        MONITORINFOEXW miMonitor = {.cbSize = sizeof(MONITORINFOEXW)};
        SYSTEM_POWER_STATUS sps = {0};
        DEVMODEW **dmDevModes = 0, dmDevMode = {.dmSize = sizeof(DEVMODEW)};
        DWORD iModeNum = 0,
              nMaxCount = 0;

        GetMonitorInfoW(MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY), (MONITORINFO *)&miMonitor);
        GetSystemPowerStatus(&sps);
        while (EnumDisplaySettingsExW(miMonitor.szDevice, iModeNum, &dmDevMode, EDS_RAWMODE | EDS_ROTATEDMODE))
        {
            if (!(dmDevMode.dmDisplayOrientation | dmDevMode.dmDisplayFixedOutput))
            {
                dmDevModes = realloc(dmDevModes, sizeof(DEVMODEW *) * (nMaxCount + 1));
                dmDevModes[nMaxCount] = alloca(sizeof(DEVMODEW));
                memcpy(dmDevModes[nMaxCount], &dmDevMode, sizeof(DEVMODEW));
                nMaxCount++;
            }
            iModeNum++;
        }
        qsort(dmDevModes,
              nMaxCount,
              sizeof(DEVMODE *),
              sps.ACLineStatus
                  ? CompareDisplaySettingsRefreshHighW
                  : CompareDisplaySettingsRefreshLowW);
        ChangeDisplaySettingsW(dmDevModes[nMaxCount - 1], CDS_UPDATEREGISTRY);
        free(dmDevModes);

    default:
        return DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg = {0};
    if (!RegisterClassExW(&((WNDCLASSEXW){.cbSize = sizeof(WNDCLASSEXW),
                                          .lpfnWndProc = WndProc,
                                          .hInstance = hInstance,
                                          .lpszClassName = L" "})) ||
        !CreateWindowExW(0, L" ", 0, 0, 0, 0, 0, 0, 0, 0, hInstance, 0))
        return 1;
    while (GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}