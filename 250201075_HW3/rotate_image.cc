// Yaşar Polatlı
// 250201075

#include <cstdlib>
#include <iostream>
#include <cmath>

#include "image.hpp"

using std::cerr;
using std::clog;
using std::endl;
using std::string;
using std::max;
using std::min;
using std::cos;
using std::sin;

using ceng391::Image;

void print_usage(const char *prog_name);

const double PI = 3.14159265359;

static inline double min4(double a, double b, double c, double d)
{
        return min(min(a, b), min(c, d));
}

static inline double max4(double a, double b, double c, double d)
{
        return max(max(a, b), max(c, d));
}

// Computes an affine tranformation that corresponds to a rotation by center
// such that the output image fits in the size given by width x height at the
// output. (The resulting A, t is still an Euclidean transform R, t)
static void affine_from_rotate_by_center(int &width, int &height,
                                         double angle, double *A, double *t)
{
        double w = width;
        double h = height;
        double ca = cos(angle);
        double sa = sin(angle);
        double x_min = min4(0, w*ca, -h*sa, w*ca - h*sa);
        double x_max = max4(0, w*ca, -h*sa, w*ca - h*sa);
        double y_min = min4(0, w*sa, h*ca, h*ca + w*sa);
        double y_max = max4(0, w*sa, h*ca, h*ca + w*sa);
        double wp = x_max - x_min;
        double hp = y_max - y_min;

        double hw = w / 2.0;
        double hh = h / 2.0;
        double hwp = wp / 2.0;
        double hhp = hp / 2.0;
        A[0] = ca;
        A[1] = sa;
        A[2] = -sa;
        A[3] = ca;
        t[0] = hwp - ca*hw + sa*hh;
        t[1] = hhp - sa*hw - ca*hh;
        width = wp+0.5;
        height = hp+0.5;
}

int main(int argc, char** argv)
{
        if (argc < 3) {
                print_usage(argv[0]);
                return EXIT_FAILURE;
        }

        string filename = argv[1];
        double angle = atof(argv[2]);
        bool rotate_by_center = true;

        Image *img = Image::from_png(filename, true);
        clog << "Loaded " << img->w() << "x" << img->h()
             << " image from " << filename << endl;
        clog << "Rotating image by " << angle << " degrees " << endl;

        Image *Ir = Image::new_gray(2*img->w(), 2*img->h());
        img->rotated(Ir, angle * PI / 180.0, rotate_by_center);
        string output_filename = "/tmp/output_rotated.png";
        Ir->save_as_png(output_filename);
        clog << "Saved rotated image to " << output_filename << endl;

        int width = img->w();
        int height = img->h();
        double A[4];
        double t[2];
        affine_from_rotate_by_center(width, height, angle * PI / 180.0,
                                     &A[0], &t[0]);
        Image *Ia = Image::new_gray(width, height);
        img->warp_affine(Ia, &A[0], &t[0], true);
        output_filename = "/tmp/output_rotated_by_affine.png";
        Ia->save_as_png(output_filename);
        clog << "Saved affine rotated image to " << output_filename << endl;

        delete img;
        delete Ir;
        delete Ia;

        return EXIT_SUCCESS;
}

void print_usage(const char *prog_name)
{
        cerr << prog_name << " <input_image> <angle_in_degrees>" << endl;
}

/// Local Variables:
/// mode: c++
/// compile-command: "make rotate-image"
/// End:
