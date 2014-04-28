#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "storage_mgr.h"
#include "dberror.h"
#include "test_helper.h"

// test name
char *testName;

/* test output files */
#define TESTPF "test_pagefile.bin"

/* prototypes for test functions */
static void testCreateOpenClose(void);
static void testSinglePageContent(void);
static void testBlockmovement(void);


/* main function running all tests */

int
main (void)
{
  testName = "";
  
  initStorageManager();

  testCreateOpenClose();
  testSinglePageContent();
  testBlockmovement();
  return 0;
 }





/* check a return code. If it is not RC_OK then output a message, error description, and exit */
/* Try to create, open, and close a page file */
void
testCreateOpenClose(void)
{
  SM_FileHandle fh;

  testName = "test create open and close methods";

  TEST_CHECK(createPageFile (TESTPF));
  
  TEST_CHECK(openPageFile (TESTPF, &fh));
  ASSERT_TRUE(strcmp(fh.fileName, TESTPF) == 0, "filename correct");
  ASSERT_TRUE((fh.totalNumPages == 1), "expect 1 page in new file");
  ASSERT_TRUE((fh.curPagePos == 0), "freshly opened file's page position should be 0");

  TEST_CHECK(closePageFile (&fh));
  TEST_CHECK(destroyPageFile (TESTPF));

  // after destruction trying to open the file should cause an error
  ASSERT_TRUE((openPageFile(TESTPF, &fh) != RC_OK), "opening non-existing file should return an error.");

  TEST_DONE();
}

/* Try to create, open, and close a page file */
void
testSinglePageContent(void)
{
  SM_FileHandle fh;
  SM_PageHandle ph;
  int i;

  testName = "test single page content";

  ph = (SM_PageHandle) malloc(PAGE_SIZE);

  // create a new page file
  TEST_CHECK(createPageFile (TESTPF));
  TEST_CHECK(openPageFile (TESTPF, &fh));
  printf("created and opened file\n");
  

 // read first page into handle
  TEST_CHECK(readFirstBlock (&fh, ph));
  // the page should be empty (zero bytes)
    ASSERT_TRUE((ph[i] == 0), "expected zero byte in first page of freshly initialized page");
printf("%s",ph);
  printf("first block was empty\n");
  

  // change ph to be a string and write that one to disk
  
  for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
  TEST_CHECK(writeBlock (0, &fh, ph));
  printf("writing first block\n");

  // change ph to be a string and write that one to disk
 for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
TEST_CHECK(writeBlock (1, &fh, ph));
  printf("writing 2 block\n");

// change ph to be a string and write that one to disk
 for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
TEST_CHECK(writeBlock (2, &fh, ph));
  printf("writing 3 block\n");


  // change ph to be a string and write that one to disk
  for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
TEST_CHECK(writeCurrentBlock(&fh, ph));
  printf("writing the current block\n");



  // read back the page containing the string and check that it is correct
  TEST_CHECK(readCurrentBlock (&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
  printf("reading current block\n");


// read back the page containing the string and check that it is correct
  TEST_CHECK(readPreviousBlock (&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
  printf("reading previous block\n");

  // read back the page containing the string and check that it is correct
  TEST_CHECK(readPreviousBlock (&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
  printf("reading previous block\n");

// read back the page containing the string and check that it is correct
  TEST_CHECK(readFirstBlock (&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
  printf("reading first block\n");

// read back the page containing the string and check that it is correct
  TEST_CHECK(readNextBlock (&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
  printf("reading next block\n");

// read back the page containing the string and check that it is correct
  TEST_CHECK(readLastBlock (&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
  printf("reading last block\n");

//Ensure Capacity
TEST_CHECK(ensureCapacity(10,&fh));
  printf("Ensure capacity Passed\n");

// Appending Empty Block
  printf("Appending empty block \n");
  TEST_CHECK(appendEmptyBlock(&fh));

  // destroy new page file
  TEST_CHECK(destroyPageFile (TESTPF));  
  
  TEST_DONE();
}

void
testBlockmovement(void)
{
SM_FileHandle fh;
  SM_PageHandle ph;
  int i;

  testName = "test block movement";

  ph = (SM_PageHandle) malloc(PAGE_SIZE);
// create a new page file
  TEST_CHECK(createPageFile (TESTPF));
  TEST_CHECK(openPageFile (TESTPF, &fh));
  printf("created and opened file\n");
  

  // change ph to be a string and write that one to disk
  
  for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
  TEST_CHECK(writeBlock (0, &fh, ph));
//printf("%s",ph);
  printf("writing first block\n");

  // change ph to be a string and write that one to disk
 for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
TEST_CHECK(writeBlock (1, &fh, ph));
//printf("%s",ph);
  printf("writing 2 block\n");

 // change ph to be a string and write that one to disk
 for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
TEST_CHECK(writeBlock (2, &fh, ph));
//printf("%s",ph);
  printf("writing 2 block\n");

// change ph to be a string and write that one to disk
 for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
TEST_CHECK(writeBlock (1, &fh, ph));
//printf("%s",ph);
  printf("writing 1 block again\n");

// change ph to be a string and write that one to disk
 for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
TEST_CHECK(writeCurrentBlock (&fh, ph));
//printf("%s",ph);
  printf("writing current block\n");

  // read back the page containing the string and check that it is correct
  TEST_CHECK(readFirstBlock (&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
//printf("%s",ph);
  printf("reading first block\n");
  
  // change ph to be a string and write that one to disk
 for (i=0; i < PAGE_SIZE; i++)
    ph[i] = (i % 10) + '0';
TEST_CHECK(writeCurrentBlock (&fh, ph));
//printf("%s",ph);
  printf("writing current block\n");
 
 // read back the page containing the string and check that it is correct
  TEST_CHECK(readBlock (1,&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
//printf("%s",ph);
  printf("reading given block\n");

 // read back the page containing the string and check that it is correct
  TEST_CHECK(readBlock (0,&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
//printf("%s",ph);
  printf("reading given block\n");

// read back the page containing the string and check that it is correct
  TEST_CHECK(readNextBlock (&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
//printf("%s",ph);
  printf("reading next block\n");
 
 // read back the page containing the string and check that it is correct
  TEST_CHECK(readBlock (2,&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
//printf("%s",ph);
  printf("reading given block\n");

// read back the page containing the string and check that it is correct
  TEST_CHECK(readNextBlock (&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
//printf("%s",ph);
  printf("reading next block\n");

// read back the page containing the string and check that it is correct
  TEST_CHECK(readCurrentBlock (&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
//printf("%s",ph);
  printf("reading current block\n");

// read back the page containing the string and check that it is correct
  TEST_CHECK(readPreviousBlock (&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
//printf("%s",ph);
  printf("reading previous block\n");

// read back the page containing the string and check that it is correct
  TEST_CHECK(readLastBlock (&fh, ph));
for (i=0; i < PAGE_SIZE; i++)
    ASSERT_TRUE((ph[i] == (i % 10) + '0'), "character in page read from disk is the one we expected.");
//printf("%s",ph);
  printf("reading last block\n");

 // destroy new page file
  TEST_CHECK(destroyPageFile (TESTPF));  
  
  TEST_DONE();

}