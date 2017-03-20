#include "DynamicTextEffectBase.h"

int DT_RainbowRight(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
             LedBitmap *bitmap, int x, int y)
{
    DT_Rainbow_Create(input, output, pparams);
    return DT_Rainbow_ProcessBinary(input, output, pparams,bitmap,x,y,rdRight);
}
