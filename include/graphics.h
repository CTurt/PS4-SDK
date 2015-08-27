#pragma once

#define RGB(r, g, b) ((r) | ((g) << 8) | ((b) << 16) | (0xff << 24))

#define RED 0xff0000ff
#define GREEN 0xff00ff00
#define BLUE 0xffff0000

#define WHITE 0xffffffff
#define BLACK 0xff000000
