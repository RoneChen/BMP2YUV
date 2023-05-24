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

    //ѡ������ļ���ģʽ����һ�δ�ģʽΪwb+��ǿ�Ƹ��� outputFile��֮��Ϊab+�������ݺ���׷��
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
            //BMP�������ݵı���˳��Ϊ���µ����ϣ�ת����YUV�ļ�ʱ��Ҫ��������˳��
            int real_j = height - 1 - j;
            int B, G, R;
            //���е�ɫ�壬��ȡ��ɫ��RGBֵ
            if (colors != 0) {
                B = quad[*(input_buffer + width * j + i)].rgbBlue_;
                G = quad[*(input_buffer + width * j + i)].rgbGreen_;
                R = quad[*(input_buffer + width * j + i)].rgbRed_;
            }
            //��û�е�ɫ�壬ֱ�Ӷ�ȡRGBֵ
            else {
                B = *(input_buffer + (width * j + i) * 3);
                G = *(input_buffer + (width * j + i) * 3 + 1);
                R = *(input_buffer + (width * j + i) * 3 + 2);
            }
            //��������Y
            int Y                                 = 0.299 * R + 0.587 * G + 0.114 * B;
            *(output_buffer + width * real_j + i) = Y;
            //���YUVͼ���ʽΪ4:2:0��������ÿ4�����ص�ɼ�һ��ɫ��ֵ
            if (real_j % 2 == 0 && i % 2 == 0) {
                int U = -0.1684 * R - 0.3316 * G + 0.5 * B + 128;
                int V = 0.5 * R - 0.4187 * G - 0.0813 * B + 128;
                *(output_buffer + width * height + width / 2 * real_j / 2 + i / 2)         = U;
                *(output_buffer + width * height * 5 / 4 + width / 2 * real_j / 2 + i / 2) = V;
            }
        }
    }
    //ԭʼͼ��
    fwrite(output_buffer, 1, width * height * 3 / 2, output_fp);


    //�ͷ��ڴ棬�ر��ļ�
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
