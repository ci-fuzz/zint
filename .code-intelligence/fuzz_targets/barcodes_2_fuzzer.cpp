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

BARCODE_POSTNET,//         40
BARCODE_MSI_PLESSEY,//     47
BARCODE_FIM,//             49
BARCODE_LOGMARS,//         50
BARCODE_PHARMA,//          51
BARCODE_PZN,//             52
BARCODE_PHARMA_TWO,//      53
BARCODE_PDF417,//          55
BARCODE_PDF417TRUNC,//     56
BARCODE_MAXICODE,//        57
BARCODE_QRCODE,//          58
BARCODE_CODE128B,//        60
BARCODE_AUSPOST,//         63
BARCODE_AUSREPLY,//        66
BARCODE_AUSROUTE,//        67
BARCODE_AUSREDIRECT,//     68
BARCODE_ISBNX,//           69
BARCODE_RM4SCC,//          70
BARCODE_DATAMATRIX,//      71
BARCODE_EAN14,//           72
BARCODE_VIN,//             73
BARCODE_CODABLOCKF,//      74
BARCODE_NVE18,//           75
BARCODE_JAPANPOST,//       76
BARCODE_KOREAPOST,//       77
BARCODE_RSS14STACK,//      79
BARCODE_RSS14STACK_OMNI,// 80
BARCODE_RSS_EXPSTACK,//    81
BARCODE_PLANET,//          82
BARCODE_MICROPDF417,//     84
BARCODE_ONECODE,//         85
BARCODE_PLESSEY,//         86

};


#include "fuzzbarcodes.c" 

extern "C" int FUZZ(const unsigned char *Data, size_t Size)
{
  return fuzzbarcodes(Data, Size);
}
