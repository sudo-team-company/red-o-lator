__kernel void a_plus_b(__global const int* const a,
                       __global const int* const b,
                       __global int* const c)
{
    *c = *a + *b;
}
