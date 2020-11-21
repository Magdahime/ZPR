#define PER_SPECIMEN 2

__kernel void neuronCalc01(
    const int specimenCount,
    const int neuronInputCount,
    const int neuronsPerSpecimen,
    const int specimenPerThread,
    const __global float* values,
    const __global float* weights,
    __global float* results
    ) {
    const int localSpecimen = get_local_id(0); 
    const int globalSpecimen = get_group_id(0); 


    float inputReg;
    float currWeight;
    float resultsReg[PER_SPECIMEN] = { 0 };

    int specimenNo;
    int neuronNo;
    int inputNo;

    specimenNo = 
        globalSpecimen * specimenPerThread
        + localSpecimen;

    for(int ic = 0; ic < neuronInputCount; ic++){
        inputNo = specimenNo * neuronInputCount + ic;
        inputReg = values[inputNo];

        for(int nc = 0; nc < neuronsPerSpecimen; nc++){
            neuronNo = specimenNo * neuronsPerSpecimen + nc;
            currWeight = weights[neuronNo*neuronInputCount + ic];
            resultsReg[nc] += inputReg*currWeight;
            if(ic == neuronInputCount-1){
                resultsReg[nc] = resultsReg[nc]/(1+fabs(resultsReg[nc]));
                results[neuronNo] = resultsReg[nc];
            }
        }
    }
}