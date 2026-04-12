#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <vector>
#include "../include/constant.h"

using namespace std;

void convertYUVtoRGB(
    uint8_t* srcAddr,
    uint8_t* dstAddr,
    int width,
    int height
) {
    int planeSize   = width * height;
    uint8_t* yPlane = srcAddr;
    uint8_t* uPlane = srcAddr + planeSize;
    uint8_t* vPlane = uPlane  + (planeSize / 4);

    auto convertYUVtoRGBPixel = [dstAddr, yPlane, uPlane, vPlane](
        int yIndex, 
        int uIndex,
        int vIndex,
        const float matrix[3][3] 
    ) {
            // Get YUV values
            float Y = yPlane[yIndex];
            float U = uPlane[uIndex] - 128.0f;
            float V = vPlane[vIndex] - 128.0f;
            // Convert YUV to RGB
            float R = matrix[0][0] * Y + matrix[0][1] * U + matrix[0][2] * V;
            float G = matrix[1][0] * Y + matrix[1][1] * U + matrix[1][2] * V;
            float B = matrix[2][0] * Y + matrix[2][1] * U + matrix[2][2] * V;

            dstAddr[yIndex * 3 + 0] = min(max(R, 0.0f), 255.0f);
            dstAddr[yIndex * 3 + 1] = min(max(G, 0.0f), 255.0f);
            dstAddr[yIndex * 3 + 2] = min(max(B, 0.0f), 255.0f);
    };

    for (int yUV = 0; yUV < height/2; yUV++) {
        for (int xUV = 0; xUV < width/2; xUV++) {
            int xStart  = xUV * 2, yStart = yUV * 2;
            int uvIndex = yUV * width/2 + xUV;
            // ------------------------------
            int y00Index = yStart       * width + xStart;
            int y01Index = yStart       * width + xStart + 1;
            int y10Index = (yStart + 1) * width + xStart;
            int y11Index = (yStart + 1) * width + xStart + 1;
            // ------------------------------
            bt709 matrix;
            convertYUVtoRGBPixel(y00Index, uvIndex, uvIndex, matrix.yuv_to_rgb_matrix);
            convertYUVtoRGBPixel(y01Index, uvIndex, uvIndex, matrix.yuv_to_rgb_matrix);
            convertYUVtoRGBPixel(y10Index, uvIndex, uvIndex, matrix.yuv_to_rgb_matrix);
            convertYUVtoRGBPixel(y11Index, uvIndex, uvIndex, matrix.yuv_to_rgb_matrix);
        }
    }
}

void convertYUVSemiPlanartoRGB(
    uint8_t* srcAddr,
    uint8_t* dstAddr,
    int width,
    int height
) {
    int planeSize   = width * height;
    uint8_t* yPlane = srcAddr;
    uint8_t* uvPlane = srcAddr + planeSize;

    auto convertYUVtoRGBPixel = [dstAddr, yPlane, uvPlane](
        int yIndex, 
        int uIndex,
        const float matrix[3][3] 
    ) {
            // Get YUV values
            float Y = yPlane[yIndex];
            float U = uvPlane[uIndex] - 128.0f;
            float V = uvPlane[uIndex + 1] - 128.0f;
            // Convert YUV to RGB
            float R = matrix[0][0] * Y + matrix[0][1] * U + matrix[0][2] * V;
            float G = matrix[1][0] * Y + matrix[1][1] * U + matrix[1][2] * V;
            float B = matrix[2][0] * Y + matrix[2][1] * U + matrix[2][2] * V;

            dstAddr[yIndex * 3 + 0] = min(max(R, 0.0f), 255.0f);
            dstAddr[yIndex * 3 + 1] = min(max(G, 0.0f), 255.0f);
            dstAddr[yIndex * 3 + 2] = min(max(B, 0.0f), 255.0f);
    };

    for (int yUV = 0; yUV < height/2; yUV++) {
        for (int xUV = 0; xUV < width/2; xUV++) {
            int xStart  = xUV * 2, yStart = yUV * 2;
            int uIndex = yUV * width + xUV * 2;
            // ------------------------------
            int y00Index = yStart       * width + xStart;
            int y01Index = yStart       * width + xStart + 1;
            int y10Index = (yStart + 1) * width + xStart;
            int y11Index = (yStart + 1) * width + xStart + 1;
            // ------------------------------
            bt709 matrix;
            convertYUVtoRGBPixel(y00Index, uIndex, matrix.yuv_to_rgb_matrix);
            convertYUVtoRGBPixel(y01Index, uIndex, matrix.yuv_to_rgb_matrix);
            convertYUVtoRGBPixel(y10Index, uIndex, matrix.yuv_to_rgb_matrix);
            convertYUVtoRGBPixel(y11Index, uIndex, matrix.yuv_to_rgb_matrix);
        }
    }
}

void convertRGBtoYUV(
    uint8_t* srcAddr,
    uint8_t* dstAddr,
    int width,
    int height
) {
    int planeSize   = width * height;
    uint8_t* yPlane = dstAddr;
    uint8_t* uPlane = dstAddr + planeSize;
    uint8_t* vPlane = uPlane  + (planeSize / 4);

    auto convertRGBtoYUVPixel = [srcAddr, yPlane, uPlane, vPlane](
        int yIndex, 
        int uIndex,
        int vIndex,
        int rIndex,
        int gIndex,
        int bIndex,
        const float matrix[3][3] 
    ) {
            // Get RGB values
            float R = srcAddr[rIndex];
            float G = srcAddr[gIndex];
            float B = srcAddr[bIndex];
            // Convert RGB to YUV
            float Y = matrix[0][0] * R + matrix[0][1] * G + matrix[0][2] * B;
            float U = matrix[1][0] * R + matrix[1][1] * G + matrix[1][2] * B + 128.0f;
            float V = matrix[2][0] * R + matrix[2][1] * G + matrix[2][2] * B + 128.0f;

            yPlane[yIndex] = min(max(Y, 0.0f), 255.0f);
            uPlane[uIndex] = min(max(U, 0.0f), 255.0f);
            vPlane[vIndex] = min(max(V, 0.0f), 255.0f);
    };
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int yIndex = y * width + x;
            int uIndex = (y/2) * (width/2) + (x/2);
            int vIndex = uIndex;
            // ------------------------------
            int rIndex = yIndex * 3 + 0;
            int gIndex = yIndex * 3 + 1;
            int bIndex = yIndex * 3 + 2;
            // ------------------------------
            bt709 matrix;
            convertRGBtoYUVPixel(yIndex, uIndex, vIndex, rIndex, gIndex, bIndex, matrix.rgb_to_yuv_matrix);
        }
    }
}

void convertRGBtoYUVSemiPlanar(
    uint8_t* srcAddr,
    uint8_t* dstAddr,
    int width,
    int height
) {
    int planeSize   = width * height;
    uint8_t* yPlane = dstAddr;
    uint8_t* uvPlane = dstAddr + planeSize;

    auto convertRGBtoYUVPixel = [srcAddr, yPlane, uvPlane](
        int yIndex, 
        int uIndex,
        int vIndex,
        int rIndex,
        int gIndex,
        int bIndex,
        const float matrix[3][3] 
    ) {
            // Get RGB values
            float R = srcAddr[rIndex];
            float G = srcAddr[gIndex];
            float B = srcAddr[bIndex];
            // Convert RGB to YUV
            float Y = matrix[0][0] * R + matrix[0][1] * G + matrix[0][2] * B;
            float U = matrix[1][0] * R + matrix[1][1] * G + matrix[1][2] * B + 128.0f;
            float V = matrix[2][0] * R + matrix[2][1] * G + matrix[2][2] * B + 128.0f;

            yPlane[yIndex] = min(max(Y, 0.0f), 255.0f);
            uvPlane[uIndex] = min(max(U, 0.0f), 255.0f);
            uvPlane[vIndex] = min(max(V, 0.0f), 255.0f);
    };
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int yIndex = y * width + x;
            int uIndex = (y/2) * width + (x/2) * 2;
            int vIndex = uIndex + 1;
            // ------------------------------
            int rIndex = yIndex * 3 + 0;
            int gIndex = yIndex * 3 + 1;
            int bIndex = yIndex * 3 + 2;
            // ------------------------------
            bt709 matrix;
            convertRGBtoYUVPixel(yIndex, uIndex, vIndex, rIndex, gIndex, bIndex, matrix.rgb_to_yuv_matrix);
        }
    }
}