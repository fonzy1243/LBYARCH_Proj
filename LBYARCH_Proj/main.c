#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <windows.h>
#include <immintrin.h>
#include <stdbool.h>

double get_time() {
    LARGE_INTEGER freq;
    LARGE_INTEGER start;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    return (double)start.QuadPart / (double)freq.QuadPart;
}

extern void imgCvtGrayFloatToInt(int n, float* img_f32, uint8_t* img_u8);

void cImgCvtGrayFloatToInv(int n, float* img_f32, uint8_t* img_u8) {
    // Sequential for loop version
    /*
    for (int i = 0; i < n; i++) {
        img_u8[i] = (uint8_t) (img_f32[i] * 255);
    }
    */

    // SIMD version

    int i = 0;
    __m256 scale_vec = _mm256_set1_ps(255.0f);
    
    for (; i + 31 < n; i += 32) {
        __m256 float_vec1 = _mm256_loadu_ps(img_f32 + i);
        __m256 float_vec2 = _mm256_loadu_ps(img_f32 + i + 8);
        __m256 float_vec3 = _mm256_loadu_ps(img_f32 + i + 16);
        __m256 float_vec4 = _mm256_loadu_ps(img_f32 + i + 24);

        __m256i int_vec1 = _mm256_cvttps_epi32(_mm256_mul_ps(float_vec1, scale_vec));
        __m256i int_vec2 = _mm256_cvttps_epi32(_mm256_mul_ps(float_vec2, scale_vec));
        __m256i int_vec3 = _mm256_cvttps_epi32(_mm256_mul_ps(float_vec3, scale_vec));
        __m256i int_vec4 = _mm256_cvttps_epi32(_mm256_mul_ps(float_vec4, scale_vec));

        __m256i pack12 = _mm256_packus_epi32(int_vec1, int_vec2);
        __m256i pack34 = _mm256_packus_epi32(int_vec3, int_vec4);
        
        __m256i pack8bit = _mm256_packus_epi16(pack12, pack34);

        _mm256_storeu_si256((__m256i*)(img_u8 + i), pack8bit);
    }
    
    // Loop for the remaining elements
    for (; i < n; i++) {
        img_u8[i] = (uint8_t)(img_f32[i] * 255);
    }
}

bool is_out_equal(int n, uint8_t* img1, uint8_t* img2) {
    for (int i = 0; i < n; i++) {
        if (img1[n] != img2[n]) {
            return false;
        }
    }

    return true;
}

int main() {
    int img_h, img_w;
    double start_time, end_time, asm_avg, c_avg;
    scanf_s("%d %d", &img_h, &img_w);

    float* img_f32 = malloc((img_h * img_w) * sizeof(float));
    uint8_t* img_u8 = malloc((img_h * img_w) * sizeof(uint8_t));
    uint8_t* c_img_u8 = malloc((img_h * img_w) * sizeof(uint8_t));

    for (int i = 0; i < img_h; i++) {
        for (int j = 0; j < img_w; j++) {
            scanf_s("%f", &img_f32[i * img_w + j]);
            img_u8[i * img_w + j] = 0;
            c_img_u8[i * img_w + j] = 0;
        }
    }

    double total = 0;

    /* Benchmark ASM code */
    for (int i = 0; i < 30; i++) {
        start_time = get_time();
        imgCvtGrayFloatToInt(img_h * img_w, img_f32, img_u8);
        end_time = get_time();
        total += (end_time - start_time);
    }

    asm_avg = total / 30;
    total = 0;

    /* Benchmark C code */
    for (int i = 0; i < 30; i++) {
        start_time = get_time();
        cImgCvtGrayFloatToInv(img_h * img_w, img_f32, c_img_u8);
        end_time = get_time();

        total += (end_time - start_time);
    }

    c_avg = total / 30;

    printf("\n==================================================\n");
    printf("Average AMD64 Assembly execution time (30 runs):\n");
    printf("%.16f ms\n", asm_avg * 1e3);
    printf("\nAverage C execution time (30 runs):\n");
    printf("%.16f ms\n", c_avg * 1e3);
    printf("==================================================\n");

    printf("Valid result: ");
    if (is_out_equal(img_h * img_w, img_u8, c_img_u8)) {
        printf("true\n");
    }
    else {
        printf("false\n");
    }

    printf("\nASM Result:\n");
    for (int i = 0; i < img_h; i++) {
        for (int j = 0; j < img_w; j++) {
            printf("%d ", img_u8[i * img_w + j]);
        }

        printf("\n");
    }

    printf("\nC Result:\n");
    for (int i = 0; i < img_h; i++) {
        for (int j = 0; j < img_w; j++) {
            printf("%d ", img_u8[i * img_w + j]);
        }

        printf("\n");
    }

    free(img_f32);
    free(img_u8);
    free(c_img_u8);

    return 0;
}
