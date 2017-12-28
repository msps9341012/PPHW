__kernel void histogram(__global unsigned int *img,__global unsigned int *result, unsigned int _size)
{
    const int idx = get_global_id(0);
    int i;
    if(idx<256){
        for(i = 0;i < _size; i+=3){
            if(idx == img[i]){
                result[idx]++;
            }
        }
    }
    if(256 <= idx && idx < 512){
        for(i = 1;i < _size; i+=3){\
            if( (idx-256) == img[i]){
                result[idx]++;
            }
        }
    }
    if(512 <= idx && idx < 768){
        for(i = 2;i < _size; i+=3){
            if((idx-512) == img[i]){
                result[idx]++;
            }
        }
    }
}
