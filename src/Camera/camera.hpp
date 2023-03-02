//
//  camera.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 10/02/2023.
//

#ifndef camera_hpp
#define camera_hpp

#include "image.hpp"

// based on pbrt book, sec 6.1, pag. 356
class Camera {
protected:
    Image *img;
public:
    Camera () {img=NULL;}
    ~Camera() {}
    bool GenerateRay() {return false;};
    bool SaveImage() {
        if (img != NULL) return img->Save("VI-RT.img");
        return false;
    }
};

#endif /* camera_hpp */
