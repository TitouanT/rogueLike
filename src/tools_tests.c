//#include <CUnit/CUnit.h>
//#include <CUnit/Basic.h>
#include <stdio.h>

#include "/usr/local/include/CUnit/CUnit.h"
#include "/usr/local/include/CUnit/Basic.h"

#include "../include/tools.h"


/* Test Suite setup and cleanup functions */
int init_suite(){
  return 0;
}
int clean_suite(){
  return 0;
}



/* ************ Test case functions ************ */
void min_test(){
  CU_ASSERT_EQUAL(min(1,2), 1);
  CU_ASSERT_EQUAL(min(2,1), 1);
  CU_ASSERT_EQUAL(min(0,0), 0);
  CU_ASSERT_EQUAL(min(1,1), 1);
  CU_ASSERT_EQUAL(min(-5,10), -5);
  CU_ASSERT_EQUAL(min(-2,-1), -2);
}

void max_test(){
  CU_ASSERT_EQUAL(max(1,2), 2);
  CU_ASSERT_EQUAL(max(2,1), 2);
  CU_ASSERT_EQUAL(max(0,0), 0);
  CU_ASSERT_EQUAL(max(1,1), 1);
  CU_ASSERT_EQUAL(max(-5,10), 10);
  CU_ASSERT_EQUAL(max(-2,-1), -1);
}




/* ****** Test runner code ****** */

int main(){


  CU_pSuite pSuite = NULL;
  /* initialize the CUnit test registry */
  if( CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();


  /* Add a suite to the registry */
  pSuite = CU_add_suite("min_max_test_suite", init_suite, clean_suite);

  if(NULL == pSuite){
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite */
  if(
    (NULL == CU_add_test(pSuite, "max_test", max_test)) ||
    (NULL == CU_add_test(pSuite, "min_test", min_test))
  ){
    CU_cleanup_registry();
    return CU_get_error();
  }



  CU_basic_set_mode(CU_BRM_VERBOSE);
  /* Run all tests using the basic interface */
  CU_basic_run_tests();


  printf("\n");
  CU_basic_show_failures(CU_get_failure_list());
  printf("\n\n");

  /* Cleanup registry and return */
  CU_cleanup_registry();
  return CU_get_error();

}
