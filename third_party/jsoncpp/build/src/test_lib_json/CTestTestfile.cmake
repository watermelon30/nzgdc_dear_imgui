# CMake generated Testfile for 
# Source directory: E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/src/test_lib_json
# Build directory: E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/build/src/test_lib_json
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(jsoncpp_test "E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/build/bin/Debug/jsoncpp_test.exe")
  set_tests_properties(jsoncpp_test PROPERTIES  _BACKTRACE_TRIPLES "E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/src/test_lib_json/CMakeLists.txt;27;add_test;E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/src/test_lib_json/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(jsoncpp_test "E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/build/bin/Release/jsoncpp_test.exe")
  set_tests_properties(jsoncpp_test PROPERTIES  _BACKTRACE_TRIPLES "E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/src/test_lib_json/CMakeLists.txt;27;add_test;E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/src/test_lib_json/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(jsoncpp_test "E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/build/bin/MinSizeRel/jsoncpp_test.exe")
  set_tests_properties(jsoncpp_test PROPERTIES  _BACKTRACE_TRIPLES "E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/src/test_lib_json/CMakeLists.txt;27;add_test;E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/src/test_lib_json/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(jsoncpp_test "E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/build/bin/RelWithDebInfo/jsoncpp_test.exe")
  set_tests_properties(jsoncpp_test PROPERTIES  _BACKTRACE_TRIPLES "E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/src/test_lib_json/CMakeLists.txt;27;add_test;E:/imgui_demo/nzgdc_dear_imgui/third_party/jsoncpp/src/test_lib_json/CMakeLists.txt;0;")
else()
  add_test(jsoncpp_test NOT_AVAILABLE)
endif()
