#ifndef __DATA_H
#define __DATA_H

#include <vector>
#include "stdint.h"
#include "stdio.h"

class Data
{
    std::vector<uint8_t> * featureVector;    // class is not included as part of fv
    uint8_t label;
    int enumeratedLabel;         // in case class labels are char, not necessary here

    public:
    void setFeatureVector (std::vector<uint8_t> *);
    void appendToFeatureVector (uint8_t);
    void setLabel (uint8_t);
    void setEnumeratedLabel (uint8_t);

    int getFeatureVectorSize();
    uint8_t getLabel();
    uint8_t getEnumeratedLabel();

    std::vector<uint8_t> * getFeatureVector();

};

#endif