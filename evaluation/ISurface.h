#pragma once
#ifndef ISURFACE_INCLUDED
#define ISURFACE_INCLUDED

// Interfaces
// ----------------------------------------------------------------------------

class ISurfacePlotPixel
{
    public: virtual void PlotPixel(int x, int y) = 0;
};

class ISurfacePlotLine
{
    public: virtual void PlotLine(int x0, int y0, int x1, int y1, ISurfacePlotPixel* surface) = 0;
};

#endif
