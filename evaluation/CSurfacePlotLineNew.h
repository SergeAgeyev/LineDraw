#pragma once
#ifndef CSURFACEPLOTLINENEW_INCLUDED
#define CSURFACEPLOTLINENEW_INCLUDED

#include "ISurface.h"
#include "Utils.h"

// Implemention of Bresenham line algorithms improved by SA (half line rounding flip)
// ----------------------------------------------------------------------------

class CSurfacePlotLineBresenhamAgeyevV1A : public ISurfacePlotLine, protected Utils
{
    // Bresenham's line algorithm [Improved with half line rounding flip (A)]
    // Based on CSurfacePlotLineBresenhamJacobsUnivercity{Opt}

    // Draws a line: Works with any combinations of xs and ys
    public: virtual void PlotLine(int x1, int y1, int x2, int y2, ISurfacePlotPixel* surface)
    {
        bool swapXY = false;

        if (abs(y2 - y1) > abs(x2 - x1))
        {
            swap(x1, y1);
            swap(x2, y2);
            swapXY = true; // use this as flag to swap coords later
        }
        if (x1 > x2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }

        int dx = x2 - x1; // always positive
        int dy = abs(y2 - y1);
        int error = dx; // half of future dx
        dx += dx; // dx = dx * 2;
        dy += dy; // dy = dy * 2;
        int ystep = (y1 < y2) ? 1 : -1; // step in + or - y-direction
        int y = y1; // start point y
        for (int x = x1; x <= x2; x++)
        {
            if (swapXY) { surface->PlotPixel(y, x); }  else { surface->PlotPixel(x, y); }
            error -= dy; // substract dy
            if ((x - x1) * 4 < dx)
            {
                // first half of line
                if (error < 0)
                {
                    y += ystep;
                    error += dx;
                }
            }
            else
            {
                // second half of line
                if (error <= 0)
                {
                    y += ystep;
                    error += dx;
                }
            }
        }
    }
};

class CSurfacePlotLineBresenhamAgeyevV1B : public ISurfacePlotLine, protected Utils
{
    // Bresenham's line algorithm [Improved with half line rounding flip (B)]
    // Based on CSurfacePlotLineBresenhamJacobsUnivercity{Opt}

    // Draws a line: Works with any combinations of xs and ys
    public: virtual void PlotLine(int x1, int y1, int x2, int y2, ISurfacePlotPixel* surface)
    {
        bool swapXY = false;
        if (abs(y2 - y1) > abs(x2 - x1))
        {
            swap(x1, y1);
            swap(x2, y2);
            swapXY = true; // use this as flag to swap coords later
        }
        if (x1 > x2)
        {
            swap(x1, x2);
            swap(y1, y2);
        }

        int dx = x2 - x1; // always positive
        int dy = abs(y2 - y1);
        int error = dx; // half of future dx
        dx += dx; // dx = dx * 2;
        dy += dy; // dy = dy * 2;
        int ystep = (y1 < y2) ? 1 : -1; // step in + or - y-direction
        int y = y1; // start point y
        for (int x = x1; x <= x2; x++)
        {
            if (swapXY) { surface->PlotPixel(y, x); }
            else { surface->PlotPixel(x, y); }
            error -= dy; // substract dy
            if ((x - x1) * 4 < dx)
            {
                // first half of line
                if (error <= 0) // A->B: < to <=
                {
                    y += ystep;
                    error += dx;
                }
            }
            else
            {
                // second half of line
                if (error < 0) // A->B: <= to <
                {
                    y += ystep;
                    error += dx;
                }
            }
        }
    }
};

class CSurfacePlotLineBresenhamAgeyevV2A : public ISurfacePlotLine, protected Utils
{
    // Bresenham's line algorithm  [Improved with half line rounding flip (A)]
    // Based on CSurfacePlotLineBresenhamRosettaC

    // Draws a line: Works with any combinations of xs and ys
    public: virtual void PlotLine(int x0, int y0, int x1, int y1, ISurfacePlotPixel* surface)
    {
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = (dx > dy ? dx : -dy), e2;
        dx += dx; // dx = dx * 2;
        dy += dy; // dy = dy * 2;
        int cx = abs(err);
        for (;;) {
            surface->PlotPixel(x0, y0);
            if (x0 == x1 && y0 == y1) break;
            e2 = err;
            if (cx > 0)
            {
                // first half of line
                if (e2 > -dx) { err -= dy; x0 += sx; }
                if (e2 < dy) { err += dx; y0 += sy; }
            }
            else
            {
                // second half of line
                if (e2 >= -dx) { err -= dy; x0 += sx; }
                if (e2 <= dy) { err += dx; y0 += sy; }
            }
            cx -= 2;
        }
    }
};

#endif
