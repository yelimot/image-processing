// ------------------------------
// Written by Mustafa Ozuysal, Yasar Polatli
// Contact <m.yasar.polatli@gmail.com> for comments and bug reports
// ------------------------------
// Copyright (c) 2020, Mustafa Ozuysal, Yasar Polatli
// All rights reserved.
// ------------------------------
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the copyright holders nor the
//       names of his/its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
// ------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ------------------------------

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
