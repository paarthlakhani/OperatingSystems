/* sleepy.h */

#ifndef SLEEPY_H_1727_INCLUDED
#define SLEEPY_H_1727_INCLUDED

/* Number of devices to create (default: sleepy0 and sleepy1) */
#ifndef SLEEPY_NDEVICES
#define SLEEPY_NDEVICES 10
#endif

/* The structure to represent 'sleepy' devices. 
 *  data - data buffer;
 *  buffer_size - size of the data buffer;
 *  block_size - maximum number of bytes that can be read or written 
 *    in one call;
 *  sleepy_mutex - a mutex to protect the fields of this structure;
 *  cdev - �haracter device structure.
 */
struct sleepy_dev {
  unsigned char *data;
  struct mutex sleepy_mutex; 
  struct cdev cdev; // Find the structure. basic bookkeeping for character device drivers
  wait_queue_head_t sleepy_queue; // wait queue
};
#endif /* SLEEPY_H_1727_INCLUDED */
