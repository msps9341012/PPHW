__kernel void histogram(__global unsigned int *image,__global unsigned int *result, unsigned int _size)
{
    const int ix = get_global_id(0);
    int i;
    if(ix<256){
        for(i = 0;i < _size; i+=3){
            if(ix == image[i]){
                result[ix]++;
            }
        }
    }
    if(256 <= ix && ix < 512){
        for(i = 1;i < _size; i+=3){\
            if( (ix-256) == image[i]){
                result[ix]++;
            }
        }
    }
    if(512 <= ix && ix < 768){
        for(i = 2;i < _size; i+=3){
            if((ix-512) == image[i]){
                result[ix]++;
            }
        }
    }
}
