// Yaşar Polatlı
// 250201075

#include <cstdlib>
#include <iostream>

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

        string filename = argv[1];
        int filter_size = atoi(argv[2]);

        Image *img = Image::from_png(filename, true);
        clog << "Loaded " << img->w() << "x" << img->h()
             << " image from " << filename << endl;
        clog << "Filtering image with a box filter of size "
             << filter_size << endl;
        img->box_filter_x(filter_size);

        string output_filename = "/tmp/output_box_x.png";
        img->save_as_png(output_filename);
        clog << "Saved box filtered in x to output to " << output_filename << endl;

        img->load_png(filename, true);
        img->box_filter_y(filter_size);
        output_filename = "/tmp/output_box_y.png";
        img->save_as_png(output_filename);
        clog << "Saved box filtered in y to output to " << output_filename << endl;

        img->load_png(filename, true);
        img->box_filter(filter_size);
        output_filename = "/tmp/output_box.png";
        img->save_as_png(output_filename);
        clog << "Saved box filtered in x&y to output to " << output_filename << endl;

        img->load_png(filename, true);
        float *K = new float[filter_size * filter_size];
        for (int c = 0; c < filter_size; ++c) {
                for (int r = 0; r < filter_size; ++r) {
                        K[c * filter_size + r] = 1.0f / (filter_size * filter_size);
                }
        }
        Image * out_img = img->filter_2d(filter_size, K);
        output_filename = "/tmp/output_filter_2d.png";
        out_img->save_as_png(output_filename);
        clog << "Saved 2d filtered in x&y to output to " << output_filename << endl;
        delete [] K;
        delete out_img;

        float sigma = (filter_size / 2) / 3.0f;
        img->load_png(filename, true);
        img->smooth_x(sigma);
        output_filename = "/tmp/output_smooth_x.png";
        img->save_as_png(output_filename);
        clog << "Saved smoothed in x to output to " << output_filename << endl;

        img->load_png(filename, true);
        img->smooth_y(sigma);
        output_filename = "/tmp/output_smooth_y.png";
        img->save_as_png(output_filename);
        clog << "Saved smoothed in y to output to " << output_filename << endl;

        img->load_png(filename, true);
        img->smooth(sigma);
        output_filename = "/tmp/output_smooth.png";
        img->save_as_png(output_filename);
        clog << "Saved smoothed in x&y to output to " << output_filename << endl;

        short *dx = img->deriv_x();
        short *dy = img->deriv_y();
        out_img = Image::from_deriv(img->w(), img->h(), dx);
        output_filename = "/tmp/output_dx.png";
        out_img->save_as_png(output_filename);
        clog << "Saved x derivative to " << output_filename << endl;
        delete out_img;
        out_img = Image::from_deriv(img->w(), img->h(), dy);
        output_filename = "/tmp/output_dy.png";
        out_img->save_as_png(output_filename);
        clog << "Saved y derivative to " << output_filename << endl;
        delete out_img;
        delete [] dx;
        delete [] dy;

        delete img;

        return EXIT_SUCCESS;
}

void print_usage(const char *prog_name)
{
        cerr << prog_name << " <input_image> <filter_length>" << endl;
}

/// Local Variables:
/// mode: c++
/// compile-command: "make filter-image"
/// End:
