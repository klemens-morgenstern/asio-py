import os

import sys

sys.path.append(os.getcwd())
import template_test_impl

assert template_test_impl.static_()   == template_test_impl.bind_int
assert template_test_impl.static_(int)   == template_test_impl.bind_int
assert template_test_impl.static_(float) == template_test_impl.bind_double
assert template_test_impl.static_(str)   == template_test_impl.bind_str
assert template_test_impl.static_(bytes) is None

assert template_test_impl.static_2()           == template_test_impl.bind_int_double
assert template_test_impl.static_2(int,int)    == template_test_impl.bind_int_int
assert template_test_impl.static_2(float, int) == template_test_impl.bind_double_int
assert template_test_impl.static_2(int,float)  == template_test_impl.bind_int_double
assert template_test_impl.static_2(int, str)   == template_test_impl.bind_int_str
assert template_test_impl.static_2(str, int)   is None
assert template_test_impl.static_2(bytes, int) is None

assert template_test_impl.static_3(float, int, str) == template_test_impl.bind_double_int_str
assert template_test_impl.static_3(float, int, int) is None


assert template_test_impl.dyn_1(int) is None
assert template_test_impl.dyn_1(float) == template_test_impl.bind_double
