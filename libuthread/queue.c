#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct node {
  void *data;
  struct node *next;
};
typedef struct node node;

struct queue {
  node *front;
  node *rear;
  int size;
};

queue_t queue_create(void) {
  queue_t queue = malloc(sizeof(queue_t));
  if (!queue)
    return NULL;
  queue->front = NULL;
  queue->rear = NULL;
  queue->size = 0;
  return queue;
}

/*
 * queue_destroy - Deallocate a queue
 * @queue: Queue to deallocate
 *
 * Deallocate the memory associated to the queue object pointed by @queue.
 *
 * Return: -1 if @queue is NULL or if @queue is not empty. 0 if @queue was
 * successfully destroyed.
 */
int queue_destroy(queue_t queue) {
  if (!queue || queue_length(queue)) // the queue is not empty(meaning there is
                                     // still data that hasn't been dequeued)
    return -1;
  free(queue);
  return 0;
}

/*
 * queue_enqueue - Enqueue data item
 * @queue: Queue in which to enqueue item
 * @data: Address of data item to enqueue
 *
 * Enqueue the address contained in @data in the queue @queue.
 *
 * Return: -1 if @queue or @data are NULL, or in case of memory allocation error
 * when enqueing. 0 if @data was successfully enqueued in @queue.
 */
int queue_enqueue(queue_t queue, void *data) {
  node *new_node = malloc(sizeof(node));
  if (!queue || !new_node || !data){
    free(new_node);
    return -1;
  }

  new_node->data = data;
  new_node->next = NULL; // new_node->next is pointing at the end of the queue
  if (!queue_length(queue)) { // if the queue is empty
    queue->front = new_node;
    queue->rear =
        new_node; // front and rear of the queue will point at new_node
  } else {        // if the queue is not empty
    queue->rear->next =
        new_node; // link the previous item in the queue to the new_node
    queue->rear = new_node; // add new_node to the rear of the queue
  }
  queue->size++;
  return 0;
}

/*
 * queue_dequeue - Dequeue data item
 * @queue: Queue in which to dequeue item
 * @data: Address of data pointer where item is received
 *
 * Remove the oldest item of queue @queue and assign this item (the value of a
 * pointer) to @data.
 *
 * Return: -1 if @queue or @data are NULL, or if the queue is empty. 0 if @data
 * was set with the oldest item available in @queue.
 */
int queue_dequeue(queue_t queue, void **data) {
  if (!queue || !data || !queue_length(queue))
    return -1;

  node *tmp = queue->front; // tmp is holding the address of the oldest item
  *data = tmp->data;        // save the value of the oldest item into data
  queue->front =
      tmp->next; // now queue->front will point to the next oldest item

  if (!queue->front) // if theres nothing left in queue, queue->rear should be
                     // pointing at NULL
    queue->rear = NULL;

  queue->size--;
  free(tmp);
  return 0;
}

/*
 * queue_delete - Delete data item
 * @queue: Queue in which to delete item
 * @data: Data to delete
 *
 * Find in queue @queue, the first (ie oldest) item equal to @data and delete
 * this item.
 *
 * Return: -1 if @queue or @data are NULL, of if @data was not found in the
 * queue. 0 if @data was found and deleted from @queue.
 */
int queue_delete(queue_t queue, void *data) {
  int found = -1;
  if (!queue)
    return found;
  void *ptr; // dont care, used as arguments for queue_dequeue
  node *previous = queue->front; // used to relink the nodes
  node *tmp; // used to save the address pointing to the found item's node
  if (queue->front->data == data) { // if the oldest item's data matches
    queue_dequeue(
        queue,
        (void **)&ptr); // just dequeue will delete and free the oldest node
    previous = NULL;    // why not, doesnt hurt
    found = 0;
  } else {
    node *current = previous->next; // current will go through the second oldest
                                    // item to newest item
    while (current) {               // while loop stops after the newest item
      if (current->data == data) {  // if the current item matches
        tmp = current;              // save the node so we can free it later
        previous->next =
            current->next; // connect previous node to current's next node ||
                           // disconnect the link between the second newest item
                           // to the newest item
        if (current == queue->rear) { // if this item happens to be the newest
                                      // item on the queue
          queue->rear = previous; // the second item becomes the newest item in
                                  // the queue nn
        } else { // if this item is anywhere in the queue but oldest or newest
          current->next = NULL; // why not, doesnt hurt
        }
        free(tmp); // delete the found item from the queue
        queue->size--;
        current = previous; // since we deleted the current node, we need to
                            // point current back to previous
        found = 0;
      }
      previous = current; // double linked list like setup to traverse the queue
      current = current->next;
    }
  }
  return found;
}

/*
 * queue_iterate - Iterate through a queue
 * @queue: Queue to iterate through
 * @func: Function to call on each queue item
 *
 * This function iterates through the items in the queue @queue, from the oldest
 * item to the newest item, and calls the given callback function @func on each
 * item. The callback function receives the current data item as parameter.
 *
 * Note that this function should be resistant to data items being deleted
 * as part of the iteration (ie in @func).
 *
 * Return: -1 if @queue or @func are NULL, 0 otherwise.
 */
int queue_iterate(queue_t queue, queue_func_t func) {
  if (!queue || !func)
    return -1;
  node *current = queue->front; // current is pointing at the oldest item
  node *tmp;        // tmp pointer is used to save the next node incase the func
                    // deletes the data item
  while (current) { // while current is not pointing at NULL
    tmp = current->next;
    func(queue, current->data);
    current = tmp;
  }
  return 0;
}

/*
 * queue_length - Queue length
 * @queue: Queue to get the length of
 *
 * Return the length of queue @queue.
 *
 * Return: -1 if @queue is NULL. Length of @queue otherwise.
 */
int queue_length(queue_t queue) {
  if (!queue)
    return -1;
  return queue->size;
}