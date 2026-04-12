#include <iostream>

struct bt601 {
    const float rgb_to_yuv_matrix[3][3] = {
        { 0.299f,     0.587f,    0.114f},
        {-0.14713f,  -0.28886f,  0.436f},
        { 0.615f,    -0.51499f, -0.10001f}
    };

    const float yuv_to_rgb_matrix[3][3] = {
        {1.0f,  0.0f,       1.13983f},
        {1.0f, -0.39465f,  -0.58060f},
        {1.0f,  2.03211f,   0.0f}
    };
};

struct bt709 {
    const float rgb_to_yuv_matrix[3][3] = {
        { 0.2126f,     0.7152f,    0.0722f},
        {-0.114572f,  -0.385428f,  0.5f},
        { 0.5f,       -0.454153f, -0.045847f}
    };

    const float yuv_to_rgb_matrix[3][3] = {
        {1.0f,  0.0f,       1.5748f},
        {1.0f, -0.187324f, -0.468124f},
        {1.0f,  1.8556f,    0.0f}
    };
};

struct bt2020 {
    const float rgb_to_yuv_matrix[3][3] = {
        { 0.2627f,     0.6780f,    0.0593f},
        {-0.138169f,  -0.360831f,  0.5f},
        { 0.5f,       -0.459786f, -0.040214f}
    };

    const float yuv_to_rgb_matrix[3][3] = {
        {1.0f,  0.0f,       1.4746f},
        {1.0f, -0.164553f, -0.571353f},
        {1.0f,  1.8814f,    0.0f}
    };
};
