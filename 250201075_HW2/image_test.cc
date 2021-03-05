// Yaşar Polatlı
// 250201075

#include <cstdlib>
#include <iostream>
#include <vector>

#include "image.hpp"

using std::clog;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

using ceng391::Image;
using ceng391::uchar;

// Ideally these test should return test/fail status. Better option is that using proper
// unit test framework such as gtest.

// You can comment some of these test functions to execute seperately but they can run at the same time as well.

//test scenario for exercise 1:
void test_2d_filtering();
//test scenario for exercise 2:
void test_deriv_x();
//test scenario for exercise 2:
void test_deriv_y();
//test scenario for exercise 3:
void test_warp_affine();

int main(int argc, char** argv)
{
        test_2d_filtering();
        test_deriv_x();
        test_deriv_y();
        test_warp_affine();

        return EXIT_SUCCESS;
}

void test_warp_affine()
{
    Image *img = Image::from_png("../data/small_city.png", true);
    clog << "Loaded " << img->w() << "x" << img->h()
             << " image small_city.png" << endl;
    // transform matrix as an double-precision floating point array
    float A[] = {1, 1, 0, 1};
    // translation vector as an double-precision floating point array
    float t[] = {0, 0};

    // if you dont want to do nearest neighbor sampling you can set this value to false:
    bool bilinear = true;

    Image *Iw = Image::new_gray(2*img->w(), 2*img->h());
        img->warp_affine(Iw, A, t, bilinear);
        string output_filename = "/tmp/small_city_warped.png";
        Iw->save_as_png(output_filename);
        clog << "Saved warped image to " << output_filename << endl;

        delete img;
        delete Iw;
}

void test_deriv_x()
{
    Image *img = Image::from_png("../data/small_city.png", true);

    clog << "Loaded image of size " << img->w()
             << "x" << img->h() << " for test deriv_x. " << endl;

    img->deriv_x();
    delete img;
}

void test_deriv_y()
{
    Image *img = Image::from_png("../data/small_city.png", true);

    clog << "Loaded image of size " << img->w()
             << "x" << img->h() << " for test deriv_y. "<< endl;

    img->deriv_y();
    delete img;
}

void test_2d_filtering()
{
    Image *img = Image::from_png("../data/small_watch.png", true);

    clog << "Loaded image of size " << img->w()
             << "x" << img->h() << " for filtering. " << endl;

    // edge detection matrix: 
    vector<vector<float>> K
    {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };

    Image *img2 = img->filter_2d(3, K);

    img2->save_as_png("/tmp/small_watch_filtered.png");
    clog << "Saved image as /tmp/small_watch_filtered.png" << endl;
    delete img;
    delete img2;
}
