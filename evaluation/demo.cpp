// **************************************************************
// NAME:            Fast Bitmap Rotation and Scaling Test
// AUTHORS:         Based on work by Steven M Mortimer,
//                  Extended By openlab.vn.ua team
// Target Platform: Win32
// ***************************************************************

#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
//#include "cdib.h"
//#include "../src/rotate.h"

#define _USE_MATH_DEFINES
#include <math.h>
static double Math_PI = M_PI; // 3.1415;

// Utils
// ============================================================================

#define COLORREF_COLOR_BLACK    RGB(0, 0, 0)
#define COLORREF_COLOR_GOLD     RGB(0xFF, 0xD7, 0x00)
#define COLORREF_COLOR_RED      RGB(0xFF, 0x00, 0x00)

// Hi speed timer
// ----------------------------------------------------------------------------

static bool    gbTimeFunction = false;
static double  gdTicksPerSec = 0.0;

static bool TimingInit()
{
    // Check if we can use the high performace counter
    // for timing the rotation function
    LARGE_INTEGER perfreq;
    if (QueryPerformanceFrequency(&perfreq))
    {
        gdTicksPerSec = (double)perfreq.LowPart;
        gbTimeFunction = true;
    }
    else
    {
        gbTimeFunction = false;
    }

    return(gbTimeFunction);
}

static bool TimingIsAvailable() { return gbTimeFunction; }

// Returns seconds since app start 
static double TimingGetValue() 
{
    if (gbTimeFunction)
    {
        LARGE_INTEGER time;
        QueryPerformanceCounter(&time);

        return (double)time.QuadPart / gdTicksPerSec;
    }
    return 0.0;
}

// Main code
// ============================================================================

#include "ISurface.h"
#include "Utils.h"

struct CProvider
{
    ISurfacePlotLine*  provider;
    const char*        name;
    HBRUSH             brush;
};

#include "CSurfacePlotLineCassic.h"
#include "CSurfacePlotLineNew.h"

#if !defined(ARRAY_SIZE)
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#endif

/*
Line style [A]
{0,0}->{2,1}
XX.
..X

Line style [B]
{0,0}->{2,1}
X..
.XX
*/

static CProvider PLOT_PRIVIDERS[] =
{
    // Erratic
  //{ new CSurfacePlotLineBresenhamWikipediaRu(),         "WikipediaRu"   , GetStockBrush(GRAY_BRUSH)  },
    // Classic A
    { new CSurfacePlotLineBresenhamRosettaC(),            "RosettaC"      , GetStockBrush(GRAY_BRUSH)  },
  //{ new CSurfacePlotLineBresenhamJacobsUnivercity(),    "JacobsUniv"    , GetStockBrush(GRAY_BRUSH)  },
    { new CSurfacePlotLineBresenhamJacobsUnivercityOpt(), "JacobsUnivOpt" , GetStockBrush(GRAY_BRUSH)  },
  //{ new CSurfacePlotLineBresenhamWikibooksRu(),         "WikibooksRu"   , GetStockBrush(GRAY_BRUSH)  },
    { new CSurfacePlotLineBresenhamWikipediaEn1(),        "WikipediaEn1"  , GetStockBrush(GRAY_BRUSH)  },
    // Classic B
    { new CSurfacePlotLineBresenhamWikipediaEn2(),        "WikipediaEn2"  , GetStockBrush(GRAY_BRUSH)  },
    { new CSurfacePlotLineBresenhamZingl(),               "Zingl"         , GetStockBrush(GRAY_BRUSH)  },
    // New
    { new CSurfacePlotLineBresenhamAgeyevV1A(),           "Ageyev[V1A]"   , GetStockBrush(WHITE_BRUSH) },
    { new CSurfacePlotLineBresenhamAgeyevV2A(),           "Ageyev[V2A]"   , GetStockBrush(WHITE_BRUSH) },
    { new CSurfacePlotLineBresenhamAgeyevV1B(),           "Ageyev[V1B]"   , GetStockBrush(WHITE_BRUSH) },
};

static const int PLOT_PRIVIDERS_COUNT = ARRAY_SIZE(PLOT_PRIVIDERS);

/////////////////////////////////////////////////////////////////

class CPlotPixelOnHDC : public ISurfacePlotPixel
{
    protected: HDC hdc;
    protected: int scale;
    protected: HBRUSH brush;

    protected: static HBRUSH GetDefaultBrush() { return GetStockBrush(WHITE_BRUSH); }

    public: void SetBrush(HBRUSH brush) { this->brush = brush; }
    public: HBRUSH GetBrush() { return this->brush; }

    public: CPlotPixelOnHDC(HDC hdc, int scale = 1)
    {
        this->hdc = hdc;
        this->scale = scale;
        this->brush = GetDefaultBrush();
    }

    public: virtual void PlotPixel(int x, int y)
    {
        RECT rect;
        SetRect(&rect, x * scale, y * scale, (x + 1) * scale - 1, (y + 1) * scale - 1);
        FillRect(hdc, &rect, this->brush);
    }
};

/////////////////////////////////////////////////////////////////

static void Update(HDC hdc)
{
    double dRotBeginT = TimingGetValue();

    // TODO

    double dRotEndT = TimingGetValue();

    double fPixlesCount; // Pixels affected

    fPixlesCount = 0; // TODO

    double dUpdateBeginT = TimingGetValue();

    //draw_horz_line_3(hdc, 10, 10, 10 + 4, 10 + 2);

    int scale  = 3;
    CPlotPixelOnHDC PixelPlotter(hdc, scale);

    int ofsx   = 5;
    int ofsy   = 5;

    int test_lens[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8 }; // line length by colunms

    int sx, sy;
    
    // cols

    sx = ofsx;
    for (int col = 0; col < ARRAY_SIZE(test_lens); col++)
    {
        sy = ofsy;

        int dx = test_lens[col];

        for (int dy = 0; dy <= dx; dy++)
        {
            for (int ii = 0; ii < PLOT_PRIVIDERS_COUNT; ii++)
            {
                PixelPlotter.SetBrush(PLOT_PRIVIDERS[ii].brush);
                PLOT_PRIVIDERS[ii].provider->PlotLine(sx, sy, sx + dx, sy + dy, &PixelPlotter);

                if ((col + 1) == ARRAY_SIZE(test_lens))
                {
                    // This is last col
                }

                sy += 3; // sy += 3 + dy;
            }

            sy += dy;
        }

        sx += dx + 2;
    }

    // rows

    ofsx = sx;

    sy = ofsy;
    for (int row = 0; row < ARRAY_SIZE(test_lens); row++)
    {
        int sx = ofsx;

        int dy = test_lens[row];

        for (int dx = 0; dx <= dy; dx++)
        {
            for (int ii = 0; ii < PLOT_PRIVIDERS_COUNT; ii++)
            {
                PixelPlotter.SetBrush(PLOT_PRIVIDERS[ii].brush);
                PLOT_PRIVIDERS[ii].provider->PlotLine(sx, sy, sx + dx, sy + dy, &PixelPlotter);

                if ((row + 1) == ARRAY_SIZE(test_lens))
                {
                    // This is last row
                }

                sx += 3; // sy += 3 + dy;
            }

            sx += dx;
        }

        sy += dy + 2;
    }

    double dUpdateEndT = TimingGetValue();
}

static BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct)
{
    const char* SZIMAGE = "";

    BOOL bSuccess = true; // gDibSrc->Load(SZIMAGE, sizeof(RotatePixel_t)); // ZZZ
    if (bSuccess)
    {
        // Start the update timer
        SetTimer(hwnd, 0, 100, NULL);
    }   
    else
    {
        char szError[512];
        wsprintf(szError, "Error loading image %s", SZIMAGE);
        MessageBox(hwnd, szError, "oops", MB_OK);
    }

    return bSuccess;
}

static void OnSize(HWND hwnd, UINT state, int x, int y)
{
}

static void OnPaint(HWND hwnd)
{
    HDC hdc;
    PAINTSTRUCT ps;
 
    hdc = BeginPaint (hwnd, &ps);
    Update(hdc);
    EndPaint (hwnd, &ps) ;
}

static void DoRedraw(HWND hwnd)
{
    HDC hdc = GetDC(hwnd);
    Update(hdc);
    ReleaseDC(hwnd, hdc);
}

static BOOL OnTimer(HWND hwnd, UINT id)
{   
    if (false) // (gbAutoMode)
    {
        //GSTATE_STEP_AUTO();
        DoRedraw(hwnd);
    }
    return TRUE;
}

static void OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
    switch (vk)
    {
        case(VK_ADD):
        case(VK_PRIOR): // PgUp
        {
            //GSTATE_STEP_SCALE_FORE();
            DoRedraw(hwnd);
        } break;

        case(VK_SUBTRACT):
        case(VK_NEXT): // PgDn
        {
            //GSTATE_STEP_SCALE_BACK();
            DoRedraw(hwnd);
        } break;

        case(VK_LEFT):
        {
            //GSTATE_STEP_ANGLE_BACK();
            DoRedraw(hwnd);
        } break;

        case(VK_RIGHT):
        {
            //GSTATE_STEP_ANGLE_FORE();
            DoRedraw(hwnd);
        } break;

        case('A'): // (VK_A):
        {
            //gbAutoMode = !gbAutoMode;
            DoRedraw(hwnd);
        } break;

        case('R'):
        {
            //GSTATE_RESET();
            DoRedraw(hwnd);
        } break;

        case('F'):
        {
            //GSTATE_STEP_FUNC_NAME();
            DoRedraw(hwnd);
        } break;

        default:
        {
            DoRedraw(hwnd);
        }
    }
}

static void OnDestroy(HWND hwnd)
{
    PostQuitMessage(0);
}

/////////////////////////////////////////////////////////////////

static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
    switch (iMsg)
    {
        HANDLE_MSG( hwnd, WM_CREATE,     OnCreate );
        HANDLE_MSG( hwnd, WM_SIZE,       OnSize );
        HANDLE_MSG( hwnd, WM_PAINT,      OnPaint );
      //HANDLE_MSG( hwnd, WM_ERASEBKGND, OnEraseBkGnd );
        HANDLE_MSG( hwnd, WM_TIMER,      OnTimer );
        HANDLE_MSG( hwnd, WM_KEYDOWN,    OnKeyDown );
        HANDLE_MSG( hwnd, WM_DESTROY,    OnDestroy );
    }
 
    return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}

/////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{

    HWND        hwnd;
    MSG         msg;

    // Register the window class
    WNDCLASS wndclass;
    ZeroMemory(&wndclass, sizeof(WNDCLASS));
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.hInstance = hInstance;
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.lpszClassName = __FILE__;
    wndclass.hbrBackground = GetStockBrush(BLACK_BRUSH);
    RegisterClass(&wndclass);

    if (!TimingInit())
    {
        MessageBox(NULL,
            "High resolution timer not available",
            "Warning", MB_OK);
    }

    if (true)
    {
        // Create Window
        hwnd = CreateWindow(
            __FILE__, "Line Draw Playground", WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
            NULL, NULL, hInstance, NULL);

        ShowWindow(hwnd, iCmdShow);
        UpdateWindow(hwnd);

        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}

