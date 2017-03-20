#ifndef GRAPHIC_H
#define GRAPHIC_H

#include <stdint.h>

#include <QPixmap>
#include <QColor>
#include <QString>

namespace Font {
static const uint32_t FONT_INDEX_DEFAULT = 7;
static const uint32_t FONT_SIZE_MIN = 5;
static const uint32_t FONT_SIZE_MAX = 100;
static const uint32_t FONT_SIZE_DEFAULT = 12;
}

namespace Color {

typedef uint32_t Argb;

static const Argb BG_COLOR_NONE = 0x00000000;
static const Argb BG_COLOR_NONE_TOP = 0x01000000;
static const Argb BG_COLOR_DEFAULT = 0xFF000000;

static const Argb OPAQUE_WHITE = 0xFFFFFFFF;
static const Argb OPAQUE_BLACK = 0xFF000000;
static const Argb OPAQUE_RED = 0xFFFF0000;
static const Argb OPAQUE_BLUE = 0xFF0000FF;
static const Argb OPAQUE_GREEN = 0xFF00FF00;
static const Argb OPAQUE_YELLOW = 0xFFFFFF00;

static const Argb OPAQUE_ORANGERED = 0xFFFF4500;
static const Argb OPAQUE_FIREBRICK = 0xFFB22222;
static const Argb OPAQUE_SEAGREEN = 0xFF2E8B57;
static const Argb OPAQUE_DARKGREEN = 0xFF006400;
static const Argb OPAQUE_ROYALBLUE = 0xFF4169E1;
static const Argb OPAQUE_STEELBLUE = 0xFF4682B4;

static const Argb OPAQUE_VIOLET = 0xFF8A2BE2;
static const Argb OPAQUE_INDIGO = 0xFF4B0082;

}

namespace Graphic
{

typedef enum {
    STRECH,
    FILL,
    FIT,
    TILE,
    CENTER,
    SCALE_MAX
}Scale;

extern const QString scaleText[SCALE_MAX];

QPixmap scale(QPixmap &source, Scale scale, const QSize &targetSize);

typedef enum {
    TOP,
    LEFT,
    RIGHT,
    BOTTOM,
    HORIZON,
    VERTICAL,
    ROTATE_MAX
}Rotate;

extern const QString rotateText[ROTATE_MAX];

QPixmap rotate(QPixmap &source, Rotate rotate);

typedef enum {
    MONO,
    BI,
    FULL,
    FILTER_MAX
}Filter;

extern const QString filterText[FILTER_MAX];

QPixmap filter(QPixmap &source, Filter filter);

}

#endif // GRAPHIC_H
