/*
You would like to use SIMD in order to transform a 512x256 pixels greyscale image of
parrots you really like. Each pixel of the image is 8 bits. In your output image, you would
like to obtain the negative version of the original image (0 becomes 255, 1 becomes
254, 2 becomes 253, ...) and keep a copy of the original image underneath. In order to
make your code as fast as possible, you decide to use the 256-bits AVX registers (ymm0
to ymm7) instead of the usual 128-bits SSE2 registers (xmm0 to xmm7)
Documentation:
Movdqu source, dest: Moves 256 bits from the source to destination operands
Pmaxsb source, dest: Compare packed signed bytes integers in source and dest and
store packed maximum values in dest.
Psubd source, dest: Substract packed world integers in source from packed world
integers in dest and stores the result in dest
Psubb source, dest: Substract packed byte integers in source from packed byte
integers in dest and stores in dest.

*/

// Write SIMD function, with initializing code in C (ignore opening and closing files etc) only allocate memory for source and destination arrays.
// write either in Intel x86 or AT&T syntax.

int W = 512;
int H = 256;

unsigned char * src = (unsigned char*) malloc(W*H);
unsigned char * dst = (unsigned char *) malloc(W*H);

// another way of declaring offset so that we always use its pointer:
// unsigned char *offset = (unsigned char*) malloc(32* sizeof(unsigned char))

unsigned char offset[32];

for (int i = 0; i < 32; i++)
{
    offset[i] = 255;
}

srcA = src;
srcB = &offset[0];

int ii = (W)*(H)/ 32; // Number of iterations in the loop

__asm__(// intel syntax btw
    mov esi,srcB;
    vmovdqu ymm0,[esi]; // Move the offset into vectorial register to perform substraction after
    mov esi,srcA;
    mov ecx, ii;
    mov edi, dest;
    loop :
    vmovdqu ymm1, [esi];// Move the src into vectorial register to perform operation after
    psubd ymm0 , ymm1; //psubd is for double word (32bits) ints, but we are working with bytes (8bits), so we need to use psubb instead?
    vmovdqu [edi], ymm1; // Store the result in edi
    add edi, 32;
    add esi, 32;
    sub ecx, 1; // counter decreases
    jnz loop; // GOTO instruction
);

// ------------- other solution ? ------------------
// AT&T syntax btw
int width = 256;
int height = 256;
int pixels = width * height;
int n_simd = pixels/16;

unsigned char * src = (unsigned char *) malloc(sizeof(unsigned char) * pixels);
unsigned char * dst = (unsigned char *) malloc(sizeof(unsigned char) * (pixels * 2));// Memory to have the negatif image and just after the original image

unsigned char * max = (unsigned char *) malloc(sizeof(unsigned char) * 16); // 32 for the exam

__asm__(
    "movq %[image_src], %%rax\n"
    "movq %[image_dst_negatif], %%rbx\n"
    "movq %[image_dst_classique], %%rsi\n"
    "movq %[n], %%rcx\n"
    "movq %[max], %%rdx\n"
    "loop: \n"
    "movdqu (%%rdx), %%xmm0\n" // charge the tab of 16 (255 values) in a 128 bits register
    "movdqu (%%rax), %%xmm1\n" // 16 pixels of the base image
    "psubb %%xmm1, %%xmm0\n" // 255 - pixels to have the negative version
    "movdqu %%xmm0, (%%rbx)\n" // Put the negatif version at the start of the new image
    "movdqu %%xmm1, (%%rsi)\n" // Put the classique version at the middle of the new image
    "addq $16, %%rax\n" // Advance the pointeur
    "addq $16, %%rbx\n"
    "addq $16, %%rsi\n"
    "subq $1, %%rcx\n"
    "jnz loop\n"

    :
    : [image_src] "r" (src), [image_dst_negatif] "r" (dst), [image_dst_classique] "r" (dst+pixels), [n] "r" ((unsigned long long) n_simd), [max] "r" (max)
    : "rax", "rbx", "rcx", "rdx", "rsi", "xmm0", "xmm1"
);
