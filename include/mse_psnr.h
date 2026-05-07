#include <iostream>
#include <fstream>

using namespace std;

/* 
MSE (Mean Squared Error - Sai số toàn phương trung bình)
là chỉ số đánh giá độ chính xác của mô hình hồi quy (machine learning),
đo trung bình bình phương sai lệch giữa giá trị dự báo và thực tế. 
MSE càng gần 0 càng tốt, luôn không âm, và đặc biệt nhạy với các 
sai số lớn (outlier) do bình phương sai số.
*/
double calculateMSE(
    const uint8_t* org_matrix, 
    const uint8_t* comp_matrix, 
    int size
) {
    long long sum = 0;
    for (int i = 0; i < size; i++) {
        int diff = org_matrix[i] - comp_matrix[i];
        sum += (diff * diff);
    }
    double mse = static_cast<double>(sum) / size;
    return mse;
}

/*
PSNR (Peak Signal-to-Noise Ratio - Tỷ số tín hiệu cực đại trên nhiễu)
là thước đo kỹ thuật dùng để đánh giá chất lượng hình ảnh/video 
sau khi nén hoặc xử lý, so với ảnh gốc. PSNR càng cao chứng tỏ 
ảnh được tái tạo càng chất lượng, ít nhiễu. Giá trị này thường 
được tính bằng dB, dựa trên sai số bình phương trung bình (MSE) 
giữa ảnh gốc và ảnh xử lý.
*/
double calculatePSNR(
    double mse, 
    int bitsPerPixel
) {
    if (mse == 0) {
        return INFINITY;
    }
    double max_bitPerPixel = pow(2, bitsPerPixel) - 1;
    double psnr = 10 * log10((max_bitPerPixel * max_bitPerPixel) / mse);
    return psnr;
}

bool readFrame(
    std::ifstream& file,
    std::vector<uint8_t>& buffer,
    size_t frameSize,
    int frameIndex
) {
    file.seekg(static_cast<std::streamoff>(frameIndex) * frameSize);

    if (!file.read(reinterpret_cast<char*>(buffer.data()), frameSize)) {
        return false;
    }

    return true;
}

void calculateMSEAndPSNR(
    ifstream org_file, 
    ifstream comp_file, 
    int width, 
    int height, 
    int bitsPerPixel
) {
    if (!org_file.is_open() || !comp_file.is_open()) {
        std::cerr << "Cannot open input files\n";
        return;
    }

    int frameIndex = 0;
    const size_t ySize = width * height;
    const size_t uvSize = (width / 2) * (height / 2);
    const size_t frameSize = ySize + uvSize * 2;

    for (int i = 0; i < 5; i++) {
        std::vector<uint8_t> frameA(frameSize);
        std::vector<uint8_t> frameB(frameSize);

        if (!readFrame(org_file, frameA, frameSize, frameIndex) ||
            !readFrame(comp_file, frameB, frameSize, frameIndex)) {
            std::cerr << "Cannot read frame\n";
            return;
        }

        // ===== Y =====
        const uint8_t* yA = frameA.data();
        const uint8_t* yB = frameB.data();

        double mseY = calculateMSE(yA, yB, ySize);
        double psnrY = calculatePSNR(mseY, 8);

        // ===== U =====
        const uint8_t* uA = frameA.data() + ySize;
        const uint8_t* uB = frameB.data() + ySize;

        double mseU = calculateMSE(uA, uB, uvSize);
        double psnrU = calculatePSNR(mseU, 8);

        // ===== V =====
        const uint8_t* vA = frameA.data() + ySize + uvSize;
        const uint8_t* vB = frameB.data() + ySize + uvSize;

        double mseV = calculateMSE(vA, vB, uvSize);
        double psnrV = calculatePSNR(mseV, 8);

        std::cout << "===== RESULT FRAME " << frameIndex << " =====\n";

        std::cout << "Y MSE  : " << mseY << "\n";
        std::cout << "Y PSNR : " << psnrY << " dB\n\n";

        std::cout << "U MSE  : " << mseU << "\n";
        std::cout << "U PSNR : " << psnrU << " dB\n\n";

        std::cout << "V MSE  : " << mseV << "\n";
        std::cout << "V PSNR : " << psnrV << " dB\n";

        frameIndex++;
    }
    
    org_file.close();
    comp_file.close();
}