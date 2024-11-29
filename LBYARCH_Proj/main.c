#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

extern void imgCvtGrayFloatToInt(int n, float* img_f32, int* img_u8);

int main() {
    int img_h, img_w;

    scanf_s("%d %d", &img_h, &img_w);

    float* img_f32 = malloc((img_h * img_w) * sizeof(float));
    uint8_t* img_u8 = malloc((img_h * img_w) * sizeof(uint8_t));

    for (int i = 0; i < img_h; i++) {
        for (int j = 0; j < img_w; j++) {
            scanf_s("%f", &img_f32[i * img_w + j]);
        }
    }

    //imgCvtGrayFloatToInt(img_h * img_w, img_f32, img_u8);

    /*for (int i = 0; i < img_h; i++) {
        for (int j = 0; j < img_w; j++) {
            printf("%u ", img_u8[i * img_w + j]);
        }
        printf("\n");
    }*/

    free(img_f32);
    free(img_u8);

    return 0;
}
