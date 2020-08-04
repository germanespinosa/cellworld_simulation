#pragma once
#include <json_cpp.h>

enum Result{
    Success,
    Fail
};

struct Reward : json_cpp::Json_object{

    double compute (Result, unsigned int) const;

    double success, fail, step, discount;
    Json_object_members({
                            Add_member(success);
                            Add_member(fail);
                            Add_member(step);
                            Add_member(discount);
                        })
};