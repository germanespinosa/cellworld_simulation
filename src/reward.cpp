#include <reward.h>
#include <cmath>

double Reward::compute(Result result, unsigned int steps) const {
    double base_reward;
    switch (result) {
        case Success:
            base_reward = success;
            break;
        case Fail:
            base_reward = fail;
            break;
        case Unknown:
            base_reward = unknown;
            break;
    }
    if ( step ) base_reward += step  * (double) steps;
    if ( discount ) base_reward *= pow((1 - discount), steps);
    return base_reward;
}
