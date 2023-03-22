//
//  Scene.hpp
//  VI-RT
//
//  Created by Luis Paulo Santos on 30/01/2023.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <iostream>
#include <string>
#include <vector>
#include "primitive.hpp"
#include "light.hpp"
#include "ray.hpp"
#include "intersection.hpp"
#include "BRDF.hpp"
#include "Phong.hpp"

class Scene {
    std::vector <Primitive> prims;
    std::vector <BRDF> BRDFs;
public:
    std::vector <Light> lights;
    int numPrimitives, numLights, numBRDFs;

    Scene (): numPrimitives(0), numLights(0), numBRDFs(0) {}
    bool Load (const std::string &fname);
    bool SetLights (void) { return true; };
    bool trace (Ray r, Intersection *isect);
    void printSummary(void) {
        std::cout << "#primitives = " << numPrimitives << " ; ";
        std::cout << "#lights = " << numLights << " ; ";
        std::cout << "#materials = " << numBRDFs << " ;" << std::endl;

        // print materials
        for (int i=0; i<numBRDFs; i++) {
            std::cout << "Material " << i << " : ";
            //convert to Phong and print values
            auto phong = (Phong*) &BRDFs[i];
            if (phong) {
                std::cout << "Phong : \n";
                //print Ka RGB
                std::cout << "Ka = " << phong->Ka.R << " " << phong->Ka.G << " " << phong->Ka.B << " ; \n";
                //print Kd RGB
                std::cout << "Kd = " << phong->Kd.R << " " << phong->Kd.G << " " << phong->Kd.B << " ; \n";
                //print Ks RGB
                std::cout << "Ks = " << phong->Ks.R << " " << phong->Ks.G << " " << phong->Ks.B << " ; \n";
            }
            else {
                std::cout << "Unknown material type" << std::endl;
            }

        }

    }
};

#endif /* Scene_hpp */
