//
//  ImagePPM.cpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#include "ImagePPM.hpp"
#include <iostream>
#include <fstream>
#include <jpeglib.h>

void ImagePPM::ToneMap () {
    imageToSave = new PPM_pixel[W*H];
    
    // loop over each pixel in the image, clamp and convert to byte format
    for (int j = 0 ; j< H ; j++) {
        for (int i = 0; i < W ; ++i) {
            imageToSave[j*W+i].val[0] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].R) * 255);
            imageToSave[j*W+i].val[1] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].G) * 255);
            imageToSave[j*W+i].val[2] = (unsigned char)(std::min(1.f, imagePlane[j*W+i].B) * 255);
        }
    }

}

bool ImagePPM::Save (std::string filename) {
    
    // convert from float to {0,1,..., 255}
    ToneMap();
    printf("ToneMap\n");

    // write imageToSave to file
    if (this->W == 0 || this->H == 0) {
        fprintf(stderr, "Can't save an empty image\n");
        return false;
    }

    std::ofstream ofs;
    try {
        ofs.open(filename, std::ios::binary);
        if (ofs.fail()) throw("Can't open output file");

        ofs << "P6\n" << this->W << " " << this->H << "\n255\n";
        unsigned char r, g, b;

        // loop over each pixel in the image, clamp and convert to byte format
        for (int i = 0; i < this->W * this->H; ++i) {
            r = imageToSave[i].val[0];
            g = imageToSave[i].val[1];
            b = imageToSave[i].val[2];
            ofs << r << g << b;
        }
        ofs.close();
        return true;
    }
    catch (const char *err) {
        fprintf(stderr, "%s\n", err);
        ofs.close();
        return false;
    }

}

bool ImagePPM::SaveJPG (std::string filename) {

    // convert from float to {0,1,..., 255}
    ToneMap();
    printf("ToneMap\n");

    // Initialize the libjpeg structures
    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    FILE* file = fopen(filename.c_str(), "wb");

    if (!file) {
        std::cerr << "Error: Cannot open file for writing: " << filename << std::endl;
        jpeg_destroy_compress(&cinfo);
        return false;
    }

    jpeg_stdio_dest(&cinfo, file);

    cinfo.image_width = this->W;
    cinfo.image_height = this->H;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;

    jpeg_set_defaults(&cinfo);

    // Adjust compression settings if needed
    //jpeg_set_quality(&cinfo, 75, TRUE);

    jpeg_start_compress(&cinfo, TRUE);

    // Allocate a row buffer for libjpeg
    JSAMPROW row_pointer[1];
    int row_stride;

    row_stride = this->W * 3; // 3 components per pixel (RGB)

    // Write the image data to libjpeg
    while (cinfo.next_scanline < cinfo.image_height) {
        row_pointer[0] = (JSAMPLE *)&imageToSave[cinfo.next_scanline * this->W];
        (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(file);

    return true;
}

bool ImagePPM::SavePFM(std::string filename) {
    std::ofstream ofs(filename, std::ios::binary);

    if (!ofs) {
        std::cerr << "Error: Cannot open file for writing: " << filename << std::endl;
        return false;
    }

    ofs << "PF\n";
    ofs << this->W << " " << this->H << "\n";
    ofs << "-1.0\n"; // Negative value indicates Little Endian

    // Write the image data in binary format in reverse order (top-left to bottom-right)
    for (int j = this->H - 1; j >= 0; --j) {
        for (int i = 0; i < this->W; ++i) {
            const RGB &rgb = imagePlane[j * this->W + i];
            ofs.write(reinterpret_cast<const char *>(&rgb.R), sizeof(float));
            ofs.write(reinterpret_cast<const char *>(&rgb.G), sizeof(float));
            ofs.write(reinterpret_cast<const char *>(&rgb.B), sizeof(float));
        }
    }

    ofs.close();
    return true;
}