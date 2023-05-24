#include"bmp2yuv.h"
#include<string.h> 

unsigned char* bmp2yuv_down() {
    
    BITMAPFILEHEADER_  fileHeader;
    BITMAPINFOHEADER_  infoHeader;
    RGBQUAD_          quad[256];

    char input_filename[]  = "1288_842.bmp";
    char output_filename[] = "foreground.yuv";

    FILE *input_fp  = fopen(input_filename, "rb");
    FILE *output_fp = fopen(output_filename, "ab+");

    //选择输出文件打开模式，第一次打开模式为wb+，强制覆盖 outputFile。之后为ab+，在内容后面追加
    int n = 0;
    
    int frames  = 1;
    int width  = 640;
    int height = 480;

    int colors = 0;

    
    //memory allocation
    unsigned char *input_buffer  = (unsigned char *)malloc(width * height * 3);
    unsigned char *output_buffer = (unsigned char *)malloc(width * height * 3 / 2);
    
    fread(input_buffer, 1, width * height * 3, input_fp);
    
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            //BMP像素数据的保存顺序为左下到右上，转换成YUV文件时需要调整数据顺序
            int real_j = height - 1 - j;
            int B, G, R;
            //若有调色板，读取调色板RGB值
            if (colors != 0) {
                B = quad[*(input_buffer + width * j + i)].rgbBlue_;
                G = quad[*(input_buffer + width * j + i)].rgbGreen_;
                R = quad[*(input_buffer + width * j + i)].rgbRed_;
            }
            //若没有调色板，直接读取RGB值
            else {
                B = *(input_buffer + (width * j + i) * 3);
                G = *(input_buffer + (width * j + i) * 3 + 1);
                R = *(input_buffer + (width * j + i) * 3 + 2);
            }
            //计算亮度Y
            int Y                                 = 0.299 * R + 0.587 * G + 0.114 * B;
            *(output_buffer + width * real_j + i) = Y;
            //输出YUV图像格式为4:2:0采样，即每4个像素点采集一次色度值
            if (real_j % 2 == 0 && i % 2 == 0) {
                int U = -0.1684 * R - 0.3316 * G + 0.5 * B + 128;
                int V = 0.5 * R - 0.4187 * G - 0.0813 * B + 128;
                *(output_buffer + width * height + width / 2 * real_j / 2 + i / 2)         = U;
                *(output_buffer + width * height * 5 / 4 + width / 2 * real_j / 2 + i / 2) = V;
            }
        }
    }
    //原始图像
    fwrite(output_buffer, 1, width * height * 3 / 2, output_fp);


    //释放内存，关闭文件
    free(input_buffer);
    fclose(input_fp);
    fclose(output_fp);

    return (unsigned char*)output_buffer;
}

int main(){
	char filename[] = "main_output_file.yuv";
	FILE *file = fopen(filename, "wb+");
	unsigned char* buf = bmp2yuv_down();
	fwrite(buf, 1, 640 * 480 * 3 / 2, file);
}
