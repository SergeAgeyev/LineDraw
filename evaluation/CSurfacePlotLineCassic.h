#pragma once
#ifndef CSURFACEPLOTLINECLASSIC_INCLUDED
#define CSURFACEPLOTLINECLASSIC_INCLUDED

#include "ISurface.h"
#include "Utils.h"

// Implemention of classic Bresenham line algorithms
// ----------------------------------------------------------------------------

class CSurfacePlotLineBresenhamWikipediaRu : public ISurfacePlotLine, protected Utils
{
    // Bresenham's line algorithm
    // From Russian wikipedia
    // https://ru.wikipedia.org/wiki/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%91%D1%80%D0%B5%D0%B7%D0%B5%D0%BD%D1%85%D1%8D%D0%BC%D0%B0

    // Subroutine, Expects: (x0 < x1) and (dx >= dy)
    protected: void DrawLineLow(int x0, int y0, int x1, int y1, ISurfacePlotPixel* surface)
    {
        int deltax = abs(x1 - x0);
        int deltay = abs(y1 - y0);
        int error = 0;
        int deltaerr = (deltay + 1);
        int y = y0;
        int diry = y1 - y0;
        if (diry > 0)
            diry = 1;
        else if (diry < 0)
            diry = -1;
        for (int x = x0; x <= x1; x++)
        {
            surface->PlotPixel(x, y);
            error = error + deltaerr;
            if (error >= (deltax + 1))
            {
                y = y + diry;
                error = error - (deltax + 1);
            }
        }
    }

    protected: class CSurfacePlotPixelXYSwap : public ISurfacePlotPixel
    {
        protected: ISurfacePlotPixel* surface;
        public: CSurfacePlotPixelXYSwap(ISurfacePlotPixel* surface) { this->surface = surface; }
        public: virtual void PlotPixel(int x, int y) { this->surface->PlotPixel(y, x); }
    };

    // Draws a line: Works with any combinations of xs and ys
    public: virtual void PlotLine(int x0, int y0, int x1, int y1, ISurfacePlotPixel* surface)
    {
        //Wrapper, to keep original alogorithm as genuie as possibe [SA]
        int deltax = x1 - x0;
        int deltay = y1 - y0;
        if (abs(deltax) >= abs(deltay))
        {
            if (deltax < 0) { swap(x0, x1); swap(y0, y1); }
            DrawLineLow(x0, y0, x1, y1, surface);
        }
        else // flip coordinates
        {
            if (deltay < 0) { swap(x0, x1); swap(y0, y1); }
            CSurfacePlotPixelXYSwap swapped_surface(surface);
            DrawLineLow(y0, x0, y1, x1, &swapped_surface);
        }
    }
};

class CSurfacePlotLineBresenhamRosettaC : public ISurfacePlotLine, protected Utils
{
    // Bresenham's line algorithm
    // From Rosetta Code project (C)
    // http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm#C

    // Draws a line: Works with any combinations of xs and ys
    public: virtual void PlotLine(int x0, int y0, int x1, int y1, ISurfacePlotPixel* surface)
    {
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = (dx > dy ? dx : -dy) / 2, e2;

        for (;;) {
            surface->PlotPixel(x0, y0);
            if (x0 == x1 && y0 == y1) break;
            e2 = err;
            if (e2 > -dx) { err -= dy; x0 += sx; }
            if (e2 < dy) { err += dx; y0 += sy; }
        }
    }
};

class CSurfacePlotLineBresenhamJacobsUnivercity : public ISurfacePlotLine, protected Utils
{
    // Bresenham's line algorithm
    // From Lecture 8 "320322: Graphics and Visualization" p294-295
    // http://www.faculty.jacobs-university.de/llinsen/teaching/320322/Lecture08.pdf

    // Draws a line: Works with any combinations of xs and ys
    // Note params names are x1,y1,x2,y2 to keep algorithm genuie
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

        int dx = x2 - x1;
        int dy = abs(y2 - y1);
        int x = x1, y = y1; // start point
        int error = dx / 2; // initialization
        int ystep = (y1 < y2) ? 1 : -1; // step in + or - y-direction
        for (int i = 0; i <= dx; i++)
        {
            if (swapXY) { surface->PlotPixel(y, x); } else { surface->PlotPixel(x, y); }
            error -= dy; // substract dy
            x++;
            if (error < 0) // new error test
            {
                y += ystep;
                error += dx;
            }
        }
    }
};

class CSurfacePlotLineBresenhamJacobsUnivercityOpt : public ISurfacePlotLine, protected Utils
{
    // Bresenham's line algorithm
    // From Lecture 8 "320322: Graphics and Visualization" p294-295
    // http://www.faculty.jacobs-university.de/llinsen/teaching/320322/Lecture08.pdf
    // Based on CSurfacePlotLineBresenhamJacobsUnivercity [Minor optimization added]

    // Draws a line: Works with any combinations of xs and ys
    // Note params names are x1,y1,x2,y2 to keep algorithm genuie
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

        int dx = x2 - x1;
        int dy = abs(y2 - y1);
        int error = dx / 2; // initialization
        int ystep = (y1 < y2) ? 1 : -1; // step in + or - y-direction
        int y = y1; // start point y
        for (int x = x1; x <= x2; x++)
        {
            if (swapXY) { surface->PlotPixel(y, x); } else { surface->PlotPixel(x, y); }
            error -= dy; // substract dy
            if (error < 0) // new error test
            {
                y += ystep;
                error += dx;
            }
        }
    }
};

class CSurfacePlotLineBresenhamWikipediaEn1 : public ISurfacePlotLine, protected Utils
{
    // Bresenham's line algorithm
    // English wiki (variant 1)
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm

    // Subroutine, Expects: (x0 < x1) and (dx >= dy)
    protected: virtual void DrawLineLow(int x0, int y0, int x1, int y1, ISurfacePlotPixel* surface)
    {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int yi = 1;

        if (dy < 0)
        {
            yi = -1;
            dy = -dy;
        }

        int D = (2 * dy) - dx;
        int y = y0;

        for (int x = x0; x <= x1; x++)
        {
            surface->PlotPixel(x, y);
            if (D > 0)
            {
                y = y + yi;
                D = D + (2 * (dy - dx));
            }
            else
            {
                D = D + 2 * dy;
            }
        }
    }

    // Subroutine, Expects: (y0 < y1) and (dy >= dx)
    protected: virtual void DrawLineHigh(int x0, int y0, int x1, int y1, ISurfacePlotPixel* surface)
    {
        int dx = x1 - x0;
        int dy = y1 - y0;
        int xi = 1;

        if (dx < 0)
        {
            xi = -1;
            dx = -dx;
        }

        int D = (2 * dx) - dy;
        int x = x0;

        for (int y = y0; y <= y1; y++)
        {
            surface->PlotPixel(x, y);
            if (D > 0)
            {
                x = x + xi;
                D = D + (2 * (dx - dy));
            }
            else
            {
                D = D + 2 * dx;
            }
        }
    }

    // Draws a line: Works with any combinations of xs and ys
    public: virtual void PlotLine(int x0, int y0, int x1, int y1, ISurfacePlotPixel* surface)
    {
        if (abs(y1 - y0) < abs(x1 - x0))
            if (x0 > x1)
                DrawLineLow(x1, y1, x0, y0, surface);
            else
                DrawLineLow(x0, y0, x1, y1, surface);
        else
            if (y0 > y1)
                DrawLineHigh(x1, y1, x0, y0, surface);
            else
                DrawLineHigh(x0, y0, x1, y1, surface);
    }
};

class CSurfacePlotLineBresenhamWikipediaEn2 : public ISurfacePlotLine, protected Utils
{
    // Bresenham's line algorithm
    // English wiki (variant 2)
    // https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
    // Use Bresenham's principles of integer incremental error to perform all octant line draws, balancing the positive and negative error between the x and y coordinates
    // Based on
    // Alois Zingl "A Rasterizing Algorithm for Drawing Curves" (2012)
    // http://members.chello.at/~easyfilter/Bresenham.pdf

    // Draws a line: Works with any combinations of xs and ys
    public: virtual void PlotLine(int x0, int y0, int x1, int y1, ISurfacePlotPixel* surface)
    {
        int dx = abs(x1 - x0);
        int sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0);
        int sy = y0 < y1 ? 1 : -1;
        int err = dx + dy;  /* error value e_xy */
        while (true) /* loop */
        {
            surface->PlotPixel(x0, y0);
            if (x0 == x1 && y0 == y1) break;
            int e2 = 2 * err;
            if (e2 >= dy) /* e_xy+e_x > 0 */
            {
                err += dy;
                x0 += sx;
            }
            if (e2 <= dx) /* e_xy+e_y < 0 */
            {
                err += dx;
                y0 += sy;
            }
        }
    }
};

class CSurfacePlotLineBresenhamZingl : public ISurfacePlotLine, protected Utils
{
    // Bresenham's line algorithm
    // Alois Zingl "A Rasterizing Algorithm for Drawing Curves" (2012)
    // http://members.chello.at/~easyfilter/Bresenham.pdf

    // Draws a line: Works with any combinations of xs and ys
    public: virtual void PlotLine(int x0, int y0, int x1, int y1, ISurfacePlotPixel* surface)
    {
        int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
        int err = dx + dy, e2; /* error value e_xy */
        for (;;) { /* loop */
            surface->PlotPixel(x0, y0);
            e2 = 2 * err;
            if (e2 >= dy) { /* e_xy+e_x > 0 */
                if (x0 == x1) break;
                err += dy; x0 += sx;
            }
            if (e2 <= dx) { /* e_xy+e_y < 0 */
                if (y0 == y1) break;
                err += dx; y0 += sy;
            }
        }
    }
};

class CSurfacePlotLineBresenhamWikibooksRu : public ISurfacePlotLine, protected Utils
{
    // Bresenham's line algorithm
    // https://ru.wikibooks.org/wiki/%D0%A0%D0%B5%D0%B0%D0%BB%D0%B8%D0%B7%D0%B0%D1%86%D0%B8%D0%B8_%D0%B0%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC%D0%BE%D0%B2/%D0%90%D0%BB%D0%B3%D0%BE%D1%80%D0%B8%D1%82%D0%BC_%D0%91%D1%80%D0%B5%D0%B7%D0%B5%D0%BD%D1%85%D1%8D%D0%BC%D0%B0

    // Draws a line: Works with any combinations of xs and ys
    public: virtual void PlotLine(int x1, int y1, int x2, int y2, ISurfacePlotPixel* surface)
    {
        const int deltaX = abs(x2 - x1);
        const int deltaY = abs(y2 - y1);
        const int signX = x1 < x2 ? 1 : -1;
        const int signY = y1 < y2 ? 1 : -1;
        int error = deltaX - deltaY;
        surface->PlotPixel(x2, y2);
        while (x1 != x2 || y1 != y2)
        {
            surface->PlotPixel(x1, y1);
            int error2 = error * 2;
            if (error2 > -deltaY)
            {
                error -= deltaY;
                x1 += signX;
            }
            if (error2 < deltaX)
            {
                error += deltaX;
                y1 += signY;
            }
        }
    }
};


#endif
