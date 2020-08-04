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
    }
    if ( step ) base_reward += step  * (double) steps;
    if ( discount != 1 ) base_reward *= pow(discount, steps);
    return base_reward;
}
