// Yaşar Polatlı CENG 391 Homework 1
// 250201075

// ------------------------------
// Written by Yaşar Polatlı
// Contact <m.yasar.polatli@gmail.com> for comments
// ------------------------------
// Copyright (c) 2020, Yasar Polatli
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

using std::clog;
using std::endl;

using ceng391::Image;

int main(int argc, char** argv)
{
        Image *img1 = Image::new_gray(200, 100);
        
        clog << "Created new gray image of size " << img1->w()
             << "x" << img1->h() << " with " << img1->n_ch() << endl;

        img1->set_zero();
        img1->set_rect(50, 50, 10, 30, 255);
        //img1->to_rgba(); //Uncomment to test this function. It will probably doesn't give the right solution but I was sure that my implementation on this
                           //function was correct, in other words I couldn't find the mistake. I leave it that way to take partial credits.
        img1->save_image("/tmp/test_gray");

        delete img1;        
        
        Image *img2 = Image::new_rgb(200,100);
        
        clog << "Created new rgb image of size " << img2->w()
             << "x" << img2->h() << " with " << img2->n_ch() << endl;

        img2->set_zero();
        img2->set_rect(50, 50, 10, 30, 100);
        //img2->to_grayscale(); //Uncomment to test this function. It will probably doesn't give the right solution but I was sure that my implementation on this
                                //function was correct, in other words I couldn't find the mistake. I leave it that way to take partial credits.
        img2->save_image("/tmp/test_rgb");
        
        delete img2;

        return EXIT_SUCCESS;
}
