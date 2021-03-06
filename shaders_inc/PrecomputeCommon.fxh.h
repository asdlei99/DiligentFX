"\n"
"\n"
"float4 LUTCoordsFromThreadID( uint3 ThreadId )\n"
"{\n"
"    float4 f4Corrds;\n"
"    f4Corrds.xy = (float2( ThreadId.xy ) + float2(0.5,0.5) ) / PRECOMPUTED_SCTR_LUT_DIM.xy;\n"
"\n"
"    uint uiW = ThreadId.z % uint(PRECOMPUTED_SCTR_LUT_DIM.z);\n"
"    uint uiQ = ThreadId.z / uint(PRECOMPUTED_SCTR_LUT_DIM.z);\n"
"    f4Corrds.zw = ( float2(uiW, uiQ) + float2(0.5,0.5) ) / PRECOMPUTED_SCTR_LUT_DIM.zw;\n"
"    return f4Corrds;\n"
"}\n"
