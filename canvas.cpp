#include "paint.h"

HBITMAP g_hbm = NULL;
static BOOL s_bDragging = FALSE;
static POINT s_ptOld;

static void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case ID_CUT:
        // TODO:
        break;
    case ID_COPY:
        // TODO:
        break;
    case ID_PASTE:
        // TODO:
        break;
    case ID_DELETE:
        // TODO:
        break;
    case ID_SELECT_ALL:
        // TODO:
        break;
    case ID_SELECT:
        // TODO:
        break;
    case ID_PENCIL:
        // TODO:
        break;
    }
}

static BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    g_hbm = DoCreate24BppBitmap(320, 120);
    return TRUE;
}

static void OnDestroy(HWND hwnd)
{
    DeleteObject(g_hbm);
    g_hbm = NULL;
}

static void OnPaint(HWND hwnd)
{
    BITMAP bm;
    GetObject(g_hbm, sizeof(bm), &bm);

    PAINTSTRUCT ps;
    if (HDC hDC = BeginPaint(hwnd, &ps))
    {
        if (HDC hMemDC = CreateCompatibleDC(NULL))
        {
            HBITMAP hbmOld = SelectBitmap(hMemDC, g_hbm);
            BitBlt(hDC, 0, 0, bm.bmWidth, bm.bmHeight,
                   hMemDC, 0, 0, SRCCOPY);
            SelectBitmap(hMemDC, hbmOld);

            DeleteDC(hMemDC);
        }
        EndPaint(hwnd, &ps);
    }
}

static void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    if (fDoubleClick)
        return;

    s_bDragging = TRUE;
    SetCapture(hwnd);

    s_ptOld.x = x;
    s_ptOld.y = y;
}

static void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
    if (!s_bDragging)
        return;

    POINT pt = { x, y };
    if (HDC hMemDC = CreateCompatibleDC(NULL))
    {
        HPEN hPenOld = SelectPen(hMemDC, GetStockPen(WHITE_PEN));
        HBITMAP hbmOld = SelectBitmap(hMemDC, g_hbm);
        MoveToEx(hMemDC, s_ptOld.x, s_ptOld.y, NULL);
        LineTo(hMemDC, x, y);
        SelectBitmap(hMemDC, hbmOld);
        SelectPen(hMemDC, hPenOld);

        DeleteDC(hMemDC);

        InvalidateRect(hwnd, NULL, TRUE);
    }

    s_ptOld = pt;
}

static void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
    if (!s_bDragging)
        return;

    POINT pt = { x, y };
    if (HDC hMemDC = CreateCompatibleDC(NULL))
    {
        HPEN hPenOld = SelectPen(hMemDC, GetStockPen(WHITE_PEN));
        HBITMAP hbmOld = SelectBitmap(hMemDC, g_hbm);
        MoveToEx(hMemDC, s_ptOld.x, s_ptOld.y, NULL);
        LineTo(hMemDC, x, y);
        SelectBitmap(hMemDC, hbmOld);
        SelectPen(hMemDC, hPenOld);

        DeleteDC(hMemDC);

        InvalidateRect(hwnd, NULL, TRUE);
    }

    ReleaseCapture();
    s_bDragging = FALSE;
}

LRESULT CALLBACK
CanvasWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
        HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);
        HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
        HANDLE_MSG(hwnd, WM_LBUTTONDOWN, OnLButtonDown);
        HANDLE_MSG(hwnd, WM_MOUSEMOVE, OnMouseMove);
        HANDLE_MSG(hwnd, WM_LBUTTONUP, OnLButtonUp);
        case WM_CAPTURECHANGED:
            s_bDragging = FALSE;
            break;
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}
