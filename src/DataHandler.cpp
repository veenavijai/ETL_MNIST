#include "../include/DataHandler.hpp"
#include <random>

DataHandler::DataHandler()
{
    dataArray = new std::vector<Data *>;
    trainingData = new std::vector<Data *>;
    testData = new std::vector<Data *>;
    validationData = new std::vector<Data *>;
}
DataHandler::~DataHandler()
{
    // free all dynamically allocated variables
    delete dataArray;
    delete trainingData;
    delete testData;
    delete validationData;
}

void DataHandler::readInputData (std::string path)
{
    uint32_t magic = 0;
    uint32_t num_images = 0;
    uint32_t num_rows = 0;
    uint32_t num_cols = 0;

    unsigned char bytes[4];
    FILE *f = fopen (path.c_str(), "r");
    if (f)
    {
        for (int i = 0; i < 4; i++)
        {
            if (fread (bytes, sizeof(bytes), 1, f))
            {
                switch(i)
                {
                    case 0: magic = formatData (bytes); break;
                    case 1: num_images = formatData (bytes); break;
                    case 2: num_rows = formatData (bytes); break;
                    case 3: num_cols = formatData (bytes); break;
                }
            }
        }

        printf ("Done getting file header\n");
        uint32_t image_size = num_rows * num_cols;

        for (int i = 0; i < num_images; i++)
        {
            Data * d = new Data();
            d->setFeatureVector (new std::vector<uint8_t>());
            uint8_t element[1];

            // append pixel by pixel
            for (int j = 0; j < image_size; j++)
            {
                if (fread (element, sizeof(element), 1, f))
                {
                    d->appendToFeatureVector (element[0]);
                }
            }

            // add ith image to dataArray
            dataArray->push_back(d);
        }

        featureVectorSize = dataArray->at(0)->getFeatureVector()->size();
        printf ("Successfully read %lu images\n", dataArray->size());
        printf ("Size of feature vector for each image is %d\n", featureVectorSize);
    }

    else
    {
        printf ("Could not find file\n");
        exit(1);
    }
}

void DataHandler::readLabelData (std::string path)
{
    uint32_t magic = 0;
    uint32_t num_items = 0;

    unsigned char bytes[4];
    FILE *f = fopen (path.c_str(), "r");
    if (f)
    {
        for (int i = 0; i < 2; i++)
        {
            if (fread (bytes, sizeof(bytes), 1, f))
            {
                switch(i)
                {
                    case 0: magic = formatData (bytes); break;
                    case 1: num_items = formatData (bytes); break;
                }
            }
        }

        for (int i = 0; i < num_items; i++)
        {
            uint8_t element[1];

            if (fread (element, sizeof(element), 1, f))
            {
                dataArray->at(i)->setLabel(element[0]);
            }
        }

        printf ("Done getting all labels\n");
    }

    else
    {
        printf ("Could not find label file\n");
        exit(1);
    }
}

void DataHandler::splitData()
{
    int trainingSize = dataArray->size() * TRAIN_SET_PERCENT;
    int testSize = dataArray->size() * TEST_SET_PERCENT;
    int validationSize = dataArray->size() * VALIDATION_SET_PERCENT;

    int dataArrayIdx = 0;

    // Training data
    int count = 0;
    while (count < trainingSize)
    {
        trainingData->push_back (dataArray->at(dataArrayIdx++));
        count++;
    }

    // Test data
    count = 0;
    while (count < testSize)
    {
        testData->push_back (dataArray->at(dataArrayIdx++));
        count++;
    }

    // Validation data
    count = 0;
    while (count < validationSize)
    {
        validationData->push_back (dataArray->at(dataArrayIdx++));
        count++;
    }

    printf ("Training data size: %lu \n", trainingData->size());
    printf ("Test data size: %lu \n", testData->size());
    printf ("Validation data size: %lu \n", validationData->size());  
}

void DataHandler::countClasses()
{
    int classCount = 0;

    for (unsigned int i = 0; i < dataArray->size(); i++)
    {
        uint8_t curLabel = dataArray->at(i)->getLabel();
        // check if the existing label is already a key in the map
        if (classFromInt.find (curLabel) == classFromInt.end())
        {
            classFromInt[curLabel] = classCount;
            dataArray->at(i)->setEnumeratedLabel (classCount);
            classCount++;
        }
        else
        {
            dataArray->at(i)->setEnumeratedLabel (classFromInt[curLabel]);
        }
    }
    
    class_counts = classCount;
    printf ("Extracted %d unique classes", class_counts);
}

uint32_t DataHandler::formatData(const unsigned char* bytes)
{
    return (uint32_t) (bytes[0] >> 24) | 
                      (bytes[1] & 0x00FF0000) >> 8) |
                      (bytes[2] & 0x0000FF00) << 8) |
                      (bytes[3] << 24))

}

int DataHandler::getClassCounts()
{
    return class_counts;
}

int DataHandler::getDataArraySize()
{
    return dataArray->size();
}

int DataHandler::getTrainingDataSize()
{
    return trainingData->size();
}

int DataHandler::getTestDataSize()
{
    return testData->size();
}

int DataHandler::getValidationDataSize()
{
    return validationData->size();
}

std::vector<data *> * DataHandler::getTrainingData()
{
    return trainingData;
}

std::vector<data *> * DataHandler::getTestData()
{
    return testData;
}

std::vector<data *> * DataHandler::getValidationData()
{
    return validationData;
}

std::map<uint8_t, int> DataHandler::getClassMap()
{
    return classFromInt;
}