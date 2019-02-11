"// CoarseInsctr.fx:\n"
"// Renders coarse unshadowed inscattering for EVERY epipolar sample and computes extinction.\n"
"// Coarse inscattering is used to refine sampling, while extinction is then transformed to \n"
"// screen space, if extinction evaluation mode is EXTINCTION_EVAL_MODE_EPIPOLAR\n"
"\n"
"#include \"BasicStructures.fxh\"\n"
"#include \"AtmosphereShadersCommon.fxh\"\n"
"\n"
"cbuffer cbParticipatingMediaScatteringParams\n"
"{\n"
"    AirScatteringAttribs g_MediaParams;\n"
"}\n"
"\n"
"cbuffer cbCameraAttribs\n"
"{\n"
"    CameraAttribs g_CameraAttribs;\n"
"}\n"
"\n"
"cbuffer cbLightParams\n"
"{\n"
"    LightAttribs g_LightAttribs;\n"
"}\n"
"\n"
"Texture2D<float2> g_tex2DOccludedNetDensityToAtmTop;\n"
"SamplerState g_tex2DOccludedNetDensityToAtmTop_sampler;\n"
"\n"
"Texture2D<float>  g_tex2DEpipolarCamSpaceZ;\n"
"\n"
"Texture2D<float2> g_tex2DMinMaxLightSpaceDepth;\n"
"\n"
"Texture2DArray<float> g_tex2DLightSpaceDepthMap;\n"
"SamplerComparisonState g_tex2DLightSpaceDepthMap_sampler;\n"
"\n"
"Texture2D<float2> g_tex2DCoordinates;\n"
"\n"
"Texture3D<float3> g_tex3DSingleSctrLUT;\n"
"SamplerState g_tex3DSingleSctrLUT_sampler;\n"
"\n"
"Texture3D<float3> g_tex3DHighOrderSctrLUT;\n"
"SamplerState g_tex3DHighOrderSctrLUT_sampler;\n"
"\n"
"Texture3D<float3> g_tex3DMultipleSctrLUT;\n"
"SamplerState g_tex3DMultipleSctrLUT_sampler;\n"
"\n"
"#include \"LookUpTables.fxh\"\n"
"#include \"ScatteringIntegrals.fxh\"\n"
"#include \"UnshadowedScattering.fxh\"\n"
"\n"
"void ShaderFunctionInternal(in float4 f4Pos,\n"
"                            out float3 f3Inscattering, \n"
"                            out float3 f3Extinction)\n"
"{\n"
"    // Compute unshadowed inscattering from the camera to the ray end point using few steps\n"
"    float fCamSpaceZ =  g_tex2DEpipolarCamSpaceZ.Load( uint3(f4Pos.xy, 0) );\n"
"    float2 f2SampleLocation = g_tex2DCoordinates.Load( uint3(f4Pos.xy, 0) );\n"
"\n"
"    ComputeUnshadowedInscattering(f2SampleLocation, fCamSpaceZ, \n"
"                                  7.0, // Use hard-coded constant here so that compiler can optimize the code\n"
"                                       // more efficiently\n"
"                                  f3Inscattering, f3Extinction);\n"
"    f3Inscattering *= g_LightAttribs.f4ExtraterrestrialSunColor.rgb;\n"
"}\n"
"\n"
"// Render inscattering only\n"
"void RenderCoarseUnshadowedInsctrPS(FullScreenTriangleVSOutput VSOut, \n"
"                                    // IMPORTANT: non-system generated pixel shader input\n"
"                                    // arguments must have the exact same name as vertex shader \n"
"                                    // outputs and must go in the same order.\n"
"                                    // Moreover, even if the shader is not using the argument,\n"
"                                    // it still must be declared.\n"
"\n"
"                                    out float4 f4Inscattering : SV_Target0) \n"
"{\n"
"    float3 f3Extinction = F3ONE;\n"
"    ShaderFunctionInternal(VSOut.f4PixelPos, f4Inscattering.rgb, f3Extinction );\n"
"    f4Inscattering.a = 1.0;\n"
"}\n"
"\n"
"// Render inscattering and extinction\n"
"void RenderCoarseUnshadowedInsctrAndExtinctionPS(FullScreenTriangleVSOutput VSOut,\n"
"                                                 out float4 f4Inscattering : SV_Target0,\n"
"                                                 out float4 f4Extinction   : SV_Target1) \n"
"{\n"
"    ShaderFunctionInternal(VSOut.f4PixelPos, f4Inscattering.rgb, f4Extinction.rgb );\n"
"    f4Inscattering.a = 0.0;\n"
"    f4Extinction.a = 0.0;\n"
"}\n"
