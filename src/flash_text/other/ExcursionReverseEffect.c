#include "DynamicTextEffectBase.h"

int DT_ExcursionReverse(const ShineFontAreaInput *input, ShineFontAreaOutput *output, DtElementBaseParams *pparams,
             LedBitmap *bitmap, int x, int y)
{
    DT_Excursion_Create(input, output, pparams);
    return DT_Excursion_ProcessBinary(input, output, pparams,bitmap,x,y,true);
}
