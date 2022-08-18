#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <cstdint>
#include <iostream>
#include "common.h"
#include "gs1.h"
#include <unistd.h>

#include <fuzzer/FuzzedDataProvider.h>

int ZBarcode_Buffer(struct zint_symbol *symbol, int rotate_angle);

const int BARCODES[] = {
    /* Zint specific */
BARCODE_AZRUNE,//          128
BARCODE_CODE32,//          129
BARCODE_EANX_CC,//         130
BARCODE_EAN128_CC,//       131
BARCODE_RSS14_CC,//        132
BARCODE_RSS_LTD_CC,//      133
BARCODE_RSS_EXP_CC,//      134
BARCODE_UPCA_CC,//         135
BARCODE_UPCE_CC,//         136
BARCODE_RSS14STACK_CC,//   137
BARCODE_RSS14_OMNI_CC,//   138
BARCODE_RSS_EXPSTACK_CC,// 139
BARCODE_CHANNEL,//         140
BARCODE_CODEONE,//         141
BARCODE_GRIDMATRIX,//      142
BARCODE_UPNQR,//           143
BARCODE_ULTRA,//           144
BARCODE_RMQR,//            145
};


#include "fuzzbarcodes.c" 

extern "C" int FUZZ(const unsigned char *Data, size_t Size)
{
  return fuzzbarcodes(Data, Size);
}
