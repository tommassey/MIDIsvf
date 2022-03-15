// // compile only if in correct env/testing situation
// #if defined(ARDUINO) && defined(UNIT_TEST)

// #include <Arduino.h>
// #include "unity.h"

// #include "mod1.h"

// void test_mod1() {
//         mod1_s  o;
//         mod1_init(&o);

//         mod1_set_a(&o, 17);
//         TEST_ASSERT_EQUAL(mod1_get_a(&o), 17);

//         mod1_process(&o);
//         TEST_ASSERT_EQUAL(mod1_get_a(&o), 18);
// }

// // setup connects serial, runs test cases (upcoming)
// void setup() {
//   delay(2000);
//   Serial.begin(9600);
//   pinMode(13, OUTPUT);
//   digitalWrite(13, HIGH);

//   //
//   UNITY_BEGIN();

//   // calls to tests will go here
//   RUN_TEST(test_mod1);

//   UNITY_END();
// }

// void loop() {
//   // nothing to be done here.
// }

// #endif