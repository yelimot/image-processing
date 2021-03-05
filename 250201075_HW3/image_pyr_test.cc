// M. Yaşar Polatlı
// 250201075

// Probably there are bugs in the code.
// I couldn't test and fix it because of the density of my exam and homework schedule, my time didn't enough to accomplish.

#include <cstdlib>
#include <iostream>

#include "image_pyr.hpp"
#include "image.hpp"

using std::cerr;
using std::clog;
using std::endl;
using std::string;

using ceng391::Image;

void print_usage(const char *prog_name);

int main(int argc, char** argv)
{
        if (argc < 3) {
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }

        // first command line argument to read an image
        string filename = argv[1];
        // second command line argument to determine number of levels
        int number_of_levels = atoi(argv[2]);

        Image *img = Image::from_png(filename, true);
        clog << "Loaded " << img->w() << "x" << img->h()
             << " image from " << filename << endl;

        float sigma = 0.5;
        ImagePyr *test = new ImagePyr(number_of_levels, img, sigma);
        test->cons_pyr();
        
        delete img;
        delete test;

        return EXIT_SUCCESS;
}

void print_usage(const char *prog_name)
{
        cerr << prog_name << " <input_image> <filter_length>" << endl;
}