#include "ai.h"

float ConvertedBuf[AI_QECG_IN_1_SIZE];


AI_ALIGNED(32)
ai_u8 activations[AI_QECG_DATA_ACTIVATIONS_SIZE];
AI_ALIGNED(32)
ai_float in_data[AI_QECG_IN_1_SIZE]; // 这里记得修改为自己的类型，以及长度�?�择SIZE，不要是byte
AI_ALIGNED(32)
ai_float out_data[AI_QECG_OUT_1_SIZE]; // 这里也是改为size
ai_buffer *ai_input;
ai_buffer *ai_output;
ai_handle network = AI_HANDLE_NULL;
ai_error err;
ai_network_report report;

int ai_init()
{
    const ai_handle acts[] = {activations};
    err = ai_qecg_create_and_init(&network, acts, NULL);
    if (err.type != AI_ERROR_NONE)
    {
        printf("ai init_and_create error\n");
        return -1;
    }
    else
    {
        printf("ai init success\n");
    }

    if (ai_qecg_get_report(network, &report) != true)
    {
        printf("ai get report error\n");
        return -1;
    }

    printf("Model name      : %s\n", report.model_name);
    printf("Model signature : %s\n", report.model_signature);
    return 0;
}

int ai_run(ai_float *in_data, ai_float *out_data, float *data, int length)
{
    ai_i32 n_batch;

    for (int i = 0; i < length; i++)
    {
        in_data[i] = data[i];
    }

    ai_input = ai_qecg_inputs_get(network, NULL);
    ai_output = ai_qecg_outputs_get(network, NULL);
    ai_input[0].data = AI_HANDLE_PTR(in_data);
    ai_output[0].data = AI_HANDLE_PTR(out_data);

    n_batch = ai_qecg_run(network, &ai_input[0], &ai_output[0]);
    if (n_batch != 1)
    {
        ai_qecg_get_error(network);
        printf("run failed\r\n");
        return -1;
    };

    return 0; // success;
}



// Function to convert values from 0-4095 to 0-10
void convert_processed_buf(uint16_t maxval)
{
    for (int i = 0; i < AI_QECG_IN_1_SIZE; i++)
    {
        ConvertedBuf[i] = ((float)ProcessedBuf[i] / (float)maxval);
    }
}
void updateAIResults(float* out_data, int length) {
    float max_possibility = out_data[0];
    int max_index = 0;

    for (int i = 1; i < length; i++) {
        if (out_data[i] > max_possibility) {
            max_possibility = out_data[i];
            max_index = i;
        }
    }

    aiClass = max_index;
    AI_possibility = max_possibility;
}
