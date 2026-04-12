cd /Volumes/D/Projects/VSCode/RawVideoConverter/src
clang++ main.cpp -std=c++17 -o main && ./main
rm -rf main
ffmpeg -y -f rawvideo -pixel_format rgb24 -video_size 480x856 -framerate 30 -i output_480x856_RGB888.yuv output_480x856_RGB888.mp4
ffmpeg -y -f rawvideo -pixel_format rgb24 -video_size 480x856 -framerate 30 -i output_nv12_480x856_RGB888.yuv output_nv12_480x856_RGB888.mp4
ffmpeg -y -f rawvideo -pixel_format yuv420p -video_size 480x856 -framerate 30 -i output_480x856_yuv420p.yuv output_480x856_yuv420p.mp4
ffmpeg -y -f rawvideo -pixel_format nv12 -video_size 480x856 -framerate 30 -i output_rgb888_480x856_nv12.yuv output_rgb888_480x856_nv12.mp4