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

#include "image_pyr.hpp"

#include <cmath>
#include <iostream>
#include <cstring>
#include <cstdlib>

using std::memcpy;
using std::free;
using std::malloc;
using std::string;
using std::to_string;

namespace ceng391 {

// the constructor:
ImagePyr::ImagePyr(int m_n_levels, Image *img, float sigma)
		: m_n_levels(m_n_levels), sigma(sigma)
{
	m_levels = (*Image)malloc(sizeof(Image)*m_n_levels);

	// Level 0 is a copy of the base image given in the constructor (do not
	// just copy the pointer, create a new image that is a copy of the base
	// image).
	Image *base = new (&m_levels[0]) Image(img->w(), img->h(), 1);

	uchar *basedata = img->data();
	uchar *copydata =  base->data();

	memcpy(basedata, copydata, img->w()*img->h()*sizeof(uchar));
}

// the destructor:
ImagePyr::~ImagePyr()
{	
	// to clear memory blocks one by one
	for(int i = 0; i < m_n_levels; i++){
		delete m_levels[i];
	}

	// to clear the object array from the memory
	free(m_levels);
}

/* Level i is created by first Gaussian smoothing the image at level i − 1
so that its scale (sigma value) is doubled. And then it is downsampled
by two in x and y dimensions so that its width and height are the half
of the width and height of the image at level i − 1 (Downsample by
throwing out every other row and column of the smoothed image).*/

// construction of the pyramid handled by this function:
void ImagePyr::cons_pyr()
{
	// pyramid will constructed after this iteration, in each step downsampled image will saved to tmp directory.
	for(int i = 0; i < m_n_levels; i++) {
		// Level i taking into level_i
		Image level_i = m_levels[i]; 
		
		// level i copying to temp to avoid over smoothing
		Image *temp = new Image(level_i->w(), level_i->h(), 1);
		uchar *basedata = level_i->data();
		uchar *copydata = temp->data();
		memcpy(basedata, copydata, img->w()*img->h()*sizeof(uchar));
		
		// gaussian smooth with the function that Mr.Ozuysal has wrote in the class
		temp->smooth(sigma);

		// downsampling by throwing every other row and column of the smoothed image
		Image *downsampled = new (&m_levels[i]) Image(level_i->w()/2, level_i->h()/2, 1);
		for (int y = 0; y < downsampled->h(); ++y) {
                uchar *row_downsampled = downsampled->data(y);
                uchar *row = temp->data(2*y);
                for (int x = 0; x < downsampled->w(); ++x) {
		            row_downsampled[x] = row[2*x];
                }
    	}
    	delete temp;

    	// adding downsampled images to the m_level object array one by one
    	m_levels[i+1] = *downsampled;

    	// casting to give output name of the saved file correctly
    	string s = to_string(i)

    	// saving operation also handled by this function
    	string output_filename = "/tmp/pyr_level_" + s + ".png";
        downsampled->save_as_png(output_filename);

	}

}

}

