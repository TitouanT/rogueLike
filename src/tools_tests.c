#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>

//#include "/usr/local/include/CUnit/CUnit.h"
//#include "/usr/local/include/CUnit/Basic.h"

#include "../include/tools.h"

#define TRUE  1
#define FALSE 0

#define PRINT_ERROR TRUE

/* Test Suite setup and cleanup functions */
int init_suite(){
  return 0;
}
int clean_suite(){
  return 0;
}

int init_suite_fic(){

  FILE *fic;

  fic = fopen("test_fic.txt", "w");
  fprintf(fic, "Cette ligne fait 30 caractères\n");
  fprintf(fic, " Cette ligne fait 31 caractères");
  fclose(fic);

  fic = fopen("test_fic_empty.txt", "w");
  fclose(fic);

  remove("test_fic_no.txt");

  return 0;
}

int clean_suite_fic(){
  remove("test_fic.txt");
  remove("test_fic_empty.txt");
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

  CU_ASSERT_EQUAL(isBetween(-10, -20, -10), TRUE);
  CU_ASSERT_EQUAL(isBetween(-15, -20, -10), TRUE);
  CU_ASSERT_EQUAL(isBetween(-20, -20, -10), TRUE);
  CU_ASSERT_EQUAL(isBetween(-4,  -20, -10), FALSE);
}

void intervalOverlaping_test(){
  CU_ASSERT_EQUAL(intervalOverlaping(0, 1, 0, 2), TRUE);
  CU_ASSERT_EQUAL(intervalOverlaping(0, 1, 1, 2), TRUE);
  CU_ASSERT_EQUAL(intervalOverlaping(0, 1, 2, 3), FALSE);
  CU_ASSERT_EQUAL(intervalOverlaping(-20, -10, -15, 20),  TRUE);
  CU_ASSERT_EQUAL(intervalOverlaping(-20, -10,  -5, 20), FALSE);
  CU_ASSERT_EQUAL(intervalOverlaping(-15, 20,  -10, -20), TRUE);
  CU_ASSERT_EQUAL(intervalOverlaping(-5, 20,  -10, -20), FALSE);
  CU_ASSERT_EQUAL(intervalOverlaping(0, 0, 0, 0), TRUE);
  CU_ASSERT_EQUAL(intervalOverlaping(-1, -1, -1, -1), TRUE);
  CU_ASSERT_EQUAL(intervalOverlaping(1, 1, 1, 1), TRUE);
}

void mapping_test(){
  CU_ASSERT_EQUAL(mapping(2, 10, 20, 1000, 2000), 200);
  CU_ASSERT_EQUAL(mapping(0, 10, 20, 1000, 2000), 0);
  CU_ASSERT_EQUAL(mapping(10, 10, 20, 1000, 2000), 1000);
  CU_ASSERT_EQUAL(mapping(20, 10, 20, 1000, 2000), 2000);

  CU_ASSERT_EQUAL(mapping(0, 1, 2, 1, 2), 0);
  CU_ASSERT_EQUAL(mapping(1, 1, 2, 1, 2), 1);
}

void randab_test(){
  int val1 = 0;
  int val2 = 1;
  int i, j, rslt;

  // On teste des intervalles de random de [0;1] à [100;1000]

  for(i = 0 ; i < 100 ; i++){
    for(j = 0 ; j < 10 ; j++){
      rslt = randab(val1,val2);
      CU_ASSERT_EQUAL(isBetween(rslt, val1, val2), TRUE);
      val2++;
    }
    val1++;
  }
}

void car2val_test(){
  CU_ASSERT_EQUAL(car2val('a'),  0);
  CU_ASSERT_EQUAL(car2val('b'),  1);
  CU_ASSERT_EQUAL(car2val('c'),  2);
  CU_ASSERT_EQUAL(car2val('d'),  3);
  CU_ASSERT_EQUAL(car2val('e'),  4);
  CU_ASSERT_EQUAL(car2val('f'),  5);
  CU_ASSERT_EQUAL(car2val('g'),  6);
  CU_ASSERT_EQUAL(car2val('h'),  7);
  CU_ASSERT_EQUAL(car2val('i'),  8);
  CU_ASSERT_EQUAL(car2val('j'),  9);
  CU_ASSERT_EQUAL(car2val('k'), 10);
  CU_ASSERT_EQUAL(car2val('l'), 11);
  CU_ASSERT_EQUAL(car2val('m'), 12);
  CU_ASSERT_EQUAL(car2val('n'), 13);
  CU_ASSERT_EQUAL(car2val('o'), 14);
  CU_ASSERT_EQUAL(car2val('p'), 15);
  CU_ASSERT_EQUAL(car2val('q'), 16);
  CU_ASSERT_EQUAL(car2val('r'), 17);
  CU_ASSERT_EQUAL(car2val('s'), 18);
  CU_ASSERT_EQUAL(car2val('t'), 19);
  CU_ASSERT_EQUAL(car2val('u'), 20);
  CU_ASSERT_EQUAL(car2val('v'), 21);
  CU_ASSERT_EQUAL(car2val('w'), 22);
  CU_ASSERT_EQUAL(car2val('x'), 23);
  CU_ASSERT_EQUAL(car2val('y'), 24);
  CU_ASSERT_EQUAL(car2val('z'), 25);

  CU_ASSERT_EQUAL(car2val('A'),  0);
  CU_ASSERT_EQUAL(car2val('B'),  1);
  CU_ASSERT_EQUAL(car2val('C'),  2);
  CU_ASSERT_EQUAL(car2val('D'),  3);
  CU_ASSERT_EQUAL(car2val('E'),  4);
  CU_ASSERT_EQUAL(car2val('F'),  5);
  CU_ASSERT_EQUAL(car2val('G'),  6);
  CU_ASSERT_EQUAL(car2val('H'),  7);
  CU_ASSERT_EQUAL(car2val('I'),  8);
  CU_ASSERT_EQUAL(car2val('J'),  9);
  CU_ASSERT_EQUAL(car2val('K'), 10);
  CU_ASSERT_EQUAL(car2val('L'), 11);
  CU_ASSERT_EQUAL(car2val('M'), 12);
  CU_ASSERT_EQUAL(car2val('N'), 13);
  CU_ASSERT_EQUAL(car2val('O'), 14);
  CU_ASSERT_EQUAL(car2val('P'), 15);
  CU_ASSERT_EQUAL(car2val('Q'), 16);
  CU_ASSERT_EQUAL(car2val('R'), 17);
  CU_ASSERT_EQUAL(car2val('S'), 18);
  CU_ASSERT_EQUAL(car2val('T'), 19);
  CU_ASSERT_EQUAL(car2val('U'), 20);
  CU_ASSERT_EQUAL(car2val('V'), 21);
  CU_ASSERT_EQUAL(car2val('W'), 22);
  CU_ASSERT_EQUAL(car2val('X'), 23);
  CU_ASSERT_EQUAL(car2val('Y'), 24);
  CU_ASSERT_EQUAL(car2val('Z'), 25);

  CU_ASSERT_EQUAL(car2val('0'), -1);
  CU_ASSERT_EQUAL(car2val('9'), -1);
  CU_ASSERT_EQUAL(car2val('&'), -1);
  CU_ASSERT_EQUAL(car2val('\"'), -1);
}

void up2low_test(){
  CU_ASSERT_EQUAL(up2low('A'), 'a');
  CU_ASSERT_EQUAL(up2low('B'), 'b');
  CU_ASSERT_EQUAL(up2low('K'), 'k');
  CU_ASSERT_EQUAL(up2low('Y'), 'y');
  CU_ASSERT_EQUAL(up2low('Z'), 'z');

  CU_ASSERT_EQUAL(up2low('a'), 'a');
  CU_ASSERT_EQUAL(up2low('b'), 'b');
  CU_ASSERT_EQUAL(up2low('k'), 'k');
  CU_ASSERT_EQUAL(up2low('y'), 'y');
  CU_ASSERT_EQUAL(up2low('z'), 'z');

  CU_ASSERT_EQUAL(up2low('a'-1), -1);
  CU_ASSERT_EQUAL(up2low('z'+1), -1);

  CU_ASSERT_EQUAL(up2low('0'), -1);
  CU_ASSERT_EQUAL(up2low('9'), -1);
  CU_ASSERT_EQUAL(up2low('&'), -1);
  CU_ASSERT_EQUAL(up2low('\"'), -1);
}

void low2up_test(){
  CU_ASSERT_EQUAL(low2up('A'), 'A');
  CU_ASSERT_EQUAL(low2up('B'), 'B');
  CU_ASSERT_EQUAL(low2up('K'), 'K');
  CU_ASSERT_EQUAL(low2up('Y'), 'Y');
  CU_ASSERT_EQUAL(low2up('Z'), 'Z');

  CU_ASSERT_EQUAL(low2up('a'), 'A');
  CU_ASSERT_EQUAL(low2up('b'), 'B');
  CU_ASSERT_EQUAL(low2up('k'), 'K');
  CU_ASSERT_EQUAL(low2up('y'), 'Y');
  CU_ASSERT_EQUAL(low2up('z'), 'Z');

  CU_ASSERT_EQUAL(low2up('a'-1), -1);
  CU_ASSERT_EQUAL(low2up('z'+1), -1);

  CU_ASSERT_EQUAL(low2up('0'), -1);
  CU_ASSERT_EQUAL(low2up('9'), -1);
  CU_ASSERT_EQUAL(low2up('&'), -1);
  CU_ASSERT_EQUAL(low2up('\"'), -1);
}

void fileExist_test(){

  CU_ASSERT_EQUAL(fileExist("test_fic.txt"),    TRUE);
  CU_ASSERT_EQUAL(fileExist("test_fic_empty.txt"),    TRUE);
  CU_ASSERT_EQUAL(fileExist("test_fic_no.txt"), FALSE);

}

void numberLinesFile_test(){

  CU_ASSERT_EQUAL(numberLinesFile("test_fic.txt"), 2);
  CU_ASSERT_EQUAL(numberLinesFile("test_fic_no.txt"), 0);
  CU_ASSERT_EQUAL(numberLinesFile("test_fic_empty.txt"), 1); //Un fichier vide possède une ligne

}

void maxColsFile_test(){

  CU_ASSERT_EQUAL(maxColsFile("test_fic.txt"), 31);
  CU_ASSERT_EQUAL(maxColsFile("test_fic_no.txt"), 0);
  CU_ASSERT_EQUAL(maxColsFile("test_fic_empty.txt"), 0);

}

/* ****** Test runner code ****** */

int main(){

  CU_pSuite pCmp = NULL;
  CU_pSuite pRandom = NULL;
  CU_pSuite pChar = NULL;
  CU_pSuite pFiles = NULL;

  /* initialize the CUnit test registry */
  if( CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();


  /* Add a suite to the registry */
  pCmp = CU_add_suite("comparaison_test_suite", init_suite, clean_suite);
  pRandom = CU_add_suite("random_test_suite", init_suite, clean_suite);
  pChar = CU_add_suite("char_test_suite", init_suite, clean_suite);
  pFiles = CU_add_suite("pile_test_suite", init_suite_fic, clean_suite_fic);

  if(NULL == pCmp || NULL == pRandom || NULL == pChar || NULL == pFiles){
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite */
  if(
    (NULL == CU_add_test(pCmp,    "max_test", max_test)) ||
    (NULL == CU_add_test(pCmp,    "min_test", min_test)) ||
    (NULL == CU_add_test(pCmp,    "isBetween_test", isBetween_test)) ||
    (NULL == CU_add_test(pCmp,    "intervalOverlaping_test", intervalOverlaping_test)) ||
    (NULL == CU_add_test(pCmp,    "mapping_test", mapping_test)) ||
    (NULL == CU_add_test(pRandom, "randab_test", randab_test)) ||
    (NULL == CU_add_test(pChar,    "car2val_test", car2val_test)) ||
    (NULL == CU_add_test(pChar,    "up2low_test", up2low_test)) ||
    (NULL == CU_add_test(pChar,    "low2up_test", low2up_test)) ||
    (NULL == CU_add_test(pFiles,   "fileExist_test", fileExist_test)) ||
    (NULL == CU_add_test(pFiles,   "numberLinesFile_test", numberLinesFile_test)) ||
    (NULL == CU_add_test(pFiles,   "maxColsFile_test", maxColsFile_test))
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
