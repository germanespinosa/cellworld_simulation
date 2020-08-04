#pragma once
#include <json_cpp.h>

struct Reward : json_cpp::Json_object{
    float success, fail, step, discount;
    Json_object_members({
                            Add_member(success);
                            Add_member(fail);
                            Add_member(step);
                            Add_member(discount);
                        })
};