/*
 * blenderFactory.h
 *
 *  Created on: 15-Aug-2022
 *      Author: mohan
 */

#ifndef SRC_WMOPENCVINTERFACE_H_
#define SRC_WMOPENCVINTERFACE_H_

#include "WMInterface.h"
#include "WMOpenCV.h"

class cWMOpenCV {
public:
        static IOverlayBlender *getBlender(BLENDER_TYPE type) {
                if(type == BLENDER_OPENCV) {
                        return new OpencvOverlayBlender(logger::getInstance());
                }
                return nullptr;
        }
};





#endif /* SRC_WMOPENCVINTERFACE_H_ */

