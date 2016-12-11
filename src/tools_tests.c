#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>

//#include "/usr/local/include/CUnit/CUnit.h"
//#include "/usr/local/include/CUnit/Basic.h"

#include "../include/tools.h"

#define TRUE  1
#define FALSE 0

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

void isBetween_test(){
  CU_ASSERT_EQUAL(isBetween(0, 0, 0), TRUE);
  CU_ASSERT_EQUAL(isBetween(1, 0, 0), FALSE);

  CU_ASSERT_EQUAL(isBetween(0, 0, 1), TRUE);
  CU_ASSERT_EQUAL(isBetween(1, 0, 1), TRUE);
  CU_ASSERT_EQUAL(isBetween(2, 0, 1), FALSE);

  CU_ASSERT_EQUAL(isBetween(10, 10, 20), TRUE);
  CU_ASSERT_EQUAL(isBetween(15, 10, 20), TRUE);
  CU_ASSERT_EQUAL(isBetween(20, 10, 20), TRUE);
  CU_ASSERT_EQUAL(isBetween(4,  10, 20), FALSE);

  CU_ASSERT_EQUAL(isBetween(-10, -10, -20), TRUE);
  CU_ASSERT_EQUAL(isBetween(-15, -10, -20), TRUE);
  CU_ASSERT_EQUAL(isBetween(-20, -10, -20), TRUE);
  CU_ASSERT_EQUAL(isBetween(-4,  -10, -20), FALSE);
}

void randab_test(){
  //CU_ASSERT_EQUAL(randab(0,1), 0 || 1);
}


/* ****** Test runner code ****** */

int main(){


  CU_pSuite pCmp = NULL;
  CU_pSuite pRandom = NULL;
  /* initialize the CUnit test registry */
  if( CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();


  /* Add a suite to the registry */
  pCmp = CU_add_suite("comparaison_suite", init_suite, clean_suite);
  pRandom = CU_add_suite("random_test_suite", init_suite, clean_suite);

  if(NULL == pCmp || NULL == pRandom){
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite */
  if(
    (NULL == CU_add_test(pCmp, "max_test", max_test)) ||
    (NULL == CU_add_test(pCmp, "min_test", min_test)) ||
    (NULL == CU_add_test(pCmp, "isBetween_test", isBetween_test)) ||
    (NULL == CU_add_test(pRandom, "randab_test", randab_test))
  ){
    CU_cleanup_registry();
    return CU_get_error();
  }

  printf("\e[1;1H\e[2J");


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
