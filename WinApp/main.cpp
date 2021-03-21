#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")

#include "basewin.h"
#include "simulator.h"

#include "spline.h"

#include <vector>


const float MAX_WORLD_X = 10.0; // 10.0 m

template <class T> void SafeRelease(T **ppT)
{
    if (*ppT)
    {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

const int NUM_SAMPLES = 30;

class MainWindow : public BaseWindow<MainWindow>
{
    
    ID2D1Factory            *pFactory;
    ID2D1HwndRenderTarget   *pRenderTarget;
    ID2D1SolidColorBrush    *pBrush;

    void    CalculateLayout();
    HRESULT CreateGraphicsResources();
    void    DiscardGraphicsResources();
    void    OnPaint();
    void    Resize();

public:

    MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL)
    {
    }

    PCWSTR  ClassName() const { return L"Circle Window Class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

// Recalculate drawing layout wnden the size of the window changes.

void MainWindow::CalculateLayout()
{
}

HRESULT MainWindow::CreateGraphicsResources()
{
    HRESULT hr = S_OK;
    if (pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hwnd, size),
            &pRenderTarget);

        if (SUCCEEDED(hr))
        {
            const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
            hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

            if (SUCCEEDED(hr))
            {
                CalculateLayout();
            }
        }
    }
    return hr;
}

void MainWindow::DiscardGraphicsResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
}



// t is a value that goes from 0 to 1 to interpolate in a C1 continuous way across uniformly sampled data points.
// when t is 0, this will return B.  When t is 1, this will return C.
float CubicHermite(float A, float B, float C, float D, float t)
{
    float a = -A / 2.0f + (3.0f * B) / 2.0f - (3.0f * C) / 2.0f + D / 2.0f;
    float b = A - (5.0f * B) / 2.0f + 2.0f * C - D / 2.0f;
    float c = -A / 2.0f + C / 2.0f;
    float d = B;

    return a * t * t * t + b * t * t + c * t + d;
}




void MainWindow::OnPaint()
{
    HRESULT hr = CreateGraphicsResources();
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);
     
        pRenderTarget->BeginDraw();

        pRenderTarget->Clear( D2D1::ColorF(D2D1::ColorF::SkyBlue) );


        Position pos = get_position();



        D2D1_SIZE_F size = pRenderTarget->GetSize();

        float aspect = 1.0f * size.width / size.height;
        float pixelsToMeters = MAX_WORLD_X / size.width;
        float metersToPixels = 1.0 / pixelsToMeters;


        float robotWidthWorld = 0.4318f;
        float robotWidthPixels = robotWidthWorld * metersToPixels;
        float halfRobotWidthPixels = robotWidthPixels * 0.5f;




        // Create a rectangle.
        D2D1_RECT_F rectangle = D2D1::Rect(-halfRobotWidthPixels, 
            -halfRobotWidthPixels, halfRobotWidthPixels, 
            halfRobotWidthPixels);


        // Rotate

        D2D1_MATRIX_3X2_F rotation = D2D1::Matrix3x2F::Rotation(
            toDegrees(pos.h),
            D2D1::Point2F(0.0f, 00.0f)
        );


        /*  TEST SPLINE DRAWING CODE*/
        /*
        double x1 = 100;
        double x2 = 300;
        double x3 = 800;
        double x4 = 1000;
        
        double y1 = 100;
        double y2 = 100;
        double y3 = 400;
        double y4 = 400;

        Spline sp(x1, y1, x2, y2, x3, y3, x4, y4);
               
        int num_circles = 100;

        for (int i = 0; i < num_circles; ++i)
        {
            double t = i * 1.0 / num_circles;

            double x = sp.getX(t);
            double y = sp.getY(t);
  
            float radius = 2;

            D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);

            pRenderTarget->FillEllipse(ellipse, pBrush);

        }
            */
        


        float robotX = pos.x * metersToPixels;
        float robotY = pos.y * metersToPixels;

        D2D1_MATRIX_3X2_F translation = D2D1::Matrix3x2F::Translation(robotX, robotY);

        pRenderTarget->SetTransform(rotation * translation);


        // Fill the rectangle.
        pRenderTarget->FillRectangle(rectangle, pBrush);

        // Draw the transformed rectangle.
        pRenderTarget->DrawRectangle(rectangle,pBrush);


        hr = pRenderTarget->EndDraw();
        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
        {
            DiscardGraphicsResources();
        }
        EndPaint(m_hwnd, &ps);
    }
}

void MainWindow::Resize()
{
    if (pRenderTarget != NULL)
    {
        RECT rc;
        GetClientRect(m_hwnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

        pRenderTarget->Resize(size);
        CalculateLayout();
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    MainWindow win;

    if (!win.Create(L"Circle", WS_OVERLAPPEDWINDOW))
    {
        return 0;
    }

    ShowWindow(win.Window(), nCmdShow);

    SetTimer(win.Window(), 1000, 100, (TIMERPROC)NULL);
    // Run the message loop.


    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        if (FAILED(D2D1CreateFactory(
                D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
        {
            return -1;  // Fail CreateWindowEx.
        }
        return 0;

    case WM_DESTROY:
        DiscardGraphicsResources();
        SafeRelease(&pFactory);
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        OnPaint();
        return 0;

     // Other messages not shown...

    case WM_TIMER:
        switch (wParam) {
        case 1000:
            // Run the model
            run_sim();


            // Draw
            InvalidateRect(m_hwnd, NULL, FALSE);

           return 0;
        }
        return 0;


    case WM_SIZE:
        Resize();
        return 0;
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}
