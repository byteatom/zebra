#include "DynamicTextEffectBase.h"


int DT_RandomColor(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
                 LedBitmap *bitmap, int x, int y)
{
    DT_NeonLight_Create(input, output, pparams);
    return DT_NeonLight_ProcessBinary(input, output, pparams,bitmap,x,y,false,true);
}
