#include <iostream>
#include <random>

using namespace std;

const int SAMPLE_SIZE = 1000000;

double inverseSample(double);

int main(int argc, char* argv[]) {
    // Command-line argument: what time do I get to the bus stop? (negative is early, positive is late)
    double timeAfter730 = stod(argv[1]);

    // Produces non-deterministic seeds for linear congruence
    random_device d;

    // Uses built-in C++ linear congruence generator (a = 48271, b = 0, c = 2147483647, x_0 is random)
    minstd_rand generator(d());

    // Generates random floating point numbers uniformly on [0,1]
    uniform_real_distribution<double> uni(0.0,1.0);



    // Simulate 1 million buses and calculate the average wait time
    double averageWaitTime = 0.0;
    int numBussesMissed = 0;

    for (int n = 0; n < SAMPLE_SIZE; ++n) {
        double arrival = inverseSample(uni(generator)); // Sample from the density function
        double waitTime = 0.0;
        if (timeAfter730 <= arrival) { // I get to the stop before the bus and have to wait
            waitTime = arrival - timeAfter730;
        }
        else { // I miss the bus and wait 10 minutes for the next one
            waitTime = 10.0;
            ++numBussesMissed;
        }

        // Updates the average wait time
        averageWaitTime = (averageWaitTime * n + waitTime) / (n + 1);
    }

    cout << "Bus was missed " << 100.0 * ((double) numBussesMissed / SAMPLE_SIZE) << "% of the time" << endl;
    cout << "Average Wait Time: " << averageWaitTime << " minutes" << endl;
    
    return 0;
}

// Uses the inverse of the CDF to sample with a uniform RV between 0 and 1
// Details at https://en.wikipedia.org/wiki/Inverse_transform_sampling
double inverseSample(double random) {
    if (random < 0.1) {
        return sqrt(10.0 * random) - 3.0;
    }
    else if (random >= 0.1 && random < 0.9) {
        return 5.0 * random - 2.5;
    }
    else if (random >= 0.9) {
        return -sqrt(10.0 - 10.0 * random) + 3.0;
    }
    else {
        return -9999.0;
    }
}
