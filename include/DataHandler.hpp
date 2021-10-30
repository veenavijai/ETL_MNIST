#ifndef __DATA_HANDLER_H
#define __DATA_HANDLER_H

#include "fstream"
#include "stdint.h"
#include "data.hpp"
#include <vector>
#include <string>
#include <map>
#include <unordered_set>

class DataHandler
{
    std::vector<Data *> * dataArray;     // all the data, pre-split
    std::vector<Data *> * trainingData;
    std::vector<Data *> * testData;
    std::vector<Data *> * validationData;

    int class_counts;
    int featureVectorSize;
    std::map<uint8_t, int> classFromInt;

    public:

    // set the splits
    const double TRAIN_SET_PERCENT = 0.75;
    const double TEST_SET_PERCENT = 0.20;
    const double VALIDATION_SET_PERCENT = 0.05;

    DataHandler();
    ~DataHandler();

    void readInputData (std::string path);
    void readLabelData (std::string path);
    void splitData();
    void countClasses();

    uint32_t formatData (const unsigned char* bytes);

    int getClassCounts();
    int getDataArraySize();
    int getTrainingDataSize();
    int getTestDataSize();
    int getValidationDataSize();

    std::vector<Data *> * getTrainingData();
    std::vector<Data *> * getTestData();
    std::vector<Data *> * getValidationData();
    std::map<uint8_t, int> getClassMap;
    
};

#endif