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
    /* Tbarcode 8 codes */
BARCODE_TELEPEN_NUM,//     87
BARCODE_ITF14,//           89
BARCODE_KIX,//             90
BARCODE_AZTEC,//           92
BARCODE_DAFT,//            93
BARCODE_MICROQR,//         97

    /* Tbarcode 9 codes */
BARCODE_HIBC_128,//        98
BARCODE_HIBC_39,//         99
BARCODE_HIBC_DM,//         102
BARCODE_HIBC_QR,//         104
BARCODE_HIBC_PDF,//        106
BARCODE_HIBC_MICPDF,//     108
BARCODE_HIBC_BLOCKF,//     110
BARCODE_HIBC_AZTEC,//      112

    /* Tbarcode 10 codes */
BARCODE_DOTCODE,//         115
BARCODE_HANXIN,//          116

    /*Tbarcode 11 codes*/
BARCODE_MAILMARK,//        121

};

#include "fuzzbarcodes.c" 

extern "C" int FUZZ(const unsigned char *Data, size_t Size)
{
  return fuzzbarcodes(Data, Size);
}
