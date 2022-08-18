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
BARCODE_CODE11,//          1
BARCODE_C25MATRIX,//       2
BARCODE_C25INTER,//        3
BARCODE_C25IATA,//         4
BARCODE_C25LOGIC,//        6
BARCODE_C25IND,//          7
BARCODE_CODE39,//          8
BARCODE_EXCODE39,//        9
BARCODE_EANX,//            13
BARCODE_EANX_CHK,//        14
BARCODE_EAN128,//          16
BARCODE_CODABAR,//         18
BARCODE_CODE128,//         20
BARCODE_DPLEIT,//          21
BARCODE_DPIDENT,//         22
BARCODE_CODE16K,//         23
BARCODE_CODE49,//          24
BARCODE_CODE93,//          25
BARCODE_FLAT,//            28
BARCODE_RSS14,//           29
BARCODE_RSS_LTD,//         30
BARCODE_RSS_EXP,//         31
BARCODE_TELEPEN,//         32
BARCODE_UPCA,//            34
BARCODE_UPCA_CHK,//        35
BARCODE_UPCE,//            37
BARCODE_UPCE_CHK,//        38

};

#include "fuzzbarcodes.c" 

extern "C" int FUZZ(const unsigned char *Data, size_t Size)
{
  return fuzzbarcodes(Data, Size);
}
