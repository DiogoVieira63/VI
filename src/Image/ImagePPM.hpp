//
//  ImagePPM.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 09/03/2023.
//

#ifndef ImagePPM_hpp
#define ImagePPM_hpp
#include "image.hpp"
#include <OpenEXR/ImfRgba.h>

class ImagePPM: public Image {
    typedef struct {
        unsigned char val[3];  // r,g,b
    } PPM_pixel;
    PPM_pixel *imageToSave;
    Imf::Rgba* imageToSaveEXR;
    void ToneMap ();
public:
    ImagePPM(const int W, const int H):Image(W, H) {}
    bool Save (std::string filename);
    bool SaveJPG (std::string filename);
    bool SavePFM (std::string filename);
    void EXRAux();
    bool SaveEXR (std::string filename);
};

#endif /* ImagePPM_hpp */
