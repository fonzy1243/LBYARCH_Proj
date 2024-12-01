# Grayscale Image Float to Integer Converter
This is a simple benchmark of raw a AMD64 assembly function compared to its equivalent C function, and also an optimized C function that uses SIMD.
## Benchmarked Functions
### Assembly Function
The assembly function makes use of SIMD registers from SSE. In particular, it uses XMM registers to process floating-point grayscale image pixel values and convert them into uint8 values. This mapping is calculated based on the equation $\frac{f}{i}=\frac{1}{255}$, where $f$ is the floating-point pixel value and $i$ is the uint8 pixel value.
It is a simple function that loops through each floating-point pixel value and multiplies it by 255, while also casting it to an 8-bit unsigned integer.
### Simple C Function
The simple C function has the same high-level concept as the assembly function; loop through each pixel value and multiplies it by 255, type casting it to the standard 8-bit integer type uint8_t.
### SIMD C Function
The final function tested uses AVX extensions to the x86 ISA. In particular, it uses the AVX2 extension to process packed 256-bit data at once. Rather than sequentially processing each floating-point pixel value one at a time, it processes 32 pixels simultaneously using 4 256-bit vectors.
## Benchmark Results
Tested on the following computer specifications:
- CPU: AMD Ryzen 7 9700X
- RAM: Kingston FURY Beast 6000 MT/S CL36 DDR5
- GPU: ROG Poseidon GTX 1080ti

Test results are validated by checking if all the output arrays are equal. The sample 1440p test image was generated randomly.

Run with Visual Studio 2022 debug mode config:
| Function | Average time | Repetitions | Valid |
| :-------- | :------------: | :-----------: | :-----: |
| AMD64 Assembly | 0.7921 ms | 30 | True |
| Simple C | 2.6968 ms | 30 | True |
| SIMD C | 0.3907 ms | 30 | True |

As seen with these results, the SIMD C function vastly outperformed the other 2 functions. The raw assembly function is not far behind, while the simple C function clearly lags behind. Surprisingly, the raw assembly function is quite fast despite its processing of each element one by one and lack of further optimization. Despite the same high-level concept for both the raw assembly and the simple C functions, the lack of compiler optimizations may be affecting the simple C function's performance. Nonetheless, the benchmark shows how being closer to the metal can have a drastic effect on performance. Additionally, the SIMD function's blazingly fast speed shows the usefulness of the new AVX extensions in applications that benefit a lot from parallelism, including computer graphics.

## Results Screenshots
![image](https://github.com/user-attachments/assets/0fe444ef-0c6a-41cb-9401-2e12221057a5)
![image](https://github.com/user-attachments/assets/e2fdb783-3c75-4351-ae2f-415c83d63e83)
