#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <random>

#include "vdsim.h"

using namespace std;

void addnoise(float es_ovr_n0, long channel_len, vector<int>& in_array, vector<float>& out_array); //Noted that out_array will becomes float types after added with noises
float gngauss(float mean, float sigma);

/*
Given the desired Es/No (for BPSK, = Eb/No - 3dB), calculate the standard deviation of the additive white gaussian noise (AWGN).
The standard deviation of the AWGN will be used to generate Gaussian random variables simulationg the noise that is added to the signal.
*/
void addnoise(float es_ovr_n0, long channel_len, vector<int>& in_array, vector<float>& out_array) {
    long t;
    float mean, es, sn_ratio, sigma, signal;

    mean = 0;
    es = 1;
    sn_ratio = (float)pow(10, (es_ovr_n0 / 10));
    sigma = (float)sqrt(es / (2 * sn_ratio));

    // Now transform the data from 0/1 to +1/-1, and add noise
    for (t=0; t<channel_len; t++) {
        // If the binary data value is 1, the channel symbol is -1;
        // Else if the binary data value is 0, the channel symbol is +1
        signal = 1 - 2 * in_array[t];

        // Now generate the gaussian noise point, add it to the channel symbol, and output the noisy channel symbol.
        out_array[t] = signal + gngauss(mean, sigma);
    }
}

/*
This uses the fact that a Rayleigh-distributed random variable R, with the probability distribution 
F(R) = 0 if R < 0 and 
F(R) = 1 - exp(-R^2 / 2 * sigma^2) if R >= 0,
is related to a pair of Gaussian variables C & D through the transformation 
C = R * cos(theta) and
D = R *  sin(theta),
where theta is a uniformly distributed variable in the interval (0, 2*pi).
*/
float gngauss(float mean, float sigma) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, INT_MAX);

    // Uniform and Rayleigh random variables
    double u, r;

    // Generate a uniformly distributed random number u between 0 and 1 - 1E-6
    u = (double)dist(rng) / INT_MAX;
    if (u == 1.0)
        u = 0.999999999;

    // Generate a Rayleigh-distributed random number r using u
    r = sigma * sqrt(2.0 * log(1.0 / (1.0 - u)));

    // Generate another uniformly distributed random number u as before
    u = (double)dist(rng) / INT_MAX;
    if (u == 1.0)
        u = 0.999999999;

    // Generate and return a Gaussian Distributed random number using r and u
    return (float)(mean + r *cos(2 * PI * u));
}