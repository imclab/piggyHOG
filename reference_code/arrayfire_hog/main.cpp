// MODIFIED FROM ARRAYFIRE image_demo.cpp EXAMPLE PROGRAM
#include <stdio.h>
#include <arrayfire.h>
#include <sys/time.h>
#include <vector>
using namespace af;
using namespace std;

double read_timer(){
    struct timeval start;
    gettimeofday( &start, NULL );
    return (double)((start.tv_sec) + 1.0e-6 * (start.tv_usec)); //in seconds
}

//for now, just return the orientations. (TODO: return the magnitudes too ... I may need to pass magnitude and orientation output arrays by reference)
array gradient_builtin(array input){
    array gradX_rgb(input); //deepcopy (is it any faster to preallocate zeros instead of copying?)
    array gradY_rgb(input);

    grad(gradX(span, span, 0), gradY(span, span, 0), input(span, span, 0));

    //array gradX, gradY;
    //grad(gradX, gradY, input);

    //output doesn't look very good. (some sort of ugly shadow effect). 
    // I wonder if this grad() function doesn't understand 3-channel.

    printf("size of gradX: %d, %d, %d\n", gradX.dims(0), gradX.dims(1), gradX.dims(2)); 
    return gradX;
}

array gradient_gfor(array input){
    array gradX, gradY;
    //gradX = constant(n,n,1);

#if 0
    //TODO: gfor loop
    for(int x=0; x<input.dims(1); x++){
        for(int y=0; y<input.dims(0); y++){
            gradX(x, y) = input(x, y, 0); //test -- just yank out first channel
        }
    }
#endif

    return gradX;
}


int main(int argc, char** argv) {
    //deviceset(1);

    try {
        info();
        array input = loadimage("../../images_640x480/carsgraz_001.image.jpg", true); //iscolor='true'
        printf("size of input: %d, %d, %d\n", input.dims(0), input.dims(1), input.dims(2));

    //builtin version
        double start_gradient = read_timer();
        array result_builtin = gradient_builtin(input);
        cudaDeviceSynchronize();
        double time_gradient = read_timer() - start_gradient;
        printf("[builtin] computed gradient in %f ms \n", time_gradient);

        saveimage("./gradient_builtin.jpg", result_builtin);

    //gfor version
        start_gradient = read_timer();
        array result_gfor = gradient_gfor(input);
        cudaDeviceSynchronize();
        time_gradient = read_timer() - start_gradient;
        printf("[gfor] computed gradient in %f ms \n", time_gradient);

        //saveimage("./gradient_gfor.jpg", result_gfor);


    } catch (af::exception& e) {
        fprintf(stderr, "%s\n", e.what());
    }
    return 0;
}
