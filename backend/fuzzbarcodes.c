int fuzzbarcodes(const unsigned char *Data, size_t Size)
{
    if (Size < 20 || Size > 1020)
        return 0;

    FuzzedDataProvider dp(Data, Size);

    int fuzzed_int = dp.ConsumeIntegral<int>();
    int fuzzed_int2 = dp.ConsumeIntegral<int>();
    int fuzzed_int3 = dp.ConsumeIntegral<int>();
    int fuzzed_symbology = dp.PickValueInArray(BARCODES);
    bool fuzzed_bool = dp.ConsumeBool();
    bool fuzzed_bool2 = dp.ConsumeBool();
    bool fuzzed_bool3 = dp.ConsumeBool(); // unused
    int rotate_angle = dp.ConsumeIntegral<int>();
    auto remaining = dp.ConsumeRemainingBytes<unsigned char>();

    if (fuzzed_bool3)
        rotate_angle = (rotate_angle % 4) * 90; // ZBarcode_Print exits if angle is not a multiple of 90

    struct zint_symbol *my_symbol = ZBarcode_Create();
    if (fuzzed_bool)
    {
        my_symbol->symbology = fuzzed_symbology;
    }
    else
    {
        my_symbol->symbology = fuzzed_int;
    }
    if (fuzzed_bool2)
    {
        my_symbol->input_mode = fuzzed_int2;
    }

    ZBarcode_Encode_and_Buffer_Vector(my_symbol, remaining.data(), remaining.size(), rotate_angle);
    ZBarcode_Encode_and_Buffer(my_symbol, remaining.data(), remaining.size(), rotate_angle);
    ZBarcode_Encode_and_Print(my_symbol, remaining.data(), remaining.size(), rotate_angle);
    // ZBarcode_Encode(my_symbol, remaining.data(), remaining.size());
    ZBarcode_Clear(my_symbol);
    ZBarcode_Delete(my_symbol);
    return 0;
}