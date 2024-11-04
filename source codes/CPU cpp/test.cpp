#include <iostream>
#include <iomanip>
#include <vector>
#include <time.h>
#include <chrono>

#include "vdsim.h"
#include "genData.cpp"
#include "cnvEnc.cpp"
#include "channel_sim.cpp"
#include "sdvd.cpp"

using namespace std;

void test1() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test1/input.txt");
    fio_golden.open("test1/golden.txt");
    fio_expected_output.open("test1/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 1.0;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 1000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;

    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);

    cout << "The elapsed time was " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test2() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test2/input.txt");
    fio_golden.open("test2/golden.txt");
    fio_expected_output.open("test2/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 1.0;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 1000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);

    cout << "The elapsed time was " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test3() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test3/input.txt");
    fio_golden.open("test3/golden.txt");
    fio_expected_output.open("test3/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 1.0;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 1000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);

    cout << "The elapsed time was " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test4() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test4/input.txt");
    fio_golden.open("test4/golden.txt");
    fio_expected_output.open("test4/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 1.0;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 1000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);

    cout << "The elapsed time was " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test5() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test5/input.txt");
    fio_golden.open("test5/golden.txt");
    fio_expected_output.open("test5/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 0.5;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 1000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);

    cout << "The elapsed time was " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test6() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test6/input.txt");
    fio_golden.open("test6/golden.txt");
    fio_expected_output.open("test6/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 1.0;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 1000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);

    cout << "The elapsed time was " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test7() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test7/input.txt");
    fio_golden.open("test7/golden.txt");
    fio_expected_output.open("test7/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 1.5;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 1000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);

    cout << "The elapsed time was " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test8() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test8/input.txt");
    fio_golden.open("test8/golden.txt");
    fio_expected_output.open("test8/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 2.0;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 1000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);

    cout << "The elapsed time was " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test9() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test9/input.txt");
    fio_golden.open("test9/golden.txt");
    fio_expected_output.open("test9/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 2.5;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 1000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);

    cout << "The elapsed time was " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test10() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test10/input.txt");
    fio_golden.open("test10/golden.txt");
    fio_expected_output.open("test10/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 3.0;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 1000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);

    cout << "The elapsed time was " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test11() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test11/input.txt");
    fio_golden.open("test11/golden.txt");
    fio_expected_output.open("test11/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 0.2;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 10000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);
    
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    cout << "The elapsed time was " << duration << "ms" << endl;
    cout << "Mbps: " << (float(msg_len) / duration * 1000) << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test12() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test12/input.txt");
    fio_golden.open("test12/golden.txt");
    fio_expected_output.open("test12/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 0.2;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 10000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);
    
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    cout << "The elapsed time was " << duration << "ms" << endl;
    cout << "Mbps: " << (float(msg_len) / duration * 1000) << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test13() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test13/input.txt");
    fio_golden.open("test13/golden.txt");
    fio_expected_output.open("test13/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 0.2;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 10000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);
    
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    cout << "The elapsed time was " << duration << "ms" << endl;
    cout << "Mbps: " << (float(msg_len) / duration * 1000) << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test14() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test14/input.txt");
    fio_golden.open("test14/golden.txt");
    fio_expected_output.open("test14/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 0.2;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 10000;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    genData01(msg_len, onezer);
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);
    
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    cout << "The elapsed time was " << duration << "ms" << endl;
    cout << "Mbps: " << (float(msg_len) / duration * 1000) << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void test15() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("test15/input.txt");
    fio_golden.open("test15/golden.txt");
    fio_expected_output.open("test15/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 1.0;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 60;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    // genData01(msg_len, onezer);
    onezer = {0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1};
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }

    for (int i=0; i < ((K * 5) - 1) * 2; i++) {
        fio_input << 0.0 << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);
    
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    cout << "The elapsed time was " << duration << "ms" << endl;
    cout << "Mbps: " << (float(msg_len) / duration * 1000) << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

void testK_9() {
    vector<int> onezer;                         // Original data arrays
    vector<int> encoded;                        // encoded data arrays
    vector<int> sdvdout;                        // Decoded data arrays
    vector<float> splusn;                       // Noisy data array

    // IO Files variables
    ofstream fio_input; 
    ofstream fio_golden;
    ofstream fio_expected_output;
    fio_input.open("testK_9/input.txt");
    fio_golden.open("testK_9/golden.txt");
    fio_expected_output.open("testK_9/expected_output.txt");

    int m = K - 1;
    float es_ovr_n0 = 1.0;         
    float number_errors_encoded, e_ber;         // Various statistics variables
    long msg_len = 60;
    float channel_len = (msg_len + m) * 2;      

    // Allocate memory for vector
    onezer.resize(msg_len);
    encoded.resize(channel_len);
    splusn.resize(channel_len);
    sdvdout.resize(msg_len);

    // Test starts
    number_errors_encoded = 0.0;
    e_ber = 0.0;
    
    // genData01(msg_len, onezer);
    onezer = {0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1};
    for (int i : onezer) {
        fio_golden << i << endl;
    }

    // cout << "Convolutionally encoding the data" << endl;
    cnvEnc(g, msg_len, onezer, encoded);

    // cout << "Adding noise to the encoded data" << endl;
    addnoise(es_ovr_n0, channel_len, encoded, splusn);
    for (float i : splusn) {
        fio_input << setprecision(6) << i << endl;
    }

    for (int i=0; i < ((K * 5) - 1) * 2; i++) {
        fio_input << 0.0 << endl;
    }
    
    // cout << "Decoding the BSC data" << endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    sdvd(es_ovr_n0, channel_len, splusn, sdvdout);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    for (int i : sdvdout) {
        fio_expected_output << i << endl;
    }

    for (int t=0; t<msg_len; t++) {
        if (onezer[t] != sdvdout[t]) {
            number_errors_encoded += 1;
        }
    }

    e_ber = number_errors_encoded / (msg_len);
    
    int duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    cout << "The elapsed time was " << duration << "ms" << endl;
    cout << "Mbps: " << (float(msg_len) / duration * 1000) << endl;
    cout << "At " << setprecision(2) << es_ovr_n0 << "db Es/No, ";
    cout << "the e_ber was " << setprecision(4) << e_ber << endl;

    return;
}

int main () {
    // test1();
    // test2();
    // test3();
    // test4();
    // test5();
    // test6();
    // test7();
    // test8();
    // test9();
    // test10();
    // test11();
    // test12();
    // test13();
    test14();
    // test15();
    // testK_9();

    return 0;
}