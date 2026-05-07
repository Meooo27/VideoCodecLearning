#include "../include/main.h"
#include "../include/mse_psnr.h"

int main() {
    int w = 480, h = 856;

    auto convert = [](
        std::string inputFileName, 
        std::string outputFileName, 
        int w, 
        int h,
        std::function<void(uint8_t*, uint8_t*, int, int)> convertSrcToDst,
        bool isSrcYUV = true
    ) {
        ifstream inputFile(inputFileName, ios::binary);
        ofstream outputFile(outputFileName, ios::binary);


        int yuvSize = w * h * 1.5;
        int rgbSize = w * h * 3;

        while (!inputFile.eof()) {
            printf("Processing frame...\n");
            uint8_t* yuvData = new uint8_t[yuvSize];
            uint8_t* rgbData = new uint8_t[rgbSize];

            if (isSrcYUV) {
                inputFile.read(reinterpret_cast<char*>(yuvData), yuvSize);
                if (inputFile.gcount() == 0) break;
                convertSrcToDst(yuvData, rgbData, w, h);
                outputFile.write(reinterpret_cast<char*>(rgbData), rgbSize);
            } else {
                inputFile.read(reinterpret_cast<char*>(rgbData), rgbSize);
                if (inputFile.gcount() == 0) break;
                convertSrcToDst(rgbData, yuvData, w, h);
                outputFile.write(reinterpret_cast<char*>(yuvData), yuvSize);
            }

            delete[] yuvData;
            delete[] rgbData;
        }
        inputFile.close();
        outputFile.close();
    };

    convert("input_480x856_yuv420p.yuv", "output_480x856_RGB888.yuv", w, h, convertYUVtoRGB);  
    convert("input_480x856_nv12.yuv", "output_nv12_480x856_RGB888.yuv", w, h, convertYUVSemiPlanartoRGB);

    convert("output_480x856_RGB888.yuv", "output_480x856_yuv420p.yuv", w, h, convertRGBtoYUV, false);   
    convert("output_nv12_480x856_RGB888.yuv", "output_rgb888_480x856_nv12.yuv", w, h, convertRGBtoYUVSemiPlanar, false);   

    calculateMSEAndPSNR(
        ifstream("input_480x856_yuv420p.yuv", ios::binary), 
        ifstream("output_480x856_yuv420p.yuv", ios::binary), 
        w, h, 8
    );

    return 0;
}