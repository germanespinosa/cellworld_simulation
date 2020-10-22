#include <training.h>

using namespace std;

int main(int argc, char **argv)
{
    Training_parameters p;
    p.load(argc, argv);
    Training training(p);
    training.run();
}