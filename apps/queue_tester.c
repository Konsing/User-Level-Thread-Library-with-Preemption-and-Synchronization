#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)                 \
    {                                       \
        printf("ASSERT: " #assert " ... "); \
        if (assert)                         \
        {                                   \
            printf("PASS\n");               \
        }                                   \
        else                                \
        {                                   \
            printf("FAIL\n");               \
            exit(1);                        \
        }                                   \
    }

// Test queue_create
void test_create(void)
{
    fprintf(stderr, "*** TEST create ***\n");

    TEST_ASSERT(queue_create() != NULL);
}

// Test queue_enqueue and queue_dequeue
void test_queue_simple(void)
{
    int data = 3, *ptr;
    queue_t q;

    fprintf(stderr, "*** TEST queue_simple ***\n");

    q = queue_create();
    queue_enqueue(q, &data);
    queue_dequeue(q, (void **)&ptr);
    TEST_ASSERT(ptr == &data);
}

// Test queue_length
void test_queue_length(void)
{
    int data_1 = 3;
    int data_2 = 4;
    int data_3 = 5;
    queue_t q;

    int *data_1_ptr = NULL;
    int *data_2_ptr = NULL;
    int *data_3_ptr = NULL;

    fprintf(stderr, "*** TEST queue_length ***\n");

    q = queue_create();
    TEST_ASSERT(queue_length(q) == 0);
    queue_enqueue(q, &data_1);
    TEST_ASSERT(queue_length(q) == 1);
    queue_enqueue(q, &data_2);
    TEST_ASSERT(queue_length(q) == 2);
    queue_enqueue(q, &data_3);
    TEST_ASSERT(queue_length(q) == 3);
    queue_dequeue(q, (void **)&data_1_ptr);
    TEST_ASSERT(queue_length(q) == 2);
    queue_dequeue(q, (void **)&data_2_ptr);
    TEST_ASSERT(queue_length(q) == 1);
    queue_dequeue(q, (void **)&data_3_ptr);
    TEST_ASSERT(queue_length(q) == 0);
}

// Test queue_destroy
void test_queue_destroy(void)
{
    int data = 3;
    int *data_ptr = NULL;
    queue_t q;

    fprintf(stderr, "*** TEST queue_destroy ***\n");

    q = queue_create();
    TEST_ASSERT(queue_destroy(q) == 0);
    q = queue_create();
    queue_enqueue(q, &data);
    queue_enqueue(q, &data);
    queue_enqueue(q, &data);
    TEST_ASSERT(queue_destroy(q) == -1);
    queue_dequeue(q, (void **)&data_ptr);
    fprintf(stderr, "queue_length = %d, value inside *data_ptr = %p, addr of data = %p\n", queue_length(q), data_ptr, &data);
    queue_dequeue(q, (void **)&data_ptr);
    fprintf(stderr, "queue_length = %d, value inside *data_ptr = %p, addr of data = %p\n", queue_length(q), data_ptr, &data);
    queue_dequeue(q, (void **)&data_ptr);
    fprintf(stderr, "queue_length = %d, value inside *data_ptr = %p, addr of data = %p\n", queue_length(q), data_ptr, &data);
    TEST_ASSERT(queue_length(q) == 0);
    TEST_ASSERT(queue_destroy(q) == 0);
}

// Test queue_delete
void test_queue_delete(void)
{
    int data = 3;
    queue_t q;

    fprintf(stderr, "*** TEST queue_delete ***\n");

    q = queue_create();
    queue_enqueue(q, &data);
    queue_enqueue(q, &data);
    queue_enqueue(q, &data);
    TEST_ASSERT(queue_delete(q, &data) == 0);
    TEST_ASSERT(queue_length(q) == 2);
    TEST_ASSERT(queue_delete(q, &data) == 0);
    TEST_ASSERT(queue_length(q) == 1);
    TEST_ASSERT(queue_delete(q, &data) == 0);
    TEST_ASSERT(queue_length(q) == 0);
    TEST_ASSERT(queue_delete(q, &data) == -1);
}

int iterate_sum = 0;
int iterate_count = 0;

// Test queue_iterate
void iterate_sum_cb(queue_t q, void *data)
{
    int *int_data = (int *)data;
    iterate_sum += *int_data;
    *int_data += 1;
    iterate_count++;
    fprintf(stdout, "queue_length: %d, curr_data: %d\n", queue_length(q), *int_data);
}

void test_queue_iterate(void)
{
    int data = 3;
    queue_t q;

    fprintf(stderr, "*** TEST queue_iterate ***\n");

    q = queue_create();
    queue_enqueue(q, &data);
    queue_enqueue(q, &data);
    queue_enqueue(q, &data);
    queue_iterate(q, iterate_sum_cb);
    TEST_ASSERT(iterate_sum == 12);
    TEST_ASSERT(iterate_count == 3);
}

int main()
{
    test_create();
    test_queue_simple();
    test_queue_length();
    test_queue_destroy();
    test_queue_delete();
    test_queue_iterate();
    return 0;
}