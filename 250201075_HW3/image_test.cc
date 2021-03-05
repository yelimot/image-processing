// Yaşar Polatlı
// 250201075

#include <cstdlib>
#include <iostream>

#include "image.hpp"

using std::clog;
using std::cerr;
using std::endl;

using ceng391::Image;
using ceng391::uchar;

// Ideally these test should return test/fail status. Better yet use a proper
// unit test framework such as gtest.
void test_grayscale_creation_and_pgm_io();
void test_grayscale_png_load_save();
void test_rgba_creation_and_ppm_io();

int main(int argc, char** argv)
{

        test_grayscale_creation_and_pgm_io();
        test_rgba_creation_and_ppm_io();
        test_grayscale_png_load_save();

        return EXIT_SUCCESS;
}

void test_grayscale_creation_and_pgm_io()
{
        clog << "***** Grayscale Creation and PGM I/O *****" << endl;
        Image *img = Image::new_gray(200, 100);

        clog << "Created new image of size " << img->w()
             << "x" << img->h() << " with " << img->n_ch() << endl;

        img->set(10);
        img->set_rect(50, 50, 10, 30, 255);

        img->save_as_pnm("/tmp/test_image");
        clog << "Saved image as /tmp/test_image.pgm" << endl;

        Image *img2 = Image::from_pnm("/tmp/test_image.pgm");
        clog << "Loaded image of size " << img2->w()
             << "x" << img2->h() << " with " << img2->n_ch() << endl;

        if (img->w() != img2->w() || img->h() != img2->h()
            || img->n_ch() != img2->n_ch()) {
                cerr << "LOADED PNM DIMENSIONS ARE DIFFERENT" << endl;
                exit(EXIT_FAILURE);
        }

        for (int y = 0; y < img2->h(); ++y) {
                const uchar *row = img->data(y);
                const uchar *row2 = img2->data(y);
                for (int x = 0; x < img2->w(); ++x) {
                        if (row[x] != row2[x]) {
                                cerr << "LOADED PNM " << x << ", " << y << " VALUES ARE DIFFERENT" << endl;
                                exit(EXIT_FAILURE);
                        }
                }
        }

        delete img;
        delete img2;
}

void test_rgba_creation_and_ppm_io()
{
        clog << "***** PPM Creation and PPM I/O *****" << endl;
        Image *img = Image::new_rgba(200, 100);

        clog << "Created new image of size " << img->w()
             << "x" << img->h() << " with " << img->n_ch() << endl;

        img->set(10);
        img->set_rect(50, 50, 10, 30, 255, 0, 0, 255);

        img->save_as_pnm("/tmp/test_image_rgba");
        clog << "Saved image as /tmp/test_image_rgba.ppm" << endl;

        Image *img2 = Image::from_pnm("/tmp/test_image_rgba.ppm");
        clog << "Loaded image of size " << img2->w()
             << "x" << img2->h() << " with " << img2->n_ch() << endl;

        if (img->w() != img2->w() || img->h() != img2->h()
            || img->n_ch() != img2->n_ch()) {
                cerr << "LOADED PNM DIMENSIONS ARE DIFFERENT" << endl;
                exit(EXIT_FAILURE);
        }

        for (int y = 0; y < img2->h(); ++y) {
                const uchar *row = img->data(y);
                const uchar *row2 = img2->data(y);
                for (int x = 0; x < img2->w(); ++x) {
                        if (row[4*x] != row2[4*x]
                            || row[4*x+1] != row2[4*x+1]
                            || row[4*x+2] != row2[4*x+2]
                            || row2[4*x+3] != 255) {
                                cerr << "LOADED PNM " << x << ", " << y << " VALUES ARE DIFFERENT" << endl;
                                exit(EXIT_FAILURE);
                        }
                }
        }

        delete img;
        delete img2;
}

void test_grayscale_png_load_save()
{
        clog << "***** Grayscale PNG Load/Save *****" << endl;
        // Ideally we must not hardcode paths in strings like this. They should
        // be taken as command line arguments or read from environment
        // variables.
        Image *img = Image::from_png("../data/small_city.png", true);

        clog << "Loaded image of size " << img->w()
             << "x" << img->h() << " with " << img->n_ch() << endl;

        img->save_as_png("/tmp/small_city_gray.png");
        clog << "Saved image as /tmp/small_city_gray.png" << endl;

        delete img;
}
