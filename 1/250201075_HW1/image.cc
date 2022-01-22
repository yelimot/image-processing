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

#include "image.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream> 

using std::cerr;
using std::clog;
using std::endl;
using std::exit;
using std::ios;
using std::ofstream;
using std::string;
using std::stringstream;

namespace ceng391 {

Image::Image(int width, int height, int n_channels, int step)
        : m_width(width), m_height(height), m_n_channels(n_channels)
{
        if (step < width * n_channels)
                step = width * n_channels;
        m_step = step;
        m_data = new uchar[m_height * m_step];
}

Image::~Image()
{
        // clog << "Deleting image" << endl;
        delete [] m_data;
}

Image *Image::new_gray(int width, int height)
{
        return new Image(width, height, 1);
}

Image *Image::new_rgb(int width, int height) // Call this function rgb because alpha channel allways will be lost in Netpbm format.
{
        return new Image(width, height, 4); // But I reserved 4 channels anyway.
}

void Image::set_rect(int tlx, int tly, int width, int height, uchar value)
{
        if (tlx < 0) {
                width += tlx;
                tlx = 0;
        }

        if (tly < 0) {
                height += tly;
                tly = 0;
        }
        
        if (m_n_channels == 1) {
                 for (int y = tly; y < tly + height; ++y) {
                if (y >= m_height)
                        break;
                uchar *row = m_data + y * m_step;
                for (int x = tlx; x < tlx + width; ++x) {
                        if (x >= m_width)
                                break;
                        row[x] = value;
                }
            }
        }
        
        else if (m_n_channels == 4 || m_n_channels == 3) { // Necessary update for multichannel images.
            for (int y = tly; y < tly + height; ++y) {
                if (y >= m_height)
                        break;
                uchar *row = m_data + y * m_step;
                for (int x = m_n_channels*tlx; x < m_n_channels*(tlx + width); x+=m_n_channels) {
                        if (x >= m_n_channels * m_width)
                                break;
                        for (int c = 0; c < m_n_channels; ++c) {
                            row[x+c] = value; // Rectangle will be gray because all the channels will be equalized same value.                        
                        }
                }
            }
        }
        
        else { 
            
            cerr << "[ERROR][CENG391::Image] Number of channels does not make sense!\n";
                exit(EXIT_FAILURE);
        }
}

void Image::save_image(const std::string &filename) {
                
                if (m_n_channels == 1) {
                    
                        const string magic_head = "P5";
                        ofstream fout;
                        string extended_name = filename + ".pgm";
                        fout.open(extended_name.c_str(), ios::out | ios::binary);
                        fout << magic_head << "\n";
                        fout << m_width << " " << m_height << " 255\n";
                        for (int y = 0; y < m_height; ++y) {
                                const uchar *row_data = this->data(y);
                                fout.write(reinterpret_cast<const char*>(row_data), m_n_channels*m_width*sizeof(uchar));
                        }
                        fout.close();
                }
                
                else if (m_n_channels == 3) { // If number of channels 3 we can store easier.
                    
                        const string magic_head = "P6";
                        ofstream fout;
                        string extended_name = filename + ".ppm";
                        fout.open(extended_name.c_str(), ios::out | ios::binary);
                        fout << magic_head << "\n";
                        fout << m_width << " " << m_height << " 255\n";
                        for (int y = 0; y < m_height; ++y) {
                                const uchar *row_data = this->data(y);
                                fout.write(reinterpret_cast<const char*>(row_data), m_n_channels*m_width*sizeof(uchar)); // By extend the storage, equivalent to the number of channels.
                        }
                        fout.close();
                }

                else if (m_n_channels == 4) { // If number of channels 4 we need to get rid of alpha channel.

                        const string magic_head = "P6";
                        ofstream fout;
                        string extended_name = filename + ".ppm";
                        fout.open(extended_name.c_str(), ios::out | ios::binary);
                        fout << magic_head << "\n";
                        fout << m_width << " " << m_height << " 255\n";
                        for (int y = 0; y < m_height; ++y) {
                                const uchar *row_data = this->data(y);
                                uchar *rgb_row_data = new uchar[m_width*3]; // Initializing this array of uchars for store only the r, g, b values.
                                int count = 0;
                                for (int i = 0; i < 4 * m_width; ++i) { 
                                        if ((i + 1) % 4 == 0) { // Elimination of alpha channel.
                                                continue;
                                        }
                                        else {
                                                rgb_row_data[count] = row_data[i];
                                                count++;
                                        }
                                }
                                fout.write(reinterpret_cast<char*>(rgb_row_data), 3*m_width*sizeof(uchar)); // For rgb elements, tripling the size is enough.
                        }
                        fout.close();
                }

                else {

                        cerr << "[ERROR][CENG391::Image] Number of channels does not make sense!\n";
                        exit(EXIT_FAILURE);
                }
        }

void Image::to_grayscale() {
    
    if(m_n_channels != 4){ // Control whether the image is already of the target format.
        cerr << "The image is already in grayscale format.\n" << endl;
        exit(EXIT_FAILURE);
    }
    
    else {
        uchar *grayed_data = new uchar[m_height*m_width];
        uchar *rgba_data = this->data();
    
        int count=0; // Need to increment indexing of gray pixels seperately because it will be less.

        for (int i = 0; i < m_width*m_height*m_n_channels; i+=m_n_channels) {
            uchar pixel = 0;
            pixel = rgba_data[i]*0.3 + rgba_data[i+1]*0.59 + rgba_data[i+2]*0.11; // In this state, indexing determines the r, g, b channels.
            if (pixel > 255) { // Check for overflow.
                grayed_data[count] = 255;
            } 
            else if (pixel < 0) { // Check for underflow.
                grayed_data[count] = 0;
            }
            else {
                grayed_data[count] = pixel;
            }
            count++;
        }
        delete [] this->m_data; // Clear the area that will be store new data.
        this->m_data = grayed_data; // Assignment of new data.
        this->m_step = this->w()/4; // Necessary updates for converting to grayscale.
        this->m_n_channels = 1;
        
        clog << "Conversion completed.\n" << endl;
    }
    
}

void Image::to_rgba() {
  
    if(m_n_channels != 1){
        cerr << "The image is already in RGBA format.\n" << endl;
        exit(EXIT_FAILURE);
    }
    else {
        uchar *rgba_data = new uchar[m_height*m_width*4];
        for (int y = 0; y < m_height; ++y){
            uchar *row_data = this->data(y);
            for (int x = 0; x < m_width; ++x) { // Copying same values to all channels. 
                rgba_data[x*4 + 0] = row_data[x];
                rgba_data[x*4 + 1] = row_data[x];
                rgba_data[x*4 + 2] = row_data[x];
                rgba_data[x*4 + 3] = row_data[x];
            }
        }
        delete [] this->m_data; // Clear the area that will be store new data.
        this->m_data = rgba_data; // Assignment of new data.
        this->m_step = this->w()*4; // Necessary updates for converting to grayscale.
        this->m_n_channels = 4;
        
        clog << "Conversion completed.\n" << endl;
    }
}

Image Image::read_pnm(const std::string &filename) {
    
    Image *img; // Initialization of image that we will return after reading complete.
    stringstream ss;
    string line = "";
    std::ifstream ifst(filename.c_str(), ios::out | ios::binary);
    std::getline(ifst, line);
    string magicHead = line;
    int value;
    int height;
    int width;
    ss << ifst.rdbuf();
    ss >> width >> height;
    ss >> value;
    if (magicHead.compare("P6") == 0) { // Checks the format.
        img = new_rgb(width, height); }
    else if (magicHead.compare("P5") == 0) {
        img = new_gray(width, height); }
    else {
        cerr << "Image type not supported.\n" << endl;
        exit(EXIT_FAILURE);
    }
    uchar *data = img->data(); // To store the data that will be read.
    char pixel_data; // Each pixel taking individually.
    for (int y = 0; y < height*width*img->n_ch(); ++y) {
        if (img->n_ch() == 4 && (y+1)%4 == 0) {
            data[y] = (uchar)255;
        }
        else {
            ss >> pixel_data;
            data[y] = (uchar)pixel_data;
        }
    }
        
    ifst.close();
    clog << "Reading successful.\n" << endl;
    return *img;
}

}
