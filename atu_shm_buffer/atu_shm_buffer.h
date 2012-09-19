/*
 * File: atu_shm_buffer.h
 * Description: A shared memory buffer. Read/write a shared memory just like a normal buffer.
 * Use a semaphore with the same name to synchronize the shared memory buffer in multi-process.
 * Created on: 2012-9-18
 * Author: PArt
 * Any questions, please mail me :) atu8292991@gmail.com
 */

#ifndef ATU_SHM_BUFFER_H_
#define ATU_SHM_BUFFER_H_
#include <string>
#include <fcntl.h>
#include <semaphore.h>

namespace atu_libs {

namespace atu_shm_buffer {

const u_int c_default_buffer_len = 256;

class atu_shm_buffer {
public:
	/*
	 *  Constructor of atu_shm_buffer.
	 *  @name:     the name of the shared memory.
	 *  @len:      if first created, specify the length of the shared memroy.
	 *  @create:   true create a new shared memory, false link to an exist shared memory.
	 */
	atu_shm_buffer(const char *name, size_t len = c_default_buffer_len, bool create = false);
	virtual ~atu_shm_buffer();

	/*
	 *  Get the name of the shared memory.
	 *  Return value: the name of the shared memory.
	 */
	inline const char * get_name(void) { return m_name.c_str(); }
	/*
	 *  Get the length of the shared memory.
	 *  Return value: the length of the shared memory.
	 */
	inline size_t get_len(void) { return m_len; }

	/*
	 *  Read from shared memory. Use a semaphore with the same name to synchronize.
	 *  @buf:    the point to a buffer, the buffer must be allocated.
	 *  @len:    the length of bytes read from shared memory.
	 */
	void read(char *buf, size_t len);
	/*
	 *  Write bytes to shared memroy. Use a semaphore with the same name to synchronize.
	 *  @content:  the point to a buffer with contents, the buffer must be allocated.
	 *  @len:      the length of bytes write to shared memory.
	 */
	void write(const void *content, size_t len);
	/*
	 *  Unlink the shared memory and semaphore from linux kernel.
	 */
	void unlink(void);
private:
	void *m_buf;
	size_t m_len;
	int m_fd;
	std::string m_name;

	sem_t *m_sem;
	struct stat m_stat;
};
}
}

#endif /* ATU_SHM_BUFFER_H_ */
