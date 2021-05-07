#pragma once
#ifndef UTILS_INCLUDED
#define UTILS_INCLUDED

class Utils
{
    public: static inline void swap(int& a, int& b) { int t = a; a = b; b = t; }
    public: static inline int abs(int x) { return (x < 0) ? -x : x; }
};

#endif
