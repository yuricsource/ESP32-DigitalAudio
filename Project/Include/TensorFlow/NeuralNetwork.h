#ifndef __NeuralNetwork__
#define __NeuralNetwork__

#include <stdint.h>
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/core/api/error_reporter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_interpreter.h"

class NeuralNetwork
{
private:
    tflite::MicroMutableOpResolver<10> *m_resolver;
    tflite::ErrorReporter *m_error_reporter;
    const tflite::Model *m_model;
    tflite::MicroInterpreter *m_interpreter;
    TfLiteTensor *input;
    TfLiteTensor *output;
    uint8_t *m_tensor_arena;

public:
	NeuralNetwork(const void *model);
    ~NeuralNetwork();
    float *getInputBuffer();
    float predict();
};

#endif